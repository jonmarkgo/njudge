/*
   ** $Log$
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

void mast_rpt(int current_power, int line_up)
{
	int cnt, tmp;

	fprintf(rfp, "%s:", powers[current_power]);
	if (line_up) {
		for (cnt = strlen(powers[current_power]); cnt < LPOWER + 1; cnt++) {
			fprintf(rfp, " ");
		};
	};
	for (cnt = 0; cnt < MAXPLAYERS; cnt++) {
		tmp = 0;
		if (dipent.players[cnt].power == current_power) {
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
		};
	};
	fprintf(rfp, "\n");
}
