/*
   ** $Log: mail.h,v $
   ** Revision 1.8  2004-01-21 00:53:36  millis
   ** Comment added
   **
   ** Revision 1.7  2003/06/22 04:10:22  nzmb
   ** Added code to allow users to record diary entries, read them, and delete them
   ** if they make a mistake. The diaries will be broadcast when the games end (this
   ** part is not as of now, yet written).
   **
   ** Revision 1.6  2003/05/24 23:57:15  millis
   ** Bug 97, removed Generic Handling code
   **
   ** Revision 1.5  2002/07/16 18:14:22  nzmb
   ** Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
   **
   ** Revision 1.4  2002/05/11 09:15:33  greg
   ** Minor bug fixes
   ** - fixed subjectline for absence requests
   ** - fixed phase length, so it's no longer hard coded for responses
   ** - partial fix for unusable builds, players with only unusable builds
   **    will no longer be flagged as having orders due, however players
   **    with some usable builds will need to waive any unusable builds,
   **    also, if one or more players have unusable builds, but no
   **    player has usable builds, the build phase will process after
   **    a short delay
   **
   ** Revision 1.3  2001/07/01 23:19:29  miller
   ** Add predict flag
   **
   ** Revision 1.2  2000/11/14 14:27:37  miller
   ** Added a few extra glags and a couple of defines to force password of resigned payers to a bad value
   **
   ** Revision 1.1  1998/02/28 17:51:14  david
   ** Initial revision
   **
   ** Revision 1.2  1996/11/05 23:11:36  rpaar
   ** USIT changes to fix minor bugs
   **
 */

/*  mail.h -- header file for mail handling routines
   **
   **  Copyright 1987, Lowe.
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the 
   **  above notices are preserved and that due credit is given to Mr.
   **  Lowe.
   **
 */

#ifndef _MAIL_H
#define _MAIL_H

#include "variant.h"		/* To pick up MAX_POWERS definition */

extern int errno;
extern int Dflg;
extern int more_orders;
extern int control;

int signedon;			/* Nonzero if player signed on                  
				   +1 if game has started, -1 if not            */
int i_am_really_master;		/* Non-zero if really a master signed on	*/
int i_am_really_jk;		/* Non-zero if really a jk signed on		*/
int starting;			/* Nonzero if quorum has been reached           */
int player;			/* Player ordinal if signon processed           */
int listflg;			/* Non zero if a list command was processed     */
int broadcast;			/* Non zero to send a press message out         */
int broadcast_master_only;	/* Non-zero if a master-only message to send    */
int broad_part;			/* Non zero if some don't get it                */
int broadcast_absence_adjust;   /* Non-zero if a requested absence is activated */
char broad_list[MAX_POWERS + 1];
			/* List of power letters who get message...     */
int broad_allbut;		/* ... or don't, as the case may be             */
int broad_read;			/* Non zero if reading broadcast text now       */
int broad_skip;			/* Non zero if skipping broadcast message input */
int broad_signon;		/* Non zero if new signon needs broadcasting    */
int broad_params;		/* Non zero if changed params need broadcasting */
int read_phase;			/* > zero if future phase processing            */
int moreaddr;			/* Non zero when address continued on 2nd line  */
int create;			/* Non zero when processing create command      */
int predict;			/* Non zero when predicting move results        */
int msg_header_done;		/* Message header has/hasn't been written out   */
char baddr[80];			/* Optional additional broadcast recipient      */
char raddr[80];			/* Reply mail address from mail header          */
char *xaddr;			/* Address that gets announced to other people  */
char *someone;			/* Generic address for gunboat games            */
char *somepower;		/* Generic power name for gunboat games         */
char name[20];			/* Signon name: pname, p=power, name=game       */
int master_press;		/* Set to 1 if press is also to master, else 0  */
int master_only_press;		/* Set to 1 if press only for master's eyes     */
int ppress_read;		/* set to 1 if reading postal press */
int ppress_skip;		/* set to 1 if skipping postal press */
int ppress_done;		/* control variable */
int diary_read;			/* control for diaries */
int diary_skip;
int diary_done;

char Mfile[40];			/* Movement orders file name                    */
char Tfile[40];			/* Temporary movement orders file name          */
FILE *mail_bfp;			/* Broadcast file pointer                       */
FILE *mail_mbfp;		/* Broadcast to master file pointer             */
FILE *mail_mfp;			/* Master file pointer                          */
FILE *mail_nfp;			/* New master file pointer                      */
FILE *mail_ofp;			/* Output file pointer                          */
FILE *mail_qfp;			/* Terminate result for Custodians file pointer */
FILE *mail_ifp;			/* Incoming message (copy thereof) file pointer */
FILE *mail_pfp;			/* Future phase file pointer                    */
FILE *mail_tfp;			/* Temporary file pointer                       */
FILE *mail_ppfp;		/* postal press file pointer*/
FILE *diaryfp;			/* diary file pointer */
char mail_bfile[20];		/* Name of broadcast file                       */
char mail_mbfile[20];		/* Name of master broadcast file                */
char *mail_rfile;		/* Name of reply file                           */

/* Following two used to signal when a power is resigning and is resigned */
#define GOING_PWD "-going-"
#define GONE_PWD "-gone-"

/*
   **  Temporary set of defines to make it easy to split the monster mail.c
   **  file up into smaller pieces.  Of course these temporary definitions
   **  will undoubtably stick around for near to forever, but it's nice to
   **  know that there were intentions to clean it up when it was first done.
 */

#define bfp		mail_bfp
#define mbfp            mail_mbfp
#define mfp		mail_mfp
#define nfp		mail_nfp
#define ofp		mail_ofp
#define qfp		mail_qfp
#define ifp		mail_ifp
#define pfp		mail_pfp
#define tfp		mail_tfp
#define bfile		mail_bfile
#define mbfile          mail_mbfile
#define rfile		mail_rfile
#define ppfp		mail_ppfp

char *lookfor();

#define pcontrol if (!(dipent.flags & (F_NOLIST|F_QUIET))) control
#define pprintf  if (!(dipent.flags & (F_NOLIST|F_QUIET))) fprintf
#define PRIVOK      (dipent.players[player].power == MASTER || \
                     !(dipent.flags & F_MODERATE))
#define any_broadcast (broadcast || broad_signon || broad_params)

/*
   ** Defines for the ALLOW/DENY Functionality
 */
#define GLOBAL_PLAYER	1
#define GLOBAL_MASTER	2
#define GAME_PLAYER	3
#define GAME_MASTER	99

#endif

/***************************************************************************/
