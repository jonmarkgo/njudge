/*
 * $Log$
 * Revision 1.49  2003/08/23 09:59:47  russblau
 * deleted commented-out keywords
 *
 * Revision 1.48  2003/08/10 15:27:51  millis
 * Fix bug 25 (Add TouchPress)
 *
 * Revision 1.47  2003/06/29 01:06:58  millis
 * Fix bug 176 (duplicate grace setting line to master)
 *
 * Revision 1.46  2003/05/24 23:57:15  millis
 * Bug 97, removed Generic Handling code
 *
 * Revision 1.45  2003/05/24 22:55:43  millis
 * Bug 142, allow use of AutoCreate flag
 *
 * Revision 1.44  2003/05/16 20:37:45  millis
 * Small compile error fix.
 *
 * Revision 1.43  2003/05/15 00:09:46  russblau
 * Corrected errors in keyword arrays, and commented out very many redundant entries.
 *
 * Revision 1.42  2003/05/10 00:33:37  millis
 * Fix bug 125, removing money also disables loans, assassinations and special units
 *
 * Revision 1.41  2003/05/05 11:52:49  millis
 * Slight text improvement
 *
 * Revision 1.40  2003/05/03 23:33:50  millis
 * Fix bug 150 (NO_GARRISONS flag)
 *
 * Revision 1.39  2003/05/03 14:22:51  millis
 * Really small change
 *
 * Revision 1.38  2003/05/03 14:19:33  millis
 * Merged in USTV version, with Neutrals etc.
 *
 * Revision 1.37  2003/04/16 04:34:42  millis
 * Implement Bug 65
 *
 * Revision 1.36  2003/03/07 21:24:09  russblau
 * Editorial changes.
 *
 * Revision 1.35  2003/02/28 22:27:28  russblau
 * Minor changes to documentation; "SET BROADCAST" should now work as described
 * in documentation (but kept previous form "SET NORMAL BROADCAST" for backwards
 * compatibility).
 *
 * Revision 1.34  2003/02/28 20:16:43  nzmb
 * Changed the name of resignation ratio to CD ratio, to avoid confusion with
 * Doug Massey's DRR.
 *
 * Revision 1.33  2003/02/25 11:11:24  russblau
 * Improved absence documentation and related Judge message strings.
 *
 * Revision 1.32  2003/02/17 15:39:20  millis
 * bug 10, prevent overlapping absences
 * Also improve text output for absences.
 *
 * Revision 1.31  2003/01/15 13:59:26  millis
 * Removed Dipstats
 *
 * Revision 1.30  2003/01/14 13:51:37  millis
 * Merges from USTV
 *
 * Revision 1.29  2002/12/28 01:00:57  millis
 * Small text change
 *
 * Revision 1.28  2002/12/28 00:52:19  millis
 * Proper fix to CR 17
 *
 * Revision 1.27  2002/12/28 00:42:54  millis
 * Implement bug 17, noBcentres option (to hide others centres in blind)
 *
 * Revision 1.26  2002/08/27 22:27:54  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.25  2002/07/17 16:50:44  nzmb
 *
 * Fixed up mistake in PRV array, added more flexibility to commands.
 *
 * Revision 1.24  2002/07/16 18:14:22  nzmb
 * Many changes dealing with the addition of szine style postal press. Also 
 * fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.23  2002/06/11 16:26:19  nzmb
 *
 * Added set [no]mustorder to require players to submit avalid set of orders
 * before they may send press (to be used in conjunction with set wait).
 *
 * Revision 1.22  2002/05/11 09:15:33  greg
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
 * Revision 1.21  2002/04/18 04:44:32  greg
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
 * Revision 1.20  2002/04/15 12:55:44  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.19  2002/02/25 12:35:14  miller
 * mall compile error fix
 *
 * Revision 1.18  2002/02/25 11:51:52  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.17  2001/11/11 21:16:21  greg
 * Subjectline Fixes
 *  - New player signons will no longer show "Preference Change"
 *  - Manual start games will no longer say "Waiting for More Players" after 
 *    the game is full
 *  - reply lines no longer assume JUDGE_CODE is four characters
 *
 * Revision 1.16  2001/10/20 12:11:13  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.15.2.3  2001/10/19 23:55:13  dedo
 * Remove warning for over-large string literal
 *
 * Revision 1.15.2.2  2001/10/19 23:43:02  dema
 * Allowed seting/clearing of NoMoney flag and Basic/Advanced
 *
 * Revision 1.15.2.1  2001/10/15 22:23:18  ustv
 * Merged concessions, duality and Colonial flags
 *
 * Revision 1.15  2001/08/31 02:03:01  greg
 * added "Ready to Start" subjectline for manual start games
 *
 * Revision 1.14  2001/08/30 05:01:30  nzmb
 * Modified second argument of calls to check_can_vote.
 *
 * Revision 1.11  2001/07/22 10:03:32  greg
 * customized subjectline for player's preference change
 *
 * Revision 1.10  2001/07/15 09:17:53  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.9  2001/07/08 23:00:05  miller
 * New commands XF_COSTALCONVOY, MOVEDISBAND
 *
 * Revision 1.8  2001/07/01 23:19:29  miller
 * Add costal convoys
 *
 * Revision 1.7  2001/06/24 05:49:30  nzmb
 * Added functionality to set ontime ratio, CD ratio in a game.
 *
 * Revision 1.5  2001/05/14 22:58:21  miller
 * Corrected transformation setting/display. Also disabled Mach1,2,3 flags 
 * (to be removed later for good).
 *
 * Revision 1.4  2001/05/10 08:33:25  greg
 * added subjectlines
 *
 * Revision 1.3  2001/04/15 21:21:22  miller
 * Add SET_(NO)ATTACKTRANS flag settings
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * ENORMOUS number of changes, including:
 *  - Checks to warn if trying to set 'silly' options  (for example, Mach 
 *    options in non-Mach game, press options in nopress game etc.)
 *  - Check when a quorum exists to start the game if number of plyers is 
 *    changed
 *  - Show players' preferences to master
 *  - New general purpose function to ease flag setting/resetting
 *  - setting of absence delay checks (non-master cannot be greater than game 
 *    limit)
 *  - New commands, which are:
 *     SET_(NO)LATECOUNT, SET_(NO)STRCONVOY, SET(NO)LATEPRESS, SET(NO)MANUALPROC,
 *     SET_(NO)MANSTART, SET_(NO)TRANSFORM, SET_(ANY/ONE/HOME)CENTRES,
 *     SET_(NO)ABSENCE, SET_MAXABSENCE, SET_(NO)NORMBROAD, SET_(NO)BLANKPRESS,
 *     SET_(NO)MINORPRESS, SET_MAC(1/2/3), SET_(NO)AIRLIFT, SET_BLANKBOARD,
 *     SET_(NO)AUTODISBAND
 *    (some of which are to be implemented still)
 *   - Observers can watch press of a game (if flag enablingthis is set)
 *   - Passwords only allow alpha-numeric characters
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/02/16 20:43:18  davidn
 * Additions to dipent structure and associated code, to allow duplex variants.
 * Command is "set players n".
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  mail_set.c -- process set commands
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
 *  DATE        NAME         REASON
 *  ----------- ------------ ---------------------------------------
 *  ?? ??? 1987 Ken Lowe     He wrote it
 *  ?? Jan 1995 BLR          See comment below
 *  29 Dec 1996 David Norman Setting of dipent.no_of_players
 *  17 Mar 1997 N Wagner     replaced unlink(2) with remove(3)
 *  17 Nov 1999 M. Miller    Added CheckForGameStart() to start if
 *                           we have a quorum if variant changed
                             Also disallow set password!=alphanum
 *  25 Nov 1999 M. Miller    Added SET [nø]LATECOUNT parameter 
 *  26 May 2001 M. Becroft   Added ontimerat & resrat commands
 *		T. Miller
 */

/*
 * Modified to remove the mfprintf calls and replace with separate
 * calls to output to the general broadcast file and the master
 * broadcast file separately, so that the real address can be placed
 * in the master broadcast file.  Many of these were unnecessary, as
 * many of the commands are master specific, but I didn't process them
 * that intelligently.  1/95  BLR
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#include "config.h"
#include "dip.h"
#include "mail.h"
#include "functions.h"
#include "diplog.h"
#include "plyrdata.h"

extern char *accesses[], *levels[];
extern int avalue[], lvalue[], naccess, nlevel;

void ChangeTransform( char *s);
void ShowTransformSettings(FILE* rfp);
char * SetSubkey(int act, char *s);

#define SETFLAGS(set,mask) dipent.flags = (dipent.flags & ~(mask)) | (set)

#define SETX2FLAGS(set,mask) dipent.x2flags = (dipent.x2flags & ~(mask)) | (set)

#define CheckMach() if (!(dipent.flags & F_MACH)) fprintf(rfp, "Game '%s' is not Machiavelli, option is useless.\n\n", dipent.name);

#define CheckNoMach() if ((dipent.flags & F_MACH)) fprintf(rfp, "Game '%s' is Machiavelli, option is useless.\n\n", dipent.name); 

#define CheckMach2() if (!(dipent.xflags & XF_MACH2)) fprintf(rfp, "Game '%s' is not Machiavelli2, option is useless.\n\n", dipent.name);

#define CheckNoMach2() if ((dipent.xflags & XF_MACH2)) fprintf(rfp, "Game '%s' is Machiavelli2, option is useless.\n\n", dipent.name);

#define CheckPress() if (!HasPress(dipent)) fprintf(rfp, "Game '%s' has no press enabled, option is useless.\n\n", dipent.name); 

#define CheckWings() if (!(dipent.flags & F_WINGS)) fprintf(rfp, "Game '%s' has no wings, option is useless.\n\n", dipent.name);

#define CheckBlind() if (!(dipent.flags & F_BLIND)) fprintf(rfp, "Game '%s' is not blind, option is useless.\n\n", dipent.name);


/* Check if the game has already been started */

int CheckNotStarted(char *text)
{
    char *outtext = text;
    int ret = 1;

    if (dipent.seq[0] != 'x') {
        if (outtext == NULL)
	    outtext = "not allowed to change setting";

        fprintf(rfp, "Game '%s' has already started: %s!\n\n",
                                    dipent.name, outtext);
	ret = 0;
    }
    return ret;
}

#define DRAW_VOTE 1
#define CONC_VOTE 2
/***** Show the curent preference settings 
** Note: Caller DID check this is the master calling this routine?!
**
*/

void ShowPreferences(FILE *out_fptr)
{
	int i;
	int one_found = 0;

        for (i=0; i < dipent.n; i++)
        {
          if (dipent.players[i].power == WILD_PLAYER) {
	    one_found++;
	    if (one_found == 1 ) fprintf(out_fptr," Preference lists are now:\n");
            fprintf(out_fptr,
                "%s has set '%s'.\n",
                 dipent.players[i].address,
                 dipent.players[i].pref);
	  }
       }
       if (one_found > 0) fprintf(out_fptr,"\n");
}


/***************************************************************************/
/***
 *** See if we are ready to start game or not yet, and start it if we are
 ***
 */

void CheckForGameStart()
{
    int i,n;

    if (dipent.seq[0] != 'x') return; /* can't start already started game */
    for (i = n = 0; i < dipent.n; i++) {
        if (dipent.players[i].power == WILD_PLAYER) {
         if (n < dipent.no_of_players) {
             n++;
             } else {
                 dipent.players[i].power = OBSERVER;
             }
         }
    }

    if (n == dipent.no_of_players) {
          /*mail_igame();*/
	  if (!(dipent.xflags & XF_MANUALSTART) ) {
	     /* Following will signal game is ready to start */
              strcpy(dipent.seq, "001");
	      starting++;
	} else {
	    fprintf(rfp,"Game '%s' is now ready for Master to start the game.\n", dipent.name);
            mfprintf(bfp, "Game '%s' is now ready for Master to start the game.\n", dipent.name);

	    sprintf(subjectline, "%s:%s - %s Ready to Start", JUDGE_CODE, dipent.name, dipent.phase);
        }
     broadcast = 1;
     }
}

#define CATF_SETON 1
#define CATF_SETOFF 0
#define CATF_NORMAL 0
#define CATF_INVERSE 1

int CheckAndToggleFlag( int *flag,   /* dipent.flags or dipent.xflags */
			    int flag_mask,  /* F_MASK or XF_MASK */
			    char *flag_name,  /* name of mask for user to see */
			    int set_on,       /* == 1 if set on, 0 if set off */
			    char *warn_text , /* Text to show when flag is changed */
			    int inverted_logic /*set to 1 if flag is negative logic, i.e. On = No... */
			)
{
	char *op_text; /* Either set or cleared */
	char *no_text;   /*Either no or nothing */
	int setting = 0; /* returns 1 if flag whas changed, else 0 */
#define CLEARED_TEXT "cleared"
#define SET_TEXT "set"

	if (set_on == CATF_SETON) {
	   if (*flag & flag_mask) {
                fprintf(rfp, "Game '%s' is already %s.\n", dipent.name, flag_name);
           } else {
                *flag |= flag_mask;
                fprintf(rfp, warn_text);
		if (inverted_logic == CATF_INVERSE) {
		   op_text = CLEARED_TEXT;
		} else {
		   op_text = SET_TEXT;
		}
                pprintf(cfp, "%s%s as %s in '%s' %s the %s flag.\n", NowString(),
			xaddr, powers[dipent.players[player].power], dipent.name, op_text, flag_name);
                /* WAS mfprintf  1/95 BLR */
                fprintf(bfp, "%s as %s in '%s' %s the %s flag.\n", xaddr, PRINT_POWER, dipent.name,
			op_text, flag_name);
                fprintf(mbfp, "%s as %s in '%s' %s the %s flag.\n", raddr, PRINT_POWER, dipent.name, 
			op_text, flag_name);
                fprintf(mbfp, warn_text);
		broadcast = 1;
		setting = 1;
	   }
	}
	else {
	    if (!(*flag & flag_mask)) {
		if (inverted_logic==CATF_INVERSE) {
		    no_text = ""; 
		} else { 
		  no_text = "No";
		}
                fprintf(rfp, "Game '%s' is already %s%s.\n", dipent.name, no_text, flag_name);
             } else {
		if (inverted_logic==CATF_INVERSE) {
		   op_text = SET_TEXT;
                } else {
                   op_text = CLEARED_TEXT;
                }
                *flag &= ~flag_mask;
                fprintf(rfp, warn_text);
                pprintf(cfp, "%s%s as %s in '%s' %s the %s flag.\n", NowString(),
                        xaddr, powers[dipent.players[player].power], dipent.name, op_text, flag_name);
               /* WAS mfprintf  1/95 BLR */
               fprintf(bfp, "%s as %s in '%s' %s the %s flag.\n", xaddr, PRINT_POWER, dipent.name, op_text, flag_name);
               fprintf(mbfp, "%s as %s in '%s' %s the %s flag.\n", raddr, PRINT_POWER, dipent.name, op_text, flag_name);
               fprintf(mbfp, warn_text);
		broadcast = 1;
		setting = 1;
	    }
       }
       return setting;
}

/*
 * The following function will determine if delay is
 * automatically permissible or not
 *
 * It will round up the requested delay to a day before comparing
 *
 */

#define TOO_BIG (-1)
#define JUST_RIGHT (0)

int absence_delay(int max_delay, long delay_period)
{
    int delay_days;

    delay_days = delay_period / (24 * 60 * 60 );
    delay_days += 1;  /* add on a day */

    if (delay_days <= max_delay )
        return JUST_RIGHT;
    else
	return TOO_BIG; 

}

/* This function is supposed to separate two dates into two strings */
/* If only one date string is found, the second one is blanked */

void break_date_into_two(char * instring, char *s1, char *s2)
{
	char *t;
	char *c;

	strcpy(s1, instring);
	*s2 = '\0';

	for (c = s1; *c != '\n'; c++)
		*c = tolower(*c);

	t = strstr(s1,"to");
	if (t) {
	    /* Remove the "to" component" */
	    *t++ = '\0'; t++;
	    
	    while (isspace(*t) && t) t++;	
	    strcpy(s2, t);
	}
}

void mail_setp(char *s)
{
	int i, k, chk24nmr;
	float f,temprat; /* Used in ratio dedication systems. */
        int passOK;  /* used to see if password string is alright on setpass */
	char c, *t, *temp,*s1, *u;
	sequence seq;
	long dates, datee;
	struct tm *tm, *localtime();
	char ss1[150],se1[150];
	char *ss = ss1;
	char *se = se1;
	char stat_text[20];

#define SET_ADD		1
#define PRV_ADD		'a'
#define SET_PW		2
#define PRV_PW		'a'
#define SET_WAIT	3
#define PRV_WAIT	'a'
#define SET_NOWAIT	4
#define PRV_NOWAIT	'a'
#define SET_MOVES	5
#define PRV_MOVES	'm'
#define SET_BUILD	6
#define PRV_BUILD	'm'
#define SET_RETREAT	7
#define PRV_RETREAT	'm'
#define SET_DEADLINE	8
#define PRV_DEADLINE	'm'
#define SET_START	9
#define PRV_START	'm'
#define SET_GRACE	10
#define PRV_GRACE	'm'
#define SET_NONMR	11
#define PRV_NONMR	'm'
#define SET_NMR		12
#define PRV_NMR		'm'
#define SET_MODERATE	13
#define PRV_MODERATE	'm'
#define SET_UNMODERATE	14
#define PRV_UNMODERATE	'm'
#define SET_NOLIST	15
#define PRV_NOLIST	'm'
#define SET_LIST	16
#define PRV_LIST	'm'
#define SET_ACCESS	17
#define PRV_ACCESS	'm'
#define SET_LEVEL	18
#define PRV_LEVEL	'm'
#define SET_VARIANT	19
#define PRV_VARIANT	'm'
#define SET_DEDICATE	20
#define PRV_DEDICATE	'm'
#define SET_NOOP	21
#define PRV_NOOP	'a'
#define SET_FLAG	22
#define PRV_FLAG	'm'
#define SET_FAKEB	23
#define PRV_FAKEB	'm'
#define SET_GREY	24
#define PRV_GREY	'm'
#define SET_GRYWHT	25
#define PRV_GRYWHT	'm'
#define SET_NOFAKEB	26
#define PRV_NOFAKEB	'm'
#define SET_NOPART	27
#define PRV_NOPART	'm'
#define SET_PART	28
#define PRV_PART	'm'
#define SET_PARTFBD	29
#define PRV_PARTFBD	'm'
#define SET_PARTMAY	30
#define PRV_PARTMAY	'm'
#define SET_WHITE	31
#define PRV_WHITE	'm'
#define SET_WHTGRY	32
#define PRV_WHTGRY	'm'
#define SET_PREF	33
#define PRV_PREF	'a'
#define SET_NOPRESS	34
#define PRV_NOPRESS	'm'
#define SET_OBANY	35
#define PRV_OBANY	'm'
#define SET_OBNONE	36
#define PRV_OBNONE	'm'
#define SET_OBWHITE	37
#define PRV_OBWHITE	'm'
#define SET_NOPROXY	38
#define PRV_NOPROXY	'm'
#define SET_PROXY	39
#define PRV_PROXY	'm'
#define SET_NODICE	40
#define PRV_NODICE	'm'
#define SET_DICE	41
#define PRV_DICE	'm'
#define SET_NOFAMINE	42
#define PRV_NOFAMINE	'm'
#define SET_FAMINE	43
#define PRV_FAMINE	'm'
#define SET_NOPLAGUE	44
#define PRV_NOPLAGUE	'm'
#define SET_PLAGUE	45
#define PRV_PLAGUE	'm'
#define SET_NOLOANS	46
#define PRV_NOLOANS	'm'
#define SET_LOANS	47
#define PRV_LOANS	'm'
#define SET_NOASSASS	48
#define PRV_NOASSASS	'm'
#define SET_ASSASS	49
#define PRV_ASSASS	'm'
#define SET_NOADJ	50
#define PRV_NOADJ	'm'
#define SET_ADJ		51
#define PRV_ADJ		'm'
#define SET_NOSPECIAL	52
#define PRV_NOSPECIAL	'm'
#define SET_SPECIAL	53
#define PRV_SPECIAL	'm'
#define SET_ALLPRESS	54
#define PRV_ALLPRESS	'a'
#define SET_NOALLPRESS	55
#define PRV_NOALLPRESS	'a'
#define SET_COMMENT	56
#define PRV_COMMENT	'm'
#define SET_QUIET	57
#define PRV_QUIET	'm'
#define SET_NOQUIET	58
#define PRV_NOQUIET	'm'
#define SET_RATE	59
#define PRV_RATE	'm'
#define SET_NORATE	60
#define PRV_NORATE	'm'
#define SET_REVEAL	61
#define PRV_REVEAL	'm'
#define SET_NOREVEAL	62
#define PRV_NOREVEAL	'm'
#define SET_SHOW	63
#define PRV_SHOW	'm'
#define SET_NOSHOW	64
#define PRV_NOSHOW	'm'
#define SET_NODIAS      65
#define PRV_NODIAS      'm'
#define SET_DIAS        66
#define PRV_DIAS        'm'
#define SET_DRAW        67
#define PRV_DRAW        'a'
#define SET_NODRAW      68
#define PRV_NODRAW      'a'
#define SET_EPNUM       69
#define PRV_EPNUM       'm'
#define SET_CENTERS     70
#define PRV_CENTERS     'm'
#define SET_BN_MN       71
#define PRV_BN_MN       'm'
#define SET_GRACEDAYS   72
#define PRV_GRACEDAYS   'm'
#define SET_NOGRACEDAYS 73
#define PRV_NOGRACEDAYS	'm'
#define SET_PLAYERALLOW 74
#define PRV_PLAYERALLOW	'm'
#define SET_PLAYERDENY  75
#define PRV_PLAYERDENY	'm'
#define SET_MASTERALLOW 76
#define PRV_MASTERALLOW	'm'
#define SET_MASTERDENY  77
#define PRV_MASTERDENY	'm'
#define SET_STRWAIT	78
#define PRV_STRWAIT	'm'
#define SET_NOSTRWAIT	79
#define PRV_NOSTRWAIT	'm'
#define SET_NO_PLAYERS  80
#define PRV_NO_PLAYERS  'm'
#define SET_LATECOUNT   81
#define PRV_LATECOUNT   'm'
#define SET_NOLATECOUNT 82
#define PRV_NOLATECOUNT 'm'
#define SET_STRCONVOY	83
#define PRV_STRCONVOY   'm'
#define SET_NOSTRCONVOY 84
#define PRV_NOSTRCONVOY 'm'
#define SET_LATEPRESS	85
#define PRV_LATEPRESS	'm'
#define SET_NOLATEPRESS 86
#define PRV_NOLATEPRESS 'm'
#define SET_MANPROC  87
#define PRV_MANPROC 'm'
#define SET_NOMANPROC 88
#define PRV_NOMANPROC 'm'
#define SET_MANSTART 89
#define PRV_MANSTART 'm'
#define SET_NOMANSTART 90
#define PRV_NOMANSTART 'm'
#define SET_TRANSFORM   91
#define PRV_TRANSFORM	'm'
#define SET_NOTRANSFORM 92
#define PRV_NOTRANSFORM 'm'
#define SET_XFLAG	93
#define PRV_XFLAG	'm'
#define SET_ANYCENTRE   94
#define PRV_ANYCENTRE   'm'
#define SET_HOMECENTRE  95
#define PRV_HOMECENTRE  'm'
#define SET_RESUME	96
#define PRV_RESUME	'm'
#define SET_NORESUME	97
#define PRV_NORESUME	'm'
#define SET_WATCHALL	98
#define PRV_WATCHALL	'm'
#define SET_NOWATCHALL  99
#define PRV_NOWATCHALL 'm'
#define SET_ABSENCE    100
#define PRV_ABSENCE	'a'
#define SET_NOABSENCE	101
#define PRV_NOABSENCE   'a'
#define SET_MAXABSENCE  102
#define PRV_MAXABSENCE  'm'
#define SET_ONECENTRE   103
#define PRV_ONECENTRE   'm'
#define SET_NORMBROAD	104
#define PRV_NORMBROAD   'a'
#define SET_NONORMBROAD 105
#define PRV_NONORMBROAD 'a'
#define SET_BLANKPRESS	106
#define PRV_BLANKPRESS	'm'
#define SET_NOBLANKPRESS 107
#define PRV_NOBLANKPRESS 'm'
#define SET_MINORPRESS  108
#define PRV_MINORPRESS 'm'
#define SET_NOMINORPRESS 109
#define PRV_NOMINORPRESS 'm'
#define SET_MACH2	110
#define PRV_MACH2	'm'
#define SET_NOMACH2     111
#define PRV_NOMACH2     'm'
/*
#define SET_MACH3       112
#define PRV_MACH3       'm'
*/
#define SET_AIRLIFT     113
#define PRV_AIRLIFT     'm'
#define SET_NOAIRLIFT   114
#define PRV_NOAIRLIFT   'm'
#define SET_BLANKBOARD  115
#define PRV_BLANKBOARD   'm'
#define SET_FORT	116
#define PRV_FORT	'm'
#define SET_NOFORT	117
#define PRV_NOFORT      'm'
#define SET_AUTODISBAND	118
#define PRV_AUTODISBAND	'm'
#define SET_NOAUTODISBAND 119
#define PRV_NOAUTODISBAND 'm'
/* NOTE: 120 CANNOT be used: it is the 'unassigned' value */
#define SET_ANYDISBAND 121
#define PRV_ANYDISBAND 'm'
#define SET_NORMALDISBAND 122
#define PRV_NORMALDISBAND 'm'
#define SET_ATTACKTRANS	  123
#define PRV_ATTACKTRANS	 'm'
#define SET_NOATTACKTRANS 124
#define PRV_NOATTACKTRANS 'm'
#define SET_ONTIMERAT     125
#define PRV_ONTIMERAT     'm'
#define SET_RESRAT        126
#define PRV_RESRAT        'm'
#define SET_COASTALCONVOY   127
#define PRV_COASTALCONVOY  'm'
#define SET_NOCOASTALCONVOY 128
#define PRV_NOCOASTALCONVOY 'm'
#define SET_MONEY	  129
#define PRV_MONEY	  'm'
#define SET_NOMONEY	  130
#define PRV_NOMONEY	  'm'
#define SET_MOVEDISBAND	  131
#define	PRV_MOVEDISBAND	  'm'
#define SET_NOMOVEDISBAND 132
#define PRV_NOMOVEDISBAND 'm'
#define SET_BASIC	  133
#define PRV_BASIC	  'm'
#define SET_ADVANCED	  134
#define PRV_ADVANCED	  'm'
/* Tim Miller's Concession stuff */
#define SET_CONCESSIONS   135
#define PRV_CONCESSIONS   'm'
#define SET_NOCONCESSIONS 136
#define PRV_NOCONCESSIONS 'm'
#define SET_CONC          137 /* This is how player votes to concede */
#define PRV_CONC          'a'
#define SET_NOCONC        138
#define PRV_NOCONC        'a'
#define SET_DUALITY	  139
#define	PRV_DUALITY	  'm'
#define SET_NODUALITY	  140
#define PRV_NODUALITY	  'm'
#define SET_HONGKONG	  141
#define PRV_HONGKONG      'm'
#define SET_NOHONGKONG	  142
#define PRV_NOHONGKONG	  'm'
#define SET_GATEWAY	  143
#define PRV_GATEWAY	  'm'
#define SET_NOGATEWAY	  144
#define PRV_NOGATEWAY	  'm'
#define SET_RAILWAY	  145
#define PRV_RAILWAY	  'm'
#define SET_NORAILWAY     146
#define PRV_NORAILWAY     'm'
#define SET_STORM	  147
#define PRV_STORM	  'm'
#define SET_NOSTORM	  148
#define PRV_NOSTORM	  'm'
#define SET_PREFLIST	  149
#define PRV_PREFLIST	  'm'
#define SET_PREFBOTH	  150
#define PRV_PREFBOTH	  'm'
#define SET_PREFRAND	  151
#define PRV_PREFRAND	  'm'
#define SET_SECRET	  152
#define PRV_SECRET	  'm'
#define SET_NOSECRET	  153
#define PRV_NOSECRET	  'm'
#define SET_NOTVARIANT    154
#define PRV_NOTVARIANT    'm'
#define SET_MUSTORDER	  155
#define PRV_MUSTORDER	  'm'
#define SET_NOMUSTORDER   156
#define PRV_NOMUSTORDER   'm'
#define SET_POSTALPRESS   157
#define PRV_POSTALPRESS   'm'
#define SET_NOPOSTALPRESS 158
#define PRV_NOPOSTALPRESS 'm'
#define SET_BLIND_CENTRES 159
#define PRV_BLIND_CENTRES 'm'
#define SET_BLIND_NOCENTRES 160
#define PRV_BLIND_NOCENTRES 'm'
#define SET_SUMMER	  161
#define PRV_SUMMER	  'm'
#define SET_NOSUMMER	  162
#define PRV_NOSUMMER	  'm'
#define SET_GARRISONS	  163
#define PRV_GARRISONS	  'm'
#define SET_NOGARRISONS	  164
#define PRV_NOGARRISONS	  'm'
#define SET_NEUTRALS	  165
#define PRV_NEUTRALS	  'm'
#define SET_NONEUTRALS	  166
#define PRV_NONEUTRALS	  'm'
#define SET_CAPTUREWIN	  167
#define PRV_CAPTUREWIN	  'm'
#define SET_NOCAPTUREWIN  168
#define PRV_NOCAPTUREWIN  'm'
#define SET_AUTOCREATE    169
#define PRV_AUTOCREATE    'm'
#define SET_NOAUTOCREATE  170
#define PRV_NOAUTOCREATE  'm'
#define SET_TOUCHPRESS    171
#define PRV_TOUCHPRESS     'm'
#define SET_NOTOUCHPRESS 172
#define PRV_NOTOUCHPRESS 'm'

/***********************************************************************
 * Note:  in keys below, a blank space indicates that whitespace is    *
 * optional at that point in the user input; a '#' character indicates *
 * that white space (or parentheses) is required in user input.        *
 ***********************************************************************/

	static char *keys[] = {
	    "", ",", "press",
	    "address", "ad#",
	    "password#", "pw#", "p#",
	    "wait", "w#", "no wait", "now#",
	    "preference", "pref",
	    "moves", "builds", "adjusts", "retreats",
	    "move", "build", "adjust", "retreat",
	    "deadline", "start", "grace",
	    "no nmr", "no cd", "nmr", "cd",
	    "moderated", "unmoderated", "moderate", "unmoderate",
	    "reveal", "no reveal",
	    "no list", "unlist", "private", "list", "public",
	    "white/grey", "grey/white", "white", "grey", "no white", "no grey",
	    "no press", "none", "gray", "white/gray", "gray/white", "no gray",
	    "no fake broadcast", "no fake", "partial may be faked",
	    "partial may", "partial fakes broadcast by default", "default",
	    "partial default", "partial fakes broadcast", "partial fakes",
	    "no partial", "partial",
	    "observer any", "any observer", "no observer", "observer none",
	    "observer no", "white observer", "observer white",
	    "proxy", "no proxy", "dice", "no dice",
	    "famine", "no famine", "plague", "no plague",
	    "loans", "bankers", "no loans", "no bankers", "bank", "no bank",
	    "assassinations", "assassins", "assassination", "no assassins",
	    "no assassinations", "no assassination",
	    "adjacency", "adjacent", "adj",
	    "no adjacency", "no adjacent", "no adj",
	    "special", "no special", "all press", "normal press",
	    "comment", "info", "information",
	    "quiet", "no quiet", "rated", "unrated",
	    "access", "level", "variant",
	    "dedication", "ontimerat", "resrat",
	    "show", "no show", "dias", "no dias", 
	    "concessions", "no concessions",
	    "conc", "concede", "no conc", "no concede",
	    "draw", "no draw", 
	    "ep", "ep number", "ep num", 
	    "centres", "centers",
	    "bn", "bn number", "bn num", 
	    "bnc", "bnc number", "bnc num", 
	    "mn", "mn number", "mn num", 
	    "mnc", "mnc number", "mnc num", 
	    "no strict grace", "strict grace",
	    "players", "allow player", "allow players",
	    "player allow", "players allow",
	    "deny player", "deny players", "player deny", "players deny",
	    "allow master", "allow masters", "master allow", "masters allow",
	    "deny master", "deny masters", "master deny", "masters deny",
	    "strict wait", "no strict wait", "late count", "no late count",
	    "strict convoy", "no strict convoy", "late press", "no late press",
	    "manual process", "man proc", "no manual process", "no man proc",
	    "auto process", "auto proc",
	    "manual start", "man start", "no manual start", "no man start", "auto start",
	    "transform", "trafo",
	    "no transform", "no trafo",
	    "xflag", "xflags",
	    "any centres", "any centre", "any centers", "any center",
	    "home centres", "home centre", "home centers", "home center",
	    "watch all press", "no watch all press",
	    "holidays", "holiday", "vacations", "vacation", "absence",
	    "no holidays", "no holiday", "no vacations", "no vacation", "no absence",
	    "max absence", "max vacations",
	    "max holidays", "one centre",
	    "one center", "broadcast", "broadcast press",
	    "normal broadcast", "no broadcast", "no broadcast press",
	    "no normal broadcast",
	    "blank press", "no blank press",
	    "minor press", "no minor press",
	    "mach2", "machiavelli2", "mach-2", "machiavelli-2",
	    "no mach2", "no machiavelli2", "no mach-2", "no machiavelli-2",
	     "air lifts", "air lift",
	    "no air lifts", "no air lift",
	    "blank board", "empty board",
	    "fortresses", "fortress", "forts", "fort",
	    "no fortresses", "no fortress", "no forts", "no fort",
	    "resume", "no resume",
	    "auto disband", "no auto disband",
	    "any disband", "normal disband",
	    "attack transform", "attack transforms", "no attack transforms", "no attack tranform",
	    "coastal convoys", "coastal convoy",
	    "no coastal convoys", "no coastal convoy",
	    "money", "no money",
	    "disband", "no disband",
	    "basic", "advanced",
	    "duality", "no duality",
	    "hong kong", "no hong kong",
	    "gateways", "gateway", "no gateways", "no gateway",
	    "railways", "railway", "no railways", "no railway",
	    "storms", "storm", "no storms", "no storm",
	    "prflist", "prfboth", "prfrand",
	    "must order", "no must order",
	    "secret", "no secret",
	    "not variant",
	    "postal press", "no postal press",
	    "bcenters", "bcentres",
	    "no bcentres", "no bcenters",
	    "summer", "no summer",
	    "garrisons", "no garrisons",
	    "neutrals", "no neutrals",
	    "capture win", "no capture win", 
	    "auto create", "no auto create",
	    "touch press", "no touch press"
	};

	static unsigned char action[] = {
	    'x', SET_NOOP, SET_NOOP, SET_ADD, SET_ADD, SET_PW, SET_PW, SET_PW,
	    SET_WAIT, SET_WAIT, SET_NOWAIT, SET_NOWAIT, SET_PREF, SET_PREF,
	    SET_MOVES, SET_BUILD, SET_BUILD, SET_RETREAT, SET_MOVES, SET_BUILD,
	    SET_BUILD, SET_RETREAT, SET_DEADLINE, SET_START, SET_GRACE,
	    SET_NONMR, SET_NONMR, SET_NMR, SET_NMR, SET_MODERATE,
	    SET_UNMODERATE, SET_MODERATE, SET_UNMODERATE, SET_REVEAL,
	    SET_NOREVEAL, SET_NOLIST, SET_NOLIST, SET_NOLIST, SET_LIST,
	    SET_LIST, SET_WHTGRY, SET_GRYWHT, SET_WHITE, SET_GREY, SET_GREY,
	    SET_WHITE, SET_NOPRESS, SET_NOPRESS, SET_GREY, SET_WHTGRY,
	    SET_GRYWHT, SET_WHITE, SET_NOFAKEB, SET_NOFAKEB, SET_PARTMAY,
	    SET_PARTMAY, SET_PARTFBD, SET_PARTFBD, SET_PARTFBD, SET_FAKEB,
	    SET_FAKEB, SET_NOPART, SET_PART, SET_OBANY, SET_OBANY, SET_OBNONE,
	    SET_OBNONE, SET_OBNONE, SET_OBWHITE, SET_OBWHITE, SET_PROXY,
	    SET_NOPROXY, SET_DICE, SET_NODICE, SET_FAMINE, SET_NOFAMINE,
	    SET_PLAGUE, SET_NOPLAGUE, SET_LOANS, SET_LOANS, SET_NOLOANS,
	    SET_NOLOANS, SET_LOANS, SET_NOLOANS, SET_ASSASS, SET_ASSASS,
	    SET_ASSASS, SET_NOASSASS, SET_NOASSASS, SET_NOASSASS, SET_ADJ,
	    SET_ADJ, SET_ADJ, SET_NOADJ, SET_NOADJ, SET_NOADJ, SET_SPECIAL,
	    SET_NOSPECIAL, SET_ALLPRESS, SET_NOALLPRESS, SET_COMMENT,
	    SET_COMMENT, SET_COMMENT, SET_QUIET, SET_NOQUIET, SET_RATE,
	    SET_NORATE, SET_ACCESS, SET_LEVEL, SET_VARIANT, SET_DEDICATE,
	    SET_ONTIMERAT, SET_RESRAT, SET_SHOW, SET_NOSHOW, SET_DIAS,
	    SET_NODIAS, SET_CONCESSIONS, 
	    SET_NOCONCESSIONS, SET_CONC, SET_CONC, SET_NOCONC, SET_NOCONC,
	    SET_DRAW, SET_NODRAW, SET_EPNUM, SET_EPNUM,
	    SET_EPNUM, SET_CENTERS, SET_CENTERS,
	    SET_BN_MN, SET_BN_MN, SET_BN_MN, 
	    SET_BN_MN, SET_BN_MN, SET_BN_MN,
	    SET_BN_MN, SET_BN_MN, SET_BN_MN,
	    SET_BN_MN, SET_BN_MN, SET_BN_MN,
	    SET_GRACEDAYS, SET_NOGRACEDAYS,
	    SET_NO_PLAYERS, SET_PLAYERALLOW, SET_PLAYERALLOW, SET_PLAYERALLOW,
	    SET_PLAYERALLOW, SET_PLAYERDENY, SET_PLAYERDENY, SET_PLAYERDENY,
	    SET_PLAYERDENY, SET_MASTERALLOW, SET_MASTERALLOW, SET_MASTERALLOW,
	    SET_MASTERALLOW, SET_MASTERDENY, SET_MASTERDENY, SET_MASTERDENY,
	    SET_MASTERDENY, SET_STRWAIT, SET_NOSTRWAIT,
	    SET_LATECOUNT, SET_NOLATECOUNT, SET_STRCONVOY, SET_NOSTRCONVOY,
	    SET_LATEPRESS, SET_NOLATEPRESS, SET_MANPROC,
	    SET_MANPROC, SET_NOMANPROC,
	    SET_NOMANPROC, SET_NOMANPROC,
	    SET_NOMANPROC, SET_MANSTART, SET_MANSTART,
	    SET_NOMANSTART, SET_NOMANSTART,
	    SET_NOMANSTART, SET_TRANSFORM, SET_TRANSFORM, SET_NOTRANSFORM,
	    SET_NOTRANSFORM, SET_XFLAG, SET_XFLAG, SET_ANYCENTRE, SET_ANYCENTRE,
	    SET_ANYCENTRE, SET_ANYCENTRE,
	    SET_HOMECENTRE, SET_HOMECENTRE, SET_HOMECENTRE, SET_HOMECENTRE,
	    SET_WATCHALL, SET_NOWATCHALL,
	    SET_ABSENCE, SET_ABSENCE, SET_ABSENCE, SET_ABSENCE, SET_ABSENCE,
	    SET_NOABSENCE, SET_NOABSENCE, SET_NOABSENCE, SET_NOABSENCE, SET_NOABSENCE,
	    SET_MAXABSENCE, SET_MAXABSENCE,
	    SET_MAXABSENCE, SET_ONECENTRE,
	    SET_ONECENTRE, SET_NORMBROAD, SET_NORMBROAD,
	    SET_NORMBROAD, SET_NONORMBROAD, SET_NONORMBROAD, SET_NONORMBROAD,
	    SET_BLANKPRESS, SET_NOBLANKPRESS,
	    SET_MINORPRESS, SET_NOMINORPRESS,
	    SET_MACH2, SET_MACH2, SET_MACH2, SET_MACH2, SET_NOMACH2,
	    SET_NOMACH2, SET_NOMACH2, SET_NOMACH2,
	    SET_AIRLIFT, SET_AIRLIFT,
	    SET_NOAIRLIFT, SET_NOAIRLIFT, SET_BLANKBOARD,
	    SET_BLANKBOARD, SET_FORT, SET_FORT, SET_FORT,
	    SET_FORT, SET_NOFORT, SET_NOFORT,
	    SET_NOFORT, SET_NOFORT, SET_RESUME,
	    SET_NORESUME, SET_AUTODISBAND,
	    SET_NOAUTODISBAND,
	    SET_ANYDISBAND, SET_NORMALDISBAND,
	    SET_ATTACKTRANS, SET_ATTACKTRANS, SET_NOATTACKTRANS,
	    SET_NOATTACKTRANS, SET_COASTALCONVOY,
	    SET_COASTALCONVOY, SET_NOCOASTALCONVOY,
	    SET_NOCOASTALCONVOY,
	    SET_MONEY, SET_NOMONEY, SET_MOVEDISBAND,
	    SET_NOMOVEDISBAND, SET_BASIC, SET_ADVANCED,
	    SET_DUALITY, SET_NODUALITY, SET_HONGKONG,
	    SET_NOHONGKONG, SET_GATEWAY,
	    SET_GATEWAY, SET_NOGATEWAY, SET_NOGATEWAY,
	    SET_RAILWAY, SET_RAILWAY,
	    SET_NORAILWAY, SET_NORAILWAY,
	    SET_STORM, SET_STORM, SET_NOSTORM,
	    SET_NOSTORM, SET_PREFLIST, SET_PREFBOTH, SET_PREFRAND,
	    SET_MUSTORDER, SET_NOMUSTORDER, SET_SECRET, SET_NOSECRET,
	    SET_NOTVARIANT, SET_POSTALPRESS,
	    SET_NOPOSTALPRESS, SET_BLIND_CENTRES, SET_BLIND_CENTRES,
	    SET_BLIND_NOCENTRES, SET_BLIND_NOCENTRES,
	    SET_SUMMER, SET_NOSUMMER, SET_GARRISONS, SET_NOGARRISONS,
	    SET_NEUTRALS, SET_NONEUTRALS,
	    SET_CAPTUREWIN,
	    SET_NOCAPTUREWIN ,
	    SET_AUTOCREATE, SET_NOAUTOCREATE,
	    SET_TOUCHPRESS, SET_NOTOUCHPRESS
	};

	static char privs[] = {
	    'x', PRV_NOOP, PRV_NOOP, PRV_ADD, PRV_ADD, PRV_PW, PRV_PW, PRV_PW,
	    PRV_WAIT, PRV_WAIT, PRV_NOWAIT, PRV_NOWAIT, PRV_PREF, PRV_PREF,
	    PRV_MOVES, PRV_BUILD, PRV_BUILD, PRV_RETREAT, PRV_MOVES, PRV_BUILD,
	    PRV_BUILD, PRV_RETREAT, PRV_DEADLINE, PRV_START, PRV_GRACE,
	    PRV_NONMR, PRV_NONMR, PRV_NMR, PRV_NMR, PRV_MODERATE,
	    PRV_UNMODERATE, PRV_MODERATE, PRV_UNMODERATE, PRV_REVEAL,
	    PRV_NOREVEAL, PRV_NOLIST, PRV_NOLIST, PRV_NOLIST, PRV_LIST,
	    PRV_LIST, PRV_WHTGRY, PRV_GRYWHT, PRV_WHITE, PRV_GREY, PRV_GREY,
	    PRV_WHITE, PRV_NOPRESS, PRV_NOPRESS, PRV_GREY, PRV_WHTGRY,
	    PRV_GRYWHT, PRV_WHITE, PRV_NOFAKEB, PRV_NOFAKEB, PRV_PARTMAY,
	    PRV_PARTMAY, PRV_PARTFBD, PRV_PARTFBD, PRV_PARTFBD, PRV_FAKEB,
	    PRV_FAKEB, PRV_NOPART, PRV_PART, PRV_OBANY, PRV_OBANY, PRV_OBNONE,
	    PRV_OBNONE, PRV_OBNONE, PRV_OBWHITE, PRV_OBWHITE, PRV_PROXY,
	    PRV_NOPROXY, PRV_DICE, PRV_NODICE, PRV_FAMINE, PRV_NOFAMINE,
	    PRV_PLAGUE, PRV_NOPLAGUE, PRV_LOANS, PRV_LOANS, PRV_NOLOANS,
	    PRV_NOLOANS, PRV_LOANS, PRV_NOLOANS, PRV_ASSASS, PRV_ASSASS,
	    PRV_ASSASS, PRV_NOASSASS, PRV_NOASSASS, PRV_NOASSASS, PRV_ADJ,
	    PRV_ADJ, PRV_ADJ, PRV_NOADJ, PRV_NOADJ, PRV_NOADJ, PRV_SPECIAL,
	    PRV_NOSPECIAL, PRV_ALLPRESS, PRV_NOALLPRESS, PRV_COMMENT,
	    PRV_COMMENT, PRV_COMMENT, PRV_QUIET, PRV_NOQUIET, PRV_RATE,
	    PRV_NORATE, PRV_ACCESS, PRV_LEVEL, PRV_VARIANT, PRV_DEDICATE,
	    PRV_ONTIMERAT, PRV_RESRAT, PRV_SHOW, PRV_NOSHOW, PRV_DIAS,
	    PRV_NODIAS, PRV_CONCESSIONS, 
	    PRV_NOCONCESSIONS, PRV_CONC, PRV_CONC, PRV_NOCONC, PRV_NOCONC,
	    PRV_DRAW, PRV_NODRAW, PRV_EPNUM, PRV_EPNUM,
	    PRV_EPNUM, PRV_CENTERS, PRV_CENTERS,
	    PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, 
	    PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, 
	    PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	    PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	    PRV_GRACEDAYS, PRV_NOGRACEDAYS,
	    PRV_NO_PLAYERS, PRV_PLAYERALLOW, PRV_PLAYERALLOW, PRV_PLAYERALLOW,
	    PRV_PLAYERALLOW, PRV_PLAYERDENY, PRV_PLAYERDENY, PRV_PLAYERDENY,
	    PRV_PLAYERDENY, PRV_MASTERALLOW, PRV_MASTERALLOW, PRV_MASTERALLOW,
	    PRV_MASTERALLOW, PRV_MASTERDENY, PRV_MASTERDENY, PRV_MASTERDENY,
	    PRV_MASTERDENY, PRV_STRWAIT, PRV_NOSTRWAIT,
	    PRV_LATECOUNT, PRV_NOLATECOUNT, PRV_STRCONVOY, PRV_NOSTRCONVOY,
	    PRV_LATEPRESS, PRV_NOLATEPRESS, PRV_MANPROC,
	    PRV_MANPROC, PRV_NOMANPROC,
	    PRV_NOMANPROC, PRV_NOMANPROC,
	    PRV_NOMANPROC, PRV_MANSTART, PRV_MANSTART,
	    PRV_NOMANSTART, PRV_NOMANSTART,
	    PRV_NOMANSTART, PRV_TRANSFORM, PRV_TRANSFORM, PRV_NOTRANSFORM,
	    PRV_NOTRANSFORM, PRV_XFLAG, PRV_XFLAG, PRV_ANYCENTRE, PRV_ANYCENTRE,
	    PRV_ANYCENTRE, PRV_ANYCENTRE,
	    PRV_HOMECENTRE, PRV_HOMECENTRE, PRV_HOMECENTRE, PRV_HOMECENTRE,
	    PRV_WATCHALL, PRV_NOWATCHALL,
	    PRV_ABSENCE, PRV_ABSENCE, PRV_ABSENCE, PRV_ABSENCE, PRV_ABSENCE,
	    PRV_NOABSENCE, PRV_NOABSENCE,
	    PRV_NOABSENCE, PRV_NOABSENCE, PRV_NOABSENCE,
	    PRV_MAXABSENCE, PRV_MAXABSENCE,
	    PRV_MAXABSENCE, PRV_ONECENTRE,
	    PRV_ONECENTRE, PRV_NORMBROAD, PRV_NORMBROAD,
	    PRV_NORMBROAD, PRV_NONORMBROAD, PRV_NONORMBROAD, PRV_NONORMBROAD,
	    PRV_BLANKPRESS, PRV_NOBLANKPRESS,
	    PRV_MINORPRESS, PRV_NOMINORPRESS,
	    PRV_MACH2, PRV_MACH2, PRV_MACH2, PRV_MACH2, PRV_NOMACH2,
	    PRV_NOMACH2, PRV_NOMACH2, PRV_NOMACH2,
	    PRV_AIRLIFT, PRV_AIRLIFT,
	    PRV_NOAIRLIFT, PRV_NOAIRLIFT, PRV_BLANKBOARD,
	    PRV_BLANKBOARD, PRV_FORT, PRV_FORT, PRV_FORT,
	    PRV_FORT, PRV_NOFORT, PRV_NOFORT,
	    PRV_NOFORT, PRV_NOFORT, PRV_RESUME,
	    PRV_NORESUME, PRV_AUTODISBAND, PRV_NOAUTODISBAND,
	    PRV_ANYDISBAND, PRV_NORMALDISBAND,
	    PRV_ATTACKTRANS, PRV_ATTACKTRANS, PRV_NOATTACKTRANS,
	    PRV_NOATTACKTRANS, PRV_COASTALCONVOY,
	    PRV_COASTALCONVOY, PRV_NOCOASTALCONVOY,
	    PRV_NOCOASTALCONVOY,
	    PRV_MONEY, PRV_NOMONEY, PRV_MOVEDISBAND,
	    PRV_NOMOVEDISBAND, PRV_BASIC, PRV_ADVANCED,
	    PRV_DUALITY, PRV_NODUALITY, PRV_HONGKONG,
	    PRV_NOHONGKONG, PRV_GATEWAY,
	    PRV_GATEWAY, PRV_NOGATEWAY, PRV_NOGATEWAY,
	    PRV_RAILWAY, PRV_RAILWAY, PRV_NORAILWAY, PRV_NORAILWAY,
	    PRV_STORM, PRV_STORM, PRV_NOSTORM,
	    PRV_NOSTORM, PRV_PREFLIST, PRV_PREFBOTH, PRV_PREFRAND,
	    PRV_MUSTORDER,
	    PRV_NOMUSTORDER, PRV_SECRET, PRV_NOSECRET,
	    PRV_NOTVARIANT, PRV_POSTALPRESS,
	    PRV_NOPOSTALPRESS, PRV_BLIND_CENTRES, PRV_BLIND_CENTRES,
	    PRV_BLIND_NOCENTRES, PRV_BLIND_NOCENTRES,
	    PRV_SUMMER, PRV_NOSUMMER, PRV_GARRISONS, PRV_NOGARRISONS,
	    PRV_NEUTRALS, PRV_NONEUTRALS, PRV_CAPTUREWIN, PRV_NOCAPTUREWIN,
	    PRV_AUTOCREATE, PRV_NOAUTOCREATE,
	    PRV_TOUCHPRESS, PRV_NOTOUCHPRESS
	};

	chk24nmr = 0;
	while (*s) {
		if (subjectline[0] == '\0')
			sprintf(subjectline, "%s:%s - %s Parameter Change", JUDGE_CODE, dipent.name, dipent.phase);

		s = lookfor(t = s, keys, nentry(keys), &i);
		sprintf(stat_text, "%3.3d %s", action[i], dipent.name);
		DIPDEBUG(stat_text);

		if (privs[i] == 'm' && !PRIVOK) {
			fprintf(rfp, "> set %s\n", t);
			fprintf(rfp, "Sorry, game '%s' is moderated.  ", dipent.name);
			fprintf(rfp, "Only the master can submit that command.\n\n");
			return;
		}
		switch (action[i]) {
		case SET_NOOP:
			break;

		case SET_ADD:
			if (mail_getaddr(s, dipent.players[player].address)) {
				if (*s)
					fprintf(rfp, "Invalid return address specified: %s", s);
				for (t = raddr, s = dipent.players[player].address; (*s++ = *t++););
			}
			fprintf(rfp, "Setting return address to %s.\n\n",
				dipent.players[player].address);
			if (!(dipent.flags & F_GUNBOAT) ||
			    dipent.players[player].power == MASTER) {
				sprintf(subjectline, "%s:%s - %s Player Address Change: %c", JUDGE_CODE, dipent.name, dipent.phase, dipent.pl[dipent.players[player].power]);

				mfprintf(bfp,
					 "The return address for %s in game '%s' has been changed\nto %s.\n\n",
					 powers[dipent.players[player].power], dipent.name,
					 dipent.players[player].address);
				pprintf(cfp,
					"%sThe return address for %s in game '%s' has been changed\nto %s.\n\n",
					NowString(),
					powers[dipent.players[player].power], dipent.name,
					dipent.players[player].address);
				broadcast = 1;
			}
			s = "";
			break;

		case SET_BUILD:
			memcpy(&seq, &dipent.builds, sizeof(seq));
			if (getseq(rfp, t, &seq))
				break;
			memcpy(&dipent.builds, &seq, sizeof(seq));
			broad_params = 1;
			chk24nmr++;
			s = "";
			break;

		case SET_DEADLINE:
			if (mail_date(&s, &dates, 0, rfp))
				fprintf(rfp, "%sInvalid deadline date specified.\n\n", t);
			else {
				sequence *seq;
				dipent.deadline = dates;
				sprintf(subjectline, "%s:%s - %s Deadline Adjustment to: %s", JUDGE_CODE, dipent.name, dipent.phase, ptime(&dates));
				fprintf(rfp, "Deadline set to %s.\n", ptime(&dates));
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s set the deadline\n", xaddr, PRINT_POWER);
				fprintf(mbfp, "%s as %s set the deadline\n", raddr, PRINT_POWER);
				mfprintf(bfp, "for game '%s' to %s.\n", dipent.name, ptime(&dates));
				broadcast = 1;
				seq = dipent.phase[5] == 'M' ? &dipent.movement :
				    dipent.phase[5] == 'R' ? &dipent.retreat :
				    dipent.phase[5] == 'B' ? &dipent.builds :
				    &dipent.movement;
				if (dipent.grace < dipent.deadline + (seq->grace * HRS2SECS)) {
					dipent.grace = dipent.deadline + (seq->grace * HRS2SECS);
					if (dipent.flags & F_GRACEDAYS) {
						for (k = 0; k < 8; k++) {
							tm = localtime(&dipent.grace);
							if (seq->days[tm->tm_wday] == '-')
								dipent.grace += 24 * 60 * 60;
							else if (islower(seq->days[tm->tm_wday]) && tm->tm_hour < 12)
								dipent.grace += (12 - tm->tm_hour) * 60 * 60 - tm->tm_min * 60 - tm->tm_sec;
							else
								break;
						}
					}
					fprintf(rfp, "Grace period deadline advanced to %s.\n",
						ptime(&dipent.grace));
					mfprintf(bfp, "Grace period deadline advanced to %s.\n",
						 ptime(&dipent.grace));
				}
				if (dipent.start > dipent.deadline) {
					dipent.start = dipent.deadline;
				}
				fprintf(rfp, "\n");
				mfprintf(bfp, "\n");
				/* Reset the SF_REMINDER flags for all players */
				for ( i = 0; i < dipent.n; i++)
				    dipent.players[i].status &= ~SF_REMIND;	
			}
			s = "";
			break;

		case SET_GRACE:
			if (mail_date(&s, &dates, 0, rfp))
				fprintf(rfp, "%sInvalid grace date specified.\n\n", t);
			else if (dates < dipent.deadline) {
				fprintf(rfp, "%sGrace date cannot be earlier than deadline.\n\n", t);
			} else {
				dipent.grace = dates;
				sprintf(subjectline, "%s:%s - %s Grace Adjustment to: %s", JUDGE_CODE, dipent.name, dipent.phase, ptime(&dates));
				fprintf(rfp, "Grace period set to %s.\n\n", ptime(&dates));
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s set the grace period\n", xaddr, PRINT_POWER);
				fprintf(mbfp, "%s as %s set the grace period\n", raddr, PRINT_POWER);
				mfprintf(bfp, "for game '%s' to %s.\n\n", dipent.name, ptime(&dates));
				broadcast = 1;
			}
			s = "";
			break;

		case SET_MOVES:
			memcpy(&seq, &dipent.movement, sizeof(seq));
			if (getseq(rfp, t, &seq))
				break;
			memcpy(&dipent.movement, &seq, sizeof(seq));
			broad_params = 1;
			chk24nmr++;
			s = "";
			break;

		case SET_NOWAIT:
			if (signedon > 0 && read_phase > 0)
				fprintf(pfp, "%c: set nowait\n", name[0]);
			else {
				if (!(dipent.players[player].status & SF_WAIT))
					fprintf(rfp, "Set nowait: no point!\n\n");
				else {
					fprintf(rfp, "Wait status cleared.  Orders may now be processed\n");
					fprintf(rfp, "before %s.\n\n", ptime(&dipent.deadline));
					dipent.players[player].status &= ~SF_WAIT;
				}
			}
			break;

		case SET_PW:
			/* Get a pointer to the players password */
			t = dipent.players[player].password;
			s1 = s;
			/* Copy the password from the input line */
                        passOK = 1;
			while (*s1 && !isspace(*s1) ) {
                               if (!isalnum(*s1)) passOK =0; /*alphanum passwords only! */ 
				s1++;
			}

			/* If no characters have been copied */
			if (s1 == s ) { 
				/* Insult player */
				fprintf(rfp, "Try to put in a non-blank password.\n\n");
			} else if (passOK == 0)
                        {
				fprintf(rfp, "Password can only be alpha-numeric characters.\n\n");
				s = s1; /* to skip whole password line */
			} else {
				/* Terminate the player and confirm change */
				while (*s && !isspace(*s)) {
                                   *t++ = isupper(c = *s++) ? tolower(c) : c;
				 }
				*t++ = '\0';
				fprintf(rfp, "Password set.\n\n");
			}

			/* Break out of the case statement */
			break;

		case SET_PREF:
			if ((dipent.seq[0] != 'x') && !starting) {
				fprintf(rfp, "You may not change the preference list after the game %s",
					"begins.\n");
			} else {
				if ((strstr(subjectline, "New Player Signon") == NULL) && (strstr(subjectline, "Ready to Start") == NULL)) {
					sprintf(subjectline, "%s:%s - %s Preference Change", JUDGE_CODE, dipent.name, dipent.phase);
				}

				for (t = s; *s && !isspace(*s) && *s != ','; s++);
				if (*s)
					*s++ = '\0';
				while (isspace(*s))
					s++;
				if (!*t || *t == '\n') {
					fprintf(rfp, "Preference list cleared.\n");
					*dipent.players[player].pref = '\0';
				} else {
					u = t;

					if (dipent.x2flags & X2F_PREFRANDONLY) {
						if (*u++ == '*' && (!*u || *u == '\0' || *u == '\n')) {
							fprintf(rfp, "Preference list '%s' set.\n", t);
							strcpy(dipent.players[player].pref, t);
							fprintf(mbfp,"Player %s changed preference list.\n\n",raddr);

							ShowPreferences(mbfp);
							broadcast_master_only = 1;  /* Only master(s) are told of this */
						} else {
							fprintf(rfp, "Preference list '%s' not set.\n", t);
							if (*dipent.players[player].pref) {
								fprintf(rfp, "Preference list remains '%s'.\n",
									dipent.players[player].pref);
							}
						}
					} else {
						if (dipent.x2flags & X2F_PREFRANDALLOW) {
							if ((*u++ == '*' && (!*u || *u == '\0' || *u == '\n')) || !(chkpref(t, NULL, NULL))) {
								fprintf(rfp, "Preference list '%s' set.\n", t);
								strcpy(dipent.players[player].pref, t);
								fprintf(mbfp,"Player %s changed preference list.\n\n",raddr);

								ShowPreferences(mbfp);
								broadcast_master_only = 1;  /* Only master(s) are told of this */
							} else {
								fprintf(rfp, "Preference list '%s' not set.\n", t);
								if (*dipent.players[player].pref) {
									fprintf(rfp, "Preference list remains '%s'.\n",
										dipent.players[player].pref);
								}
							}
						} else {
							if (!chkpref(t, NULL, NULL)) {
								fprintf(rfp, "Preference list '%s' set.\n", t);
								strcpy(dipent.players[player].pref, t);
								fprintf(mbfp,"Player %s changed preference list.\n\n",raddr);

								ShowPreferences(mbfp);
								broadcast_master_only = 1;  /* Only master(s) are told of this */
							} else {
								fprintf(rfp, "Preference list '%s' not set.\n", t);
								if (*dipent.players[player].pref) {
									fprintf(rfp, "Preference list remains '%s'.\n",
										dipent.players[player].pref);
								}
							}
						}
					}
				}
			}
			break;

		case SET_RETREAT:
			memcpy(&seq, &dipent.retreat, sizeof(seq));
			if (getseq(rfp, t, &seq))
				break;
			memcpy(&dipent.retreat, &seq, sizeof(seq));
			broad_params = 1;
			chk24nmr++;
			s = "";
			break;

		case SET_START:
			if (mail_date(&s, &dates, 0, rfp))
				fprintf(rfp, "%sInvalid start date specified.\n\n", t);
			else {
				sequence *seq;	/* Local scope variable because of cut&paste. */
				/*  To be redone right in judge reorg. */

				seq = dipent.phase[5] == 'M' ? &dipent.movement :
				    dipent.phase[5] == 'R' ? &dipent.retreat :
				    dipent.phase[5] == 'B' ? &dipent.builds :
				    &dipent.movement;
				dipent.start = dates;
				fprintf(rfp, "Earliest start date set to %s.\n", ptime(&dates));
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s set the earliest start date\n",
					xaddr, PRINT_POWER);
				fprintf(mbfp, "%s as %s set the earliest start date\n",
					raddr, PRINT_POWER);
				mfprintf(bfp, "for game '%s' to %s.\n", dipent.name, ptime(&dates));
				if (dipent.deadline < dipent.start) {
					dipent.deadline = dipent.start;
					fprintf(rfp, "Deadline set to %s.\n", ptime(&dates));
					mfprintf(bfp, "Deadline set to %s.\n", ptime(&dates));
				}
				if (dipent.grace < dipent.deadline) {
					dipent.grace = dipent.deadline + 48 * 60 * 60;
					if (dipent.flags & F_GRACEDAYS) {
						for (k = 0; k < 8; k++) {
							tm = localtime(&dipent.grace);
							if (seq->days[tm->tm_wday] == '-')
								dipent.grace += 24 * 60 * 60;
							else if (islower(seq->days[tm->tm_wday]) && tm->tm_hour < 12)
								dipent.grace += (12 - tm->tm_hour) * 60 * 60 - tm->tm_min * 60 - tm->tm_sec;
							else
								break;
						}
					}
					fprintf(rfp, "Grace period deadline set to %s.\n",
						ptime(&dipent.grace));
					mfprintf(bfp, "Grace period deadline set to %s.\n",
						 ptime(&dipent.grace));
				}
				fprintf(rfp, "\n");
				fprintf(rfp, "\n");
				broadcast = 1;
			}
			s = "";
			break;

		case SET_WAIT:
			if (signedon < 0)
				fprintf(rfp, "Set wait: no point!\n\n");
			else {
				if (read_phase > 0)
					fprintf(pfp, "%c: set wait\n", name[0]);
				else {
					if (dipent.flags & F_STRWAIT) {
						if ((dipent.players[player].status & SF_MOVE) ||
						    (dipent.players[player].power == MASTER)) {
							fprintf(rfp, "'Wait' status set.\n\n");
							dipent.players[player].status |= SF_WAIT;
						} else
							fprintf(rfp, "You are not allowed to set wait!\n\n");
					} else {
						fprintf(rfp, "'Wait' status set.\n\n");
						dipent.players[player].status |= SF_WAIT;
					}
				}
			}
			break;

		case SET_ACCESS:
			s = lookfor(s, accesses, naccess, &i);
			if (!i) {
				fprintf(rfp, "Invalid access: %s", s);
			} else {
				if (!strncasecmp(s, "-site", 5))
					s += 5;
				dipent.access = avalue[i];
				fprintf(rfp, "Access for game '%s' set to %s-site.\n", dipent.name,
					accesses[avalue[i]]);
				pprintf(cfp, "%s%s as %s in '%s' set the\naccess mode to '%s-site'.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power],
				   dipent.name, accesses[dipent.access]);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the\naccess mode to '%s-site'.\n",
					xaddr, PRINT_POWER, dipent.name, accesses[dipent.access]);
				fprintf(mbfp, "%s as %s in '%s' set the\naccess mode to '%s-site'.\n",
					raddr, PRINT_POWER, dipent.name, accesses[dipent.access]);
				broadcast = 1;
			}
			break;

		case SET_DEDICATE:
			i = atoi(s);
			while (isdigit(*s) || *s == '-' || *s == '+')
				s++;
			if (i == dipent.dedicate) {
				fprintf(rfp, "Set Dedication %d: It already was!\n", i);
			} else if (i > ded[dipent.players[player].userid].r) {
				fprintf(rfp, "You cannot set dedication to %d which is above your ", i);
				fprintf(rfp, "own of %d.\n", ded[dipent.players[player].userid].r);
			} else {
				dipent.dedicate = i;
				fprintf(rfp, "Dedication level for game '%s' set to %d.\n", dipent.name,
					dipent.dedicate);
				pprintf(cfp, "%s%s as %s in '%s' set the\ndedication level to %d.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power],
					dipent.name, dipent.dedicate);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the\ndedication level to %d.\n",
					xaddr, PRINT_POWER, dipent.name, dipent.dedicate);
				fprintf(mbfp, "%s as %s in '%s' set the\ndedication level to %d.\n",
					raddr, PRINT_POWER, dipent.name, dipent.dedicate);
				broadcast = 1;
			}
			break;

		case SET_ONTIMERAT:
		f = (float)atof(s);
			while (isdigit(*s) || *s == '.' || *s == '+')
						s++;
			if(get_data(dipent.players[player].userid,total) != 0)  
			{
				temprat = 1.0 * get_data(dipent.players[player].userid,ontime) / get_data(dipent.players[player].userid,total);
			} else temprat = 1;
			if(f > 1 || f < 0)
			{
				fprintf(rfp,"set ontimerat %.3f\n",f);
				fprintf(rfp,"Ontime ratio must be between 0 an 1.\n");
			} else if(f == dipent.orded) {
				fprintf(rfp,"The ontime ratio already is %.3f!\n",dipent.orded);	
			} else if(f > temprat) {
				fprintf(rfp,"set ontimerat %.3f\n",f);
				fprintf(rfp,"You may not set an ontime ratio higher than your own.\n");
				fprintf(rfp,"Use 'get dedication' to check your ratio.\n");
			} else {
				dipent.orded = f;
				fprintf(rfp,"Ontime ratio in game '%s' set to %.3f.\n",dipent.name,dipent.orded);
				pprintf(cfp,"%s%s as %s in '%s' set the\nontime ratio to %.3f.\n",
					NowString(),
					xaddr,powers[dipent.players[player].power],
					dipent.name,dipent.orded);
				fprintf(bfp,"%s as %s in '%s' set the\nontime ratio to %.3f.\n",
					xaddr,PRINT_POWER,dipent.name,dipent.orded);
				fprintf(mbfp,"%s as %s in '%s' set the\nontime ratio to %.3f.\n",
					xaddr,PRINT_POWER,dipent.name,dipent.orded);
				broadcast = 1;
				}
				break;

		case SET_RESRAT:
			f = (float)atof(s);
			while (isdigit(*s) || *s == '.' || *s == '+')
				s++;
			if(get_data(dipent.players[player].userid,started) == 0 && get_data(dipent.players[player].userid,tookover) == 0)
			{
				temprat = 0;
			}
			else
			{
				temprat = 1.0 * get_data(dipent.players[player].userid,resigned) / (get_data(dipent.players[player].userid,started) + get_data(dipent.players[player].userid,total));
			}
			if(f > 1 || f < 0)
			{
				fprintf(rfp,"set resrat %.3f\n",f);
				fprintf(rfp,"Ratio value must be between 0 and 1.\n");
			} else if(f == dipent.rrded) {
				fprintf(rfp,"The CD ratio is already %.3f.\n", dipent.rrded);
			} else if(f < temprat) {
				fprintf(rfp,"set resrat %.3f\n",f);
				fprintf(rfp,"You may not set a minimum CD ratio lower than your own.\n");
				fprintf(rfp,"Use 'get dedication' to check your record.\n");
			} else {
				dipent.rrded = f;
				fprintf(rfp,"CD ratio in game '%s' set to %.3f.\n",dipent.name,dipent.rrded);
				pprintf(cfp,"%s%s as %s in '%s' set the\nCD ratio to %.3f.\n",
					NowString(),
					xaddr,powers[dipent.players[player].power],
					dipent.name,dipent.rrded);
				fprintf(bfp,"%s as %s in '%s' set the\nCD ratio to %.3f.\n",
					xaddr,PRINT_POWER,dipent.name,dipent.rrded);
				fprintf(mbfp,"%s as %s in '%s' set the\nCD ratio to %.3f.\n",
					xaddr,PRINT_POWER,dipent.name,dipent.rrded);
				broadcast = 1;
				}
				break;

		case SET_LEVEL:
			s = lookfor(s, levels, nlevel, &i);
			if (!i) {
				fprintf(rfp, "Invalid level: %s", s);
			} else {
				dipent.level = lvalue[i];
				fprintf(rfp, "Level for game '%s' set to %s.\n", dipent.name,
				      (temp = printlevel(dipent.level)));
				pprintf(cfp, "%s%s as %s in '%s' set the\nlevel to %s.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power],
					dipent.name, temp);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the\nlevel to %s.\n",
				  xaddr, PRINT_POWER, dipent.name, temp);
				fprintf(mbfp, "%s as %s in '%s' set the\nlevel to %s.\n",
				  raddr, PRINT_POWER, dipent.name, temp);
				free(temp);
				broadcast = 1;
			}
			break;

		case SET_NMR:
			if (dipent.flags & F_NONMR) {
				dipent.flags ^= F_NONMR;
				fprintf(rfp, "Game '%s' is no longer NoNMR.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' cleared the NoNMR flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the NoNMR flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the NoNMR flag.\n",
					raddr, PRINT_POWER, dipent.name);
			} else {
				fprintf(rfp, "Game '%s' is already set NMR.\n", dipent.name);
			}
			chk24nmr++;
			break;

		case SET_NONMR:
			if (dipent.flags & F_NONMR) {
				fprintf(rfp, "Game '%s' is already set NoNMR.\n", dipent.name);
			} else {
				dipent.flags |= F_NONMR;
				fprintf(rfp, "Game '%s' is now NoNMR.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' set the NoNMR flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the NoNMR flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the NoNMR flag.\n",
					raddr, PRINT_POWER, dipent.name);
				mfprintf(bfp, "Orders will not be processed until all powers ");
				mfprintf(bfp, "have submitted complete orders.\n\n");
			}
			break;

		case SET_LIST:
			if (dipent.flags & F_NOLIST) {
				dipent.flags ^= F_NOLIST;
				fprintf(rfp, "Game '%s' is no longer NoList.\n", dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the NoList flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the NoList flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			} else {
				fprintf(rfp, "Game '%s' is already set List.\n", dipent.name);
			}
			break;

		case SET_NOLIST:
			if (dipent.flags & F_NOLIST) {
				fprintf(rfp, "Game '%s' is already set NoList.\n", dipent.name);
			} else {
				dipent.flags |= F_NOLIST;
				fprintf(rfp, "Game '%s' is now NoList.\n", dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the NoList flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the NoList flag.\n",
					raddr, PRINT_POWER, dipent.name);
				mfprintf(bfp, "The game will not be listed in the standard catalogue ");
				mfprintf(bfp, "of available games.\n\n");
				broadcast = 1;
			}
			break;

		case SET_MODERATE:
			if (dipent.flags & F_MODERATE) {
				fprintf(rfp, "Game '%s' is already set moderated.\n", dipent.name);
			} else {
				dipent.flags |= F_MODERATE;
				fprintf(rfp, "Game '%s' is now moderated.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' set the moderated flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the moderated flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the moderated flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			}
			break;

		case SET_UNMODERATE:
			if (dipent.flags & F_MODERATE) {
				dipent.flags ^= F_MODERATE;
				fprintf(rfp, "Game '%s' is no longer moderated.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' cleared the moderated flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the moderated flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the moderated flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			} else {
				fprintf(rfp, "Game '%s' is already set unmoderated.\n", dipent.name);
			}
			break;

		case SET_NOSHOW:
			if (dipent.flags & F_NOSHOW) {
				fprintf(rfp, "Game '%s' is already set no show.\n", dipent.name);
			} else {
				dipent.flags |= F_NOSHOW;
				fprintf(rfp, "Game '%s' will not reveal powers in broadcasts.\n",
					dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' set the no show flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the no show flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the no show flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			}
			break;

		case SET_SHOW:
			if (dipent.flags & F_NOSHOW) {
				dipent.flags ^= F_NOSHOW;
				fprintf(rfp, "Game '%s' is no longer set no show.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' cleared the no show flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the no show flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the no show flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			} else {
				fprintf(rfp, "Game '%s' is already set to show powers in broadcasts.\n",
					dipent.name);
			}
			break;

		case SET_DRAW:
			if (check_can_vote(player, DRAW_VOTE))
				break;
			if (dipent.flags & F_NODIAS) {

				for (t = s; *s && !isspace(*s) && *s != ','; s++);
				if (*s)
					*s++ = '\0';
				while (isspace(*s))
					s++;
				if (!*t || *t == '\n') {
					fprintf(rfp, "This game is set NoDIAS. You must specify a power%s",
					   " list to vote on a draw.\n");
				} else if (chkdraw(t)) {
					fprintf(rfp, "Draw list '%s' not set.\n", t);
					if (dipent.players[player].status & SF_DRAW) {
						fprintf(rfp, "Draw list remains '%s'.\n",
							dipent.players[player].pref);
					}
				} else {
					fprintf(rfp, "Draw list '%s' set.\n", t);
					strcpy(dipent.players[player].pref, t);
					dipent.players[player].status |= SF_DRAW;
					process_draw();
				}
			} else {
				fprintf(rfp, "You will now accept a DIAS draw.\n");
				dipent.players[player].status |= SF_DRAW;
				process_draw();
			}
			break;
		case SET_NODRAW:
			if (check_can_vote(player,DRAW_VOTE))
				break;
			if (dipent.flags & F_NODIAS)
				fprintf(rfp, "You will now only accept a concession.\n");
			else
				fprintf(rfp, "You will now not accept a DIAS draw.\n");
			dipent.players[player].status &= ~SF_DRAW;
			break;

		case SET_CONC:
			if(check_can_vote(player, CONC_VOTE))
				break;
			if(dipent.flags & F_NODIAS)
			{
				fprintf(rfp,"It is pointless to vote for a concession in a NoDIAS game\n");
				fprintf(rfp,"Simply vote for a one player draw!\n\n");
				fprintf(rfp, "Send 'get draws' for more details.\n");
				break;
			}
			for (t = s; *s && !isspace(*s) && *s != ','; s++);
                                if (*s)
                                        *s++ = '\0';
                                while (isspace(*s))
                                        s++;
                                if (!*t || *t == '\n') {
                                        fprintf(rfp, "You must specify the largest power on the board as the power\nyou are proposing the concession to.\n");
                                } else if (chkconc(t)) {
                                        fprintf(rfp, "Concession not set.\n");
                                } else {
					fprintf(rfp, "Concession to '%s' set.\n", t);
					dipent.players[player].status |= SF_CONC;
					process_conc();
			}
			break;

		case SET_NOCONC:
			if (check_can_vote(player, CONC_VOTE))
                                break;
			fprintf(rfp,"You will no longer accept a concession to the largest power on the board.\n");
                        dipent.players[player].status &= ~SF_CONC;
                        break;
		
		case SET_DIAS:
			if (dipent.flags & F_NODIAS) {
				dipent.flags ^= F_NODIAS;
				fprintf(rfp, "Draws must now include all survivors. All draw%s",
					" flags cleared.\n");
				pprintf(cfp, "%s%s as %s in '%s' set the DIAS flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the DIAS flag.\nDraws must now %s %s",
					xaddr, PRINT_POWER, dipent.name,
					"include all survivors.\nReminder: all draw flags",
					"are cleared when the DIAS flag changes.\n");
				fprintf(mbfp, "%s as %s in '%s' set the DIAS flag.\nDraws must now %s %s",
					raddr, PRINT_POWER, dipent.name,
					"include all survivors.\nReminder: all draw flags",
					"are cleared when the DIAS flag changes.\n");
				broadcast = 1;
				for (i = 0; i < dipent.n; i++)
					dipent.players[i].status &= ~SF_DRAW;
			} else {
				fprintf(rfp, "Game '%s' is already DIAS.\n", dipent.name);
			}
			break;
		case SET_NODIAS:
			if (dipent.flags & F_NODIAS) {
				fprintf(rfp, "Game '%s' is already no-DIAS.\n", dipent.name);
			} else {
				dipent.flags ^= F_NODIAS;
				fprintf(rfp, "Draws need not now include all survivors.\n%s",
					"All draw flags cleared.\n");
				pprintf(cfp, "%s%s as %s in '%s' cleared the DIAS flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the DIAS flag.\nDraws need %s %s",
					xaddr, PRINT_POWER, dipent.name,
					"not now include all survivors.\nReminder: all draw",
					"flags are cleared when the DIAS flag changes.\n");
				fprintf(mbfp, "%s as %s in '%s' cleared the DIAS flag.\nDraws need %s %s",
					raddr, PRINT_POWER, dipent.name,
					"not now include all survivors.\nReminder: all draw",
					"flags are cleared when the DIAS flag changes.\n");
				broadcast = 1;
				for (i = 0; i < dipent.n; i++)
					dipent.players[i].status &= ~SF_DRAW;
			}
			break;

		case SET_CONCESSIONS:
			if(dipent.flags & F_NODIAS)
			{
				fprintf(rfp,"NODIAS game already allow concessions!\n");
				break;
			}
			if(dipent.xflags & XF_NOCONCESSIONS)
			{
				dipent.xflags ^= XF_NOCONCESSIONS;
				fprintf(rfp,"Concessions are now permitted in %s.\n",dipent.name);
				pprintf(cfp,"%s%s as %s in '%s' set the concessions flag.\nPowers may concede the game to the largest power on the board.\n",
					NowString(),xaddr,
					powers[dipent.players[player].power],dipent.name);
				fprintf(bfp,"%s as %s in '%s' set the concessions flag.\nPowers may now concede to the largest power on the board.\n",
					xaddr,PRINT_POWER,dipent.name);
				fprintf(mbfp,"%s as %s in '%s' set the concession flag.\nPowers may now concede to the largest power on the board.\n",
					xaddr,PRINT_POWER,dipent.name);
                                broadcast = 1;
			}
			else
			{
				fprintf(rfp,"Game %s already allows concessions.\n",dipent.name);
			}
			break;

		case SET_NOCONCESSIONS:
			if(dipent.xflags & XF_NOCONCESSIONS)
			{
				fprintf(rfp,"Game %s is already set to disallow concessions!\n", dipent.name);
			} else {
				dipent.xflags ^= XF_NOCONCESSIONS;
				fprintf(rfp,"Concessions are now disallowed.\n");
				pprintf(cfp,"%s%s as %s in '%s' disallowed concessions.\n",
					NowString(),xaddr,powers[dipent.players[player].power],dipent.name);
				fprintf(bfp,"%s as %s in '%s' disallowed concessions.\n",
					xaddr,PRINT_POWER,dipent.name);
				fprintf(mbfp,"%s as %s in '%s' disallowed concessions.\n",
				xaddr,PRINT_POWER,dipent.name);
				broadcast = 1;
			}
			break;


		case SET_NOREVEAL:
			if (dipent.flags & F_NOREVEAL) {
				fprintf(rfp, "Game '%s' is already set no reveal.\n", dipent.name);
			} else {
				dipent.flags |= F_NOREVEAL;
				fprintf(rfp, "Game '%s' will not reveal players on termination.\n",
					dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' set the no reveal flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the no reveal flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the no reveal flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			}
			break;

		case SET_REVEAL:
			if (dipent.flags & F_NOREVEAL) {
				dipent.flags ^= F_NOREVEAL;
				fprintf(rfp, "Game '%s' is no longer no reveal.\n", dipent.name);
				pprintf(cfp, "%s%s as %s in '%s' cleared the no reveal flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the no reveal flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the no reveal flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			} else {
				fprintf(rfp, "Game '%s' is already set to reveal players.\n",
					dipent.name);
			}
			break;

		case SET_VARIANT:
			s = lookforv(s, variants, NVARIANT + NVAROPTS,&i,1);
			if (!i) {
				fprintf(rfp, "Invalid variant: %s", s);
				fputs("Valid variants are:\n", rfp);
				for (i = 1; i < NVARIANT; i++)
					fprintf(rfp, "   %s\n", variants[i]);
				fputs("Options on these variants are:\n", rfp);
				while (i < NVARIANT + NVAROPTS)
					fprintf(rfp, "   %s\n", variants[i++]);
			} else if (dipent.seq[0] != 'x') {
				fprintf(rfp,
					"The variant cannot be changed after the game has started.\n");
			} else {
				if (i >= NVARIANT)
					dipent.flags |= vvalue[i];
				else
					dipent.variant = vvalue[i];
				SETNP(dipent.variant);
				dipent.has_natives = GetNativeIndex();
				if (dipent.flags & F_GUNBOAT &&
				dipent.players[player].power != MASTER) {
					xaddr = someone;
				}
				broad_params = 1;
				dipent.no_of_players = dipent.np; /* reset no of players */
				CheckForGameStart(); /*see if we have quorum */
			}
			break;

               case SET_NOTVARIANT:
                        s = lookforv(s, variants, NVARIANT + NVAROPTS,&i,1);
                        if (!i) {
                                fprintf(rfp, "Invalid variant: %s", s);
                                fputs("Valid variants flags are:\n", rfp);
                                while (i < NVARIANT + NVAROPTS)
                                        fprintf(rfp, "   %s\n", variants[i++]);
                        } else if (dipent.seq[0] != 'x') {
                                fprintf(rfp,
                                        "The variant setting cannot be changed after the game has started.\n");
                        } else {
                                if (i >= NVARIANT) {
                                        dipent.flags &= ~vvalue[i];
                                        if (dipent.flags & F_GUNBOAT &&
                                     dipent.players[player].power != MASTER) {
                                        xaddr = someone;
                                    }
                                    broad_params = 1;
                                } else {
                                    fprintf(rfp, "Cannot remove variant: try setting to another variant.\n");
                                }
                        }
                        break;



		case SET_FLAG:
			fprintf(rfp, "Flag word used2b %x.\n", dipent.flags);
			sscanf(s, "%x", &dipent.flags);
			fprintf(rfp, "Flag word set to %x.\n", dipent.flags);
			break;

		case SET_XFLAG:
                        fprintf(rfp, "xFlag word used2b %x.\n", dipent.xflags);
                        sscanf(s, "%x", &dipent.xflags);
                        fprintf(rfp, "Flag word set to %x.\n", dipent.xflags);
                        break;

		case SET_WHITE:
			SETFLAGS(0, F_GREY | F_NOWHITE | F_DEFWHITE);
			broad_params = 1;
			break;

		case SET_WHTGRY:
			SETFLAGS(F_GREY | F_DEFWHITE, F_GREY | F_NOWHITE | F_DEFWHITE);
			broad_params = 1;
			break;

		case SET_GREY:
			SETFLAGS(F_GREY | F_NOWHITE, F_GREY | F_NOWHITE | F_DEFWHITE);
			broad_params = 1;
			break;

		case SET_GRYWHT:
			SETFLAGS(F_GREY, F_GREY | F_NOWHITE | F_DEFWHITE);
			broad_params = 1;
			break;

		case SET_NOPRESS:
			SETFLAGS(F_NOWHITE, F_GREY | F_NOWHITE | F_DEFWHITE);
	/* Intentionally left blank for fall-thru */
		case SET_OBNONE:
			SETFLAGS(F_OBNONE, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_NOPART:
			SETFLAGS(F_NOPARTIAL, F_NOPARTIAL);
	/* Intentionally left blank for fall-thru */
		case SET_OBWHITE:
			SETFLAGS(F_OBWHITE, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_PART:
			SETFLAGS(0, F_NOPARTIAL);
			/* if there's no broadcast, set it */
			if (dipent.flags & F_NOWHITE) SETFLAGS(0, F_NOWHITE);
	/* Intentionally left blank for fall-thru */
		case SET_OBANY:
			SETFLAGS(0, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_FAKEB:
			SETFLAGS(0 | F_DEFFAKE, F_FAKE | F_DEFFAKE | F_NOPARTIAL);
			broad_params = 1;
			break;

		case SET_NOFAKEB:
			SETFLAGS(0 | 0, F_FAKE | F_DEFFAKE);
			broad_params = 1;
			break;

		case SET_PARTFBD:
			SETFLAGS(F_FAKE | F_DEFFAKE, F_FAKE | F_DEFFAKE | F_NOPARTIAL);
			broad_params = 1;
			break;

		case SET_PARTMAY:
			SETFLAGS(F_FAKE | 0, F_FAKE | F_DEFFAKE | F_NOPARTIAL);
			broad_params = 1;
			break;

		case SET_QUIET:
			SETFLAGS(F_QUIET, F_QUIET);
			broad_params = 1;
			break;

		case SET_NOQUIET:
			SETFLAGS(0, F_QUIET);
			broad_params = 1;
			break;

		case SET_RATE:
			SETFLAGS(0, F_NORATE);
			broad_params = 1;
			break;

		case SET_NORATE:
			SETFLAGS(F_NORATE, F_NORATE);
			broad_params = 1;
			break;

		case SET_NOPROXY:
			SETFLAGS(0, F_PROXY);
			broad_params = 1;
			break;

		case SET_PROXY:
			SETFLAGS(F_PROXY, F_PROXY);
			broad_params = 1;
			break;

		case SET_NODICE:
			CheckMach();
			SETFLAGS(F_NODICE | F_NOFAMINE | F_NOPLAGUE | F_NOLOANS | F_NOASSASS,
				 F_NODICE | F_NOFAMINE | F_NOPLAGUE | F_NOLOANS | F_NOASSASS);
			dipent.xflags &= ~XF_STORMS;
			broad_params = 1;
			break;

		case SET_DICE:
			CheckMach();
			SETFLAGS(0, F_NODICE);
			broad_params = 1;
			break;

		case SET_NOFAMINE:
			CheckMach();
                        SETFLAGS(F_NOFAMINE, F_NOFAMINE);
			broad_params = 1;
			break;

		case SET_FAMINE:
			CheckMach();
                        SETFLAGS(0, F_NOFAMINE | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOPLAGUE:
			CheckMach();
                        SETFLAGS(F_NOPLAGUE, F_NOPLAGUE);
			broad_params = 1;
			break;

		case SET_PLAGUE:
			CheckMach();
                        SETFLAGS(0, F_NOPLAGUE | F_NODICE);
                        broad_params = 1;
                        break;

		case SET_NOLOANS:
			CheckMach();
                        SETFLAGS(F_NOLOANS, F_NOLOANS);
			broad_params = 1;
			break;

		case SET_LOANS:
			CheckMach();
                        SETFLAGS(0, F_NOLOANS | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOASSASS:
			CheckMach();
                        SETFLAGS(F_NOASSASS, F_NOASSASS);
			broad_params = 1;
			break;

		case SET_ASSASS:
			CheckMach();
                        SETFLAGS(0, F_NOASSASS | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOADJ:
			CheckMach();
                        SETFLAGS(F_NOADJ, F_NOADJ);
			broad_params = 1;
			break;

		case SET_ADJ:
			CheckMach();
                        SETFLAGS(0, F_NOADJ);
			broad_params = 1;
			break;

		case SET_NOSPECIAL:
			CheckMach();
                        SETFLAGS(F_NOSPECIAL, F_NOSPECIAL);
			broad_params = 1;
			break;

		case SET_SPECIAL:
			CheckMach();
                        SETFLAGS(0, F_NOSPECIAL);
			broad_params = 1;
			break;

		case SET_ALLPRESS:
			CheckPress();
			if (!(dipent.xflags & XF_WATCHPRESS)) {
				if (dipent.players[player].power != MASTER) {
					fprintf(rfp, "Sorry, only the master can request all press in game '%s'.\n", 
						dipent.name);
					break;
				}
			} else {
				if (dipent.players[player].power != MASTER && dipent.players[player].power != OBSERVER ) {
				fprintf(rfp, "Sorry, players cannot request all press.\n");
					break;
				}
			}
			dipent.players[player].status |= SF_PRESS;
			fprintf(rfp, "You will now receive all the partial press.  Be sure ");
			fprintf(rfp, "to pay attention to\nnot leak any information thus ");
			fprintf(rfp, "gained to any of the players.\n");
			if (dipent.players[player].power != MASTER) {
			    /* Let everyone know this person is watching press */
                                fprintf(bfp, "%s is now watching all press sent (including partial).\n",
                                        xaddr);
                                fprintf(mbfp, "%s is now watching all press sent (including partial).\n",
                                        raddr);
                            broadcast = 1;

			}
			break;

		case SET_NOALLPRESS:
			CheckPress();
			dipent.players[player].status &= ~SF_PRESS;
			fprintf(rfp, "You will now receive only the normal amount of press.\n");
			if (dipent.players[player].power != MASTER) {
                            /* Let everyone know this person is not watching press */
                                fprintf(bfp, "%s is no longer watching all press sent.\n",
                                        xaddr);
                                fprintf(mbfp, "%s is no longer watching all press sent.\n",
                                        raddr);
                            broadcast = 1;

                        }
\
			break;

		case SET_EPNUM:
			while (isspace(*s))
				s++;
			t = dipent.epnum;
			while (*s && *s != '\n'
			  && t < dipent.epnum + sizeof(dipent.epnum) - 1)
				*t++ = *s++;
			*t++ = '\0';
			if (strlen(dipent.epnum)) {
				fprintf(rfp, "EP number set to: %s\n", dipent.epnum);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the EP number to %s.\n",
					xaddr, PRINT_POWER, dipent.name, dipent.epnum);
				fprintf(mbfp, "%s as %s in '%s' set the EP number to %s.\n",
					raddr, PRINT_POWER, dipent.name, dipent.epnum);
			} else {
				fprintf(rfp, "EP number cleared.\n");
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the EP number.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the EP number.\n",
					raddr, PRINT_POWER, dipent.name);
			}
			broadcast = 1;

			/*
			 * Force the summaries to get the new EP number.
			 */
			{
				char line[150];
				sprintf(line, "%s%s/summary", GAME_DIR, dipent.name);
				remove(line);
				sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
				remove(line);
			}
			break;

		case SET_BN_MN:
			while (isspace(*s))
				s++;
			t = dipent.bn_mnnum;
			while (*s && *s != '\n'
			       && t < dipent.bn_mnnum + sizeof(dipent.bn_mnnum) - 1)
				*t++ = *s++;
			*t++ = '\0';
			if (strlen(dipent.bn_mnnum) > 7) {
				fprintf(rfp, "Miller number set to: %s\n", dipent.bn_mnnum);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the Miller number to %s.\n",
					xaddr, PRINT_POWER, dipent.name, dipent.bn_mnnum);
				fprintf(mbfp, "%s as %s in '%s' set the Miller number to %s.\n",
					raddr, PRINT_POWER, dipent.name, dipent.bn_mnnum);
			} else {
				if (strlen(dipent.bn_mnnum) <= 7) {
					fprintf(rfp, "Boardman number set to: %s\n", dipent.bn_mnnum);
					/* WAS mfprintf  1/95 BLR */
					fprintf(bfp, "%s as %s in '%s' set the Boardman number to %s.\n",
						xaddr, PRINT_POWER, dipent.name, dipent.bn_mnnum);
					fprintf(mbfp, "%s as %s in '%s' set the Boardman number to %s.\n",
						raddr, PRINT_POWER, dipent.name, dipent.bn_mnnum);
				} else {
					fprintf(rfp, "Boardman/Miller number cleared.\n");
					/* WAS mfprintf  1/95 BLR */
					fprintf(bfp, "%s as %s in '%s' cleared the Boardman\\Miller number.-n",
					xaddr, PRINT_POWER, dipent.name);
					fprintf(mbfp, "%s as %s in '%s' cleared the Boardman\\Miller number.-n",
					raddr, PRINT_POWER, dipent.name);
				}
			}
			broadcast = 1;

			/*
			   **  Force the summaries to get the new Boardman\Miller number.
			 */
			{
				char line[150];
				sprintf(line, "%s%s/summary", GAME_DIR, dipent.name);
				remove(line);
				sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
				remove(line);
			}
			break;

		case SET_COMMENT:
			while (isspace(*s))
				s++;
			if (strncasecmp(s, "begin", 5)) {

				t = dipent.comment;
				while (*s && *s != '\n'
				       && t < dipent.comment + sizeof(dipent.comment) - 2) {
					*t++ = *s++;
				}
				*t++ = '\0';
				fprintf(rfp, "Brief comment set: [%s].\n", dipent.comment);

			} else {
				int n, skipping;
				char line[150];

				sprintf(line, "%s%s/info", GAME_DIR, dipent.name);
				if (!(tfp = fopen(line, "w"))) {
					perror(line);
					fprintf(log_fp, "Can't open info file: %s", line);
					bailout(1);
				}
				fprintf(rfp, "----- Full comment set:\n");
				for (n = skipping = 0; fgets(line, sizeof(line), inp); n++) {
					fputs(line, log_fp);
					for (t = line; isspace(*t); t++);
					if (!strncasecmp(t, "signoff", 7)) {
						skipping++;
					} else if (!skipping) {
						if (*line == '~') {
							fputc('~', rfp);
							fputc('~', tfp);
						}
						fputs(line, rfp);
						fputs(line, tfp);
					}
				}
				fprintf(rfp, "---- End of comment.\n");
				if (!n)
					remove(line);
				fclose(tfp);

				/*
				   **  Force the summaries to get the new comment.
				 */

				sprintf(line, "%s%s/summary", GAME_DIR, dipent.name);
				remove(line);
				sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
				remove(line);

			}
			*s = '\0';
			break;

		case SET_CENTERS:
			i = atoi(s);
			while (isdigit(*s))
				s++;
			if (i == dipent.vp) {
				fprintf(rfp, "Set Centers %d: It already was!\n", i);
			} else {
				dipent.vp = i;
				fprintf(rfp, "Winning Centers for game '%s' set to %d.\n", dipent.name,
					dipent.vp);
				pprintf(cfp, "%s%s as %s in '%s' set the winning centers to %d.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power],
					dipent.name, dipent.vp);
				fprintf(bfp, "%s as %s in '%s' set the winning centers to %d.\n",
					xaddr, PRINT_POWER,
					dipent.name, dipent.vp);
				broadcast = 1;
				sprintf(subjectline, "%s:%s - %s Winning Centers Changed to %d",
					JUDGE_CODE, dipent.name, dipent.phase, dipent.vp);
			}
			break;

		case SET_GRACEDAYS:
			if (dipent.flags & F_GRACEDAYS) {
				fprintf(rfp, "Game '%s' is already NoStrictGrace.\n", dipent.name);
			} else {
				dipent.flags |= F_GRACEDAYS;
				fprintf(rfp, "Grace periods will now only expire on normal %s",
					"order processing days for each phase.\n");
				pprintf(cfp, "%s%s as %s in '%s' cleared the StrictGrace flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the StrictGrace flag.\nGrace %s %s",
					xaddr, PRINT_POWER, dipent.name,
				"periods will now only expire on normal",
					"order processing days.\n");
				fprintf(mbfp, "%s as %s in '%s' cleared the StrictGrace flag.\nGrace %s %s",
					raddr, PRINT_POWER, dipent.name,
				"periods will now only expire on normal",
					"order processing days.\n");
				broadcast = 1;
			}
			break;

		case SET_NOGRACEDAYS:
			if (dipent.flags & F_GRACEDAYS) {
				dipent.flags ^= F_GRACEDAYS;
				fprintf(rfp, "Grace periods will now expire in an\n%s %s",
				  "absolute time period which might NOT",
					"end\non a normal order processing day.\n");
				pprintf(cfp, "%s%s as %s in '%s' set the StrictGrace flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the StrictGrace flag.\nGrace periods %s %s",
					xaddr, PRINT_POWER, dipent.name,
					"will now expire in an\nabsolute time period which might",
					"NOT end\non a normal order processing day.\n");
				fprintf(mbfp, "%s as %s in '%s' set the StrictGrace flag.\nGrace periods %s %s",
					raddr, PRINT_POWER, dipent.name,
					"will now expire in an\nabsolute time period which might",
					"NOT end\non a normal order processing day.\n");
				broadcast = 1;
			} else {
				fprintf(rfp, "Game '%s' is already StrictGrace.\n", dipent.name);
			}
			break;
		case SET_MASTERALLOW:
			process_allowdeny(&s, "masters.ALLOW");
			break;
		case SET_PLAYERALLOW:
			process_allowdeny(&s, "players.ALLOW");
			break;
		case SET_MASTERDENY:
			process_allowdeny(&s, "masters.DENY");
			break;
		case SET_PLAYERDENY:
			process_allowdeny(&s, "players.DENY");
			break;

		case SET_LATECOUNT:
			if (dipent.xflags & XF_LATECOUNT) {
				fprintf(rfp, "Game '%s' is already showing Late Counters.\n", dipent.name);
			} else {
				dipent.xflags |= XF_LATECOUNT;
				fprintf(rfp, "All players will now be shown with their Late Counter.\n");
				pprintf(cfp, "%s%s as %s in '%s' set the LateCount flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' set the LateCount flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' set the LateCount flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			}
			break;

		case SET_NOLATECOUNT:
			if (!(dipent.xflags & XF_LATECOUNT)) {
				fprintf(rfp, "Game '%s' is already not showing Late Counters.\n", dipent.name);
			} else {
				dipent.xflags &= ~XF_LATECOUNT;
				fprintf(rfp, "No player will now be shown with their Late Counter.\n");
				pprintf(cfp, "%s%s as %s in '%s' cleared the LateCount flag.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power], dipent.name);
				/* WAS mfprintf  1/95 BLR */
				fprintf(bfp, "%s as %s in '%s' cleared the LateCount flag.\n",
					xaddr, PRINT_POWER, dipent.name);
				fprintf(mbfp, "%s as %s in '%s' cleared the LateCount flag.\n",
					raddr, PRINT_POWER, dipent.name);
				broadcast = 1;
			}
			break;

		case SET_NO_PLAYERS:
			i = atoi(s);
			while (isdigit(*s))
				s++;
			if (i == dipent.no_of_players) {
				fprintf(rfp, "Set Players %d: It already was!\n", i);
			} else if ((i < 1) || (i > dipent.np)) {
				fprintf(rfp,
					"Set Players %d: Must be in the range 1 to %d for this variant\n",
					i, dipent.np);
			} else {
				dipent.no_of_players = i;
				fprintf(rfp, "Number of Players for game '%s' set to %d.\n",
				      dipent.name, dipent.no_of_players);
				pprintf(cfp, "%s%s as %s in '%s' set the number of players to %d.\n",
					NowString(),
					xaddr, powers[dipent.players[player].power],
				      dipent.name, dipent.no_of_players);
				fprintf(bfp, "%s as %s in '%s' set the number of players to %d.\n",
					xaddr, PRINT_POWER,
				      dipent.name, dipent.no_of_players);
				CheckForGameStart();

			}
			break;

		case SET_STRWAIT:
                        if (dipent.flags & F_STRWAIT) {
                                fprintf(rfp, "Game '%s' is already StrictWait.\n", dipent.name);
                        } else {
                                dipent.flags |= F_STRWAIT;
                                fprintf(rfp, "Only players with moves required can set WAIT.\n");
                                pprintf(cfp, "%s%s as %s in '%s' set the StrictWait flag.\n",
                                        NowString(),
                                        xaddr, powers[dipent.players[player].power], dipent.name);
                                /* WAS mfprintf  1/95 BLR */
                                fprintf(bfp, "%s as %s in '%s' set the StrictWait flag.\n",
                                        xaddr, PRINT_POWER, dipent.name);
                                fprintf(mbfp, "%s as %s in '%s' set the StrictWait flag.\n",
                                        raddr, PRINT_POWER, dipent.name);
                                broadcast = 1;
                        }
                        break;

                case SET_NOSTRWAIT:
                        if (!(dipent.flags & F_STRWAIT)) {
                                fprintf(rfp, "Game '%s' is already NoStrictWait.\n", dipent.name);
                        } else {
                                dipent.flags &= ~F_STRWAIT;
                                fprintf(rfp, "Any player can set WAIT.\n");
                                pprintf(cfp, "%s%s as %s in '%s' cleared the StrictWait flag.\n",
                                        NowString(),
                                        xaddr, powers[dipent.players[player].power], dipent.name);
                                /* WAS mfprintf  1/95 BLR */
                                fprintf(bfp, "%s as %s in '%s' cleared the StrictWait flag.\n",
                                        xaddr, PRINT_POWER, dipent.name);
                                fprintf(mbfp, "%s as %s in '%s' cleared the StrictWait flag.\n",
                                        raddr, PRINT_POWER, dipent.name);
                                broadcast = 1;
                        }
                        break;

		case SET_STRCONVOY:
			CheckNoMach();
			CheckAndToggleFlag(&dipent.xflags,  XF_STRCONVOY, "StrictConvoy", CATF_SETON,
                                              "Only plausible convoys will now be accepted.\n",CATF_NORMAL);
                        break;

		case SET_NOSTRCONVOY:
                        CheckNoMach();
			CheckAndToggleFlag(&dipent.xflags,  XF_STRCONVOY, "StrictConvoy", CATF_SETOFF,
                                               "Impossible convoys will now be accepted.\n",CATF_NORMAL);
                        break;

                case SET_RESUME:
                        CheckAndToggleFlag(&dipent.xflags,  XF_NORESUME, "Resume", CATF_SETOFF,
                                               "Resume can now be performed by any player.\n",CATF_INVERSE);
                        break;

                case SET_NORESUME:
                        CheckAndToggleFlag(&dipent.xflags,  XF_NORESUME, "Resume", CATF_SETON,
                                               "Resume can now only be performed by moderators.\n",CATF_INVERSE);
                        break;

                case SET_ATTACKTRANS:
                        CheckNoMach();
			if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change Transform settings!\n\n",
                                    dipent.name);
                        } else if (!(dipent.xflags & XF_TRANS_MOVE)) {
                            fprintf(rfp, "Move transformations must be enabled first!\n\n");
                        } else  {
				CheckAndToggleFlag(&dipent.xflags,  XF_NOATTACK_TRANS, "NoAttackTransform", CATF_SETOFF,
                                               "Move transformations will now only fail on dislodged units.\n",CATF_INVERSE);
                            ShowTransformSettings(bfp);
                            ShowTransformSettings(mbfp);
                            ShowTransformSettings(rfp);
                        }
			break;

                case SET_NOATTACKTRANS:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change Transform settings!\n\n",
                                    dipent.name);
                        }
			else if (!(dipent.xflags & XF_TRANS_MOVE)) {
                            fprintf(rfp, "Move transformations must be enabled first!\n\n");
                        } else  {
			    CheckAndToggleFlag(&dipent.xflags,  XF_NOATTACK_TRANS, "AttackTransform", CATF_SETON,
                                               "Move transformations will now also fail on unsuccessful attacks.\n",CATF_INVERSE);
                            ShowTransformSettings(bfp);
                            ShowTransformSettings(mbfp);
                            ShowTransformSettings(rfp);
                        }
			break;

                case SET_AUTODISBAND:
                       if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change AutoDisband flag!\n\n",
                                    dipent.name);
                        } else { 
                        CheckAndToggleFlag(&dipent.xflags,  XF_AUTODISBAND, "AutoDisband", CATF_SETON,
                                               "No retreats are allowed: will be made into automatic disbands.\n",CATF_NORMAL);
                        }
			break;

                case SET_NOAUTODISBAND:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change AutoDisband flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.xflags,  XF_AUTODISBAND, "AutoDisband", CATF_SETOFF,
                                               "Retreats will now be allowed (when possible).\n",CATF_NORMAL);
                        }
			break;


		case SET_NEUTRALS:
			CheckNoMach();
                       if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change Neutrals flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_NEUTRALS, "Neutrals", CATF_SETON,
                                               "Neutral units will now exist in every unowned centre at game start.\n",CATF_NORMAL);
                        }
                        break;

                case SET_NONEUTRALS:
			CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change Neutrals flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_NEUTRALS, "Neutrals", CATF_SETOFF,
                                               "All unowned centres will now be unoccupied at game start.\n",CATF_NORMAL);
                        }
                        break;

		case SET_TOUCHPRESS:
			CheckPress();
                       if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change TouchPress flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_TOUCHPRESS, "TouchPress", CATF_SETON,
                                               "Players can now only send one broadcast per turn and press with adjacent powers.\n",CATF_NORMAL);
                        }
                        break;

                case SET_NOTOUCHPRESS:
			CheckPress();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change TouchPress flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_TOUCHPRESS, "TouchPress", CATF_SETOFF,
                                               "Players can now send unlimited press.\n",CATF_NORMAL);
                        }

		case SET_CAPTUREWIN:
                        CheckNoMach();
                       if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change CaptureWin flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_CAPTUREWIN, "CaptureWin", CATF_SETON,
                                               "Game will now be won when a power captures ALL home SCs of another.\n",CATF_NORMAL);
                        }
                        break;

                case SET_NOCAPTUREWIN:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change CaptureWin flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_CAPTUREWIN, "CaptureWin", CATF_SETOFF,
                                               "Game will now be won when a power captures enough supply centres.\n",CATF_NORMAL);
                        }
                        break;

		case SET_AUTOCREATE:
                       if (dipent.seq[6] == 'X') {
                            fprintf(rfp, "Game '%s' has already finished: not allowed to change AutoCreate flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_AUTOCREATE, "AutoCreate",
CATF_SETON,
                                               "A new game with identical settings will be automatically created when this one finishes.\n",CATF_NORMAL);
                        }
                        break;

                case SET_NOAUTOCREATE:
                        if (dipent.seq[6] == 'X') {
                            fprintf(rfp, "Game '%s' has already finished: not allowed to change AutoCreate flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_AUTOCREATE, "AutoCreate",
CATF_SETOFF,
                                               "No new game will be started when this game finishes.\n",CATF_NORMAL);
                        }
                        break;


		case SET_ANYDISBAND:
			CheckNoMach();
                       if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set AnyDisband flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.xflags,  XF_ANYDISBAND, "AnyDisband", CATF_SETON,
                                               "Disbands can now be made in a build phase of ANY unit.\n",CATF_NORMAL);
                        }
                        break;

                case SET_NORMALDISBAND:
			CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set NormalDisband flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.xflags,  XF_ANYDISBAND, "NormalDisband", CATF_SETON,
                                               "Disbands can now only be made in a build phase if short of units.\n",CATF_INVERSE);
                        }
                        break;

		 case SET_LATEPRESS:
			CheckPress();
			CheckAndToggleFlag(&dipent.xflags,  XF_NOLATEPRESS, "LatePress", CATF_SETOFF,
                                           "Press will be permitted from late countries.\n",
					   CATF_INVERSE);
			break;

                case SET_NOLATEPRESS:
                        CheckPress();
			CheckAndToggleFlag(&dipent.xflags,  XF_NOLATEPRESS, "LatePress", CATF_SETON,
					   "Press will NOT be allowed from late countries.\n",
					    CATF_INVERSE);
			break;

		case SET_BLANKPRESS:
                        CheckPress();
                        CheckAndToggleFlag(&dipent.xflags,  XF_BLANKPRESS, "BlankPress", CATF_SETON,
                                           "Blank Press messages can now be sent.\n",
                                           CATF_NORMAL);
                        break;

                case SET_NOBLANKPRESS:
                        CheckPress();
                        CheckAndToggleFlag(&dipent.xflags,  XF_BLANKPRESS, "BlankPress", CATF_SETOFF,
                                           "No blank press messages will be permitted.\n",
                                            CATF_NORMAL);
                        break;

		case SET_MINORPRESS:
			CheckPress();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change MinorPress flag!\n\n",
                                    dipent.name);
                        } else {
				CheckAndToggleFlag(&dipent.xflags,  XF_NOMINORPRESS, "MinorPress", CATF_SETOFF,
                                           "Press will be permitted during retreat & build phases.\n",
                                           CATF_INVERSE);
			}
                        break;

                case SET_NOMINORPRESS:
                        CheckPress();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change MinorPress flag!\n\n",
                                    dipent.name);
                        } else {
				CheckAndToggleFlag(&dipent.xflags,  XF_NOMINORPRESS, "MinorPress", CATF_SETON,
                                           "Press will NOT be permitted during retreat & build phases.\n",
                                            CATF_INVERSE);
                        }
			break;

		case SET_MACH2:
                        CheckMach();
			if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Mach2 flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_MACH2, "Mach2", CATF_SETON,
                                           "Machiavelli will now be played according to 2nd Edition rules.\n",
                                            CATF_NORMAL);
                        }
			break;

                case SET_NOMACH2:
                         CheckMach();
                       if (dipent.seq[0] != 'x') {
                               fprintf(rfp, "Game '%s' has already started: not allowed to clear Mach2 flag!\n\n",
                                        dipent.name);
                          } else {
                                      CheckAndToggleFlag(&dipent.xflags,  XF_MACH2, "Mach2", CATF_SETOFF,
                                               "Machiavelli will now be played according to 1st Edition rules.\n",
                                        CATF_NORMAL);
                          }
                  break;


                case SET_FORT:
                        CheckMach();
                        CheckNoMach2();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Forts flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_FORT, "Forts", CATF_SETON,
                                           "Machiavelli will now be played with fortresses.\n",
                                            CATF_NORMAL);
                        }
                        break;

                case SET_NOFORT:
                        CheckMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear Forts flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_FORT, "Forts", CATF_SETOFF,
                                           "Machiavelli will now be played with NO fortresses.\n",
                                            CATF_NORMAL);
                        }
                        break;

                case SET_STORM:
                        CheckMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Storms flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_STORMS, "Storms", CATF_SETON,
                                           "Machiavelli will now be played with storms.\n",
                                            CATF_NORMAL);
				/* IF no dice set, set them! */
				if (dipent.flags & F_NODICE) dipent.flags &= ~F_NODICE;
				broad_params = 1;
                        }
                        break;

                case SET_NOSTORM:
                        CheckMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear Storms flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_STORMS, "Storms", CATF_SETOFF,
                                           "Machiavelli will now be played with NO storms.\n",
                                            CATF_NORMAL);
                        }
                        break;

               case SET_COASTALCONVOY:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set CoastalConvoy flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_COASTAL_CONVOYS, "CoastalConvoy", CATF_SETON,
                                           "Coastal convoys will now be permitted.\n",
                                            CATF_NORMAL);
                        }
                        break;

                case SET_NOCOASTALCONVOY:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear CoastalConvoy flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_COASTAL_CONVOYS, "CoastalConvoy", CATF_SETOFF,
                                           "Coastal convoys are now NOT allowed.\n",
                                            CATF_NORMAL);
                        }
                        break;


               case SET_DUALITY:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Duality flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_PROV_DUALITY, "Duality", CATF_SETON,
                                           "Some map specific provinces may now work with dual land and water behaviour.\n",
                                            CATF_NORMAL);
                        }
                        break;
 
                case SET_NODUALITY:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear Duality flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_PROV_DUALITY, "Duality", CATF_SETOFF,
                                           "All map provinces now only have one behaviour.\n",
                                            CATF_NORMAL);
                        }
                        break;              

		case SET_HONGKONG:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to set HongKong flag!\n\n",
				    dipent.name);
/*			} else if (nhk == 0) {
			    fprintf(rfp, "Variant has no HongKong provinces: flag setting ignored!\n\n");
 */			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_HONGKONG, "HongKong", CATF_SETON,
						"HongKong flag set.\n",
						CATF_NORMAL);
			}
			break;

		case SET_NOHONGKONG:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to clear HongKong flag!\n\n",
				dipent.name);
			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_HONGKONG, "NoHongKong", CATF_SETOFF,
					"HongKong flag cleared.\n",
					CATF_NORMAL);
			}
			break;

		case SET_GATEWAY:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to set Gateways flag!\n\n", dipent.name);
/*			} else if (ngw == 0) {
			    fprintf(rfp," Variant has no gatways defined: flag ignored!\n\n");
 */			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_GATEWAYS, "Gateways", CATF_SETON,
				"Gateways are now enabled.\n", CATF_NORMAL);
			}
			break;

		case SET_NOGATEWAY:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to clear Gateways flag!\n\n", dipent.name);
			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_GATEWAYS, "NoGateways", CATF_SETOFF,
				"Gateways are now disabled.\n", CATF_NORMAL);
			}
			break;
			
		case SET_RAILWAY:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to set Railways flag!\n\n", dipent.name);
/*			} else if (nrw == 0) {
			    fprintf(rfp, "Variant has no railways defined: flag ignored!\n\n");
 */			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_RAILWAYS, "Railways", CATF_SETON,
				"Railways are now enabled.\n", CATF_NORMAL);
			}
			break;

		case SET_NORAILWAY:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: not allowed to clear Railways flag!\n\n", dipent.name);
			} else {
			    CheckAndToggleFlag(&dipent.x2flags, X2F_RAILWAYS, "NoRailways", CATF_SETOFF,
			    "Railways are now disabled.\n", CATF_NORMAL);
			}
			break;

/*
                case SET_AIRLIFT:
                        CheckNoMach();
			CheckWings();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Airlifts flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_AIRLIFTS, "Airlifts", CATF_SETON,
                                           "Airlifts will now be allowed.\n",
                                            CATF_NORMAL);
                        }
                        break;

                case SET_NOAIRLIFT:
                        CheckNoMach();
			CheckWings();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear Airlifts flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_AIRLIFTS, "Airlifts", CATF_SETOFF,
                                           "No airlifts are now permitted.\n",
                                            CATF_NORMAL);
                        }
                        break;
*/
		case SET_BLANKBOARD:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set BlankBoard flag!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_BLANKBOARD, "BlankBoard", CATF_SETON,
                                           "Game will now start in a build phase with no units placed.\n",
                                            CATF_NORMAL);
                        }
                        break;

                case SET_BLIND_CENTRES:
                       CheckBlind();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change BlindCentres flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_BLIND_CENTRES, "BCentres", CATF_SETON,
                                               "Blind game will show ALL owned centres.\n",CATF_NORMAL);
                        }
                        break;

                case SET_BLIND_NOCENTRES:
                        CheckBlind();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change BlindCentres flag!\n\n",
                                    dipent.name);
                        } else {
                        CheckAndToggleFlag(&dipent.x2flags,  X2F_BLIND_CENTRES, "BCentres", CATF_SETOFF,
                                               "Blind game will show only power's owned centres.\n",CATF_NORMAL);
                        }
                        break;


		case SET_MANSTART:
                        if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: no point in setting ManualStart flag!\n\n",
				    dipent.name);
			} else {
			    CheckAndToggleFlag(&dipent.xflags,  XF_MANUALSTART, "ManualStart", CATF_SETON,
                                              "Moderator will now have to manually start the game.\n",CATF_NORMAL);
                        }
			break;

                case SET_NOMANSTART:
                        if (dipent.seq[0] != 'x') {
			    fprintf(rfp, "Game '%s' has already started: no point in clearing ManualStart flag!\n\n",
				    dipent.name);
                        } else {
				CheckAndToggleFlag(&dipent.xflags,  XF_MANUALSTART, "ManualStart", CATF_SETOFF,
                                               "The game will now automatically start when a quorum exists.\n",CATF_NORMAL);
				CheckForGameStart(); /* OK, see if we can start game already */
                        }
			break;
               
                case SET_MANPROC:
                        CheckAndToggleFlag(&dipent.xflags,  XF_MANUALPROC, "ManualProcess", CATF_SETON,
                                              "Moderator will now have to manually start turn processing for every turn.\n",CATF_NORMAL);
                        break;

                case SET_NOMANPROC:
                        if (CheckAndToggleFlag(&dipent.xflags,  XF_MANUALPROC, "ManualProcess", CATF_SETOFF,
                                               "Turn processing will now be automatic.\n",CATF_NORMAL)) {
				if (dipent.seq[0] != 'x' ) deadline( ( sequence *) NULL, 0); /* See if time to do a turn */
			}
                        break;

		case SET_TRANSFORM:
			CheckNoMach();
			if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to change Transform settings!\n\n",
                                    dipent.name);
                        } else  {
			    ChangeTransform(s);
			    pprintf(cfp, "%s%s as %s in '%s' changed transform settings.\n", NowString(),
                        xaddr, powers[dipent.players[player].power], dipent.name);
                        fprintf(bfp, "%s as %s in '%s' changed transform settings.\n", xaddr, PRINT_POWER, dipent.name);
			ShowTransformSettings(bfp);
                        fprintf(mbfp, "%s as %s in '%s' changed transform settings.\n", raddr, PRINT_POWER, dipent.name);
			ShowTransformSettings(mbfp);
			ShowTransformSettings(rfp);
                        }
			*s = '\0';
			break;

                case SET_NOTRANSFORM:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to disable Transform settings!\n\n",
                                    dipent.name);
                        } else {
                            ChangeTransform("no"); /* User wants no transformations */
                        }
			    pprintf(cfp, "%s%s as %s in '%s' disabled transformations.\n", NowString(),
                        xaddr, powers[dipent.players[player].power], dipent.name);
                        fprintf(bfp, "%s as %s in '%s' disabled transformations.\n", xaddr, PRINT_POWER, dipent.name);
                        ShowTransformSettings(bfp);
                        fprintf(mbfp, "%s as %s in '%s'  disabled transformations.\n", raddr, PRINT_POWER, dipent.name);
			*s = '\0';
                        break;



		case SET_ANYCENTRE:
                        CheckNoMach();
			if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set AnyCentres!\n\n",
				    dipent.name);
                        } else {
				if (CheckAndToggleFlag(&dipent.xflags,  XF_BUILD_ANYCENTRES, "AnyCentres", CATF_SETON,
                                              "Powers can now build on any vacant owned centre.\n",CATF_NORMAL))
				{
				    dipent.xflags &= ~XF_BUILD_ONECENTRE;
				    dipent.xflags |= XF_BUILD_ANYCENTRES;
				}
			}
                        break;

                case SET_HOMECENTRE:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set HomeCentres!\n\n",
				    dipent.name);
                        } else {
				CheckAndToggleFlag(&dipent.xflags,  XF_BUILD_ONECENTRE, "HomeCentres", CATF_SETOFF,
                                               "Powers can only build on vacant owned home centres.\n",CATF_INVERSE);
                        }
			break;

		case SET_ONECENTRE:
                        CheckNoMach();
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set OneCentre!\n\n",
                                    dipent.name);
                        } else {
                                CheckAndToggleFlag(&dipent.xflags,  XF_BUILD_ONECENTRE, "OneCentre", CATF_SETON,
                                              "Powers can now build on any vacant owned centre\nproviding they own at least one home centre.\n",
						   CATF_NORMAL);
                        }
                        break;


		 case SET_WATCHALL:
			CheckPress();
			if (CheckAndToggleFlag(&dipent.xflags, XF_WATCHPRESS, "WatchPress", CATF_SETON,
				"Observers can now set all press.\n", CATF_NORMAL)) {
				/*
				 * See if observer press is allowed
				 */
				if (!(dipent.flags & F_OBNONE)) {
					fprintf(rfp,"\n.WARNING: Game '%s' has a possible security hole by allowing ",
						dipent.name);
					fprintf(rfp,"observers to see all press as well as sending press.\n");
					fprintf(rfp,"Be sure you know what you are doing!\n\n");
				}
			}
			break;

		case SET_NOWATCHALL:
			CheckPress();
			if (CheckAndToggleFlag(&dipent.xflags, XF_WATCHPRESS, "WatchPress", CATF_SETOFF,
				"Only master can now set all press.\n", CATF_NORMAL)) {
				/*
				 * See if observer press is allowed
				 */
				fprintf(bfp,"Any observers with 'set all press' no longer have this set.\n");
				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power == OBSERVER) {
						dipent.players[i].status &= ~SF_PRESS;
					}
				}
			}
			break;

		case SET_ABSENCE:
			if (dipent.phase[6] == 'X' ) {
				fprintf(rfp,"Game is already terminated - no point!");
				s = "";
				break;
			}
			if (dipent.players[player].power == OBSERVER) {
				fprintf(rfp, "Observers cannot request a game delay.\n\n");
				s = "";
				break;
			}
			if (dipent.players[player].absence_count >=MAX_ABSENCES ) {
				fprintf(rfp, "Maximum number of absences already reached.\n");
				fprintf(rfp,"You must remove some absences first.\n\n");
				s = "";
				break;
			}
			if (dipent.players[player].power != MASTER &&
			  dipent.players[player].status & SF_DEAD) {
				fprintf(rfp,"Sorry, only active players are allowed to request an absence.\n\n");
				s = "";
				break;
			}
		/* Look to see if date has a <dates> TO <datee> construct */
			break_date_into_two(s, ss, se );
			if (mail_date(&ss, &dates, 0, rfp)) {
				fprintf(rfp, "%sInvalid absence start date specified.\n\n", t);
				s = "";
				break;
			}
			if (*ss) {
				fprintf(rfp, "Extra date characters '%s': command rejected.\n\n", ss);
				s = "";
				break;
			}
			if (!*se || mail_date(&se, &datee, 0, rfp)) {
				fprintf(rfp, "%sNo valid absence end date specified - assuming one day only.\n\n", t);
				datee = dates + (24l * 60l *60l );
			}

			if (datee <= dates ) {
				fprintf(rfp, "Absence end date \n  %s\nis not after absence start date \n", ptime(&datee));
				fprintf(rfp,"  %s\nRequest ignored.\n\n", ptime(&dates));
				s="";
				break;
			}
			if (datee < time(NULL)) {
				fprintf(rfp,"Absence end date has already passed - request ignored.\n\n");
				s = "";
				break;
			}
			if (!i_am_really_master && dipent.players[player].power != MASTER &&
			absence_delay(dipent.max_absence_delay, datee-dates) == TOO_BIG) {
				sprintf(subjectline, "%s:%s - %s Oversized Absence Request", JUDGE_CODE, 
					dipent.name, dipent.phase);

				fprintf(rfp, "Requested absence delay is too large. \nRequest forwarded to master to approve.\n\n");
				fprintf(mbfp, "%s as %s requested absence between \n%s",
					raddr, PRINT_POWER, ptime(&dates));

				fprintf(mbfp," to \n%s.\n\n", ptime(&datee));
				fprintf(mbfp,"A master must approve this as it exceeds game limit of %d days.\n\n",
					dipent.max_absence_delay);
				fprintf(mbfp,"To approve, send the following commands:\n");
				fprintf(mbfp,"BECOME %c\n", pletter[dipent.variant][dipent.players[player].power]);
				fprintf(mbfp,"SET ABSENCE %s", abs_time(&dates));
				fprintf(mbfp," TO %s\n\n", abs_time(&datee));
				broadcast_master_only = 1;
				s="";
				break;
			}
			for (i=0; i < MAX_ABSENCES; i++) {
			    if (((dipent.players[player].absence_start[i] <=  dates &&
				  dipent.players[player].absence_end[i] >=  dates) ||
				 (dipent.players[player].absence_start[i] <=  datee &&
                                  dipent.players[player].absence_end[i] >=  datee)) &&
				dipent.players[player].absence_start[i] != 0 ) {
				fprintf(rfp, "Requested absence from %s",  ptime(&dates));
				fprintf(rfp," to %s is inside\n", ptime(&datee));
				fprintf(rfp, " existing absence from %s",
					ptime(&dipent.players[player].absence_start[i]));
				fprintf(rfp, " to %s.\n\n", 
					 ptime(&dipent.players[player].absence_end[i]));

				fprintf(rfp, "Please cancel existing absence first, using 'SET NOABSENCE %s'.\n\n",
					abs_time(&dipent.players[player].absence_start[i]));
				s="";
				break;
			    }
			}

			for (i=0; i < MAX_ABSENCES && dipent.players[player].absence_start[i] != 0; i++) ;
			if (dipent.players[player].absence_start[i] == 0 && *s) {
				dipent.players[player].absence_start[i] = dates;
				dipent.players[player].absence_end[i] = datee;
				dipent.players[player].absence_count++;
				fprintf(rfp, "Absence recorded from %s to\n", ptime(&dates));
				fprintf(rfp,"%s.\n\n", ptime(&datee));

				if ((strstr(subjectline, "New Player Signon") == NULL) && (strstr(subjectline, "Ready to Start") == NULL))
					sprintf(subjectline, "%s:%s - %s Absence Notice", JUDGE_CODE, dipent.name, dipent.phase);

				fprintf(mbfp, "%s as %s set absence from %s to\n",
				raddr, PRINT_POWER, ptime(&dates));
				fprintf(mbfp,"%s; Master's approval NOT required.\n\n", ptime(&datee));
				fprintf(mbfp,"To revoke this, if you feel it necessary, send the following commands:\n");
				fprintf(mbfp,"    BECOME %c\n", pletter[dipent.variant][dipent.players[player].power]);
				fprintf(mbfp,"    SET NOABSENCE %s\n", abs_time(&dates));
				broadcast_master_only = 1;
			} else {
			    if (*s) {
				fprintf(rfp, "Too many absences pending.\n\n");
				s = "";
				break;
			    }
			}

			s="";
			break;

		case SET_NOABSENCE:
			if (dipent.phase[6] == 'X' ) {
				fprintf(rfp,"Game is already terminated - no point!");
				s = "";
				break;
			}
			if (dipent.players[player].power == OBSERVER) {
				fprintf(rfp, "Observers cannot request a game delay.\n\n");
				s = "";
				break;
			}
			if (dipent.players[player].absence_count <= 0 ) {
				fprintf(rfp, "No absences currently pending - command ignored.\n\n");
				s = "";
				break;
                        }
			if (dipent.players[player].power != MASTER &&
			  dipent.players[player].status & SF_DEAD) {
				fprintf(rfp,"Sorry, only active players can have absences to cancel.\n\n");
				s = "";
				break;
			}

			if (mail_date(&s, &dates, 0, rfp)) {
				fprintf(rfp, "%sInvalid absence cancel date specified.\n\n", t);
				s = "";
				break;
			}
			k = 0;
			for (i=0; i < MAX_ABSENCES; i++) {
				if (dipent.players[player].absence_start[i] <= dates &&
				dipent.players[player].absence_end[i] >=  dates) {
					k++;
					fprintf(rfp, "Absence cancelled from \n%s",
						ptime(&dipent.players[player].absence_start[i]));
					fprintf(rfp," to \n%s.\n\n",
						ptime(&dipent.players[player].absence_end[i]));

					if ((strstr(subjectline, "New Player Signon") == NULL) && (strstr(subjectline, "Ready to Start") == NULL))
						sprintf(subjectline, "%s:%s - %s Absence Cancellation", JUDGE_CODE, dipent.name, dipent.phase);

					fprintf(mbfp, "%s as %s cancelled absence from \n%s",
						raddr, PRINT_POWER,
						ptime(&dipent.players[player].absence_start[i]));
					fprintf(mbfp, " to \n%s.\n\n",
						ptime(&dipent.players[player].absence_end[i]));
					dipent.players[player].absence_start[i] = 0;
					dipent.players[player].absence_end[i] = 0;
					dipent.players[player].absence_count--;
					broadcast_master_only = 1;
					s = "";
					break;
				}
			}
			if (!k) {
				fprintf(rfp,"No absence found for specified date.\n");
			}
			s="";
			break;

		case SET_MAXABSENCE:
#define LOW_MAXABSENCE (0)
#define HIGH_MAXABSENCE (31)

			 i = atoi(s);
                        while (isdigit(*s) || *s == '-' || *s == '+')
                                s++;
			if (i < LOW_MAXABSENCE || i > HIGH_MAXABSENCE )
			{
			    fprintf(rfp, "Invalid maximum absence %d. Must be between %d and %d",
				    i, LOW_MAXABSENCE, HIGH_MAXABSENCE);
			} else if (i == dipent.max_absence_delay) {
			    fprintf(rfp, "No point: max absence is already set to %d",i);
			} else {
				dipent.max_absence_delay = i;
                                fprintf(rfp, "Max absence for game '%s' set to %d.\n", 
					dipent.name,
                                        dipent.max_absence_delay);
                                pprintf(cfp, "%s%s as %s in '%s' set the\nmax absence to %d.\n",
                                        NowString(),
                                        xaddr, powers[dipent.players[player].power],
                                        dipent.name, dipent.max_absence_delay);
                                /* WAS mfprintf  1/95 BLR */
                                fprintf(bfp, "%s as %s in '%s' set the\nmax absence to %d.\n",
                                        xaddr, PRINT_POWER, dipent.name, dipent.max_absence_delay);
                                fprintf(mbfp, "%s as %s in '%s' set the\nmax absence to %d.\n",
                                        raddr, PRINT_POWER, dipent.name, dipent.max_absence_delay);
                                broadcast = 1;
			}
			break;

		case SET_NORMBROAD:
                        CheckPress();
			CheckAndToggleFlag(&dipent.players[player].status,  
					SF_NOBROAD, "NoBroadcasts", CATF_SETOFF,
					"Player will now receive all broadcast press messages.\n",
                                                   CATF_NORMAL);
                        break;

		case SET_NONORMBROAD:
			CheckPress();
                        if (dipent.players[player].power != MASTER && 
			    dipent.players[player].power != OBSERVER ) {
				fprintf(rfp, "This option is not allowed for players.\n\n");
				break;
			}
			CheckAndToggleFlag(&dipent.players[player].status,   
                                        SF_NOBROAD, "NoBroadcasts", CATF_SETON,
                                        "Player will not now receive ANY broadcast press messages.\n",
                                                   CATF_NORMAL);
                        break;

		case SET_MONEY:
			CheckMach();
                        CheckAndToggleFlag(&dipent.xflags, XF_NOMONEY, "Money", CATF_SETOFF,
                                                "Game now has money, loans, assassinations and special units.\n",
						 CATF_INVERSE);
			dipent.flags &= ~(F_NOLOANS | F_NOASSASS | F_NOSPECIAL);
			break;

		case SET_NOMONEY:
                        CheckMach();
                        CheckAndToggleFlag(&dipent.xflags, XF_NOMONEY, "Money", CATF_SETON,
                            "Game now does not have money, loans, assassinations and special units.\n",
			     CATF_INVERSE);
			dipent.flags |= (F_NOLOANS | F_NOASSASS | F_NOSPECIAL);
			break;

		case SET_MOVEDISBAND:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to set Disband!\n\n",
                                    dipent.name);
                        } else {
			    CheckAndToggleFlag(&dipent.xflags, XF_MOVEDISBAND, "Disband", CATF_SETON,
                                                "Players may now disband in a movement phase.\n", CATF_NORMAL);
                        }
			break;

		case SET_NOMOVEDISBAND:
                        if (dipent.seq[0] != 'x') {
                            fprintf(rfp, "Game '%s' has already started: not allowed to clear Disband!\n\n",
                                    dipent.name);
                        } else {
                            CheckAndToggleFlag(&dipent.xflags, XF_MOVEDISBAND, "Disband", CATF_SETOFF,
                                                "Players may not disband in a movement phase.\n", CATF_NORMAL);
                        }
			break;

		case SET_BASIC:
			CheckMach();
			CheckAndToggleFlag(&dipent.xflags, XF_NOMONEY, "Basic", CATF_SETON,
                                                "Game is now in basic settings.\n", CATF_INVERSE);
			dipent.flags |= (F_NOLOANS | F_NOASSASS | F_NOSPECIAL);
			CheckAndToggleFlag(&dipent.flags, F_NODICE, "NoDice",
					   CATF_SETON,
					   "Dice disabled.\n", CATF_INVERSE);
                        break;


		case SET_ADVANCED:
			CheckMach();
			CheckAndToggleFlag(&dipent.xflags, XF_NOMONEY, "Advanced", CATF_SETOFF,
                                                "Game is now in advanced settings.\n", CATF_INVERSE);
			dipent.flags &= ~(F_NOLOANS | F_NOASSASS | F_NOSPECIAL);
			CheckAndToggleFlag(&dipent.flags, F_NODICE, "NoDice",
					   CATF_SETOFF,
					   "Dice enabled.\n", CATF_INVERSE);
			break;


		case SET_SUMMER:
			if (CheckNotStarted(NULL))
			{
			    if (dipent.flags & F_MACH)
                                CheckAndToggleFlag(&dipent.x2flags, X2F_NOSUMMER, "NoSummer", CATF_SETOFF,
                                                "Game now has summer turns.\n", CATF_NORMAL);
			    else
				CheckAndToggleFlag(&dipent.x2flags, X2F_SUMMER, "Summer", CATF_SETON,
                                                "Game now has summer turns.\n", CATF_NORMAL);
                        }
			break;

		case SET_NOSUMMER:
			if (CheckNotStarted(NULL))
			{
                            if (dipent.flags & F_MACH)
                                CheckAndToggleFlag(&dipent.x2flags, X2F_NOSUMMER, "NoSummer", CATF_SETON,
                                                "Game now has NO summer turns.\n", CATF_NORMAL);
			    else
				 CheckAndToggleFlag(&dipent.x2flags, X2F_SUMMER, "Summer", CATF_SETOFF,
                                                "Game now has NO summer turns.\n", CATF_NORMAL);
			}
                        break;

		case SET_GARRISONS:
			if (CheckNotStarted(NULL))
                        {
                            CheckMach();
                            CheckAndToggleFlag(&dipent.x2flags, X2F_NOGARRISONS, "NoGarrison", CATF_SETOFF,                                                "Game now has garrisons.\n", CATF_NORMAL);
                        }
                        break;

		case SET_NOGARRISONS:
			if (CheckNotStarted(NULL))
                        {
                            CheckMach();
                            CheckAndToggleFlag(&dipent.x2flags, X2F_NOGARRISONS, "NoGarrison", CATF_SETON,                                                "Game now disallows garrisons.\n", CATF_NORMAL);
                        }
                        break;

		case SET_PREFLIST:
			if ((dipent.seq[0] != 'x') && !starting) {
				fprintf(rfp, "The preference settings cannot be changed after the game has started.\n");
			} else {
				SETX2FLAGS(0, X2F_PREFRANDALLOW);
				SETX2FLAGS(0, X2F_PREFRANDONLY);
				broad_params = 1;
			}
			break;

		case SET_PREFBOTH:
			if ((dipent.seq[0] != 'x') && !starting) {
				fprintf(rfp, "The preference settings cannot be changed after the game has started.\n");
			} else {
				SETX2FLAGS(X2F_PREFRANDALLOW, X2F_PREFRANDALLOW);
				SETX2FLAGS(0, X2F_PREFRANDONLY);
				broad_params = 1;
			}
			break;

		case SET_PREFRAND:
			if ((dipent.seq[0] != 'x') && !starting) {
				fprintf(rfp, "The preference settings cannot be changed after the game has started.\n");
			} else {
				SETX2FLAGS(X2F_PREFRANDALLOW, X2F_PREFRANDALLOW);
				SETX2FLAGS(X2F_PREFRANDONLY, X2F_PREFRANDONLY);
				broad_params = 1;
			}
			break;

		case SET_SECRET:
			SETX2FLAGS(X2F_SECRET, X2F_SECRET);
			broad_params = 1;
			break;

		case SET_NOSECRET:
			SETX2FLAGS(0, X2F_SECRET);
			broad_params = 1;
			break;

		case SET_MUSTORDER:
			SETX2FLAGS(X2F_MUSTORDER, X2F_MUSTORDER);
			fprintf(rfp,"Must_order flag set.\n");
			broad_params = 1;
			break;

		case SET_NOMUSTORDER:
			fprintf(rfp,"Players now need not have orders submitted to send press.\n");
			SETX2FLAGS(0, X2F_MUSTORDER);
			broad_params = 1;
			break;

		case SET_POSTALPRESS:
			fprintf(rfp,"Szine style press enabled.\n");
			SETX2FLAGS(X2F_POSTALPRESS, X2F_POSTALPRESS);
			broad_params = 1;
			broadcast = 1;
			break;

		case SET_NOPOSTALPRESS:
			fprintf(rfp,"Szine style press disabled.\n");
			SETX2FLAGS(0, X2F_POSTALPRESS);
			broad_params = 1;
			broadcast = 1;
			break;

		default:
			fprintf(rfp, "Invalid command: set %s\n", t);
			return;
		}
		while (*s && isspace(*s))
			s++;
	}

	if (chk24nmr && !(dipent.flags & F_NONMR) &&
	    (dipent.movement.grace < 36 ||
	     dipent.retreat.grace < 36 ||
	     dipent.builds.grace < 36)) {
		fprintf(rfp, "\nWARNING: Game '%s' is set NMR with a short grace period.\n",
			dipent.name);
		fprintf(rfp, "Powers may become abandoned with no 'deadline missed' notice");
		fprintf(rfp, "\nbeing sent.  Grace periods should be at least 48hrs.\n\n");
	}
}

void process_allowdeny(char **info, char *basename)
{
	char player_name[100];
	char *t, line[100];
	char addflag;
	char *s;

	s = *info;

	while (isspace(*s))
		s++;
	t = player_name;
	addflag = 1;
	if (*s == '-') {
		addflag = 0;
		s++;		/* TODO *s++ was here, perhaps (*s)++ was meant? unlikely tho */
		while (isspace(*s))
			s++;
	}
	while (*s && (*s != '\n') && (*s != ' '))
		*t++ = *s++;
	*t = '\0';

	if (!strcmp(dipent.name, "control"))
		strcpy(line, basename);
	else
		sprintf(line, "%s%s/%s", GAME_DIR, dipent.name, basename);
	add_player(player_name, line, addflag);

	*info = s;
}
/*
 * Alter the transformation settings as requested
 * Return 0 if failure, !0 if ok
 */

void ChangeTransform( char *s)
{

        int i;
        char *t;
/* First set up the various types of changes possible */
#define TSET_NOOP       0
#define TSET_BUILD      1
#define TSET_MOVE       2
#define TSET_ANYWHERE   3
#define TSET_HOME       4
#define TSET_ONEC       5
#define TSET_ANYC       6
#define TSET_NONE       7
#define TSET_SUBKEY     8

        static char *keys[] =
        {"", ",", ":",
         "builds", "build",
         "moves", "move",
         "none", "no", "never"
        };

        static char action[] =
        { 'x', TSET_NOOP, TSET_SUBKEY,
         TSET_BUILD, TSET_BUILD,
         TSET_MOVE, TSET_MOVE
        };

        int last_type = TSET_BUILD;

        while (*s)
        {
                s = lookfor(t = s, keys, nentry(keys), &i);
                switch (action[i])
                {

                case TSET_NOOP:  /* No command, wait for another one */
                    break;

                case TSET_BUILD:
                    dipent.xflags |= XF_TRANS_BUILD;
                    last_type = TSET_BUILD;
                    break;

                case TSET_MOVE:
                    dipent.xflags |= XF_TRANS_MOVE;
                    last_type = TSET_MOVE;
                    break;

                case TSET_NONE:
                    dipent.xflags &= ~(XF_TRANS_MOVE | XF_TRANS_BUILD);
                    break;

                case TSET_SUBKEY:
                     s = SetSubkey(last_type,s);
                     break;

                default:
                    fprintf(rfp, "Invalid transform command: %s\n", t);
                        return;
                }
            while (*s && isspace(*s))
                        s++;
        }
}

char * SetSubkey(int act, char *s)
{

        static char *subkeys[] =
        { "",
         "home centres", "home centers", "homecenters", "homecentres",
         "home centre", "home center", "homecenter", "homecentre",
         "home", "any centres", "any centers", "anycenters", "anycentres",
         "any centre", "any center", "anycenter", "anycentre",
         "one centres", "one centers", "onecenters", "onecentres",
         "one centre", "one center", "onecenter", "onecentre",
         "one", "any where", "anywhere",
         "none", "never", "no"
        };

        static char subaction[] =
        { 'x',
         TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME, TSET_HOME,
         TSET_ANYC, TSET_ANYC, TSET_ANYC, TSET_ANYC, TSET_ANYC, TSET_ANYC, TSET_ANYC, TSET_ANYC,
         TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC, TSET_ONEC,
         TSET_ANYWHERE, TSET_ANYWHERE,
         TSET_NONE, TSET_NONE, TSET_NONE
        };
        char *t;
        int i;

    s = lookfor(t = s, subkeys, nentry(subkeys), &i);
    switch (subaction[i])

    {
                case TSET_HOME:
                    if (act == TSET_MOVE) {
                        dipent.xflags &= ~(XF_TRANS_MANYC | XF_TRANS_MONEC);
                    } else {
                        dipent.xflags &= ~(XF_TRANS_BANYC | XF_TRANS_BONEC);
                    }
                    break;

                case TSET_ONEC:
                    if (act == TSET_MOVE) {
                    dipent.xflags |= XF_TRANS_MONEC;
                    dipent.xflags &= ~XF_TRANS_MANYC;
                    } else {
                        dipent.xflags |= XF_TRANS_BONEC;
                        dipent.xflags &= ~XF_TRANS_BANYC;
                    }
                    break;

                case TSET_ANYC:
                    if (act == TSET_MOVE) {
                    dipent.xflags |= XF_TRANS_MANYC;
                    dipent.xflags &= ~XF_TRANS_MONEC;
                    } else {
                        dipent.xflags |= XF_TRANS_BANYC;
                        dipent.xflags &= ~XF_TRANS_BONEC;
                    }break;

                case TSET_ANYWHERE:
                    if (act == TSET_MOVE) {
                     dipent.xflags |= (XF_TRANS_MANYC | XF_TRANS_MONEC);
                    } else {
                        dipent.xflags |= (XF_TRANS_BANYC | XF_TRANS_BONEC);
                    }
                     break;

              default:
                    fprintf(rfp, "Invalid transform qualifier: %s\n", t);
        }
        return s;
}
void ShowTransformSettings(FILE* rfp)
{
        fprintf(rfp,"Current transform setting:");
        if (dipent.xflags & (XF_TRANS_MOVE | XF_TRANS_BUILD))
            {
               if (dipent.xflags & XF_TRANS_BUILD) {
                    fprintf(rfp," Build:");
                    switch( dipent.xflags & (XF_TRANS_BANYW))
                    {
                        case XF_TRANS_BANYW:
                            fprintf(rfp,"Anywhere");
                            break;

                        case XF_TRANS_BONEC:
                            fprintf(rfp,"OneCentre");
                            break;

                        case XF_TRANS_BANYC:
                            fprintf(rfp,"AnyCentre");
                            break;

                        default:
                            fprintf(rfp,"HomeCentre");
                            break;
                    }

                }

                if (dipent.xflags &  XF_TRANS_MOVE) {
                    fprintf(rfp, " Move:");

                    switch( dipent.xflags & (XF_TRANS_MANYW))
                    {
                        case XF_TRANS_MANYW:
                            fprintf(rfp,"Anywhere");
                            break;

                        case XF_TRANS_MONEC:
                            fprintf(rfp,"OneCentre");
                            break;

                        case XF_TRANS_MANYC:
                            fprintf(rfp,"AnyCentre");
                            break;

                        default:
                            fprintf(rfp,"HomeCentre");
                            break;
                    }
                    if (dipent.xflags & XF_NOATTACK_TRANS )
                    {
                        fprintf(rfp, " (if not attacked)");
                    }
                    else
                        fprintf(rfp, " (if not dislodged)");

                }

            }
            else
                fprintf(rfp,"Never");

        fprintf(rfp,".\n");

}

/***************************************************************************/
