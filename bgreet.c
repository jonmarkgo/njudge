/*
 * $Log$
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "dip.h"
#include "functions.h"

/*
 *  Bgreet: Send out birthday greetings to our clientele.
 */

#define nentry(x) (sizeof(x)/sizeof(x[0]))

struct {
	int flags;
	int month;
	int day;
	char name[256];
	char addr[256];
} users[MAXUSER];

#define F_NAME	0x01
#define F_DATE	0x02
#define F_ACTV  0x04

#define M_GOOD  0x03
#define F_GOOD	0x03

int eflg = 0;
char *dflg = NULL;

#define eprintf if (eflg) fprintf


struct {
	char *name;
	int number;
} months[] = {

	{
		"Jan", 1
	}, {
		"Feb", 2
	}, {
		"Mar", 3
	}, {
		"Apr", 4
	}, {
		"May", 5
	}, {
		"Jun", 6
	},
	{
		"Jul", 7
	}, {
		"Aug", 8
	}, {
		"Sep", 9
	}, {
		"Oct", 10
	}, {
		"Nov", 11
	}, {
		"Dec", 12
	},

	{
		"January", 1
	}, {
		"February", 2
	}, {
		"March", 3
	}, {
		"April", 4
	},
	{
		"June", 6
	}, {
		"July", 7
	}, {
		"August", 8
	}, {
		"September", 9
	},
	{
		"October", 10
	}, {
		"November", 11
	}, {
		"December", 12
	}, {
		"Sept", 9
	}

};

int main(int argc, char **argv)
{

	int u, i, d0, d1, d2, sh, sm, sd, em, ed;
	char *s, s0[256], s1[256];
	char line[256], host[20], myname[20];
	FILE *fp, *fp1;
	long now;
	struct tm *tm;
	struct utsname unameinfo;

	if (!(s = (char *) getenv("USER"))) {
		s = "judge";
	}
	strcpy(myname, s);
	if (islower(*myname))
		*myname = toupper(*myname);

	/* TODO just make this read the config file instead */
	if (uname(&unameinfo) == -1) {
		strncpy(host, "unknown.host", sizeof(host));
	} else {
		strncpy(host, unameinfo.nodename, sizeof(host));
	}

	/*
	 *  Parse any argument list we might have.
	 */

	i = 1;
	while (i < argc) {
		if (argv[i][0] == '-') {
			for (s = argv[i++] + 1; *s; s++) {
				switch (*s) {

				case 'd':
					dflg = i < argc ? argv[i++] : "0";
					break;

				case 'e':
					eflg++;
					break;

				default:
					goto usage;
				}
			}
		} else {
		      usage:
			fprintf(stderr, "Usage: bgreet [-e] [-d days[.seconds]]\n");
			fprintf(stderr, "  e: Show unparsable birthdays.\n");
			fprintf(stderr, "  d: Show upcoming birthdays as if N days from now.\n");
			exit(1);
		}
	}

	/*
	 *  Scan through the whois file looking for names and birthdays.
	 */

	if (!(fp = fopen("dip.whois", "r"))) {
		perror("whois");
		exit(1);
	}
	while (fgets(line, sizeof(line), fp)) {
		if (!strncmp(line, "User:", 5)) {
			u = 0;
			sscanf(line, "User: %d", &u);
			if (u > MAXUSER - 1 || u < 1) {
				fprintf(stderr, "Bad user number: %s", line);
				u = 0;
			}
		} else if (!strncmp(line, "Name:", 5)) {
			i = sscanf(line, "Name: %s %s %s", users[u].name, s0, s1);
			if (i == 3 && ((users[u].name[1] == '.' && users[u].name[2] == '\0') ||
				       (users[u].name[1] == '\0'))) {
				strcpy(users[u].name, s0);
			}
			if (i > 1) {
				users[u].flags |= F_NAME;
			}
		} else if (!strncmp(line, "Birth", 5)) {

			while ((s = strchr(line, '.')))
				*s = ' ';
			while ((s = strchr(line, '-')))
				*s = ' ';
			while ((s = strchr(line, ',')))
				*s = ' ';
			while ((s = strchr(line, '/')))
				*s = ' ';
			strcpy(s1, "th");

			if (!(s = strchr(line, ':'))) {
				continue;
			}
			if (sscanf(s, ": %d %d %d\n", &d0, &d1, &d2) == 3) {
				if (0 < d1 && d1 < 13 && 12 < d0 && d0 < 32) {
					i = d0;
					d0 = d1;
					d1 = i;
				} else if (d0 != d1 && !(0 < d0 && d0 < 13 && 12 < d1 && d1 < 32)) {
					fprintf(stderr, "Can't parse #2: %d %s", u, line);
					continue;
				}
			} else if (sscanf(s, ": %s %d %d\n", s0, &d1, &d2) == 3 ||
				   sscanf(s, ": %d %s %d\n", &d1, s0, &d2) == 3 ||
				   sscanf(s, ": %s %d%s %d\n", s0, &d1, s1, &d2) == 4 ||
				   sscanf(s, ": %d%s %s %d\n", &d1, s1, s0, &d2) == 4) {

				if (strcmp(s1, "th") && strcmp(s1, "rd") &&
				  strcmp(s1, "nd") && strcmp(s1, "st")) {
					fprintf(stderr, "Can't parse #0: %d %s", u, line);
					continue;
				}
				d0 = 0;
				for (i = 0; i < nentry(months); i++) {
					if (!strcasecmp(months[i].name, s0)) {
						d0 = months[i].number;
						break;
					}
				}
				if (!d0 || 0 > d1 || d1 > 31) {
					fprintf(stderr, "Can't parse #1: %d %s", u, line);
					continue;
				}
			} else {
				eprintf(stdout, "Can't parse #3: %d %s", u, line);
				continue;
			}


			if ((d2 < 1930 || d2 > 1990) && (d2 < 30 || d2 > 80)) {
				eprintf(stdout, "Can't parse #4: %d %s", u, line);
				continue;
			}
			users[u].month = d0;
			users[u].day = d1;
			users[u].flags |= F_DATE;

		}
	}

	fclose(fp);

	/*
	 *  Now figure out which of these people are active and what their 
	 *  addresses are by scanning the master file.
	 */

	if (!(fp = fopen("dip.master", "r"))) {
		perror("master");
		exit(1);
	}
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "%s %d %d %d %d %d %s %s\n", s0, &i, &i, &i, &u, &i, s0, s1) == 8) {
			if (0 < u && u < MAXUSER && *s1 != '*') {
				users[u].flags |= F_ACTV;
				strcpy(users[u].addr, s1);
			}
		}
	}

	fclose(fp);

	/*
	 *  Now figure out which of these people have a birthdays this week.
	 *  We want the greeting to make it to Europe and the west coast of
	 *  the US on the proper day so we send it at 8am GMT.  People in
	 *  Hawaii will get their message a little early and Japan a little
	 *  late (it would be too much trouble to try to interpret the timezone
	 *  in the whois file).  
	 *
	 *  We assume that we send the greeting out in the morning local time
	 *  and that it's the afternoon now.
	 */

	time(&now);
	if (dflg) {
		now += 24 * 60 * 60 * atoi(dflg);
		if ((s = strchr(dflg, '.')))
			now += atoi(s + 1);
		printf("Assuming it is now %s", ctime(&now));
	}
	tm = gmtime(&now);
	i = 8 - tm->tm_hour;
	if (i < 0)
		i += 24;
	now += i * 60 * 60;
	tm = localtime(&now);
	sm = tm->tm_mon + 1;
	sd = tm->tm_mday;
	sh = tm->tm_hour;
	now += (7 - 1) * 24 * 60 * 60;
	tm = localtime(&now);
	em = tm->tm_mon + 1;
	ed = tm->tm_mday;

	fprintf(stdout, "Looking for birthdays: %d/%d - %d/%d.  Execute at %2.2d00.\n",
		sm, sd, em, ed, sh);

	if (sm == em) {
		d0 = sd;
		d1 = ed;
	} else {
		d0 = 0;
		d1 = 32;
	}

	for (u = 1; u < MAXUSER; u++) {
		if ((users[u].flags & (M_GOOD | F_ACTV)) == (F_GOOD | F_ACTV)) {
			if ((users[u].month == sm && sd <= users[u].day && users[u].day <= d1) ||
			    (users[u].month == em && d0 <= users[u].day && users[u].day <= ed)) {
				fprintf(stdout, "%d\t%s %2d %4x %-16.16s %s\n",
					u, months[users[u].month - 1].name, users[u].day, users[u].flags,
					users[u].name, users[u].addr);
#ifdef LINUX
				sprintf(line, "at %2.2d00 %s %d", sh,
					months[users[u].month - 1].name, users[u].day);
#else
				sprintf(line, "at -s %2.2d00 %s %d", sh,
					months[users[u].month - 1].name, users[u].day);
#endif
				if (!dflg) {
					if (!(fp = popen(line, "w"))) {
						perror(line);
						exit(1);
					}
					fprintf(fp, "mail -s 'Greetings' %s << EOF\n", users[u].addr);
					fprintf(fp, "Greetings, %s;\n\n", users[u].name);
					if (!(fp1 = fopen("greeting", "r"))) {
						fprintf(fp,
							"The Diplomacy Adjudicator and its administrator here on %s wish you a\n",
							host);
						fprintf(fp,
							"very happy birthday and the best of luck throughout the coming year!\n");
					} else {
						while (fgets(line, sizeof(line), fp1)) {
							fputs(line, fp);
						}
						fclose(fp1);
					}
					fprintf(fp, "\nDiplomatically yours, %s.\n", myname);
					fprintf(fp, "EOF\n");

					pclose(fp);
				}
			}
		}
	}
	return 0;
}
