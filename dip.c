/*
 * $Log: dip.c,v $
 * Revision 1.69  2006-05-03 03:09:08  alange
 *
 * Bug 467. Spurious absence activation notices quashed.
 *
 * Revision 1.68  2005/06/16 01:44:52  alange
 * Bug 257: Notify masters on bailout recovery and time-warp.
 *
 * Revision 1.67  2004/10/24 09:02:31  millis
 * Small victory handling corrections
 *
 * Revision 1.66  2004/10/23 22:43:28  millis
 * Bug 363 and 368, AlliedWin and Conced/NoDias in duplex games fixes
 *
 * Revision 1.65  2004/10/13 00:23:45  alange
 * Added newline to end of Congrats line so dip.footer would start on a new line.
 *
 * Revision 1.64  2004/09/06 22:08:16  millis
 * Bug363 Allow more than one victor
 *
 * Revision 1.63  2004/08/17 09:49:01  millis
 * Bug 355: missed incrementing just_now_abandoned flag.
 *
 * Revision 1.62  2004/08/16 21:51:34  millis
 * Refix bug 347 (games can go NoNMR) and also Bug 355 (no abandoned player(s)
 * messages.
 *
 * Revision 1.60  2004/08/03 21:57:04  millis
 * Fix Bug 347 (games incorrectly process when NoNMR and abandoned playerss)
 * Fix Bug 350 (too many powers shown as abandoned in Intimate)
 *
 * Revision 1.59  2004/07/27 23:07:17  millis
 * Bug 343 (say how many missing players) and Bug 247 (extra dots in secret
 * game late warning)
 *
 * Revision 1.57  2004/07/25 16:13:42  millis
 * Bug fixes for Bug 91 (Duplex powers), Bug 233 (Abandoned power cannot
 * return in duplex) and Bug 206 (allow takeover of unknown abandoned
 * countries)
 *
 * Revision 1.56  2004/07/12 09:46:14  millis
 * Bug 226: ON reflection, 3 day reminders for paused games seems right
 * (as if less, players would get most annoyed!).
 *
 * Revision 1.55  2004/07/12 00:21:59  millis
 * Fix Bug 226: remind every move frequency for paused games.
 * Also a fix for Bug 91 / Bug 297, to only mail real players on a game pause.
 *
 * Revision 1.54  2004/07/07 22:50:39  millis
 * Bug91: further fixes for Duplex code
 * (these mainly to get absences and late handling working)
 *
 * Revision 1.53  2004/07/04 03:28:12  millis
 * Bug 97: don't make each power have to move if duplex and supress
 * extra 'reminder' messages.
 *
 * Revision 1.52  2004/06/27 01:50:21  millis
 * Futher Intimate fixes (Bug 297) specifically to allow phased orders
 * and correct turns not processing, plus more information printed.
 *
 * Revision 1.51  2004/05/23 15:53:40  millis
 * Restored inadvertently deleted 1.49 and 1.48 changes.
 *
 * Revision 1.50  2004/05/22 08:53:14  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.49  2004/04/04 15:58:37  millis
 * Fixed bug 285 (inform extra custodians of game start)
 *
 * Revision 1.48  2004/03/28 09:52:12  millis
 * Fix bug 282 (reset msg_header_done on closing rfp file)
 *
 * Revision 1.47  2004/02/14 23:32:11  millis
 * Allow use of fixed time via parameter (for debugging)
 *
 * Revision 1.46  2004/01/21 00:55:57  millis
 * Allow passing of forced date/time (for testing)
 *
 * Revision 1.45  2003/12/28 00:13:38  millis
 * Fix bug 229 (display on email title if game with abandoned players is NoList)
 *
 * Revision 1.44  2003/09/09 19:57:08  nzmb
 * Fixed Bug 31 -- the time to deadline/grace expiration messages are no
 * longer printed for terminated games, and neither are the "orders not
 * received ... you will be considered late/abandoned" message. The list of
 * entered orders is still displayed, though.
 *
 * Revision 1.43  2003/08/25 14:39:35  millis
 * Fixed bug 220
 *
 * Revision 1.42  2003/08/14 22:56:32  millis
 * Fix bug 158
 *
 * Revision 1.41  2003/07/17 22:59:29  millis
 * Bug 185
 *
 * Revision 1.40  2003/06/29 21:37:40  nzmb
 * Made EOG draw entries broadcasted at the end of the game.
 *
 * Revision 1.39  2003/05/13 00:07:25  millis
 * Bug 110, move on process deadline by 24 hours on bailout recovery
 *
 * Revision 1.38  2003/05/12 23:23:45  millis
 * Fix bug 133, allow turn to process when set to manualprocess and process command sent.
 *
 * Revision 1.37  2003/05/12 02:37:17  millis
 * Removed superfluos SF_WAIT for transform games
 * Also deleted incorrect use of uninitilized variable
 *
 * Revision 1.36  2003/05/03 23:08:41  millis
 * Fixed bug 99 (resigned players still receiving messages)
 * Now resigned players will only receive the termination messages.
 *
 * Revision 1.35  2003/02/17 16:39:27  millis
 * Fix bug 81, to call at for year too
 *
 * Revision 1.34  2003/02/17 09:29:28  millis
 * Changed setenv() to putenv() to work on Sun platforms
 *
 * Revision 1.33  2003/02/05 23:53:44  millis
 * Removed dipstats files.
 * Also added in new JUDGE_TZ variable, to specify timezone
 *
 * Revision 1.32  2003/01/13 22:38:51  millis
 * merged in from ustv
 *
 * Revision 1.31  2003/01/12 00:14:37  nzmb
 * Fixed it so that postal press reports are not sent when no press is submitted.
 *
 * Revision 1.30  2002/12/28 00:52:18  millis
 * Proper fix to CR 17
 *
 * Revision 1.29  2002/11/13 22:26:58  millis
 * Bug 23, don't show missing players in a secret game
 *
 * Revision 1.28  2002/10/19 22:44:17  millis
 * Removed a simple warning
 *
 * Revision 1.27  2002/08/27 23:56:09  millis
 * Added better victory display (fixing Mach victory bug)
 *
 * Revision 1.26  2002/08/27 22:27:49  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.25  2002/07/16 18:14:20  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.24  2002/05/16 13:05:33  miller
 * Added used of X2F_MORE:HOMES flags
 * Don't send blind games to openings list
 * Allow blind games to not show centres
 *
 * Revision 1.23  2002/05/11 09:15:30  greg
 * Minor bug fixes
 * - fixed subjectline for absence requests
 * - fixed phase length, so it's no longer hard coded for responses
 * - partial fix for unusable builds, players with only unusable builds
 *    will no longer be flagged as having orders due, however players
 *    with some usable builds will need to waive any unusable builds,
 *    also, if one or more players have unusable builds, but no
 *    player has usable builds, the build phase will process after
 *    a short delay
 *
 * Revision 1.22  2002/05/04 02:06:16  nzmb
 * Added code to display the time left until the deadline and grace at the
 * bottom of their reply whenever a player signs on.
 *
 * Revision 1.21  2002/04/18 04:44:30  greg
 * Added the following commands:
 * - unstart
 * - set secret
 * - set [prflist|prfrand|prfboth]
 *
 * Fixed Set Absence so that "to" is not case sensitive
 *
 * Fixed Quiet games so that new players are announced
 * before the game starts
 *
 * Fixed ascii_to_ded.c so thatit no longer generates an
 * error when compiled
 *
 * Revision 1.20  2002/04/15 12:55:41  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.19  2002/04/09 10:54:38  miller
 * Add check before resending abandoned messages
 *
 * Revision 1.18  2002/04/06 14:40:27  miller
 * Fixed a bad '=' that casued all builds to not set SF_MOVE flag (DAMN!)
 *
 * Revision 1.17  2002/03/10 12:32:52  miller
 * Added automatic wait for all players in BUILD_TRANSFORM games
 *
 * Revision 1.16  2001/11/20 07:43:45  greg
 * Fixed last (hopefully) problem with "Waiting for Master to Start" subjectline:
 *
 * Revision 1.15  2001/11/11 21:16:19  greg
 * Subjectline Fixes
 *  - New player signons will no longer show "Preference Change"
 *  - Manual start games will no longer say "Waiting for More Players" after the game is full
 *  - reply lines no longer assume JUDGE_CODE is four characters
 *
 * Revision 1.14  2001/08/18 06:09:15  nzmb
 * Added code to reset concession flags when turn processes, and not to warn
 * people in terminated games.
 *
 * Revision 1.13  2001/07/15 09:13:52  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.12  2001/07/14 07:05:17  greg
 * a few bug fixes & show who's late to GM in quiet games
 *
 * Revision 1.11  2001/07/10 04:31:24  nzmb
 * Fixed bug in last upgrade that caused dedpoints to not always be applied.
 *
 * Revision 1.10  2001/07/08 22:53:25  miller
 * Use define for warp file
 *
 * Revision 1.9  2001/06/24 05:19:25  nzmb
 * Added inteface to player data database so player records are recorded,
 * and also provided option of have D_LATE applied at the deadline or
 * (default) 24 hours later.
 *
 * Revision 1.8  2001/05/12 07:57:56  greg
 * added Mario Becroft's dedication problem fix
 *
 * Revision 1.7  2001/05/12 06:57:39  greg
 * added subjectlines
 *
 * Revision 1.6  2001/04/17 21:30:31  miller
 * Refixed problem with CONFIG_DIR parsing
 *
 * Revision 1.5  2001/04/15 21:21:22  miller
 * Correctly show pending orders when late warning is sent
 *
 * Revision 1.4  2001/02/22 23:25:17  miller
 * Fixed bug
 *
 * Revision 1.3  2001/02/03 10:36:16  miller
 * fixed blind crashing bug
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Many changes, including -
 *   Reminding players to make moves when close to deadline
 *   Special handling for blind games
 *   Passing of '-C' option for specifying dip.conf path
 *   Informing all masters when a time-warp was detected
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
#include <errno.h>

#include "config.h"
#include "dip.h"
#include "mail.h"
#include "conf.h"
#include "functions.h"
#include "diplog.h"
#include "plyrdata.h"

#define pcontrol if (!(dipent.flags & (F_NOLIST|F_QUIET))) control
#define pprintf  if (!(dipent.flags & (F_NOLIST|F_QUIET))) fprintf

/* Comment the next line out if you wish the Judge to apply D_LATE right
   at the deadline rather than 24 hours later. */
/*#define NORMDED */

void init(int, char **);
void phase_pending(void);	/* defined in phase.c */
void  inform_party_of_blind_turn(int player_index, char *turn_text, char*);
void CheckRemindPlayer( int player, long one_quarter);
void CheckSizes(void);   /* Check that no sizes have changed */

extern int time_warp;  /* Set to 1 if a time-warp was detected */

/****************************************************************************/

int main(int argc, char *argv[])
{
	char exe_name[100];
	char tz_name[50];
	char *t;	
        struct stat buf;

	init(argc, argv);
	

	/* 
	 * Change the judge timezone, if set
	 */

	t = JUDGE_TZ;

	if (*t) {
	    sprintf(tz_name,"TZ=%s", t);
	    putenv(tz_name);
	    tzset();
	}

 	sprintf(exe_name,"%s-%s", JUDGE_CODE, "dip");

	OPENDIPLOG(exe_name);
	DIPINFO("Started dip");

	/* Check if xforward file exists, indicating a bailout-recovery situation */

	if (!stat(XFORWARD, &buf)) {
	    bailout_recovery = 1;
	}

        CheckSizes();

	if(open_plyrdata() != 0)
	{
		fprintf(log_fp,"Unable to open plyrdata file.\n");
	}
	put_data(0,total);
	
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
	close_plyrdata();

	/* If block file exists, remove it */

	t = BLOCK_FILE;
	if (t[0])
	    remove(t);

        DIPINFO("Ended dip");	
	exit(0);

}

void CheckSizes(void)
{
    FILE *fptr;
    int ss, si, sl;

    fptr = fopen(".size.dat","r");
    if (!fptr) return;  /* Assume it is ok if no file found */
    fscanf(fptr, "%d %d %d", &ss, &si, &sl);
    fclose(fptr);

    if (ss != sizeof(short) || si != sizeof(int) || sl != sizeof(long))
        bailout(E_FATAL);   /* Don't run if sizes bad */
}


/* 
 * This function will tell all players that a blind turn has happened
 * The text will be varied depending on who is being told
 */

void inform_party_of_blind_turn( int player_index, char *turn_text, char *in_file)
{
        char line[150];

/* ito be made configureable */
        char *out_file = "dip.rreply";

        sprintf(line,
                "%s %s %s %s -i=%s -o=%s",
                "./zpblind",
                powers[dipent.players[player_index].power],
                owners[dipent.players[player_index].power],
		dipent.x2flags & X2F_BLIND_CENTRES ? "" : "-v=HideAll",
                in_file,
                out_file);

        system(line);

       if (!(dipent.players[player_index].status & SF_RESIGN)) {
           sprintf(line, "%s '%s:%s - %s Blind Results'",
                out_file, JUDGE_CODE, dipent.name, turn_text);

           if (*(dipent.players[player_index].address) != '*' && !Dflg) {
                MailOut(line, dipent.players[player_index].address);
           }
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
	FILE *fptr;
	char *t;
	char *datetime;

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

	subjectline[0] = '\0';

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
                                else if (i < argc+1)
                                        CONFIG_DIR = argv[i+1];
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


			case 'T':
			        if (*++s) {
			            datetime = s;
			        } else if (++i < argc)
			            datetime = argv[i];
				else {
				    fprintf(stderr, "Date-time must follow T option.\n");
				    goto usage;
			        }
				if (mail_date(&datetime, &dip_time, 1, stderr, 0)) {
				    fprintf(stderr, "Invalid date-time %s specified.\n", datetime);
				    goto usage;
			        }
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

			case ' ':
				break; /* Ignore spaces */

			default:
				goto usage;
		    }
		} else {
		      usage:
			fprintf(stderr, "Usage: [/directory/]%s [-C <directory>] [-c<CONFIG>=<value>] [-d<date>] [-aADqvx] [-sseq] [-tvar] [-d directory] [-i<filename>] [-r name]\n", nflg);
			fprintf(stderr, "  The directory specifies where we'll find our data.\n");
			fprintf(stderr, "  -a Don't mess with the at queue.\n");
			fprintf(stderr, "  -A Don't remove anything from the at queue.\n");
			fprintf(stderr, "  -C <sudirectory> Directory where dip.conf is.\n");
			fprintf(stderr, "  -c<CONFIG>=<value> Set <CONFIG> variable to <value>.\n");
			fprintf(stderr, "  -D increments the debugging flag.\n");
			fprintf(stderr, "  -d change current directory.\n");
			fprintf(stderr, "  -i Use <filename> for input.\n");
			fprintf(stderr, "  -q quick mode, just process mail.\n");
			fprintf(stderr, "  -T Used <date-time> string as current date & time.\n");
			fprintf(stderr, "  -t force variant var.\n");
			fprintf(stderr, "  -v Verbose, issue all error messages.\n");
			fprintf(stderr, "  -x no input, don't read stdin for mail.\n");
			fprintf(stderr, "  -r Original name for randomizer (Machiavelli).\n");
			fprintf(stderr, "  -s specifies test mode and sequence.\n");
			fprintf(stderr, "  -t specifies test mode and variant.\n");
			exit(E_FATAL);
		}
	}

	/* Reset the random seed */
	time(&now);
	srand(now);


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

	/* OK, now see if we're supposed to use block file */

	t = BLOCK_FILE;
	if (t[0]) {
	    if (!stat(t, &sbuf)) {
		fprintf(stderr, 
			"Block File %s exists - previous dip crashed.\n",
			t);
		bailout(E_FATAL);
	    }

	    fptr = fopen(t,"w");
	    time(&now);
	    fprintf(fptr, ctime(&now));
	    fclose(fptr);
	}

	time(&now);
	fprintf(log_fp, "%15.15s: dip -%s%s%s\n", ctime(&now) + 4,
		Aflg ? "A" : "", qflg ? "q" : "", xflg ? "x" : "");

	if ((fd = open("dip.ded", O_RDONLY)) < 0)
		if ((fd = open("dip.dedicate", O_RDONLY)) < 0)
			nded = 0;
		else {
			int i, dedicate[MAXUSER];
			nded = read(fd, dedicate, MAXUSER * sizeof(int)) / sizeof(int);
			for (i = 0; i <= nded; i++) {
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
				if (dipent.phase[6] != 'X' ) 
	                                CheckRemindPlayer(i, dipent.deadline - now);
                        }
                    }
                }

		if (time_warp) {
			/* A global time-warp was detected: inform all found masters of this */
			ibmfp = fopen(WARP_FILE, "w");
			if (ibmfp != NULL) {
			    fprintf(ibmfp, "A time warp was detected on the %s judge. Please check that",JUDGE_CODE);
			    fprintf(ibmfp, " the deadline for %s is correct, and inform players accordingly.\n",dipent.name);
			    fclose(ibmfp);

                           for (i = 0; i < dipent.n; i++) {
                            if (dipent.players[i].power < 0)
                                continue;
			    if (dipent.players[i].power == MASTER && 
				!(dipent.players[i].status & SF_RESIGN)) {
                                sprintf(line,
                                        "%s '%s:%s - %s Diplomacy time-warp'",
                                         WARP_FILE, JUDGE_CODE, dipent.name, dipent.phase);
				MailOut(line, dipent.players[i].address);
			      }
  
			    }
			}
		}

		if (bailout_recovery)
		{
		    /* Judge is recovering from a bailout: inform all found masters of this */
		    ibmfp = fopen(WARP_FILE, "w");
		    if (ibmfp != NULL) 
		    {
			fprintf(ibmfp, "The %s judge is recovering from a bailout. Please check that", JUDGE_CODE);
			fprintf(ibmfp, " the deadline for %s is correct, and inform players accordingly.\n",dipent.name);
			fclose(ibmfp);

                        for (i = 0; i < dipent.n; i++) 
			{
                            if (dipent.players[i].power < 0)
                                continue;
			    if (dipent.players[i].power == MASTER && 
				!(dipent.players[i].status & SF_RESIGN)) 
			    {
				sprintf(line, "%s '%s:%s - %s Bailout recovery'",
		  			WARP_FILE, JUDGE_CODE, dipent.name, dipent.phase);
				MailOut(line, dipent.players[i].address);
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
			} else if (GAME_PAUSED && (dipent.wait < now)) {
			    ibmfp = fopen(WARP_FILE, "w");
                            if (ibmfp != NULL) {
                                fprintf(ibmfp, 
				"Game is paused - master must send the 'resume' command to resume it or 'terminate' to finish.\n\n");
                                fclose(ibmfp);

                               for (i = 0; i < dipent.n; i++) {
                                if (dipent.players[i].power < 0)
                                    continue;
				if (dipent.players[i].controlling_power != 0) continue;
                                if (!(dipent.players[i].status & SF_RESIGN)) {
                                    sprintf(line,
                                        "%s 'Game pause reminder: %s'",
                                         WARP_FILE, dipent.name);
				    MailOut(line, dipent.players[i].address);
				}
                              }
			      dipent.process = now + 3 * 24 * 60 * 60; /* Remind every 3 days that game is paused */
			      dipent.wait = dipent.process;
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
	sprintf(line, "%s %s %2.2s%2.2s %6.6s %4.4s", ATRUN_CMD,
		Aflg ? "norm" : "dorm", s + 11, s + 14, s + 4, s + 20);
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
void CheckRemindPlayer(int player, long one_quarter)
{
    int num_hours;
    char *temp_file = "dip.temp";
    char line[150];
    char *pchar;
    time_t now;

    if(dipent.phase[6] == 'X') return; 
    if (!WAITING(dipent.players[player].status) ) return; /* Not waiting for a move */

    if (dipent.players[player].status & SF_REMIND) return; /* Already been reminded */

    if (dipent.players[player].controlling_power != 0) return; /* only players */

    num_hours = (int)  (one_quarter / (60 * 60));
    num_hours++;
    if (num_hours != 1) 
	pchar = "s ";
    else
	pchar = " ";

    if ((rfp = fopen(temp_file, "w")) == NULL) {
	 perror(temp_file);
         exit(E_FATAL);
    }

	msg_header(rfp);

/*
 *	code added to fix bug where error messages
 *	for other players are displayed if there have been
 *	no intervening signons
 */

	sprintf(Tfile, "%s%s/T%s", GAME_DIR, dipent.name, dipent.seq);
	if ((ofp = fopen(Tfile, "w")) == NULL) {
		fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
	}
	sprintf(Mfile, "%s%s/M%s", GAME_DIR, dipent.name, dipent.seq);
	if ((tfp = fopen(Mfile, "r")) != NULL) {
		while (fgets(line, sizeof(line), tfp)) {
			if (!strcmp(line, "X-marker\n"))
				*line = 'Y';
			fputs(line, ofp);
		}
		fclose(tfp);
	}

	fputs("X-marker\n", ofp);
	fclose(ofp);

	if (rename(Tfile, Mfile)) {
		fprintf(rfp, "Error renaming %s to %s.\n", Tfile, Mfile);
	}

/*
 * end added code
 */

	porder('M', player, 0); /* Call this to send copy of player's actual orders */
	fprintf(rfp,"\n\nThere is less than %d hour%sto the deadline", num_hours, pchar);

	if ((dipent.players[player].status) &  SF_PART) {
		fprintf(rfp, " but you\nneed to clear your error status with a valid message.\n");
	} else {
		fprintf(rfp," and you\nhave not yet placed complete and valid moves.\n");
	}

	fprintf(rfp,"\nPlease do so before you are marked as late.\n\n");
	now=time(NULL);

        if(dipent.phase[6] != 'X')
        {
		if(now < dipent.deadline)
	   		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
		if(now < dipent.grace)
			fprintf(rfp, "Time to grace period expiration: %s.\n",timeleft(&dipent.grace));
	}
	fclose(rfp);
	msg_header_done = 0;  /* Bug 282, header will need to be redone */

	sprintf(line, "%s '%s:%s - %s Reminder'",
	  temp_file, JUDGE_CODE, dipent.name, dipent.phase);

	dipent.players[player].status |= SF_REMIND;

	if (*(dipent.players[player].address) != '*' &&
	    !(dipent.players[player].status & SF_RESIGN)) {
		MailOut(line, dipent.players[player].address);
		sprintf(line,"Move reminder sent to %s in game %s", dipent.players[player].address, dipent.name);
		DIPINFO(line);
	}
}

/***********************************************************************/
int process(void)
{
	int i, n;
/*	int latecnt = 0;*/
	int dedtest; /* Buffer variable for dipent.dedapplied. */
	time_t now, then;
	FILE *dfp, *gfp,
	*lfp, *mlfp; /* late file pointers */
	char line[150];
	char pppath[70];
	char ppline[200];
	char title_text[150];
	char phase[sizeof(dipent.phase)];
	static char *dedfmt = "Adding %d to user %d's dedication to yield %d.\n";
	char late[51];
	struct stat ppinfo;
	int one_printed = 0;
	int vic, vic_count;
	static char vic_string[MAXPLAYERS+1];
	char temps[2];
	int just_now_abandoned = 0;  /* Set to non-zero if at least one power hsa just abandoned */

	if (GAME_PAUSED) return 0;  /* game is paused, do not process! */

	fprintf(log_fp, "Processing game '%s'.\n", dipent.name);
	rfp = NULL;
	lfp = NULL;
	mlfp = NULL;
	time(&now);

	/* Set dedtest equal to dipent.dedapplied. This keeps dipent.dedapplied
	** from being updated until the Judge finishes processing the game.
	** At that point dipent.dedapplied will be set to dedtest.
	*/
	dedtest = dipent.dedapplied;


	if (now < dipent.grace) {
		for (n = 0, i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;
			if (dipent.flags & F_INTIMATE && dipent.players[i].controlling_power != 0)
				continue;  /* Don't warn about non players in intimate */

			if (now < dipent.deadline && (dipent.players[RealPlayerIndex(i)].status & SF_WAIT)) {
				dipent.process = dipent.deadline;
				return 0;
			}
			if (WAITING(dipent.players[RealPlayerIndex(i)].status)) {
				if (now < dipent.deadline) {
					dipent.process = dipent.deadline;
					return 0;
				}
				dipent.process = dipent.grace;
				while ((then = dipent.process - 24 * 60 * 60) > now)
					dipent.process = then;

				if (dipent.players[RealPlayerIndex(i)].status & SF_PART &&
				  now >= dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60)) {
					dipent.players[RealPlayerIndex(i)].status |= SF_MOVED;
				} else {
					if (!n++) {
						if (Dflg) {
							lfp = stdout;
							mlfp = stdout;
						}
						else {
							if (!(lfp = fopen("dip.late", "w"))) {
								perror("dip_process: dip.late");
								bailout(1);
							} else {
								msg_header(lfp);

								if (dipent.x2flags & X2F_SECRET) {
									fprintf(lfp, "Diplomacy game '%s' is waiting for orders from one or more powers",
										dipent.name);
									one_printed++;
								}
							}

							if (!(mlfp = fopen("dip.mlate", "w"))) {
								perror("dip_process: dip.mlate");
								bailout(1);
							} else {
								msg_header(mlfp);
							}
						}
					}

					if (!(dipent.x2flags & X2F_SECRET)) {
						fprintf(lfp, "Diplomacy game '%s' is waiting for %s's orders",
							dipent.name, dipent.flags & F_QUIET ? "some power"
							: powers[dipent.players[i].power]);
						one_printed++;
					}

					fprintf(mlfp, "Diplomacy game '%s' is waiting for %s's orders",
						dipent.name, powers[dipent.players[i].power]);


					late[n-1] = (dipent.flags & F_QUIET ? '?' : dipent.pl[dipent.players[i].power]);

					if (!(dipent.players[RealPlayerIndex(i)].status & SF_LATE)) {
					    dipent.players[i].status |= SF_LATE;
					    dipent.players[i].late_count++; /* bump up the late count */
					}
					if (dipent.xflags & XF_LATECOUNT) {
						if (!(dipent.x2flags & X2F_SECRET))
							fprintf(lfp, ": %d time%s late",
								dipent.players[RealPlayerIndex(i)].late_count,
								dipent.players[RealPlayerIndex(i)].late_count == 1 ? "" : "s" );
						fprintf(mlfp, ": %d time%s late",
							dipent.players[RealPlayerIndex(i)].late_count,
							dipent.players[RealPlayerIndex(i)].late_count == 1 ? "" : "s" );
					}
					if (one_printed) {
					    fprintf(lfp,".\n");
					    one_printed = 0;
					}
					fprintf(mlfp,".\n");
				}
			}
		}

		if (n) {
			late[n] = '\0';
			then = dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60);

			if (dipent.x2flags & X2F_SECRET) {
				fputs("\nThese powers", lfp);
				fputs(now < then ? " will be" : " are now", lfp);
			} else {
				fputs(n == 1 ? "\nThis power" : "\nThese powers", lfp);
				fputs(now < then ? " will be" : n == 1 ? " is now" : " are now", lfp);
			}

			fputs(" considered abandoned and free for takeover", lfp);

			fputs(n == 1 ? "\nThis power" : "\nThese powers", mlfp);
			fputs(now < then ? " will be" :
			      n == 1 ? " is now" : " are now", mlfp);
			fputs(" considered abandoned and free for takeover", mlfp);

			if (now >= then) {
				if (dipent.flags & F_NONMR) {
					fputs(".\n", lfp);
					fputs(".\n", mlfp);
				} else {
					if (dipent.x2flags & X2F_SECRET) {
						fputs(".\nThey", lfp);
					} else {
						fputs(n == 1 ? ".\nIt" : ".\nThey", lfp);
					}

					fputs(" will be considered in civil disorder if orders are ", lfp);
					fprintf(lfp, "not received\nby %s.\n", ptime(&dipent.grace));

					fputs(n == 1 ? ".\nIt" : ".\nThey", mlfp);
					fputs(" will be considered in civil disorder if orders are ", mlfp);
					fprintf(mlfp, "not received\nby %s.\n", ptime(&dipent.grace));
				}
				n = -1;
			} else {
				fprintf(lfp, "\nif orders are not received by %s.\n", ptime(&then));
				fprintf(mlfp, "\nif orders are not received by %s.\n", ptime(&then));
				n = now < dipent.deadline + 24 * 60 * 60 ? 1 : 0;
			}


			if (!Dflg) {
				fclose(lfp);
				fclose(mlfp);
			}

			for (i = 0; i < dipent.n; i++) {
				register int d, w;

				if (dipent.players[i].power < 0)
					continue;

				if ((w = WAITING(dipent.players[RealPlayerIndex(i)].status)) || n) {
					if (w && n == -1) {
						if (!(dipent.players[i].status & SF_ABAND)) {
							dipent.players[i].status |= SF_ABAND;
							just_now_abandoned++;
							if (!(dipent.flags & F_NORATE) &&
							    dipent.players[i].controlling_power == 0) {
								ded[dipent.players[i].userid].r += D_ABANDON;
								put_data(dipent.players[i].userid,resigned);
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
					if (dipent.players[i].controlling_power == 0) {
#ifdef NORMDED
					    if (!(dipent.flags & F_NORATE)) {
						d = n == 1 ?
						    (!w && (dipent.players[i].status & SF_MOVE) ? D_ONTIME : 0)
						    : (w ? D_LATE : 0);
						ded[dipent.players[i].userid].r += d;
						fprintf(log_fp, dedfmt, d, dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					    }

					    if(n == 1 && dipent.dedapplied == 0 && (!(dipent.flags & F_NORATE)))
					    {
						if(w)
						{
							put_data(dipent.players[i].userid,total);
						}
						else
						{
							put_data(dipent.players[i].userid,ontime);
							put_data(dipent.players[i].userid,total);
						}
						dedtest = 1;
					    }
#else
					    d = 0;
					    if(dipent.dedapplied == 0 && n == 1)
					    {
						if(dipent.players[i].status & SF_MOVE)
						{
							if(w)
							{
								d = D_LATE;
								put_data(dipent.players[i].userid,total);
							}
							else
							{
								d = D_ONTIME;
								put_data(dipent.players[i].userid,ontime);
								put_data(dipent.players[i].userid,total);
							}
							dedtest = 1;
						}
						ded[dipent.players[i].userid].r += d;
						fprintf(log_fp,dedfmt,d,dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					    }
					}
#endif
					if (*(dipent.players[i].address) != '*' &&
					    !(dipent.players[i].status & SF_RESIGN)) {
						if (dipent.players[i].power == MASTER) {
							sprintf(line, "dip.mlate '%s:%s - %s Late Notice: %s'",
								JUDGE_CODE, dipent.name, dipent.phase, late);
							MailOut(line, dipent.players[i].address);
						} else {
							sprintf(line, "dip.late '%s%s:%s - %s Late Notice: %s'",
								 w ? "[You are late!] " : "", JUDGE_CODE, dipent.name, dipent.phase,
									(dipent.x2flags & X2F_SECRET) ? "?" : late);
							MailOut(line, dipent.players[i].address);
						}
					}
				}
			}
			if (n) {
				sprintf(line, "%s:%s - %s Late Notice: %s", JUDGE_CODE, dipent.name, dipent.phase, (dipent.x2flags & X2F_SECRET) ? "?" : late);
				archive("dip.late", line);
			}
			if (n == -1) {
				pprintf(cfp, "%sGrace period for '%s' expires %s.\n",
					NowString(), dipent.name, ptime(&dipent.grace));
			}
			dipent.dedapplied = dedtest;
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
		i = dipent.powers + '0' - dipent.seq[1];
		if (dipent.xflags & XF_MANUALSTART && i <= 0) {
			sprintf(subjectline, "%s:%s - Waiting for Master to Start", JUDGE_CODE, dipent.name);
			fprintf(rfp, "Diplomacy game '%s' is still waiting for master to start it.\n", dipent.name );
			pprintf(cfp, "%sDiplomacy game '%s' is still waiting for master to start it.\n", NowString(), dipent.name);
			sprintf(title_text, "Diplomacy game %s startup waiting", dipent.name);
		} else {
			sprintf(subjectline, "%s:%s - Waiting for More Players", JUDGE_CODE, dipent.name);
			if (dipent.x2flags & X2F_SECRET) {
			    sprintf(subjectline, "%s:%s - Waiting for More Player(s)", JUDGE_CODE, dipent.name);
			    fprintf(rfp, "Diplomacy game '%s' is still waiting for some player(s) to sign on.\n", dipent.name);
			} else {	
			    sprintf(subjectline, "%s:%s - Waiting for %d More Player%s", JUDGE_CODE, dipent.name, i, i == 1 ? "" : "s");
			    fprintf(rfp, "Diplomacy game '%s' is still waiting for %d player%s to sign on.\n", dipent.name, i, i == 1 ? "" : "s");
			}
			pprintf(cfp, "%sDiplomacy game '%s' is still waiting for %d player%s to sign on.\n", NowString(), dipent.name, i, i == 1 ? "" : "s");
			sprintf(title_text, "Diplomacy game %s signup waiting", dipent.name);
		}
		if (!Dflg)
		{
			now=time(NULL);   
			if(dipent.phase[6] != 'X')
			{
                		if(now < dipent.deadline)
                        		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                		if(now < dipent.grace)
                 	       		fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
			}
			fclose(rfp);
		        msg_header_done = 0;  /* Bug 282, header will need to be redone */

		}
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (*(dipent.players[i].address) != '*' &&
			    !(dipent.players[i].status & SF_RESIGN) &&
			     RealPlayerIndex(i) == i) {
				sprintf(line, "dip.result '%s'", subjectline);
				MailOut(line, dipent.players[i].address);
			}
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
		/* Dietmar Kulsch change 10/10/2000 to avoid players going
		 * abandoned when they submit error orders
		 */
				if ((!(dipent.players[RealPlayerIndex(i)].status & SF_PART)) && WAITING(dipent.players[RealPlayerIndex(i)].status)) {
					dipent.players[i].status |= SF_CD;
					just_now_abandoned++;
					if (!(dipent.flags & F_NORATE) &&
					    RealPlayerIndex(i) == i) {
						put_data(dipent.players[i].userid,resigned);
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
		if (!(dipent.flags & F_NORATE) && now < dipent.deadline + 60 * 60 && dipent.dedapplied == 0) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status & SF_MOVE) &&
				    !(dipent.players[i].status & SF_CD) &&
				    RealPlayerIndex(i) == i) {
					put_data(dipent.players[i].userid,ontime);
					put_data(dipent.players[i].userid,total);
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

				if ((dipent.players[RealPlayerIndex(i)].status &
				     (SF_MOVE | SF_MOVED | SF_PART)) == SF_MOVE) {
				    /* If intimate, only show real players abandoned */
				    if (!((dipent.flags & F_INTIMATE) && (dipent.players[i].controlling_power != 0))) {
					if (!(dipent.x2flags & X2F_SECRET)) {
						fprintf(rfp, "Diplomacy game '%s' is waiting for someone ", dipent.name);
						fprintf(rfp, "to take over the abandoned %s.\n",
						  dipent.flags & F_QUIET ? "power" :
						  powers[dipent.players[i].power]);
					}
					pprintf(cfp, "%sDiplomacy game '%s' is waiting for someone ", NowString(), dipent.name);
					pprintf(cfp, "to take over the abandoned\n");
					pprintf(cfp, "%s with %d of %d units on the board (%s).\n",
						powers[dipent.players[i].power], dipent.players[i].units,
						dipent.players[i].centers, dipent.phase);

					if (!(dipent.x2flags & X2F_SECRET) || n == 0) {

					    late[n] = (dipent.flags & F_QUIET || dipent.x2flags & X2F_SECRET ? '?' : dipent.pl[dipent.players[i].power]);
					    n++;
					}
				    }
				}
			}

			/* Only send warning if process time passed */
			if (n && dipent.process < now ) {

			    dipent.process = now + 48 * 60 * 60;

			    /* Only send warnings if two days since last one */
			    /* or if at least one player has just abandoned */
			    if (dipent.wait < now || just_now_abandoned) {

				late[n] = '\0';

				if (dipent.x2flags & X2F_SECRET) {
					fprintf(rfp, "Diplomacy game '%s' is waiting for someone ", dipent.name);
					fprintf(rfp, "to take over the abandoned power(s).\n");
				}

				dipent.wait = dipent.process;

				if (!Dflg)
				{
					if(signedon && (dipent.phase[6] != 'X'))
					{
						now=time(NULL);   
			                	if(now < dipent.deadline)
                        				fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                				if(now < dipent.grace)
                        				fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
					}
					fclose(rfp);
				        msg_header_done = 0;  /* Bug 282, header will need to be redone */

				}
				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;

					if (*(dipent.players[RealPlayerIndex(i)].address) != '*' && !Dflg &&
					    !(dipent.players[RealPlayerIndex(i)].status & SF_RESIGN) &&
					    RealPlayerIndex(i) == i) {
						sprintf(line, "dip.result '%s:%s - %s %s Waiting for Replacements: %s'",
							JUDGE_CODE, dipent.name, dipent.phase,
							(dipent.flags & F_NOLIST) ? "NoList" : "",
							(dipent.x2flags & X2F_SECRET) ? "?" : late);
						MailOut(line, dipent.players[i].address);
					}
				}
			    }
			    dipent.dedapplied = dedtest;
			    return 0; /* Bug 347, always return even if no warnings are sent! */
			}
		}
		/*
		 * See if manual processing is enabled, and block if so
                 */
		if (dipent.xflags & XF_MANUALPROC) {
			if (!(dipent.players[0].status & SF_PROCESS)) {
				fprintf(rfp, "Game '%s' is waiting for master to process turn.\n", dipent.name);
				if (!Dflg) {
				    fclose (rfp);
			            msg_header_done = 0;  /* Bug 282, header will need to be redone */
				}

				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;
					sprintf(line, "dip.result '%s:%s - %s Turn Waiting'",
					  JUDGE_CODE, dipent.name, dipent.phase);

					if (*(dipent.players[RealPlayerIndex(i)].address) != '*' &&
					    !(dipent.players[RealPlayerIndex(i)].status & SF_RESIGN) &&
					      RealPlayerIndex(i) == i)
						MailOut(line, dipent.players[RealPlayerIndex(i)].address);
				}
				dipent.process = now + 24 *60 *60;  /* Remind each day */
				dipent.dedapplied = dedtest;
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
			if (!Dflg) {
				fclose(rfp);
        			msg_header_done = 0;  /* Bug 282, header will need to be redone */
			}
			sprintf(line, "dip.result 'Diplomacy error'");
			MailOut(line, GAMES_MASTER);
			bailout(1);
		}

		pprintf(cfp, "%sPhase %s processed for game '%s'.\n", NowString(), phase, dipent.name);

		/*
		   **  Remove any draw/win information (for summary) if it exists
		 */

		sprintf(line, "%s%s/draw", GAME_DIR, dipent.name);
		remove(line);

		if (dipent.phase[6] == 'X') {
			sprintf(line, "%s%s/draw", GAME_DIR, dipent.name);
			if ((dfp = fopen(line, "w")) == NULL) {
				fprintf(log_fp, "dip: Error opening draw file.\n");
				bailout(E_FATAL);
			}
			if ((ofp = fopen("dip.temp", "w")) == NULL) {
				fprintf(log_fp, "process: Error opening second temporary file.\n");
				bailout(E_FATAL);
			}
			if ((gfp = fopen("dip.victory", "w")) == NULL) {
				fprintf(log_fp, "dip: Error opening victory file.\n");
				bailout(E_FATAL);
			}

			msg_header(ofp);

			pprintf(cfp, "%sGame '%s' completed.\n", NowString(), dipent.name);
			fprintf(rfp, "\nThe game is over.  Thank you for playing.\n");
			fprintf(ofp, "Game '%s' has ended in a victory for ", dipent.name);
			time(&now);
			fprintf(dfp, "Game won: %s\n", ctime(&now));
			fprintf(dfp, "The game was won by ");
			fprintf(gfp, "The game was won by ");

			/*  Find out who has won */

		
			vic_string[0] = '\0';
			for (vic_count = 0; vic_count < victor; vic_count++)
			{
			    vic = victors[vic_count];
			    if (vic_count > 0 && vic_count + 1 < victor ) {
		                fprintf(ofp, ", ");
                                fprintf(dfp, ", ");
                                fprintf(gfp, ", ");
			    } else if ( vic_count > 0 && vic_count + 1 >= victor) {
				fprintf(ofp, " and ");
				fprintf(dfp, " and ");
	                        fprintf(gfp, " and ");
			    }
			    fprintf(ofp, "%s", powers[vic]);
			    fprintf(dfp, "%s", powers[vic]);
			    fprintf(gfp, "%s", powers[vic]);
			    sprintf(temps, "%c", dipent.pl[vic]);
			    strcat(vic_string, temps);
			}
			fprintf(ofp, ".\n\n");
                        fprintf(dfp, ".\n");
                        fprintf(gfp, ".\n");


			fprintf(gfp, "Congratulations on a well deserved victory!\n");

			/*  If game is standard, it has a Boardman Number, if not
			   it has a Miller Number.  Check to see if it is an EP
			   game.  Alert Custodians to victory */
			fclose(ofp);
			fclose(dfp);
			fclose(gfp);

			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if (*(dipent.players[i].address) != '*' && !Dflg && 
				    RealPlayerIndex(i) == i ) {
					sprintf(line, "dip.victory '%s:%s - %s Victory: %s'",
					  JUDGE_CODE, dipent.name, phase, vic_string);
					MailOut(line, dipent.players[i].address);
				}
			}

			{
				InformCustodians(dipent.name,
						 "%s '%s: Victory in game %s'",
						 dipent.variant, 
						 dipent.flags & F_GUNBOAT);
			}

			/*
			 * This seems as good a place as any to send out EOG diaries
			 */
			send_diary();

			/*
			 * Force regeneration of the summary file if it's a gunboat game.
			 */

			if (dipent.flags & F_GUNBOAT) {
				sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
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

			sprintf(line, "%s%s/summary 'HoF: Victory in %s'",
				GAME_DIR, dipent.name, dipent.name);
			MailOut(line, HALL_KEEPER);

		} else {
			deadline((sequence *) NULL, 1);

			fprintf(rfp, "\nThe next phase of '%s' will be %s for %s of %4.4s.\n",
				dipent.name,
				dipent.phase[5] == 'M' ? "Movement" :
				dipent.phase[5] == 'R' ? "Retreats" : 
				dipent.phase[5] == 'A' ? "Bids" : "Adjustments",
				dipent.phase[5] == 'A' ? "Year" :
				dipent.phase[5] == 'B' ? "Winter" :
				dipent.phase[0] == 'F' ? "Fall" :
				dipent.phase[0] == 'U' ? "Summer" : "Spring", dipent.phase + 1);
			if (broadcast_absence_adjust)
			{
			    fprintf(rfp,"Requested absence(s) activated.\n");
			    broadcast_absence_adjust = 0;
			}
			fprintf(rfp, "The deadline for orders will be %s.\n",
				ptime(&dipent.deadline));
			if ((dipent.phase[5] == 'B' && 
				 (dipent.x2flags & X2F_MORE_HOMES)) ||
				 (dipent.phase[5] == 'A')) {
			    /* game allows home centre assignments */
			    /* Thus set active players in a wait state */
			    for  (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;
					if (dipent.players[RealPlayerIndex(i)].power < WILD_PLAYER &&
					  !(dipent.players[RealPlayerIndex(i)].status & SF_DEAD) &&
					    dipent.players[RealPlayerIndex(i)].controlling_power == 0) {
					/* A real player, set wait status */
						dipent.players[RealPlayerIndex(i)].status |= SF_WAIT;
					}
			    }
			}
		}

		if (!Dflg)
		{
			if(signedon && (dipent.phase[6] != 'X'))
			{
				now=time(NULL);   
                		if(now < dipent.deadline)
                        		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                		if(now < dipent.grace)
		                        fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
			}
			fclose(rfp);
		        msg_header_done = 0;  /* Bug 282, header will need to be redone */

		}
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

/*
 * This looks as good a place as any to clear draw flags...
 *    (Pãositron, 11 Mar 1993)
 * And concession flags (Tim Miller 12 Aug 2001)
 */
			dipent.players[i].status &= ~SF_CONC;
			dipent.players[i].status &= ~SF_DRAW;

			if (!(dipent.flags & F_BLIND) || (dipent.players[i].power == MASTER)) 
			{
				sprintf(line, "dip.result '%s:%s - %s Results'",
				  JUDGE_CODE, dipent.name, phase);
			}

		        /* let's do postal press */
			/* TODO: tidy this code up */ 
			sprintf(pppath, "%s%s/ppress-%s", GAME_DIR, dipent.name, phase);
		        if((dipent.x2flags & X2F_POSTALPRESS) && (stat(pppath, &ppinfo) != -1))
	    		    sprintf(ppline, "%s%s/ppress-%s '%s:%s - %s game press'",
	        		GAME_DIR, dipent.name, phase,
				JUDGE_CODE, dipent.name, phase);

			if (*(dipent.players[i].address) != '*' && !Dflg &&
					    !(dipent.players[i].status & SF_RESIGN) &&
					    RealPlayerIndex(i) == i) 
			{
			    if(!(dipent.flags & F_BLIND) || dipent.players[i].power == MASTER)
				MailOut(line, dipent.players[i].address);
			    if((dipent.x2flags & X2F_POSTALPRESS) && (stat(pppath, &ppinfo) != -1))
				MailOut(ppline, dipent.players[i].address);
			}

			if (dipent.flags & F_BLIND) {
			    /* Ooops, it's a blind variant */
			    /* Special routine to work out what to tell who */
			    inform_party_of_blind_turn(i, phase,"dip.result");
			}
		}

		if (!strcmp(dipent.seq, "002") && !(dipent.flags & F_BLIND)) {
			sprintf(line, "dip.result 'Diplomacy results %s %s'",
				dipent.name, phase);
			MailOut(line, GAMES_OPENER);
		}
		sprintf(line, "%s:%s - %s Results", JUDGE_CODE, dipent.name, phase);
		archive("dip.result", line);

		phase_pending();
		deadline((sequence *) NULL, 0);
	}
	
	return 0;		/* reached ? */
}
/***********************************************************************/
