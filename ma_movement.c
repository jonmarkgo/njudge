/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  po_movechk.c
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

#define convoyable(p) (1)

/*
 * Input processing of the regular movement orders
 */
int ma_movein(char **s, int p)
{
	/* char **s; Input stream */
	/* int p; Power specification */

	char c, order, *t;
	int i, j, p1, p2, u, u1, u2, c1, c2, bl;
	unsigned char *bp;

/*  Process lines of the form:

 *    cmd ::= <power>: <move list>
 *    move list ::= <move>{; <move list>}
 *    move ::= <type> <province> - <province>
 *      | <type> <province> holds
 *      | <type> <province> support {<power>:} <type> <province> {-<province>}
 *      | <type> <province> convoy  {<power>:} <type> <province> {-<province>}
 *    type ::= Army | Fleet
 */

	*s = get_type(*s, &c);
	*s = get_prov(*s, &p1, &c1);
	if (!p1) {
		errmsg("Unrecognized source province -> %s", *s);
		return E_WARN;
	}
	u1 = pr[p1].unit;
	u2 = pr[p1].gunit;

	u = c == 'G' ? u2 :
	    c != 'x' ? u1 :
	    u1 && unit[u1].owner == p ? u1 :
	    u2 && unit[u2].owner == p ? u2 :
	    u1 ? u1 : u2;

	if (!u) {
		errmsg("No %s present %s %s.\n", utype(c),
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	if (p != unit[u].owner && p != MASTER) {
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
	if (c != 'x' && c != unit[u].type) {
		errmsg("The unit %s %s is %s, not %s.\n",
		       water(p1) ? "in the" : "in", pr[p1].name,
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

	case 'b':		/* besiege */
		if (unit[u].type == 'G') {
			errmsg("Invalid order for garrison.\n");
			return E_WARN;
		}
		if (!has_fortress(p1)) {
			errmsg("%s has no fortress to siege.\n", pr[p1].name);
			return E_WARN;
		}
		if (unit[u].type == 'F' && !has_port(p1)) {
			errmsg("A fleet can only besiege ports.\n");
			return E_WARN;
		}
		if (!has_crebellion(p1) && pr[p1].gunit == 0) {
			errmsg("%s has no garrison or rebellion to siege.\n", pr[p1].name);
			return E_WARN;
		}
		*s = get_type(*s, &c);
		if (!strncasecmp(*s, "rebellion", 9))
			*s += 9;
		break;


	case 'c':
	case 's':
		*s = get_type(*s, &c);
		*s = get_prov(*s, &p2, &c2);
		if (!p2) {
			errmsg("Unrecognized source province for support/convoy -> %s", *s);
			return E_WARN;
		}
		if (c == 'G') {
			u2 = has_garrison(p2);
			if (!u2) {
				errmsg("No garrison present %s %s.\n",
				water(p2) ? "in the" : "in", pr[p2].name);
				return E_WARN;
			}
		} else {
			u2 = pr[p2].unit;
			if (!u2) {
				errmsg("No unit present %s %s.\n",
				water(p2) ? "in the" : "in", pr[p2].name);
				return E_WARN;
			}
		}

		if (order == 'c' && unit[u2].type != 'A') {
			if (is_garrison(u))
				errmsg("Invalid order for garrison.\n");
			else
				errmsg("The convoy order should specify source %s\n",
				    "and final destination of an army.");
			return E_WARN;
		}
		if (c != 'x' && c != unit[u2].type) {
			errmsg("The unit %s %s is %s, not %s.\n",
			       water(p2) ? "in the" : "in", pr[p2].name,
			       autype(unit[u2].type), autype(c));
			return E_WARN;
		}
		t = get_action(*s, &c);

		if (c == 'm') {
			*s = get_prov(t, &p2, &c2);
			if (!p2) {
				errmsg("Support/convoy movement to unrecognized province -> %s",
				       *s);
				return E_WARN;
			}
		}
		if (order == 's' && c == 'v') {
			if (!is_garrison(u2)) {
				errmsg("Can only support conversions from Garrison.\n");
				return E_WARN;
			}
			*s = get_type(t, &c);
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
		c1 = 0;
		if (order == 's' &&
		    ((is_garrison(u) && p1 != p2) ||
		     (!is_garrison(u) && (!valid_move(u, p2, &c1, &bl)
					  || c1 == MX)))) {
			errmsg("The %s %s %s can't get to %s%s to support.\n",
			       utype(unit[u].type),
			       water(p1) ? "in the" : "in", pr[p1].name,
			       water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p1].type == 'v') {
			errmsg("Invalid order for fall in the %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p2].type == 'v') {
			errmsg("Invalid order for fall in the %s.\n", pr[p2].name);
			return E_WARN;
		}
		if (!c2 && unit[u2].type == 'F')
			valid_move(u2, p2, &c2, &i);	/* set c2 */
		break;


	case 'h':
	case 'n':
		break;


	case 'l':		/* lift siege */
		if (is_garrison(u)) {
			errmsg("Invalid order for garrison.\n");
			return E_WARN;
		}
		if (!is_sieged(p1)) {
			errmsg("No siege in progress in %s.\n", pr[p1].name);
			return E_WARN;
		}
		break;


	case 'm':
		if (is_garrison(u)) {
			errmsg("Invalid order for garrison.\n");
			return E_WARN;
		}
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
					if (!valid_move(i, p2, &c2, &j) ||
					    !convoyable(p2) ||
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
				errmsg("Invalid order syntax for the fleet in %s%s.\n",
				   water(p1) ? "the " : "", pr[p1].name);
				return E_WARN;
			}
		} else {
			if (!valid_move(u, p2, &c2, &bl)) {
				errmsg("The %s %s %s can't get to %s%s.\n", utype(unit[u].type),
				water(p1) ? "in the" : "in", pr[p1].name,
				   water(p2) ? "the " : "", pr[p2].name);
				return E_WARN;
			}
		}

		if (dipent.phase[0] == 'F' && pr[p1].type == 'v') {
			errmsg("Invalid order for fall in the %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (dipent.phase[0] == 'F' && pr[p2].type == 'v') {
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


	case 'v':		/* convert */
		*s = get_type(*s, &c);
		if (is_garrison(u) && c == 'x') {
			if (has_port(p1)) {
				errmsg("Conversion must specify army or fleet in port.\n");
				return E_WARN;
			} else {
				c = 'A';
			}
		} else if (!is_garrison(u) && c != 'x' && c != 'G') {
			errmsg("Fleets and armies can only convert to garrisons.\n");
			return E_WARN;
		}
		if (unit[u].type == c) {
			errmsg("The %s in %s already is %s.\n", utype(unit[u].type),
			       pr[p1].name, autype(c));
			return E_WARN;
		}
		if (c == 'x')
			c = 'G';

		if (!has_fortress(p1)) {
			errmsg("No fortress or fortified city in %s.\n", pr[p1].name);
			return E_WARN;
		}
		if (c == 'F' && !has_port(p1)) {
			errmsg("%s is not a port.  Can only convert to an army.\n",
			       pr[p1].name);
			return E_WARN;
		}
		p2 = p1;
		u2 = c;
		c2 = c == 'F' ? XC : MV;
		break;


	default:
		errmsg("Invalid order for the %s %s %s.\n",
		       utype(unit[u].type),
		       water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}

	unit[u].order = order;
	unit[u].unit = u2;
	unit[u].dest = p2;
	unit[u].dcoast = c2;
	unit[u].convoy = bp;
	unit[u].bloc = bl;

	return 0;
}

int ma_moveout(int pt)
{

/*  Process movement orders.  */

	int u, u2, u3, bounce = 0, i, index, p;
	unsigned char *s, *t, c, contest[NPROV + 1];

	char cbuffer[1024];

	int result[MAXUNIT];
	int support[MAXUNIT];

#define supval(u)   (unit[u].stype == 'm' || unit[u].stype == 'p' ? 2 : 1)

#define VOID        1
#define NO_CONVOY   2
#define CUT         3
#define BOUNCE      4
#define NO_SUPPORT  5
#define BESIEGE     6
#define DOSIEGE	    7
#define BLOCKED	    8
#define BAD_CONVOY  9
#define DISLODGED       32	/* added on */
#define MAYBE_NO_CONVOY 33	/* no message */

	static char *results[] =
	{"dislodged",
	 "void",
	 "no convoy",
	 "cut",
	 "bounce",
	 "no support",
	 "siege in progress",
	 "siege required",
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

	fputc('\n', rfp);

	/*
	   **  Flag bogus orders, initialize support.
	 */

	for (u = 1; u <= nunit; u++) {
		result[u] = 0;

		support[u] = unit[u].dcoast == MX ? -1 : supval(u) - 1;

		/*
		   **  You get an extra support moving into a rebelling province
		   **  as long as no one else is trying it at the same time.
		 */

		if (unit[u].order == 'm' &&
		    pr[p = unit[u].dest].owner != unit[u].owner &&
		    has_rebellion(p)) {
			for (u2 = 1; u2 <= nunit; u2++) {
				if (u2 != u &&
				    unit[u2].owner != pr[p].owner &&
				    unit[u2].order == 'm' &&
				    unit[u2].dest == p)
					break;
			}
			if (u2 == nunit + 1)
				support[u]++;
		}
		/*
		   **  Verify that besiege orders are valid.
		 */

		p = unit[u].loc;
		switch (unit[u].order) {
		case 'b':	/* BESIEGE ORDER */
			if (!has_garrison(p) && !has_crebellion(p))
				result[u] = VOID;
			break;
		case 'v':	/* CONVERT ORDER */
			/* A besieged unit can not convert */
			if (is_garrison(u)) {
				if (is_sieged(p))
					result[u] = BESIEGE;
			} else {
				/* 
				   ** A city with a garrison or rebellion 
				   ** can't be entered 
				 */
				if (has_garrison(p) || has_crebellion(p)) {
					result[u] = DOSIEGE;
				}
			}
			break;
		default:
			/* All other commands */
			if (is_sieged(p) && !is_garrison(u) && unit[u].order != 'l')
				result[u] = BESIEGE;
		}
	}

	/*
	   **  If we're not really processing, clear all the bogus proxy orders and
	   **  skip to the report generation.
	 */

	if (!processing) {
		for (u = 1; u <= nunit; u++) {
			if (unit[u].proxy != 0 && unit[u].order == 'n')
				unit[u].owner = 0;
		}
	} else {

/*  Pass 0: Substitute all proxy orders. */

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
			   **  All the proxy orders will be at the end so we can safely delete
			   **  the extra ones here on a single pass.
			 */

			if (unit[u].proxy != 0) {
				unit[u].owner = 0;
				unit[u].order = 'n';
			}
		}
		if (p)
			fprintf(rfp, "\n");


/*  Pass 1: Tally up all the support orders, verify convoys. */


		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;

			if ((p = unit[u].bloc) && (u2 = pr[p].unit) && unit[u2].type == 'F'
			    && !allies[unit[u2].owner][unit[u].owner]) {
				result[u] = BLOCKED;
			}
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
				}
			}
		}

/*  Pass 2a: Check for support cut from non-convoyed units */

		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0 || result[u])
				continue;
			if (((unit[u].order == 'm' && !unit[u].convoy && unit[u].dcoast != MX) ||
			     unit[u].order == 'v')
			    && (u2 = pr[unit[u].dest].unit)
			    && unit[u2].order == 's' && !result[u2]
			    && unit[u2].dest != unit[u].loc	/* X */
			    && unit[u2].owner != unit[u].owner) {	/* IX.6.note */
				result[u2] = CUT;
				support[unit[u2].unit] -= supval(u2);
			}
		}

/*  Pass 2a.1: Check for support cut from "support needed" units */

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

/*  Pass 3a: Check for dislodged convoys. XII.3 */

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

/*  Pass 2b: Check for support cut from convoyed units */

		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'm' && unit[u].convoy && !result[u]
			    && (u2 = pr[unit[u].dest].unit)
			    && unit[u2].order == 's' && !result[u2]
			    && unit[u2].dest != unit[u].loc	/* X */
			    && unit[u2].owner != unit[u].owner) {	/* IX.6.note */

/*  XII.5: You can't cut support of attacks against your convoy */

				for (s = unit[u].convoy; s != NULL && *s; s++)
					if (unit[u2].unit == *s)
						goto nextp2b;

				result[u2] = CUT;
				support[unit[u2].unit] -= supval(u2);
			}
		      nextp2b:;
		}

/*  Pass 3b: Recheck for dislodged convoys. XII.3 */

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

/*  Pass 2c: Check for support cut from convoyed units */

		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (result[u] == MAYBE_NO_CONVOY) {
				result[u] = 0;
				if ((u2 = pr[unit[u].dest].unit)
				  && unit[u2].order == 's' && !result[u2]
				    && unit[u2].dest != unit[u].loc	/* X */
				    && unit[u2].owner != unit[u].owner) {	/* IX.6.note */

/*  XII.5: You can't cut support of attacks against your convoy */

					for (s = unit[u].convoy; s != NULL && *s; s++)
						if (unit[u2].unit == *s)
							goto nextp2c;

					result[u2] = CUT;
					support[unit[u2].unit] -= supval(u2);
				}
			}
		      nextp2c:;
		}

/*  Pass 4a: Check for conditional movement blocked by an incoming fleet. */

		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0 || result[u])
				continue;
			if ((p = unit[u].bloc)) {
				int j;
				i = -1;
				u3 = 0;
				for (u2 = 1; u2 <= nunit; u2++) {
					j = -1;
					if (unit[u2].order == 'm' ||
					    (unit[u2].order == 'v' && unit[u2].type == 'G')) {
						if (!result[u2]) {
							if (unit[u2].dest == p) {
								j = support[u2];
							}
						} else {
							if (unit[u2].loc == p && unit[u2].type != 'G') {
								j = supval(u2);
							}
						}
					} else {
						if (unit[u2].loc == p && unit[u2].type != 'G') {
							j = support[u2];
						}
					}

					if (j > i) {	/* The one with more support makes it?  */
						i = j;
						u3 = u2;
					} else if (j == i) {	/* If the same, neither makes it        */
						u3 = 0;
					}
				}

				if (u3 && (unit[u3].type == 'F' ||
					   (unit[u3].type == 'G' && unit[u3].unit == 'F')) &&
				!allies[unit[u3].owner][unit[u].owner]) {
					result[u] = BLOCKED;
				}
			}
		}

/*  Pass 4b: Check for dislodged support. X */

		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if (unit[u].order == 'm' && (u2 = pr[unit[u].dest].unit)
			    && unit[u2].order == 's' && !result[u] && !result[u2]
			    && unit[u2].dest == unit[u].loc) {

/*  Your support cannot dislodge your own unit.  IX.3  */

				for (u3 = 1, p = 0; u3 <= nunit; u3++) {
					if (unit[u3].owner <= 0)
						continue;
					if (!result[u3] && unit[u3].order == 's' &&
					    unit[u3].unit == u && unit[u3].owner == unit[u2].owner)
						p += supval(u3);
				}

				if (support[u] - p > support[u2]) {
					result[u2] = DISLODGED;
					support[unit[u2].unit] -= supval(u2);
				}
			}
		}


/*  Pass 5: Check for movement bounces */

		do {
			bounce = 0;
			for (u = 1; u <= nunit; u++) {
				if (unit[u].owner <= 0)
					continue;
				if ((unit[u].order == 'm' ||
				     (unit[u].order == 'v' && unit[u].type == 'G')) && !result[u]) {

					/*
					   **  The destination is contested by unit u unless there is a
					   **  unit there moving into unit u's starting location.
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
						   **  Your support cannot dislodge your own unit.  IX.3
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
						   **  Unit u bounces if the unit there is holding with more support.
						 */

						if ((unit[u2].order != 'm' && unit[u2].order != 'v')
						    || (unit[u2].dest == unit[u].loc
							&& !unit[u2].convoy && !unit[u].convoy)) {	/* XIV.6 */
							if (support[u] - p <= support[u2]
							    || unit[u].owner == unit[u2].owner) {	/* IX.3 */
								bounce++;
								result[u] = BOUNCE;
								goto nextp5;
							}
							/*
							   **  ...or if the unit's movement bounced and we have no support.
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
					   **  Unit bounces if another unit is moving to the same spot with
					   **  the same amount or more support.
					 */

					for (u3 = 1; u3 <= nunit; u3++) {
						if (unit[u3].owner <= 0)
							continue;
						if (u != u3 && unit[u].dest == unit[u3].dest &&
						((unit[u3].order == 'm' ||
						  (unit[u3].order == 'v' && unit[u3].type == 'G')) &&
						 (!result[u3] || result[u3] == BOUNCE)) &&
						    support[u] - p <= support[u3]) {

							/*
							   **  Won't bounce if unit there dislodges other unit.  IX.7.note
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

/*  Pass 5a: flag dislodgements */

		bounce = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;
			if ((unit[u].order == 'm' ||
			(unit[u].order == 'v' && unit[u].type == 'G')) &&
			    !result[u]) {
				if ((u2 = pr[unit[u].dest].unit) &&
				    ((unit[u2].order != 'm' && unit[u2].order != 'v') ||
				     result[u2])) {
/*                fprintf(rfp, "First part: Dislodge in %s unit %d by %d\n",
 */
/*                              pr[unit[u2].loc].name, u2, u); */
					if (result[u2] < DISLODGED)
						result[u2] += DISLODGED;
					unit[u2].status = 'r';
					bounce++;
				}
			} else if (unit[u].order == 'b' && !result[u]) {
				if ((u2 = has_garrison(unit[u].loc)) && is_sieged(unit[u].loc)) {
					for (u3 = 1; u3 <= nunit; u3++) {
						if (unit[u3].order == 'm'
						    && unit[u3].dest == unit[u].loc
						    && !result[u3]) {
							break;
						}
					}
					if (u3 > nunit) {
/*                fprintf(rfp, "Second part: Dislodge in %s unit %d by %d\n",
 */
/*                              pr[unit[u2].loc].name, u2, u); */
						if (result[u2] < DISLODGED)
							result[u2] += DISLODGED;
						unit[u2].status = 'r';
						bounce++;
					}
				}
			}
		}
/*   fprintf(rfp, "%s : %d\n", pr[unit[1].loc].name, result[1]); */
	}

/*  Pass 6: Process movement, print report */

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
			case 'b':
				fprintf(rfp, " BESIEGE");
				if ((u2 = has_garrison(unit[u].loc))) {
					fprintf(rfp, " %s %s", owners[unit[u2].owner], Utype(unit[u2].type));
				} else if (has_crebellion(unit[u].loc)) {
					fprintf(rfp, " Rebellion");
				}
				break;

			case 'c':
				fprintf(rfp, " CONVOY ");
				if ((i = unit[u2 = unit[u].unit].owner) != p)
					fprintf(rfp, "%s ", owners[i]);
				else if (!processing && unit[u2].dest != unit[u].dest)
					result[u] = BAD_CONVOY;
				fprintf(rfp, "Army %s -> %s", pr[unit[u2].loc].name,
					pr[unit[u].dest].name);
				break;

			case 'h':
				fprintf(rfp, " HOLD");
				break;

			case 'l':
				fprintf(rfp, " LIFT SIEGE");
				break;

			case 'm':
				if ((s = unit[u].convoy)) {
					while (*s) {
						if (!processing && unit[*s].owner == unit[u].owner &&
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
				if ((i = unit[u2 = unit[u].unit].owner) != p)
					fprintf(rfp, "%s ", owners[i]);
				fprintf(rfp, "%s %s", Utype(unit[u2].type), pr[unit[u2].loc].name);
				if (unit[u2].coast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u2].coast]);

				if (unit[u2].loc != unit[u].dest) {
					fprintf(rfp, " -> %s", pr[unit[u].dest].name);
					if (unit[u].dcoast > XC)
						fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);
				}
				if (unit[u2].type == 'G') {
					fprintf(rfp, " CONVERSION");
				}
				break;

			case 'v':
				fprintf(rfp, " CONVERT TO %s", Utype(unit[u].unit));
				break;

			default:
				fprintf(rfp, " INVALID ORDER (internal error)");
			}

			if (result[u]) {
				fprintf(rfp, ".  (*%s%s*)\n", results[result[u] % DISLODGED],
					result[u] > DISLODGED ? ", dislodged" : "");
			} else {
				fprintf(rfp, ".\n");
			}
		}
	}

/*  Pass 7: Print out retreats.  */

	if (processing) {
		i = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].owner <= 0)
				continue;

			/*
			   **  Advance moving unit into the destination province.
			 */

			if (unit[u].order == 'm' && !result[u]) {
				unit[u].loc = p = unit[u].dest;
				unit[u].coast = unit[u].dcoast;
				/*
				   ** Moving into anothers power province which is in rebellion or
				   ** which city is in rebellion liberates the rebellion.
				 */
				if (has_rebellion(p) &&
				    pr[p].owner != unit[u].owner) {
					if (!i++)
						fprintf(rfp, "\n");
					fprintf(rfp, "Rebellion in %s liberated.\n", pr[p].name);
					remove_rebellion(p);
				}
			}
			/*
			   **  Holding in a province will put down a rebellion.
			 */

			if (unit[u].order == 'h' && !result[u]) {
				if (has_prebellion(p = unit[u].loc)) {
					if (!i++)
						fprintf(rfp, "\n");
					fprintf(rfp, "Rebellion in %s put down.\n", pr[p].name);
					remove_prebellion(p);
				}
			}
			/*
			   **  Change the type of units who are converting.
			 */

			else if (unit[u].order == 'v' && !result[u]) {
				unit[u].type = unit[u].unit;
				unit[u].coast = unit[u].dcoast;
				p = unit[u].loc;
				/*
				   ** Adjust the gunit flag
				 */
				if (is_garrison(u))
					pr[unit[u].loc].gunit = u;
				else
					pr[unit[u].loc].gunit = 0;

				if (has_rebellion(p) &&
				    pr[p].owner != unit[u].owner) {
					if (!i++)
						fprintf(rfp, "\n");
					fprintf(rfp, "Rebellion in %s liberated.\n", pr[p].name);
					remove_rebellion(p);
				}
			}
			/*
			   **  A besiege sets the flag, or if already set, clears it.
			 */

			else if (unit[u].order == 'b' && !result[u]) {
				if (is_sieged(p = unit[u].loc)) {
					if (has_crebellion(p)) {
						if (!i++)
							fprintf(rfp, "\n");
						fprintf(rfp, "Rebellion in %s put down.\n", pr[p].name);
					}
					remove_siege(p);
					remove_rebellion(p);
				} else {
					set_siege(p);
				}
			}
			/*
			   **  Clear the siege in process flag if lifted.
			 */

			else if (unit[u].order == 'l' && !result[u]) {
				remove_siege(unit[u].loc);
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
				sprintf(t, "The %s %s in %s%s", owners[unit[u].owner],
					Utype(unit[u].type),
					water(unit[u].loc) ? "the " : "",
					pr[unit[u].loc].name);
				remove_siege(unit[u].loc);
				while (*t)
					t++;
				if (unit[u].coast > XC) {
					sprintf(t, " (%s)", mtype[unit[u].coast]);
					while (*t)
						t++;
				}
				i = 0;
				if (!is_garrison(u)) {
					for (s = pr[unit[u].loc].move; (p = *s++); s++) {
						if (!contest[p] && (*s >> 4) == unit[u].coast && (*s & 0x0f) != MX
						    && (!(u2 = pr[p].unit)	/* XI: can't retreat to */
						     ||(unit[u2].loc != p	/* attackers origin.    */
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

					/*
					   ** The rules to retreat to a garrison are:
					   ** 1) There should be a fortress.
					   ** 2) It should be unoccupied.
					   ** 3) Free of rebellion.
					   ** 4) And if we are a fleet, it should have a port.
					 */
					if (has_fortress(unit[u].loc) &&
					    !has_garrison(unit[u].loc) &&
					  !has_crebellion(unit[u].loc) &&
					    (has_port(unit[u].loc) || unit[u].type == 'A')) {
						if (!i++) {
							sprintf(t, " can convert to a Garrison");
							while (*t)
								t++;
							heap[hp++] = unit[u].loc;
							bounce++;
						}
					}
				} else {
					/*
					   ** The garrison was eliminated.
					 */
					pr[unit[u].loc].gunit = 0;
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
	return bounce;		/* return true if retreats are needed */
}
