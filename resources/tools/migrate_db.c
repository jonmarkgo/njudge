/*
 * dump_to_sql.c
 *
 *  Created on: Oct 19, 2012
 *      Author: cjw
 */

#include <glib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <getopt.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../../user.h"
#include "../../player.h"
#include "../../datetime.h"

#define PLYRDATA_MAGIC          0x504C5952
#define PLYRDATA_VERSION        2

#define LINE_BUFLEN				512

#define APP_ERROR				app_error_quark()

typedef enum {

	APP_ERROR_FILE_OPEN = 1,
	APP_ERROR_MEMORY_ALLOCATION,
	APP_ERROR_UNKNOWN_OUT_FORMAT

} AppError;
typedef enum {

	OUTFORMAT_MYSQL = 0,
	OUTFORMAT_NEWDIP

} OutFormat;

typedef struct plyrheader_s {

    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t record_size;

} plyrheader_t;
typedef struct ded_s {

	short  rating;
	short  i1_na;
	short  i2_na;
	time_t last_signon;
	time_t md_na;
	long   d0_na;

} ded_t;
/*typedef struct user_s {

	int    id;
	char*  name;
	int	   level;
	time_t birthdate;
	int    sex;
	char*  phone;
	char*  site;
	char*  address;
	char*  country;
	char*  timezone;
	GSList* mail;
	GSList* extra_fields;

} user_t;*/
typedef struct mailrec_s {

	guint id;
	gchar email[];

} mailrec_t;
typedef struct plyrec_s {

    int ontime;
    int total;
    int started;
    int resigned;
    int tookover;
    int reserved;

} plyrec_t;
struct opts_s {

	int   clean_recs;
	int   ignore_ded_integ;
	int   verbose;
	int   out_format;
	char* addr_fn;
	char* ded_fn;
	char* plyr_fn;
	char* whois_fn;

};
typedef struct addr_s {

	uint32_t id;
	uint32_t def;
	GPtrArray* addrs;

} addr_t;

GQuark app_error_quark(void) {

	return g_quark_from_static_string("app-error-quark");

}

void    append_user_addresses(char* addr_data, user_t* user_rec);
int     cmp_mail_record(void* rec_a, void* rec_b);
int     cmp_mail_address(void* rec_a, void* rec_b);
int     sort_user_by_id(void* rec_a, void* rec_b);
char*   escape_semicolon(char* str);
void    free_user_record(user_t* user_rec);
GSList* get_all_user_records(GSList** addr_lst, GError** err);
GSList* get_mail_addresses(GError** err);
int     init(int argc, char** argv, GError** err);
user_t* next_user_record(FILE* whois_fp);
void    print_usage(void);
int     print_sql(GSList* usr_lst, ded_t* deds, plyrec_t* stat, GError** err);
int     read_ded_data(ded_t** deds);
int     read_plyr_data(plyrec_t** plyr_rec);
gchar*  stpcpy(gchar* dst, gchar* src);
int     write_newdb(GSList* usr_lst, ded_t* deds, plyrec_t* stats, GError** err);

struct opts_s opts;

int main(int argc, char** argv) {

	int rtn = 1; /* assume failure */
	gint      ded_recs;
	gint      stat_recs;
	gchar*    addr_data = NULL;
	GSList*   itr       = NULL;
	GSList*   usr_lst   = NULL;
	GSList*   addr_lst  = NULL;
	ded_t*    deds      = NULL;
	GError*   err       = NULL;
	plyrec_t* stats     = NULL;

	if (!init(argc, argv, &err)) {
		if (err) fprintf(stderr, "%s\n", err->message);
		return 1;
	}

	/* read dedication data */
	if (opts.verbose)
		fprintf(stderr, "Reading dedication data from %s...\n", opts.ded_fn);
	if (!(ded_recs = read_ded_data(&deds))) goto exit_main;
	/* read player stats data */
	if (opts.verbose)
		fprintf(stderr, "Reading plyr data from %s...\n", opts.plyr_fn);
	if (!(stat_recs = read_plyr_data(&stats))) goto exit_main;
	/* read mail addresses */
	if (opts.verbose)
		fprintf(stderr, "Reading mail addresses from %s...\n", opts.addr_fn);
	if ((addr_lst = get_mail_addresses(&err)) == NULL) {
		fprintf(stderr, "%s\n", err->message);
		goto exit_main;
	}
	/* read user records */
	if (opts.verbose)
		fprintf(stderr, "Reading user records from %s...\n", opts.whois_fn);
	if (!(usr_lst = get_all_user_records(&addr_lst, &err))) {
		fprintf(stderr, "%s\n", err->message);
		goto exit_main;
	}
	if ((ded_recs -= ((user_t*) g_slist_last(usr_lst)->data)->id) < 0) {
		fprintf(stderr, "%s ", opts.ignore_ded_integ ? "WARNING!" : "ERROR!");
		fprintf(stderr, "Data inconsistency error.\n"
				"Dedication database contains %d records against max user id.\n", ded_recs);
		if (!opts.ignore_ded_integ) goto exit_main;
	}
	if ((stat_recs -= ((user_t*) g_slist_last(usr_lst)->data)->id) < 0) {
		fprintf(stderr, "%s ", opts.ignore_ded_integ ? "WARNING!" : "ERROR!");
		fprintf(stderr, "Data inconsistency error.\n"
				"Plyrdata database contains %d records against max user id.\n", stat_recs);
		if (!opts.ignore_ded_integ) goto exit_main;
	}
	if (opts.verbose) {
		fprintf(stderr, "Read %u users from %s\n", g_slist_length(usr_lst), opts.whois_fn);
		fprintf(stderr, "Skipping %u unused mail addresses from %s\n",
				g_slist_length(addr_lst), opts.addr_fn);
	}

	/* free remaining mail address list */
	for (itr = addr_lst; itr; itr = itr->next)
		g_free(itr->data);
	g_slist_free(addr_lst);

	if (opts.out_format == OUTFORMAT_MYSQL) {
		if (!print_sql(usr_lst, deds, stats, &err)) {
			goto exit_main;
		}
	} else if (opts.out_format == OUTFORMAT_NEWDIP) {
		if (!write_newdb(usr_lst, deds, stats, &err)) {
			goto exit_main;
		}
	}

	rtn = 0; /* success */

exit_main:

	if (addr_lst)  g_slist_free(addr_lst);
	if (err)       g_error_free(err);
	if (deds)      free(deds);
	if (stats)     free(stats);
	if (addr_data) g_free(addr_data);

	return rtn;

}
int cmp_mail_record(void* rec_a, void* rec_b) {

	return ((mailrec_t*) rec_a)->id - ((mailrec_t*) rec_b)->id;

}
int cmp_mail_address(void* rec_a, void* rec_b) {

	return strcasecmp(((mailrec_t*) rec_a)->email, ((mailrec_t*) rec_b)->email);

}
int sort_user_by_id(void* rec_a, void* rec_b) {

	int ret;

	if (rec_a == NULL) ret = -1;
	else
		ret = ((user_t*) rec_a)->id - ((user_t*) rec_b)->id;

	return ret;

}
char* escape_semicolon(char* str) {

	guint  occ = 0;
	gchar* ptr;
	gchar* ret;
	gchar* ret_ptr;

	if (!str)
		return g_strdup("");

	for (ptr = str; (*ptr); ptr ++) {
		if (*ptr == ';')
			occ ++;
	}

	ret = ret_ptr = g_malloc0(strlen(str) + occ + 1);

	for (ptr = str; *ptr; ptr ++, ret_ptr ++) {
		if (*ptr == ';')
			*(ret_ptr ++) = '\\';
		*ret_ptr = *ptr;
	}

	return ret;

}
void free_user_record(user_t* user_rec) {

	if (user_rec->name)     g_free(user_rec->name);
	if (user_rec->phone)    g_free(user_rec->phone);
	if (user_rec->site)     g_free(user_rec->site);
	if (user_rec->address)  g_free(user_rec->address);
	if (user_rec->country)  g_free(user_rec->country);
	if (user_rec->timezone) g_free(user_rec->timezone);

	g_slist_foreach(user_rec->mail, (GFunc) g_free, NULL);
	g_slist_foreach(user_rec->extra_fields, (GFunc) g_free, NULL);
	g_slist_free(user_rec->mail);
	g_slist_free(user_rec->extra_fields);

	g_free(user_rec);

}
GSList* get_all_user_records(GSList** addr_lst, GError** err) {

	FILE*   whois_fp = NULL;
	GSList* usr_lst = NULL;
	GSList* mail_rec;
	user_t* usr_rec;
	mailrec_t search;

	if ((whois_fp = fopen(opts.whois_fn, "r")) == NULL) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening %s: %s", opts.whois_fn, strerror(errno));
		goto exit_get_all_user_records;
	}

	while ((usr_rec = next_user_record(whois_fp)) != NULL) {
		usr_lst = g_slist_insert_sorted(usr_lst, usr_rec, (GCompareFunc) sort_user_by_id);
		search.id = usr_rec->id;
		while ((mail_rec = g_slist_find_custom(*addr_lst, &search, (GCompareFunc) sort_user_by_id))) {
			*addr_lst = g_slist_remove_link(*addr_lst, mail_rec);
			user_add_mail(usr_rec, ((mailrec_t*) mail_rec->data)->email, err);
			g_free(mail_rec->data);
			g_slist_free_1(mail_rec);
		}
	}

exit_get_all_user_records:

	if (whois_fp) fclose(whois_fp);

	return usr_lst;

}
GSList* get_mail_addresses(GError** err) {

	gchar*     cptr;
	gchar      line[LINE_BUFLEN];
	FILE*      addr_fp = NULL;
	GSList*    addr_lst = NULL;
	mailrec_t* mail_rec;

	if ((addr_fp = fopen(opts.addr_fn, "r")) == NULL) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening %s: %s", opts.addr_fn, strerror(errno));
		goto exit_get_mail_addresses;
	}

	while (fgets(line, LINE_BUFLEN, addr_fp)) {
		if (!(cptr = strchr(line, '=')) || !*g_strstrip(++cptr)) continue;
		if (addr_lst && !strcmp(cptr, ((mailrec_t*) addr_lst->data)->email))
			continue;
		mail_rec = (mailrec_t*) g_malloc(sizeof(guint) + strlen(cptr) + 1);
		mail_rec->id = atoi(line + 1);
		strcpy(mail_rec->email, cptr);
		addr_lst = g_slist_prepend(addr_lst, mail_rec);
	}

	addr_lst = g_slist_reverse(addr_lst);

exit_get_mail_addresses:

	if (addr_fp) fclose(addr_fp);

	return addr_lst;

}
int init(int argc, char** argv, GError** err) {

	int opt;
	int rtn = 0;

	memset(&opts, 0, sizeof opts);

	opts.addr_fn  = "dip.addr";
	opts.ded_fn   = "dip.ded";
	opts.plyr_fn  = "plyrdata";
	opts.whois_fn = "dip.whois";

	while ((opt = getopt(argc, argv, "a:Cd:f:hip:vw:")) > -1) {
		switch(opt) {
			case 'a':
				opts.addr_fn = optarg;
				break;
			case 'C':
				opts.clean_recs = 1;
				break;
			case 'd':
				opts.ded_fn = optarg;
				break;
			case 'f':
				if (!strcasecmp("mysql", optarg)) {
					opts.out_format = OUTFORMAT_MYSQL;
				} else if(!strcasecmp("newdip", optarg)) {
					opts.out_format = OUTFORMAT_NEWDIP;
				} else {
					g_set_error(err, APP_ERROR, APP_ERROR_UNKNOWN_OUT_FORMAT,
							"unknown output format - %s", optarg);
					goto exit_init;
				}
				break;
			case 'h':
				print_usage();
				goto exit_init;
				break;
			case 'i':
				opts.ignore_ded_integ = 1;
				break;
			case 'p':
				opts.plyr_fn = optarg;
				break;
			case 'v':
				opts.verbose = 1;
				break;
			case 'w':
				opts.whois_fn = optarg;
				break;
			default:
				goto exit_init;
		}
	}

	rtn = 1;

exit_init:

	return rtn;

}
user_t* next_user_record(FILE* whois_fp) {

	char* tmp;
	char* key = NULL;
	char* val = NULL;
	char  line[LINE_BUFLEN];
	long  offset;
	user_t*     user_rec  = NULL;
	GError*     gerr      = NULL;
	GMatchInfo* rex_match = NULL;
	GRegex*     rex       = NULL;

	rex = g_regex_new("\\s*(.+?):\\s*(.+?)$", 0, 0, NULL);

	while (1) {
		/* rewind to offset when/if a new user record is found */
		offset = ftell(whois_fp);
		if (!fgets(line, LINE_BUFLEN, whois_fp)) {
			if (!feof(whois_fp)) {
				fprintf(stderr, "read error in %s\n", opts.whois_fn);
				goto exit_read_user_record;
			}
			break;
		}
		if (!g_regex_match(rex, line, 0, &rex_match)) {
			g_match_info_free(rex_match);
			continue;
		}
		key = g_match_info_fetch(rex_match, 1);
		val = g_match_info_fetch(rex_match, 2);

		if (!strcasecmp(key, "user")) {
			if (user_rec != NULL) {
				fseek(whois_fp, offset, SEEK_SET);
				g_free(key); g_free(val);
				break;
			}
			user_rec = g_new0(user_t, 1);
			user_rec->id = atoi(val);
		} else if (!strcasecmp(key, "name")) {
			user_rec->name = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "phone")) {
			user_rec->phone = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "site")) {
			user_rec->site = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "address")) {
			user_rec->address = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "country")) {
			user_rec->country = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "timezone")) {
			user_rec->timezone = g_strescape(val, NULL);
		} else if (!strcasecmp(key, "email")) {
			if (!user_add_mail(user_rec, val, &gerr)) {
				fprintf(stderr, "bad e-mail address for user id %u: %s\n  -> %s\n",
						user_rec->id, gerr->message, val);
				g_clear_error(&gerr);
			}
		} else if (!strcasecmp(key, "level")) {
			if (!strcasecmp(val, "novice"))
				user_rec->level = 2;
			else if (!strcasecmp(val, "advanced"))
				user_rec->level = 3;
			else if (!strcasecmp(val, "intermediate"))
				user_rec->level = 4;
			else if (!strcasecmp(val, "expert"))
				user_rec->level = 6;
			else user_rec->level = 5; /* 5 = amateur */
		} else if (!strcasecmp(key, "birthdate")) {
			user_rec->birthdate = datetime_epoc_from_string(val);
		} else if (!strcasecmp(key, "sex")) {
			if (!strcasecmp(val, "male"))
				user_rec->sex = -1;
			else if (!strcasecmp(val, "female"))
				user_rec->sex = 1;
			else
				user_rec->sex = 0;
		} else {
			if (opts.clean_recs) {
				if (!strcasecmp(key, "package") || !strcasecmp(key, "ipaddress"))
					goto skip_field;
			}
			tmp = g_malloc(strlen(key) + 1 + strlen(val) + 1);
			strcpy(stpcpy(tmp, key) + 1, val);
			user_rec->extra_fields = g_slist_append(user_rec->extra_fields, tmp);
		}
		skip_field:
		g_free(key); key = NULL;
		g_free(val); val = NULL;
		g_match_info_free(rex_match); rex_match = NULL;
	}

exit_read_user_record:

	if (rex)       g_regex_unref(rex);
	if (rex_match) g_match_info_free(rex_match);

	return user_rec;

}
void print_usage(void) {

	fprintf(stderr, "usage: migrate_db [options]\n\n"
			"  -a filename\t\tAddr file\n"
			"  -C\t\t\tDon't write empty/unused records\n"
			"  -d filename\t\tDed file\n"
			"  -f format\t\tOutput format (mysql, newdip)\n"
			"  -h\t\t\tPrint this help and exit\n"
			"  -i\t\t\tIgnore inconsistency errors\n"
			"  -v\t\t\tVerbose\n"
			"  -w filename\t\tWhois file\n"
			"\n");

}
int print_sql(GSList* usr_lst, ded_t* deds, plyrec_t* stats, GError** err) {

	gint    ret = 0;
	gchar*  line;
	FILE*   tmp_addr = NULL;
	FILE*   tmp_stat = NULL;
	FILE*   tmp_flds = NULL;
	user_t* usr_rec;
	GSList* itr;
	GSList* itr_b;

	if (((tmp_addr = tmpfile()) == NULL) || ((tmp_stat = tmpfile()) == NULL) ||
			((tmp_flds = tmpfile()) == NULL)) {
		fprintf(stderr, "error opening temporary files: %s\n", strerror(errno));
		goto exit_print_sql;
	}

	printf("--\n-- User records --\n--\n");
	fprintf(tmp_stat, "--\n-- User stats --\n--\n");
	fprintf(tmp_addr, "--\n-- E-mails --\n--\n");
	fprintf(tmp_flds, "--\n-- Extra fields --\n--\n");

	for (itr = usr_lst; itr; itr = itr->next) {
		usr_rec = itr->data;
		printf("INSERT INTO users (user_id, name, birthdate, sex, level, phone, "
				 "address, country, site, timezone, tz)\n\t");
		printf("VALUES (%u, \"%s\", %d, %d, %u, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %u);\n",
				usr_rec->id, usr_rec->name, (int32_t) usr_rec->birthdate,
				usr_rec->sex, usr_rec->level, usr_rec->phone, usr_rec->address,
				usr_rec->country, usr_rec->site, usr_rec->timezone, 0);
		fprintf(tmp_stat, "INSERT INTO user_stats (user_id, rating, last_signon, total, "
				"ontime, started, resigned, tookover)\n\t");
		fprintf(tmp_stat, "VALUES (%u, %d, %u, %u, %u, %u, %u, %u);\n",
				usr_rec->id, (int) deds[usr_rec->id].rating, (guint) deds[usr_rec->id].last_signon,
				stats[usr_rec->id].total, stats[usr_rec->id].ontime, stats[usr_rec->id].started,
				stats[usr_rec->id].resigned, stats[usr_rec->id].tookover);
		for (itr_b = usr_rec->mail; itr_b; itr_b = itr_b->next) {
			fprintf(tmp_addr, "INSERT INTO email (user_id, address, is_default)\n\t");
			fprintf(tmp_addr, "VALUES (%u, \"%s\", %u);\n", usr_rec->id,
					(char*) itr_b->data, (itr_b == usr_rec->mail));
		}
		for (itr_b = usr_rec->extra_fields; itr_b; itr_b = itr_b->next) {
			fprintf(tmp_flds, "INSERT INTO extra_fields (user_id, field_name, field_value)\n\t");
			fprintf(tmp_flds, "VALUES (%u, \"%s\", \"%s\");\n", usr_rec->id, (gchar*) itr_b->data,
					(gchar*) itr_b->data + strlen(itr_b->data) + 1);
		}
		user_free(usr_rec);
	}

	fseek(tmp_addr, 0, SEEK_SET);
	fseek(tmp_stat, 0, SEEK_SET);
	fseek(tmp_flds, 0, SEEK_SET);

	line = g_malloc(1024);

	while (fgets(line, 1024, tmp_addr))
		printf("%s", line);

	while (fgets(line, 1024, tmp_stat))
		printf("%s", line);

	while (fgets(line, 1024, tmp_flds))
		printf("%s", line);

	g_free(line);

	ret = 1;

exit_print_sql:

	return ret;

}
int read_ded_data(ded_t** deds) {

	int    rtn = 0;
	int    ded_fd;
	struct stat ded_stat;

	if ((ded_fd = open(opts.ded_fn, O_RDONLY)) < 0) {
		fprintf(stderr, "error opening %s: %s\n", opts.ded_fn, strerror(errno));
		goto exit_read_stat_data;
	}

	fstat(ded_fd, &ded_stat);

	if ((*deds = malloc(ded_stat.st_size)) == NULL) {
		fprintf(stderr, "memory allocation error @ %s\n", __func__);
		goto exit_read_stat_data;
	}

	read(ded_fd, *deds, ded_stat.st_size);

	close(ded_fd);

	rtn = ded_stat.st_size / sizeof(ded_t);

exit_read_stat_data:

	return rtn;

}
int read_plyr_data(plyrec_t** plyr_rec) {

	int ret = 0;
	int plyr_fd = -1;
	plyrheader_t header = {0};
	struct stat file_stat = {0};

	if ((plyr_fd = open(opts.plyr_fn, O_RDONLY)) < 0) {
		fprintf(stderr, "error opening %s: %s\n", opts.plyr_fn, strerror(errno));
		goto exit_read_plyr_data;
	}
	if (fstat(plyr_fd, &file_stat)) {
		fprintf(stderr, "error reading %s: %s\n", opts.plyr_fn, strerror(errno));
		goto exit_read_plyr_data;
	}

	read(plyr_fd, &header, sizeof header);
	if ((header.magic != PLYRDATA_MAGIC) || header.version != 2) {
		fprintf(stderr, "plyrdata data error: this data file is not supported\n");
		goto exit_read_plyr_data;
	}

	if ((*plyr_rec = malloc(file_stat.st_size - sizeof(header))) == NULL) {
		fprintf(stderr, "memory allocation error @ %s\n", __func__);
		goto exit_read_plyr_data;
	}

	read(plyr_fd, *plyr_rec, file_stat.st_size - sizeof(header));

	ret = (file_stat.st_size - sizeof(plyrheader_t)) / sizeof(plyrec_t);

exit_read_plyr_data:

	if (plyr_fd > 0) close(plyr_fd);

	return ret;

}
gchar* stpcpy(gchar* dst, gchar* src) {

	g_assert((dst != NULL) && (src != NULL));

	do {
		*(dst++) = *(src++);
	} while (*src != '\0');

	*dst = '\0';

	return dst;
}
int write_newdb(GSList* usr_lst, ded_t* deds, plyrec_t* stats, GError** err) {

	gint  ret = 0;
	char* temp_str[6];
	FILE* stat_file  = NULL;
	FILE* mail_file  = NULL;
	FILE* field_file = NULL;
	FILE* whois_file = NULL;
	GSList* sl_cur;
	GSList* user_cur;
	user_t* user_rec;
	plrstat_t* stat_recs = NULL;

	if (!(whois_file = fopen("dipwhois.tbl", "w"))) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening dipwhois.tbl - %s", g_strerror(errno));
		goto exit_write_newdb;
	}
	if (!(stat_file = fopen("dipstats.tbl", "w"))) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening dipstats.tbl - %s", g_strerror(errno));
		goto exit_write_newdb;
	}
	if (!(mail_file = fopen("dipmail.tbl", "w"))) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening dipmail.tbl - %s", g_strerror(errno));
		goto exit_write_newdb;
	}
	if (!(field_file = fopen("dipfields.tbl", "w"))) {
		g_set_error(err, APP_ERROR, APP_ERROR_FILE_OPEN,
				"error opening dipfields.tbl - %s", g_strerror(errno));
		goto exit_write_newdb;
	}

	user_cur = usr_lst;
	stat_recs = g_malloc_n(g_slist_length(usr_lst), sizeof(plrstat_t));

	do {
		user_rec = user_cur->data;
		fprintf(whois_file, "%u;%s;%u;%d;%d;%s;%s;%s;%s;%s\n",
				user_rec->id,
				(temp_str[0] = escape_semicolon(user_rec->name)),
				user_rec->level,
				(gint) user_rec->birthdate,
				user_rec->sex,
				(temp_str[1] = escape_semicolon(user_rec->phone)),
				(temp_str[2] = escape_semicolon(user_rec->site)),
				(temp_str[3] = escape_semicolon(user_rec->address)),
				(temp_str[4] = escape_semicolon(user_rec->country)),
				(temp_str[5] = escape_semicolon(user_rec->timezone)));

		g_free(temp_str[0]);
		g_free(temp_str[1]);
		g_free(temp_str[2]);
		g_free(temp_str[3]);
		g_free(temp_str[4]);
		g_free(temp_str[5]);

		fprintf(stat_file, "%u;%d;%d;%u;%u;%u;%u;%u\n",
				user_rec->id,
				(gint) deds[user_rec->id].rating,
				(guint) deds[user_rec->id].last_signon,
				stats[user_rec->id].total,
				stats[user_rec->id].ontime,
				stats[user_rec->id].started,
				stats[user_rec->id].resigned,
				stats[user_rec->id].tookover);

		for (sl_cur = ((user_t*) user_cur->data)->mail;
				sl_cur;
				sl_cur = g_slist_next(sl_cur)) {
			fprintf(mail_file, "%u;%s;%d\n",
					user_rec->id,
					(char*) sl_cur->data,
					(sl_cur == ((user_t*) user_cur->data)->mail));
		}
		for (sl_cur = ((user_t*) user_cur->data)->extra_fields;
				sl_cur;
				sl_cur = g_slist_next(sl_cur)) {
			fprintf(field_file, "%u;%s;%s\n", user_rec->id,
					(temp_str[0] = escape_semicolon((char*) sl_cur->data)),
					(temp_str[1] = escape_semicolon(
							(char*) sl_cur->data + strlen(sl_cur->data) + 1)));

			g_free(temp_str[0]);
			g_free(temp_str[1]);
		}

	} while ((user_cur = g_slist_next(user_cur)));

	ret = 1; /* Success */

exit_write_newdb:

	if (whois_file)  fclose(whois_file);
	if (stat_file)   fclose(stat_file);
	if (mail_file)   fclose(mail_file);
	if (field_file) fclose(field_file);
	if (stat_recs)   g_free(stat_recs);

	return ret;
}
