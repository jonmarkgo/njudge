/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  st_retreat.c
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

int retreatin(char **s, int pt)
{

/*  Read retreat orders in from input file.  */

	char c, order;
	unsigned char *b;
	int p1, p2, u, c1, c2, bl;

/*  Process lines of the form:

 *    cmd ::= <power>: <move list>
 *    move list ::= <move>{; <move list>}
 *    move ::= <type> <province> - <province>
 *           | <type> <province> disband
 *    type ::= Army | Fleet
 */

	*s = get_type(*s, &c);
	*s = get_prov(*s, &p1, &c1);
	if (!p1) {
		errmsg("Unrecognized source province -> %s", *s);
		return E_WARN;
	}
	for (u = 1; u <= nunit; u++)
		if (unit[u].loc == p1 && (unit[u].owner == pt ||
				(unit[u].status == 'r' && pt == MASTER)))
			break;

	if (u > nunit) {
		errmsg("%s doesn't have a unit %s %s.\n", powers[pt],
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	if (c != 'x' && c != unit[u].type) {
		errmsg("The unit %s %s is %s, not %s.\n",
		       water(p1) ? "in the" : "in", pr[p1].name,
		       autype(unit[u].type), autype(c));
		return E_WARN;
	}
	if (unit[u].status != 'r') {
		errmsg("The %s %s %s was not dislodged.\n",
		       utype(unit[u].type),
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	*s = get_action(*s, &order);
	switch (order) {
	case 'm':
		*s = get_prov(*s, &p2, &c2);
		if (!p2) {
			errmsg("Movement from %s%s to unrecognized province -> %s",
			       water(p1) ? "the " : "", pr[p1].name, *s);
			return E_WARN;
		}
		if (!valid_move(u, p2, &c2, &bl)) {
			errmsg("The %s %s %s can't get to %s%s.\n",
			       utype(unit[u].type),
			       water(p1) ? "in the" : "in", pr[p1].name,
			       water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		for (b = unit[u].convoy; b && *b && *b != p2; b++);
		if (!*b) {
			errmsg("The %s %s %s can't retreat to %s%s.\n",
			       utype(unit[u].type),
			       water(p1) ? "in the" : "in", pr[p1].name,
			       water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		break;

	case 'd':
		break;


	default:
		errmsg("Invalid order for the %s %s %s.\n", utype(unit[u].type),
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}

	unit[u].order = order;
	unit[u].dest = p2;
	unit[u].dcoast = c2;
	unit[u].bloc = bl;

	return 0;

}

void retreatout(int pt)
{

	int p, i, u, u2;
	char mastrpt_pr[MAXPLAYERS];

/*  Generate report  */

	if (err)
		fprintf(rfp, "\n");
	fprintf(rfp, "Retreat orders for %s of %d.  (%s.%s)\n\n",
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);


/*  Pass one, check for conflicts. */

	if (processing) {
		for (u = 1; u <= nunit; u++) {
			if (unit[u].status == 'r') {
				if (unit[u].order == 'm') {
					if (unit[u].bloc && (u2 = pr[unit[u].bloc].unit) &&
					    unit[u2].type == 'F') {
						unit[u].status = 'b';
					} else if ((u2 = pr[unit[u].dest].unit)) {
						if (unit[u2].status == 'r')
							unit[u2].status = 'd';
						unit[u].status = 'd';
					} else {
						pr[unit[u].dest].unit = u;
					}
				}
			}
		}
	}
/*  Pass two, display results and move the units.  */

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
		if (unit[u].status != ':' &&
		    (pt == (p = unit[u].owner) || pt == MASTER || processing)) {
			fprintf(rfp, "%s: ", powers[p]);
			for (i = strlen(powers[p]); i < LPOWER; i++)
				putc(' ', rfp);
			fprintf(rfp, "%-5s %s", Utype(unit[u].type), pr[unit[u].loc].name);

			if (unit[u].coast > XC)
				fprintf(rfp, " (%s)", mtype[unit[u].coast]);

			if (unit[u].order == 'm') {
				fprintf(rfp, " -> %s", pr[unit[u].dest].name);
				if (unit[u].dcoast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);

				if (unit[u].status == 'd') {
					fprintf(rfp, " (*bounce, destroyed*)");
					unit[u].owner = 0;

				} else if (unit[u].status == 'b') {	/* Originates from Machiavelli */
					fprintf(rfp, " (*blocked, destroyed*)");
					unit[u].owner = 0;

				} else {
					unit[u].loc = unit[u].dest;
					unit[u].coast = unit[u].dcoast;
				}

			} else if (unit[u].order == 'd') {
				fprintf(rfp, " DISBAND");
				unit[u].owner = 0;

			} else {
				more_orders++;
				fprintf(rfp, ", No Order Processed (DISBAND)");
				unit[u].owner = 0;
			}
			fprintf(rfp, ".\n");

			unit[u].status = ':';

		}
	}

}

/****************************************************************************/
