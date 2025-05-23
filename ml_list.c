/*
   ** $Log$
   ** Revision 1.15  2009-01-25 02:44:46  alange
   **
   ** Bug 552. sprintf reading and writing from same buffer is undefined.
   **     Deleted old commented out code.
   **
   ** Revision 1.14  2004-07-12 00:41:12  millis
   ** Fix bug 335 (don't special list Any-Site games)
   **
   ** Revision 1.13  2004/07/07 23:20:45  millis
   ** Bug 332: Do not reveal player move status in a secret game
   **
   ** Revision 1.12  2004/06/09 22:05:08  millis
   ** More fixes for Bug 297, Intimate Diplomacy
   **
   ** Revision 1.11  2004/05/22 08:56:53  millis
   ** Bug 297: Add Intimate Diplomacy
   **
   ** Revision 1.10  2003/07/28 14:23:09  millis
   ** Fix bug 205
   **
   ** Revision 1.9  2003/01/14 13:51:37  millis
   ** Merges from USTV
   **
   ** Revision 1.8  2002/08/27 22:27:54  millis
   ** Updated for automake/autoconf functionality
   **
   ** Revision 1.7  2002/04/18 04:44:32  greg
   ** Added the following commands:
   ** - unstart
   ** - set secret
   ** - set [prflist|prfrand|prfboth]
   **
   ** Fixed Set Absence so that "to" is not case sensitive
   **
   ** Fixed Quiet games so that new players are announced
   ** before the game starts
   **
   ** Fixed ascii_to_ded.c so thatit no longer generates an
   ** error when compiled
   **
   ** Revision 1.6  2001/07/15 09:17:15  greg
   ** added support for game directories in a sub directory
   **
   ** Revision 1.5  2001/05/13 17:18:42  greg
   ** took off ")" at line 224
   **
   ** Revision 1.4  2001/05/12 07:42:55  greg
   ** minor bug fixes proposed by Mario Becroft & Philippe Lalande
   ** to display zero dedications in listing and problems with
   ** user's dedication
   **
   ** Revision 1.3  2001/04/15 21:21:22  miller
   ** Add use of GetMaxCountryStrlen() to space out properly for big country names
   **
   ** Revision 1.2  2000/11/14 14:27:37  miller
   ** Various changes including:
   **  - Adjust display for blind games
   **  - Display Late Count if enabled
   **  - Display to master who is whatching press
   **  - Show new flags
   **  - Different message if game has quota but has XF_MANUALSTART set
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.2  1997/02/16 20:43:18  davidn
   ** Additions to dipent structure and associated code, to allow duplex variants.
   ** Command is "set players n".
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  ml_list.c -- Generate listings of game parameters.
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
   **  DATE        NAME         REASON
   **  ----------- ------------ -----------------------------------------
   **  ?? ??? 1987 Ken Lowe     He wrote it
   **  30 Dec 1996 David Norman Additions for dipent.no_of_players
   **  18 Nov 1999 Millis Miller Changed display for a non-started, terminated 
				 game
   **  26 Nov 1999 Millis Miller Added Display if in Shrthand mode
   */

#include <string.h>
#include <time.h>

#include "config.h"
#include "functions.h"
#include "dip.h"
#include "mail.h"


/***************************************************************************/

void mail_listit(void)
{

/*  List out current status of the game  */

	int i, f, n;
	int ccount;
	char *s, line[150];
	long now;
	FILE *fp;
	int ok_for_blind = 0;

	/* If it's not blind, ok to show everything */
	if (!(dipent.flags & F_BLIND)) ok_for_blind = 1;

	time(&now);

	if (dipent.phase[6] == 'X' && dipent.seq[0] == 'x') {
	       fprintf(rfp, "Game '%s' is already terminated before starting.\n", 
			dipent.name);
	}
	else if (dipent.seq[0] == 'x') {

		n = dipent.no_of_players - (dipent.seq[1] - '0');

		if (!(dipent.flags & F_QUIET)) {
			fprintf(rfp, "Game '%s' is waiting for %d%s player%s to sign on.\n",
			dipent.name, n, n == dipent.no_of_players ? "" : " more",
				n == 1 ? "" : "s");
		}

	} else if (dipent.phase[6] == 'X') {
		ok_for_blind = 1; /* Game over, so ok to show all for blind */

		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (dipent.players[i].centers >= dipent.vp) {
				fprintf(rfp, "Game '%s' completed in %4.4s.\n",
					dipent.name, &dipent.phase[1]);
				break;
			}
		}
		if (i == dipent.n) {
			fprintf(rfp, "Game '%s' was terminated in %s of %4.4s.\n", dipent.name,
				dipent.phase[1] == 'F' ? "fall" :
				dipent.phase[1] == 'U' ? "summer" :
				"spring", &dipent.phase[1]);
		}
	} else {

		fprintf(rfp, "Game '%s' order #%s (%s) has a deadline of %s.\n",
			dipent.name, dipent.seq, dipent.phase, ptime(&dipent.deadline));
		if (now < dipent.start)
			fprintf(rfp, "The earliest orders will be processed is %s.\n",
				ptime(&dipent.start));
		if (now < dipent.deadline) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if (WAITING(dipent.players[i].status)) {
					fprintf(rfp, "One or more players have not gotten their orders in.\n");
					break;
				}
			}
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status &
				     (SF_ABAND | SF_CD | SF_WAIT)) == SF_WAIT) {
					fprintf(rfp, "One or more players have requested that orders not be ");
					fprintf(rfp, "processed\nuntil the deadline.\n");
					break;
				}
			}
		}
		fprintf(rfp, "Those who haven't gotten their orders in will be %s if ",
			dipent.flags & F_NONMR ? "abandoned" : "CD");
		fprintf(rfp, "nothing\nis received by %s.\n", ptime(&dipent.grace));
	}

	fprintf(rfp, "\nThe parameters for '%s' are as follows:\n", dipent.name);
	params(rfp);

	sprintf(line, "%s%s/info", GAME_DIR, dipent.name);
	if ((fp = fopen(line, "r"))) {
		fputc('\n', rfp);
		while (fgets(line, sizeof(line), fp))
			fputs(line, rfp);
		fclose(fp);
	}
	if (dipent.n) {
		
		int lates, abandons;
		fprintf(rfp, "\nThe following players are signed up for game '%s':\n",
			dipent.name);
		lates = abandons = 0;

		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			f = dipent.players[i].status;
			s = "";

			if (!(dipent.x2flags & X2F_SECRET)) {

			    if (f & (SF_CD | SF_ABAND))
				abandons++;

			    if (now > dipent.deadline && WAITING(f))
				lates++;

			    if (dipent.flags & F_QUIET && (!signedon ||
						       (player != i && dipent.players[player].power != MASTER))) {
				if (f & SF_MOVE && ok_for_blind) {
					s = "move";
				}
			    } else if (f & SF_CD) {
				s = "CD";
			    } else if (f & SF_ABAND) {
				s = "abandon";
			    } else if (f & SF_MOVE && ok_for_blind) {
				s = "move";
				if (now > dipent.deadline && WAITING(f)) {
					s = f & SF_PART ? "part" : "late";
				}
			    }
			}

			sprintf(line, "   %-*.*s %-7.7s", GetMaxCountryStrlen(), GetMaxCountryStrlen(), powers[dipent.players[i].power], s);

			if (ok_for_blind && 
			     (dipent.players[i].units || dipent.players[i].centers)) {
				ccount = dipent.players[i].centers-dipent.players[i].centres_blockaded;
				sprintf(line + strlen(line), "%2d/%d%s",
					dipent.players[i].units, 
					ccount,
					dipent.players[i].centers < 10 ? " " : "");

			} else
				strcat(line, "     ");

			/* Now display late count if either master or not quiet/late_count */
		        if ((dipent.xflags & XF_LATECOUNT) ) {	
				if ((dipent.players[i].power >= 0 && dipent.players[i].power < WILD_PLAYER) &&
				    ((signedon && (dipent.players[player].power == MASTER || player == i)) || (!(dipent.flags & F_QUIET)))) 
					sprintf(line + strlen(line), " %2d ", dipent.players[i].late_count);
				else
					strcat(line, "   ");
			}
			else
				strcat(line, "   ");
			
			if (signedon && (dipent.players[player].power == MASTER || player == i)) {
				if (dipent.players[i].status & SF_PRESS)
					strcat(line, "*");
				else
					strcat(line, " ");
			} else {
				strcat(line, " ");
			}

	
			if ( (!(dipent.flags & F_GUNBOAT)
			    && 0 < dipent.players[i].userid
			    && dipent.players[i].userid <= nded)
/*			    && ded[dipent.players[i].userid].r */
			    && (0 != strcmp(dipent.players[i].password,GONE_PWD)))
				sprintf(line + strlen(line), "%4d", ded[dipent.players[i].userid].r);
			else
				strcat(line, "    ");

			if (dipent.flags & F_GUNBOAT
			    && dipent.players[i].power != MASTER
			    && (!signedon || dipent.players[player].power != MASTER)
			    && ((signedon && player != i)
			|| strcasecmp(raddr, dipent.players[i].address)))
				if (dipent.flags & F_INTIMATE && dipent.players[i].controlling_power !=0 && !(dipent.x2flags & X2F_SECRET)) {
				 strcat(line, "\n");
				} else if (IS_DUPLEX(dipent) && !(dipent.x2flags & X2F_SECRET)) {
				    /* TODO, work out how to show player's numeral index */
				    sprintf(line + strlen(line), " %s\n", someone);
				} else
					sprintf(line + strlen(line), " %s\n", someone);
			else
				sprintf(line + strlen(line), " %s\n", dipent.players[i].address);

			if (!(dipent.x2flags & X2F_SECRET) ||
			  (dipent.seq[0] != 'x') ||
			  (signedon && dipent.players[player].power == MASTER) ||
			  (dipent.players[i].power == MASTER) ||
			  !(strcasecmp(raddr, dipent.players[i].address)))
				fprintf(rfp, "%s", line);

		}

		if ((dipent.seq[0] == 'x') &&
		  (dipent.x2flags & X2F_SECRET) &&
		  !(signedon && dipent.players[player].power == MASTER)) {
			fprintf(rfp, "\n  Other players kept secret.\n");
		}

		if (dipent.flags & F_QUIET && (lates || abandons)) {
			fputs("\n", rfp);
			if (lates) {
				if (dipent.x2flags & X2F_SECRET)
					fprintf(rfp, "One or more powers are late.\n");
				else
					fprintf(rfp, "%d power%s late.\n", lates, lates == 1 ? " is" : "s are");
			}
			if (abandons) {
				if (dipent.x2flags & X2F_SECRET)
					fprintf(rfp, "One or more powers are abandoned.\n");
				else
					fprintf(rfp, "%d power%s abandoned.\n", abandons, abandons == 1 ? " is" : "s are");
			}

/*			fputs("\n", rfp); */
		}
	}


	fputs("\n", rfp);

}


/***************************************************************************/

void mail_listbrief(void)
{

/*  List out current status of the game briefly */

	int i, n;
	char *s;
	static int count = 0;


	if (!count++) {
		fprintf(rfp, "Name              Phase    Variant/Options\n");
		fprintf(rfp, "---------------   ------   ---------------\n");
	}
	fprintf(rfp, "%-18.18s%-9.9s", dipent.name, dipent.phase);

	fprintf(rfp, "%c%s",
	toupper(*variants[dipent.variant]), variants[dipent.variant] + 1);

	if (dipent.flags & F_GUNBOAT)
		fprintf(rfp, ", Gunboat");

	if (dipent.flags & F_BLIND)
		fprintf(rfp, ", Blind");

	if (dipent.flags & F_AFRULES)
		fprintf(rfp, ", A/F");

        if (dipent.flags & F_SHORTHAND)
		fprintf(rfp, ", Shorthand");

	if (dipent.flags & F_WINGS)
		fprintf(rfp, ", Wings");

	if (dipent.flags & F_INTIMATE)
	        fprintf(rfp, ", Intimate");

	if (!(dipent.flags & F_NONMR))
		fprintf(rfp, ", NMR");

	if ((i = (int) dipent.movement.next) < 72)
		fprintf(rfp, ", %d hrs", i);

	if (dipent.access == A_DIFF)
		fprintf(rfp, ", Different-site");

	if (dipent.access == A_SAME)
		fprintf(rfp, ", Same-site");

	if (dipent.flags & F_MODERATE) {
		fprintf(rfp, ", Moderated (");
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power == MASTER) {
				for (n = 0, s = dipent.players[i].address; *s && *s != '@'; s++)
					fputc(!n++ && islower(*s) ? toupper(*s) : *s, rfp);
				break;
			}
		}
		fputc(')', rfp);
	}
/*  Comment this out to not display press in brief listing   *RV*  */
/*    Comment until the next "*RV*" */

	fprintf(rfp, ", Press:");

	if (dipent.flags & F_NOWHITE)
		fprintf(rfp, "-");
	else
		fprintf(rfp, "W");

	if (dipent.flags & F_GREY)
		fprintf(rfp, "G");
	else
		fprintf(rfp, "-");

	if (dipent.flags & F_NOPARTIAL || dipent.flags & F_NOWHITE)
		fprintf(rfp, "-");
	else
		fprintf(rfp, "P");

	if (dipent.flags & F_FAKE)
		fprintf(rfp, "F");
	else
		fprintf(rfp, "-");

/*     *RV*             */

	if (dipent.flags & F_NORATE)
		fprintf(rfp, ", Unrated");

/*  Uncomment this for a display of Winning Centres in brief listing */
/*  fprintf(rfp,", Win=%d",dipent.vp); */

	if (dipent.phase[6] == 'X') {
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (dipent.players[i].centers >= dipent.vp) {
				fprintf(rfp, ", Completed");
				break;
			}
		}
		if (i == dipent.n) {
			fprintf(rfp, ", Terminated");
		}
	}
	fputs(".\n", rfp);

	if ((n = strlen(dipent.comment))) {
		if (n < 60)
			n = 20;
		else if (n < 69)
			n = 11;
		else
			n = 79 - n;
		fprintf(rfp, "%*s%s\n", n, "", dipent.comment);
	}
        
	if (dipent.phase[6] != 'X' && dipent.seq[0] == 'x') {
		n = dipent.no_of_players - (dipent.seq[1] - '0');
		if (n == 0) fprintf(rfp, "%21.21sFormed: waiting to be started.\n","");
		else if (dipent.x2flags & X2F_SECRET) {
		    fprintf(rfp, "%21.21sForming: more players needed.\n", "" );
		} else {
		     fprintf(rfp, "%21.21sForming: %d%s player%s needed.\n", "",
		        n, n == dipent.no_of_players ? "" : " more", n == 1 ? "" : "s");
		}
	} else if (dipent.phase[6] != 'X') {
		n = 0;
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if ((dipent.players[i].status & (SF_CD | SF_ABAND)
			     || *dipent.players[i].address == '*') &&
			    (dipent.players[i].status & SF_MOVE
			     || dipent.players[i].centers != 0)) {

				if (!n++)
					fprintf(rfp, "%21.21sOpenings: ", "");
				else
					fputs(", ", rfp);

				fprintf(rfp, "%s", powers[dipent.players[i].power]);
                                if (!(dipent.flags & F_BLIND))
                                    fprintf(rfp, " (%d/%d)",
                                        dipent.players[i].units,
                                        dipent.players[i].centers-dipent.players[i].centres_blockaded);
 			}
		}
		if (n)
			fputs("\n", rfp);
	}
}


/***************************************************************************/
