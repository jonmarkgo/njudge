/*
 * $Log$
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Many changes, including -
 *   Reminding players to make moves when close to deadline
 *   Special handling for blind games
 *   Passing of '-C' option for specifying dip.conf path
 *   Informaing all masters when a time-warp was detected
 *   Handling of NOLATEPRESS and MANUALSTART flags
 *   Adding automatic wati for all players in a build phase of a transform game
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/03/16 06:52:13  rpaar
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  dip.c
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 *
 *  Portions of this file are public domain by nathan wagner.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "dip.h"
#include "mail.h"
#include "conf.h"
#include "functions.h"
#include "diplog.h"

#define pcontrol if (!(dipent.flags & (F_NOLIST|F_QUIET))) control
#define pprintf  if (!(dipent.flags & (F_NOLIST|F_QUIET))) fprintf

void init(int, char **);
void phase_pending(void);	/* defined in phase.c */
void  inform_party_of_blind_turn(int player_index, char *turn_text);
void CheckRemindPlayer( int player, long one_quarter);

extern int time_warp;  /* Set to 1 if a time-warp was detected */

/****************************************************************************/

int main(int argc, char *argv[])
{
	char exe_name[100];
	
	init(argc, argv);
	
	sprintf(exe_name,"%s-%s", JUDGE_CODE, "dip");

	OPENDIPLOG(exe_name);
	DIPINFO("Started dip");
	
	control = ded[0].d0;

	dipent.pr_valid = 0; /* not yet loaded pr */
	dipent.valid = 0;   /* nor dipent itself */
	if (!tflg) {
		if (!xflg) {
			DIPDEBUG("Processing mail");
			mail();	/* Process mail message on stdin  */
		}
		if (!qflg) {
			DIPDEBUG("Looking for events");
			master();	/* Process any events pending     */
		}
	} else {
		rfp = stdout;
		Dflg++;
		testdipent(sflg, tflg);
		process();
	}

	if (control > 1000)
		control = 0;

	ded[0].d0 = control;

	if (nded > 0) {
		int fd;
		fd = open("dip.ded", O_RDWR | O_CREAT, 0666);
		write(fd, ded, nded * sizeof(ded[0]));
		remove("dip.dedicate");
		close(fd);
	}
        DIPINFO("Ended dip");	
	exit(0);

}

/* 
 * This function will tell all players that a blind turn has happened
 * The text will be varied depending on who is being told
 */

void inform_party_of_blind_turn( int player_index, char *turn_text)
{
	char line[150];

/* ito be made configureable */
	char *rfile = "dip.reply";

	if (Dflg)
              rfp = stdout;
         else if (!(rfp = fopen(rfile, "w"))) {
	        perror(rfile);
                bailout(1);
	}

	fprintf(rfp, "Blind turn %s has occurred.\n\n", turn_text);
	
	switch (dipent.players[player_index].power)
	{
		case MASTER:
			fprintf(rfp, "It is your responsability to inform affected player(s).\n");
			break;

		case OBSERVER:
			break;

		default:
			if (dipent.players[player_index].centers > 0) {
			    /* Player still has some interest */
			    if (dipent.players[player_index].status & SF_MOVE) {
				fprintf(rfp,"A move is expected from you. The master should contact you shortly.\n");
			    } else {
				fprintf(rfp,"The master should send your results shortly.\n");
			    }
			} else {
			    /* What more does a dead player want to know?! */
			}
	}
	fclose(rfp);

	sprintf(line, "%s %s 'Diplomacy blind results %s %s' '%s'",
                              SMAIL_CMD, rfile, 
			      dipent.name, turn_text, dipent.players[player_index].address);
        if (*(dipent.players[player_index].address) != '*' && !Dflg) {
                                execute(line);
           }
}			


/****************************************************************************/

void gotalarm(int unused)
{
	time_t now;

	time(&now);
	fprintf(log_fp, "%24.24s: Log file lock timed-out.\n", ctime(&now));
	savemail();
	bailout(0);
}

/*************************************************************************/

void savemail(void)
{

	/*
	 *  We've run into a problem.  Save our mail input.
	 */

	FILE *fp;
	char line[1024];
	time_t now;

	if (!(fp = fopen(SAVE_FILE, "a"))) {
		perror(SAVE_FILE);
		
		exit(E_FATAL);
	}
	if (lockfd(fileno(fp), 0)) {
		perror("FLOCKF1");
		exit(E_FATAL);
	}
	while (fgets(line, (int) sizeof(line), inp)) {
		fputs(line, fp);
	}

	time(&now);
	fprintf(log_fp, "%24.24s: Message saved on %s.\n", ctime(&now), SAVE_FILE);

	return;

}

/***************************************************************************/

void init(int argc, char **argv)
{

	int i, fd;
	unsigned char *s;
	time_t now;
	struct stat sbuf;


	inp = stdin; /* default */

	/*
	 * read in the configuration, first from config file,
	 * then from environment, finally from the command line
	 * TODO: fold in command line processing for configuration
	 * variables with regular argument processing.
	 */
	conf_init();
/*
	conf_readfile(CONFIG_FILE);
	conf_cmdline(argc, argv);
 */
	/*
	 *  Process command line arguments
	 *
	 *  The initial value of dflg is the directory from which we were loaded.
	 *  We'll cd to that directory to find our datafiles.
	 */

	time(&now);
	srand(now);

	dflg = argv[0];
	if ((nflg = strrchr(argv[0], '/'))) {
		dflg = argv[0];
		*nflg++ = '\0';
	} else {
		dflg = "";
		nflg = argv[0];
	}

	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-')
		for (s = argv[i] + 1; *s; s++) {
			switch (*s) {

			case 'a':
				aflg++;
				break;

			case 'A':
				Aflg++;
				break;

			case 'c':
				/* Will be handled later in conf treatment */
				break; 
			case 'C':
                                if (*++s)
                                        CONFIG_DIR = s;
                                else if (++i < argc)
                                        CONFIG_DIR = argv[i];
                                else {
                                        fprintf(stderr, "Directory must follow C option.\n");
                                        goto usage;
                                }
                                i++;
                                s = " ";
				break;

			case 'D':
				Dflg++;
				break;

			case 'd':
				if (*++s) 
					dflg = s;
				else if (++i < argc)
					dflg = argv[i];
				else {
					fprintf(stderr, "Directory must follow d option.\n");
					goto usage;
				}
				i++;
				s = " ";
				break;

			case 'i':
				if (*++s) {
				    inname = s;
				} else if (++i < argc)
                                        inname = argv[i];
				else {
                                        fprintf(stderr, "File name must follow i option.\n");
                                        goto usage;
                                }
				i++;
				s = " ";
				break;


			case 'q':
				qflg++;
				break;

			case 'r':
				if (*++s)
					rflg = s;
				else if (++i < argc)
					rflg = argv[i];
				else {
					fprintf(stderr, "Game name must follow r option.\n");
					goto usage;
				}
				i++;
				s = " ";
				break;

			case 's':
				if (isdigit(*(s + 1)))
					sflg = atoi(++s);
				else if (i + 1 < argc && (sflg = atoi(argv[i + 1])))
					i++;
				else
					sflg = 1;
				if (!tflg)
					tflg = V_STANDARD;
				i++;
				s = " ";
				break;

			case 't':
				tflg = 0;
				if (isdigit(*(s + 1)))
					tflg = atoi(++s);
				else if (i + 1 < argc && (tflg = atoi(argv[i + 1])))
					i++;
				else
					tflg = V_STANDARD;
				if (!sflg)
					sflg = 1;
				i++;
				s = " ";
				break;

			case 'v':
				vflg++;
				break;

			case 'x':
				xflg++;
				break;

			default:
				goto usage;
		    }
		} else {
		      usage:
			fprintf(stderr, "Usage: [/directory/]%s [-C <directory>] [-c<CONFIG>=<value>] [-aADqvx] [-sseq] [-tvar] [-d directory] [-i<filename>] [-r name]\n", nflg);
			fprintf(stderr, "  The directory specifies where we'll find our data.\n");
			fprintf(stderr, "  -a Don't mess with the at queue.\n");
			fprintf(stderr, "  -A Don't remove anything from the at queue.\n");
			fprintf(stderr, "  -C <sudirectory> Directory where dip.conf is.\n");
			fprintf(stderr, "  -c<CONFIG>=<value> Set <CONFIG> variable to <value>.\n");
			fprintf(stderr, "  -D increments the debugging flag.\n");
			fprintf(stderr, "  -i Use <filename> for input.\n");
			fprintf(stderr, "  -q quick mode, just process mail.\n");
			fprintf(stderr, "  -v Verbose, issue all error messages.\n");
			fprintf(stderr, "  -x no input, don't read stdin for mail.\n");
			fprintf(stderr, "  -r Original name for randomizer (Machiavelli).\n");
			fprintf(stderr, "  -s specifies test mode and sequence.\n");
			fprintf(stderr, "  -t specifies test mode and variant.\n");
			exit(E_FATAL);
		}
	}

	if (CONFIG_FILE == NULL)
	{
		fprintf(stderr, "Must specifiy a configuration file with -C option.\n");
		exit(E_FATAL);	
	}
	/* Read in the config file */
        conf_readfile(CONFIG_DIR, CONFIG_FILE);
        conf_cmdline(argc, argv);

	if (*dflg && chdir(dflg)) {
		perror(dflg);
		exit(E_FATAL);
	}
	/* Interlock the log file to ensure single threading  */

	if ((fd = open(LOG_FILE, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0) {
		perror(LOG_FILE);
		exit(E_FATAL);
	}
	if (!(log_fp = fdopen(fd, "a"))) {
		perror("fdopen");
		exit(E_FATAL);
	}
	if (inname != NULL) {
	    if (!(inp = fopen(inname, "r"))) {
                perror(inname);
                exit(E_FATAL);
            }
	}

	
	if (!stat(KEEPOUT, &sbuf)) {
		savemail();
		exit(0);
	}
	/* TODO change to POSIX sigaction */
	signal(SIGALRM, gotalarm);
	alarm(LOCK_TIMEOUT);

	if (lockfd(fd, 0)) {
		perror("FLOCKF");
		exit(E_FATAL);
	}
	alarm(0);

	time(&now);
	fprintf(log_fp, "%15.15s: dip -%s%s%s\n", ctime(&now) + 4,
		Aflg ? "A" : "", qflg ? "q" : "", xflg ? "x" : "");

	if ((fd = open("dip.ded", O_RDONLY)) < 0)
		if ((fd = open("dip.dedicate", O_RDONLY)) < 0)
			nded = 0;
		else {
			int i, dedicate[MAXUSER];
			nded = read(fd, dedicate, MAXUSER * sizeof(int)) / sizeof(int);
			for (i = 0; i < nded; i++) {
				ded[i].r = dedicate[i];
			}
			close(fd);
	} else {
		nded = read(fd, ded, MAXUSER * sizeof(ded[0])) / sizeof(ded[0]);
		close(fd);
	}

	if (!(cfp = fopen("dip.control", "a"))) {
		perror("dip.control");
		exit(E_FATAL);
	}
	if (!(xfp = fopen("dip.xcontrol", "a"))) {
		perror("dip.control");
		exit(E_FATAL);
	}
}


/****************************************************************************/

void master(void)
{

/*  Process the master file.  Perform any duties and set next time to look. */

	char *s, line[150];
	time_t now, next_time;
	FILE *ifp, *ofp, *ibmfp;
	struct stat sbuf;
        int one_quarter = 8 * 60 * 60; /* Set to 8 hours */
	int i;

#define TWO_HOURS 2*60*60

	if ((ifp = fopen(MASTER_FILE, "r")) == NULL) {
		perror(MASTER_FILE);
		printf("Unable to open master file %s.\n", MASTER_FILE);
		exit(E_FATAL);
	}
	if ((ofp = fopen(TMASTER_FILE, "w")) == NULL) {
		printf("Unable to open new master file temp.master.\n");
		exit(E_FATAL);
	}
	time(&now);
	next_time = now + 365 * 24 * 60 * 60;
	while (getdipent(ifp)) {

                if (now < dipent.deadline) {
                    if (one_quarter >= TWO_HOURS && (now > (dipent.deadline - one_quarter))) {
                        for ( i = 0; i < dipent.n; i++) {
                                CheckRemindPlayer(i, dipent.deadline - now);
                        }
                    }
                }

		if (time_warp) {
#define WARP_FILE "./dip.warp"
			/* A global tiem-warp was detected: inform all found masters of this */
			ibmfp = fopen(WARP_FILE, "w");
			if (ibmfp != NULL) {
			    fprintf(ibmfp, "A time warp was detected. Check that your game deadlines");
			    fprintf(ibmfp, " are correct and inform players accordingly.\n");
			    fclose(ibmfp);

                           for (i = 0; i < dipent.n; i++) {
                            if (dipent.players[i].power < 0)
                                continue;
			    if (dipent.players[i].power == MASTER) {
                                sprintf(line,
                                        "%s %s 'Diplomacy time-warp: %s' '%s'",
                                         SMAIL_CMD, WARP_FILE, dipent.name, dipent.players[i].address);
                                execute(line);
                                while (*s++);
			      }
  
			    }
			}
		}

		if (dipent.process <= now) {
			if (dipent.phase[6] == 'X' || dipent.n <= 0) {
				if ((dipent.flags & (F_GUNBOAT | F_NOREVEAL)) == (F_GUNBOAT | F_NOREVEAL)) {
					dipent.process = now + 365 * 24 * 60 * 60;
				} else {
					continue;
				}
			} else {
				process();
			}
		}
		if (next_time > dipent.process)
			next_time = dipent.process;

		putdipent(ofp, 1);
	}
	fclose(ifp);
	ferrck(ofp, 1001);
	fclose(ofp);
	if (rename(TMASTER_FILE, MASTER_FILE)) {
		fprintf(log_fp, "Error renaming %s to %s.\n", TMASTER_FILE,MASTER_FILE);
		fprintf(stderr, "Error renaming %s to %s.\n", TMASTER_FILE,MASTER_FILE);
		bailout(E_FATAL);
	}
	/*
	   **  Compute the next time and throw ourselves onto the AT queue.
	 */

	next_time += 60;	/* Round up to next minute */
	time(&now);
	if (now + (15 * 60) > next_time)
		next_time = now + (15 * 60);
	s = ctime(&next_time);
	if (!strncmp(s + 11, "14:00", 5))
		s[15] = '1';	/* Prevent 2pm schedules */
	sprintf(line, "%s %s %2.2s%2.2s %6.6s", ATRUN_CMD,
		Aflg ? "norm" : "dorm", s + 11, s + 14, s + 4);
	if (!aflg)
		execute(line);

	/*
	   **  Process any info on the control file.
	 */

	if (control < 1000)
		fclose(cfp);

	if (control && control < 1000 && ded[0].md < now) {
		stat("dip.control", &sbuf);
		if (sbuf.st_size > 0) {
			s = notifies;
			while (*s && *s != '*') {
				if (*s == '+')
					s++;
				sprintf(line,
					"%s dip.control 'Diplomacy control information' '%s'", 
					 SMAIL_CMD, s);
				execute(line);
				while (*s++);
			}
		}
		remove("dip.control");
		ded[0].md = now + MIN_CONTROL;
	}
	fclose(xfp);
	stat("dip.xcontrol", &sbuf);
	if (sbuf.st_size > 0) {
		s = notifies;
		while (*s && *s != '*') {
			if (*s == '+') {
				s++;
				sprintf(line,
					"%s dip.xcontrol 'Diplomacy xcontrol information' '%s'", 
					SMAIL_CMD, s);
				execute(line);
			}
			if (control >= 1000)
				break;
			while (*s++);
		}
	}
	remove("dip.xcontrol");
	control = 0;
}

/* See if the passed player need to make a move, 
   and send reminder message if not yet made */
void CheckRemindPlayer( int player, long one_quarter)
{
    int num_hours;
    char *temp_file = "dip.temp";
    char line[150];
    char *pchar;

    if (!WAITING(dipent.players[player].status) ) return; /* Not waiting for a move */

    if (dipent.players[player].status & SF_REMIND) return; /* Already been reminded */
    num_hours = (int)  (one_quarter / (60 * 60));
    if (num_hours < 1) num_hours = 1;
    if (num_hours != 1) 
	pchar = "s ";
    else
	pchar = " ";

    if ((rfp = fopen(temp_file, "w")) == NULL) {
	 perror(temp_file);
         exit(E_FATAL);
    }

    msg_header(rfp);
    porder('T', player, 0); /* Call this to send copy of player's actual orders */
    fprintf(rfp,"\n\nThere is less than %d hour%sto the deadline", num_hours, pchar);
    fprintf(rfp," and you\nhave not placed complete and valid moves.\n");
    fprintf(rfp,"\nPlease do so before you are marked as late.\n\n");
    fclose(rfp);

    sprintf(line, "%s %s 'Diplomacy %s: %s' '%s'",
                                                SMAIL_CMD,
						temp_file,
                                                "reminder",
                                                dipent.name,
                                              dipent.players[player].address);

   dipent.players[player].status |= SF_REMIND;

   if (*(dipent.players[player].address) != '*') {
	execute(line);
   	sprintf(line,"Move reminder send to %s in game %s", dipent.players[player].address, dipent.name);
   	DIPINFO(line);
   }
}
 
/***********************************************************************/
int process(void)
{
	int i, n;
	time_t now, then;
	FILE *dfp;
	char line[150];
	char title_text[150];
	char phase[sizeof(dipent.phase)];
	int process_set; /* Set to 1 if process has been set */
	static char *dedfmt = "Adding %d to user %d's dedication to yield %d.\n";

	fprintf(log_fp, "Processing game '%s'.\n", dipent.name);
	rfp = NULL;
	time(&now);

	if (now < dipent.grace) {
		for (n = 0, i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (now < dipent.deadline && (dipent.players[i].status & SF_WAIT)) {
				dipent.process = dipent.deadline;
				return 0;
			}
			if (WAITING(dipent.players[i].status)) {
				if (now < dipent.deadline) {
					dipent.process = dipent.deadline;
					return 0;
				}
				dipent.process = dipent.grace;
				while ((then = dipent.process - 24 * 60 * 60) > now)
					dipent.process = then;

				if (dipent.players[i].status & SF_PART &&
				    now >= dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60)) {
					dipent.players[i].status |= SF_MOVED;
				} else {
					if (!n++) {
						if (Dflg)
							rfp = stdout;
						else if (!rfp && !(rfp = fopen("dip.result", "w"))) {
							perror("dip_process: dip.result");
							bailout(1);
						} else {
							msg_header(rfp);
						}
					}
					fprintf(rfp, "Diplomacy game '%s' is waiting for %s's orders",
						dipent.name, dipent.flags & F_QUIET ? "some power"
						: powers[dipent.players[i].power]);
					if (!(dipent.players[i].status & SF_LATE)) {
					    dipent.players[i].status |= SF_LATE;
					    dipent.players[i].late_count++; /* bump up the late count */
					}
					if (dipent.xflags & XF_LATECOUNT) {
						fprintf(rfp, ": %d time%s late",
						dipent.players[i].late_count,
						dipent.players[i].late_count == 1 ? "" : "s" );
					}
					fprintf(rfp,".\n");
				}
			}
		}

		if (n) {
			then = dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60);
			fputs(n == 1 ? "\nThis power" : "\nThese powers", rfp);
			fputs(now < then ? " will be" :
			      n == 1 ? " is now" : " are now", rfp);
			fputs(" considered abandoned and free for takeover", rfp);
			if (now >= then) {
				if (dipent.flags & F_NONMR) {
					fputs(".\n", rfp);
				} else {
					fputs(n == 1 ? ".\nIt" : ".\nThey", rfp);
					fputs(" will be considered in civil disorder if orders are ", rfp);
					fprintf(rfp, "not received\nby %s.\n", ptime(&dipent.grace));
				}
				n = -1;
			} else {
				fprintf(rfp, "\nif orders are not received by %s.\n", ptime(&then));
				n = now < dipent.deadline + 24 * 60 * 60 ? 1 : 0;
			}

			if (!Dflg)
				fclose(rfp);

			for (i = 0; i < dipent.n; i++) {
				register int d, w;

				if (dipent.players[i].power < 0)
					continue;

				if ((w = WAITING(dipent.players[i].status)) || n) {
					if (w && n == -1) {
						if (!(dipent.players[i].status & SF_ABAND)) {
							dipent.players[i].status |= SF_ABAND;
							if (!(dipent.flags & F_NORATE)) {
								ded[dipent.players[i].userid].r += D_ABANDON;
								fprintf(log_fp, dedfmt, D_ABANDON, dipent.players[i].userid,
									ded[dipent.players[i].userid].r);
							}
						}
						pprintf(cfp, "%s%s in game '%s' is now considered abandoned ",
							NowString(), powers[dipent.players[i].power], dipent.name);
						pprintf(cfp, "(%s, %d of %d units).\n", dipent.phase,
							dipent.players[i].units, dipent.players[i].centers);
						pcontrol++;
					}
					if (!(dipent.flags & F_NORATE)) {
						d = n == 1 ?
						    (!w && (dipent.players[i].status & SF_MOVE) ? D_ONTIME : 0)
						    : (w ? D_LATE : 0);
						ded[dipent.players[i].userid].r += d;
						fprintf(log_fp, dedfmt, d, dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					}
					sprintf(line, "%s dip.result 'Diplomacy %s: %s' '%s'",
						SMAIL_CMD,
						w ? "deadline missed"
						: "notice",
						dipent.name,
					      dipent.players[i].address);
					if (*(dipent.players[i].address) != '*')
						execute(line);
				}
			}
			if (n) {
				sprintf(line, "Diplomacy notice: '%s'", dipent.name);
				archive("dip.result", line);
			}
			if (n == -1) {
				pprintf(cfp, "%sGrace period for '%s' expires %s.\n",
					NowString(), dipent.name, ptime(&dipent.grace));
			}
			return 0;
		}
	}
	if (dipent.seq[0] == 'x') {
		if (Dflg)
			rfp = stdout;
		else if (!(rfp = fopen("dip.result", "w"))) {
			perror("dip_process: dip.result");
			bailout(1);
		} else {
			msg_header(rfp);
		}
		i = dipent.np + '0' - dipent.seq[1];
		if (dipent.xflags & XF_MANUALSTART && i <= 0) {
			fprintf(rfp,
                        	"Diplomacy game '%s' is still waiting for master to start it.\n",
                        	dipent.name );
                	pprintf(cfp,
                        	"%sDiplomacy game '%s' is still waiting for master to start it.\n",
                        	NowString(), dipent.name);
			sprintf(title_text,
				"Diplomacy game %s startup waiting",
				dipent.name);

		} else {
		fprintf(rfp,
			"Diplomacy game '%s' is still waiting for %d player%s to sign on.\n",
			dipent.name, i, i == 1 ? "" : "s");
		pprintf(cfp,
			"%sDiplomacy game '%s' is still waiting for %d player%s to sign on.\n",
			NowString(), dipent.name, i, i == 1 ? "" : "s");
			sprintf(title_text,
			        "Diplomacy game %s signup waiting",
				dipent.name);
		}
		if (!Dflg)
			fclose(rfp);
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			sprintf(line, "%s dip.result '%s' '%s'",
				SMAIL_CMD, title_text, dipent.players[i].address);
			if (*(dipent.players[i].address) != '*')
				execute(line);
		}
		dipent.process = now + 168 * 60 * 60;

	} else if (now > dipent.process) {

		if (Dflg)
			rfp = stdout;
		else if (!(rfp = fopen("dip.result", "w"))) {
			perror("dip_process: dip.result");
			bailout(1);
		} else {
			msg_header(rfp);
		}

		if (now >= dipent.grace && dipent.grace > dipent.deadline) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;
		/* Dietmar Kulsch change 10/10/2000 to avoiod players going
		 * abandoned when they submit error orders
		 */
				if ((!(dipent.players[i].status & SF_PART)) && WAITING(dipent.players[i].status)) { 
					dipent.players[i].status |= SF_CD;
					if (!(dipent.flags & F_NORATE)) {
						ded[dipent.players[i].userid].r += D_CD;
						fprintf(log_fp, dedfmt, D_CD, dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					}
					if (!(dipent.flags & F_NONMR)) {
						pprintf(cfp, "%s%s in game '%s' is now considered in civil disorder ",
							NowString(), powers[dipent.players[i].power], dipent.name);
						pprintf(cfp, "(%s, %d of %d units).\n", dipent.phase,
							dipent.players[i].units, dipent.players[i].centers);
						pcontrol++;
					}
				}
			}
		}
		if (!(dipent.flags & F_NORATE) && now < dipent.deadline + 60 * 60) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status & SF_MOVE) &&
				    !(dipent.players[i].status & SF_CD)) {
					ded[dipent.players[i].userid].r += D_ONTIME;
					fprintf(log_fp, dedfmt, D_ONTIME, dipent.players[i].userid,
					ded[dipent.players[i].userid].r);
				}
			}
		}
		if (dipent.flags & F_NONMR) {
			for (n = i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status &
				     (SF_MOVE | SF_MOVED | SF_PART)) == SF_MOVE) {
					fprintf(rfp, "Diplomacy game '%s' is waiting for someone ", dipent.name);
					fprintf(rfp, "to take over the abandoned %s.\n",
					dipent.flags & F_QUIET ? "power" :
					powers[dipent.players[i].power]);
					pprintf(cfp, "%sDiplomacy game '%s' is waiting for someone ", NowString(), dipent.name);
					pprintf(cfp, "to take over the abandoned\n");
					pprintf(cfp, "%s with %d of %d units on the board (%s).\n",
						powers[dipent.players[i].power], dipent.players[i].units,
						dipent.players[i].centers, dipent.phase);
					n++;
				}
			}
			if (n) {
				dipent.process = now + 48 * 60 * 60;

				if (!Dflg)
					fclose(rfp);

				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;

					sprintf(line, "%s dip.result 'Diplomacy waiting %s %s' '%s'",
						SMAIL_CMD,
						dipent.name, dipent.phase, dipent.players[i].address);
					if (*(dipent.players[i].address) != '*' && !Dflg)
						execute(line);
				}
				return 0;
			}
		}
		/* 
		 * See if manual processing is enabled, and block if so
                 */
                if (dipent.xflags & XF_MANUALPROC) {
			process_set = 0;
			/* See if anyone has requested processing */
			for (i = 0; i < dipent.n && !process_set; i++) {
                                if (dipent.players[i].power < 0)
                                        continue;
					if (dipent.players[i].status & SF_TURNGO)
						process_set = 1;
			}
			if (!process_set) {
                            fprintf(rfp, "Game '%s' is waiting for master to process turn.\n",
                                  dipent.name);
                            if (!Dflg) fclose (rfp);
                            for (i = 0; i < dipent.n; i++) {
                                if (dipent.players[i].power < 0)
                                        continue;

                                sprintf(line, "%s dip.result '%s' '%s' '%s'",
                                SMAIL_CMD, "Diplomacy turn waiting:", 
				dipent.name, dipent.players[i].address);
                                if (*(dipent.players[i].address) != '*')
                                        execute(line);
                            }
		            dipent.process = now + 24 *60 *60;  /* Remind each day */
			    return 0;
		        }
                  } 

		/*
		   **  This is something we can actually process!
		 */

		strcpy(phase, dipent.phase);
		if ((i = porder('M', -1, 0))) {
			fprintf(rfp, "Error %d processing orders.\n", i);
			fprintf(stderr, "Error %d processing orders.\n", i);
			if (!Dflg)
				fclose(rfp);
			sprintf(line, "%s dip.result 'Diplomacy error' '%s'", SMAIL_CMD, GAMES_MASTER);
			execute(line);
			bailout(1);
		}
		pprintf(cfp, "%sPhase %s processed for game '%s'.\n", NowString(), phase, dipent.name);

		/*
		   **  Remove any draw/win information (for summary) if it exists
		 */

		sprintf(line, "D%s/draw", dipent.name);
		remove(line);

		if (dipent.phase[6] == 'X') {

			sprintf(line, "D%s/draw", dipent.name);
			if ((dfp = fopen(line, "w")) == NULL) {
				fprintf(log_fp, "dip: Error opening draw file.\n");
				bailout(E_FATAL);
			}
			if ((ofp = fopen("dip.temp", "w")) == NULL) {
				fprintf(log_fp, "process: Error opening second temporary file.\n");
				bailout(E_FATAL);
			}
			msg_header(ofp);

			pprintf(cfp, "%sGame '%s' completed.\n", NowString(), dipent.name);
			fprintf(rfp, "\nThe game is over.  Thank you for playing.\n");
			fprintf(ofp, "Game '%s' has ended in a victory for ", dipent.name);
			time(&now);
			fprintf(dfp, "Game won: %s\n", ctime(&now));
			fprintf(dfp, "The game was won by ");

			/*  Find out who has won */

			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if (dipent.players[i].centers >= dipent.vp) {
					fprintf(ofp, "%s.\n\n", powers[dipent.players[i].power]);
					fprintf(dfp, "%s.\n", powers[dipent.players[i].power]);
					break;
				}
			}

			/*  If game is standard, it has a Boardman Number, if not
			   it has a Miller Number.  Check to see if it is an EP
			   game.  Alert Custodians to victory */
			fclose(ofp);
			fclose(dfp);

			{
				if (dipent.variant != V_STANDARD || dipent.flags & F_GUNBOAT)
					sprintf(line, "%s dip.temp 'MNC: Victory in game %s' '%s'",
					      SMAIL_CMD,
					      dipent.name, MN_CUSTODIAN);
				else
					sprintf(line, "%s dip.temp 'BNC: Victory in game %s' '%s'",
					      SMAIL_CMD, dipent.name, BN_CUSTODIAN);
			}
			execute(line);

			/*
			 * Force regeneration of the summary file if it's a gunboat game.
			 */

			if (dipent.flags & F_GUNBOAT) {
				sprintf(line, "D%s/msummary", dipent.name);
				remove(line);
			}
			/*  This code, ripped from mail.c, around line 1000, should force
			   a new summary to be created nicely */
			{
				char *mflg, *gflg;
				gflg = (dipent.flags & F_GUNBOAT && (dipent.phase[6] != 'X' ||
				  dipent.flags & F_NOREVEAL)) ? "g" : "";
				mflg = (*gflg && dipent.players[player].power == MASTER)
				    ? "m" : "";
				sprintf(line, "%s -C %s -%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, mflg, gflg,
					dipent.flags & F_QUIET ? "q" : "", dipent.variant, dipent.name);
				system(line);
			}

			/*  Mail summary to HALL_KEEPER */

			sprintf(line, "%s D%s/summary 'HoF: Victory in %s' '%s'",
				SMAIL_CMD, dipent.name, dipent.name, HALL_KEEPER);
			execute(line);

		} else {

			deadline((sequence *) NULL, 1);

			fprintf(rfp, "\nThe next phase of '%s' will be %s for %s of %4.4s.\n",
				dipent.name,
				dipent.phase[5] == 'M' ? "Movement" :
				dipent.phase[5] == 'R' ? "Retreats" : "Adjustments",
				dipent.phase[5] == 'B' ? "Winter" :
				dipent.phase[0] == 'F' ? "Fall" :
				dipent.phase[0] == 'U' ? "Summer" : "Spring", dipent.phase + 1);
			if (broadcast_absence_adjust) 
			    fprintf(rfp,"Requested absence(s) activated.\n");
			fprintf(rfp, "The deadline for orders will be %s.\n",
				ptime(&dipent.deadline));
			if (dipent.phase[5] == 'B' && 
				((dipent.xflags & XF_TRANS_BUILD) || (dipent.xflags & XF_ANYDISBAND))) {
			    /* A build phase in a transform game means that all can transform */
			    /* OR when any player can disband */
			    /* Thus set active players in a wait state */
			    for  (i = 0; i < dipent.n; i++) {
                            if (dipent.players[i].power < 0)
                                continue;
				dipent.players[i].status &= ~SF_TURNGO;
				if (dipent.players[i].power != MASTER && 
				    !(dipent.players[i].status & SF_DEAD)) {
					/* A real player, set wait status */
					dipent.players[i].status |= SF_WAIT;
		                 }
			    }
			}
			
		}

		if (!Dflg)
			fclose(rfp);

		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

/*
 * This looks as good a place as any to clear draw flags...
 *    (P Ùositron, 11 Mar 1993)
 */
			dipent.players[i].status &= ~SF_DRAW;
			if (!(dipent.flags & F_BLIND) || (dipent.players[i].power == MASTER)) {
			    sprintf(line, "%s dip.result 'Diplomacy results %s %s' '%s'",
			      SMAIL_CMD, dipent.name, phase, dipent.players[i].address);
			    if (*(dipent.players[i].address) != '*' && !Dflg) {
				execute(line);
			    }
			}
			if (dipentlags & F_BLIND) {
			    /* Ooops, it's a blind variant */
			    /* Special routine to work out what to tell who */
			    inform_party_of_blind_turn(i, phase);
			}
  
		}

		if (!strcmp(dipent.seq, "002")) {
			sprintf(line, "%s dip.result 'Diplomacy results %s %s' '%s'",
				SMAIL_CMD, dipent.name, phase, GAMES_OPENER);
			execute(line);
		}
		sprintf(line, "Diplomacy results '%s' %s", dipent.name, phase);
		archive("dip.result", line);

		phase_pending();
		deadline((sequence *) NULL, 0);
	}
	return 0;		/* reached ? */
}
/***********************************************************************/
