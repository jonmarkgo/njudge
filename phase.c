/*
 * $Log$
 * Revision 1.13  2003/07/21 21:37:45  millis
 * Fix bug 199 (phased waits not working correctly)
 *
 * Revision 1.12  2003/07/17 00:01:29  millis
 * Use MailOut to send emails
 *
 * Revision 1.11  2003/07/13 00:31:04  millis
 * Bug 189 fix, make line[] a static
 *
 * Revision 1.10  2003/05/02 18:19:20  millis
 * More for Bug 137, Optional summer turns
 *
 * Revision 1.9  2002/08/27 22:27:56  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.8  2002/05/11 09:15:34  greg
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
 * Revision 1.7  2002/02/03 23:47:11  nzmb
 * Fixed phased order bug in Ancient Med and made phase.c able to handle a wider range of years (all positive years).
 *
 * Revision 1.6  2001/10/20 12:11:14  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.5.2.1  2001/10/20 00:52:49  dedo
 * Remvoe compile warnings
 *
 * Revision 1.5  2001/07/15 09:18:58  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.4  2001/07/01 23:19:29  miller
 * typo fix
 *
 * Revision 1.3  2001/05/09 06:08:50  greg
 * added subjectline
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Limited syntax checking for phased orders added (only in non-Mach games)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  phase.c

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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "dip.h"
#include "porder.h"
#include "functions.h"

static char line[1024]; /* Local temporary buffer */
extern int msg_header_done;

int phase(char *s)
{

	/*
	 *  Determine whether the specified phase is past, present or future
	 *  and, if something in the future, what type of phase it will be.
	 *
	 *  The return value is:
	 *    -1: Past or invalid phase specification
	 *     0: The current phase
	 *     1: A future movement phase
	 *     2: A future retreat phase
	 *     3: A future adjustment phase
	 */
	char ss[100]; // Use 'ss' as 's' gets overwritten by gamein() call
	char *t;
	
	int i, j;
	int s1, s2 = -999999, y1, y2 = -999999, p1, p2 = -999999;
	strcpy(ss,s);
        t = ss;

        if (!dipent.pr_valid) {
                /* pr[] must be filled to run this code */
                po_init();
                gamein();
                dipent.pr_valid++;
        }

#ifdef notdef 
	fprintf(log_fp, "Phase: Comparing %s and %s", dipent.phase, s);
#endif 				/* notdef */
	s1 = *dipent.phase == 'S' ? 0 : *dipent.phase == 'U' ? 1 : 2;
	y1 = atoi((dipent.phase) + 1);
	p1 = (i = dipent.phase[5]) == 'M' ? 1 : i == 'R' ? 2 : 3;

	i = toupper(ss[0]);
	if (i == 'S' || i == 'U') {
		s2 = 0;
		if ((i == 'U' || !strncasecmp(s, "summer", 6))) {
		    /* See if game can have summer turns */
		    if ((dipent.flags & F_MACH && !(dipent.x2flags & X2F_NOSUMMER)) ||
			(!(dipent.flags & F_MACH) && dipent.x2flags & X2F_SUMMER))
			s2 = 1;
		}
	} else if (i == 'F' || i == 'W') {
		s2 = 2;
	}
	while (*t && !(isdigit((int) *t)))
		t++;
	if (!*t || ((y2 = atoi(t)) < 0 /*1000*/ || y1 + 5 < y2))
		y2 = -1;

	while (isdigit((int) *t))
		t++;
	while (isspace((int) *t))
		t++;
	if ((j = toupper(*t)) == 'M')
		p2 = 1;
	else if (j == 'R')
		p2 = 2;
	else if (j == 'B' || j == 'A' || i == 'W') {
		p2 = 3;
		if (s2)
			s2 = 2;
	}
	if (s2 < 0)
		fprintf(rfp, "Unrecognized season: phase %s\n", ss);

	if (y2 < 0)
		fprintf(rfp, "Invalid year: phase %s\n", ss);

	if (p2 < 0)
		fprintf(rfp, "Unrecognized phase: phase %s\n", ss);

	i =  ((y2 * 100 + s2 * 10 + p2) - (y1 * 100 + s1 * 10 + p1));
	if (i > 0 ) 
		j = p2;
	else if (i == 0) 
		j = 0;
	else
		j = -1;

#ifdef notdef
	fprintf(log_fp, "Phase: Returning %d. y1/s1/p1=%d/%d/%d, y2/s2/p2=%d/%d/%d.\n",
		j, y1, s1, p1, y2, s2, p2);
#endif				/* notdef */
	return j;

}

void phase_pending(void)
{

	/*
	 *  Process the pending orders now that we've advanced to the next phase.
	 *
	 *  We have to go through each power individually so that we can generate
	 *  a report to send to him in case there were any errors.  We'll set the
	 *  "orders received" flag regardless so unless he does a "set wait" he
	 *  could get his orders processed even with errors or if incomplete.
	 */

	int i, n, p, found, skip;
	char *s, Tfile[40], Mfile[40];
	FILE *ifp, *ofp, *tfp = NULL, *mfp;

	static char *words[] =
	{0, "phase", "set#wait", "set#nowait"};

#define PHASE  1
#define WAIT   2
#define NOWAIT 3

	sprintf(Tfile, "%s%s/P%3.3d", GAME_DIR, dipent.name, atoi(dipent.seq) - 1);
	if (!(ifp = fopen(Tfile, "r")))
		return;

	sprintf(Tfile, "%s%s/P%s", GAME_DIR, dipent.name, dipent.seq);
	if (!(ofp = fopen(Tfile, "w"))) {
		perror(Tfile);
		bailout(1);
	}
	sprintf(Tfile, "%s%s/T%s", GAME_DIR, dipent.name, dipent.seq);
	sprintf(Mfile, "%s%s/M%s", GAME_DIR, dipent.name, dipent.seq);

	found = 1;
	for (p = 1; p < WILD_PLAYER; p++) {

		for (n = 0; n < dipent.n; n++)
			if (dipent.players[n].power == p)
				break;
		if (n == dipent.n)
			continue;

		if (found) {
			if (!(tfp = fopen(Tfile, "w"))) {
				perror(Tfile);
				bailout(1);
			}
			if ((mfp = fopen(Mfile, "r"))) {
				while (fgets(line, sizeof(line), mfp)) {
					if (!strcmp(line, "X-marker\n"))
						*line = 'Y';
					fputs(line, tfp);
				}
				fclose(mfp);
			}
			fputs("X-marker\n", tfp);
		}
		found = 0;
		skip = -1;
		rewind(ifp);
		while (fgets(line, sizeof(line), ifp)) {
			if (power(*line) == p) {
				s = lookfor(line + 3, words, nentry(words), &i);
				switch (i) {
				case PHASE:
					if (!(skip = phase(s)))
						continue;
					break;

				case WAIT:
					if (!skip) {
						if (dipent.flags & F_STRWAIT) {
						    if (dipent.players[n].status & SF_MOVE) {
							dipent.players[n].status |= SF_WAIT;
						    }
						} else {
						    dipent.players[n].status |= SF_WAIT;
						}
						continue;
					}
					break;

				case NOWAIT:
					if (!skip) {
						dipent.players[n].status &= ~SF_WAIT;
						continue;
					}
				}

				if (!skip) {
					fputs(line, tfp);
					found++;
				} else if (skip > 0)
					fputs(line, ofp);
			}
		}

		if (found) {
			ferrck(tfp, 3001);
			fclose(tfp);
			rfp = fopen("dip.reply", "w");
			if (porder('T', n, 0) == E_FATAL) {
				sprintf(line, "dip.reply 'Pending orders error'");
				fclose(rfp);
			        msg_header_done = 0;  /* Bug 282, header will need to be redone */

				MailOut(line, GAMES_MASTER);
			} else {

				dipent.players[n].status |= SF_PART;
				if (more_orders) {
					fprintf(rfp, "The partial orders will be processed unless complete ");
					fprintf(rfp, "orders are received by\n%s.\n\n", ptime(&dipent.grace));
				} else {
					dipent.players[n].status |= SF_MOVED;
				}

				rename(Tfile, Mfile);
				sprintf(line, "dip.reply '%s:%s - %s Pending Orders'",
					JUDGE_CODE, dipent.name, dipent.phase);
				fclose(rfp);
			        msg_header_done = 0;  /* Bug 282, header will need to be redone */

				MailOut(line, dipent.players[n].address);
				if (*dipent.players[n].address == '*')
					continue;
				dipent.players[n].status &= ~(SF_ABAND | SF_CD);
			}
		}
	}

	fclose(ofp);
	fclose(ifp);

}
#define CHECKFORIF 1
int phase_syntax(int phase, char *s)
{
	/*
	 *  Do a preliminary syntax check on the given order.  We're just trying
	 *  to warn the player that there may be some sort of problem when we go
	 *  to process his order in the future phase.
	 */

	switch (phase) {
		case 1:
			if (!(dipent.flags & F_MACH) ){
			     if (move_syntaxcheck(s,CHECKFORIF, NULL)) return E_WARN;
				else return 0;
			}
			break;	
		case 2:
			if (!(dipent.flags & F_MACH)) {
                             if (retreat_syntaxcheck(s,CHECKFORIF, NULL)) return E_WARN;
                                else return 0;
			}
			break;
		case 3:
			if (!(dipent.flags & F_MACH)){
                             if (build_syntaxcheck(s,CHECKFORIF, NULL)) return E_WARN;
                                else return 0;
			}
			break;
		default:
			break;
	}

	return 0;			/*  Heck, I don't know!!  Looks good to me. */
}
