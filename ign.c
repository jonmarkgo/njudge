/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXMSG 2000
#define MSGLEN 100
#define MAXALIAS 100

/* TODO eliminate or move global variables */
int done[MAXMSG], size[MAXMSG], pos[MAXMSG];
char addr[MAXMSG][MSGLEN];
char alias1[MAXALIAS][MSGLEN];
char alias2[MAXALIAS][MSGLEN];
int nalias;
char line[255];

int main(int argc, char *argv[])
{

	int i, j, n, p, edit, skip, show, quit, verb, debug;
	char *s, *t;
	FILE *fp1, *fp2;

	debug = 1;

	if ((fp1 = fopen(".ignrc", "r"))) {
		while (fgets(line, sizeof(line), fp1)) {
			if (!strncasecmp(line, "alias", 5)) {
				for (s = line + 5; isspace(*s); s++);
				for (t = alias1[nalias]; *s && !isspace(*s); s++)
					*t++ = *s;
				*t = '\0';
				while (isspace(*s))
					s++;
				for (t = alias2[nalias]; *s && !isspace(*s); s++)
					*t++ = *s;
				*t = '\0';
				nalias++;
				if (nalias > MAXALIAS) {
					fprintf(stderr, "Too many aliases, %d max.\n", MAXALIAS);
					exit(1);
				}
			} else {
				fprintf(stderr, "Unrecognized .ignrc line: %s\n", line);
			}
		}
		fclose(fp1);
	} else {
		perror(".ignrc");
	}


	if (!(fp1 = fopen("ignore", "r"))) {
		perror("ignore");
		exit(1);
	}
	n = 0;
	j = 0;
	p = 0;
	while (fgets(line, sizeof(line), fp1)) {
		j++;
	      nextmsg:
		if (!strncmp(line, "From ", 5)) {
			if (debug) {
				fprintf(stderr, "%6d %16.16s\r", n, line + strlen(line) - 25);
			}
			if (n)
				size[n - 1] = j;
			pos[n] = p;
			j = 1;
			p = ftell(fp1);
			while (fgets(line, sizeof(line), fp1)) {
				j++;
				if (!strncmp(line, "From ", 5)) {
#if 0
					fprintf(stderr, "Unknown address for message %d.\n", n);
					fputs(line, stderr);
#endif
					strncpy(addr[n++], "UNKNOWN", MSGLEN - 1);
					goto nextmsg;
				}
				/* TODO get rid of these hard coded addresses */
				if (!strncmp(line, "From: William Blake", 19)) {
					strcpy(line, "To: games_master\n");
				}
				if (!strncmp(line, "From: P.QUINTIN@DGC.ceo.dg.com\n", 31)) {
					strcpy(line, "To: P.QUINTIN@DGC.ceo.dg.com\n");
				}
				for (s = line; *s == ' ' || *s == '\t'; s++);
				if (!strncmp(s, "554 <", 5)) {
					/* TODO parenthesize the below if so i can 
					 * follow it */
					if ((t = strchr(s, '>')))
						*(t + 1) = '\0';
					s++;
					*(s + 0) = 'T';
					*(s + 1) = 'o';
					*(s + 2) = ':';
					*(s + 3) = ' ';
				}
				if (!strncasecmp(s, "Apparently-To: ", 15))
					s += 11;
				if (!strncasecmp(s, "To: ", 4)) {
					s += 4;
					while (isspace(*s))
						s++;
					if (!strncasecmp(s, "judge", 5) ||
					  !strncasecmp(s, "<judge", 6) ||
					 !strncasecmp(s, "\"judge", 6) ||
					    !strncasecmp(s, "<jdr", 4) ||
					    !strncasecmp(s, "<@angelo.Stanford.EDU:judge", 27) ||
					    !strncasecmp(s, "@vm.uoguelph.ca:judge", 21) ||
					    !strncasecmp(s, "ken", 3) ||
					    !strncasecmp(s, "jdr", 3) ||
					    !strncasecmp(s, "postmaster", 10) ||
					    !strncasecmp(s, "Diplomatic", 10) ||
					    !strncasecmp(s, "<@blake.u.washington.edu:judge", 30) ||
					    ((t = strrchr(s, '!')) && (!strncasecmp(t, "!judge", 6) ||
					  !strncasecmp(t, "!ken", 4))) ||
					    !strcasecmp(s, "ken@milton.u.washington.edu\n"))
						continue;
					line[strlen(line) - 1] = '\0';
					for (i = 0; i < nalias; i++) {
						if (!strcasecmp(s, alias1[i])) {
							strncpy(addr[n], alias2[i], MSGLEN - 1);
							break;
						}
					}
					if (i == nalias) {
						strncpy(addr[n], s, MSGLEN - 1);
					}
					n++;
					if (n > MAXMSG) {
						fprintf(stderr, "Too many messages in file, %d max.\n", MAXMSG);
						exit(1);
					}
					break;
				}
			}
		}
		p = ftell(fp1);
	}
	if (n)
		size[n - 1] = j;

	show = 1;
	for (;;) {
		for (i = j = 0; i < n; i++) {
			if (!done[i]) {
				if (!j++ && show)
					printf("Holding messages for:\n");
				if (show)
					printf("%4d, %4d: %s\n", i, size[i], addr[i]);
			}
		}
		if (!j)
			break;
		printf("Pick a number (use ? for options): ");
		if (!fgets(line, sizeof(line), stdin))
			break;
		edit = 0;
		show = 0;
		quit = 0;
		verb = 0;
		for (s = line; *s && !isdigit(*s); s++) {
			switch (*s) {
			case '\n':
			case '\t':
			case ' ':
				break;

			case 'e':
				edit = 1;
				break;

			case 'x':
				edit = -1;
				break;

			case 'q':
				quit = 1;
				break;

			case 's':
				show = 1;
				break;

			case 'v':
				verb = 1;
				break;

			default:
				printf("Use: [xx]nnn  To process all matches for nnn's address\n");
				printf("  xx = e  To invoke the editor on the messages\n");
				printf("       x  To skip all messages for nnn's address\n");
				printf("       s  To redisplay the list\n");
				printf("       q  To quit, writing rest of messages to ignore.new\n");
				printf("       v  For verbose option on mail\n");
				printf("       ?  For this list\n");
				continue;
			}
		}

		if (!isdigit(*s) && !quit)
			continue;

		i = atoi(s);
		if (i < 0 || n <= i)
			break;
		if (quit || !done[i]) {
			if (!(fp2 = fopen("temp", "w"))) {
				perror("temp");
				exit(1);
			}
			if (!quit) {
				fprintf(fp2,
					"Note: The following messages are out of sequence and may very\n");
				fprintf(fp2,
					"well be obsolete by now.  Check the dates in the headers to\n");
				fprintf(fp2, "put the messages back in sequence.\n");
			}
			rewind(fp1);
			skip = j = 0;
			if (quit) {
				printf("Saving");
			} else if (edit >= 0) {
				printf("Doing %s", addr[i]);
			} else {
				printf("Discarding %s", addr[i]);
			}
#if 0
			while (fgets(line, sizeof(line), fp1)) {
				if (!strncmp(line, "From ", 5)) {
					if (!(skip = !((quit && !done[j]) ||
						       (!quit && !strcasecmp(addr[i], addr[j]))))) {
						printf(" %d", j);
						fflush(stdout);
						done[j]++;
						if (!quit) {
							fputs("\n-----\n\n", fp2);
						}
					}
					j++;
				}
				if (!skip) {
					fputs(line, fp2);
				}
			}
			printf(".\n");
			if (j != n) {
				fprintf(stderr, "Oops, miscounted the From header lines.\n");
				exit(1);
			}
#else
			for (j = 0; j < n; j++) {
				if (!done[j] && (quit || !strcasecmp(addr[i], addr[j]))) {
					printf(" %d", j);
					fflush(stdout);
					done[j]++;
					if (!quit) {
						fputs("\n-----\n\n", fp2);
					}
					fseek(fp1, pos[j], 0);
					fgets(line, sizeof(line), fp1);
					for (;;) {
						fputs(line, fp2);
						if (!fgets(line, sizeof(line), fp1))
							break;
						if (!strncmp(line, "From ", 5))
							break;
					}
				}
			}
			printf(".\n");
#endif
			fclose(fp2);
			if (!quit) {
				/* TODO remove hard coded /usr/ucb/mail */
				sprintf(line,
					"%s/usr/ucb/mail %s -s 'Retrying bounced mail' %s<temp",
					edit ? "vi temp;" : "", verb ? "-v" : "", addr[i]);
			} else {
				rename("temp", "ignore.new");
				printf("Rest of messages written to 'ignore.new'.\n");
				break;
			}

			if (edit >= 0) {
				printf("Doing: %s\n", line);
				system(line);
			} else {
				printf("Discarded.\n");
			}
		} else {
			printf("Been there, done that.\n");
		}
	}
	remove("temp");
	return 0;
}
