/*
 * $Log$
 * Revision 1.55  2004/10/23 21:08:59  millis
 * Fix Bug 375, Mach2 allow besieged to dislodge
 *
 * Revision 1.54  2004/09/13 01:33:35  nzmb
 * Trivial typo fix
 *
 * Revision 1.53  2004/09/07 13:19:57  millis
 * Invert sense of ALLIEDWIN flag
 *
 * Revision 1.52  2004/09/06 22:08:17  millis
 * Bug363 Allow more than one victor
 *
 * Revision 1.51  2004/09/03 13:30:17  millis
 * Added use of AlliedWin value.
 *
 * Revision 1.50  2004/07/25 16:13:43  millis
 * Bug fixes for Bug 91 (Duplex powers), Bug 233 (Abandoned power cannot
 * return in duplex) and Bug 206 (allow takeover of unknown abandoned
 * countries)
 *
 * Revision 1.49  2004/07/04 03:33:38  millis
 * Added Portage and irregular flags
 *
 * Revision 1.48  2004/05/22 09:39:43  millis
 * Repleaced inadvertently lost changes from 1.46 and 1.45
 *
 * Revision 1.47  2004/05/22 08:54:04  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.46  2004/03/28 16:06:08  millis
 * Added approval flags (for bug 193)
 *
 * Revision 1.45  2004/02/22 23:26:09  millis
 * Small Bug91 change
 *
 * Revision 1.44  2004/02/14 23:15:32  millis
 * Fixed X2F_PRESS_OPTIONS to use '|' not '&', else shows blank x2flags setting
 *
 * Revision 1.43  2004/01/05 13:54:51  millis
 * Added extra_centres definition
 *
 * Revision 1.42  2004/01/05 13:49:33  millis
 * Added missing definition for extra_centre
 *
 * Revision 1.41  2004/01/04 11:34:34  millis
 * Implement Bug #262 (ExtraCentres for 1900 Steamroller)
 *
 * Revision 1.40  2004/01/02 10:00:15  millis
 * no message
 *
 * Revision 1.39  2003/12/29 17:59:53  millis
 * Work for Bug 91 (keep duplex powers together)
 *
 * Revision 1.38  2003/12/28 00:00:40  millis
 * Fix bug 262 (add Extra Units flag for 1900 SteamRoller)
 *
 * Revision 1.37  2003/09/09 18:51:28  jaldhar
 * Got rid of port.h and replaced with some extra configure checks.  The
 * include strings.h was not carried over because it is commented out and
 * likely wouldn't work anyway.
 *
 * Revision 1.36  2003/08/25 14:39:36  millis
 * Fixed bug 220
 *
 * Revision 1.35  2003/08/18 23:16:43  millis
 * Refix bug 211
 *
 * Revision 1.34  2003/08/14 12:46:26  millis
 * Fix bug 211
 *
 * Revision 1.33  2003/08/10 15:27:50  millis
 * Fix bug 25 (Add TouchPress)
 *
 * Revision 1.32  2003/07/23 00:11:43  millis
 * Bug 192
 *
 * Revision 1.31  2003/07/20 08:43:52  millis
 * Missing include
 *
 * Revision 1.30  2003/07/20 08:37:04  millis
 * make ls & md members of type time_t
 *
 * Revision 1.29  2003/05/14 07:59:16  nzmb
 * Fixed bug #122 -- splits dip.reply into part created before and after
 * the GM issues a "become" command.
 *
 * Revision 1.28  2003/05/13 00:07:26  millis
 * Bug 110, move on process deadline by 24 hours on bailout recovery
 *
 * Revision 1.27  2003/05/12 23:23:46  millis
 * Fix bug 133, allow turn to process when set to manualprocess and process command sent.
 *
 * Revision 1.26  2003/05/09 11:09:57  millis
 * Added display of Mach2 flag
 *
 * Revision 1.25  2003/05/06 01:51:49  millis
 * Maks out Mach flags in X2F display
 *
 * Revision 1.24  2003/05/02 17:28:43  millis
 * Added define for CAPTUREWIN, prior to actual implementation
 *
 * Revision 1.23  2003/04/27 17:23:35  millis
 * Bug 142, add flag for AutoCreate
 *
 * Revision 1.22  2003/04/16 04:29:59  millis
 * Fix Bug 65
 *
 * Revision 1.21  2003/02/18 14:00:46  millis
 * Added some new type defines
 *
 * Revision 1.20  2003/01/13 17:39:24  millis
 * Merge from ustv branch
 *
 * Revision 1.19  2002/12/28 00:52:18  millis
 * Proper fix to CR 17
 *
 * Revision 1.18  2002/12/11 16:13:16  millis
 * Added new flag to set up autonomous garrisons in every fort (for Mach)
 *
 * Revision 1.17  2002/08/27 23:56:10  millis
 * Added better victory display (fixing Mach victory bug)
 *
 * Revision 1.16  2002/07/16 18:14:21  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.15  2002/06/11 16:26:18  nzmb
 *
 * Added set [no]mustorder to require players to submit avalid set of orders
 * before they may send press (to be used in conjunction with set wait).
 *
 * Revision 1.14  2002/05/14 23:34:00  miller
 * Added some more flags and globals
 *
 * Revision 1.13  2002/05/11 09:15:31  greg
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
 * Revision 1.12  2002/04/18 04:44:31  greg
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
 * Revision 1.11  2002/04/15 12:55:42  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.10  2002/02/25 11:51:50  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.9  2001/10/20 12:11:11  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.8.2.2  2001/10/20 00:54:08  dedo
 * *** empty log message ***
 *
 * Revision 1.8.2.1  2001/10/15 00:48:38  ustv
 * Addedextra flags x2flags
 *
 * Revision 1.8  2001/08/18 06:25:40  nzmb
 * Added XF_NOCONCESSIONS & SF_CONC
 *
 * Revision 1.7  2001/07/08 22:53:57  miller
 * New defines
 *
 * Revision 1.6  2001/07/01 23:19:29  miller
 * New Mach flags
 *
 * Revision 1.5  2001/06/24 06:12:35  nzmb
 * Added dipent variables (dedapplied, orded, rrded) for use in new dedication
 * and deadline features.
 *
 * Revision 1.4  2001/05/07 04:55:17  greg
 * added subjectline
 *
 * Revision 1.3  2001/04/15 21:28:30  miller
 * Added XF_NOATTACKTRANS flag
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added XF_FLAG defines for Extended flags
 * Added absences to player and game structures
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.2  1997/02/16 20:43:18  davidn
 * Additions to dipent structure and associated code, to allow duplex variants.
 * Command is "set players n".
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  dip.h

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
 *  ----------- ------------ ----------------------------------------
 *  ?? ??? 1987 Ken Lowe     He wrote it
 *  29 Dec 1996 David Norman Addition to dipent (Marked DAN 903)
 *  26 Nov 1999 Millis Miller Added xflags, shorthand, late_count flags and value
 *  08 Dec 1999 Millis Miller Added dipent.build_anywhere flag to fix 'chaos-bug'
 *  06 May 2001 Gregory A Greenman Added Subjectline variable
 */

#ifndef _DIP_H
#define _DIP_H

#include <stdio.h>
#include <ctype.h>
#include "diptime.h"

#include "conf.h"

#define min(a,b)      ((a) < (b) ? (a) : (b))
#define max(a,b)      ((a) > (b) ? (a) : (b))

/* Redefine time() so it can be over-ridden for testing purposes */
#define time(a)	      diptime(a)

#define nentry(x)     (sizeof(x)/sizeof(x[0]))
#define E_FATAL  -2
#define E_WARN   -1

#define A_ANY           1	/* Anyone can access this game                */
#define A_DIFF          2	/* Players must be from different sites       */
#define A_SAME          3	/* Players must be from the same site         */
#define A_ALT	        4	/* Players from same site become alternates       */

#define L_ANY           1	/* Players can be of any level                */
#define L_NOVICE        2	/* Novice player level                        */
#define L_ADVANCED      3	/* Players must not be of Novice level        */
#define L_INTERMEDIATE  4	/* Intermediate player level                  */
#define L_AMATEUR       5	/* Players must not be of Expert level        */
#define L_EXPERT        6	/* Expert player level                        */
#define L_WINNER        7	/* Winner player level                        */

#define F_NONMR     0x1		/* Game will wait if no moves are received  */
#define F_NOLIST    0x2		/* Game is not listed with the rest         */
#define F_MODERATE  0x4		/* Game is moderated by an active master    */
#define F_NOWHITE   0x8		/* Game does not allow white press          */
#define F_GREY      0x10	/* Game allows grey press                   */
#define F_DEFWHITE  0x20	/* Default press is white                   */
#define F_NOPARTIAL 0x40	/* Press must be sent to all participants   */
#define F_FAKE      0x80	/* Partial press can appear as broadcast    */
#define F_DEFFAKE   0x100	/* Partial press as broadcast by default    */
#define F_GUNBOAT   0x200	/* Game is a gunboat variant                */
#define F_BLIND     0x400	/* Game is a blind variant                  */
/* Variant related flags                    */
#define F_VFLAGS    (F_GUNBOAT | F_BLIND | F_SHORTHAND | F_AFRULES | F_WINGS | F_INTIMATE) 	
#define F_OBWHITE   0x800	/* Observers can only post white press      */
#define F_OBNONE    0x1000	/* Observers cannot post press at all       */
#define F_MACH      0x2000	/* Game is a Machiavelli variant            */
#define F_INTIMATE  0x4000	/* Intimate Diplomacy                       */
#define F_NODICE    0x8000	/* Game does not contain random elements    */
#define F_NOFAMINE  0x10000	/* Game does not use famine rules           */
#define F_NOPLAGUE  0x20000	/* Game does not use plague rules           */
#define F_NOLOANS   0x40000	/* Game does not use banker rules           */
#define F_NOASSASS  0x80000	/* Game does not allow assassination        */
#define F_NOADJ     0x100000	/* Bribe orders do not require adjacency    */
#define F_NOSPECIAL 0x200000	/* No special units are allowed             */
#define F_QUIET     0x400000	/* Take-overs and such are done silently    */
#define F_NORATE    0x800000	/* Game does not affect dedication rating   */
#define F_NOREVEAL  0x1000000	/* Gunboat players are not to be revealed   */
#define F_AFRULES   0x2000000	/* A/F rules are in play                    */
#define F_NOSHOW    0x4000000	/* Setting changes show someone@somewhere   */
#define F_SPARE1    0x8000000	/* Unused (was F_NODIAS)                    */
#define F_GRACEDAYS 0x10000000	/* Grace period expire on processing days   */
#define F_SPARE2    0x20000000	/* Unused (was F_STRWAIT)                   */
#define F_SHORTHAND 0x40000000  /* Set if variant of type Shorthand Press   */
#define F_WINGS     0x80000000  /* Wings variant (for modern)		    */

#define HasPress(x)  ( (x.flags & F_GREY) | !(x.flags & F_NOWHITE) | !(x.flags & F_NOPARTIAL) | (x.flags & F_OBWHITE) | !(x.flags & F_OBNONE))
#define DIPENT_NO_PRESS ((dipent.flags & F_NOWHITE) && !(dipent.flags & F_GREY))

#define XF_MANUALPROC	0x1	/* Set if master needs to process turns     */
#define XF_MANUALSTART	0x2	/* Set if master needs to start game        */
#define XF_NOLATEPRESS  0x4	/* Set if country cannot press when late    */
#define XF_TRANS_MOVE   0x8
#define XF_STRCONVOY	0x10	/* Set when strict convoys only are allowed */
#define XF_AHCONVOY	0x20	/* Set when AH style convoy syntax is used  */
#define XF_LATECOUNT    0x40    /* Set if count is kept of player lateness  */
#define XF_BUILD_ANYCENTRES 0x80 /* Set if can build on any owned centre    */
#define XF_TRANS_BUILD   0x100
#define XF_TRANS_ANYT     (XF_TRANS_BUILD | XF_TRANS_MOVE)
#define XF_WATCHPRESS   0x200   /* Allow observer to use set all press      */
#define XF_BUILD_ONECENTRE (0x400 | XF_BUILD_ANYCENTRES) /* Set if need one home centre to build */
#define XF_BLANKPRESS	0x800   /* Allow blank press to be sent             */
#define XF_NOMINORPRESS 0x1000  /* Do not allow press in minor phases       */
#define XF_MACH2	0x2000  /* Machiavelli Version 2		    */
#define XF_STORMS       0x4000  /* Machiavelli storms (plauge for seas)     */
#define XF_TRANS1       0x8000  /* Transform flags 1                        */
#define XF_TRANS2      0x10000 /* Transform flag 2                          */
#define XF_TRANS_MANYC  XF_TRANS1 /* Transform on any centre	            */
#define XF_TRANS_MONEC   XF_TRANS2 /* Transform on any if one home owned      */
#define XF_TRANS_MANYW   (XF_TRANS1 | XF_TRANS2)  /*Transform any where at all*/
#define XF_AIRLIFTS   0x20000  /* Allow airlifts (wing-based convoys)	    */
#define XF_BLANKBOARD 0x40000  /* Game starts in build with no units        */
#define XF_FORT       0x80000  /* Mach, game allows fortresses 		    */
#define XF_TRANS3     0x100000 /* Transform flag 3 */
#define XF_TRANS4     0x200000 /* Transform flag 4 */
#define XF_TRANS_BANYC XF_TRANS3 /* Build transform on any centre */
#define XF_TRANS_BONEC XF_TRANS4 /* Build transform any if one home owned */
#define XF_TRANS_BANYW (XF_TRANS3 | XF_TRANS4) /* Build transf. anywhere */
#define XF_NORESUME	0x400000 /* Only master can resume game */
#define XF_AUTODISBAND	0x800000 /* There is no retreats */
#define XF_ANYDISBAND	0x1000000 /* Can disband any unit in a build phase */
#define XF_NOATTACK_TRANS  0x2000000 /* An attacked unit fails transformations  */
#define XF_COASTAL_CONVOYS  0x4000000 /* Convoys allowed by fleets on coasts */
#define XF_PROV_DUALITY 0x8000000 /* Provinces can be both land and water  */
#define XF_NOMONEY	0x10000000 /* Set to have no money (basic game Mach) */
#define XF_MOVEDISBAND  0x20000000 /* Allow disbands in movement turns 	     */
#define XF_NOCONCESSIONS 0x40000000 /* No concessions permitted */

/* Mach2 flags, currently combined into one flag, XF_MACH2 */
#define XF_GCONVERT_ANYTIME     XF_MACH2  /* Can retreat to a garrison anytime */
#define XF_NOLIFT_SIEGE         XF_MACH2  /* No need to use a 'lift siege' order*/
#define XF_LIMIT_TYPES          XF_MACH2  /* Certain countries have unit limits */
#define XF_NO_MIXED_CONTROL     XF_MACH2  /* Cannot own province if city is not empty */
#define XF_CITY_ONLY_VICTORY    XF_MACH2  /* Only need to win cities for victory */
#define XF_BESEIGED_CAN_DISLODGE XF_MACH2  /* A supported convert can dislodge if beseiged */

/* Extra 2 flags */
#define X2F_RAILWAYS		0x1	/* allows railways */
#define X2F_HONGKONG		0x2
#define X2F_GATEWAYS		0x4
#define X2F_COLONIAL (X2F_RAILWAYS | X2F_HONGKONG | X2F_GATEWAYS)
#define X2F_PREFRANDALLOW	0x8
#define X2F_PREFRANDONLY	0x10
#define X2F_SECRET		0x20
#define X2F_MORE_HOMES		0x40  /* Allow powers to declare more homes up to num_homes */
#define X2F_BLIND_CENTRES	0x80 /* in Blind, don't display centre summary */
#define X2F_BURN_BOATS		0x100 /* Native African, burn boats on landing */
#define X2F_SETUP_USED		0x200 /* Set when setup has been used once */
#define X2F_NEUTRALS		0x400 /* Flag to set up Neutrals in each province */
#define X2F_NO_TREASURY		0x800  /* In Mach, don't show treasury	*/
#define X2F_MUSTORDER		0x1000  /* don't allow press (except to m) unless orders are in */
#define X2F_POSTALPRESS		0x2000  /* allow szine style press (published w/ moves) */
#define X2F_FORT_GARRISON	0x4000 /* When set, all forts start with garrisons */
#define X2F_RIVERS              0x8000 /* Duality provinces work like rivers */
#define X2F_HOMETRANSFER        0x10000 /* Centre becomes your home if occupied 1 full year */
#define X2F_CAVALRY             0x20000 /* Units that can move two spaces per turn */
#define X2F_ARTILLERY           0x40000 /* Units that can support from two spaces away */
#define X2F_NOSUMMER		0x80000 /* In Mach, to disable the summer turn */
#define X2F_SUMMER		0x80000 /* In non-Mach, to enable a summer turn */
#define X2F_NOGARRISONS		0x100000 /* In Mach, to disable garrisons */

#define X2F_AUTOCREATE		0x200000 /* When game ends, start another one */
#define X2F_CAPTUREWIN          0x400000 /* Powers win the game by capturing another's HCs */
#define X2F_TOUCHPRESS		0x800000 /* Powers can only press to neighbours */
#define X2F_EXTRA_HC	        0x1000000 /* Game has extra home centre(s) according to map file */
#define X2F_APPROVAL 		0x2000000 /* Master needs to approve players to let them make moves */
#define X2F_PROXY		0x4000000 /* Game allows proxy orders */
#define X2F_NODIAS		0x8000000 /* Game has NODIAS settings */
#define X2F_STRWAIT		0x10000000 /* Only players with moves can place waits */
#define X2F_IRREGULAR		0x20000000 /* Game settings were changed in play */
#define X2F_PORTAGE		0x40000000 /* Game has armies able to convoy */

#define X3F_ALLIEDWIN	0x1 /* If set, duplex game can end with an allied win */

/* Define for X2F flags for params.c to display */
/* Only display special press settings */
#define X2F_PRESS_OPTIONS 	(X2F_TOUCHPRESS | X2F_MUSTORDER)
/* Do not display Colonial, Preference and Mach-only flags */
#define X2F_PRINT_OPTIONS 	(~X2F_PRESS_OPTIONS & ~X2F_COLONIAL & ~X2F_PREFRANDALLOW & ~X2F_PREFRANDONLY & ~X2F_NO_TREASURY & ~X2F_FORT_GARRISON & ~X2F_NOGARRISONS & ~X2F_STRWAIT & ~X2F_NODIAS & ~X2F_PROXY)

#define GAME_TERMINATED       (dipent.phase[6] == 'X')
#define GAME_PAUSED           (dipent.phase[6] == 'P')
#define GAME_SETUP            (dipent.phase[6] == 'S')
#define HAS_PORTAGE	      (dipent.x2flags & X2F_PORTAGE)

/* Following is when everyone alive can move in a build turn */
#define XF_ALTBUILD (XF_ANYDISBAND | XF_TRANS_BUILD)

typedef struct Sequence {
	int clock;		/* Time of day for orders to be due             */
	float mint;		/* Minimum time before orders will be processed */
	float next;		/* Maximum time before orders will be processed */
	float grace;		/* Grace period for the tardy people            */
	float delay;		/* Minimum delay after last orders received     */
	char days[8];		/* List of acceptable days, eg: xMTWTFx         */
} sequence;

#define SF_MOVE    0x1		/* Expecting move from this player              */
#define SF_MOVED   0x2		/* Received move from this player               */
#define SF_CD      0x4		/* This player has gone CD, won't wait for him  */
#define SF_WAIT    0x8		/* Player requests processing to hold til dline */
#define SF_ABAND   0x10		/* Power has been abandoned, free for takeover  */
#define SF_PROCESS 0x20		/* Master has requested immediate processing    */
#define SF_PRESS   0x40		/* Master requests all partial press            */
#define SF_PART    0x80		/* We've gotten valid orders for all units      */
#define SF_DRAW   0x100		/* This player will accede to a draw            */
#define SF_NOBROAD 0x200	/* This player (master) does not want broadcasts*/
#define SF_DEAD    0x400	/* This player is officially dead! 		*/
#define SF_RESIGN  0x800	/* This player has resigned 			*/
#define SF_LATE	   0x1000	/* This player has already been told is late    */
#define SF_REMIND  0x2000	/* This player has been reminded to make a move */
#define SF_NOT_APPROVED  0x4000 /* Player needs to be approved before can move  */
#define SF_CONC    0x8000       /* Player agreed to concession */
#define SF_SIGNED  0x10000	/* Player has signed onto judge this turn */
#define SF_BROAD_SENT 0x20000   /* Player has sent one broadcast message this turn */


/* This will clear out non-essential status for a new player */
#define NewPlayerSF(i) dipent.players[i].status &= ~(SF_RESIGN | SF_CD | SF_ABAND | SF_REMIND | SF_LATE | SF_DRAW);

#define WAITING(s)  (((s) & (SF_MOVE | SF_MOVED | SF_CD)) == SF_MOVE)
#define MAXPLAYERS 50		/* Maximum number of players/observers per game */
#define HRS2SECS   (60.0*60.0)

/* Change this define at your peril! */
/* Check the sscanf/fprintf in dipent if you do! */
#define MAX_ABSENCES (3)  

typedef struct PLAYER {
	int power;		/* Power ordinal                              */
	int status;		/* Status flags                               */
	int userid;		/* User's unique identifier                   */
	int siteid;		/* User's site or area identifier             */
	int units;		/* Number of units this player has            */
	int centers;		/* Number of centers this player has          */
	int centres_blockaded;  /* Number of centres this power has blockaded */
	int late_count;		/* number of times this player has gone late  */
	int absence_count;	/* Number of active absence requests          */
	int controlling_power;  /* Ordinal of power controlling (0 if real)   */
	long absence_start[MAX_ABSENCES]; /* Start of each absence period     */
 	long absence_end[MAX_ABSENCES];  /* End of each absence period        */
	long absence_total;	/* Total absence time requested in game       */
	char password[30];	/* Player's password                          */
	char address[100];	/* Player's electronic mail address           */
	char pref[MAXPLAYERS];	/* Player's power preference list             */
	char draw[MAXPLAYERS];  /* Player's draw preference list              */
} Player;

struct dipent {
	int valid;		/* =0 if dipent itself is empty 		*/
	int pr_valid;		/* =0 if pr array is non_valid			*/
	char name[9];		/* Game name                                    */
	char seq[11];		/* Game sequence number                         */
	char phase[10];		/* Game phase of the form F1901M                */
	char comment[70];	/* Comment associated with game                 */
	char epnum[20];		/* Electronic Protocol number                   */
	char bn_mnnum[20];	/* Boardman/Miller number                       */
	int access;		/* Access:  A_ANY, A_DIFF or A_SAME (site)      */
	int variant;		/* Variant: V_STANDARD                          */
	int level;		/* Level:   L_ANY, L_NOVICE, L_EXPERIENCED      */
	int flags;		/* Flags:   F_NONMR                             */
	int xflags;		/* Extra flags: XF_*				*/
	int x2flags;		/* More extra flags                             */
	int x3flags;		/* Yet more flags 				*/

	int dedicate;		/* Minimum dedication requirement               */
	float orded;	        /* Minimum ontime ratio requirement             */
	float rrded;            /* Maximum resignation ratio requirement        */
	int dedapplied;         /* Have we applied dedication / taken stats yet?*/		
	time_t process;		/* Time to process this entry                   */
	time_t start;		/* Minimum time before processing this entry    */
	time_t deadline;	/* Current deadline to process this entry       */
	time_t grace;		/* Absolute deadline including grace period     */
	time_t wait;		/* Time to wait before sending a pause reminder */

	sequence movement;	/* Parameters for movement order deadlines      */
	sequence retreat;	/* Parameters for retreat order deadlines       */
	sequence builds;	/* Parameters for build order deadlines         */
	int n;			/* Number of players/observers in the game      */
	int np;			/* Number of powers for this variant            */
	int powers;		/* Number of active powers for this variant     */
	int vp;			/* Number of victory points required            */
	int avp;		/* Number of victory points for an allied win   */
	char *pl;		/* Power significant letter                     */
	Player players[MAXPLAYERS];	/* The players involved in the game     */
	int no_of_players;	/* The number of players in the game            */
	int max_absence_delay;  /* the maximum number of turns for absences     */
        int num_homes;		/* Maximum number of configurable homes		*/
	int has_natives;	/* index of native power, 0 if no natives       */
	int has_multi_unit_provs; /* !0 if has multi-unit provinces, else 0     */
	int extra_centres;	/* !0 if X2F_INITIAL_CENTRES set */
};

/* Following calculates the allied win vp, as being an extra third of VP, rounded up */
#define DEFAULT_AVP(dipent) (dipent.vp + (dipent.vp / 3) + (dipent.vp % 3 == 0 ? 0 : 1))

#define IS_DUPLEX(dipent) (dipent.np != dipent.no_of_players)
#define MAXUSER 8000

struct dedication {
	short r;		/* Actual rating */
	short i1, i2;		/* Accumulators for expanded formula */
	time_t ls;		/* Last signon time */
	time_t md;		/* Last motd time */
	long d0;		/* Date for expanded formula */
};

struct extra_centre {
    char power_letter;		/* Letter of power that is involved */
    int count;			/* How many extra centres this power has */
};

extern struct extra_centre extra_centres[MAXPLAYERS];

#include "variant.h"

#define PRINT_POWER \
   ((((dipent.flags & (F_GUNBOAT | F_NOSHOW)) == (F_GUNBOAT | F_NOSHOW)) && \
		dipent.players[player].power < WILD_PLAYER) ? somepower :\
				   powers[dipent.players[player].power])

extern struct dipent dipent;
extern FILE *log_fp;		/* General log/interlock file */
extern FILE *rfp;		/* Reply file pointer */
extern FILE *cfp;		/* Control file pointer */
extern FILE *xfp;		/* Control file pointer */
extern FILE *inp;		/* Input FPTR (usually stdin unless -i used     */
extern int nded;
extern struct dedication ded[MAXUSER];

extern char subjectline[80];

extern char *notifies;
extern int aflg;		/* -a: don't mess with the at queue at all      */
extern int Aflg;		/* -A: don't take anything off the at queue     */
extern char *dflg;		/* -d home directory                            */
extern int Dflg;		/* -D: output goes to stdout rather than reply  */
extern char *nflg;		/* Our program name                             */
extern int qflg;		/* -q: quick mode, just process mail, not games */
extern int tflg;		/* -t variant: test adjudication                */
extern char *rflg;		/* -r name: randomizer for die rolls            */
extern int sflg;		/* -s sequence: test adjudication               */
extern int vflg;		/* -v: verbose, issue all error messages        */
extern int xflg;		/* -x: no incoming mail on stdin                */
extern char *inname;		/* Filename to use for input			*/
extern int control;
extern int predict;		/* =1 when predicting a turn			*/

extern int canbuild;	/* can a player build? (used when being reminded to move */

extern int victor;              /* Set to power count if there is a game winner */
				/* (could be > 1 if duplex game) */

extern int victors[MAXPLAYERS];	/* Powers that have won */

extern int bailout_recovery;    /* Set to 1 if recovering from a bail-out       */

extern int GM_Became;		/* set to 1 if GM issued become */

extern int has_treasury;	/* Set to 1 if the game datafile has treasury   */

#endif				/* _DIP_H */
