/*
 * $Log$
 * Revision 1.3  1997/03/16 06:53:13  rpaar
 *
 * Revision 1.2  1996/11/05 23:11:36  rpaar
 * USIT changes to fix minor bugs
 */

/*  mail.c -- process an incoming mail message on standard input
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dip.h"
#include "mail.h"
#include "functions.h"
#include "conf.h"

static int junkmail = 0;	/* Non zero if no reply to be sent            */
static int command = 0;		/* Non zero if some intelligable command found */
static int skipping = 0;	/* Non zero if skipping rest of input         */
static int movement = 0;	/* Non zero if movement orders received       */
static char subject[80];	/* Reply mail subject                         */
static char line[150];		/* Temporary line buffer                      */
static char temp[40];		/* Temporary file name                        */
static char rdcom[50];		/* Temp file for executing rundipmap          */
static int press_number = 0;	/* Number for distinct press filenames */

/* Comment this out if you don't want the MAP command */
#define MAP_COMMAND

#define FROM      1
#define GET       2
#define HELP      3
#define LIST      4
#define REPLY     5
#define SET       6
#define SIGNOFF   7
#define SIGNON    8
#define SUBJECT   9
#define OBSERVE   10
#define RESIGN    11
#define BROADCAST 12
#define PHASE     13
#define CLEAR     14
#define TERMINATE 15
#define RESUME    16
#define BECOME    17
#define REGISTER  18
#define WHOIS     19
#define FIXID     20
#define IAMALSO   21
#define NOCONTROL 22
#define ADJUST    23
#define JUNKMAIL  24
#define PROCESS   25
#define ROLLBACK  26
#define VERSION   27
#define CREATE    28
#define SUMMARY   29
#define HISTORY   30
#define MAP       31
#define PRESS     32
#define RESENT    33
/* #define DEDICATE  34 */
#define WHOGAME   35
/* #define DEDGAME   36 */

static char *prelim[] =
{"", "list", "help", "from:",
 "reply-to:", "resent-from:",
 "precedence: junk", "from#mailer-daemon#",
 "subject:", "sign on", "hello", "login",
 "create",
 "observe", "watch",
 "get#", "send me", "send#",
 "summary", "status",
 "register", "i am also",
 "nocontrol", "adjust",
 "version", "history",
 "who game#", "who is#", "who#", "fixid",
 "map" /* , "ded game#", "dedicate#", "ded#" */ };

static int pvalue[] =
{0, LIST, HELP, FROM,
 REPLY, RESENT,
 JUNKMAIL, JUNKMAIL,
 SUBJECT, SIGNON, SIGNON, SIGNON,
 CREATE,
 OBSERVE, OBSERVE,
 GET, GET, GET,
 SUMMARY, SUMMARY,
 REGISTER, IAMALSO,
 NOCONTROL, ADJUST,
 VERSION, HISTORY,
 WHOGAME, WHOIS, WHOIS, FIXID,
 MAP /* , DEDGAME, DEDICATE, DEDICATE */ };

static char *commands[] =
{"", "list", "help", "get", "send me", "send",
 "set", "sign off", "resign", "withdraw",
 "broadcast", "press", "phase", "clear",
 "register", "i am also",
 "who game", "who is#", "who#",
 "version", "history",
 "summary", "status",
 "terminate", "resume", "become",
 "process", "roll back", "map"
			     /* , "ded game", "dedicate#", "ded#" */ };

static int cvalue[] =
{0, LIST, HELP, GET, GET, GET,
 SET, SIGNOFF, RESIGN, RESIGN,
 BROADCAST, PRESS, PHASE, CLEAR,
 REGISTER, IAMALSO,
 WHOGAME, WHOIS, WHOIS,
 VERSION, HISTORY,
 SUMMARY, SUMMARY,
 TERMINATE, RESUME, BECOME,
 PROCESS, ROLLBACK, MAP
			     /* , DEDGAME, DEDICATE, DEDICATE */ };

extern char *generic_names[];

static int address_not_in_list(char *reply_address, char *players_addresses);

/*
 *  Mail: Process a mail file sent to the Diplomacy judge.
 */

int mail(void)
{

	int i, j, k, l, n, got_reply, got_resent, not_eof, full;
	int done_headers;
	char *s, *t;
	FILE *check, *termfp, *dfp, *qfp;
	long now;

	char uuenc;

	someone = "someone@somewhere";

	starting = msg_header_done = 0;

	rfile = "dip.reply";	/* TODO make this configurable */
	if (Dflg)
		rfp = stdout;
	else if (!(rfp = fopen(rfile, "w"))) {
		perror(rfile);
		bailout(1);
	}
	/*
	 * Open the press files (dip.broadcast.#### for other players,
	 * dip.mbroadcast.#### for the Master).
	 */

	open_press();

	skipping = 0;
	*baddr = '*';

/*  Scan the mail file for something recognizable  */

	got_reply = got_resent = 0;
	fprintf(log_fp, "===== Received mail:\n");

	if (!(ifp = fopen("dip.incoming", "w+"))) {
		perror("dip.incoming");
		bailout(1);
	}
	done_headers = 0;
	while (fgets(line, sizeof(line), stdin)) {
		fputs(line, log_fp);
		fputs(line, ifp);
		if (skipping)
			continue;

		/* Force a newline if there's not one, & trim spaces off end & start of
		   line.  */

		if (!(s = strchr(line, '\n'))) {
			s = line + strlen(line);
			strcpy(s, "\n");
		}
		for (--s; isspace(*s); --s) {
			strcpy(s, s + 1);
		}
		for (s = line; isspace(*s); s++);

		if (!*s)
			done_headers = 1;

		/*  If we're reading or skipping press, get out and set 'skipping' flag if
		   we see 'signoff' alone on a line.  */

		if (broad_read || broad_skip) {
			if (!strcasecmp(s, "signoff\n")) {
				skipping++;

				/*
				 * Or, terminate message copy in reply file and reset
				 * reading/skipping press flags if we see 'endpress' or
				 * 'endbroadcast' alone on a line.
				 */

			} else if (!strcasecmp(s, "endpress\n") ||
				   !strcasecmp(s, "endbroadcast\n")) {
				fprintf(rfp, "\nEnd of message.\n\n");
				broad_read = broad_skip = 0;

				/*
				 * Or, copy the press line to the reply file, and if we're
				 * not skipping press copy it to the broadcast files
				 * (duplicate initial '~' for email reasons).
				 */

			} else {
				if (*line == '~')
					fputc('~', rfp);
				fputs(line, rfp);
				if (broad_read) {
					if (*line == '~') {
						fputc('~', bfp);
						fputc('~', mbfp);
					}
					fputs(line, bfp);
					fputs(line, mbfp);
				}
			}

			/*
			 * In any case, we don't need to check commands--if there's
			 * anything following, we'll get it on the next pass. 
			 */

			continue;
		}
		if (*s) {

			if (!signedon) {
				if (moreaddr && isspace(*line))
					lookfor(got_reply ? "Reply-To: " : "From: ", prelim, nentry(prelim), &i);
				else
					s = lookfor(s, prelim, nentry(prelim), &i);
				create = moreaddr = 0;
				switch (pvalue[i]) {
				case FROM:	/*  From: */
					if (got_reply)
						break;
				case REPLY:	/*  Reply-To: */
					if (i == REPLY)
						got_reply++;
					if (got_resent)
						break;
				case RESENT:	/*  Resent-From: */
					if (i == RESENT)
						got_resent++;
					mail_getaddr(s, raddr);
					if (!is_allowed(GLOBAL_PLAYER)) {
						fprintf(rfp, "You have been blacklisted from this judge.\n");
						fprintf(rfp, "Please contact the judge keeper if you want to dispute this decision.\n");
						while (fgets(line, sizeof(line), stdin)) {
							fputs(line, log_fp);
							fputs(line, ifp);
						}
						mail_reply(E_WARN);
						return E_WARN;
					}
					break;
				case JUNKMAIL:
					junkmail++;
					skipping++;
					break;
				case SUBJECT:
					t = subject;
					while (t < subject + sizeof(subject) - 1 && *s && *s != '\n') {
						if (*s != '\'' && *s != '`' && *s != '"')
							*t++ = *s;
						s++;
					}
					*t = '\0';
					break;
				}

				if (!done_headers)
					continue;
				switch (pvalue[i]) {
				case CREATE:
					if ((check = fopen(NO_CREATE, "r"))) {
						fprintf(rfp, "Sorry, but game creation has been disabled");
						if (fgets(line, sizeof(line), check)) {
							fprintf(rfp, " for the following reason:\n");
							do {
								fprintf(rfp, "  %s", line);
							} while (fgets(line, sizeof(line), check));
						} else {
							fprintf(rfp, ".\n");
						}
						fclose(check);
						break;
					} else if (!is_allowed(GLOBAL_MASTER)) {
						fprintf(rfp, "Sorry, but you are not currently allowed to act as Master on this Judge.\n");
						fprintf(rfp, "Please contact the judge keeper if you want to dispute this decision.\n");
						mail_reply(E_WARN);
						return E_WARN;
						break;
					} else if (atoi(config("MAXGAMES")) >= 0 && countgames() >= atoi(config("MAXGAMES"))) {
						/* max games check 13 oct 1997 -- nw */
						fprintf(rfp, "%s currently has a limit of %d games.\n",
							config("JUDGE_CODE"), atoi(config("MAXGAMES")));
						fprintf(rfp, "There are currently %d games listed.\n", countgames());
						fprintf(rfp, "Creating another game would exceed that limit.\n");
						break;
					}
					create++;	/* create name password */
					/* FALL THROUGH */

				case OBSERVE:	/* observe name password */
					if (pvalue[i] == OBSERVE)
						*--s = 'o';
					/* FALL THROUGH */

				case SIGNON:	/* signon name password */
					command++;

					/*
					 *  Open the master file and a new copy.
					 */

					if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
						fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
						return E_FATAL;
					}
					if ((nfp = fopen("dip.tmast", "w")) == NULL) {
						fprintf(rfp, "Error opening dip.tmast master file.\n");
						return E_FATAL;
					}
					if ((i = mail_signon(s))) {
						fclose(mfp);
						fclose(nfp);
						fputs("Bad signon, skipping remainder:\n", log_fp);
						while (fgets(line, sizeof(line), stdin)) {
							fputs(line, log_fp);
							fputs(line, ifp);
						}
						mail_reply(i);
						return 0;
					}
					if (!msg_header_done)
						msg_header(rfp);

					/* TODO: fix this to use a time_t rather than an int.
					 * will probably require another auto variable */
					time((time_t *) & i);
					if (ded[dipent.players[player].userid].md < i - 24 * 60 * 60) {
						ded[dipent.players[player].userid].md = i;
						if ((tfp = fopen("motd", "r"))) {
							while (fgets(line, sizeof(line), tfp)) {
								fputs(line, rfp);
							}
							fclose(tfp);
						}
					}
					break;

				case HELP:
					command++;
					rfile = "data/info";
					fputs("Help command found, skipping remainder:\n", log_fp);
					while (fgets(line, sizeof(line), stdin)) {
						fputs(line, log_fp);
						fputs(line, ifp);
					}
					mail_reply(0);
					return 0;

				case GET:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					while (isspace(*s))
						++s;
					if (strcasecmp(s, "package\n") == 0) {
						send_package(raddr);
					} else if (strcasecmp(s, "dedication\n") == 0) {
						send_dedication(raddr);
					} else {
						strcpy(temp, "data/");
						t = temp + strlen(temp);
						i = '/';
						while (*s && !isspace(*s) && t < temp + sizeof(temp)) {
							if (i == '/' && *s == '.')
								*s = 'x';
							i = *s;
							*t++ = isupper(*s) ? tolower(*s) : *s;
							s++;
						}
						*t = '\0';
						if ((tfp = fopen(temp, "r"))) {
							fclose(tfp);
							sprintf(line, "./smail %s 'Diplomacy file %s' '%s'",
								temp, temp, raddr);
							execute(line);
							fprintf(rfp, "File %s sent.\n\n", temp);
							listflg++;
						} else {
							fprintf(rfp, "Sorry, there is no file %s available.\n", temp);
						}
					}
					break;

/****************  RV  **********************************************
     MAP command added by Russell Vincent (vincent@cc.und.ac.za)
              December 1992
*********************************************************************/
#ifdef MAP_COMMAND

				case MAP:	/* Send a map of this game */

					command++;
					if (!msg_header_done)
						msg_header(rfp);
					listflg++;
					t = name;
					while (isspace(*s))
						s++;
					while (*s && !isspace(*s))
						*t++ = tolower(*s++);
					*t = '\0';

					while (isspace(*s))
						s++;
					uuenc = tolower(*s++);
					if (*name == '*') {
						if (!(tfp = fopen("ALLOW.map-star", "r"))) {
							fprintf(rfp, "The 'map *' command is not supported on this judge\n");
						} else {
							fclose(tfp);
							strcpy(rdcom, "./runlistmap ");
							if (uuenc == 'n')
								strcat(rdcom, "n ");
							else
								strcat(rdcom, "u ");

							tfp = fopen("map.list", "w");
							rewind(tfp);
							while (fgets(line, sizeof(line), stdin)) {
								fputs(line, tfp);
							}
							fclose(tfp);

							execute(rdcom);

							if ((tfp = fopen("map.out", "r"))) {
								fclose(tfp);
								sprintf(line, "./smail map.out 'Diplomacy map request' '%s'",
								  raddr);
								execute(line);
								fprintf(rfp, "\nMap request sent.\n");
								if (uuenc != 'n') {
									fprintf(rfp, "\nThe file is sent in a UUencoded unix compressed format.\n");
									fprintf(rfp, "To extract the map, use:\n");
									fprintf(rfp, "   uudecode <filename\n   uncompress filename\n\n");
									fprintf(rfp, "If you want the map sent in normal ps format, use a <n> on the\n");
									fprintf(rfp, "end of the <map> command: e.g: map * n\n");
								}
							} else {
								fprintf(rfp, "\nWas not able to create a map \n");
							}
							skipping++;
						}
						break;
					}
					if (!(tfp = fopen("ALLOW.map", "r"))) {
						fprintf(rfp, "Map command not supported on this judge\n");
						break;
					}
					fclose(tfp);

					if (!*name) {
						fprintf(rfp, "\nUse 'map name' to retrieve a postscript ");
						fprintf(rfp, "map of the game positions.\n");
						break;
					}
					if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
						fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
						return E_FATAL;
					}
					/*  Search for this entry in the master file. */

					while ((not_eof = getdipent(mfp))) {
						if ((!*name && !(dipent.flags & F_NOLIST))
						    || !strcmp(dipent.name, name)) {
							if (dipent.seq[0] == 'x') {
								fprintf(rfp, "\nGame '%s' has not started yet - no map produced.\n", name);
								break;
							}
							if (!((dipent.variant == V_STANDARD)
							      || (dipent.variant == V_youngstown)
							      || (dipent.variant == V_loeb9)
							      || (dipent.variant == V_chaos)
							      || (dipent.variant == V_1898)
							      || (dipent.variant == V_fleet_rome))) {
								fprintf(rfp, "\nCannot produce maps for %s games.\n", variants[dipent.variant]);
								break;
							}
							tfp = rfp;
							rfp = fopen("map.list", "w");
							mail_listit();
							if (*name && dipent.seq[0] != 'x'
							    && strcmp(dipent.name, "control")) {
								porder('T', -1, listflg);
							}
							fclose(rfp);
							rfp = tfp;
							strcpy(rdcom, "./rundipmap ");
							if (uuenc == 'n')
								strcat(rdcom, "n ");
							else
								strcat(rdcom, "u ");
							if (dipent.variant == V_youngstown)
								strcat(rdcom, "-y ");
							if (dipent.variant == V_loeb9)
								strcat(rdcom, "-l ");
							if (dipent.variant == V_chaos)
								strcat(rdcom, "-c ");
							execute(rdcom);
							if ((tfp = fopen("map.out", "r"))) {
								fclose(tfp);
								sprintf(line, "./smail map.out 'Diplomacy map for %s' '%s'",
									name, raddr);
								execute(line);
								fprintf(rfp, "\nMap of game '%s' sent.\n", name);
								if (uuenc != 'n') {
									fprintf(rfp, "\nThe file is sent in a UUencoded unix compressed format.\n");
									fprintf(rfp, "To extract the map, use:\n");
									fprintf(rfp, "   uudecode <filename\n   uncompress filename\n\n");
									fprintf(rfp, "If you want the map sent in normal ps format, use a <n> on the\n");
									fprintf(rfp, "end of the <map> command: e.g: map <gamename> n\n");
								}
							} else {
								fprintf(rfp, "\nWas not able to create a map for game '%s'\n", name);
							}

							if (*name)
								break;
							if (l)
								fprintf(rfp, "\n");
						}
					}
					fclose(mfp);

					if (!not_eof) {
						if (*name)
							fprintf(rfp, "\nThere is no game '%s' active.\n", name);
						else if (!l) {
							fprintf(rfp, "\nUse 'list name' or 'list full' for more ");
							fprintf(rfp, "information on these games.\n");
							fprintf(rfp, "Press options are: WGPF\n");
							fprintf(rfp, "    where: W = White press allowed\n");
							fprintf(rfp, "           G = Grey press allowed\n");
							fprintf(rfp, "           P = Partial press allowed\n");
							fprintf(rfp, "           F = Fake press allowed\n");
							fprintf(rfp, "           - = Option not allowed\n");
						}
					}
					break;

#else

				case MAP:	/* Map is unavailable */

					command++;
					if (!msg_header_done)
						msg_header(rfp);
					fprintf(rfp, "\nThe map command is unavailable.");
					break;

#endif

/**************************************************************/

				case LIST:	/* List out the status of this game */
					command++;
					listflg++;
					t = name;
					while (isspace(*s))
						s++;
					while (*s && !isspace(*s))
						*t++ = tolower(*s++);
					*t = '\0';

					if ((full = !strcasecmp("full", name)))
						*name = '\0';
					if ((!*name) && (!msg_header_done))
						msg_header(rfp);

					if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
						if (!msg_header_done)
							msg_header(rfp);
						fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
						return E_FATAL;
					}
					/*  Search for this entry in the master file. */

					while ((not_eof = getdipent(mfp))) {
						if ((!*name && !(dipent.flags & F_NOLIST))
						    || !strcmp(dipent.name, name)) {
							if (*name) {
								msg_header(rfp);
								mail_listit();
								if (dipent.seq[0] != 'x' && strcmp(dipent.name, "control"))
									porder('T', -1, listflg);
								break;
							} else if (full) {
								mail_listit();
								fprintf(rfp, "\n");
							} else
								mail_listbrief();
						}
					}
					fclose(mfp);

					if (!not_eof) {
						if (*name) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "There is no game '%s' active.\n", name);
						} else if (!full) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "\nUse 'list name' or 'list full' for more ");
							fprintf(rfp, "information on these games.\n");
							fprintf(rfp, "Press options are: WGPF\n");
							fprintf(rfp, "    where: W = White press allowed\n");
							fprintf(rfp, "           G = Grey press allowed\n");
							fprintf(rfp, "           P = Partial press allowed\n");
							fprintf(rfp, "           F = Fake press allowed\n");
							fprintf(rfp, "           - = Option not allowed\n");
						}
					}
					break;

				case SUMMARY:	/* Get the summary file */
					command++;
					t = name;
					while (isspace(*s))
						s++;
					while (*s && !isspace(*s))
						*t++ = tolower(*s++);
					*t = '\0';

					if (!*name) {
						if (!msg_header_done)
							msg_header(rfp);
						fprintf(rfp, "Name of game for summary must be specified.\n");
						break;
					}
					sprintf(line, "D%s/summary", name);
					if (!(tfp = fopen(line, "r"))) {
						sprintf(line, "D%s/G001", name);
						if (!(tfp = fopen(line, "r"))) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "There is no summary record for game '%s'.\n", name);
							break;
						}
						fclose(tfp);

						/*
						 * The game exists, we have to read the master file
						 * to figure whether or not it is a gunboat game.
						 */

						if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
							return E_FATAL;
						}
						while ((not_eof = getdipent(mfp))) {
							if (!strcmp(dipent.name, name)) {
								break;
							}
						}
						fclose(mfp);

						sprintf(line, "./summary -%s%slv%d %s", not_eof &&
							(dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL)
							&& dipent.flags & F_GUNBOAT ? "g" : "",
							dipent.flags & F_QUIET ? "q" : "",
							not_eof ? dipent.variant : V_STANDARD, name);
						fflush(log_fp);
						if (system(line)) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "Problem generating summary for game '%s'.\n", name);
							break;
						}
						sprintf(line, "D%s/summary", name);
						if (!(tfp = fopen(line, "r"))) {
							if (!msg_header_done)
								msg_header(rfp);
							fprintf(rfp, "Sorry, unable to generate summary for '%s'.\n", name);
							break;
						}
					}
					msg_header(rfp);
					while (fgets(line, sizeof(line), tfp))
						fputs(line, rfp);
					fclose(tfp);
					fputc('\n', rfp);
					break;

				case REGISTER:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					if ((i = newuser(raddr, stdin))) {
						mail_reply(i);
						return (i);
					}
					break;

				case IAMALSO:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					iamalso(raddr, s);
					break;

				case NOCONTROL:
					control = 1000;
					fclose(cfp);
					cfp = xfp;
					fprintf(xfp, "No control information sent.\n");
					fprintf(rfp, "No control information sent.\n");
					break;

				case WHOGAME:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					t = name;
					while (isspace(*s))
						s++;
					while (*s && !isspace(*s))
						*t++ = tolower(*s++);
					*t = '\0';

					if (!(mfp = fopen(MASTER_FILE, "r"))) {
						fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
						return E_FATAL;
					}
					if (!strcasecmp("full", name))
						*name = '\0';

					not_eof = 0;
					if (*name) {
						while (isspace(*s))
							s++;
						full = !strncasecmp("full", s, 4);
						while ((not_eof = getdipent(mfp)))
							if (!strcmp(dipent.name, name)) {
								whogame(full);
								break;
							}
					}
					fclose(mfp);

					if (!not_eof)
						if (*name)
							fprintf(rfp, "There is no game '%s' active.\n", name);
						else
							fprintf(rfp, "Whogame what game?\n");

					break;

				case WHOIS:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					whois(s);
					break;
/* TODO why is this section commented out ? -- nw */
#if 0
				case DEDGAME:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					t = name;
					while (isspace(*s))
						s++;
					while (*s && !isspace(*s))
						*t++ = tolower(*s++);
					*t = '\0';

					if (!(mfp = fopen(MASTER_FILE, "r"))) {
						fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
						return E_FATAL;
					}
					if (!strcasecmp("full", name))
						*name = '\0';

					not_eof = 0;
					if (*name) {
						while (isspace(*s))
							s++;
						full = !strcasecmp("full", s);
						while (not_eof = getdipent(mfp))
							if (!strcmp(dipent.name, name)) {
								dedgame(full);
								break;
							}
					}
					fclose(mfp);

					if (!not_eof)
						if (*name)
							fprintf(rfp, "There is no game '%s' active.\n", name);
						else
							fprintf(rfp, "Dedgame what game?\n");

					break;

				case DEDICATE:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					getded(s);
					break;
#endif
				case VERSION:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					version(rfp);
					break;

				case HISTORY:
					command++;
					history(s);
					break;

				case FIXID:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					setsite(s);
					break;

				case ADJUST:
					command++;
					if (!msg_header_done)
						msg_header(rfp);
					if (sscanf(s, "%d %d", &i, &n) != 2) {
						fprintf(rfp, "Invalid adjustment: adjust %s", s);
					} else {
						if (i < 0 || i > nded) {
							fprintf(rfp, "Invalid user id %d for adjustment: adjust %s", i, s);
						} else {
							fprintf(xfp, "%s adjusted %d from %d ", raddr, i, ded[i].r);
							fprintf(rfp, "%s adjusted %d from %d ", raddr, i, ded[i].r);
							ded[i].r += n;
							fprintf(xfp, "to %d.\n", ded[i].r);
							fprintf(rfp, "to %d.\n", ded[i].r);
						}
					}
					break;

				default:;

				}
			}
			/*  End of presignon commands */
			else {
				s = lookfor(s, commands, nentry(commands), &i);
				switch (cvalue[i]) {

					/*
					 * If we've accumulated any partial press, go ahead and
					 * send it out since a change of parameters can cause a
					 * non-partial broadcast to occur.
					 */

				case SET:
					if (any_broadcast && broad_part) {
						send_press();
						open_press();
					}
					mail_setp(s);	/* Process SET command  */
					break;

				case LIST:
					listflg++;
					break;

				case HELP:
					s = "info";

					/* FALL THRU */

				case GET:
					while (isspace(*s))
						++s;
					if (strcasecmp(s, "package\n") == 0) {
						send_package(raddr);
					} else if (strcasecmp(s, "dedication\n") == 0) {
						send_dedication(raddr);
					} else {
						strcpy(temp, "data/");
						t = temp + strlen(temp);
						i = '/';
						while (*s && !isspace(*s) && t < temp + sizeof(temp)) {
							if (i == '/' && *s == '.')
								*s = 'x';
							i = *s;
							*t++ = isupper(*s) ? tolower(*s) : *s;
							s++;
						}
						*t = '\0';
						if ((tfp = fopen(temp, "r"))) {
							fclose(tfp);
							sprintf(line, "./smail %s 'Diplomacy file %s' '%s'",
								temp, temp, raddr);
							execute(line);
							fprintf(rfp, "File %s sent.\n\n", temp);
							listflg++;
						} else {
							fprintf(rfp, "Sorry, there is no file %s available.\n", temp);
						}
					}
					break;

				case SIGNOFF:
					skipping++;
					break;

					/*
					 * If we've accumulated any partial press, go ahead and
					 * send it out since a resign can cause a non-partial
					 * broadcast to occur.
					 */

				case RESIGN:
					if (any_broadcast && broad_part) {
						send_press();
						open_press();
					}
					if (!strcmp(dipent.players[player].address, "*")) {
						fprintf(rfp, "You are no longer a player in this game.\nYou may not resign again!\n\n");
						break;
					}
					fprintf(rfp, "%s has resigned from game '%s'.\n\n",
						powers[dipent.players[player].power], dipent.name);
					/* WAS mfprintf  1/95 BLR */
					fprintf(bfp, "%s has resigned %s\nas %s in game '%s'.\n\n", xaddr,
					   ((dipent.flags & F_GUNBOAT) &&
					    (dipent.players[player].power != MASTER))
						? someone
					: dipent.players[player].address,
						powers[dipent.players[player].power], dipent.name);
					fprintf(mbfp, "%s has resigned %s\nas %s in game '%s'.\n\n", raddr,
					  dipent.players[player].address,
						powers[dipent.players[player].power], dipent.name);
					broad_signon = 1;
					if (dipent.players[player].power != OBSERVER) {
						pprintf(cfp, "%s%s has resigned in game '%s' (%s, %d of %d units).\n",
							NowString(),
							powers[dipent.players[player].power], dipent.name, dipent.phase,
							dipent.players[player].units, dipent.players[player].centers);
						pcontrol++;
					}
					strcpy(dipent.players[player].address, "*");
					strcpy(dipent.players[player].password, "GONE");
					dipent.players[player].status |= SF_ABAND;
					if (dipent.players[player].power == WILD_PLAYER)
						dipent.seq[1]--;
					if (dipent.players[player].power >= WILD_PLAYER) {
						dipent.players[player].power = -1;
					}
					dipent.players[player].siteid = 0;
					break;

					/*
					 * If there is any accumulated press already, send it
					 * out before we start the new 'press ...' or
					 * 'broadcast' command.
					 */

				case PRESS:
				case BROADCAST:
					if (any_broadcast) {
						send_press();
						open_press();
					}
					mail_press(s, (cvalue[i] == PRESS ? 1 : 0));
					break;

				case CLEAR:
					tfp = fopen("dip.temp", "w");
					rewind(pfp);
					while (fgets(line, sizeof(line), pfp))
						if (dipent.players[player].power != power(*line))
							fputs(line, tfp);
					fclose(pfp);
					sprintf(line, "D%s/P%s", dipent.name, dipent.seq);
					ferrck(tfp, 2001);
					rename("dip.temp", line);
					pfp = tfp;
					fprintf(rfp, "Pending orders cleared.\n\n");
					break;

				case TERMINATE:
					if (dipent.seq[0] == 'x') {
						fprintf(rfp, "Terminate on unstarted game ignored.  ");
						fprintf(rfp, "Use 'resign'.\n");
						break;
					}
					if (dipent.phase[6] == 'X') {
						fprintf(rfp, "Terminate on completed game ignored.\n");
						break;
					}
					if (!PRIVOK) {
						fprintf(rfp, "Sorry, game '%s' is moderated.  ", dipent.name);
						fprintf(rfp, "Only the master can terminate this game.\n\n");
						break;;
					}
					fprintf(rfp, "You have terminated game '%s'.  ", dipent.name);
					fprintf(rfp, "Use the 'resume' command to start\n");
					fprintf(rfp, "it back up.  The game will self destruct ");
					fprintf(rfp, "if no one signs on for one week.\n");

					/* Open file for sending to custodians */
					if ((qfp = fopen("dip.temp", "w")) == NULL) {
						fprintf(log_fp,
							"open_press: Error opening second temporary file.\n");
						bailout(E_FATAL);
					}
					msg_header(qfp);

					mfprintf(bfp, "%s as %s has terminated\n",
						 xaddr, powers[dipent.players[player].power]);
					mfprintf(bfp, "game '%s'.  Anyone who signs on can restart ",
						 dipent.name);
					mfprintf(bfp, "the game by using\n");
					mfprintf(bfp, "the 'resume' command.  The game will self-");
					mfprintf(bfp, "destruct if no one signs\non for one week.\n");
					pprintf(cfp, "%s%s as %s has terminated\n",
						NowString(), xaddr, powers[dipent.players[player].power]);
					pprintf(cfp, "game '%s'.\n", dipent.name);
					pprintf(qfp, "%s as %s has terminated\n",
						xaddr, powers[dipent.players[player].power]);
					pprintf(qfp, "game '%s'.\n", dipent.name);
					dipent.phase[6] = 'X';

					fclose(qfp);

					/* Open draw file (for summary) */
					sprintf(line, "D%s/draw", dipent.name);
					if (!(termfp = fopen(line, "r"))) {
						if ((dfp = fopen(line, "w")) == NULL) {
							fprintf(log_fp, "mail: Error opening draw file.\n");
							bailout(E_FATAL);
						}
						time(&now);
						fprintf(dfp, "Game terminated: %s", ctime(&now));
						fclose(dfp);
					} else {
						fclose(termfp);
					}

					/*
					 * If game is standard, it has a Boardman Number; if
					 * not it has a Miller Number.  Check to see if it is
					 * an EP game.  Alert Custodians to termination
					 */

					{
						if (dipent.variant != V_STANDARD || dipent.flags & F_GUNBOAT)
							sprintf(line,
								"./smail dip.temp 'MNC: Termination in %s' '%s'",
								dipent.name, MN_CUSTODIAN);
						else
							sprintf(line,
								"./smail dip.temp 'BNC: Termination in %s' '%s'",
								dipent.name, BN_CUSTODIAN);
					}
					execute(line);

					/*
					 * Force regeneration of the summary file if it's
					 * a gunboat game.
					 */

					if (dipent.flags & F_GUNBOAT) {
						sprintf(line, "D%s/msummary", dipent.name);
						remove(line);
					}
					/*
					 * This code, ripped from mail.c, around line 1000,
					 * should force a new summary to be created nicely.
					 * NB: so many radical changes to the code have been
					 * made that the line 1000 is like rather inaccurate.
					 */
					{
						char *mflg, *gflg;
						gflg = (dipent.flags & F_GUNBOAT && (dipent.phase[6] != 'X'
										     || dipent.flags & F_NOREVEAL)) ? "g" : "";
						mflg = (*gflg && dipent.players[player].power == MASTER)
						    ? "m" : "";
						sprintf(line, "./summary -%s%s%slv%d %s", mflg, gflg,
							dipent.flags & F_QUIET ? "q" : "", dipent.variant,
							dipent.name);
						system(line);
					}

					/*  Mail summary to HALL_KEEPER */

					sprintf(line, "./smail D%s/summary 'HoF: Termination in %s' '%s'"
						,dipent.name, dipent.name, HALL_KEEPER);
					execute(line);

					broadcast = 1;
					break;

				case RESUME:
					if (dipent.phase[6] != 'X' || !strcmp(dipent.name, "control")) {
						fprintf(rfp, "Resume on active game ignored.\n");
						break;
					}
					dipent.phase[6] = '\0';
					deadline((sequence *) NULL, 1);

					fprintf(rfp, "You have resumed game '%s'.\n", dipent.name);

					fprintf(rfp, "\nThe next phase will be %s for %s of %4.4s.\n",
						dipent.phase[5] == 'M' ? "Movement" :
						dipent.phase[5] == 'R' ? "Retreats" : "Adjustments",
						dipent.phase[5] == 'B' ? "Winter" :
					dipent.phase[0] == 'F' ? "Fall" :
						dipent.phase[0] == 'U' ? "Summer" : "Spring", dipent.phase + 1);
					fprintf(rfp, "The deadline for orders will be %s.\n",
						ptime(&dipent.deadline));

					pprintf(cfp, "%s%s as %s has resumed game '%s'.\n", NowString(),
						xaddr, powers[dipent.players[player].power], dipent.name);

					/* WAS mfprintf  1/95 BLR */
					fprintf(bfp, "%s as %s has resumed game '%s'.\n",
						xaddr, powers[dipent.players[player].power], dipent.name);
					fprintf(mbfp, "%s as %s has resumed game '%s'.\n",
						raddr, powers[dipent.players[player].power], dipent.name);

					mfprintf(bfp, "\nThe next phase will be %s for %s of %4.4s.\n",
						 dipent.phase[5] == 'M' ? "Movement" :
						 dipent.phase[5] == 'R' ? "Retreats" : "Adjustments",
						 dipent.phase[5] == 'B' ? "Winter" :
					dipent.phase[0] == 'F' ? "Fall" :
						 dipent.phase[0] == 'U' ? "Summer" : "Spring", dipent.phase + 1);
					mfprintf(bfp, "The deadline for orders will be %s.\n",
						 ptime(&dipent.deadline));

					/*
					 * Force regeneration of the summary file if it's a
					 * gunboat game.
					 */

					if (dipent.flags & F_GUNBOAT) {
						sprintf(line, "D%s/summary", dipent.name);
						remove(line);
						sprintf(line, "D%s/msummary", dipent.name);
						remove(line);
					}
					for (i = 0; i < dipent.n; i++)
						dipent.players[i].status &= ~SF_DRAW;
					broadcast = 1;
					break;

				case BECOME:
					if (dipent.players[player].power != MASTER) {
						if (dipent.n == 1 && dipent.players[player].power == WILD_PLAYER) {
							/* Not likely to occur, but just in case the player managed
							   to create a game and is now wanting to become Master. */
							if (!is_allowed(GLOBAL_MASTER)) {
								fprintf(rfp, "Sorry, but you are not currently allowed to act as Master on this Judge.\n");
								fprintf(rfp, "Please contact the judge keeper if you want to dispute this decision.\n");
								mail_reply(E_WARN);
								return E_WARN;
							}
							dipent.seq[1]--;
							fprintf(rfp, "%s is now Master for game '%s'.\n",
								raddr, dipent.name);
							pprintf(cfp, "%s%s is now Master for game '%s'.\n", NowString(),
								raddr, dipent.name);
							dipent.flags |= F_MODERATE;
							dipent.flags &= ~F_NORATE;
							dipent.players[player].power = MASTER;
							break;
						} else {
							fprintf(rfp, "Become: Fat chance; you're not a master!\n");
							fprintf(rfp, "Ignoring rest of input.\n");
							skipping++;
							break;
						}
					}
					get_power(s, &n);
					if (n == 0)
						n = power(*s);

					if (0 >= n || n >= WILD_PLAYER) {
						fprintf(rfp, "Become unknown power: %s", s);
						break;
					}
					for (i = 0; i < dipent.n; i++)
						if (dipent.players[i].power == n)
							break;

					if (i == dipent.n) {
						fprintf(rfp, "No one is signed up to play %s.\n", powers[n]);
						break;
					}
					fprintf(rfp, "Master %s assuming identity of %s.\n", raddr, powers[n]);
					fprintf(rfp, "Password %s; Address %s.\n\n",
					      dipent.players[i].password,
					      dipent.players[i].address);
					player = i;
					break;

				case PROCESS:
					if (dipent.players[player].power != MASTER) {
						fprintf(rfp, "Only the master can use the process command.\n");
						break;
					}
					if (*s && phase(s) != 0) {
						fprintf(rfp, "> %sCurrent phase is %s, process command ignored.\n",
						     line, dipent.phase);
						break;
					}
					time(&dipent.start);
					dipent.start -= 24 * 60 * 60;
					for (i = 0; i < dipent.n; i++)
						dipent.players[i].status &= ~(SF_WAIT | SF_MOVE | SF_WAIT);
					dipent.players[0].status |= SF_PROCESS;
					fprintf(rfp, "Phase %s of '%s' will be processed immediately.\n\n",
					      dipent.phase, dipent.name);
					break;

				case ROLLBACK:
					if (dipent.players[player].power != MASTER) {
						fprintf(rfp, "Only the master can use the rollback command.\n\n");
						break;
					}
					if (!(i = atoi(s)))
						i = atoi(dipent.seq) - 1;
					sprintf(temp, "D%s/G%3.3d", dipent.name, i);
					if (!(tfp = fopen(temp, "r"))) {
						fprintf(rfp, "Sorry, %s does not exist, roll back not possible.\n",
							temp);
						break;
					}
					if (!fgets(line, sizeof(line), tfp)) {
						fprintf(rfp, "Sorry, problems reading %s.\n", temp);
						fclose(tfp);
						break;
					}
					line[strlen(line) - 1] = '\0';
					strcpy(dipent.phase, line);
					sprintf(dipent.seq, "%3.3d", i);
					fprintf(rfp, "Game '%s' rolled back to turn %s, %s.\n\n",
						dipent.name, dipent.seq, dipent.phase);
					mfprintf(bfp, "Game '%s' rolled back to turn %s, %s by %s.\n\n",
						 dipent.name, dipent.seq, dipent.phase, raddr);
					broadcast = 1;
					while (fgets(line, sizeof(line), tfp))
						if (*line == '-')
							break;
					while (fgets(line, sizeof(line), tfp)) {
						if (*line == '-')
							break;
						if (sscanf(line, "%*s%d%d%d", &j, &k, &l) == 3) {
							n = power(*line);
							for (i = 0; i < dipent.n; i++) {
								if (dipent.players[i].power == n) {
									dipent.players[i].status = j;
									dipent.players[i].units = k;
									dipent.players[i].centers = l;
									break;
								}
							}
						}
					}
					fclose(tfp);

					fclose(ofp);
					sprintf(Tfile, "D%s/T%s", dipent.name, dipent.seq);
					if ((ofp = fopen(Tfile, "w")) == NULL) {
						fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
						return E_FATAL;
					}
					sprintf(Mfile, "D%s/M%s", dipent.name, dipent.seq);
					if ((tfp = fopen(Mfile, "r")) != NULL) {
						while (fgets(line, sizeof(line), tfp)) {
							if (!strcmp(line, "X-marker\n"))
								*line = 'Y';
							fputs(line, ofp);
						}
						fclose(tfp);
					}
					fputs("X-marker\n", ofp);
					break;

				case SUMMARY:
					{
						char *mflg, *gflg;

						gflg = (dipent.flags & F_GUNBOAT &&
							(dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL))
						    ? "g" : "";
						mflg = (*gflg && dipent.players[player].power == MASTER)
						    ? "m" : "";

						sprintf(line, "D%s/%ssummary", dipent.name, mflg);
						if (!(tfp = fopen(line, "r"))) {
							sprintf(line, "./summary -%s%s%slv%d %s", mflg, gflg,
								dipent.flags & F_QUIET ? "q" : "",
								dipent.variant, dipent.name);
							fflush(log_fp);
							if (system(line)) {
								fprintf(rfp, "Problem generating summary for game '%s'.\n",
									dipent.name);
								break;
							}
							sprintf(line, "D%s/%ssummary", dipent.name, mflg);
							if (!(tfp = fopen(line, "r"))) {
								fprintf(rfp, "Sorry, unable to generate summary for '%s'.\n",
									dipent.name);
								break;
							}
						}
					}

					while (fgets(line, sizeof(line), tfp))
						fputs(line, rfp);
					fclose(tfp);
					fputc('\n', rfp);
					break;

				case REGISTER:
					if (newuser(raddr, stdin) == E_FATAL) {
						mail_reply(E_FATAL);
						return E_FATAL;
					}
					break;

				case IAMALSO:
					iamalso(raddr, s);
					break;

				case WHOGAME:
					while (isspace(*s))
						s++;
					full = !strncasecmp("full", s, 4);
					whogame(full);
					break;

				case WHOIS:
					whois(s);
					break;
#if 0
				case DEDGAME:
					while (isspace(*s))
						s++;
					full = !strcasecmp("full", s);
					dedgame(full);
					break;

				case DEDICATE:
					getded(s);
					break;
#endif
				case VERSION:
					version(rfp);
					break;

				case HISTORY:
					history(s);
					break;

				case PHASE:
					if (!(read_phase = phase(s)))
						break;

					/* FALL THRU */
				default:	/* Assume this is a movement order */
					if (signedon > 0) {
						if (read_phase == 0) {
							movement++;
							fprintf(ofp, "%c: %s",
								dipent.pl[dipent.players[player].power], line);
						} else if (read_phase > 0) {
							phase_syntax(read_phase, line);
							fprintf(pfp, "%c: %s",
								dipent.pl[dipent.players[player].power], line);
						}
					}
				}
			}
		}
	}
	fprintf(log_fp, "===== End of received mail\n");

	if (broad_read || broad_skip)
		fprintf(rfp, "\nEnd of message.\n\n");

	if (starting)
		mail_igame();

	if (listflg && signedon)
		mail_listit();

	if (signedon > 0 && strcmp(dipent.name, "control")) {
		ferrck(ofp, 2002);
		fclose(ofp);
		rewind(pfp);

		if ((i = porder('T', player, listflg)) == E_FATAL) {
			fprintf(stderr, "Fatal errors encountered.  Entire order discarded.\n");
			fprintf(rfp, "Fatal errors encountered.  Entire order discarded.\n");
			mail_reply(E_FATAL);
		}
		if (dipent.players[player].power == MASTER) {
			int j;

			fprintf(rfp, "\n");
			for (j = 0; j < dipent.n; j++) {
				if (dipent.players[j].power < 0)
					continue;

				if (dipent.players[j].status & SF_DRAW) {
					if (dipent.flags & F_NODIAS) {
						fprintf(rfp, "%-15s: DRAW %s\n", powers[dipent.players[j].power],
						 dipent.players[j].pref);
					} else {
						fprintf(rfp, "%-15s: DRAW YES\n", powers[dipent.players[j].power]);
					}
				}
			}
		} else {
			if (dipent.players[player].status & SF_DRAW) {
				if (dipent.flags & F_NODIAS) {
					fprintf(rfp, "%s: DRAW %s\n", powers[dipent.players[player].power],
					    dipent.players[player].pref);
				} else {
					fprintf(rfp, "%s: DRAW YES\n", powers[dipent.players[player].power]);
				}
			}
		}


		if (rename(Tfile, Mfile)) {
			fprintf(rfp, "Error renaming %s to %s.\n", Tfile, Mfile);
			mail_reply(E_FATAL);
		}
		if (*dipent.players[player].address != '*')
			dipent.players[player].status &= ~(SF_CD | SF_ABAND);

		if (!more_orders)
			dipent.players[player].status |= SF_PART;
		else
			dipent.players[player].status &= ~SF_PART;

		if (i == 0) {
			if (more_orders) {
				if (dipent.players[player].power == MASTER) {
					fprintf(rfp, "\n\nOrders not listed as received from all players.  ");
					fprintf(rfp, "Deadline is\n%s, ", ptime(&dipent.deadline));
					fprintf(rfp, "grace is %s.\n\n", ptime(&dipent.grace));
				} else {
					dipent.players[player].status &= ~SF_MOVED;
					fprintf(rfp, "\n\nOrders not received for all units.  If complete ");
					fprintf(rfp, "orders are not\n");
					fprintf(rfp, "received by %s, you will ", ptime(&dipent.deadline));
					fprintf(rfp, "be considered late.\n");
					if (dipent.flags & F_NONMR) {
						fprintf(rfp, "You will be considered abandoned if nothing is ");
						fprintf(rfp, "received by\n%s.\n\n", ptime(&dipent.grace));
					} else {
						fprintf(rfp, "The partial orders will be processed if nothing ");
						fprintf(rfp, "is received by\n%s.\n\n", ptime(&dipent.grace));
					}
				}
			} else {
				dipent.players[player].status |= SF_MOVED;
				if (dipent.players[player].status & SF_WAIT) {
					fprintf(rfp, "\n\nYou have set 'wait' status so orders will not be ");
					fprintf(rfp, "processed\nbefore %s.\n\n", ptime(&dipent.deadline));
				}
			}
		} else {
			dipent.players[player].status &= ~SF_MOVED;
			fprintf(rfp, "\n\n%d error%s encountered.\n\n", i, i == 1 ? "" : "s");
			if (dipent.players[player].status & SF_MOVE) {
				long then;
				fprintf(rfp, "Unless error-free orders are received by the deadline ");
				fprintf(rfp, "of\n%s you will be considered late.\n",
					ptime(&dipent.deadline));

				then = dipent.grace - ((dipent.flags & F_NONMR) ? 0 : 24 * HRS2SECS);
				if (more_orders) {
					fprintf(rfp, "You will be considered abandoned if nothing is ");
					fprintf(rfp, "received by\n%s.\n\n", ptime(&then));
				} else {
					fprintf(rfp, "The partial orders will be processed if nothing ");
					fprintf(rfp, "is received by\n%s.\n\n", ptime(&then));
				}
			}
		}
		deadline((sequence *) NULL, 0);

		for (i = 0; fgets(line, sizeof(line), pfp);) {
			if (dipent.players[player].power == MASTER ||
			  dipent.players[player].power == power(*line)) {
				if (!i++)
					fprintf(rfp, "\n----- Pending orders:\n\n");
				fputs(line, rfp);
			}
		}
		fclose(pfp);
	}
	if (!command) {
		fprintf(rfp, "Signon command not encountered.  Use 'help' for info.\n");
		mail_reply(E_WARN);
	} else {
		mail_reply(0);
	}

	if (signedon) {
		do {
			putdipent(nfp, 1);
			if (generic) {
				char c;
				for (c = dipent.name[0]; c <= 'z'; c++) {
					if (generic_names[c - 'a'][1] != '*') {
						i = dipent.flags;
						newdipent(generic_names[c - 'a'], V_STANDARD);
						dipent.seq[2] = 'x';	/* Make this the new generic game */
						dipent.seq[3] = '\0';
						if (dipent.players[0].power == MASTER) {
							dipent.n = 1;
							dipent.flags = i;
						}
						putdipent(nfp, 1);
						generic = 0;
						pprintf(cfp, "%sGame '%s' is now waiting for new players.\n", NowString(),
							dipent.name);
						break;
					}
				}
				if (c > 'z') {
					fprintf(xfp, "Out of generic names: '%s'.\n", dipent.name);
				}
			}
		} while (getdipent(mfp));
		fclose(mfp);
		ferrck(nfp, 2003);
		fclose(nfp);
		rename("dip.tmast", MASTER_FILE);
	}
	/* TODO not sure what to return here, i'll return a one in hopes that that
	 * will work */
	return 1;
}

void open_press(void)
{

	/*
	 *  open_press:  Open the press files and reset all press variables.
	 */

	int i;

	sprintf(bfile, "dip.broadcast.%4.4d", press_number);
	if (!(bfp = fopen(bfile, "w"))) {
		perror(bfile);
		bailout(1);
	}
	sprintf(mbfile, "dip.mbroadcast.%4.4d", press_number);
	if (!(mbfp = fopen(mbfile, "w"))) {
		perror(mbfile);
		bailout(1);
	}
	broadcast = broad_part = broad_allbut = broad_read = broad_skip =
	    broad_signon = broad_params = 0;
	for (i = 0; i < sizeof(broad_list); broad_list[i++] = '\0');
	if (++press_number == 10000)
		press_number = 0;

	return;
}

void mail_reply(int err)
{

	/*
	 *  reply: send a mail message to appropriate recipients.
	 */

	char line[1024];
	char *s;
	int i;

	if ((err != E_FATAL) && (!junkmail))
		send_press();

	if (err != 0 && !junkmail) {
		fflush(ifp);
		rewind(ifp);
		fprintf(rfp, "\n---- Original message follows:\n\n");
		while (fgets(line, sizeof(line), ifp))
			fputs(line, rfp);
	}
	if (!Dflg)
		fclose(rfp);
	fclose(ifp);

	if (err == E_FATAL) {
		sprintf(line, "./smail %s 'Diplomacy Error' '%s'", rfile, GAMES_MASTER);
		execute(line);
		bailout(E_FATAL);
	}
	if (junkmail)
		return;

	if (signedon)
		s = dipent.players[player].address;
	else
		s = raddr;
	if (*s && *s != '*' && !Dflg) {
		sprintf(line, "./smail %s 'Re: %s' '%s'", rfile, subject, s);
		if ((i = execute(line))) {
			fprintf(log_fp, "Error %d sending mail to %s.\n", i, s);
		}
	}
	if (*raddr != '*' && address_not_in_list(s, raddr) && !Dflg) {

		/* TODO make the ./smail configurable */
		sprintf(line, "./smail %s 'Re: %s' '%s'", rfile, subject, raddr);
		if ((i = execute(line))) {
			/* TODO, why not just have execute() write it's errors to the log */
			fprintf(log_fp, "Error %d sending mail to %s.\n", i, raddr);
		}
	}
	return;
}

/*
 *  Write message header to output file.
 */

void msg_header(FILE * fp)
{
	char *temp;

	/* The 4-letter Judge code (or "XXXX" if no code assigned yet).  */

	fprintf(fp, ":: Judge: %s  ", config("JUDGE_CODE"));

	/*  The game name (omitted if not signed onto a game).  */

	if (strlen(dipent.name)) {
		fprintf(fp, "Game: %s  ", (temp = strcap(dipent.name)));
		free(temp);

		/*  The game variant (omitted if not signed onto a game).  */

		fprintf(fp, "Variant: %s", (temp = strcap(variants[dipent.variant])));
		if (dipent.flags & F_GUNBOAT)
			fprintf(fp, " Gunboat");
		if (dipent.flags & F_BLIND)
			fprintf(fp, " Blind");
		fprintf(fp, "\n");
		free(temp);

		/*  The phase and deadline (omitted if not signed onto a game).  */

		fprintf(fp, ":: Deadline: %s %s  ", dipent.phase, ptime(&dipent.deadline));

		/*  The Boardman or Miller number (omitted if not signed onto a game).  */

		if (strlen(dipent.bn_mnnum) > 7) {
			fprintf(fp, "Miller: %s  ", dipent.bn_mnnum);
		} else if (strlen(dipent.bn_mnnum)) {
			fprintf(fp, "Boardman: %s  ", dipent.bn_mnnum);
		}
	}
	/*  Terminate the line and set the flag.  */

	fprintf(fp, "\n\n");
	msg_header_done = 1;
	return;
}

/*
 * Broadcast a message to all other players if necessary.
 */

void send_press(void)
{
	char line[1024];
	char *s;
	int i, j;

	/*
	 * If parameters have been changed, copy the new parameters to the
	 * broadcast files.
	 */

	if (signedon && broad_params) {
		if (broadcast)
			mfprintf(bfp, "\nEnd of message.\n\n");
		fprintf(rfp, "Parameters for game '%s' changed to:\n", dipent.name);
		params(rfp);
		fputs("\n", rfp);
		/* WAS mfprintf  1/95 BLR */
		fprintf(bfp, "%s as %s has changed the parameters for game '%s' to:\n",
			xaddr, PRINT_POWER, dipent.name);
		fprintf(mbfp, "%s as %s has changed the parameters for game '%s' to:\n",
			raddr, PRINT_POWER, dipent.name);
		params(bfp);
		params(mbfp);
		fputs("\n", bfp);
		fputs("\n", mbfp);
	}
	/*  Close the broadcast and master-broadcast files.  */

	fclose(bfp);
	fclose(mbfp);

	/*
	 * If this is a 'quiet'ed game, and a signon/resign took place,
	 * broadcast it only to the master.
	 */

	if (dipent.flags & F_QUIET && broad_signon) {
		broad_list[0] = dipent.pl[MASTER];
		broad_list[1] = '\0';
		broad_part = 1;
		broad_allbut = 0;
	}
	/*
	 * For each player in the game, if he isn't the originating player and
	 * has an email address, and is in the broadcast list (if partial
	 * press), send him the broadcast file (send the Master the
	 * master-broadcast file, of course).
	 */

	if (signedon && any_broadcast && control < 1000) {
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if ((i != player || dipent.players[i].power == MASTER) &&
			    *dipent.players[i].address != '*') {
				if (broad_part && (dipent.players[i].power != MASTER
						   || !(dipent.players[i].status & SF_PRESS))) {
					for (s = broad_list; *s; s++)
						if (power(*s) == dipent.players[i].power)
							break;
					if (!(!*s ^ !broad_allbut))
						continue;
				}
				if (dipent.players[i].power == MASTER) {
					sprintf(line, "./smail %s 'Diplomacy notice: %s' '%s'",
						mbfile, dipent.name, dipent.players[i].address);
				} else {
					sprintf(line, "./smail %s 'Diplomacy notice: %s' '%s'",
						bfile, dipent.name, dipent.players[i].address);
				}
				if ((j = execute(line))) {
					fprintf(log_fp, "Error %d sending broadcast message to %s.\n",
					   j, dipent.players[i].address);
				}
			}
		}

		/*
		 * If the incoming mail came from somewhere other than the player's
		 * registered email address, then send a copy to that address as
		 * well.
		 */

		if (*baddr != '*') {
			sprintf(line, "./smail %s 'Diplomacy notice: %s' '%s'",
				bfile, dipent.name, baddr);
			if ((j = execute(line)))
				fprintf(log_fp, "Error %d sending broadcast message to %s.\n", j, baddr);
		}
		/*  Put a copy into the archives.  */

		j = 1;
		if (broad_part) {
			for (s = broad_list; *s; s++)
				if (power(*s) == OBSERVER)
					break;
			if (!(!*s ^ !broad_allbut))
				j = 0;
		}
		if (j) {
			sprintf(line, "Diplomacy notice: '%s'", dipent.name);
			archive(bfile, line);
		}
	}
	return;
}

/*
 * address_not_in_list: Checks whether the reply_address is part of
 *                      the players_address.
 *                      The "not" is there so that it is a drop-in
 *                      replacement for strcasecmp().
 * written 5 Aug 1997 by Chris Brand cbrand@ccgate.hac.com
 * added Tue Oct 28 17:41:44 GMT 1997 
 */
static int address_not_in_list(char *reply_address, char *players_addresses)
{
	char *address = NULL;
	int result = 0;
	const char tokens[] = ", ";

	/* strtok messes up the first string, so use a copy */
	/* TODO use a local char[] */
	char *copy = strdup(players_addresses);

	/* We need to split players_addresses into it's constituent parts */
	/* These will be separated by commas (RFC 822) */

	/* Fail safe - use the original functionality */
	if (copy == NULL)
		return strcasecmp(reply_address, players_addresses);

	/* If there's a comma in reply_address, do a straight comparison */
	/* This avoids annoying players who set Reply-to: to avoid multiple replies */
	if (strchr(reply_address, ',') != NULL) {
		result = strcasecmp(reply_address, players_addresses);
	} else {
		/* Get the first address from the list of addresses */
		address = strtok(copy, tokens);

		while (address != NULL) {
			/* Compare this address with reply_address */
			result = strcasecmp(reply_address, address);

			/* If it matches, we know the address is in the list */
			if (!result) {
				address = NULL;
			} else {
				/* Otherwise, we have to keep going */
				address = strtok(NULL, tokens);
			}
		}
	}

	/* Get rid of the local copy */
	free(copy);

	return result;
}
