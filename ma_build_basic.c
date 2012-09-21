
/*
   ** $Log: ma_build_basic.c,v $
   ** Revision 1.9  2007-08-21 17:42:35  millis
   ** Fix Bug 522 (mastrpt_pr too small)
   **
   ** Revision 1.8  2003-05-12 02:44:37  millis
   ** Removed unused variable
   **
   ** Revision 1.7  2003/05/10 00:46:15  millis
   ** Bug 140 fix, display 'orders' when orders and 'results' when results
   **
   ** Revision 1.6  2003/01/20 12:33:00  millis
   ** Small USTV change
   **
   ** Revision 1.5  2002/10/19 21:39:16  millis
   ** Fixed Bug 21: Mach2 games allowing Armies in Venice
   **
   ** Revision 1.4  2002/02/25 11:51:51  miller
   ** Various updates for Machiavelli bug fixes
   **
   ** Revision 1.1.2.1  2001/10/19 23:34:42  dema
   ** First proper version to allow builds in NoMoney Mach games
   **
   ** Revision 1.1  2001/07/08 22:50:03  miller
   ** Initial revision
   **
   **
 */

/*  ma_build_basic.c
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

/*
 *   Copy of st_build.c, modified for Mach Basic rules 
 */

#include <stdlib.h>
#include <string.h>
#include "dip.h"
#include "functions.h"
#include "porder.h"
#include "mach.h"


static int nu[NPOWER + 1], lu[NPOWER + 1];

/* See if passed location is in conditions to be built on */
int MachCheckOwnedOKBasic( char type, int u, int p, int p1, int *c1)
{
        unsigned char *t;

        if (type == 'x') {
                errmsg("Unit type must be specified.\n");
                return 0;
        }
        if (type != 'F')
                *c1 = MV;
        else {
                if (!(*c1))
                        *c1 = XC;
                for (t = (char *) pr[p1].move; *t; t++)
                        if (*++t >> 4 == *c1)
                                break;
                if (!*t) {
                        errmsg("Invalid coast specified for fleet in %s.\n",
                               pr[p1].name);
                        return 0;
                }
        }
        return 1; /* it passed ok */
}
/****************************************************************************/
int ma_init_build_basic(void)
{
	int i=0, p;
	int move_to_make = 0;

	/*  Count number of units allowed to be built.  */

	for (p = 1; p <= NPOWER; p++) {
		nu[p] = 0;
		lu[p] = 0;
		need_order[p] = 0;
		for (i = 1; i <= npr; i++) {
			/* Owned if both city and province are player AND
			   city has some value (i.e. not a fort) */
			if (pr[i].cown == p && pr[i].owner == p && 
			    (pr[i].flags & 0x7))
				nu[p]++;
		}
		for (i = 1; i <= nunit; i++)
			if (unit[i].owner == p)
				nu[p]--;
		if (nu[p] < 0) {
			move_to_make++;
			nu[p]--;
			need_order[p]++;
		}
		if (nu[p] > 0) {
			move_to_make++;
			nu[p]++;
			need_order[p]++;
		}
		for (i = 1; i <= npr; i++) {
			if (pr[i].cown == p && 
			    ((pr[i].home == p))) { 
			/* was doing something here, no longer! */	
			}
		}
	}
	return move_to_make;
}

int build_syntaxcheck_basic(char *in_text, int precheck, char *out_string)
{
	return 0; /* For now, Mach has no syntax checking */
}

int ma_buildin_basic(char **s, int p)
{

/*  Read build orders in from input file.  */

	char type, order;
	unsigned char *t;
	int i, j, u, p1, c1;
	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: {<order>} <type> <province>
	   **    order ::= build | remove ¦ transform
	   **    type  ::= Army | Fleet | Wing
	 */


	*s = get_order(*s, &order);
	*s = get_type(*s, &type);
	*s = get_prov(*s, &p1, &c1);

	if (type == 'G' && !has_fortress(p1)) {
		errmsg("There is no fort in %s to have a garrison.\n", pr[p1].name);
		return E_WARN;
	}
	
	if (!p1 && order != 'w') {
		errmsg("Unrecognized province -> %s.\n", *s);
		return E_WARN;
	}
	if (p == MASTER) {
		if (pr[p1].unit) {
			p = unit[pr[p1].unit].owner;
		} else {
			p = pr[p1].home;
		}
	}
	if (order == 'x')
		order = nu[p] >= 0 ? 'b' : 'r';

	if ((order == 'b' && (nu[p] <= 0 )) ||
	    (order == 'w' && (nu[p] <= 0 )) ||
	    (order == 'r' && nu[p] >= 0)) {
		errmsg("%s is not permitted to %s any units.\n",
		       powers[p], order == 'r' ? "remove" : "build");
		return E_WARN;
	}
	switch (order) {
	case 'b':
                if (pr[p1].type == 'w' || pr[p1].type == 'l') {
                        errmsg("%s is not a supply centre.\n",
                                pr[p1].name);
                        return E_WARN;
                }
		if (pr[p1].home != p) {
			errmsg("%d %d %s is not a home province for %s.\n",
			       pr[p1].home, dipent.pl[p], 
			       pr[p1].name, powers[p]);
			return E_WARN;
		}
		if (pr[p1].cown != p) {
			errmsg("%s does not control %s city.\n",
			       powers[p], pr[p1].name);
			return E_WARN;
		}
		if (pr[p1].owner != p) {
                        errmsg("%s does not control %s province.\n",
				 powers[p], pr[p1].name);
                        return E_WARN;
                }       
		if ((pr[p1].flags & 0x7) == 0) {
			errmsg("%s is not a city.\n",
			       pr[p1].name);
		}     
		if (type == 'x') {
			errmsg("Unit type must be specified for build.\n");
			return E_WARN;
		}
		if (pr[p1].blockaded) {
			errmsg("Cannot build in %s as it is blockaded.\n",
				pr[p1].name);
			return E_WARN;
		}
		if (type == 'F' && !(pr[p1].flags & PF_PORT)) {
			errmsg("Cannot build fleet in %s as it is not a port.\n",
				pr[p1].name);
			return E_WARN;
		}
		if (type == 'A' && pr[p1].flags & PF_VENICE && dipent.xflags & XF_MACH2) {
			errmsg("Armies not allowed in %s.\n", pr[p1].name);
			return E_WARN;
		}

		if (type != 'F')
			c1 = MV;
		else {
			if (!c1)
				c1 = XC;
			for (t = (char *) pr[p1].move; *t; t++)
				if (*++t >> 4 == c1)
					break;
			if (!*t) {
				errmsg("Invalid coast specified for fleet in %s.\n",
				       pr[p1].name);
				return E_WARN;
			}
		}
                /* If Venice, only allow one unit to exist at a time */
                if (is_venice(p1)) {
                        if ((pr[p1].unit && unit[pr[p1].unit].status != 'b' ) ||
                            (pr[p1].gunit && unit[pr[p1].gunit].status != 'b')) {
                            errmsg("Cannot have more than one unit in %s.\n", pr[p1].name);
                            return E_WARN;
                        }
                }


		/*  FALL THROUGH  */
	case 'w':

		if (type != 'G')
		    u = pr[p1].unit;
		else
		    u = pr[p1].gunit;

		if (u) {
			if (unit[u].status != 'b') {
				errmsg("%s already has a unit present.\n", pr[p1].name);
				return E_WARN;
			} else {
				for (i = lu[p]; i != u && unit[i].order != u; i = unit[i].order);
				if (i == u)
					lu[p] = unit[i].order;
				else
					unit[i].order = unit[u].order;
			}
			if (type != 'G') 
				pr[unit[u].loc].unit = 0;
			else
				pr[unit[u].loc].gunit = 0;

			/*
			   **  If he's building too many, the earliest built gets chucked.
			 */

		} else if (nu[p] == 1) {
			for (u = lu[p], i = 0; unit[u].order; u = unit[u].order)
				i = u;
			if (i)
				unit[i].order = 0;
			else
				lu[p] = 0;
			if (type != 'G') 
				pr[unit[u].loc].unit = 0;
			else 
				pr[unit[u].loc].gunit = 0;
		} else {
			nu[p]--;
			u = ++nunit;
		}

		if (type != 'G') 
		    pr[p1].unit = order == 'w' ? 0 : u;
		else
		    pr[p1].gunit = order == 'w' ? 0 : u; 
		unit[u].owner = p;
		unit[u].type = type;
		unit[u].stype = 'x';
		unit[u].loc = p1;
		unit[u].coast = c1;
		unit[u].status = order;
		unit[u].order = lu[p];
		lu[p] = u;
		break;

	case 'm':
		break;

	case 'r':
		if (type == 'G') 
		    u = pr[p1].gunit;
		else
		    u = pr[p1].unit;

		if (!(u) || unit[u].owner != p) {
			errmsg("%s does not own %s %s %s to remove.\n",
			powers[p], autype(type), mov_type(p1,u), pr[p1].name);
			return E_WARN;
		}

		/*
		   *  If this one's already been removed, put it back.
		 */

		if (unit[u].status == 'd') {
			nu[p]--;
			for (i = lu[p], j = 0; i && i != u; i = unit[i].order)
				j = i;
			if (j)
				unit[j].order = unit[u].order;
			else
				lu[p] = unit[u].order;
		}
		/*
		   *  If he's removing too many, the earliest one removed comes back.
		 */

		if (nu[p] == -1) {
			for (i = lu[p], j = 0; unit[i].order; i = unit[i].order)
				j = i;
			if (j)
				unit[j].order = 0;
			else
				lu[p] = 0;
			unit[i].status = ':';
		} else {
			nu[p]++;
		}

		unit[u].status = 'd';
		unit[u].order = lu[p];
		lu[p] = u;
		break;

	default:
		fprintf(rfp, "Invalid build order encountered.\n");
		err++;
		return E_WARN;
	}
	return 0;
}

void ma_buildout_basic(int pt)
{
	int i, u, p, c1;
	char mastrpt_pr[MASTER + 1];    // Used to be [MAXPLAYERS]. DAN 04/02/1999
	int seq_val;

	seq_val = atoi(dipent.seq) -1; /* = 0 at start of game */

	fprintf(rfp, "Adjustment %s for Winter of %d.  (%s.%s)\n\n",
		 pt ? "orders" : "results",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	if (pt == MASTER) {
		for (u = 0; u <= NPOWER; u++)    // Used to be < MAXPLAYERS. DAN.
			mastrpt_pr[u] = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (mastrpt_pr[unit[u].owner] != 1) {
				mastrpt_pr[unit[u].owner] = 1;
				mast_rpt(unit[u].owner, 1);
			};
		};
		fprintf(rfp, "\n");
	};
	for (u = 1; u <= nunit; u++) {
		if (processing || pt == unit[u].owner || pt == MASTER) {

			/* Also print builds if first turn (adjust) */
			if (unit[u].status == 'b' || seq_val == 0) {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Builds %s in %s", autype(unit[u].type),
					pr[unit[u].loc].name);
				if ((c1 = unit[u].coast) > XC)
					fprintf(rfp, " (%s).\n", mtype[c1]);
				else
					fprintf(rfp, ".\n");
				unit[u].status = ':';

			} else if (unit[u].status == 'd') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Removes the %s %s %s.\n",
					utype(unit[u].type),
					mov_type(unit[u].loc,u),
					pr[unit[u].loc].name);

				unit[u].owner = 0;
				unit[u].status = ':';


			} else if (unit[u].status == 'w') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Build waived.\n");
				unit[u].owner = 0;

			} else if (unit[u].status == 't') {
				/* Only notify when changing type */
                                        fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
                                        for (i = strlen(powers[p]); i < LPOWER; i++)
                                                putc(' ', rfp);
                                        fprintf(rfp, "Transform the %s %s %s to %s",
                                                utype(unit[u].type),
                                                mov_type(unit[u].loc,u),
                                                pr[unit[u].loc].name,
                                                autype(unit[u].new_type));
                                        if ((c1 = unit[u].new_coast) > XC)
                                                fprintf(rfp, " (%s).\n", mtype[c1]);
                                        else
                                                fprintf(rfp, ".\n");
                                        if (processing) {
                                                unit[u].type = unit[u].new_type;
                                                unit[u].coast = unit[u].new_coast;
					}
				unit[u].status = ':';
			}	
		}
	}

	for (p = 1; p <= NPOWER; p++) {
		if (nu[p] > 1 && (processing || pt == p || pt == MASTER)) {
			fprintf(rfp, "%s: ", powers[p]);
			for (i = strlen(powers[p]); i < LPOWER; i++)
				putc(' ', rfp);
			for (i = 1; i <= npr; i++) {
				if (pr[i].owner == p && !pr[i].unit &&
				    (pr[i].home == p))
					break;
			}

			if (i > npr) {
				i = nu[p] - 1;
				fprintf(rfp, "%d unusable build%s pending.\n", i, i == 1 ? "" : "s");
				nu[p] = 1;
			} else {
				i = nu[p] - 1;
	
				fprintf(rfp, "%d build%s pending.\n", i, i == 1 ? "" : "s");
			}
		}
	}

	if (!processing && (nu[pt] < -1 || nu[pt] > 1))
		more_orders++;

	for (u = 1; u <= nunit; u++) {
		if ((p = unit[u].owner) && nu[p] < -1 ) {
			nu[p]++;
			unit[u].owner = 0;
			if (processing || pt == p || pt == MASTER) {
				fprintf(rfp, "%s: ", powers[p]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Defaults, removing the %s in %s%s.\n",
					utype(unit[u].type),
					water(unit[u].loc) ? "the " : "",
					pr[unit[u].loc].name);
			}
		}
	}
}

/****************************************************************************/
