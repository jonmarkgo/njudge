/*
 * $Log$
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

#include "dip.h"
#include "mail.h"
#include "functions.h"

extern char *accesses[], *levels[];
extern int avalue[], lvalue[], naccess, nlevel;

#define SETFLAGS(set,mask) dipent.flags = (dipent.flags & ~(mask)) | (set)
/***************************************************************************/

void mail_setp(char *s)
{

	int i, k, chk24nmr, n;
	char c, *t, *temp;
	sequence seq;
	long dates;
	struct tm *tm, *localtime();

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
#define PRV_ALLPRESS	'm'
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


	static char *keys[] =
	{"", ",", "press",
	 "address", "a#", "password#", "pw#", "p#",
	 "wait", "w#", "no wait", "now#",
	 "preference", "pref",
	 "moves", "builds", "adjusts", "retreats",
	 "move", "build", "adjust", "retreat",
	 "deadline", "start", "grace",
	 "nonmr", "nocd", "nmr", "cd",
	 "moderated", "unmoderated",
	 "moderate", "unmoderate",
	 "reveal", "no reveal",
	 "nolist", "unlist", "private", "list", "public",
	 "white/grey", "grey/white", "white", "grey",
	 "no white", "no grey", "no press", "none",
	 "gray", "white/gray", "gray/white", "no gray",
	 "no fake broadcast", "no fake",
	 "partial may be faked", "partial may",
	 "partial fakes broadcast by default", "default",
	 "partial default",
	 "partial fakes broadcast", "partial fakes",
	 "no partial", "partial",
	 "observer any", "any observer",
	 "no observer", "observer none", "observer no",
	 "white observer", "observer white",
	 "proxy", "no proxy",
	 "dice", "no dice",
	 "famine", "no famine",
	 "plague", "no plague",
	 "loans", "bankers", "no loans", "no bankers",
	 "bank", "no bank",
	 "assassinations", "assassins", "assassination",
	 "no assassins", "no assassinations", "no assassination",
	 "adjacency", "adjacent", "adj",
	 "no adjacency", "no adjacent", "no adj",
	 "special", "no special",
	 "all press", "normal press",
	 "comment", "info", "information",
	 "quiet", "no quiet",
	 "rated", "unrated",
	 "access", "level", "variant", "dedication",
	 "show", "no show",
	 "dias", "no dias", "nodias",
	 "draw", "no draw", "nodraw",
	 "ep", "ep number", "ep num", "epnum", "epnumber",
	 "centres", "centers",
	 "bn", "bn number", "bn num", "bnnum", "bnnumber",
	 "bnc", "bnc number", "bnc num", "bncnum", "bncnumber",
	 "mn", "mn number", "mn num", "mnnum", "mnnumber",
	 "mnc", "mnc number", "mnc num", "mncnum", "mncnumber",
	 "no strict grace", "nostrictgrace",
	 "strict grace", "strictgrace",
	 "allow player", "allow players", "player allow", "players allow",
	 "deny player", "deny players", "player deny", "players deny",
	 "allow master", "allow masters", "master allow", "masters allow",
	 "deny master", "deny masters", "master deny", "masters deny",
	 "strict wait", "strictwait",
	 "no strict wait", "nostrictwait",
	 "players"};


	static char action[] =
	{'x', SET_NOOP, SET_NOOP,
	 SET_ADD, SET_ADD, SET_PW, SET_PW, SET_PW,
	 SET_WAIT, SET_WAIT, SET_NOWAIT, SET_NOWAIT,
	 SET_PREF, SET_PREF,
	 SET_MOVES, SET_BUILD, SET_BUILD, SET_RETREAT,
	 SET_MOVES, SET_BUILD, SET_BUILD, SET_RETREAT,
	 SET_DEADLINE, SET_START, SET_GRACE,
	 SET_NONMR, SET_NONMR, SET_NMR, SET_NMR,
	 SET_MODERATE, SET_UNMODERATE,
	 SET_MODERATE, SET_UNMODERATE,
	 SET_REVEAL, SET_NOREVEAL,
	 SET_NOLIST, SET_NOLIST, SET_NOLIST, SET_LIST, SET_LIST,
	 SET_WHTGRY, SET_GRYWHT, SET_WHITE, SET_GREY,
	 SET_GREY, SET_WHITE, SET_NOPRESS, SET_NOPRESS,
	 SET_GREY, SET_WHTGRY, SET_GRYWHT, SET_WHITE,
	 SET_NOFAKEB, SET_NOFAKEB,
	 SET_PARTMAY, SET_PARTMAY,
	 SET_PARTFBD, SET_PARTFBD,
	 SET_PARTFBD,
	 SET_FAKEB, SET_FAKEB,
	 SET_NOPART, SET_PART,
	 SET_OBANY, SET_OBANY,
	 SET_OBNONE, SET_OBNONE, SET_OBNONE,
	 SET_OBWHITE, SET_OBWHITE,
	 SET_PROXY, SET_NOPROXY,
	 SET_DICE, SET_NODICE,
	 SET_FAMINE, SET_NOFAMINE,
	 SET_PLAGUE, SET_NOPLAGUE,
	 SET_LOANS, SET_LOANS, SET_NOLOANS, SET_NOLOANS,
	 SET_LOANS, SET_NOLOANS,
	 SET_ASSASS, SET_ASSASS, SET_ASSASS,
	 SET_NOASSASS, SET_NOASSASS, SET_NOASSASS,
	 SET_ADJ, SET_ADJ, SET_ADJ,
	 SET_NOADJ, SET_NOADJ, SET_NOADJ,
	 SET_SPECIAL, SET_NOSPECIAL,
	 SET_ALLPRESS, SET_NOALLPRESS,
	 SET_COMMENT, SET_COMMENT, SET_COMMENT,
	 SET_QUIET, SET_NOQUIET,
	 SET_RATE, SET_NORATE,
	 SET_ACCESS, SET_LEVEL, SET_VARIANT, SET_DEDICATE,
	 SET_SHOW, SET_NOSHOW,
	 SET_DIAS, SET_NODIAS, SET_NODIAS,
	 SET_DRAW, SET_NODRAW, SET_NODRAW,
	 SET_EPNUM, SET_EPNUM, SET_EPNUM, SET_EPNUM, SET_EPNUM,
	 SET_CENTERS, SET_CENTERS,
	 SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN,
	 SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN,
	 SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN,
	 SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN, SET_BN_MN,
	 SET_GRACEDAYS, SET_GRACEDAYS,
	 SET_NOGRACEDAYS, SET_NOGRACEDAYS,
	 SET_PLAYERALLOW, SET_PLAYERALLOW, SET_PLAYERALLOW, SET_PLAYERALLOW,
	 SET_PLAYERDENY, SET_PLAYERDENY, SET_PLAYERDENY, SET_PLAYERDENY,
	 SET_MASTERALLOW, SET_MASTERALLOW, SET_MASTERALLOW, SET_MASTERALLOW,
	 SET_MASTERDENY, SET_MASTERDENY, SET_MASTERDENY, SET_MASTERDENY,
	 SET_STRWAIT, SET_STRWAIT,
	 SET_NOSTRWAIT, SET_NOSTRWAIT,
	 SET_NO_PLAYERS};


	static char privs[] =
	{'x', PRV_NOOP, PRV_NOOP,
	 PRV_ADD, PRV_ADD, PRV_PW, PRV_PW, PRV_PW,
	 PRV_WAIT, PRV_WAIT, PRV_NOWAIT, PRV_NOWAIT,
	 PRV_PREF, PRV_PREF,
	 PRV_MOVES, PRV_BUILD, PRV_BUILD, PRV_RETREAT,
	 PRV_MOVES, PRV_BUILD, PRV_BUILD, PRV_RETREAT,
	 PRV_DEADLINE, PRV_START, PRV_GRACE,
	 PRV_NONMR, PRV_NONMR, PRV_NMR, PRV_NMR,
	 PRV_MODERATE, PRV_UNMODERATE,
	 PRV_MODERATE, PRV_UNMODERATE,
	 PRV_REVEAL, PRV_NOREVEAL,
	 PRV_NOLIST, PRV_NOLIST, PRV_NOLIST, PRV_LIST, PRV_LIST,
	 PRV_WHTGRY, PRV_GRYWHT, PRV_WHITE, PRV_GREY,
	 PRV_GREY, PRV_WHITE, PRV_NOPRESS, PRV_NOPRESS,
	 PRV_GREY, PRV_WHTGRY, PRV_GRYWHT, PRV_WHITE,
	 PRV_NOFAKEB, PRV_NOFAKEB,
	 PRV_PARTMAY, PRV_PARTMAY,
	 PRV_PARTFBD, PRV_PARTFBD,
	 PRV_PARTFBD,
	 PRV_FAKEB, PRV_FAKEB,
	 PRV_NOPART, PRV_PART,
	 PRV_OBANY, PRV_OBANY,
	 PRV_OBNONE, PRV_OBNONE, PRV_OBNONE,
	 PRV_OBWHITE, PRV_OBWHITE,
	 PRV_PROXY, PRV_NOPROXY,
	 PRV_DICE, PRV_NODICE,
	 PRV_FAMINE, PRV_NOFAMINE,
	 PRV_PLAGUE, PRV_NOPLAGUE,
	 PRV_LOANS, PRV_LOANS, PRV_NOLOANS, PRV_NOLOANS,
	 PRV_LOANS, PRV_NOLOANS,
	 PRV_ASSASS, PRV_ASSASS, PRV_ASSASS,
	 PRV_NOASSASS, PRV_NOASSASS, PRV_NOASSASS,
	 PRV_ADJ, PRV_ADJ, PRV_ADJ,
	 PRV_NOADJ, PRV_NOADJ, PRV_NOADJ,
	 PRV_SPECIAL, PRV_NOSPECIAL,
	 PRV_ALLPRESS, PRV_NOALLPRESS,
	 PRV_COMMENT, PRV_COMMENT, PRV_COMMENT,
	 PRV_QUIET, PRV_NOQUIET,
	 PRV_RATE, PRV_NORATE,
	 PRV_ACCESS, PRV_LEVEL, PRV_VARIANT, PRV_DEDICATE,
	 PRV_SHOW, PRV_NOSHOW,
	 PRV_DIAS, PRV_NODIAS, PRV_NODIAS,
	 PRV_DRAW, PRV_NODRAW, PRV_NODRAW,
	 PRV_EPNUM, PRV_EPNUM, PRV_EPNUM, PRV_EPNUM, PRV_EPNUM,
	 PRV_CENTERS, PRV_CENTERS,
	 PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	 PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	 PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	 PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN, PRV_BN_MN,
	 PRV_GRACEDAYS, PRV_GRACEDAYS,
	 PRV_NOGRACEDAYS, PRV_NOGRACEDAYS,
	 PRV_PLAYERALLOW, PRV_PLAYERALLOW, PRV_PLAYERALLOW, PRV_PLAYERALLOW,
	 PRV_PLAYERDENY, PRV_PLAYERDENY, PRV_PLAYERDENY, PRV_PLAYERDENY,
	 PRV_MASTERALLOW, PRV_MASTERALLOW, PRV_MASTERALLOW, PRV_MASTERALLOW,
	 PRV_MASTERDENY, PRV_MASTERDENY, PRV_MASTERDENY, PRV_MASTERDENY,
	 PRV_STRWAIT, PRV_STRWAIT,
	 PRV_NOSTRWAIT, PRV_NOSTRWAIT,
	 PRV_NO_PLAYERS};


	chk24nmr = 0;
	while (*s) {
		s = lookfor(t = s, keys, nentry(keys), &i);
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
			}
			s = "";
			break;

		case SET_GRACE:
			if (mail_date(&s, &dates, 0, rfp))
				fprintf(rfp, "%sInvalid grace date specified.\n\n", t);
			else {
				dipent.grace = dates;
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

			/* Copy the password from the input line */
			while (*s && !isspace(*s)) {
				*t++ = isupper(c = *s++) ? tolower(c) : c;
			}

			/* If no characters have been copied */
			if (t == dipent.players[player].password) {
				/* Insult player */
				fprintf(rfp, "Its bozos like you who give RTers a bad name.\n\n");
			} else {
				/* Terminate the player and confirm change */
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
				for (t = s; *s && !isspace(*s) && *s != ','; s++);
				if (*s)
					*s++ = '\0';
				while (isspace(*s))
					s++;
				if (!*t || *t == '\n') {
					fprintf(rfp, "Preference list cleared.\n");
					*dipent.players[player].pref = '\0';
				} else if (chkpref(t, NULL, NULL)) {
					fprintf(rfp, "Preference list '%s' not set.\n", t);
					if (*dipent.players[player].pref) {
						fprintf(rfp, "Preference list remains '%s'.\n",
							dipent.players[player].pref);
					}
				} else {
					fprintf(rfp, "Preference list '%s' set.\n", t);
					strcpy(dipent.players[player].pref, t);
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
			if (check_can_vote(player))
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
			if (check_can_vote(player))
				break;
			if (dipent.flags & F_NODIAS)
				fprintf(rfp, "You will now only accept a concession.\n");
			else
				fprintf(rfp, "You will now not accept a DIAS draw.\n");
			dipent.players[player].status &= ~SF_DRAW;
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
			s = lookfor(s, variants, NVARIANT + NVAROPTS, &i);
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
				if (dipent.flags & F_GUNBOAT &&
				dipent.players[player].power != MASTER) {
					xaddr = someone;
				}
				broad_params = 1;
			}
			break;

		case SET_FLAG:
			fprintf(rfp, "Flag word used2b %x.\n", dipent.flags);
			sscanf(s, "%x", &dipent.flags);
			fprintf(rfp, "Flag word set to %x.\n", dipent.flags);
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
			broad_params = 1;
			break;

		case SET_OBANY:
			SETFLAGS(0, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_OBNONE:
			SETFLAGS(F_OBNONE, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_OBWHITE:
			SETFLAGS(F_OBWHITE, F_OBNONE | F_OBWHITE);
			broad_params = 1;
			break;

		case SET_NOPART:
			SETFLAGS(F_NOPARTIAL, F_NOPARTIAL);
			broad_params = 1;
			break;

		case SET_PART:
			SETFLAGS(0, F_NOPARTIAL);
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
			SETFLAGS(F_NODICE | F_NOFAMINE | F_NOPLAGUE | F_NOLOANS | F_NOASSASS,
				 F_NODICE | F_NOFAMINE | F_NOPLAGUE | F_NOLOANS | F_NOASSASS);
			broad_params = 1;
			break;

		case SET_DICE:
			SETFLAGS(0, F_NODICE);
			broad_params = 1;
			break;

		case SET_NOFAMINE:
			SETFLAGS(F_NOFAMINE, F_NOFAMINE);
			broad_params = 1;
			break;

		case SET_FAMINE:
			SETFLAGS(0, F_NOFAMINE | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOPLAGUE:
			SETFLAGS(F_NOPLAGUE, F_NOPLAGUE);
			broad_params = 1;
			break;

		case SET_PLAGUE:
			SETFLAGS(0, F_NOPLAGUE | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOLOANS:
			SETFLAGS(F_NOLOANS, F_NOLOANS);
			broad_params = 1;
			break;

		case SET_LOANS:
			SETFLAGS(0, F_NOLOANS | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOASSASS:
			SETFLAGS(F_NOASSASS, F_NOASSASS);
			broad_params = 1;
			break;

		case SET_ASSASS:
			SETFLAGS(0, F_NOASSASS | F_NODICE);
			broad_params = 1;
			break;

		case SET_NOADJ:
			SETFLAGS(F_NOADJ, F_NOADJ);
			broad_params = 1;
			break;

		case SET_ADJ:
			SETFLAGS(0, F_NOADJ);
			broad_params = 1;
			break;

		case SET_NOSPECIAL:
			SETFLAGS(F_NOSPECIAL, F_NOSPECIAL);
			broad_params = 1;
			break;

		case SET_SPECIAL:
			SETFLAGS(0, F_NOSPECIAL);
			broad_params = 1;
			break;

		case SET_ALLPRESS:
			if (dipent.players[player].power != MASTER) {
				fprintf(rfp, "Sorry, only the master can request all press.\n");
				break;
			}
			dipent.players[player].status |= SF_PRESS;
			fprintf(rfp, "You will now receive all the partial press.  Be sure ");
			fprintf(rfp, "to pay attention to\nnot leak any information thus ");
			fprintf(rfp, "gained to your other players.\n");
			break;

		case SET_NOALLPRESS:
			dipent.players[player].status &= ~SF_PRESS;
			fprintf(rfp, "You will now receive only the normal amount of press.\n");
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
				sprintf(line, "D%s/summary", dipent.name);
				remove(line);
				sprintf(line, "D%s/msummary", dipent.name);
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
				sprintf(line, "D%s/summary", dipent.name);
				remove(line);
				sprintf(line, "D%s/msummary", dipent.name);
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

				sprintf(line, "D%s/info", dipent.name);
				if (!(tfp = fopen(line, "w"))) {
					perror(line);
					fprintf(log_fp, "Can't open info file: %s", line);
					bailout(1);
				}
				fprintf(rfp, "----- Full comment set:\n");
				for (n = skipping = 0; fgets(line, sizeof(line), stdin); n++) {
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

				sprintf(line, "D%s/summary", dipent.name);
				remove(line);
				sprintf(line, "D%s/msummary", dipent.name);
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
					mail_igame();
				}
				broadcast = 1;
			}
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
		sprintf(line, "D%s/%s", dipent.name, basename);
	add_player(player_name, line, addflag);

	*info = s;
}


/***************************************************************************/
