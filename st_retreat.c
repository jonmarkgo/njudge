/*
   ** $Log$
   ** Revision 1.3  2000/11/14 14:27:37  miller
   ** Allow wing units
   ** REcalcultate blockade settings
   ** Allow retreat moves syntax checking
   **
   ** Revision 1.2  1999/07/13 19:55:05  davidn
   ** Size of array increased as array is indexed by owner number, not index of
   ** player in master file entry.
   ** Was causing core dump on USTR July 1999
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
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
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"


int retreat_syntaxcheck( char *in_text, int precheck, char *out_string)
{
/*  Read retreat orders in from input file.  */

        char c, order;
        int p1, p2, c1, c2;
        char *s;
	char *out_text = NULL;
	char temp_out[256];
	temp_out[0]='\0';
	s = in_text;
        if (out_string != NULL ) out_text = temp_out;

/*  Process lines of the form:

 *    cmd ::= <power>: <move list>
 *    move list ::= <move>{; <move list>}
 *    move ::= <type> <province> - <province>
 *           | <type> <province> disband
 *    type ::= Army | Fleet
 */

        /* 
         * See if a precheck is possible
         * If so, see if it is an canprocess line and assume ok if so
         */

        if (precheck)
        {
                if (canpreprocess(s))  return 0;
        }

        s = get_type(s, &c);
        s = get_prov(s, &p1, &c1);
        AddUnitProvinceToOrder(out_text, c, p1);
	if (!p1) {
                errmsg("Unrecognized source province -> %s", *s);
                return E_WARN;
        }
        s = get_action(s, &order);
        AddOrderToOrder(out_text, order);
	switch (order) {
        case 'm':
                s = get_prov(s, &p2, &c2);
		AddProvinceToOrder(out_text, p2);
                if (!p2) {
                        errmsg("Movement from %s%s to unrecognized province -> %s",
                               water(p1) ? "the " : "", pr[p1].name, *s);
                        return E_WARN;
                }
        case 'd':
                break;


        default:
                errmsg("Invalid order for the unit in %s.\n",
                       pr[p1].name);
                return E_WARN;
        }

	*s = '\0';  /* end of string */
	if (out_text != NULL) strcat(out_string, out_text);
	return 0;
}

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
		       mov_type(p1, u), pr[p1].name,
		       autype(unit[u].type), autype(c));
		return E_WARN;
	}
	if (unit[u].status != 'r') {
		errmsg("The %s %s %s was not dislodged.\n",
		       utype(unit[u].type),
		       mov_type(p1, u), pr[p1].name);
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
			       mov_type(p1, u), pr[p1].name,
			       water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		for (b = unit[u].convoy; b && *b && *b != p2; b++);
		if (!*b) {
			errmsg("The %s %s %s can't retreat to %s%s.\n",
			       utype(unit[u].type),
			       mov_type(p1, u), pr[p1].name,
			       water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		break;

	case 'd':
		break;


	default:
		errmsg("Invalid order for the %s %s %s.\n", utype(unit[u].type),
		       mov_type(p1,u), pr[p1].name);
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
	char mastrpt_pr[ NPOWER + 1 ];  // Used to be [MAXPLAYERS]. DAN 13/07/99

/*  Generate report  */

	if (err)
		fprintf(rfp, "\n");
	fprintf(rfp, "Retreat orders for %s of %d.  (%s.%s)\n\n",
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);


/*  Pass one, check for conflicts. */

	if (processing || predict) {
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
		for (u = 0; u <  NPOWER + 1; u++)
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

/* Pass 3: recalculate blockade settings */
        if ((dipent.flags & F_WINGS) && (processing || predict)) {
                /* Firstly, set all provinces to 'unblockaded' */
                for (i = 1; i <= npr;i++) {
                        pr[i].blockaded = 0;
                }
                /* Now, go through all units and set to blockaded if occupied by an
                   enemy wing
                 */
                 for (u = 1; u <= nunit; u++) {
                        if (unit[u].status == ':' && unit[u].owner != 0 ) {
                                /* only non-retreating units */
                                if (unit[u].type == 'W' && unit[u].owner != pr[unit[u].loc].owner) {
                                        /* we can mark non-scs as blockaded, but who cares! */
                                        pr[unit[u].loc].blockaded = 1;
                                }
                        }
                }
        }


}

/****************************************************************************/
