/*
 * $Log$
 * Revision 1.10  2003/01/13 22:38:51  millis
 * merged in from ustv
 *
 * Revision 1.9  2002/12/22 02:00:34  millis
 * Added DIE_STORMS setting for new random seed for storms
 *
 * Revision 1.8  2002/08/27 22:27:45  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.7  2002/04/15 12:55:41  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.6  2001/07/17 15:16:48  greg
 * added GAME_DIR
 *
 * Revision 1.5  2001/07/08 22:56:29  miller
 * Add TIME_TOLERANCE and WARP_FILE
 *
 * Revision 1.4  2001/07/01 23:19:29  miller
 * New variant guardians
 *
 * Revision 1.3  2001/01/06 18:48:07  davidn
 * Changes to allow game creator be automatically made master
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added new dip.conf variables
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "conf.h"
#include "hashtable.h"

/* this file contains global variables and functions to set them
 * to customize judge behavior at runtime.
 * These are added to a global hash table, and macros are provided
 * in conf.h to pull them out with the appropriate type, they are
 * stored as strings internally
 * this file is public domain by nathan wagner
 */

struct pd_ht conf_table;

static int conf_doline(char *line);

int conf_set(char *var, char *val)
{
	/* if it's the judge code put it into the environment so
	 * smail can pick it up.
	 */
	if (!strcmp("JUDGE_CODE", var)) {
		char myline[1024];
		sprintf(myline, "%s=%s", var, val);
		putenv(myline);
	}
	return pd_ht_stash(&conf_table, var, strdup(val), free);
}

char *config(char *var)
{
	return (char *) pd_ht_fetch(&conf_table, var);
}

/*
 * BN_CUSTODIAN is the Boardman Custodian, who receives game starts
 *              and ends for standard, nongunboat games
 * MN_CUSTODIAN is the Miller Number Custodian, who receives the game
 *              starts and ends for other games
 * Neither of these should be altered, except on test judges.  These
 * addresses may change from time to time.  Please revise if necessary.
 */

int conf_init(void)
{

	pd_ht_init(&conf_table, 0, NULL);

	/* defaults follow */

	/* Path for judge to reside in */
	conf_set("JUDGE_PATH", "/.");

	/* and add default commands */
	conf_set("SUMMARY_CMD", "./summary");
	conf_set("SMAIL_CMD", "./smail");
	conf_set("RUNLISTMAP_CMD", "./runlistmap");
	conf_set("RUNDIPMAP_CMD", "./rundipmap");
	conf_set("DIP_CMD","./dip");
	conf_set("ATRUN_CMD","./atrun");
	conf_set("GAME_DIR","D");

	conf_set("GAMES_MASTER", "judge_request");
	/* games_opener is place to send opening moves for statistics */
	conf_set("GAMES_OPENER", "nobody");

	/* hall_keeper is person who receives summaries for the Hall of Fame */
	conf_set("HALL_KEEPER", "nobody");
	conf_set("BN_CUSTODIAN", "nobody");
	conf_set("MN_CUSTODIAN", "nobody");
	conf_set("EP_CUSTODIAN","nobody");
	conf_set("OURSELVES", "judge");
	conf_set("BITNET_GATEWAY1", "");
	conf_set("BITNET_GATEWAY2", "");
	conf_set("FORWARD", "");
	conf_set("XFORWARD", "");
	conf_set("YFORWARD", "");
	conf_set("KEEPOUT", "");
	conf_set("SAVE_FILE", "saved_mail");
	conf_set("PLAN", "");
	conf_set("BAILOUT_PLAN", "");
	conf_set("BAILOUT_MSG", "bailout.msg");
	conf_set("RGD_GATEWAY", "nobody");
	conf_set("MIN_CONTROL", "43200");
	conf_set("JUDGE_CODE", "XXXX");
	conf_set("LOCK_TIMEOUT", "3600");
	conf_set("D_ONTIME", "3");
	conf_set("D_LATE", "-1");
	conf_set("D_ABANDON", "-49");
	conf_set("D_CD", "-100");
	conf_set("MAP_FILE", "data/map");
	conf_set("MASTER_FILE", "dip.master");
	conf_set("TMASTER_FILE", "dip.tmast");
	conf_set("NO_CREATE", "dip.nocreate");
	conf_set("LOG_FILE", "dip.log");
	conf_set("CUTOFF_LENGTH", "78");	/* Length of longest print strings */
	conf_set("DIE_ASSASSIN", "382204");
	conf_set("DIE_EXPENSE", "148741");
	conf_set("DIE_FAMPLAG", "995816");
	conf_set("DIE_STORMS",  "341291");
	conf_set("DIE_INCOME", "66144");
	conf_set("CREATE_DEDICATION", "-10");
	conf_set("SPECIAL_PW", "default");
	conf_set("SYSLOG_FLAG", "0");
	conf_set("AUTO_MASTER","no");
	conf_set("TIME_TOLERANCE", "60");  /* Time drift before detecting warp */
	conf_set("WARP_FILE", "./dip.warp");

/* Default custodians for sailho (other variants will default to nothing */
	conf_set("CUSTODIAN_sailho", "tarzan_monkeyman@bigfoot.com");
	conf_set("CUSTODIAN_sailho_crowded", "tarzan_monkeyman@bigfoot.com"); 

	conf_set("CUSTODIAN_1900_1", "VonPowell@aol.com");
	conf_set("CUSTODIAN_1900_2", "VonPowell@aol.com");
	conf_set("CUSTODIAN_1900_3", "VonPowell@aol.com");

	conf_set("BLIND", "./zpblind");

	conf_set("NOBODY", "nobody@localhost");

	conf_set("BLOCK_FILE", ""); /* File to block dip running */

	return 1;
}

void conf_cmdline(int argc, char **argv)
{
	int c;

	for (c = 1; c < argc; c++) {
		if ((argv[c][0] == '-') && (argv[c][1] == 'c')) {
			conf_doline(&argv[c][2]);
		}
	}
}

void conf_usage(void)
{
	fprintf(stderr, "There is an error in your command line configuration arguments.\n");
	exit(-2);
}

void conf_readfile(char *directory, char *fname)
{
	char myline[255];
	char filename[255];
	FILE *conffile;
	char *tmp;

	fflush(stdout);
	fflush(stderr);

	sprintf(filename, "%s/%s", directory, fname);

	conffile = fopen(filename, "r");
	if (conffile == NULL) {
		perror("unable to open config file");
		perror(filename);
		exit(-2);
	}
	while (fgets(myline, 255, conffile) != NULL) {
		/* set the last newline to a null */
		if ((tmp = strrchr(myline, '\n'))) {
			*tmp = '\0';
		}
		/* skip leading whitespace */
		tmp = myline;
		while (isspace((unsigned char) *tmp)) {
			tmp++;
		}

		/* blank line ? */
		if ((*tmp == 0) || (*tmp == '#')) {
			continue;
		}
		/* TODO: clean up memory when we are done.  i am ignoring
		 * it for now, as the OS should reclaim the memory when we exit.
		 */

		conf_doline(tmp);
	}

	fclose(conffile);

	return;
}

void conf_print(FILE * where)
{
	struct pd_ht_search s;
	struct pd_ht_entry *e;
	for (e = pd_ht_firstentry(&conf_table, &s); e; e = pd_ht_nextentry(&s)) {

		fprintf(where, "%s = %s\n", (char *) e->key, (char *) e->data);
	}
	return;
}

static int conf_doline(char *tmp)
{
	char conf_var[1024];
	char *varloc;
	size_t namlen;

	namlen = strcspn(tmp, " \t=");
	strncpy(conf_var, tmp, namlen);
	/* wierd, strncpy doesn't null terminate the string */
	*(conf_var + namlen) = '\0';

	varloc = strchr(tmp, '=') + 1;
	/* skip over whitespace at the start of the value */
	while (isspace((unsigned char) *varloc)) {
		varloc++;
	}

	/* put it into the config table */
	conf_set(conf_var, varloc);

	return 1;
}
