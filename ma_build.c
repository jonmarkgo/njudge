
/*
 * $Log$
 * Revision 1.7  2003/01/05 11:18:10  millis
 * Fixed bug 87
 *
 * Revision 1.6  2002/10/19 21:39:15  millis
 * Fixed Bug 21: Mach2 games allowing Armies in Venice
 *
 * Revision 1.5  2002/05/31 12:38:28  millis
 * Correct problem not allowing builds in Venice to change mind
 * Also allow first turn to have an over-spend (needed for scenarios)
 *
 * Revision 1.4  2002/02/25 11:51:51  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.2.2.1  2001/10/19 23:34:03  dema
 * Added missed include
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Unit type limits and predict
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

/*
 *  Process lines of the form:
 *
 *    cmd   ::= <power>: {<order>} <type> <province>
 *    order ::= build | remove | maintain
 *    type  ::= Army | Fleet | Garrison
 */

int ma_buildin(char **s, int p)
{
	char type, stype, order;
	unsigned char *t;
	int u, u1, p1, c1;

	*s = get_order(*s, &order);
	*s = get_stype(*s, &stype);
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
			p = pr[p1].home;
		}
	}
	if (order == 'x')
		order = 'm';

	switch (order) {
	case 'b':
		if (pr[p1].home != p) {
			errmsg("%s is not a home province for %s.\n",
			       pr[p1].name, powers[p]);
			return E_WARN;
		}
		if (!cityvalue(p1)) {
			errmsg("%s does not contain a city.\n", pr[p1].name);
			return E_WARN;
		}
		if (pr[p1].owner != p) {
			errmsg("%s does not control %s.\n", powers[p], pr[p1].name);
			return E_WARN;
		}
		if (is_infected(p1)) {
			errmsg("You cannot build in a famined province.\n");
			return E_WARN;
		}
		if (has_rebellion(p1)) {
			errmsg("You cannot build while a rebellion is in a province.\n");
			return E_WARN;
		}
		if (type == 'A') {
			c1 = MV;
			if (pr[p1].type & PF_VENICE && dipent.xflags & XF_MACH2) {
			        errmsg("Armies not allowed in %s.\n", pr[p1].name);
				return E_WARN;
			}

		} else if (type == 'F') {
			if (!has_port(p1)) {
				errmsg("Cannot build a fleet in %s which isn't a port.\n",
				       pr[p1].name);
				return E_WARN;
			}
			if (!c1)
				c1 = XC;
			for (t = (char *) pr[p1].move; *t; t++)
				if (*++t >> 4 == c1)
					break;
			if (!*t) {
				errmsg("Invalid coast specified for fleet in %s.\n", pr[p1].name);
				return E_WARN;
			}
		} else if (type == 'G') {
			if (!has_fortcity(p1)) {
				errmsg("Cannot build a garrison in %s which isn't fortified.\n",
				       pr[p1].name);
				return E_WARN;
			}
			c1 = MV;
		} else {
			errmsg("Unit type must be specified for build.\n");
			return E_WARN;
		}
		
		if ((u = pr[p1].unit)) {
			if (unit[u].status == 'b' || unit[u].status == 'x') {
				unit[u].status = 'x';
				unit[u].owner = 0;
			} else if (type != 'G') {
				errmsg("Cannot debuild and build in %s.\n", pr[p1].name);
				return E_WARN;
			}
		}
		if ((u = pr[p1].gunit)) {
			if (unit[u].status == 'b' || unit[u].status == 'x') {
				unit[u].status = 'x';
				unit[u].owner = 0;
			} else if (type == 'G') {
				errmsg("Cannot debuild and build in %s.\n", pr[p1].name);
				return E_WARN;
			}
		}

                /* If Venice, only allow one unit to exist at a time */
                if (is_venice(p1)) {
                        if ((pr[p1].unit && unit[pr[p1].unit].status != 'b' && unit[pr[p1].unit].status != 'x') ||
                            (pr[p1].gunit && unit[pr[p1].gunit].status != 'b' && unit[pr[p1].gunit].status != 'x' )) {
                            errmsg("Cannot order for more than one unit in %s.\n", pr[p1].name);
                            return E_WARN;
                        }
                }

		if (stype != 'x') {
			if (NO_SPECIAL_UNITS) {
				errmsg("Game %s does not allow special units.\n", dipent.name);
				return E_WARN;
			}
			for (u1 = 1; u1 <= nunit; u1++) {
				if (unit[u1].status != 'd' &&
				    unit[u1].owner == p && unit[u1].stype != 'x') {
					errmsg("You already have %s %s in %s.\n",
					       astype(unit[u1].stype, unit[u1].type),
					       utype(unit[u1].type),
					       pr[unit[u1].loc].name);
					errmsg("Only one special unit allowed per major power.\n");
					return E_WARN;
				}
			}
		}
                /* MLM 19/6/2001 OK, see if unit is allowed */
		if (!PermittedMachUnit(p,type,stype, PP_BUILD)) {
		    errmsg("Power is not allowed to build this unit type.\n");
		    return E_WARN;
		}
		
		for (u = 1; u <= nunit; u++) {
			if (unit[u].status == 'x') {
				if (is_garrison(u)) {
					pr[unit[u].loc].gunit = 0;
				} else {
					pr[unit[u].loc].unit = 0;
				}
				break;
			}
		}

		if (u > nunit)
			u = ++nunit;

		if (type == 'G')
			pr[p1].gunit = u;
		else
			pr[p1].unit = u;
		unit[u].owner = p;
		unit[u].type = type;
		unit[u].stype = stype;
		unit[u].loc = p1;
		unit[u].coast = c1;
		unit[u].status = 'b';
		break;


	case 'r':
		if ((!((u = pr[p1].unit) &&
		       type != 'G') &&
		     !((u = pr[p1].gunit) &&
		       (type == 'G' || type == 'x'))) ||
		    unit[u].owner != p) {
			errmsg("%s does not own a unit in %s%s to remove.\n",
			powers[p], water(p1) ? "the " : "", pr[p1].name);
			return E_WARN;
		}
		if (((is_garrison(u) && (u1 = pr[p1].unit)) ||
		     (is_garrison(u) && (u1 = pr[p1].gunit))) &&
		    unit[u1].status == 'b' && unit[u1].owner == p) {
			errmsg("You cannot build and debuild in %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (unit[u].status == 'b') {
			unit[u].status = 'x';
			unit[u].owner = 0;
		} else {
			unit[u].status = 'd';
		}
		break;

	case 'm':
		if ((!((u = pr[p1].unit) &&
		       type != 'G') &&
		!((u = pr[p1].gunit) && (type == 'G' || type == 'x'))) ||
		    unit[u].owner != p) {
			errmsg("%s does not own a unit in %s%s to maintain.\n",
			powers[p], water(p1) ? "the " : "", pr[p1].name);
			return E_WARN;
		}
		if (unit[u].status == 'd' && unit[u].stype != 'x') {
			for (u1 = 1; u1 <= nunit; u1++) {
				if (unit[u1].owner == p &&
				    unit[u1].status != 'd' &&
				    unit[u1].stype != 'x') {
					errmsg("You already have %s %s in %s.\n",
					       astype(unit[u1].stype, unit[u1].type),
					       utype(unit[u1].type),
					       pr[unit[u1].loc].name);
					errmsg("Only one special unit allowed per major power.\n");
					return E_WARN;
				}
			}
		}
		unit[u].status = 'm';
		break;

	case 'w':
		errmsg("Waive ignored for Machiavelli variant.\n");
		break;

	default:
		fprintf(rfp, "Invalid build order encountered (internal error).\n");
		err++;
		return E_WARN;
	}
	return 0;
}

void ma_buildout(int pt)
{
	int n, u, p, i, c1;
	char mastrpt_pr[MAXPLAYERS];

	if (err)
		fputc('\n', rfp);
	fprintf(rfp, "Adjustment orders for Winter of %d.  (%s.%s)\n",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	n = (int) strlen(powers[AUTONOMOUS]);

	fprintf(rfp, "\n");
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
	for (p = 0, u = 1; u <= nunit; u++) {
		if (processing || pt == unit[u].owner || pt == MASTER) {

			if (p != unit[u].owner) {
				fputc('\n', rfp);
				p = unit[u].owner;
			}
			if (p == AUTONOMOUS) {
				unit[u].status = 'm';
			}
			if (unit[u].status == 'b' || unit[u].status == 'm') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < n; i++)
					putc(' ', rfp);

				if (unit[u].status == 'b') {
					fprintf(rfp, "Builds %s %s in %s", astype(unit[u].stype, unit[u].type),
						utype(unit[u].type),
						pr[unit[u].loc].name);
				} else {
					fprintf(rfp, "Maintains the %s%s in %s", bstype(unit[u].stype),
						utype(unit[u].type),
						pr[unit[u].loc].name);
				}

				if ((c1 = unit[u].coast) > XC)
					fprintf(rfp, " (%s)", mtype[c1]);

				c1 = unit[u].stype == 'x' ? 3 : unit[u].stype == 'p' ? 9 : 6;
				if (ducats[p].treasury >= c1) {
					fprintf(rfp, "  (%d ducats)\n", c1);
					ducats[p].treasury -= c1;
				} else if (unit[u].owner == AUTONOMOUS) {
					fprintf(rfp, "  (free)\n");
				} else if (atoi(dipent.seq) <= 2) {
				       /* First turn, allow overspend */
					fprintf(rfp, "  (%d ducats)\n", c1);
                                        ducats[p].treasury = 0;  
				} else {
					fprintf(rfp, "  (%d ducats *** NSF)\n", c1);
					unit[u].owner = 0;
					if (!processing && pt != MASTER) {
						more_orders++;
						err++;
					}
				}
				unit[u].status = ':';

			} else if (unit[u].status == 'd') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < n; i++)
					putc(' ', rfp);
				fprintf(rfp, "Removes the %s in %s%s",
					utype(unit[u].type),
					water(unit[u].loc) ? "the " : "",
					pr[unit[u].loc].name);

				if ((processing || predict) && is_sieged(unit[u].loc)) {
					remove_siege(unit[u].loc);
					fprintf(rfp, " (siege lifted)\n");
				} else {
					fprintf(rfp, ".\n");
				}

				unit[u].owner = 0;
				unit[u].status = ':';

			} else if (unit[u].status == ':') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < n; i++)
					putc(' ', rfp);
				fprintf(rfp, "%s %s", Utype(unit[u].type), pr[unit[u].loc].name);
				if (!processing && !predict ) {
					fprintf(rfp, " *** No order received, maintain or remove");
					if (pt != MASTER) {
					    more_orders++;
					    err++;
					}
				} else {
					fprintf(rfp, " *** No order received, removed");
				}

				fprintf(rfp, ".\n");

				unit[u].owner = 0;

			}
		}
	}
}
