
/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  st_build.c
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

#include <stdlib.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

static int nu[NPOWER + 1], lu[NPOWER + 1], cnb[NPOWER + 1];

/****************************************************************************/
void init_build(void)
{
	int i, p;

	/*  Count number of units allowed to be built.  */

	for (p = 1; p <= NPOWER; p++) {
		nu[p] = 0;
		lu[p] = 0;
		for (i = 1; i <= npr; i++)
			if (pr[i].owner == p)
				nu[p]++;
		for (i = 1; i <= nunit; i++)
			if (unit[i].owner == p)
				nu[p]--;
		if (nu[p] < 0)
			nu[p]--;
		if (nu[p] > 0)
			nu[p]++;
		cnb[p] = (dipent.variant == V_aberration)
		    || (dipent.variant == V_h31)
		    || (dipent.variant == V_h32)
		    || (dipent.variant == V_classical);;

		for (i = 1; i <= npr; i++)
			if (pr[i].owner == p && pr[i].type == dipent.pl[p])
				cnb[p] = 0;
	}
}

int buildin(char **s, int p)
{

/*  Read build orders in from input file.  */

	char type, order, *t;
	int i, j, u, p1, c1;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: {<order>} <type> <province>
	   **    order ::= build | remove
	   **    type  ::= Army | Fleet
	 */


	*s = get_order(*s, &order);
	*s = get_type(*s, &type);
	*s = get_prov(*s, &p1, &c1);
	if (!p1 && order != 'w') {
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
	if (order == 'x')
		order = nu[p] >= 0 ? 'b' : 'r';

	if ((order == 'b' && (nu[p] <= 0 || cnb[p])) ||
	    (order == 'w' && (nu[p] <= 0 || cnb[p])) ||
	    (order == 'r' && nu[p] >= 0)) {
		errmsg("%s is not permitted to %s any units.\n",
		       powers[p], order == 'r' ? "remove" : "build");
		return E_WARN;
	}
	switch (order) {
	case 'b':
		if (pr[p1].type != dipent.pl[p] && pr[p1].type != 'X'
		    && dipent.variant != V_aberration
		    && dipent.variant != V_h31
		    && dipent.variant != V_h32
		    && dipent.variant != V_classical) {
			errmsg("%s is not a home province for %s.\n",
			       pr[p1].name, powers[p]);
			return E_WARN;
		}
		if (pr[p1].owner != p) {
			errmsg("%s does not control %s.\n",
			       powers[p], pr[p1].name);
			return E_WARN;
		}
		if (type == 'x') {
			errmsg("Unit type must be specified for build.\n");
			return E_WARN;
		}
		if (type == 'A')
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

		/*  FALL THROUGH  */
	case 'w':

		if ((u = pr[p1].unit)) {
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
			pr[unit[u].loc].unit = 0;

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
			pr[unit[u].loc].unit = 0;
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

	case 'm':
		break;

	case 'r':
		if (!(u = pr[p1].unit) || unit[u].owner != p) {
			errmsg("%s does not own a unit in %s%s to remove.\n",
			powers[p], water(p1) ? "the " : "", pr[p1].name);
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
		fprintf(rfp, "Invalid build order encountered (internal error).\n");
		err++;
		return E_WARN;
	}
	return 0;
}

void buildout(int pt)
{
	int i, u, p, c1;
	char mastrpt_pr[MAXPLAYERS];

	fprintf(rfp, "Adjustment orders for Winter of %d.  (%s.%s)\n\n",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	if (pt == MASTER) {
		for (u = 0; u < MAXPLAYERS; u++)
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

			if (unit[u].status == 'b') {

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
				fprintf(rfp, "Removes the %s in %s%s.\n",
					utype(unit[u].type),
					water(unit[u].loc) ? "the " : "",
					pr[unit[u].loc].name);

				unit[u].owner = 0;
				unit[u].status = ':';


			} else if (unit[u].status == 'w') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Build waived.\n");
				unit[u].owner = 0;

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
				    (pr[i].type == dipent.pl[p] || (pr[i].type == 'X' &&
				     ((dipent.variant == V_aberration) ||
				      (dipent.variant == V_h31) ||
				      (dipent.variant == V_h32) ||
				      (dipent.variant == V_classical)))))
					break;
			}
			if (i > npr || cnb[p]) {
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
