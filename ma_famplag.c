/*
 * $Log$
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 *
 */

#include <stdlib.h>

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
	char *typetext;
	char Typetext[20];

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
					pr[p].name, typetext);
				unit[u].owner = 0;
				remove_siege(unit[u].loc);
			}
		}
		for (p = 1; p <= npr; p++) {
			remove_infection(p);
		}
	}
	if (t==PLAGUE)
		pftab = ptab;
	else if (t== FAMINE) {
		pftab = ftab;
	} else
	    pftab = stab;

	if ((t == PLAGUE && !NO_PLAGUE) ||
	    (t == FAMINE && !NO_FAMINE) ||
	    (t == STORM && !NO_STORMS)) {

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
			for (p = 1; p < npr; p++) {
				if (is_infected(p)) {
					sprintf(s, "%s%s", s != buf ? ", " : "", pr[p].name);
					while (*s)
						s++;
				}
			}
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
					remove_infection(p);
				}
			}
		}
	}
}
