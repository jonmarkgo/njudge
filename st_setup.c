
/*
   ** $Log$
   **
 */

/*  st_setup.c
   **
   **  Copyright 2002, Lowe.
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


#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

static int nu[NPOWER + 1], lu[NPOWER + 1];
int one_owned[NPOWER + 1];

int setupin(char **s, int p)
{
/*  Read build orders in from input file.  */

	char type, order, *t;
	int pow;
	int i, j, u, p1, c1;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: {<order>} <type> <province>
	   **    order ::= build | remove 
	   **    type  ::= Army | Fleet | Wing
	 */


	*s = get_order(*s, &order);
	*s = get_power(*s, &pow);
	*s = get_type(*s, &type);
	*s = get_prov(*s, &p1, &c1);
	if (!p1) {
		errmsg("Unrecognized province -> %s", *s);
		return E_WARN;
	}
	if (p == MASTER) {
		if (pr[p1].unit) {
			p = unit[pr[p1].unit].owner;
		} else {
			p = power(pr[p1].type);
		}
	}


	switch (order) {
	case 'b':
		if (pow == 0) {
			errmsg("Power owner must be specified for build.\n");
			return E_WARN;
		}
		if (type == 'x') {
			errmsg("Unit type must be specified for build.\n");
			return E_WARN;
		}
		if (type == 'A')
			c1 = MV;
		else if (type == 'W' ) {
			c1 = MV;  /* Temporary for Wings */
			if (!(dipent.flags & F_WINGS)) {
				errmsg("This game does not allow wings.\n");
				return E_WARN;
			}	
		} else {
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

		if ((u = pr[p1].unit)) {
		} else {
			nu[p]--;
			u = ++nunit;
		}

		pr[p1].unit = order == 'w' ? 0 : u;
		unit[u].owner = p;
		unit[u].type = type;
		unit[u].stype = 'x';
		unit[u].loc = p1;
		unit[u].coast = c1;
		unit[u].status = order;
		unit[u].order = lu[p];
		lu[p] = u;
		break;

	case 'r':
		if (!(u = pr[p1].unit)) {
			errmsg("%s does not own a unit %s %s to remove.\n",
			powers[p], mov_type(p1,u), pr[p1].name);
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

		unit[u].status = 'd';
		unit[u].order = lu[p];
		lu[p] = u;
		if (!unit[u].exists) {
                    unit[u].owner = 0;
                }

		break;

	default:
		fprintf(rfp, "Invalid build order encountered.\n");
		err++;
		return E_WARN;
	}
	return 0;

}

void setupout(int pt)
{
	int i, u, p, c1, p_index, counting_centres, u_diff;
	char mastrpt_pr[NPOWER + 1];    // Used to be [MAXPLAYERS]. DAN 04/02/1999
        int num_units[NPOWER +1];

	fprintf(rfp, "Adjustment orders for Winter of %d.  (%s.%s)\n\n",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	for ( u = 0; u <= NPOWER; u++)
		num_units[u] = 0;   /* Initialise array */
	
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

			if (unit[u].status == 'b' || unit[u].status == ':') {

				num_units[unit[u].owner]++;
				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "%s %s", Utype(unit[u].type),
					pr[unit[u].loc].name);
				if ((c1 = unit[u].coast) > XC)
					fprintf(rfp, " (%s).\n", mtype[c1]);
				else
					fprintf(rfp, ".\n");
				unit[u].status = ':';
                        }
		}
	}

	for (p = 1; p <= NPOWER; p++) {
		if (( nu[p] > 1) && 
			(processing || pt == p || pt == MASTER)) {
			fprintf(rfp, "%s: ", powers[p]);
			for (i = strlen(powers[p]); i < LPOWER; i++)
				putc(' ', rfp);
			for (i = 1; i <= npr; i++) {
				if (pr[i].owner == p && !pr[i].unit &&
				    (pr[i].type == dipent.pl[p] || (centre(i) &&
				     (dipent.xflags & XF_BUILD_ANYCENTRES))))
					break;
			}
			p_index = FindPower(p);
			counting_centres = dipent.players[p_index].centers - dipent.players[p_index].centres_blockaded;
			u_diff = num_units[p] + dipent.players[p_index].units  - counting_centres;

			if ((dipent.xflags & XF_ALTBUILD) && u_diff != 0 ) {
			    if (!processing && !predict && ! pt == MASTER) 
		                err++;	
			    if (u_diff > 0) 
				fprintf(rfp,"%d too many units: need to remove some.\n", u_diff);
			    else
				fprintf(rfp,"%d too few units: need to build/waive/maintain.\n", -u_diff);
			}	
		}
	}

	if (dipent.xflags & XF_ALTBUILD) {

        } else {
            if (!processing && (nu[pt] < -1 || nu[pt] > 1))
                more_orders++;
        }

	for (u = 1; u <= nunit; u++) {
		if ((p = unit[u].owner) && nu[p] < -1 && pr[unit[u].loc].owner != p) {
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
