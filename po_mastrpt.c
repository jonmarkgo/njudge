/*
   ** $Log: po_mastrpt.c,v $
   ** Revision 1.7  2004-07-09 03:32:23  nzmb
   ** Missing #include of functions.h.
   **
   ** Revision 1.6  2004/07/08 22:20:47  millis
   ** Bug 91: small changes to improve status handling and show only
   ** real player information to master
   **
   ** Revision 1.5  2004/05/22 08:51:47  millis
   ** Bug 297: Add Intimate Diplomacy
   **
   ** Revision 1.3  2003/05/02 22:16:57  millis
   ** Remove moves from non-players (stops Chaos games with lots of junk lines)
   **
   ** Revision 1.2  2003/01/14 13:56:46  millis
   ** Updated with ustv merged changed
   **
   ** Revision 1.1.2.1  2003/01/13 16:04:56  millis
   ** ustv latest versions
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.2  1997/03/12 21:33:31  davidn
   ** Fix to ensure judge does not try to send 4Gb of spaces to the gm
   ** when there is a power with more than 10 letters in its name.
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/********************************************************************/
/*           Code for Players' status in Master signon              */
/*                                                                  */
/* Written by Sean Starkey, starkey@netcom - December 16, 1993      */
/********************************************************************/

#include <stdio.h>
#include <string.h>
#include "dip.h"
#include "variant.h"
#include "functions.h"

void mast_rpt(int current_power, int line_up)
{
	int cnt, tmp;

	if (current_power == dipent.has_natives) 
		return;  /* Don't show moves pending from natives */

	if (FindPower(current_power) >= dipent.n)
		return;  /* Don't show moves pending from non-players */

	fprintf(rfp, "%s:", powers[current_power]);
	if (line_up) {
		for (cnt = strlen(powers[current_power]); cnt < LPOWER + 1; cnt++) {
			fprintf(rfp, " ");
		};
	};
	for (cnt = 0; cnt < MAXPLAYERS; cnt++) {
		tmp = 0;
		if (dipent.players[cnt].power == current_power) {
		    if (dipent.players[cnt].controlling_power == 0 || 
		        dipent.players[cnt].controlling_power >= WILD_PLAYER) {
			if ((dipent.players[cnt].status & SF_MOVE) &&
			    !(dipent.players[cnt].status & SF_PART)) {
				fprintf(rfp, " Move Required");
				tmp = 1;
			};
			if ((dipent.players[cnt].status & SF_MOVE) &&
			    (dipent.players[cnt].status & SF_PART)) {
				if (tmp)
					fprintf(rfp, ",");
				fprintf(rfp, " Move Submitted");
				tmp = 1;
			};
			if (~dipent.players[cnt].status & SF_MOVE) {
				if (tmp)
					fprintf(rfp, ",");
				fprintf(rfp, " No Move Required");
				tmp = 1;
			};
			if ((dipent.players[cnt].status & SF_PART) &&
			    !(dipent.players[cnt].status & SF_MOVED)) {
				if (tmp)
					fprintf(rfp, ",");
				fprintf(rfp, " Error Flag");
				tmp = 1;
			};
			if (dipent.players[cnt].status & SF_CD) {
				if (tmp)
					fprintf(rfp, ",");
				tmp = 1;
				fprintf(rfp, " CD");
			};
			if (dipent.players[cnt].status & SF_ABAND) {
				if (tmp)
					fprintf(rfp, ",");
				tmp = 1;
				fprintf(rfp, " Abandoned");
			};
			if (dipent.players[cnt].status & SF_WAIT) {
				if (tmp)
					fprintf(rfp, ",");
				tmp = 1;
				fprintf(rfp, " WAIT");
			};
			fprintf(rfp, ".");
		    } else {
		        fprintf(rfp, " controlled by %s.", powers[dipent.players[RealPlayerIndex(cnt)].power]);
		    }
		};
	};
	fprintf(rfp, "\n");
}
