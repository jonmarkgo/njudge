/*
** $Log$
** Revision 1.12  2001/10/20 12:11:15  miller
** Merged in changes from DEMA and USTV 
**
** Revision 1.11.2.2  2001/10/20 00:52:49  dedo
** Remvoe compile warnings
**
** Revision 1.11.2.1  2001/10/15 22:28:33  ustv
** Added handling for province duality
**
** Revision 1.11  2001/07/08 23:01:38  miller
** Space fixup and DISBAND flag and predict usage
**
** Revision 1.10  2001/07/01 23:19:29  miller
** Limit tables
**
** Revision 1.9  2001/06/05 00:52:39  miller
** Fix proxy-unit order bug
**
** Revision 1.8  2001/05/14 23:08:49  miller
** Remove DOS ^M
**
** Revision 1.7  2001/05/09 05:59:50  greg
** minor bug fix
**
** Revision 1.6  2001/04/18 12:33:53  miller
** Fixed blind flag problem
**
** Revision 1.5 2001/04/15 21:21:22 miller
** Added handling of XF_NOTRANSATTACK flag
**
** Revision 1.3 2001/02/03 10:40:00 miller
** Fix blind bug
** Now no show of others results
**
** Revision 1.2 2000/11/14 14:27:37 miller
** Added move syntax checker
** Allow wing units to be used
** Allow transformations
** REcalculate blockade settings
**
** Revision 1.1 1998/02/28 17:49:42 david
** Initial revision
**
** Revision 1.2 1996/11/18 18:39:44 rpaar
** Applied K.Lowe's convoy bug patch.
**
** Revision 1.1 1996/10/20 12:29:45 rpaar
** Morrolan v9.0
**
*/
/* po_movein.c
**
** Copyright 1987, Lowe.
**
** Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
** Maryland, all rights reserved; used with permission.
**
** Redistribution and use in source and binary forms are permitted
** provided that it is for non-profit purposes, that this and the
** above notices are preserved and that due credit is given to Mr.
** Lowe.
**
*/
/*
* 03 Dec 1999 Millis Miller Added handling for wing blockading 
*/
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "dip.h"
#include "porder.h"

int convoyable(int p) 
{ 
    return (water(p) | \
	              (dipent.xflags & XF_COASTAL_CONVOYS) | \
	      ((dipent.xflags & XF_PROV_DUALITY) && (pr[p].type2 == 'w')));
}


extern int one_owned[];
int StrictConvoy(int p_index)
{
	if (!(dipent.xflags & XF_STRCONVOY ) ) return 1; /* allow when no flag */
	
	return 0; /* Invalid so disallow */
}
 
/*
* General purpose move checker routine.
* Will see if a move is syntactically correct only
* but will not check ownership etc.
*/
int move_syntaxcheck( char *in_line, int precheck, char *out_string )
{
/* Read movement in from input file. */
	char c, order, *t;
	int i, p1, p2, c1, c2;
	char temp[256], temp_text[256],*s;
	char temp_out[1024]; /* to store correctly parsed order */
	char *out_text = NULL;
	temp_out[0]='\0';
	strcpy(temp, in_line);
	s = temp;
	if (out_string != NULL ) out_text = temp_out;
 
	/* Process lines of the form:
	**
	** cmd ::= <power>: <move list>
	** move list ::= <move>{; <move list>}
	** move ::= <type> <province> - <province>
	** | <type> <province> holds
	** | <type> <province> support {<power>:} <type> <province> {-<province>}
	** | <type> <province> convoy {<power>:} <type> <province> {-<province>}
	** | <type> <province> transform <type>
	** type ::= Army | Fleet | Wing
	*/
	/* 
	* See if a precheck is possible
	* If so, see if it is an canprocess line and assume ok if so
	*/
	if (precheck)
	{
	    if (canpreprocess(s)) return 0;
	}
 
	/*
	 ** Process regular orders.
	 */
	s = get_type(s, &c);
	s = get_prov(s, &p1, &c1);
	AddUnitProvinceToOrder(out_text, c, p1);
	if (!p1) {
		errmsg("Unrecognized source province -> %s", s);
		return E_WARN;
	}
	s = get_action(s, &order);
	AddOrderToOrder(out_text, order);
	switch (order) {
	case 'c':
	case 's':
		s = get_type(s, &c);
		s = get_prov(s, &p2, &c2);
		AddUnitProvinceToOrder(out_text, c, p2);
		if (!p2) {
			errmsg("Unrecognized source province for support/convoy -> %s", s);
			return E_WARN;
		}
		if (order == 'c' && !StrictConvoy(p2))
		{
			errmsg("The unit %s %s is landlocked and cannot be convoyed.",
				water(p2) ? "in the" : "in", pr[p2].name );
			return E_WARN;
		}
		t = get_action(s, &c);
		AddOrderToOrder(out_text,c);
		if (c == 'm') {
			s = get_prov(t, &p2, &c2);
			AddPlaceToOrder(out_text, p2);
			if (!p2) {
				errmsg("Support/convoy movement to unrecognized province -> %s", s);
				return E_WARN;
			}
		}
		if (order == 'c' && water(p2)) {
			errmsg("The convoy order should specify source %s\n",
			 "and final destination of an army.");
			return E_WARN;
		}
		if (order == 'c' && !StrictConvoy(p2)) {
		    errmsg("The destination %s is landlocked and cannot be convoyed to.",
		    pr[p2].name );
		    return E_WARN;
		}
		c1 = 0;
		break;
 
	case 'd':
		if (!(dipent.xflags & XF_MOVEDISBAND)) {
		    errmsg("This game does not allow disband orders in movement phases.");
		    return E_WARN;
		}
		break;

	case 'h':
	case 'n':
		break;
	case 'm':
		s = get_prov(s, &p2, &c2);
		AddPlaceToOrder(out_text, p2);
		if (!p2) {
			errmsg("Movement from %s%s to unrecognized province -> %s",
			 water(p1) ? "the " : "", pr[p1].name, s);
			return E_WARN;
		}
		t = get_action(s, &c);
		if (c == 'm') {
			while (c == 'm') {
				AddOrderToOrder(out_text, c);
				s = get_prov(t, &p2, &c2);
				AddPlaceToOrder(out_text,p2);
				if (!p2) {
					errmsg("Movement from %s%s to unrecognized province -> %s",
					 water(p1) ? "the " : "", pr[p1].name, s);
					return E_WARN;
				}
				t = get_action(s, &c);
				/* Check that not disallowed for Army/Fleet rules */
				if (dipent.flags & F_AFRULES && c == 'm') {
				    errmsg("Multi-hop convoys are not allowed with A/F rules.");
				    return E_WARN;
				}
			 c2 = MV;
			}
		}
		if (c != 'x' ) { 
			errmsg("Invalid order syntax for the unit in %s.\n",
			 pr[p1].name);
				return E_WARN;
	 }
		break;
 
	case 'p':		/* proxy */
		s = get_power(s, &i);
		AddPowerToOrder(out_text, i);
		if (i == 0 || i >= WILD_PLAYER) {
			errmsg("Valid power must be specified for proxy order.\n");
			return E_WARN;
		}
		if (!(dipent.flags & F_PROXY)) {
			errmsg("Game %s does not allow proxy orders.\n", dipent.name);
			return E_WARN;
		}
		break;
	case 't':
		if (!(dipent.xflags & XF_TRANS_MOVE )) {
		    errmsg ("Game '%s' does not permit move transformations.\n",
		    dipent.name);
		    return E_WARN;
		}
		if (water(p1)) {
		    errmsg("Only units on/over land can be transformed");
		    return E_WARN;
		}
 
		s = get_type(s, &c);
		AddUnitToOrder(out_text, c);
		if (c == 'x') {
		    errmsg("Uknown unit type to transform to.\n");
		    return E_WARN;
                }
		if (c== 'W' && !(dipent.flags & F_WINGS)) {
		 errmsg("This game does not allow wings.\n");
			return E_WARN;
		}
		if (c == 'F') {
		    sprintf(temp_text,"%s %s", pr[p1].name, s);
		    get_prov(temp_text, &p2, &c1);
		    if (!c1)
		        c1 = XC;
		    for (t = (char *) pr[p1].move; *t; t++)
		    if (*++t >> 4 == c1)
		        break;
		    if (!*t) {
		        errmsg("Invalid coast specified for a fleet in %s.\n",
		        pr[p1].name);
		        return E_WARN;
		    }
			if (c1 != XC) AddCoastToOrder(out_text, c1);
                }
		break;
 
	default:
		errmsg("Invalid order for the unit at %s.\n",
		 pr[p1].name);
		return E_WARN;
	}
	*s = '\0'; /* terminate string */
	/* See if processed text string is wanted */
	if (out_text != NULL ) strcpy (out_string, out_text);
	return 0;
}
/****************************************************************************/
int movein(char **s, int p)
{
/* Read movement in from input file. */
	char c, order, *t;
	int i, j, p1, p2, p3, u, u1, u2, c1, c2, c3, bl;
	unsigned char *bp;
	char temp_text[256];
 
/* Process lines of the form:
**
** cmd ::= <power>: <move list>
** move list ::= <move>{; <move list>}
** move ::= <type> <province> - <province>
** | <type> <province> holds
** | <type> <province> support {<power>:} <type> <province> {-<province>}
** | <type> <province> convoy {<power>:} <type> <province> {-<province>}
** | <type> <province> transform <type>
** type ::= Army | Fleet | Wing
*/
	/*
	 ** Process regular orders.
	 */
	*s = get_type(*s, &c);
	*s = get_prov(*s, &p1, &c1);
	if (!p1) {
		errmsg("Unrecognized source province -> %s", *s);
		return E_WARN;
	}
	u1 = pr[p1].unit;
	u = u1;
	/* p3 and c3 initialisation */
	p3 = p1;
	c3 = c1;
	if (!u) {
		errmsg("No %s present %s %s.\n", utype(c),
		 water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	if (p != unit[u].owner && p != MASTER) {
		if (!(dipent.flags & (F_PROXY))) {
			errmsg("%s doesn't own the %s %s %s.\n", powers[p], utype(c),
			 mov_type(p1, u), pr[p1].name);
			errmsg("Game %s does not allow proxy orders.\n", dipent.name);
			return E_WARN;
		} else {
			for (u2 = 1; u2 <= nunit; u2++) {
				if (unit[u2].owner == p && unit[u2].proxy == u)
					break;
			}
			if (u2 > nunit) {
				if (nunit > MAXUNIT - 2) {
					fprintf(rfp, "Too many units in %s.\n", dipent.name);
					fprintf(log_fp, "Too many units in %s.\n", dipent.name);
					bailout(1);
				}
				++nunit;
			}
			memcpy(&unit[u2], &unit[u], sizeof(unit[0]));
			unit[u2].owner = p;
			unit[u2].proxy = u;
			unit[u2].order = 'n';
			u = u2;
		}
	}
	if (c != 'x' && c != unit[u].type) {
		errmsg("The unit %s %s is %s, not %s.\n",
		 mov_type(p1,u), pr[p1].name,
		 autype(unit[u].type), autype(c));
		return E_WARN;
	}
	p2 = unit[u].loc;
	c2 = unit[u].coast;
	u2 = 0;
	bp = NULL;
	bl = 0;
	*s = get_action(*s, &order);
	switch (order) {
	case 'c':
	case 's':
		*s = get_type(*s, &c);
		*s = get_prov(*s, &p3, &c3);
		p2 = p3;
		c2 = c3;
		if (!p2) {
			errmsg("Unrecognized source province for support/convoy -> %s", *s);
			return E_WARN;
		}
		u2 = pr[p2].unit;
	 if (!(dipent.flags & F_BLIND)) {
		 if (!(u2)) {
			errmsg("No unit present %s %s.\n",
			 water(p2) ? "in the" : "in", pr[p2].name);
			return E_WARN;
		 }
		 if (order == 'c' && unit[u2].type != 'A') {
			errmsg("The convoy order should specify source %s\n",
			 "and final destination of an army.");
			return E_WARN;
		 }
		 if ((c != 'x' && c != unit[u2].type)) {
			errmsg("The unit %s %s is %s, not %s.\n",
			 mov_type(p2,u2), pr[p2].name,
			 autype(unit[u2].type), autype(c));
			return E_WARN;
		 }
		}
		if (order == 'c' && !StrictConvoy(p2))
		{
			errmsg("The unit %s %s is landlocked and cannot be convoyed.",
				water(p2) ? "in the" : "in", pr[p2].name );
			return E_WARN;
		}
		t = get_action(*s, &c);
		if (c == 'm') {
			*s = get_prov(t, &p2, &c2);
			if (!p2) {
				errmsg("Support/convoy movement to unrecognized province -> %s", *s);
				return E_WARN;
			}
		}
		if (order == 'c' && unit[u].type != 'F') {
			errmsg("The %s in %s can't convoy anything!!\n",
			 utype(unit[u].type), pr[p1].name);
			return E_WARN;
		}
		if (order == 'c' && water(p2)) {
			errmsg("The convoy order should specify source %s\n",
			 "and final destination of an army.");
			return E_WARN;
		}
		if (order == 'c' && !StrictConvoy(p2)) {
                    errmsg("The destination %s is landlocked and cannot be convoyed to.",
                    pr[p2].name );
                    return E_WARN;
                }
		c1 = 0;
		if (order == 's' &&
		 (!valid_move(u, p2, &c1, &bl) || c1 == MX)) {
			errmsg("The %s %s %s can't get to %s%s to support.\n",
			 utype(unit[u].type),
			 mov_type(p1, u), pr[p1].name,
			 water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p1].type == 'v' && unit[u].type != 'W') {
			errmsg("Invalid order for fall in the %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p2].type == 'v' && unit[u].type != 'W') {
			errmsg("Invalid order for fall in the %s.\n", pr[p2].name);
			return E_WARN;
		}
		if (!c2 && unit[u2].type == 'F')
			valid_move(u2, p2, &c2, &i);	/* set c2 */
		break;

        case 'd':
                if (!(dipent.xflags & XF_MOVEDISBAND)) {
                    errmsg("This game does not allow disband orders in movement phases.");
                    return E_WARN;
                }
                break;

	case 'h':
	case 'n':
		break;
	case 'm':
		*s = get_prov(*s, &p2, &c2);
		if (!p2) {
			errmsg("Movement from %s%s to unrecognized province -> %s",
			 water(p1) ? "the " : "", pr[p1].name, *s);
			return E_WARN;
		}
		t = get_action(*s, &c);
		if (c == 'm') {
			if (unit[u].type == 'A') {
				i = nunit + 1;
				unit[i].loc = unit[u].loc;
				unit[i].coast = XC;
				bp = &heap[hp];
				while (c == 'm') {
					if (!valid_move(i, p2, &c2, &j) || !convoyable(p2) ||
					 (!(u2 = pr[p2].unit) || unit[u2].type != 'F')) {
						errmsg("The army in %s can't convoy through %s%s.\n",
						 pr[p1].name, water(p2) ? "the " : "", pr[p2].name);
						return E_WARN;
					}
					if (j && !bl)
						bl = j;
					heap[hp++] = pr[p2].unit;
					unit[i].loc = p2;
					*s = get_prov(t, &p2, &c2);
					if (!p2) {
						errmsg("Movement from %s%s to unrecognized province -> %s",
						 water(p1) ? "the " : "", pr[p1].name, *s);
						return E_WARN;
					}
					t = get_action(*s, &c);
					/* Check that not disallowed for Army/Fleet rules */
					if (dipent.flags & F_AFRULES && c == 'm') {
                                            errmsg("Multi-hop convoys are not allowed with A/F rules.");
                                            return E_WARN;
                                        }
				}
				if (!valid_move(i, p2, &c2, &j)) {
					errmsg("The army in %s can't convoy from %s to %s.\n",
					 pr[p1].name, pr[unit[i].loc].name, pr[p2].name);
					return E_WARN;
				}
				if (water(p2)) {
					errmsg("The army in %s can't convoy into the %s.\n",
					 pr[p1].name, pr[p2].name);
					return E_WARN;
				}
				heap[hp++] = 0;
				c2 = MV;
			} else {
				errmsg("Invalid order syntax for the unit %s %s.\n",
				 mov_type(p1, u), pr[p1].name);
				return E_WARN;
			}
		} else {
			if (!valid_move(u, p2, &c2, &bl)) {
				errmsg("The %s %s %s can't get to %s%s.\n", utype(unit[u].type),
				mov_type(p1,u), pr[p1].name,
				 water(p2) ? "the " : "", pr[p2].name);
				return E_WARN;
			}
		}
		if (dipent.phase[0] == 'F' && pr[p1].type == 'v' && unit[u].type != 'W') {
			errmsg("Invalid order for fall in the %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p2].type == 'v' && unit[u].type != 'W') {
			errmsg("Invalid order for fall in the %s.\n", pr[p2].name);
			return E_WARN;
		}
		break;
 
	case 'p':		/* proxy */
		*s = get_power(*s, &i);
		if (i == 0 || i >= WILD_PLAYER) {
			errmsg("Valid power must be specified for proxy order.\n");
			return E_WARN;
		}
		if (!(dipent.flags & F_PROXY)) {
			errmsg("Game %s does not allow proxy orders.\n", dipent.name);
			return E_WARN;
		}
		u2 = i;
		break;
	case 't':
		if (!(dipent.xflags & XF_TRANS_MOVE )) {
                    errmsg ("Game '%s' does not permit move transformations.\n",
                    dipent.name);
                    return E_WARN;
                }
		if (water(p1)) {
		 errmsg("Cannot transform %s while %s water.\n",
				utype(unit[u].type),
				unit[u].type == 'F' ? "in" : "over");
		 return E_WARN;
		}
                *s = get_type(*s, &c);
                if (c == 'x') {
                    errmsg("Uknown unit type to transform to.\n");
                    return E_WARN;
                }
                if (unit[u].type == c && c != 'F') {
                    errmsg("Error: Unit in %s is already of type %s.\n",
                    pr[p1].name, Utype(unit[u].type));
                    return E_WARN;
                }
                /* if a fleet, check if this is a coast */
                /* If a fleet, check if coast needs to be specified */
                if (c == 'F') {
                    sprintf(temp_text,"%s %s", pr[p1].name, *s);
                    get_prov(temp_text, &p2, &c1);
                    if (!c1)
                        c1 = XC;
                    for (t = (char *) pr[p1].move; *t; t++)
                    if (*++t >> 4 == c1)
                        break;
                    if (!*t) {
                        errmsg("Invalid coast specified for a fleet in %s.\n",
                        pr[p1].name);
                        return E_WARN;
                    }
                    **s = '\0'; /* terminate input string */
                    if (c1 == unit[u].coast) {
                        if (c1 == XC) {
                            errmsg("Error: Unit in %s is already of type fleet", pr[p1].name);
                        } else {
                            errmsg("Fleet %s already on specified coast.\n", pr[p1].name);
                        }
                        return E_WARN;
                    }
                } else {
                    c1 = MV; /*Default coast for non-fleet units */
                }
		/* OK, now check if unit can be transformed */
		switch ((dipent.xflags & XF_TRANS_MANYW))
		{
		 case 0: /* Corresponds to Home centres only */
			if (pr[p1].type != dipent.pl[p] && pr[p1].type != 'x' ) {
                            errmsg("%s is not a home province for %s.\n",
                            pr[p1].name, powers[p]);
                            return E_WARN;
	                 }
			break;
	 case XF_TRANS_MONEC: /* can do any centre if at least one home owned */
			if (one_owned[p] < 1) {
                            errmsg("%s must own at least one home centre to build.\n",
                            powers[p]);
                            return E_WARN;
	                }
		 case XF_TRANS_MANYC: /* Can do on any centre */
			/* but must own the centre */ 
			if (pr[p1].owner != p) {
                            errmsg("%s does not control %s.\n",
                            powers[p], pr[p1].name);
                            return E_WARN;
	                }
			break;
		 default: /* Can do anywhere */
			break; /* Nothing left to_ do */
		}
		
                unit[u].new_type = c; /* Store the new requested type */
                unit[u].new_coast = c1; /* and the new coast */
		break;
 
	default:
		errmsg("Invalid order for the %s %s %s.\n",
		 utype(unit[u].type),
		 mov_type(p1,u), pr[p1].name);
		return E_WARN;
	}
	unit[u].order = order;
	unit[u].unit = u2;
	unit[u].unit_prov = p3;
	unit[u].unit_coast = c3;
	unit[u].dest = p2;
	unit[u].dcoast = c2;
	unit[u].convoy = bp;
	unit[u].bloc = bl;
	return 0;
}
/****************************************************************************/
int moveout(int pt)
{
/* Process movement orders. */
	int u, u2, u3, u4, bounce = 0, i, index, p;
	unsigned char *s, *t, c, contest[NPROV + 1];
	int c1;
	int unit_dislodged;
	char cbuffer[1024];
	int result[MAXUNIT];
	int support[MAXUNIT];
	int fail[MAXUNIT]; /* Marked as failed when attacked, useful for transforms only */
#define supval(u) (unit[u].stype == 'm' || unit[u].stype == 'p' ? 2 : 1)
#define VOID 1
#define NO_CONVOY 2
#define CUT 3
#define BOUNCE 4
#define NO_SUPPORT 5
/* #define BESIEGE 6 */
/* #define DOSIEGE 7 */
#define BLOCKED	 8
#define BAD_CONVOY 9
#define DISLODGED 32	/* added on */
#define MAYBE_NO_CONVOY 33	/* no message */
	static char *results[] =
	{"dislodged",
	 "void",
	 "no convoy",
	 "cut",
	 "bounce",
	 "no support",
	 "",
	 "",
/* "siege in progress", */
/* "siege required", */
	 "blocked",
	 "WARN: check convoy"};
 
	for (p = 1; p <= npr; p++) {
		contest[p] = 0;
	}
	if (err)
		fprintf(rfp, "\n");
	fprintf(rfp, "Movement %s for %s of %d.", pt ? "orders" : "results",
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]));
	fprintf(rfp, " (%s.%s)\n", dipent.name, dipent.seq);
	/*
	 ** Flag bogus orders, initialize support.
	 */
	for (u = 1; u <= nunit; u++) {
		result[u] = 0;
		fail[u] = 0;
		/*** Not sure why this was here, but it messed up land-units
		 giving support to fleets to a costal region
		 MLM 28/09/2000
		if (unit[u].type != 'F' && unit[u].order != 't')
unit[u].dcoast = 0;***/ /* non-fleets not transforming have no coast */
		if (unit[u].type != 'F')
			unit[u].coast = 0; /* non-fleets not transforming have no coast */
			
		support[u] = unit[u].dcoast == MX ? -1 : supval(u) - 1;
	}
	/*
	 ** If we're not really processing, clear all the bogus proxy orders and
	 ** skip to the report generation.
	 */
	if (!processing && !predict) {
		for (u = 1; u <= nunit; u++) {
			if (unit[u].proxy != 0 && unit[u].order == 'n')
				unit[u].owner = 0;
		}
	} else {
/* Pass 0: Substitute all proxy orders. */
		for (p = 0, u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'p') {
				if (unit[u].owner != p)
					fprintf(rfp, "\n");
				fprintf(rfp, "%s: %s%s %s", powers[p = unit[u].owner], Stype(unit[u].stype),
					Utype(unit[u].type), pr[unit[u].loc].name);
				if (unit[u].coast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].coast]);
				fprintf(rfp, " Proxy given to %s.\n", powers[unit[u].unit]);
				unit[u].order = 'n';
				for (u2 = 1; u2 <= nunit; u2++) {
					if (unit[u2].proxy == u && unit[u2].owner == unit[u].unit) {
						unit[u].order = unit[u2].order;
						unit[u].unit = unit[u2].unit;
						unit[u].dest = unit[u2].dest;
						unit[u].dcoast = unit[u2].dcoast;
						unit[u].convoy = unit[u2].convoy;
						unit[u2].owner = 0;
					}
				}
			}
			/*
			 ** All the proxy orders will be at the end so we can safely delete
			 ** the extra ones here on a single pass.
			 */
			if (unit[u].proxy != 0) {
				unit[u].owner = 0;
				unit[u].order = 'n';
			}
		}
		if (p)
			fprintf(rfp, "\n");
 
/* Pass 1: Tally up all the support orders, verify convoys. */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 's' && !result[u]) {
				if (unit[u2 = unit[u].unit].order == 'm') {
					if (unit[u2].dest == unit[u].dest &&
					 (unit[u2].dcoast == unit[u].dcoast || unit[u2].dcoast <= XC)) {
						if (!result[u2])
							support[u2] += supval(u);
					} else {
						result[u] = VOID;
					}
				} else {
					if (unit[u2].loc == unit[u].dest)
						support[u2] += supval(u);
					else {
						result[u] = VOID;
					}
				}
			} else if (unit[u].order == 'm' && unit[u].convoy != NULL) {
				for (s = unit[u].convoy; *s; s++) {
					if (unit[*s].order != 'c' || unit[*s].unit != u ||
					(unit[*s].dest != unit[u].dest &&
					 unit[*s].dest != unit[*(s + 1)].loc)) {
						result[u] = NO_CONVOY;
						support[u] = supval(u) - 1;
						break;
					}
					if (unit[unit[u].dest].owner != unit[u].owner) {
					 /* Non owner attacked this unit, so set the fail flag */
					 if (dipent.xflags & XF_NOATTACK_TRANS)
					 if (dipent.xflags & XF_TRANS_MANYW)
						 fail[unit[u].dest] = 'f';
					}
				}
			}
		}
/* Pass 2a: Check for support cut from non-convoyed units */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0 || result[u])
				continue;
			if ((unit[u].order == 'm' && !unit[u].convoy && unit[u].dcoast != MX)
			 && (u2 = pr[unit[u].dest].unit)
			 && unit[u2].order == 's' && !result[u2]
			 && unit[u2].dest != unit[u].loc	/* X */
			 && unit[u2].owner != unit[u].owner) {	/* IX.6.note */
				result[u2] = CUT;
				support[unit[u2].unit] -= supval(u2);
			}
		}
/* Pass 2a.1: Check for support cut from "support needed" units */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0 || result[u])
				continue;
			if (unit[u].dcoast == MX) {
				unit[u].dcoast = MV;
				if (unit[u].order == 'm' && !unit[u].convoy && support[u] >= 0
				 && (u2 = pr[unit[u].dest].unit)
				 && unit[u2].order == 's' && !result[u2]
				 && unit[u2].dest != unit[u].loc	/* X */
				 && unit[u2].owner != unit[u].owner) {	/* IX.6.note */
					result[u2] = CUT;
					support[unit[u2].unit] -= supval(u2);
				}
			}
		}
/* Pass 3a: Check for dislodged convoys. XII.3 */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'c' && !result[u2 = unit[u].unit]) {
				for (s = unit[u2].convoy; s != NULL && *s; s++) {
					if (*s == u) {
						for (u3 = 1; u3 <= nunit; u3++) {
							if (unit[u3].owner <= 0)
								continue;
							if (unit[u3].order == 'm' && !result[u3] &&
							 unit[u3].owner != unit[u].owner &&
							 unit[u3].dest == unit[u].loc && support[u3] > support[u]) {
								result[u2] = MAYBE_NO_CONVOY;
								goto nextp3a;
							}
						}
					}
				}
			}
		 nextp3a:;
		}
/* Pass 2b: Check for support cut from convoyed units */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'm' && unit[u].convoy && !result[u]
			 && (u2 = pr[unit[u].dest].unit)
			 && unit[u2].order == 's' && !result[u2]
			 && unit[u2].dest != unit[u].loc	/* X */
			 && unit[u2].owner != unit[u].owner) {	/* IX.6.note */
/* XII.5: You can't cut support of attacks against your convoy */
				for (s = unit[u].convoy; s != NULL && *s; s++)
					if (unit[u2].unit == *s)
						goto nextp2b;
				result[u2] = CUT;
				support[unit[u2].unit] -= supval(u2);
			}
		 nextp2b:;
		}
/* Pass 3b: Recheck for dislodged convoys. XII.3 */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'c' && result[u2 = unit[u].unit] == MAYBE_NO_CONVOY) {
				for (s = unit[u2].convoy; s != NULL && *s; s++) {
					if (*s == u) {
						for (u3 = 1; u3 <= nunit; u3++) {
							if (unit[u3].owner <= 0)
								continue;
							if (unit[u3].order == 'm' && !result[u3] &&
							 unit[u3].owner != unit[u].owner &&
							 unit[u3].dest == unit[u].loc && support[u3] > support[u]) {
								for (u4 = 1; u4 <= nunit; u4++) {
									if (u4 == u3)
										continue;
									if (unit[u4].order == 'm' &&
									 unit[u4].dest == unit[u].loc &&
									 support[u4] >= support[u3])
										break;
								}
								if (u4 <= nunit)
									continue;
								result[u] = DISLODGED;
								result[u2] = NO_CONVOY;
								support[u2] = supval(u2) - 1;
								goto nextp3b;
							}
						}
					}
				}
			}
		 nextp3b:;
		}
/* Pass 2c: Check for support cut from convoyed units */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (result[u] == MAYBE_NO_CONVOY) {
				result[u] = 0;
				if ((u2 = pr[unit[u].dest].unit)
				 && unit[u2].order == 's' && !result[u2]
				 && unit[u2].dest != unit[u].loc	/* X */
				 && unit[u2].owner != unit[u].owner) {	/* IX.6.note */
/* XII.5: You can't cut support of attacks against your convoy */
					for (s = unit[u].convoy; s != NULL && *s; s++)
						if (unit[u2].unit == *s)
							goto nextp2c;
					result[u2] = CUT;
					support[unit[u2].unit] -= supval(u2);
				}
			}
		 nextp2c:;
		}
/* Pass 4a: Check for conditional movement blocked by an incoming fleet. */
/*
for (u = 1; u <= nunit; u++) {
if (unit[u].owner <= 0 || result[u]) continue;
if (p = unit[u].bloc) {
int j;
i = -1;
u3 = 0;
for (u2 = 1; u2 <= nunit; u2++) {
j = -1;
if (unit[u2].order == 'm') { 
if (!result[u2]) {
if (unit[u2].dest == p) {
j = support[u2];
}
} else {
if (unit[u2].loc == p) {
j = supval(u2);
}
}
} else {
if (unit[u2].loc == p) {
j = support[u2];
}
}
if (j > i) { 
i = j;
u3 = u2;
} else if (j == i) { 
u3 = 0;
}
}
if (u3 && (unit[u3].type == 'F')) {
result[u] = BLOCKED;
}
}
}
*/
/* Pass 4b: Check for dislodged support. X */
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'm' && (u2 = pr[unit[u].dest].unit)
			 && unit[u2].order == 's' && !result[u] && !result[u2]
			 && unit[u2].dest == unit[u].loc) {
/* Your support cannot dislodge your own unit. IX.3 */
				for (u3 = 1, p = 0; u3 <= nunit; u3++) {
					if (unit[u3].owner <= 0)
						continue;
					if (!result[u3] && unit[u3].order == 's' &&
					 unit[u3].unit == u && unit[u3].owner == unit[u2].owner)
						p += supval(u3);
				}
				if (support[u] - p > support[u2]) {
				     for (u4=1, unit_dislodged=1; u4<=nunit; u4++ ) {
                                        if ( ( u != u4 )
                                        && ( unit[ u4 ].order == 'm' )
                                        && ( !result[ u4 ] )
                                        && ( unit[ u4 ].dest == unit[ u ].dest )
                                        && ( support[ u4 ] >= support[u]-p ) )
                                            unit_dislodged = 0;
                                    }

                                    if ( unit_dislodged == 1 ) {
                                    result[u2] = DISLODGED;
                                    support[unit[u2].unit] -= supval(u2);
                                    }
                                }
			}
		}
 
/* Pass 5: Check for movement bounces */
		do {
			bounce = 0;
			for (u = 1; u <= nunit; u++) {
				if (unit[u].owner <= 0)
					continue;
				if ((unit[u].order == 'm') && !result[u]) {
					/*
					 ** The destination is contested by unit u unless there is a
					 ** unit there moving into unit u's starting location.
					 */
					if (!((u2 = pr[unit[u].dest].unit) && !result[u2]	/* IX.7.note */
					 &&unit[u2].order == 'm'
					 && unit[u2].dest == unit[u].loc
					 && !unit[u2].convoy && !unit[u].convoy)) {	/* XIV.6 */
						contest[unit[u].dest]++;
					}
					p = 0;
					if (u2) {
						/*
						 ** Your support cannot dislodge your own unit. IX.3
						 */
						if (result[u2] || (unit[u2].order != 'm' &&
						unit[u2].order != 'v')) {
							for (u3 = 1; u3 <= nunit; u3++) {
								if (unit[u3].owner <= 0)
									continue;
								if (!result[u3] && unit[u3].order == 's' &&
								 unit[u3].unit == u && unit[u3].owner == unit[u2].owner)
									p += supval(u3);
							}
						}
						/*
						 ** Unit u bounces if the unit there is holding with more support.
						 */
						if ((unit[u2].order != 'm')
						 || (unit[u2].dest == unit[u].loc
							&& !unit[u2].convoy && !unit[u].convoy)) {	/* XIV.6 */
							if (support[u] - p <= support[u2]
							 || unit[u].owner == unit[u2].owner) {	/* IX.3 */
								bounce++;
								result[u] = BOUNCE;
								goto nextp5;
							}
							/*
							 ** ...or if the unit's movement bounced and we have no support.
							 */
						} else if (result[u2]
							 && ((support[u] - p < supval(u2))
							 || unit[u].owner == unit[u2].owner)) {	/* IX.3 */
							bounce++;
							result[u] = BOUNCE;
							goto nextp5;
						}
					}
					/*
					 ** Unit bounces if another unit is moving to the same spot with
					 ** the same amount or more support.
					 */
					for (u3 = 1; u3 <= nunit; u3++) {
						if (unit[u3].owner <= 0)
							continue;
						if (u != u3 && unit[u].dest == unit[u3].dest &&
						 ((unit[u3].order == 'm') &&
						 (!result[u3] || result[u3] == BOUNCE)) &&
						 support[u] - p <= support[u3]) {
							/*
							 ** Won't bounce if unit there dislodges other unit. IX.7.note
							 */
							if (!(u2 && unit[u2].order == 'm' && !result[u2]
							 && unit[u2].dest == unit[u3].loc
							 && !unit[u2].convoy && !unit[u3].convoy)) {
								bounce++;
								result[u] = BOUNCE;
								goto nextp5;
							}
						}
					}
				}
			 nextp5:;
			}
		} while (bounce);

/* Pass 5aa: Disband requested disband units */
                for (u = 1; u <= nunit; u++) {
                        if (unit[u].owner <= 0)
                                continue;
                        if (unit[u].order == 'd') {
			    result[u] = 0; /* Disbands never fail */
			}
		}
 
/* Pass 5a: flag dislodgements */
		bounce = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if ((unit[u].order == 'm') && !result[u]) {
				if ((u2 = pr[unit[u].dest].unit) &&
				((unit[u2].order != 'm') || result[u2])) {
					if (result[u2] < DISLODGED)
						result[u2] += DISLODGED;
					unit[u2].status = 'r';
					bounce++;
				}
			}
		}
	}
/* Pass 6: Process movement, print report */
	for (p = 0, u = 1; u <= nunit; u++) {
		if (unit[u].owner <= 0)
			continue;
		if (processing || pt == unit[u].owner || pt == MASTER) {
			if (unit[u].owner != p)
				fprintf(rfp, "\n");
			if ((unit[u].owner != p) && (pt == MASTER)) {
				mast_rpt(unit[u].owner, 0);
			};
			fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
			if (unit[u].proxy != 0)
				fprintf(rfp, "%s ", owners[unit[unit[u].proxy].owner]);
			fprintf(rfp, "%s%s %s", Stype(unit[u].stype),
			 Utype(unit[u].type), pr[unit[u].loc].name);
			if (unit[u].coast > XC)
				fprintf(rfp, " (%s)", mtype[unit[u].coast]);
			switch (unit[u].order) {
			case 'c':
				fprintf(rfp, " CONVOY ");
				if ((i = unit[u2 = unit[u].unit].owner) != p) {
                                    if (!(dipent.flags & F_BLIND))
                                        fprintf(rfp, "%s ", owners[i]);
                                    else
                                        fprintf(rfp,"unknown ");
                                }
				else if (!processing && !predict && unit[u2].dest != unit[u].dest)
					result[u] = BAD_CONVOY;
				fprintf(rfp, "Army %s -> %s", pr[unit[u].unit_prov].name,
					pr[unit[u].dest].name);
				break;
			case 'h':
				fprintf(rfp, " HOLD");
				break;

			case 'd':
				fprintf(rfp, " DISBAND");
				 unit[u].owner = 0;
				break;

/* case 'l': */
/* fprintf(rfp," LIFT SIEGE"); */
/* break; */
			case 'm':
				if ((s = unit[u].convoy)) {
					while (*s) {
						if (!processing && !predict && unit[*s].owner == unit[u].owner &&
						 (unit[*s].order != 'c' || unit[*s].unit != u ||
						 unit[*s].dest != unit[u].dest)) {
							result[u] = BAD_CONVOY;
						}
						fprintf(rfp, " -> %s", pr[unit[*s++].loc].name);
					}
				}
				fprintf(rfp, " -> %s", pr[unit[u].dest].name);
				if (unit[u].dcoast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);
				break;
			case 'n':
				if (unit[u].owner != AUTONOMOUS) {
					fprintf(rfp, ", No Order Processed");
					if (dipent.phase[0] != 'F' || pr[unit[u].loc].type != 'v')
						more_orders++;
				}
				break;
			case 'p':
				fprintf(rfp, " PROXY TO %s", powers[unit[u].unit]);
				break;
			case 's':
				fprintf(rfp, " SUPPORT ");
				if ((i = unit[u2 = unit[u].unit].owner) != p) {
				 if (!(dipent.flags & F_BLIND))
					fprintf(rfp, "%s ", owners[i]);
				 else
					fprintf(rfp,"unknown ");
				}
				if (!(dipent.flags & F_BLIND))
				 fprintf(rfp, "%s %s", Utype(unit[u2].type), pr[unit[u].unit_prov].name);
				else
				 fprintf(rfp, "%s %s", "Unit", pr[unit[u].unit_prov].name);
				if (unit[u].unit_coast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].unit_coast]);
				if (unit[u2].loc != unit[u].dest) {
					fprintf(rfp, " -> %s", pr[unit[u].dest].name);
					if (unit[u].dcoast > XC)
						fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);
				}
				break;
			 case 't':
                            fprintf(rfp, " TRANSFORMS TO %s", Utype(unit[u].new_type));
                            if (unit[u].new_type == 'F' ) {
                            /* Fleets can have coast, so show it if it has one */
                            if ((c1 = unit[u].new_coast) > XC)
                                fprintf(rfp, " (%s)", mtype[c1]);
                            }
				/* Now apply the changes iif not dislodged */
				if (unit[u].status != 'r' && !fail[u]) {
				 unit[u].type = unit[u].new_type;
				 unit[u].coast = unit[u].new_coast;
				}
                            break;
 
			default:
				fprintf(rfp, " INVALID ORDER (internal error)");
			}
			if (fail[u])
				fprintf(rfp, ". (*failed*)\n");
			else if (result[u]) {
				fprintf(rfp, ". (*%s%s*)\n", results[result[u] % DISLODGED],
					result[u] > DISLODGED ? ", dislodged" : "");
			} else {
				fprintf(rfp, ".\n");
			}
		}
	}
/* Pass 7: Print out retreats. */
	if (processing || predict) {
		i = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			/*
			 ** Advance moving unit into the destination province.
			 */
			if (unit[u].order == 'm' && !result[u]) {
				unit[u].loc = p = unit[u].dest;
				unit[u].coast = unit[u].dcoast;
			}
		}
		if (bounce)
			fprintf(rfp, "\n\nThe following units were dislodged:\n\n");
		bounce = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].status == 'r') {
				unsigned char buffer[1024];
				unit[u].convoy = &heap[hp];
				t = buffer;
				sprintf(t, "The %s %s %s %s", owners[unit[u].owner],
					Utype(unit[u].type),
					mov_type(unit[u].loc,u), 
					pr[unit[u].loc].name);
				while (*t)
					t++;
				if (unit[u].coast > XC) {
					sprintf(t, " (%s)", mtype[unit[u].coast]);
					while (*t)
						t++;
				}
				i = 0;
				/* TODO ugh! a side effect in the loop test */
				for (s = pr[unit[u].loc].move; 
				 (!(dipent.xflags & XF_AUTODISBAND)) &&(p = *s++); 
				 s++) {
					if (!contest[p] 
					&& ((*s >> 4) == unit[u].coast || unit[u].type == 'W' )
				 && (*s & 0x0f) != MX
					 && (!(u2 = pr[p].unit)	/* XI: can't retreat to */
						||(unit[u2].loc != p	/* attackers origin. */
					 && unit[u2].loc != unit[u].loc))
					 && (dipent.phase[0] != 'F' || pr[p].type != 'v')) {
						if (!i++)
							sprintf(t, " can retreat to ");
						else
							sprintf(t, " or ");
						while (*t)
							t++;
						heap[hp++] = p;
						sprintf(t, "%s", pr[p].name);
						while (*t)
							t++;
						if ((c = *s & 0x0f) > XC) {
							sprintf(t, " (%s)", mtype[c]);
							while (*t)
								t++;
							cbuffer[i - 1] = c;
						} else
							cbuffer[i - 1] = 0;
						bounce++;
					}
				}
				if (!i) {
					sprintf(t, " with no valid retreats was destroyed.\n");
					unit[u].owner = 0;
				} else {
					sprintf(t, ".\n");
				}
				heap[hp++] = 0;
				/* put the coasts on the heap */
				unit[u].rcoast = &heap[hp];
				for (index = 0; index < i; index++)
					heap[hp++] = cbuffer[index];
				heap[hp++] = 0;
				for (t = s = buffer, p = 0; *s; s++, p++) {
					if (p > 78) {
						while (*--s != ' ');
						*s++ = '\0';
						fprintf(rfp, "%s\n", t);
						t = s;
						p = 0;
					}
				}
				fprintf(rfp, "%s", t);
			}
		}
	}
/* Pass 8: recalculate blockade settings */
	if ((dipent.flags & F_WINGS) && (processing || predict)){
		/* Firstly, set all provinces to 'unblockaded' */
            for (i = 1; i <= npr;i++) {
			pr[i].blockaded = 0;
		}
		/* Now, go through all units and set to blockaded if occupied by an
		 enemy wing
		 */
		 for (u = 1; u <= nunit; u++) {
			if (unit[u].status == ':' && unit[u].owner != 0 ) {
				/* only non-retreating, existing units */
				if (unit[u].type == 'W' && unit[u].owner != pr[unit[u].loc].owner) {
					/* we can mark non-scs as blockaded, but who cares! */
					pr[unit[u].loc].blockaded = 1;
				}
			}
		}
	} 
	return bounce;
}
 
/******************************************************************************/
