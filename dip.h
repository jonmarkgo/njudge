/*
 * $Log$
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
 */

#ifndef _DIP_H
#define _DIP_H

#include <stdio.h>
#include <ctype.h>
#include "conf.h"

#define min(a,b)      ((a) < (b) ? (a) : (b))
#define max(a,b)      ((a) > (b) ? (a) : (b))

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
#define F_VFLAGS    (F_GUNBOAT | F_BLIND | F_SHORTHAND | F_AFRULES | F_WINGS ) 	
#define F_OBWHITE   0x800	/* Observers can only post white press      */
#define F_OBNONE    0x1000	/* Observers cannot post press at all       */
#define F_MACH      0x2000	/* Game is a Machiavelli variant            */
#define F_PROXY     0x4000	/* Game allows proxy orders                 */
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
#define F_NOSHOW    0x4000000	/* aPower shown in gunboat set broadcasts   */
#define F_NODIAS    0x8000000	/* Draws need not include all survivors     */
#define F_GRACEDAYS 0x10000000	/* Grace period expire on processing days   */
#define F_STRWAIT   0x20000000	/* Only set wait if moving (or MASTER)      */
#define F_SHORTHAND 0x40000000  /* Set if variant of type Shorthand Press   */
#define F_WINGS     0x80000000  /* Wings variant (for modern)		    */

#define HasPress(x)  ( (x.flags & F_GREY) | !(x.flags & F_NOWHITE) | !(x.flags & F_NOPARTIAL) | (x.flags & F_OBWHITE) | !(x.flags & F_OBNONE))

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
#define XF_MACH1        0x4000  /* Machiavelli Version 1 (original)         */
#define XF_MACH3	(XF_MACH2 | XF_MACH1) /* Some to be defined variant */
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
#define XF_TRANS_BONEC XF_TRANS4 /* Build transform any if one hoem owned */
#define XF_TRANS_BANYW (XF_TRANS3 | XF_TRANS4) /* Build transf. anywhere */
#define XF_NORESUME	0x400000 /* Only master can resume game */
#define XF_AUTODISBAND	0x800000 /* There is no retreats */
#define XF_ANYDISBAND	0x1000000 /* Can disband any unit in a build phase */

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
#define SF_ABAND   0x10		/* Power has been abandoned, freee for takeover */
#define SF_PROCESS 0x20		/* Master has requested immediate processing    */
#define SF_PRESS   0x40		/* Master requests all partial press            */
#define SF_PART    0x80		/* We've gotten valid orders for all units      */
#define SF_DRAW   0x100		/* This player will accede to a draw            */
#define SF_NOBROAD 0x200	/* This player (master) does not want broadcasts*/
#define SF_DEAD    0x400	/* This player is officially dead! 		*/
#define SF_RESIGN  0x800	/* This player has resigned 			*/
#define SF_LATE	   0x1000	/* This player has already been told is late    */
#define SF_REMIND  0x2000	/* This player has been reminded to make a move */
#define SF_TURNGO  0x4000	/* Set by the master to make a manual turn go   */

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
	long absence_start[MAX_ABSENCES]; /* Start of each absence period   */
 	long absence_end[MAX_ABSENCES];  /* End of each absence period     */
	long absence_total;	/* Total absence time requested in game       */
	char password[30];	/* Player's password                          */
	char address[100];	/* Player's electronic mail address           */
	char pref[MAXPLAYERS];	/* Player's power preference list             */
} Player;

struct dipent {
	int valid;		/* =0 if dipent itself is empty 		*/
	int pr_valid;		/* =0 if pr array is non_valid			*/
	char name[9];		/* Game name                                    */
	char seq[11];		/* Game sequence number                         */
	char phase[10];		/* Game phase of the form F1901M                */
	char comment[70];	/* Comment associated with game                         */
	char epnum[20];		/* Electronic Protocol number                   */
	char bn_mnnum[20];	/* Boardman/Miller number                       */
	int access;		/* Access:  A_ANY, A_DIFF or A_SAME (site)      */
	int variant;		/* Variant: V_STANDARD                          */
	int level;		/* Level:   L_ANY, L_NOVICE, L_EXPERIENCED      */
	int flags;		/* Flags:   F_NONMR                             */
	int xflags;		/* Extra flags: XF_*				*/
	int dedicate;		/* Minimum dedication requirement               */
	long process;		/* Time to process this entry                   */
	long start;		/* Minimum time before processing this entry    */
	long deadline;		/* Current deadline to process this entry       */
	long grace;		/* Absolute deadline including grace period     */
	sequence movement;	/* Parameters for movement order deadlines      */
	sequence retreat;	/* Parameters for retreat order deadlines       */
	sequence builds;	/* Parameters for build order deadlines         */
	int n;			/* Number of players/observers in the game      */
	int np;			/* Number of powers for this variant            */
	int vp;			/* Number of victory points required            */
	char *pl;		/* Power significant letter                     */
	Player players[MAXPLAYERS];	/* The players involved in the game     */
	int no_of_players;	/* The number of players in the game    */
	int max_absence_delay;  /* the maximum number of turns for absences    */
	/* DAN 903 Line added */
};

#define MAXUSER 8000

struct dedication {
	short r;		/* Actual rating */
	short i1, i2;		/* Accumulators for expanded formula */
	long ls;		/* Last signon time */
	long md;		/* Last motd time */
	long d0;		/* Date for expanded formula */
};

#include "port.h"
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

#endif				/* _DIP_H */
