/*
 * $Log$
 * Revision 1.7  2003-07-23 23:13:39  millis
 * Fix Bug 195, do not block retreat into straights if a fleet is retreating to the controlling province.
 *
 * Revision 1.6  2003/05/10 00:46:15  millis
 * Bug 140 fix, display 'orders' when orders and 'results' when results
 *
 * Revision 1.5  2002/12/28 00:09:29  millis
 * Fixed bug 79, not showing special type on retreats
 *
 * Revision 1.4  2002/02/25 11:51:52  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.2.2.1  2001/10/19 23:07:42  dema
 * Added missing header file include
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Various
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdlib.h>
#include <string.h>
#include "dip.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

/*  Process lines of the form:
 *    cmd ::= <power>: <move list>
 *    move list ::= <move>{; <move list>}
 *    move ::= <type> <province> - <province>
 *           | <type> <province> disband
 *    type ::= Army | Fleet
 */

int ma_retreatin(char **s, int p)
{
	char order, c;
	int u;
	int c1, c2, p1, p2, bl;
	unsigned char *b;

	/* 
	   * Get the unit type
	 */
	*s = get_type(*s, &c);
	if (c == 'x') {
		errmsg("Illegal unit type %s specified", *s);
		return E_WARN;
	}
	/* 
	   ** Get the province
	 */
	*s = get_prov(*s, &p1, &c1);
	if (!p1) {
		errmsg("Unrecognized source province -> %s", *s);
		return E_WARN;
	}
	/*
	   ** Test if there is a unit of this type at this location from
	   ** this player or if the MASTER is ordering the retreat.
	 */
	for (u = 1; u <= nunit; u++)
		if (unit[u].loc == p1 &&
		    unit[u].type == c &&
		    (unit[u].owner == p ||
		     (unit[u].status == 'r' && p == MASTER)))
			break;

	if (u > nunit) {
		errmsg("%s doesn't have %s %s %s.\n", powers[p], autype(c),
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	/* 
	   ** This one is now covered above 
	   ** The bug was that when two units were on the same location,
	   ** for example a garrison and an army, and the army had to retreat,
	   ** but the garrison was first in the list, the following error message
	   ** was generated 
	 */
	/*
	   if (c != 'x' && c != unit[u].type) {
	   errmsg("The unit %s %s is %s, not %s.\n",
	   water(p1) ? "in the"  : "in", pr[p1].name,
	   autype(unit[u].type), autype(c));
	   return E_WARN;
	   }
	 */

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

	case 'v':
		*s = get_type(*s, &c);
		if (c == 'x')
			c = 'G';
		if (c != 'G') {
			errmsg("Retreat phase conversions can only be to Garrisons.\n");
			return E_WARN;
		}
		for (b = unit[u].convoy; b && *b && *b != p1; b++);
		if (!*b) {
			errmsg("The %s %s %s can't convert to a Garrison.\n",
			       utype(unit[u].type),
			       water(p1) ? "in the" : "in", pr[p1].name);
			return E_WARN;
		}
		p2 = p1;
		c2 = MV;
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

void ma_retreatout(int pt)
{
	int u, u2, p,p1, i;
	char mastrpt_pr[MASTER+1];

	int had_rebellion[NPROV+1];  /* Remember if a province had a rebellion */
	

	for (p = 1; p <= npr; p++) {
	     had_rebellion[p] = 0;
	}

	/*  Generate report  */

	if (err)
		fprintf(rfp, "\n");
	fprintf(rfp, "Retreat %s for %s of %d.  (%s.%s)\n\n",
		 pt ? "orders" : "results",
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);


	/*  Pass one, check for conflicts. */

	if (processing || predict) {
		for (u = 1; u <= nunit; u++) {
			if (unit[u].status == 'r') {
				if (unit[u].order == 'm') {
					if (unit[u].bloc && (u2 = pr[unit[u].bloc].unit) &&
					    unit[u2].type == 'F' && !allies[unit[u2].owner][unit[u].owner] && unit[u2].status != 'r') {
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
		for (u = 0; u <= MASTER; u++)
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
			/* Bug 79, show type of special unit */
			fprintf(rfp, "%s%s %s", Stype(unit[u].stype),
			Utype(unit[u].type), pr[unit[u].loc].name);

			if (unit[u].coast > XC)
				fprintf(rfp, " (%s)", mtype[unit[u].coast]);

			if (unit[u].order == 'm') {
				fprintf(rfp, " -> %s", pr[unit[u].dest].name);
				if (unit[u].dcoast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);

				if (unit[u].status == 'd') {
					fprintf(rfp, " (*bounce, destroyed*)");
					unit[u].owner = 0;

				} else if (unit[u].status == 'b') {
					fprintf(rfp, " (*blocked, destroyed*)");
					unit[u].owner = 0;

				} else {
					unit[u].loc = unit[u].dest;
					unit[u].coast = unit[u].dcoast;
					/* Bug 489, clear rebellions if retreating in */
					p1 = unit[u].loc;
				         if (has_rebellion(p1) &&
				             pr[p1].owner != unit[u].owner) {
					     had_rebellion[p1] = 1;
			                }
				}

			} else if (unit[u].order == 'd') {
				fprintf(rfp, " DISBAND");
				unit[u].owner = 0;

			} else if (unit[u].order == 'v') {
				fprintf(rfp, " CONVERT TO Garrison");
				unit[u].type = 'G';
				/* MachMLM 28/4/01 Update ownership */
				pr[unit[u].loc].gunit = u;  

			} else {
				more_orders++;
				fprintf(rfp, ", No Order Processed (DISBAND)");
				unit[u].owner = 0;
			}
			fprintf(rfp, ".\n");

			unit[u].status = ':';

		}
	}

	/* Bug 489, now print where we do have rebellions quashed */
	 i = 0;
	 for (p = 1; p <= npr && (processing || predict); p++) {
	    if (had_rebellion[p]) {
	        if (!i++)
	           fprintf(rfp, "\n");
	        fprintf(rfp, "Rebellion in %s liberated.\n", pr[p].name);
		remove_rebellion(p);
	    }
	}

	    
}
