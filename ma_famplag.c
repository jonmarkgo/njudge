/*
 * $Log$
 * Revision 1.8  2003/06/10 10:06:56  millis
 * Bug 173, missed a '=' in use of npr
 *
 * Revision 1.7  2003/01/05 00:06:43  millis
 * Fix bug 90
 *
 * Revision 1.6  2002/12/22 02:02:24  millis
 * corrected bug 56 (making storms at end of Fall)
 *
 * Revision 1.5  2002/02/25 11:51:51  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.3.2.1  2001/10/19 23:36:03  dema
 * Added missing include
 *
 * Revision 1.3  2001/08/19 14:31:11  miller
 * Fixed small message error for famined units removed duing plague
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Added Storms
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 *
 */

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "mail.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

/***************************************************************************/
void fam_plag(int t)
{

/*
 *  Generate famine results.
 */

	int i, d, r, c, u, p;
	char *s, buf[1024];
	PFTAB(pftab);
	char *typetext = NULL;
	char Typetext[20];
	int at_least_one = 0;   /* True if at least one province is struck */

	switch (t)
	{
	    case FAMINE:
		typetext = "famine";
		break;
	    case PLAGUE:
		typetext = "plague";
		break;
	   case STORM:
		typetext = "storm";
		break;
	   default:
		bailout(1);  /* Unknown famine/plague type */
	}
	strcpy(Typetext,typetext);
	Typetext[0] = toupper(typetext[0]);

	/*
	 *  Eliminate any units remaining in famine provinces at start of plague.
	 */

	d = 0;
	if (t == PLAGUE) {
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (is_infected(p = unit[u].loc) &&
			   (t != STORM || unit[u].type == 'F')) {
				if (!d++)
					fputc('\n', rfp);
				fprintf(rfp, "%s %s%s in %s eliminated by %s.\n",
					owners[unit[u].owner],
				Stype(unit[u].stype), Utype(unit[u].type), 
					pr[p].name, "famine");
				unit[u].owner = 0;
				remove_siege(unit[u].loc);
			}
		}
		for (p = 1; p <= npr; p++) {
			remove_infection(p);
		}
	}

	if (t == PLAGUE)
	    if (atoi(&dipent.phase[1]) < 1348) return;  /* No plague prior to 1348 */

	if (t==PLAGUE)
		pftab = ptab;
	else if (t== FAMINE) {
		pftab = ftab;
	} else
	    pftab = stab;

	if ((t == PLAGUE && !NO_PLAGUE) ||
	    (t == FAMINE && !NO_FAMINE) ||
	    (t == STORM && !NO_STORMS)) {

		if (t == STORM)
			die_rolls(DIE_STORMS);
		else
			die_rolls(DIE_FAMPLAG);

		if (dipent.xflags & XF_MACH2) {
		    d = die(1,6);
		    /* Now map to 2d6 settings */
		   switch (d)
		   {
			case 1:  /* No diasters */
			    d = 2;
			    break;
			case 2:
			case 3: /* Row only */
			    d = 4;
			    break;
			case 4:
			case 5:
			    d = 5;
			    break;

			default:
			   d = 12;
		    }
		}
		else
		    d = die(2,6);

		fprintf(rfp, "\n%s results: %d.   ", Typetext , d );
		r = c = 0;
		switch (d) {
		case 2:
		case 3:
			fprintf(rfp, "No disaster.\n");
			break;

		case 4:
		case 6:
			fprintf(rfp, "Good year, row %d only.\n", r = die(2, 6));
			break;

		case 5:
		case 7:
			fprintf(rfp, "Good year, column %d only.\n", c = die(2, 6));
			break;

		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
/*         fprintf(rfp,"Bad year, row %d and column %d.\n",r=die(2,6),c=die(2,6)); */
/* Somehow the row and colums on EFF are reversed */
			c = die(2, 6);
			r = die(2, 6);
			fprintf(rfp, "Bad year, row %d and column %d.\n", r, c);
			break;

		default:
			fprintf(rfp, "Geez, must be loaded dice!!\n");
		}

		/*
		   **  Set the famine flag for all the affected provinces.
		 */

		if (r || c) {
			if (r)
				for (i = 2; i <= 12; i++) {
					if ((p = pftab[r - 2][i - 2])) {
						set_infection(p);
					}
				}
			if (c)
				for (i = 2; i <= 12; i++) {
					if ((p = pftab[i - 2][c - 2])) {
						set_infection(p);
					}
				}
			/*
			   **  Generate the list of provinces (it's sorted this way).
			 */

			fprintf(rfp, "\nProvinces struck: ");
			s = buf;
			for (p = 1; p <= npr; p++) {
			        if (t == STORM && !water(p))
					continue;  /* No storms on land! */
				if (t!= STORM && water(p))
					continue;  /* No famine/plauge on water! */
				if (is_infected(p)) {
					sprintf(s, "%s%s", s != buf ? ", " : "", pr[p].name);
					while (*s)
						s++;
				    at_least_one = 1;
				}
			}
			if (!at_least_one)
			    fprintf(rfp,"*None*");

			sprintf(s, ".\n");
			wrap(rfp, buf, 18, 18);

			/*
			   **  Eliminate any unit in a plagued/storm province.
			 */

			if (t != FAMINE) {
				d = 0;
				for (u = 1; u <= nunit; u++) {
					if (unit[u].owner <= 0)
						continue;
					if (is_infected(p = unit[u].loc) && 
					   ( t != STORM || unit[u].type == 'F')) {
						if (!d++)
							fputc('\n', rfp);
						fprintf(rfp, "%s %s%s in %s eliminated by %s.\n",
						   owners[unit[u].owner],
							Stype(unit[u].stype), Utype(unit[u].type),
							pr[p].name, typetext);
						unit[u].owner = 0;
						remove_siege(unit[u].loc);
					}
				}
				for (p = 1; p <= npr; p++) {
                                        if (t != STORM || water(p))
					    remove_infection(p);
				}
			}
		}
	}
}
