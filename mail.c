/*
 * $Log$
 * Revision 1.65  2004/03/28 09:52:13  millis
 * Fix bug 282 (reset msg_header_done on closing rfp file)
 *
 * Revision 1.64  2003/12/20 07:05:30  nzmb
 * Changed judge so that the original message sent to it is echoed back only
 * when either the sender is registered with a judge or it contains a valid
 * command such as REGISTER. This is to solve judges crashing with smail errors
 * when smail tries to interpret binary files as part of the e-mail message.
 *
 * Also, when a message is echoed back, added code to strip off the headers.
 *
 * Revision 1.63  2003/12/12 10:16:13  millis
 * Fixed compile error
 *
 * Revision 1.62  2003/12/11 13:18:01  millis
 * Removed tests for msg_header_done.
 *
 * Revision 1.61  2003/12/11 13:11:45  millis
 * In msg_header(), exit if msg_header_done is set (avoid tests everywhere)
 *
 * Revision 1.60  2003/12/07 00:52:08  millis
 * Fix bug 217
 *
 * Revision 1.59  2003/11/09 18:59:42  millis
 * Fix bug 145
 *
 * Revision 1.58  2003/09/14 00:18:28  millis
 * Fix bug 223
 *
 * Revision 1.57  2003/09/09 19:57:09  nzmb
 * Fixed Bug 31 -- the time to deadline/grace expiration messages are no
 * longer printed for terminated games, and neither are the "orders not
 * received ... you will be considered late/abandoned" message. The list of
 * entered orders is still displayed, though.
 *
 * Revision 1.56  2003/08/25 14:39:36  millis
 * Fixed bug 220
 *
 * Revision 1.55  2003/07/22 00:13:15  millis
 * Fix bug 198
 *
 * Revision 1.54  2003/07/20 08:38:34  millis
 * Removed incorrect execute(line) calls
 * Also deleted some unused variables
 *
 * Revision 1.53  2003/07/19 13:11:59  millis
 * Bug 197, remove map command
 *
 * Revision 1.52  2003/07/17 00:04:00  millis
 * *** empty log message ***
 *
 * Revision 1.51  2003/07/15 22:47:07  millis
 * Fix Bug 185 (call smail for each email individually)
 *
 * Revision 1.50  2003/07/15 16:16:48  millis
 * Make null mail 'nobody'
 *
 * Revision 1.49  2003/07/12 17:47:38  nzmb
 * Fix so that lenlimit gets called on outgoing press messages, not just
 * replies.
 *
 * Revision 1.48  2003/06/22 04:10:20  nzmb
 * Added code to allow users to record diary entries, read them, and delete them
 * if they make a mistake. The diaries will be broadcast when the games end (this
 * part is not as of now, yet written).
 *
 * Revision 1.46  2003/05/24 23:57:14  millis
 * Bug 97, removed Generic Handling code
 *
 * Revision 1.45  2003/05/14 07:59:18  nzmb
 * Fixed bug #122 -- splits dip.reply into part created before and after
 * the GM issues a "become" command.
 *
 * Revision 1.44  2003/05/13 07:23:58  nzmb
 * Fixed bug #48 by limiting to 100 the number of infoplayer request that can be sent in one e-mail
 *
 * Revision 1.43  2003/05/12 23:23:46  millis
 * Fix bug 133, allow turn to process when set to manualprocess and process command sent.
 *
 * Revision 1.42  2003/05/03 23:08:42  millis
 * Fixed bug 99 (resigned players still receiving messages)
 * Now resigned players will only receive the termination messages.
 *
 * Revision 1.41  2003/04/28 22:35:39  millis
 * Fixed missing define
 *
 * Revision 1.40  2003/04/27 15:45:03  millis
 * Fixed Bug 5 (allowing game to be Force begun without all players)
 *
 * Revision 1.39  2003/04/27 14:47:35  millis
 * Fix bug 141 (unintentional FORCE BEGIN)
 *
 * Revision 1.38  2003/03/31 08:19:51  nzmb
 * Fix to previous fix, should be two dashes, space, and newline counts as
 * signoff.
 *
 * Revision 1.37  2003/03/29 11:31:14  nzmb
 * Made two dashes (--) followed by a newline the moral equivalent of signoff
 * -- fixes bug #60.
 *
 * Revision 1.36  2003/03/28 21:30:34  nzmb
 * Added extra newline before grace period display -- resolves bug #121
 *
 * Revision 1.35  2003/03/21 20:25:04  russblau
 * Allow space in "sign off" when used to terminate a press message (to be
 * consistent with usage in other contexts).
 *
 * Revision 1.34  2003/03/08 23:25:26  russblau
 * Fixed minor spacing glitch in "set wait" notification.
 *
 * Revision 1.33  2003/03/07 21:24:08  russblau
 * Editorial changes.
 *
 * Revision 1.32  2003/02/17 12:41:41  millis
 * Fixed Bug 108, make lines >=1024 terminate in '\n'
 *
 * Revision 1.31  2003/01/30 22:49:11  millis
 * Fixed small error (SET not SETUP)
 *
 * Revision 1.30  2003/01/15 13:59:25  millis
 * Removed Dipstats
 *
 * Revision 1.29  2003/01/15 13:43:00  millis
 * Fixed small compiler errors
 *
 * Revision 1.28  2003/01/13 22:15:03  millis
 * ustv merge
 *
 * Revision 1.27  2003/01/07 23:41:36  millis
 * Fix bug 93
 *
 * Revision 1.26  2003/01/05 11:16:01  millis
 * Fixed bug 75, also make NOBROAD flag work correctly (at last!)
 *
 * Revision 1.25  2002/11/08 21:57:45  millis
 * fix bug 36, so that players not given blackist message if judge is
 * a private one.
 * Also, do not delete player's email on resignation
 *
 * Revision 1.24  2002/08/27 22:27:52  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.23  2002/07/16 18:14:21  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.22  2002/06/11 14:50:38  hauer
 * Reject conditional orders in Blind.
 *
 * Revision 1.21  2002/05/31 22:55:33  nzmb
 *
 * Fixed bug in mail.c that was causing the judge to do a segmentation fault
 * when trying to write to the xcontrol file.
 *
 * Revision 1.20  2002/05/17 11:36:52  miller
 * Parameters for address_not_in_list() incorrectly reverserd
 *
 * Revision 1.19  2002/05/11 09:15:32  greg
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
 * Revision 1.18  2002/05/04 02:06:18  nzmb
 * Added code to display the time left until the deadline and grace at the
 * bottom of their reply whenever a player signs on.
 *
 * Revision 1.17  2002/04/18 04:44:32  greg
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
 * Revision 1.16  2002/04/15 12:55:44  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.15  2002/01/08 21:22:01  miller
 * Redid GG fix for JUDGE_CODE != 4
 *
 * Revision 1.14  2001/12/29 20:38:04  nzmb
 *
 * Added infoplayer, record commands. Put judge version to 1.0.0 as we think it is
 * stable.
 *
 * Revision 1.12  2001/10/23 00:39:52  greg
 * Added subjectline for Promote command and fixed subjectline for Eject command to show power ejected - not master.
 *
 * Revision 1.11  2001/10/22 20:09:05  nzmb
 * Cw.c draw.c added chenges and fixes so summaries for draws and concessions are properly sent to the HALL_KEEPER address.
 *
 * Revision 1.9  2001/07/22 10:03:13  greg
 * subjectline tweaks
 *
 * Revision 1.8  2001/07/16 22:53:11  miller
 * Fixed EJECT command (was previously doing nothing!)
 *
 * Revision 1.7  2001/07/15 09:16:14  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.6  2001/07/14 07:40:17  greg
 * minor bug fix & put "[Error Flag]" in subjectline when appropriate
 *
 * Revision 1.5  2001/07/01 23:19:29  miller
 * Added predict and eject commands
 *
 * Revision 1.4  2001/05/11 07:18:02  greg
 * added subjectlines and allowed "signoff"
 * even if there's no valid "signon"
 *
 * Revision 1.3  2001/01/06 18:48:43  davidn
 * Changes to allow game creator to be automatically made master
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Innumerous changes, some of which are:
 *  - Add new command 'promote'
 *  - Resign command now needs email for master (and can resign observers)
 *  - Added calls to shorthand functions for SHORTHAND press games
 *  - set a player flag when a power resigns (for future use to tell resigned players game has ended.)
 *
 *  - Allow termination of unstarted games
 *  - Restrict games witn F_NORESUME to master-only resumation
 *  - Do not show password of player when doing a 'become' command
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
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

#include "config.h"
#include "dip.h"
#include "mail.h"
#include "functions.h"
#include "conf.h"
#include "diplog.h"
#include "plyrdata.h" /* Change Sep 23, 2001 */

static int junkmail = 0;	/* Non zero if no reply to be sent		*/
static int spammail = 0;	/* Mark messages as spam if they don't come from a valid user and don't contain a valid command */
static int command = 0;		/* Non zero if some intelligable command found	*/
static int skipping = 0;	/* Non zero if skipping rest of input		*/
static int movement = 0;	/* Non zero if movement orders received		*/
static char subject[80];	/* Reply mail subject				*/
static char line[1024];		/* Temporary line buffer			*/
static char temp[40];		/* Temporary file name				*/
static int press_number = 0;	/* Number for distinct press filenames 		*/
static int errorflag = 0;	/* Is the error flag set?			*/

static int InsertDummyPlayers( void );


/* set the max. infoplayer requests per e-mail here */
#define INFOPLAYER_MAX 100

char *mail_r2file;
#define r2file mail_r2file

#define FROM      	1
#define GET       	2
#define HELP      	3
#define LIST      	4
#define REPLY     	5
#define SET       	6
#define SIGNOFF   	7
#define SIGNON    	8
#define SUBJECT   	9
#define OBSERVE   	10
#define RESIGN    	11
#define BROADCAST 	12
#define PHASE     	13
#define CLEAR     	14
#define TERMINATE 	15
#define RESUME    	16
#define BECOME    	17
#define REGISTER  	18
#define WHOIS     	19
#define FIXID     	20
#define IAMALSO   	21
#define NOCONTROL 	22
#define ADJUST    	23
#define JUNKMAIL  	24
#define PROCESS   	25
#define ROLLBACK  	26
#define _VERSION   	27
#define CREATE    	28
#define SUMMARY   	29
#define HISTORY   	30
#define MAP       	31
#define PRESS     	32
#define RESENT    	33
/* #define DEDICATE  	34 */
#define WHOGAME   	35
/* #define DEDGAME   	36 */
#define PROMOTE   	37
#define PREDICT 	38
#define EJECT   	39
#define RECORD  	40
#define INFOPLAYER	41
#define UNSTART		42
#define SETUP		43
#define PAUSE		44
#define COND            45      /* -- Tamas -- 2002-06-11 -- */
#define POSTALPRESS	46
#define FORCE_BEGIN	47
#define DIARY           48

/* Note: in both prelim and commands below, a blank character means
 * that whitespace is optional in the user input; a '#' character
 * means that whitespace (or parentheses) is required in the user input.
 */

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
 "no control", "adjust",
 "version", "history",
 "who game#", "who is#", "who#", "fix id",
 "map", "sign off", "-- \n", "record", "info player",
 "if", "else", "endif"        /* -- Tamas -- 2002-06-11 -- */
 /* , "ded game#", "dedicate#",
	"ded#" */ };

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
 _VERSION, HISTORY,
 WHOGAME, WHOIS, WHOIS, FIXID,
 MAP, SIGNOFF, SIGNOFF, RECORD, INFOPLAYER, 
 COND, COND, COND                   /* -- Tamas -- 2002-06-11 -- */
 /* , DEDGAME, DEDICATE, DEDICATE */ };

static char *commands[] =
{"", "list", "help", "get", "send me", "send",
 "setup", "sign off", "-- \n", "resign", "withdraw",
 "broadcast", "press", "phase", "clear",
 "register", "i am also",
 "who game", "who is#", "who#",
 "version", "history",
 "summary", "status",
 "terminate", "resume", "become",
 "process", "roll back", "map",
 "promote", "predict",
 "eject", "record", "info player", "unstart",
 "set", "pause", "suspend", "postal press", 
 "force begin", 
 "if", "else", "endif",   
 "diary"
			     /* , "ded game", "dedicate#", "ded#" */ };

static int cvalue[] =
{0, LIST, HELP, GET, GET, GET,
 SETUP, SIGNOFF, SIGNOFF, RESIGN, RESIGN,
 BROADCAST, PRESS, PHASE, CLEAR,
 REGISTER, IAMALSO,
 WHOGAME, WHOIS, WHOIS,
 _VERSION, HISTORY,
 SUMMARY, SUMMARY,
 TERMINATE, RESUME, BECOME,
 PROCESS, ROLLBACK, MAP,
 PROMOTE, PREDICT,
 EJECT, RECORD, INFOPLAYER, UNSTART,
 SET, PAUSE, PAUSE, POSTALPRESS,
 FORCE_BEGIN,
 COND, COND, COND,
 DIARY
			     /* , DEDGAME, DEDICATE, DEDICATE */ };


void shiftleft(int dist) {
	int x = 0;

	do  {
		subject[x] = subject[x+dist];
		x++;
	} while (subject[x+dist-1] != '\0');
}


static int address_not_in_list(char *reply_address, char *players_addresses);

/* Resign passed player */
void ResignPlayer( int resign_index)
{
    if (any_broadcast && broad_part) {
        send_press();
        open_press();
    }
    if (!strcmp(dipent.players[player].address, "*")) {
        fprintf(rfp, "You are no longer a player in this game.\nYou may not resign again!\n\n");
        return;
    }

    dipent.players[resign_index].status |= SF_RESIGN;
    fprintf(rfp, "%s has resigned from game '%s'.\n\n",
        powers[dipent.players[resign_index].power], dipent.name);
    /* WAS mfprintf  1/95 BLR */
    sprintf(subjectline,
            "%s:%s - %s Resignation: %s",
            JUDGE_CODE,
            dipent.name,
            dipent.phase,
            powers[dipent.players[resign_index].power]);

    fprintf(bfp, "%s has resigned %s\nas %s in game '%s'.\n\n", xaddr,
       ((dipent.flags & F_GUNBOAT) &&
        (dipent.players[resign_index].power != MASTER))
        ? someone
        : dipent.players[resign_index].address,
        powers[dipent.players[resign_index].power], dipent.name);

    fprintf(mbfp, "%s has resigned %s\nas %s in game '%s'.\n\n", raddr,
        dipent.players[resign_index].address,
        powers[dipent.players[resign_index].power], dipent.name);


    broad_signon = 1;
    if (dipent.players[resign_index].power != OBSERVER) {
        pprintf(cfp, "%s%s has resigned in game '%s' (%s, %d of %d units).\n",
                NowString(),
                powers[dipent.players[resign_index].power], dipent.name, dipent.phase,
                dipent.players[resign_index].units, dipent.players[resign_index].centers);
        pcontrol++;
    }
    strcpy(dipent.players[resign_index].password, GOING_PWD);
 }

/*
 * Try to resign passed power (Master command)
 * retruns id in player array, else -1
 */

#define RP_INVALID (-1)
#define RP_MULTIPLE (-2)
#define RP_BLANK (-3)
#define RP_NOT_FOUND (-4)
/*
 * Try to resign passed power by email address (Master command)
 */
int FindByEmail(char *power_text)
{
        int i;
        int found = 0;
        int found_index = -1;

        for (i = 0; i < dipent.n; i++) {
                if (strstr(dipent.players[i].address,power_text) != NULL) {
                        found++;
                        found_index = i;
                }
        }

        switch (found) {
            case 0:  /* nothing found */
                found_index = RP_NOT_FOUND;
                break;

           case 1: /* only one found */
                break; /* and will return found index */

           default:
                found_index = RP_MULTIPLE;
	}
        return found_index;
}

/*
 * Try to resign passed power (Master command)
 * retruns id in player array, else -1
 */

int ResignPower(char *power_text)
{
	int i,n;
	int found = 0;
        int found_index = RP_INVALID; 

        if (GetOneWord(power_text) == 0) return RP_BLANK;
	
	/* If an '@' in string, it must be email, so check player addresses */
        if (strstr(power_text, "@") != NULL) return FindByEmail(power_text);
	
	get_power(power_text, &n);
	if (n==0) n = power(*power_text);
	if (0 >= n ) return RP_INVALID; /* bad return, cannot be no-one */
	/* See if this power is found */
	for (i = 0; i < dipent.n; i++) {
          if (dipent.players[i].power == n)
	  {
                  found++;
		  found_index = i;
	  }
	}

	switch (found) 
	{
		case 0:  /* No mtch found */
		return RP_INVALID;

		case 1:
		return found_index;
		
		default:
		return RP_MULTIPLE;
	}


	return 0==1; /* temp return */
}

/*
 *  Mail: Process a mail file sent to the Diplomacy judge.
 */

int mail(void)
{

	int i, j, k, l, n, got_reply, got_resent, not_eof, full;
	int ret = 0; /* return code for shorthand */
	int done_headers;
	char *s, *t;
	FILE *check, *termfp, *dfp, *qfp;
	long now;
	int resign_index;
	char *whotext;
	char x[30];
	char cmdline[64];
	PLYRDATA_RECORD record;
	static unsigned int no_of_infop = 0;
	time_t now_time;
	int temp1;

	someone = "someone@somewhere";

	starting = msg_header_done = 0;
	rfile = "dip.reply";
	r2file = "dip.gmreply";

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

	got_reply = got_resent = ppress_done = 0;
	fprintf(log_fp, "===== Received mail:\n");

	if (!(ifp = fopen("dip.incoming", "w+"))) {
		perror("dip.incoming");
		bailout(1);
	}
	done_headers = 0;
	while (fgets(line, sizeof(line), inp)) {
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

		/* are we dealing with postal press here? */
		if(ppress_read || ppress_skip)
		{
			if(!strcasecmp(s,"signoff\n") ||
			   !strcasecmp(s,"sign off\n") ||
                           !strcasecmp(s,"-- \n"))
				skipping++;
			else if(!strcasecmp(s,"endpress\n") ||
			        !strcasecmp(s,"endbroadcast\n"))
			{
				fprintf(rfp,"End of message.\n");
				if(!ppress_skip)
				{
					fprintf(ppfp,"End of press message.\n");
					fprintf(mbfp,"End of press message.\n");
					fclose(ppfp);
				}
				ppress_read = ppress_skip = 0;
				ppress_done = 1;
			}
			else
			{
				fputs(line, rfp);
				if(!ppress_skip)
				{
					fputs(line, mbfp);
					fputs(line, ppfp);
				}
			}
			continue;
		}

		if(diary_read || diary_skip) {
			if(!strcasecmp(s, "signoff\n") ||
			   !strcasecmp(s, "sign off\n") ||
			   !strcasecmp(s, "-- \n")) {
				skipping++;
			}
			else if(!strcasecmp(s, "endpress\n") ||
				!strcasecmp(s, "endbroadcast\n")) {
				fprintf(rfp, "\nEnd of diary entry.\n\n");
				if(!diary_skip)
					fclose(diaryfp);
				diary_read = diary_skip = 0;
				diary_done = 1;
			} else {
				fputs(line, rfp);
				if(!diary_skip)
				{
					fputs(line,mbfp);
					fputs(line,diaryfp);
				}
			}
			continue;
		}


		/*  If we're reading or skipping press, get out and set 'skipping' flag if
		   we see 'signoff' alone on a line.  */

		if (broad_read || broad_skip) {
			if (!strcasecmp(s, "signoff\n") ||
			    !strcasecmp(s, "sign off\n") ||
                            !strcasecmp(s, "-- \n")) {
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
				if (!master_press && !broad_skip && dipent.flags & F_SHORTHAND && !(dipent.flags & F_MACH))
				{
				    /* OK, it is a shorthand game. Now see if shorthand press is to be used */
				    /* If at the start, no press is allowed */
				    /* If at the end, normal press is allowed */
    				    /* If in the game, only shorthand press is allowed */
				    if (dipent.seq[0] == 'x' ) {
					fprintf(rfp,"No press allowed in shorthand before game starts - message ignored.\n");
					broad_skip = 1;
				    } else {
				        if (dipent.phase[6] != 'X' ) {
					    if (*line == '~')  { 
						ret += mail_shorthand(&line[1]);
					    } else {
					      ret += mail_shorthand(line);  /* Convert to shorthand press (if enabled) */
				 	    }
				        }
					/* else will allow press as normal */
				    }
				}
				fputs(line, rfp);
				if (broad_read && ret == 0 && !broad_skip) {
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
				       DIPINFO(s);
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
					if (mail_getaddr(s, raddr))
                                                return E_WARN; /* Ignore email from bad address */
					if (!is_allowed(GLOBAL_PLAYER)) {
					    if (!is_disallowed(GLOBAL_PLAYER)) {
                                                fprintf(rfp, "You are not an allowed player on this judge.\n");
                                                fprintf(rfp, "Please contact the judge keeper to be permitted to play.\n");
                                                while (fgets(line, sizeof(line), inp)) {
                                                        fputs(line, log_fp);
                                                        fputs(line, ifp);
                                                }
                                                mail_reply(E_WARN);
                                                return E_WARN;

					    } else {
					
						fprintf(rfp, "You have been blacklisted from this judge.\n");
						fprintf(rfp, "Please contact the judge keeper if you want to dispute this decision.\n");
						while (fgets(line, sizeof(line), inp)) {
							fputs(line, log_fp);
							fputs(line, ifp);
						}
						mail_reply(E_WARN);
						return E_WARN;
					    }
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
					if ((nfp = fopen(TMASTER_FILE, "w")) == NULL) {
						fprintf(rfp, "Error opening %s master file.\n",
							TMASTER_FILE);
						return E_FATAL;
					}
					if ((i = mail_signon(s))) {
						fclose(mfp);
						fclose(nfp);
						fputs("Bad signon, skipping remainder:\n", log_fp);
						while (fgets(line, sizeof(line), inp)) {
							fputs(line, log_fp);
							fputs(line, ifp);
						}
						mail_reply(i);
						return 0;
					}
					dipent.players[player].status &= ~SF_RESIGN;
					if (dipent.players[player].power == MASTER) {
						i_am_really_master = 1;
					} else {
						i_am_really_master = 0;
					}
					msg_header(rfp);

					time(&now_time);
					if (ded[dipent.players[player].userid].md < now_time - 24 * 60 * 60) {
						ded[dipent.players[player].userid].md = now_time;
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
					while (fgets(line, sizeof(line), inp)) {
						fputs(line, log_fp);
						fputs(line, ifp);
					}
					mail_reply(0);
					return 0;

				case GET:
					command++;
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
							sprintf(line, "%s 'Diplomacy file %s'",
								temp, temp);
							MailOut(line, raddr);
							fprintf(rfp, "File %s sent.\n\n", temp);
							listflg++;
						} else {
							fprintf(rfp, "Sorry, there is no file %s available.\n", temp);
						}
					}
					break;

				case MAP:	/* Map is unavailable */

					command++;
					msg_header(rfp);
					fprintf(rfp, "\nThe map command is no longer suppoered.");
					break;



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
					if (!*name)  msg_header(rfp);

					if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
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
							msg_header(rfp);
							fprintf(rfp, "There is no game '%s' active.\n", name);
						} else if (!full) {
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
						msg_header(rfp);
						fprintf(rfp, "Name of game for summary must be specified.\n");
						break;
					}
					sprintf(line, "%s%s/summary", GAME_DIR, name);
					if (!(tfp = fopen(line, "r"))) {
						sprintf(line, "%s%s/G001", GAME_DIR, name);
						if (!(tfp = fopen(line, "r"))) {
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

						sprintf(line, "%s -C %s -%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, not_eof &&
							(dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL)
							&& dipent.flags & F_GUNBOAT ? "g" : "", 
							dipent.flags & F_BLIND ? "b" : "",
							dipent.flags & F_QUIET ? "q" : "",
							not_eof ? dipent.variant : V_STANDARD, name);
						fflush(log_fp);
						if (system(line)) {
							msg_header(rfp);
							fprintf(rfp, "Problem generating summary for game '%s' - has first turn processed yet?\n", name);
							break;
						}
						sprintf(line, "%s%s/summary", GAME_DIR, name);
						if (!(tfp = fopen(line, "r"))) {
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
					msg_header(rfp);
					if ((i = newuser(raddr, inp))) {
						mail_reply(i);
						return (i);
					}
					break;

				case IAMALSO:
					command++;
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

					if (!not_eof) {
						if (*name)
							fprintf(rfp, "There is no game '%s' active.\n", name);
						else
							fprintf(rfp, "Whogame what game?\n");
					}

					break;

				case WHOIS:
					command++;
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
					msg_header(rfp);
					getded(s);
					break;
#endif
				case _VERSION:
					command++;
					msg_header(rfp);
					version(rfp);
					break;

				case HISTORY:
					command++;
					history(s,dipent.players[player].power);
					break;

				case FIXID:
					command++;
					msg_header(rfp);
					setsite(s);
					break;

				case ADJUST:
					command++;
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

				case RECORD:
					command++;
					msg_header(rfp);
					if(sscanf(s, "%d %d %d %30s", &i, &j, &k, x) != 4)
					{
						fprintf(rfp, "Bad record command %s.\n", s);			
					}
					else
					{
						if(strcmp(x,SPECIAL_PW) != 0)
						{
							fprintf(rfp, "The record command may only be used by an administrator.\n");
						}
						else if(i < 0 || i > nded)
						{
							fprintf(rfp, "Bad userid # %d.\n", i);
						}
						else if(j < 1 || j > 5)
						{
							fprintf(rfp,"Bad record mod. type %d.\n", j);
							fprintf(rfp,"Use 1: ontime, 2:total, 3: start, 4: resigned, 5: tookover.\n");
						}
						else
						{
							/* open the record; if it doesn't exist, create it */
							if(get_plyrdata_record(i, &record) < 1)
								memset(&record, 0, sizeof(PLYRDATA_RECORD));

							/* everything looks OK, so process the request */
							switch(j)
							{
								case 1: n = record.ontime; 
									record.ontime = k;
									break;
								case 2: n = record.total; 
									record.total = k;
									break;
								case 3: n = record.started; 
									record.started = k; 
									break;
								case 4: n = record.resigned; 
									record.resigned = k;
									break;
								default: n = record.tookover; 
									 record.tookover = k;
							}
							put_plyrdata_record(i, &record);
							fprintf(rfp,"User %d's plyrdata field %d adjusted to %d.\n", i, j, k);				
							fprintf(xfp,"%s adjusted %d's plyrdata field %d from %d to %d.\n",
								raddr, i, j, n, k);			
						}
					}
					break;

				case INFOPLAYER:
					no_of_infop++;
					if(no_of_infop > INFOPLAYER_MAX)
						break;
					command++;
					msg_header(rfp);
					fprintf(rfp,"Dedication request info on %s\n", s);
					switch(send_dedication(s))
					{
						case 0:
						case E_WARN:
							break;
						default:
							fprintf(stderr,"send_dedication error on %s.\n", s);
							fprintf(rfp,"Request not sucessfully processed.\n");
					}
					break;
/*					if(sscanf(s,"%d", &i) != 1)
					{
						fprintf(rfp, "Bad user ID in info command: Inform %s\n",s);
					} else {
						if(i < 0 || i > nded)
						{
							fprintf(rfp,"Invalid user ID #%d\n", i);
						}
						else
						{
							fprintf(rfp,"Information for user #%d\n",i);
							fprintf(rfp,"Current dedication is: %d\n", ded[i].r);
							fprintf(rfp,"Number of turns ontime: %lu\n", get_data(i,ontime));
							fprintf(rfp,"Total turns played: %lu\n", get_data(i,total));
							fprintf(rfp,"Games started: %lu\n", get_data(i,started));
							fprintf(rfp,"Abandoned positions taken over: %lu\n", get_data(i,tookover));
							fprintf(rfp,"Positions abandoned / resigned: %lu\n", get_data(i,resigned));	
						}
					}
				break; */

				case SIGNOFF:
					skipping++;
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
							sprintf(line, "%s 'Diplomacy file %s'",
								temp, temp);
							MailOut(line, raddr);
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

				case EJECT:
                                        if (dipent.players[player].power == MASTER)
                                        {
                                                while (isspace(*s)) ++s;
 
                                                resign_index = ResignPower(s);
                                                switch (resign_index) {
                                                    case RP_BLANK:
                                                        fprintf(rfp, "Master MUST specify who is to be resigned.\n\n");
                                                        mail_reply(E_WARN);
                                                        return(E_WARN);
                                                        break;
                                                    case RP_INVALID:
                                                        fprintf(rfp,
                                                                "Power '%s' not recognized.\n\n", s);
                                                        mail_reply(E_WARN);
                                                        return E_WARN;
                                                        break;
                                                    case RP_NOT_FOUND:
                                                        fprintf(rfp,
                                                                "Email '%s' not found in this game.\n\n", s);
                                                        mail_reply(E_WARN);
                                                        return E_WARN;
                                                        break;
                                                    case RP_MULTIPLE:
                                                        fprintf(rfp, "Multiple instances of '%s' found: be more specific.\n\n", s);
                                                        mail_reply(E_WARN);
                                                        return E_WARN;
                                                        break;
 
                                                }
						ResignPlayer(resign_index);
 
                                        } else {
					    fprintf(rfp, "Only Master can use the 'eject' command.\n\n");
					    mail_reply(E_WARN);
					    return E_WARN;
					}
				        break;

				case RESIGN:
					if (dipent.players[player].power == MASTER) 
					{
						fprintf(rfp, "Master MUST use 'eject' command.\n\n");
						mail_reply(E_WARN);
						return(E_WARN);
					
					} else {
					    ResignPlayer(player);
					}
					break;

				case PRESS:
				case BROADCAST:
					if (any_broadcast || ppress_done || diary_done) 
					{
						send_press();
						open_press();
						if(ppress_done)
							ppress_done = 0;
						if(diary_done)
							diary_done = 0;
					}
					mail_press(s, (cvalue[i] == PRESS ? 1 : 0));
					break;

				case POSTALPRESS:
					if(any_broadcast || ppress_done || diary_done)
					{
						send_press();
						open_press();
						if(ppress_done)
							ppress_done = 0;
						if(diary_done)
							diary_done = 0;
					}
					process_ppress();
					break;

				case CLEAR:
					tfp = fopen("dip.temp", "w");
					rewind(pfp);
					while (fgets(line, sizeof(line), pfp))
						if (dipent.players[player].power != power(*line))
							fputs(line, tfp);
					fclose(pfp);
					sprintf(line, "%s%s/P%s", GAME_DIR, dipent.name, dipent.seq);
					ferrck(tfp, 2001);
					rename("dip.temp", line);
					pfp = tfp;
					fprintf(rfp, "Pending orders cleared.\n\n");
					break;

                                case PAUSE:
                                    if (!PRIVOK) {
                                        fprintf(rfp, "Sorry, game '%s' is moderated.  ", dipent.name);
                                        fprintf(rfp, "Only the master can pause this game.\n\n");
                                                break;;
                                    }
				    if (GAME_SETUP) {
				        fprintf(rfp, "Game '%s' is in setup mode: no need to pause.  ", dipent.name);
                                                break;
                                    }

                                    dipent.phase[6] = 'P';

                                       fprintf(rfp, "You have paused game '%s'.\n", dipent.name);

                                        pprintf(cfp, "%s%s as %s has paused game '%s'.\n", NowString(),
                                                xaddr, powers[dipent.players[player].power], dipent.name);

                                        fprintf(bfp, "%s as %s has paused game '%s'.\n",
                                                xaddr, powers[dipent.players[player].power], dipent.name);
                                        fprintf(mbfp, "%s as %s has paused game '%s'.\n",
                                                raddr, powers[dipent.players[player].power], dipent.name);

                                        sprintf(subjectline, "%s:%s - %s Game Paused", JUDGE_CODE, dipent.name, dipent.phase);

                                    break;
/** Setup not yet working, disabled 
                                case SETUP:
                                    if (!PRIVOK) {
                                        fprintf(rfp, "Sorry, game '%s' is moderated.  ", dipent.name);
                                        fprintf(rfp, "Only the master can go to setup mode.\n\n");
                                        break;
                                    }
				
                                    dipent.phase[6] = 'S';

                                       fprintf(rfp, "You have entered setup mode for game '%s'.\n", dipent.name);

                                        pprintf(cfp, "%s%s as %s has entered setup mode for game '%s'.\n", NowString(),
                                                xaddr, powers[dipent.players[player].power], dipent.name);

                                        fprintf(bfp, "%s as %s has entered setup mode for game '%s'.\n",
                                                xaddr, powers[dipent.players[player].power], dipent.name);
                                        fprintf(mbfp, "%s as %s has entered setup mode for game '%s'.\n",
                                                raddr, powers[dipent.players[player].power], dipent.name);

                                        sprintf(subjectline, "%s:%s - %s Game in Setup", JUDGE_CODE, dipent.name, dipent.phase);

                                    break;
***/
				case TERMINATE:
					/* Allow termination of unstarted games */
					if (dipent.seq[0] == 'x') {
				        /* Allow termination of unstarted games
*/	
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
					if (dipent.seq[0] == 'x') {
					      fprintf(rfp,"The game will self destruct in one week.\n");

                                            mfprintf(bfp, "%s as %s has terminated ",
                                                 xaddr, powers[dipent.players[player].power]);
                                        mfprintf(bfp, "game '%s'.  ", dipent.name);
                                        mfprintf(bfp, "The game will self-destruct in  one week.\n");
					/* If game was list, set nolist */
					    if (!(dipent.flags & F_NOLIST)) {
					      fprintf(rfp,"\nNote: game has also been forced 'nolist'.\n");
				              dipent.flags |= F_NOLIST;	
					    }
					    qfp = NULL; /* to remove warning */
					}	
					else {
					 fprintf(rfp, "Use the 'resume' command to start\n");
                                        fprintf(rfp, "it back up.  The game will self destruct ");
                                        fprintf(rfp, "if no one signs on for one week.\n");
					mfprintf(bfp, "%s as %s has terminated\n",
                                                 xaddr, powers[dipent.players[player].power]);
					if (dipent.xflags & XF_NORESUME) {
						whotext = "Any master";
					} else {
						whotext = "Anyone";
					}
                                        mfprintf(bfp, "game '%s'.  %s who signs on can restart ",
                                                 dipent.name, whotext);
                                        mfprintf(bfp, "the game by using\n");
                                        mfprintf(bfp, "the 'resume' command.  The game will self-");
                                        mfprintf(bfp, "destruct if no one signs\non for one week.\n");
					/* Only inform people if game had already started! */
					/* Open file for sending to custodians */
					if ((qfp = fopen("dip.temp", "w")) == NULL) {
						fprintf(log_fp,
							"open_press: Error opening second temporary file.\n");
						bailout(E_FATAL);
					}
					msg_header(qfp);

					sprintf(subjectline, "%s:%s - %s Game Terminated", JUDGE_CODE, dipent.name, dipent.phase);

					pprintf(cfp, "%s%s as %s has terminated\n",
						NowString(), xaddr, powers[dipent.players[player].power]);
					pprintf(cfp, "game '%s'.\n", dipent.name);
					pprintf(qfp, "%s as %s has terminated\n",
						xaddr, powers[dipent.players[player].power]);
					pprintf(qfp, "game '%s'.\n", dipent.name);
					}
					dipent.phase[6] = 'X';

					if ( dipent.seq[0] != 'x') {
					fclose(qfp);

					/* Open draw file (for summary) */
					sprintf(line, "%s%s/draw", GAME_DIR, dipent.name);
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
						if (dipent.variant != V_STANDARD || dipent.flags & F_GUNBOAT) {
							sprintf(line,
								"dip.temp 'MNC: Termination in %s' ",
								dipent.name);
							MailOut(line, MN_CUSTODIAN);
						} else {
							sprintf(line,
								"dip.temp 'BNC: Termination in %s'",
								dipent.name);
							MailOut(line, BN_CUSTODIAN);
						}
					}

					/*
					 * Force regeneration of the summary file if it's
					 * a gunboat game.
					 */

					if (dipent.flags & F_GUNBOAT) {
						sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
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
						sprintf(line, "%s -C %s -%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, mflg, gflg,
							dipent.flags & F_QUIET ? "q" : "", dipent.variant,
							dipent.name);
						system(line);
					}

					/*  Mail summary to HALL_KEEPER */

					sprintf(line, "%s%s/summary 'HoF: Termination in %s'",
						GAME_DIR, dipent.name, dipent.name);
					MailOut(line, HALL_KEEPER);
					}
					broadcast = 1;
					break;

				case RESUME:
					if (!(GAME_TERMINATED || GAME_PAUSED || GAME_SETUP) || !strcmp(dipent.name, "control")) {
						fprintf(rfp, "Resume on active game ignored.\n");
						break;
					}
					if ((dipent.xflags & XF_NORESUME) && !PRIVOK ) {
					    fprintf(rfp, "Only the master is allowed to resume this game.\n\n");
					    return E_WARN;
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

					sprintf(subjectline, "%s:%s - %s Game Resumed", JUDGE_CODE, dipent.name, dipent.phase);

					/*
					 * Force regeneration of the summary file if it's a
					 * gunboat game.
					 */

					if (dipent.flags & F_GUNBOAT) {
						sprintf(line, "%s%s/summary", GAME_DIR, dipent.name);
						remove(line);
						sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
						remove(line);
					}
					for (i = 0; i < dipent.n; i++)
						dipent.players[i].status &= ~SF_DRAW;
					broadcast = 1;
					break;

				
                                case PREDICT:
                                        /* Predict the move results (to master only) */
                                        if (dipent.players[player].power != MASTER) {
                                                          fprintf(rfp, "Sorry, but only masters can predict turns.\n");
                                                          mail_reply(E_WARN);
                                                          return(E_WARN);
                                                                  }
                                        if GAME_SETUP {
                                            fprintf(rfp, "You can't predict while in setup mode.\n");
                                            mail_reply(E_WARN);
                                            return(E_WARN);
                                        }
					fprintf(rfp, "Predicting turn %s for game '%s'.\n",dipent.phase, dipent.name);
					fprintf(rfp, "*** Note: This is NOT definitive results, DO NOT DISCLOSE TO PLAYERS ***\n\n");
                                        predict = 1;
                                        break;

				case PROMOTE: 
					/* Make an observer a (joint) master */
					if (dipent.players[player].power != MASTER) {
						fprintf(rfp, "Sorry, but only masters can promote observers.\n");
						mail_reply(E_WARN);
						return(E_WARN);
					}
					GetOneWord(s);
					i = FindByEmail(s);
                                        switch (i) {
                                                    case RP_BLANK:
                                                        fprintf(rfp, "Master MUST specify who is to be promoted.\n\n");
                                                        mail_reply(E_WARN);
                                                        return(E_WARN);
                                                        break;
                                                    case RP_NOT_FOUND:
                                                        fprintf(rfp,
                                                                "Email '%s' not found in this game.\n\n", s);
                                                        mail_reply(E_WARN);
                                                        return E_WARN;
                                                        break;
                                                    case RP_MULTIPLE:
                                                        fprintf(rfp, "Multiple instances of '%s' found: be more specific.\n\n", s);
                                                        mail_reply(E_WARN);
                                                        return E_WARN;
                                                        break;
					}
					dipent.players[i].power = MASTER;  /* Welcome to masterhood! */

					sprintf(subjectline, "%s:%s - %s Promotion of %s", JUDGE_CODE, dipent.name, dipent.phase, dipent.players[i].address);

					fprintf(rfp, "%s is now also a Master for game '%s'.\n",
                                                      dipent.players[i].address, dipent.name);
					fprintf(bfp," %s is now also a Master for game '%s'.\n",
                                                      dipent.players[i].address, dipent.name);
					fprintf(mbfp," %s is now also a Master for game '%s'.\n",
                                                      dipent.players[i].address, dipent.name);
                                        pprintf(cfp, "%s%s is now also a Master for game '%s'.\n", NowString(),
                                                                 dipent.players[i].address, dipent.name);
					broad_signon = 1; /* Tell the world of the happy news! */
					break;

	
				case BECOME:
                                        if (predict) {
                                                fprintf(rfp, "Cannot do a become when predicting.\n");
                                                mail_reply(E_WARN);
                                                return E_WARN;
                                        }

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

                                        if ( n == MASTER )
                                                break;
 
					if (0 >= n || n >= WILD_PLAYER) {
						fprintf(rfp, "Become error: unknown power- %s", s);
						break;
					}
					for (i = 0; i < dipent.n; i++)
						if (dipent.players[i].power == n)
							break;

					if (i == dipent.n) {
						fprintf(rfp, "No one is signed up to play %s.\n", powers[n]);
						break;
					}

					/* gm only gets confirmation of commands to this point */
					fclose(rfp);
				        msg_header_done = 0;  /* Bug 282, header will need to be redone */

					sprintf(cmdline, "mv %s %s", rfile, r2file);
					system(cmdline);
					
					/* open new reply for both */
					if((rfp = fopen(rfile, "w")) == NULL)
					{
						fprintf(stderr, "Error reopening %s.\n", rfile);
						bailout(E_FATAL);
					}
					msg_header(rfp);
					fprintf(rfp, "Master %s assuming identity of %s.\n", raddr, powers[n]);
					fprintf(rfp, "Password not shown; Address %s.\n\n",
					      dipent.players[i].address);
					player = i;
					GM_Became = 1;
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
					sprintf(temp, "%s%s/G%3.3d", GAME_DIR, dipent.name, i);
					
					if (i == 1) {
                                            /* First phase, so check if it was an adjustment */
                                            if (sphase[dipent.variant][5] == 'A')
                                                i = 2;  /* For adjustments, first phase is #2 */
                                        }

                                        if (atoi(dipent.seq) == i ) {
                                                fprintf(rfp, "Already on specified turn, roll back ignored.\n\n");
                                                break;
                                        }

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

					if GAME_SETUP
                                           dipent.phase[6] = ' ';

					sprintf(subjectline, "%s:%s - %s Rollback to ", JUDGE_CODE, dipent.name, dipent.phase);

					line[strlen(line) - 1] = '\0';
					strcpy(dipent.phase, line);
					sprintf(dipent.seq, "%3.3d", i);

					strcat(subjectline, dipent.phase);

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
					sprintf(Tfile, "%s%s/T%s", GAME_DIR, dipent.name, dipent.seq);
					if ((ofp = fopen(Tfile, "w")) == NULL) {
						fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
						return E_FATAL;
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
					UpdateBlockades();
					break;

				case SUMMARY:
					{
						char *mflg, *gflg;

						gflg = (dipent.flags & F_GUNBOAT &&
							(dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL))
						    ? "g" : "";
						mflg = (*gflg && dipent.players[player].power == MASTER)
						    ? "m" : "";

						sprintf(line, "%s%s/%ssummary", GAME_DIR, dipent.name, mflg);
						if (!(tfp = fopen(line, "r"))) {
							sprintf(line, "%s -C %s -%s%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, mflg, gflg,
								dipent.flags & F_QUIET ? "q" : "",
								dipent.flags & F_BLIND ? "b" : "",
								dipent.variant, dipent.name);
							fflush(log_fp);
							if (system(line)) {
								fprintf(rfp, "Problem generating summary for game '%s' - has first turn processed yet?\n",
									dipent.name);
								break;
							}
							sprintf(line, "%s%s/%ssummary", GAME_DIR, dipent.name, mflg);
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
					if (newuser(raddr, inp) == E_FATAL) {
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
				case _VERSION:
					version(rfp);
					break;

				case HISTORY:
					history(s, dipent.players[player].power);
					break;

				case PHASE:
					if (signedon > 0) {
					    if (0 > (read_phase = phase(s)))
					    {
						fprintf(rfp, "Invalid phase %s specified!\nPhased orders will be ignored and an error set.\n\n", s);
						errorflag++;
					    } else {
						fprintf(pfp, "%c: %s",
                                                                   dipent.pl[dipent.players[player].power], line);
					    }
					} else fprintf(rfp,"Cannot phase when not signed on.\n\n");
				break;

				case INFOPLAYER:
					no_of_infop++;
                                        if(no_of_infop > INFOPLAYER_MAX)
                                                break;
					fprintf(rfp,"Dedication request info on %s\n", s);
					switch(send_dedication(s))
					{
						case 0:
						case E_WARN:
							/* all is well */
							break;
						default:		
							/* something is wrong*/
							fprintf(rfp,"Request not sucessfully processed.\n");		
							fprintf(stderr,"send_dedication error on %s.\n", s);
					}
					break;
				
				case UNSTART:
					if (dipent.players[player].power != MASTER) {
						fprintf(rfp, "Only the master can use the unstart command.\n\n");
						break;
					}
					if (dipent.seq[0] == 'x') {
						fprintf(rfp, "The game may not be unstarted because it has not yet started.\n\n");
						break;
					}

					if (strcmp(dipent.seq, "001")) {
						fprintf(rfp, "The game may not be unstarted after a phase has processed.\n\n");
						break;
					}

					sprintf(subjectline, "%s:%s - %s Unstart", JUDGE_CODE, dipent.name, dipent.phase);

					fprintf(rfp, "Game '%s' unstarted.\n\n", dipent.name);
					mfprintf(bfp, "Game '%s' unstarted by %s.\n\n", dipent.name, raddr);
					broadcast = 1;

					for (i = 0, j = 0, k = 0; i < dipent.n; i++) {
						if (!(dipent.players[i].status & SF_RESIGN)) {
							for (l = 0, n = 0; l < j; l++) {
								n |= (dipent.players[l].userid == dipent.players[i].userid);
							}

							if (!n) {
								dipent.players[j] = dipent.players[i];

								if ((dipent.players[j].power != MASTER) && (dipent.players[j].power != OBSERVER)) {
									dipent.players[j].power = WILD_PLAYER;
									dipent.players[j].status = 0;
									dipent.players[j].units = 0;
									dipent.players[j].centers = 0;
									k++;
								}

								j++;
							}
						}
					}

					dipent.n = j;
					dipent.deadline = 0;
					dipent.start = 0;
					dipent.grace = 0;

					sprintf(dipent.seq, "x%i", k);
					dipent.xflags |= XF_MANUALSTART;
					signedon = -1;

					sprintf(x, "%s%s/G001", GAME_DIR, dipent.name);
					remove(x);
					sprintf(x, "%s%s/M001", GAME_DIR, dipent.name);
					remove(x);
					sprintf(x, "%s%s/P001", GAME_DIR, dipent.name);
					remove(x);
					sprintf(x, "%s%s/T001", GAME_DIR, dipent.name);
					remove(x);

					break;

				case DIARY:
					if(any_broadcast || ppress_done || diary_done)
					{
						send_press();
						open_press();
						if(ppress_done)
							ppress_done = 0;
						if(diary_done)
							diary_done = 0;
					}
					process_diary(s);
					break;

				case FORCE_BEGIN:
					if (dipent.players[player].power != MASTER) {
                                                fprintf(rfp, "Only the master can use the force begin command.\n\n");
                                                break;
                                        }
                                        if (dipent.seq[0] != 'x') {
                                                fprintf(rfp, "The game has already started!\n\n");
                                                break;
                                        }
					if (InsertDummyPlayers()) {
                                            sprintf(subjectline, "%s:%s - %s Forced Begin",JUDGE_CODE, dipent.name, dipent.phase);

                                            fprintf(rfp, "Game '%s' has been forced to start.\n\n", dipent.name);
                                            mfprintf(bfp, "Game '%s' has been forced to start by %s.\n\n", dipent.name, raddr);
                                            broadcast = 1;

                                        /* Now set the flags that will start the game */
                                            strcpy(dipent.seq, "001");
                                            starting++;
                                        } else {
                                        fprintf(rfp, "Game has too many observers/players already to be force started!\n\n");
                                        }


					break;

/*-------------------------------------------------------------*
 *--  Modified by Tamas  --------------  2002-06-11  ----------*
 *-------------------------------------------------------------*/
                                case COND:
                                        if (dipent.flags & F_BLIND) {
                                          fprintf(rfp, "Conditionals are not supported in blind variant. ");
                                          fprintf(rfp, "Rejected: %s\n", line);
                                          break;
                                        }
/*---------------------------------------------  Tamas End  ---*/

/**---- Intentional lack of "break;" - do NOT insert code here ----**/

				default:	/* Assume this is a movement order */
					if (signedon > 0) {
						if (read_phase == 0) {
							movement++;
							fprintf(ofp, "%c: %s",
								dipent.pl[dipent.players[player].power], line);
						} else if (read_phase > 0) {
							if (phase_syntax(read_phase, line)) {
							    fprintf(rfp,"Invalid phase order rejected: -> %s", s);
							} else {
							    fprintf(pfp, "%c: %s",
								   dipent.pl[dipent.players[player].power], line);
						        }
						} 
					}
				}
			}
		}
	}
	fprintf(log_fp, "===== End of received mail\n");

	if (broad_read || broad_skip)
		fprintf(rfp, "\nEnd of message.\n\n");

	if (starting) {
		mail_igame();
	}

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
		{
			if(dipent.players[player].status & (SF_CD | SF_ABAND))
				put_data(dipent.players[player].userid, tookover);
			dipent.players[player].status &= ~(SF_CD | SF_ABAND);
		}
		if (!more_orders)
		{
			dipent.players[player].status |= SF_PART;
		}
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
					if (time(NULL) > dipent.deadline) {
						 fprintf(rfp, "\n\nNote: you are LATE and orders are still not");
						 fprintf(rfp, " received for all units.\n");
					} else if(dipent.phase[6] != 'X') {
					    fprintf(rfp, "\n\nOrders not received for all units.  If complete ");
					    fprintf(rfp, "orders are not\n");
					    fprintf(rfp, "received by %s, you will ", ptime(&dipent.deadline));
					    fprintf(rfp, "be considered late.\n");
					}
					if(dipent.phase[6] != 'X')
					{
						if (dipent.flags & F_NONMR) {
							fprintf(rfp, "You will be considered abandoned if all orders\nare not ");
							fprintf(rfp, "received by\n%s.\n\n", ptime(&dipent.grace));
						} else {
							fprintf(rfp, "The partial orders will be processed if nothing ");
							fprintf(rfp, "is received by\n%s.\n\n", ptime(&dipent.grace));
						}
					}
				}
			} else {
				dipent.players[player].status |= SF_MOVED;
				if (dipent.players[player].status & SF_WAIT) {
					fprintf(rfp, "\n\nYou have set 'wait' status");
					if (time(NULL) <= dipent.deadline) {
						fprintf(rfp, " so orders will not be ");
						fprintf(rfp, "processed\nbefore %s.\n\n", ptime(&dipent.deadline));
					} else {
						fprintf(rfp,".\n\n");
					}
				}
			}
		} else {
			dipent.players[player].status &= ~SF_MOVED;
			fprintf(rfp, "\n\n%d error%s encountered.\n\n", i, i == 1 ? "" : "s");

			/* don't set error flag if player doesn't have
			   moves do -- Tim Miller Jun 18, 2003 */
			if(dipent.players[player].status & SF_MOVE)
			{
				errorflag++;
				fprintf(rfp,"Error flag set.\n\n");
			} else {
				fprintf(rfp,"Since you have no moves due, the error flag is not set.\n\n");
			}
			
			if ((dipent.players[player].status & SF_MOVE) &&
                           (dipent.players[player].power != MASTER) &&
                            !GAME_PAUSED && (dipent.phase[6] != 'X')) {
                                long then;
                                if (time(NULL) <= dipent.deadline) {
                                    fprintf(rfp, "Unless error-free orders are received by the deadline ");
                                    fprintf(rfp, "of\n%s you will be considered late.\n",
                                            ptime(&dipent.deadline));
                                }

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

		temp1 = (dipent.players[0].status & SF_PROCESS);
		deadline((sequence *) NULL, 0);
		dipent.players[0].status |= temp1;

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
		int tuid, tsid, tlvl;

		fprintf(rfp, "Signon command not encountered.  Use 'help' for info.\n");
		if(!getuser(raddr, &tuid, &tsid, &tlvl)) /* do we know this guy? */
			spammail = 1;
		mail_reply(E_WARN);
	} else {
		mail_reply(0);
	}

	if (signedon) {
		do {
			putdipent(nfp, 1);
		} while (getdipent(mfp));
		fclose(mfp);
		ferrck(nfp, 2003);
		fclose(nfp);
		rename(TMASTER_FILE, MASTER_FILE);
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
	    broad_signon = broad_params = 
	    broadcast_master_only = broadcast_absence_adjust = 0;
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
	int slen;
	char jline[50];
	time_t now;

	if ((err != E_FATAL) && (!junkmail))
		send_press();

	if (err != 0 && !junkmail && !spammail) {
		int hdr_end = 0;

		fflush(ifp);
		rewind(ifp);
		fprintf(rfp, "\n---- Original message follows:\n");
		while (fgets(line, sizeof(line), ifp))
		{
			if(!strcmp(line, "\n") || !strcmp(line, "\r\n"))
				hdr_end = 1;
			if(hdr_end)
				fputs(line, rfp);
		}
	} else if (err != 0 && !junkmail && spammail) {
		fprintf(rfp, "\nYour original message is not being echoed because you are not registered on this judge.\n");
	}

	if (!Dflg)
	{
		if(signedon && (dipent.phase[6] != 'X'))
		{
			now=time(NULL);   
        	        if(now < dipent.deadline)
                	        fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                	if(now < dipent.grace)
                        	fprintf(rfp, "\nTime to grace period expiration: %s.\n", timeleft(&dipent.grace));
		}	
		fclose(rfp);
        	msg_header_done = 0;  /* Bug 282, header will need to be redone */

	}
	fclose(ifp);

	if (err == E_FATAL) {
		sprintf(line, "%s 'Diplomacy Error'", rfile);
		MailOut(line, GAMES_MASTER);
		bailout(E_FATAL);
	}
	if (junkmail)
		return;

	if (!strncmp(subject, "Re: ", 4)) {
		shiftleft(4);

		if (!strncmp(subject, "[Error Flag] ", 13))
			shiftleft(13);

		if (!strncmp(subject, "[You are late!] ", 16))
			shiftleft(16);

		if (!strncmp(subject, JUDGE_CODE, strlen(JUDGE_CODE))) {
			shiftleft(strlen(JUDGE_CODE));

			if (signedon) {
				if (!strncmp(subject, ":", 1)) {
					shiftleft(1);

					slen = strlen(dipent.name);

					if (!strncmp(subject, dipent.name, slen)) {
						shiftleft(slen);

						if (!strncmp(subject, " - ", 3)) {
							shiftleft(3);

							slen = strlen(dipent.phase);

							if (!strncmp(subject, dipent.phase, slen)) {
								shiftleft(slen);

								while (subject[0] == ' ')
									shiftleft(1);
							}
						}
					}
				}
			} else {
				if (!strncmp(subject, ":", 1))
					shiftleft(1);

				while (subject[0] == ' ')
					shiftleft(1);
			}
		}
	}

	if (signedon) 
	{
		s = dipent.players[player].address;
		sprintf(jline, "%s:%s - %s", JUDGE_CODE, dipent.name, dipent.phase);

		if (errorflag && (dipent.players[player].status & SF_MOVE)) {
			dipent.players[player].status &= ~SF_MOVED; /* Make an error */

		}
	} 
	else 
	{
		s = raddr;
		sprintf(jline, "%s", JUDGE_CODE);
	}

	if (*s && *s != '*' && !Dflg) {
		/* call lenlimit to make sure lines <=1024 long */
		sprintf(line, "%s %s > %s.tmp", LENLIMIT_CMD, rfile, rfile);
                system(line);
                sprintf(line, "mv %s.tmp %s", rfile, rfile);
                system(line);

		/*
		 * This code sends mail to whoever owns the power, which is
		 * cool, but it may reveal master commands to the player
		 * if the GM "became" him, fix by Tim Miller, so we only send
		 * rfile.
		 * May 14, 2003
		 */
		sprintf(line, "%s 'Re: %s%s %s'", rfile, errorflag ? "[Error Flag] " : "", jline, subject);
		MailOut(line,s);

	}

	if (*raddr != '*' && address_not_in_list(raddr,s) && !Dflg) {
                        
                /*
                 * this code returns a reply to whoever sent the mail, if it's
                 * the GM and he became, we need to give him his old stuff
                 * too.
                 */
                if(GM_Became)
                {
                        sprintf(line, "cat %s %s > dip.tempreply", r2file
                                , rfile);
                        system(line);
                        sprintf(line, "mv dip.tempreply %s", rfile);
                        system(line);
                }
                sprintf(line, "%s 'Re: %s%s %s'", 
                        rfile, errorflag ? "[Error Flag]" : "", jline, subject);
		MailOut(line, raddr);
        }

	return;
}

/*
 *  Write message header to output file.
 */

void msg_header(FILE * fp)
{
	char *temp;
	
	if (fp == rfp && msg_header_done) return; /* Already done, so exit */

	fprintf(fp, "\n");  /* Bug 280, add always a blank line before judge messages */

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
		if (dipent.flags & F_AFRULES)
			fprintf(fp, " AF_Rules");
		if (dipent.flags & F_SHORTHAND)
			fprintf(fp, " Shorthand");
		if (dipent.flags & F_WINGS)
			fprintf(fp, " Wings");
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
	int lmdone, lbdone;   /* state variables on whether lenlimit has been called on bfile and mbfile */

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

	if (broad_signon &&
	   ((dipent.flags & F_QUIET && dipent.seq[0] != 'x') ||
	   (dipent.x2flags & X2F_SECRET))) {
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

	lmdone = lbdone = 0;
	if (signedon && any_broadcast && control < 1000) {
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;
			/* Don't broadcast to those with nobroad set */
			if (dipent.players[i].status & SF_NOBROAD & !broad_part) continue;

			if ((i != player || dipent.players[i].power == MASTER) &&
			    *dipent.players[i].address != '*') {
				if (broad_part && !(dipent.players[i].status & SF_PRESS)) {
					for (s = broad_list; *s; s++)
						if (power(*s) == dipent.players[i].power)
							break;
					if (!(!*s ^ !broad_allbut))
						continue;
				}
				if (dipent.players[i].power == MASTER && 
				    !(dipent.players[i].status & SF_RESIGN)) {
					broadcast_master_only = 0; /* We're sending to master anyway */

					if(!lmdone) {
						sprintf(line, "%s %s > %s.tmp", LENLIMIT_CMD, mbfile, mbfile);
						system(line);
						sprintf(line, "mv %s.tmp %s", mbfile, mbfile);
						system(line);
						lmdone = 1;
					}
					sprintf(line, "%s '%s'", mbfile, subjectline);
                                        MailOut(line, dipent.players[i].address);


				} else if (!master_only_press ||
					  (master_only_press && dipent.players[i].power == power(broad_list[0]))) {
					/* send if not master only press or if master-only but this is the
					   destination power */
					sprintf(line, "%s '%s'", bfile, subjectline);
				
					if(!lbdone) {
						sprintf(line, "%s %s > %s.tmp", LENLIMIT_CMD, bfile, bfile);
						system(line);
						sprintf(line, "mv %s.tmp %s", bfile, bfile);
						system(line);
						lbdone = 1;
					}

					sprintf(line, "%s '%s'", bfile, subjectline);

				    if (!(dipent.players[i].status & SF_RESIGN))
					MailOut(line, dipent.players[i].address);

				}
			}
		}

		/*
		 * If the incoming mail came from somewhere other than the player's
		 * registered email address, then send a copy to that address as
		 * well.
		 */

		if (*baddr != '*') {
			sprintf(line, "%s 'Diplomacy notice: %s'", bfile, dipent.name);
			MailOut(line, baddr);
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
	/* OK, let's look for resigning players and mark them as resigned
	 * (left this late so that they get notice of resignation if master
	 * did it for them)!
         */
	for (i = 0; i < dipent.n; i++) {
		if (!strcmp(dipent.players[i].password,GOING_PWD)) {
                     dipent.players[i].status |= SF_ABAND;
                     if (dipent.players[i].power == WILD_PLAYER)
                         dipent.seq[1]--;
                     if (dipent.players[i].power >= WILD_PLAYER) {
                             dipent.players[i].power = -1;
                     }
                     dipent.players[i].siteid = 0;
		     strcpy(dipent.players[i].password,GONE_PWD);
			     /*strcpy(dipent.players[i].address,"*");*/
		}
		/* Let's also take advantage and see if a master-only
		   message should be sent */
		if (broadcast_master_only && dipent.players[i].power == MASTER &&
		    !(dipent.players[i].status & SF_RESIGN)) {
			sprintf(line, "%s '%s'", mbfile, subjectline);
			MailOut(line, dipent.players[i].address);
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

#define NULL_EMAIL "nobody@localhost"

static int InsertDummyPlayers()
{
    int i;
    int players = 0;

    /* First, let's count the players that we have */

    for (i = 0; i < MAXPLAYERS && dipent.players[i].power > 0; i++)
        if (dipent.players[i].power == WILD_PLAYER)
            players++;

    /* See if enough space to insert dummy players */
    if (i + (dipent.np - players) >= MAXPLAYERS && (dipent.np - players) > 0)
        return 0;

    if (players < dipent.np) {
        /* OK, we are missing players, so let's add the right number of dummy ones */
        while (players != dipent.np && i < MAXPLAYERS ) {
            dipent.players[i].power = WILD_PLAYER;
            dipent.players[i].userid = 0;
            strcpy(dipent.players[i].address,NULL_EMAIL);
            dipent.players[i].status = SF_ABAND;
            players++;
            dipent.n++;
            i++;
        }
    }
    return 1;  /* Dummy players inserted ok */
}
