/*
   ** $Log$
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
		fprintf(rfp, "Game '%s' is waiting for %d%s player%s to sign on.\n",
		dipent.name, n, n == dipent.no_of_players ? "" : " more",
			n == 1 ? "" : "s");

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

	sprintf(line, "D%s/info", dipent.name);
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
		/* TODO: Reorder so that you can't tell which power signed on first */
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			f = dipent.players[i].status;
			s = "";

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
			fprintf(rfp, "   %-10.10s %-7.7s", powers[dipent.players[i].power], s);
			if (ok_for_blind && 
			     (dipent.players[i].units || dipent.players[i].centers)) {
				ccount = dipent.players[i].centers-dipent.players[i].centres_blockaded;
				fprintf(rfp, "%2d/%d%s",
					dipent.players[i].units, 
					ccount,
					dipent.players[i].centers < 10 ? " " : "");
			} else
				fputs("     ", rfp);

			/* Now display late count if either master or not quiet/late_count */
		        if ((dipent.xflags & XF_LATECOUNT) ) {	
				if ((dipent.players[i].power >= 0 && dipent.players[i].power < WILD_PLAYER) &&
				    ((signedon && (dipent.players[player].power == MASTER || player == i)) || (!(dipent.flags & F_QUIET)))) 
				    fprintf(rfp, " %2d ", dipent.players[i].late_count); 
				else
				    fprintf(rfp, "   ");
			}
			else
				fprintf(rfp, "   ");
			
			if (signedon && (dipent.players[player].power == MASTER || player == i)) {
				if (dipent.players[i].status & SF_PRESS)
					fprintf(rfp,"*");
				else
					fprintf(rfp," ");
			} else {
				fprintf(rfp," ");
			}

	
			if ( (!(dipent.flags & F_GUNBOAT)
			    && 0 < dipent.players[i].userid
			    && dipent.players[i].userid < nded
			    && ded[dipent.players[i].userid].r 
			    && (0 != strcmp(dipent.players[i].password,GONE_PWD))))
				fprintf(rfp, "%4d", ded[dipent.players[i].userid].r);
			else
				fputs("    ", rfp);

			if (dipent.flags & F_GUNBOAT
			    && dipent.players[i].power != MASTER
			    && (!signedon || dipent.players[player].power != MASTER)
			    && ((signedon && player != i)
			|| strcasecmp(raddr, dipent.players[i].address)))
				fprintf(rfp, " %s\n", someone);
			else
				fprintf(rfp, " %s\n", dipent.players[i].address);
		}

		if (dipent.flags & F_QUIET && (lates || abandons)) {
			fputs("\n", rfp);
			if (lates)
				fprintf(rfp, "%d power%s late.  ",
				    lates, lates == 1 ? " is" : "s are");
			if (abandons)
				fprintf(rfp, "%d power%s abandoned.",
					abandons, abandons == 1 ? " is" : "s are");
			fputs("\n", rfp);
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
		fprintf(rfp, "Name      Phase    Variant/Options\n");
		fprintf(rfp, "-------   ------   ---------------\n");
	}
	fprintf(rfp, "%-10.10s%-9.9s", dipent.name, dipent.phase);

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

	if (!(dipent.flags & F_NONMR))
		fprintf(rfp, ", NMR");

	if ((i = (int) dipent.movement.next) < 72)
		fprintf(rfp, ", %d hrs", i);

	if (dipent.access == A_ANY)
		fprintf(rfp, ", Any-site");

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
		n = dipent.np - (dipent.seq[1] - '0');
		if (n == 0) fprintf(rfp, "%21.21sFormed: waiting to be started.\n","");
		else 
		fprintf(rfp, "%21.21sForming: %d%s player%s needed.\n", "",
		    n, n == dipent.np ? "" : " more", n == 1 ? "" : "s");
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

				fprintf(rfp, "%s (%d/%d)", powers[dipent.players[i].power],
					dipent.players[i].units, 
					dipent.players[i].centers-dipent.players[i].centres_blockaded);
			}
		}
		if (n)
			fputs("\n", rfp);
	}
}


/***************************************************************************/
