/*
** $Log$
** Revision 1.34  2004/07/10 07:18:22  nzmb
** Final tweaks to new convoy code. Fixes bug #290.
**
** Revision 1.33  2004/07/09 17:57:04  millis
** Restored disabled code for multi-province units checks
**
** Revision 1.32  2004/07/09 04:07:13  nzmb
** Major convoy changes -- the convoy field in the unit struct now is a
** list of provinces being convoyed through, not a list of units. This
** simplifies convoy processing in blind (it allows us to accept all convoys,
** which allows a fix to bug #290). Fair warning: this code compiles but is
** not tested -- that's coming.
**
** Revision 1.31  2004/07/07 22:00:00  millis
** Small fix as was showing 'Unit' not real type for convoyed units
**
** Revision 1.30  2004/07/05 07:17:34  nzmb
** Partial fix to bug 290. Unfortunately, convoys in blind seem to be
** somewhat broken because of this fix (by my testing, long convoys don't
** work). I'll fix this as soon as I can.
**
** Revision 1.29  2004/07/05 06:33:30  nzmb
** Code cleanup: fixed unnecessary tests in if clauses and fixed formatting
** in part of movein.
**
** Revision 1.28  2004/07/04 03:25:25  millis
** Fix bug 328, implement Portage variant
**
** Revision 1.26  2004/05/22 08:51:46  millis
** Bug 297: Add Intimate Diplomacy
**
** Revision 1.24  2004/02/14 23:19:36  millis
** Fix so that blocked gateway movement doesn't contest movement (so that space
** can be considered for retreats)
**
** Revision 1.23  2004/01/11 01:02:29  millis
** Fix bug 265, so that convoys work in high seas.
**
** Revision 1.22  2003/07/27 15:50:07  millis
** Fix bug 204
**
** Revision 1.21  2003/07/22 23:32:21  millis
** Fix Bug 201
**
** Revision 1.20  2003/06/04 23:17:22  millis
** Fix bug 165, mis-ajudication over TSR
**
** Revision 1.19  2003/05/12 02:05:25  millis
** Small text correction
**
** Revision 1.18  2003/05/02 22:16:08  millis
** Various changes (NEUTRALS, Artillery etc.)
**
** Revision 1.17  2003/02/09 23:07:45  millis
** Fixed blind game reports showing unrequested support of unit to itself
**
** Revision 1.16  2003/01/18 14:36:05  millis
** Merged in USTV changes, basically:
** Native Units
** 1/2 stregnth movements
** Multi-unit provinces
**
** Revision 1.15  2002/08/28 00:32:11  millis
** Removed some warnings only
**
** Revision 1.14  2002/04/15 12:55:46  miller
** Multiple changes for blind & Colonial & setup from USTV
**
** Revision 1.13  2002/03/05 23:19:49  miller
** Fix special bleagured garisson bug
**
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
** Remove DOS 
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
** Recalculate blockade settings
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
#include <limits.h>  /* for UCHAR_MAX */

#include "functions.h"
#include "dip.h"
#include "porder.h"

#if NPROV > UCHAR_MAX
#error "NPROV can be at most UCHAR_MAX (255 on most systems)"
#endif

#define INVALID_ORDER(p1,u)  errmsg("Invalid order for the %s %s %s.\n", \
                 utype(unit[u].type), \
                 mov_type(p1,u), pr[p1].name); \
                return E_WARN;


static int HasCoast(int p, char c) {
/* See if the province has the required coast */

    int ret = 0;


    /* If has coasts but none specified, also fail */
    if (c == MV && pr[p].coasts) {
	/* No coast specified but province has coasts */
	ret = 0;
    } else if (c != MV && !pr[p].coasts) {
	/* Coast specified but province has no coasts */
	ret = 0;
    } else if (c == MV && !pr[p].coasts && HAS_PORTAGE ) {
	/* Not coast specified, province has no coasts and 
	 * playing PORTAGE variant */
	ret = 1;
    } else 
    
      switch (c) {

        case EC:
	    ret = pr[p].coasts & HAS_EC;
	    break;

	case NC:
	    ret = pr[p].coasts & HAS_NC;
	    break;

	case SC:
	    ret = pr[p].coasts & HAS_SC;
	    break;

	case WC:
	    ret = pr[p].coasts & HAS_WC;
	    break;

	case XC:
	    ret = 1;  /* Always have coast */

	default:
	    break;


    }

    return ret;
}

static int valid_aw_move(int i, int p2, int *c2, int *j)
{
    /* See firstly if a valid fleet move */
    /* Then, if airlifts or portage permitted, check if a valid land move */

    int ret;

    ret = valid_move(i, p2, c2, j);

    if (!ret)
        if ((dipent.xflags & XF_AIRLIFTS || HAS_PORTAGE) && (unit[i].coast != MV)) {
	    unit[i].coast = MV;
	    ret = valid_aw_move(i, p2, c2, j);
        }
	
    return ret;

	
}

static int CheckValidACUnit( int u) 
{

    /* See if unit in place is of a valid type to airlift/convoy */

    /* If game is blind, always permit this movement (to prevent players checking
     * for adjacent units 
     * Otherwise, if a fleet, permit
     * Otherwise, if a wing and airlifts in operation, permit
     * Otherwise, no good 
     */

   if (dipent.flags & F_BLIND)
        return 1;

   if (unit[u].type == 'F')
	return 1;

   if (unit[u].type == 'W' && dipent.xflags & XF_AIRLIFTS)
	return 1;

   if (unit[u].type == 'A' && HAS_PORTAGE)
	return 1;

   return 0;
}


	
int convoyable(int p) 
{ 
    if ((dipent.xflags & XF_AIRLIFTS))
	return 1;  /* we have airlifts enabled: possible anywhere! */

    if (water(p)) 
        return 1; /* province has water */

    if ((dipent.xflags & XF_COASTAL_CONVOYS))
    	return 1; /* Game has costal convoys */

    if ((dipent.xflags & XF_PROV_DUALITY) && (pr[p].type2 == 'w'))
        return 1; /* Province can also act as water and game is duality */

    if (HAS_PORTAGE)
	return 1;  /* All land provinces can also convoy */

    return 0;  /* Other cases, nothing doing! */

}

int dual_province(int p)
{
	return ((dipent.xflags & XF_PROV_DUALITY) && (pr[p].type2 != pr[p].type));
}

int dual_land(int p)
{
	return dual_province(p) && pr[p].type != 'w';
}

static int NoValidConvoyingFleet(char *s, int u)
{
    /*
     * *s is now an entry into the province table, not the unit table.
     */
    int ret_code = 1;
    int p = *s;
    int u1;

    pr[p].order_index = 1; /* order_index updated in GetUnitIndex() */
    u1 = GetUnitIndex(p, MASTER);
    do {

        if ((unit[u1].order != 'c' && unit[u1].order != 'a') || unit[u1].unit != u ||
           (unit[u1].dest != unit[u].dest && unit[u1].dest != (*(s + 1))) ) {
	    /* Unit found but is doing something else */
        } else {
            /* Unit found and IS convoying/airlifting this one! */
            ret_code = 0;
        }
        u1 = GetUnitIndex(p, MASTER);

    } while (ret_code == 1 && pr[p].order_index > 1);

    pr[p].order_index = 1; /* Reset the index so will start from first unit */

    return ret_code;
}

extern int one_owned[];
int StrictConvoy(int p_index)
{
	if (!(dipent.xflags & XF_STRCONVOY ) ) return 1; /* allow when no flag */

	/* See if unit is landlocked, and thus cannot be convoyed ever */
	if (LandLocked(p_index))
	    return 0;

	return 1;
	
}
/*
* General purpose move checker routine.
* Will see if a move is syntactically correct only
* but will not check ownership etc.
*/
int move_syntaxcheck( char *in_line, int precheck, char *out_string )
{
/* Read movement in from input file. */
	char c, order;
	unsigned char *t;
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
	case 'a':
		if (order == 'a' && !(dipent.xflags & XF_AIRLIFTS)) {
		    errmsg("Invalid order for the unit at %s.\n",
		           pr[p1].name);
	            return E_WARN;
		}
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
		if (order == 'c' && (water(p2) || (HAS_PORTAGE && !LandLocked(p2)))) {
			errmsg("The convoy order should specify source %s\n",
			 "and final destination.");
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
				    errmsg("Multi-hop convoys are not allowed with A/F rules.\n");
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
		if (!(dipent.x2flags & X2F_PROXY)) {
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
		if (c== 'R' && !(dipent.x2flags & X2F_ARTILLERY)) {
                 errmsg("This game does not allow artillery.\n");
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
	char c, order;
	unsigned char *t;
	int i, j, p1, p2, p3, u, u1, u2, c1, c2, c3, bl, igw=-1, irw=-1;
	int railway_flag = 0;
	unsigned char *bp;
	char temp_text[256];
	int dummy;
 
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


	if (railway(p1)) {
		errmsg("Cannot start order with railway");
		return E_WARN;
	}

	u1 = pr[p1].unit;
	    
	if (gateway(p1))
		igw = pr[p1].type2; 

	u = u1;
	/* p3 and c3 initialisation */
	p3 = p1;
	c3 = c1;
	if (!u) {
		errmsg("No %s present %s %s.\n", utype(c),
		 water(p1) ? "in the" : "in", pr[p1].name);
		return E_WARN;
	}
	u = GetUnitIndex(p1, p); /* return next unit if multi-unit province */

	unit[u].railway_index = -1;  /* to reset */
	if (p != unit[u].owner && p != MASTER && !(IS_DUPLEX(dipent) 
					&& (unit[u].controller == 0 || unit[u].controller == p))) {
		if (!((dipent.x2flags & (X2F_PROXY)))) {
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
			unit[u2].railway_index = -1;
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

	if (gateway(p1) && order != 's' && order != 'h' && order != 'n') {
	    errmsg("Gateway can only be ordered to support or hold");
	    return E_WARN;
	} 
	switch (order) {
	case 'c':
	case 's':
	case 'a':
		if (order == 'a' && !(dipent.xflags & XF_AIRLIFTS)) {
		    INVALID_ORDER(p1,u);
		}
		if (unit[u].type == 'T' && order == 'c') {
		    errmsg("Army/Fleets cannot convoy.\n");
		    return E_WARN;
		}
		*s = get_type(*s, &c);
		*s = get_prov(*s, &p3, &c3);
		p2 = p3;
		c2 = c3;
		if (!p2) {
			errmsg("Unrecognized source province for support/convoy -> %s", *s);
			return E_WARN;
		}

		u2 = pr[p2].unit;

		if (highsea(p2)) {
		    errmsg("Cannot support units moving from High Seas.\n");
		    return E_WARN;
		}

		if (!(dipent.flags & F_BLIND)) {
			if (!(u2)) {
				errmsg("No unit present %s %s.\n",
				 water(p2) ? "in the" : "in", pr[p2].name);
				return E_WARN;
			}
			if ((order == 'a' || order == 'c') &&
			    unit[u2].type != 'A' && unit[u2].type != 'R' && 
			    !(HAS_PORTAGE && unit[u2].type == 'F')) {
				errmsg("The specified unit is of a type that cannot be %sed.\n",
				 order == 'c' ? "convoy" : "airlift");
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
		if (order == 'c' && !(unit[u].type == 'F' || (HAS_PORTAGE && unit[u].type == 'A'))) {
			errmsg("The %s in %s can't convoy anything!!\n",
			 utype(unit[u].type), pr[p1].name);
			return E_WARN;
		}
		if (order == 'a' && unit[u].type != 'W') {
			errmsg("The %s in %s can't airlift anything!!\n",
			utype(unit[u].type), pr[p1].name);
			return E_WARN;
		}

		if (order == 'c' && water(p2)) {
			errmsg("The convoy order should specify source %s\n",
			 "and final destination.");
			return E_WARN;
		}
		if (order == 'c' && !StrictConvoy(p2)) {
                    errmsg("The destination %s is landlocked and cannot be convoyed to.",
                    pr[p2].name );
                    return E_WARN;
                }
		c1 = 0;
		if (!gateway(p1) && (order == 's' &&
		 (!valid_artillery_move(u, p2, &c1, &bl) || c1 == MX))) {
			errmsg("The %s %s %s can't get to %s%s to support.\n",
			 utype(unit[u].type),
			 mov_type(p1, u), pr[p1].name,
			 water(p2) ? "the " : "", pr[p2].name);
			return E_WARN;
		}
		if (unit[u].type == 'R' && order == 's' && !valid_move(u, p2, &c1, &bl)) {
		    /* Artillery is giving a support over two spaces 
		     * Check that it is supporting an attack */
		    if (c != 'm') {
			errmsg("Artillery can only support %s to attack.\n",
			       pr[p2].name);
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
		if (railway(p2)) {
		    irw = pr[p2].type2;
		    if (unit[u].type != 'A') {
			errmsg("Only armies can use railways.\n");
			return E_WARN;
		    }
		    if (!ValidRailwayProvince(irw, p1,&dummy)) {
			errmsg("Source province is not on railway.\n");
			return E_WARN;
		    }
		    if(!PermittedRailwayPower(irw, pletter[dipent.variant][unit[u].owner])) {

			errmsg("Power is not allowed to use railway.\n");
			return E_WARN;
		    }
		}
		t = get_action(*s, &c);
		/*if (railway(p2)) {
		    if (c != 'm') {
			errmsg("Armies must move off railway - order failed.\n");
			return E_WARN;
		    } else {
			*s = get_prov(t, &p3, &c3);
			if (!ValidRailwayProvince(irw, p3,&dummy)) {
			    errmsg("Destination province is not on railway.\n");
			    return E_WARN;
			}
			if (p2 == p3) {
			    errmsg("Cannot get on and off of railway at same province.\n");
			    return E_WARN;
			}
		    }
		} else*/
		  if (c == 'm') {
			if (unit[u].type == 'A' || (HAS_PORTAGE && unit[u].type == 'F')) {
				i = nunit + 1;
				unit[i].loc = unit[u].loc;
				unit[i].coast = XC;
				unit[i].railway_index = -1;
				bp = &heap[hp];
				while (c == 'm' && !railway_flag) {
					if (!railway(p2) &&(!valid_aw_move(i, p2, &c2, &j) || !convoyable(p2) ||
					 (!(u2 = pr[p2].unit) || !CheckValidACUnit(u2)))) {
						/*
						 * Tim Miller -- don't let blind
						 * players use convoys to test if fleets 
						 * exist in a particular body of water.
						 */
					 	if(!(dipent.flags & F_BLIND))
						{	
							errmsg("The %s in %s can't convoy through %s%s.\n",
							 utype(u2), pr[p1].name, water(p2) ? "the " : "", pr[p2].name);
							return E_WARN;
						}
					}
					if (j && !bl)
						bl = j;

					/*
					 * Tim Miller: convoy rewrite.
					 */
					heap[hp++] = p2;

					unit[i].loc = p2;
					if (railway(p2)) railway_flag = 1;
					p3 = p2;  /* Remember previous province */
					*s = get_prov(t, &p2, &c2);
					if (!p2) {
						errmsg("Movement from %s%s to unrecognized province -> %s",
						 water(p1) ? "the " : "", pr[p1].name, *s);
						return E_WARN;
					}
					t = get_action(*s, &c);
					/* Check that not disallowed for Army/Fleet rules */
					if (!railway_flag && ((dipent.flags & F_AFRULES) && c == 'm')) {
                                            errmsg("Multi-hop convoys are not allowed with A/F rules.");
                                            return E_WARN;
                                        }
					if (railway_flag && !ValidRailwayProvince(irw, p2,&dummy)) {
                            			errmsg("Destination province is not on railway.\n");
                            			return E_WARN;
                        		}
				}
				if (unit[u].type == 'F' && LandLocked(p2)) {
				    errmsg("Fleet cannot end up in landlocked province %s.\n", pr[p2].name);
				    return E_WARN;
				}
				c3 = MV;
				if (!railway_flag && (!valid_aw_move(i, p2, &c3, &j))) {
					errmsg("The %s in %s can't convoy from %s to %s.\n",
					 utype(unit[u].type), pr[p1].name, pr[unit[i].loc].name, pr[p2].name);
					return E_WARN;
				}
				if (water(p2)) {
					errmsg("The %s in %s can't convoy into the %s.\n",
					 utype(unit[u].type),
					 pr[p1].name, pr[p2].name);
					return E_WARN;
				}
				heap[hp++] = 0;
				if (unit[u].type != 'F' )
				    c2 = MV;  /* Fleets will retain their specified coast */
				else {
				    /* It is a fleet movement, let's check that the fleet's going to end up
				     * on a valid coast */
				    if ((c2 == MV && pr[p2].coasts) || (c2 != MV && !pr[p2].coasts) ) {
					errmsg("Province has %scoasts - %s specify one.\n",
					       c2 == MV ? "" : "no ",
					       c2 == MV ? "please" : "do not");
					return E_WARN;
				    }
					       
				    if (!HasCoast(p2, c2)) {
					errmsg("Invalid coast specified for %s.\n", 
					       pr[p2].name);
					return E_WARN;
				    }
				}
				
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
			if (unit[u].type != 'F')
			    c2 = MV;  /* Non-fleets cannot move to coasts */
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
		if (!(dipent.x2flags & X2F_PROXY)) {
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
                    errmsg("Unknown unit type to transform to.\n");
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
                    if (((unsigned char) *++t) >> 4 == c1)
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

	case 'e':  /* Embark order, AF only */
		if (!(dipent.flags & F_AFRULES)) {
		    INVALID_ORDER(p1, u);
		}
		/* Unit needs to be an army, trying to get onto a fleet */
		if (unit[u].type != 'A') {
		    errmsg("Only armies can embark.\n");
		    return E_WARN;
		}

		/* Check that destination unit is a fleet */
		*s = get_type(*s, &c);
                *s = get_prov(*s, &p3, &c3);
                p2 = p3;
                c2 = c3;
		u2 = pr[p2].unit;
		if ((!(dipent.flags & F_BLIND) && (unit[u2].type != 'F' || !u2)) || (c != 'F' && c != 'x' )) {
		    errmsg("Can only embark onto fleets.\n");
		    return E_WARN;
		} 
 
		break;

	case 'k':  /* disembark order, AF only */
		if (!(dipent.flags & F_AFRULES)) {
		    INVALID_ORDER(p1,u);
		}
		/* Unit needs to be a fleet or army/fleet */
		if (unit[u].type != 'T' && unit[u].type != 'F' ) {
		    errmsg("Cannot disembark from non-fleet units.\n");
		    return E_WARN;
		}

		*s = get_prov(*s, &p3, &c3);
	
		if (!water(p1)) {
		    errmsg("%s is not a water province to disembark from",
			pr[p1].name);
		    return E_WARN;
		}	
		c3 = XC;  /* Any coast, NOT army movement */
		if (!valid_move(u, p3, &c3, &bl)) {
		    errmsg("%s is not adjacent to %s to disembark to.\n",
			pr[p3].name, pr[p1].name);
		    return E_WARN;
		}
		if (water(p3)) {
		    errmsg("%s is not a land province for disembarking to.\n",
			pr[p3].name);
		    return E_WARN;
		};

		p2=p3; c2=c3;
		break;

	default:
		INVALID_ORDER(p1,u);

	}
	if (gateway(p1)) {
            switch (order) {
                case 's':
                    /* OK, gateway has a support order: see if for valid province movement or support */
                    if (ValidGatewayProvince(igw, p2) && ValidGatewayProvince(igw, p3) ) {
			/* All ok, fall through */
                    } else {
                        errmsg("Invalid provinces for gateway.\n");
                        return E_WARN;
		    }
                    break;
		case 'h':
		case 'n':
		    break;

                default:
                    errmsg("Invalid order for gateway.\n");
                    return E_WARN;
            }

	}
	unit[u].order = order;
	unit[u].unit = u2;
	unit[u].unit_prov = p3;
	unit[u].unit_coast = c3;
	unit[u].dest = p2;
	unit[u].dcoast = c2;
	unit[u].convoy = bp;
	unit[u].bloc = bl;
	if (railway_flag)
	    unit[u].railway_index = irw;

	return 0;
}

	static int bounce;
        static int rw_used[MAX_RAILWAYS]; /* check if railway being used mroe than once */
#define supval(u) (unit[u].stype == 'm' || unit[u].stype == 'p' ? 2 : 1)
#define VOID 1
#define NO_CONVOY 2
#define CUT 3
#define BOUNCE 4
#define NO_SUPPORT 5
/* #define BESIEGE 6 */
/* #define DOSIEGE 7 */
#define BLOCKED  8
#define BAD_CONVOY 9
#define BAD_AIRLIFT 10
#define DISRUPTED 11
#define DISLODGED 32    /* added on, used as bit mask */
#define MAYBE_NO_CONVOY 33      /* no message */
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
         "WARN: check convoy",
	 "WARN: check airlift",
	 "disrupted" };

static int result[MAXUNIT];
static float support[MAXUNIT];
static int fail[NPROV+1]; /* Marked as failed when attacked, useful for transforms only */
static unsigned char  contest[NPROV + 1];
static unsigned char last_type[MAXUNIT];

/**** called to actually do move processing ****/
static void DoMoves( void)
{
        int u, u2, u3, u4, p, igw;
	unsigned char *s;
	int unit_dislodged;
	int unsupported_native[MAXUNIT];
	int c1, bl;

/* Pass 0a: See if railway being used more than once and fail the moves if so */

    for (u=0; u < nrw; u++)
	rw_used[u] = 0;

    for (u = 1; u <= nunit; u++) {
        if (unit[u].railway_index >= 0 && unit[u].owner)
            rw_used[unit[u].railway_index]++;
    }

   for (u = 1; u <= nunit; u++) {
	last_type[u] = '\0';  /* Empty the array first */
        if (unit[u].railway_index >= 0 && unit[u].owner)
            if (rw_used[unit[u].railway_index] > 1)
                result[u] = BLOCKED;  /* Mark duplicate uses as BLOCKED moves */

        if (IsGatewayOrder(u, &igw)) {
            if (!IsGatewayOrdered(igw,u, result))
                result[u] = BLOCKED;
        }

	/* See if native with no support */

	unsupported_native[u] = 0;
        if (dipent.has_natives && unit[u].owner == dipent.has_natives ) {
	    unsupported_native[u] = 1;
	    for (u2 = 1; u2 <= nunit && unsupported_native[u]; u2++) {
		if (unit[u2].order == 's' && 
		    unit[u2].unit == u && 
		    unit[u2].dest == unit[u].loc)
		        unsupported_native[u] = 0;
	    }
        }

	/* See if trying to support a unit over a half coast (not allowed) */
	if (unit[u].order == 's') {
	    c1 = 0;
	    valid_move(u, unit[u].dest, &c1, &bl);  /* To get the coast to dest prov */
	    if (c1 == HX || c1 == LX)
	        result[u] = BLOCKED;
	}

		    
    }

/* Pass 0a, void all supports for unit to/in multi-unit provinces */

	for (u = 1; u <= nunit; u++) {
        	if (unit[u].owner <= 0)
                                continue;
                if (unit[u].order == 's' && 
		    IsMultiProvince(unit[unit[u].unit].loc))
		    result[u] = VOID;

		if (unit[u].type == 'R' && unit[u].order == 'm')
		    support[u] = -1;  /* Moving artillery unit has no value in attacks */
	}
	

/* Pass 1: Tally up all the support orders, verify convoys. */
                for (u = 1; u <= nunit; u++) {
                        if (unit[u].owner <= 0)
                                continue;
                        if (unit[u].order == 's' && !result[u]) {
                                if (unit[u2 = unit[u].unit].order == 'm') {
                                        if (unit[u2].dest == unit[u].dest &&
                                         (unit[u2].dcoast == unit[u].dcoast || unit[u2].dcoast <= XC)) {
                                                if (!result[u2] && unit[u].type != ' ' && unit[u2].type != 'R')
                                                        support[u2] += supval(u);
                                        } else {
                                                result[u] = VOID;
                                        }
                                } else {
                                        if (unit[u2].loc == unit[u].dest) {
					    if (unit[u].type != ' ')
                                                support[u2] += supval(u);
					} else {
                                                result[u] = VOID;
                                        }
                                }

                        } else if (unit[u].order == 'm' && unit[u].convoy != NULL) {
                                for (s = unit[u].convoy; *s; s++) {
					if (NoValidConvoyingFleet(s, u)) {
                                            if (pr[*s].type != 'r') {
                                                /* only disallow if not a railway unit involved */
                                                result[u] = NO_CONVOY;
                                                support[u] = supval(u) - 1;
                                                break;
                                            }
                                        }
                                        if ( HasUnit(unit[u].dest) && unit[pr[unit[u].dest].unit].owner != unit[u].owner) {
                                         /* Non owner attacked this unit, so set the fail flag */
                                         if (dipent.xflags & XF_NOATTACK_TRANS)
                                         if (dipent.xflags & XF_TRANS_MANYW)
                                                 fail[unit[u].dest] = 'f';
                                        }
					/* See if convoying to half area */
					c1=0;
					valid_move(*s, unit[*s].dest, &c1, &bl);
					if (c1 == HX) support[u] -= 0.5;
					last_type[u] = unit[*s].type; /* Remember last unit convoying/airlifting */
                                }
                        } else if (unit[u].order == 'm' ) {

                            if (HasUnit(unit[u].dest) && unit[pr[unit[u].dest].unit].owner != unit[u].owner) {
                                         /* Non owner attacked this unit, so set the fail flag */
                                         if (dipent.xflags & XF_NOATTACK_TRANS)
                                         if (dipent.xflags & XF_TRANS_MANYW)
                                                 fail[unit[u].dest] = 'f';
                                        }

			/***
                            if (IsGatewayOrder(u, &igw)) {
                                if (!IsGatewayOrdered(igw,u))
                                    result[u] = BLOCKED;
                            }
			 ***/

                        }
                }

/* Pass 2aa, void all supports for airlifts */

        for (u = 1; u <= nunit; u++) {
            if (unit[u].owner <= 0)
                   continue;
            if (unit[u].order == 's' && 
                last_type[unit[u].unit] == 'W' ) {
		   if (!result[u]) {
		       support[unit[u].unit] -= supval(u);
		       result[u] = VOID;
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
                         && unit[u2].dest != unit[u].loc        /* X */
                         && unit[u2].owner != unit[u].owner) {  /* IX.6.note */
			     if (unit[u].dcoast != LX && unit[u].dcoast != HX &&
				!IsMultiProvince(unit[u2].loc)) {
                                result[u2] = CUT;
                                support[unit[u2].unit] -= supval(u2);
			     }
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
                                 && unit[u2].dest != unit[u].loc        /* X */
                                 && unit[u2].owner != unit[u].owner) {  /* IX.6.note */
				    if (unit[u].dcoast != LX && unit[u].dcoast != HX &&
				    !IsMultiProvince(unit[u2].loc)) {
                                        result[u2] = CUT;
                                        support[unit[u2].unit] -= supval(u2);
				    }
                                }
                        }
                }
/* Pass 3a: Check for dislodged convoys. XII.3 */
                for (u = 1; u <= nunit; u++) {
                        if (unit[u].owner <= 0)
                                continue;
                        if (unit[u].order == 'c' && !result[u2 = unit[u].unit]) {
                                for (s = unit[u2].convoy; s != NULL && *s; s++) {
					/*
					 * convoy clean up: s is an entry in the
					 * province table, so get the actual ordinal
					 * of the convoying unit (cunit).
					 */
					int cunit = pr[*s].unit; 

                                        if (cunit == u) {
                                                for (u3 = 1; u3 <= nunit; u3++) {
                                                        if (unit[u3].owner <= 0)
                                                                continue;
                                                        if (unit[u3].order == 'm' && !result[u3] &&
                                                         unit[u3].owner != unit[u].owner &&
                                                         unit[u3].dest == unit[u].loc && support[u3] > support[u]) {
                                                                result[u2] = MAYBE_NO_CONVOY;
                                                                goto nextp3a;
                                                        }
							if ((dipent.x2flags & X2F_RIVERS) && (
							unit[u3].order == 'm' && !result[u3] &&
                                                         unit[u3].owner != unit[u].owner &&
                                                         unit[u3].dest == unit[u].loc && unit[3].type == 'F') &&
							 dual_land(unit[u].loc)) {
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
                         && unit[u2].dest != unit[u].loc        /* X */
                         && unit[u2].owner != unit[u].owner) {  /* IX.6.note */
/* XII.5: You can't cut support of attacks against your convoy */
                                for (s = unit[u].convoy; s != NULL && *s; s++)
                                        if (unit[u2].unit == pr[*s].unit)
                                                goto nextp2b;
                                if (unit[u].dcoast != HX && !IsMultiProvince(unit[u2].loc)) {
				    result[u2] = CUT;
                                    support[unit[u2].unit] -= supval(u2);
				}
                        }
                 nextp2b:;
                }
/* Pass 3b: Recheck for dislodged convoys. XII.3 */
                for (u = 1; u <= nunit; u++) {
                        if (unit[u].owner <= 0)
                                continue;
                        if (unit[u].order == 'c' && result[u2 = unit[u].unit] == MAYBE_NO_CONVOY) {
                                for (s = unit[u2].convoy; s != NULL && *s; s++) {
                                        if (pr[*s].unit == u) {
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
								if (IsMultiProvince(unit[u].loc))
									continue;
                                                                result[u] = DISLODGED;
                                                                result[u2] = NO_CONVOY;
                                                                support[u2] = supval(u2) - 1;
                                                                goto nextp3b;
                                                        }
                                                }
						for (u3 = 1; u3 <= nunit && dipent.x2flags & X2F_RIVERS; u3++) {
                                                       if (unit[u3].owner <= 0)
                                                                continue;
                                                        if (unit[u3].order == 'm' && !result[u3] &&
                                                         unit[u3].owner != unit[u].owner &&
                                                         unit[u3].dest == unit[u].loc && unit[u3].type == 'F' &&
							 dual_land(unit[u].loc)) {
                                                                for (u4 = 1; u4 <= nunit; u4++) {
                                                                        if (u4 == u3)
                                                                                continue;
                                                                        if (unit[u4].order == 'm' &&
                                                                         unit[u4].dest == unit[u].loc &&
                                                                         unit[u4].type == 'F')
                                                                                break;
                                                                }
                                                                if (u4 <= nunit)
                                                                        continue;
                                                                if (IsMultiProvince(unit[u].loc))
                                                                        continue;
                                                                result[u] = DISRUPTED;
                                                                result[u2] = NO_CONVOY;
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
                                 && unit[u2].dest != unit[u].loc        /* X */
                                 && unit[u2].owner != unit[u].owner) {  /* IX.6.note */
/* XII.5: You can't cut support of attacks against your convoy */
                                        for (s = unit[u].convoy; s != NULL && *s; s++)
                                                if (unit[u2].unit == pr[*s].unit)
                                                        goto nextp2c;
					if (unit[u].dcoast != HX && !IsMultiProvince(unit[u2].loc)) {
                                            result[u2] = CUT;
                                            support[unit[u2].unit] -= supval(u2);
					}
                                }
                        }
                 nextp2c:;
                }
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
         			if (support[u]-p > support[u2]) {
           			    for (u4=1, unit_dislodged=1; u4<=nunit; u4++ ) {
             			        if ( ( u != u4 )
               			        && ( unit[ u4 ].order == 'm' )
               			        && ( !result[ u4 ] )
               			        && ( unit[ u4 ].dest == unit[ u ].dest )
               			        && ( support[ u4 ] >= support[u]-p ) )
                 			    unit_dislodged = 0;
           			    }

           			    if ( unit_dislodged == 1 && !IsMultiProvince(unit[u2].loc) ) {
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
                                        if (!((u2 = pr[unit[u].dest].unit) && !result[u2]       /* IX.7.note */
                                         &&unit[u2].order == 'm'
                                         && unit[u2].dest == unit[u].loc
                                         && !unit[u2].convoy && !unit[u].convoy)) {     /* XIV.6 */
					    if ((unit[u2].railway_index == -1 || unit[u2].fallback == 0 ))
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
                                                if ((unit[u2].order != 'm' && unit[u2].order != 'd')
                                                 || (unit[u2].dest == unit[u].loc
                                                        && !unit[u2].convoy && !unit[u].convoy)) {      /* XIV.6 */
                                                        if (support[u] - p <= support[u2] - unsupported_native[u2]
                                                         || unit[u].owner == unit[u2].owner) {  /* IX.3 */
							    /* Only bounce if not multi-province or 
							       a railways with no fallback */
							    if (!IsMultiProvince(unit[u].dest) && 
								  (unit[u2].railway_index == -1 ||
								   unit[u2].fallback == 0)) {
                                                                bounce++;
                                                                result[u] = BOUNCE;
                                                                goto nextp5;
							    }
                                                        }
                                                        /*
                                                         ** ...or if the unit's movement bounced and we have no support.
                                                         */
                                                } else if (result[u2]
                                                         && ((support[u] - p < supval(u2))
                                                         || unit[u].owner == unit[u2].owner)) { /* IX.3 */
						    if (!IsMultiProvince(unit[u].dest) &&
                                                                  (unit[u2].railway_index == -1 ||
                                                                   unit[u2].fallback == 0)) {
                                                        bounce++;
                                                        result[u] = BOUNCE;
                                                        goto nextp5;
						    }
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
							    if (!IsMultiProvince(unit[u].dest)) {
                                                                bounce++;
                                                                result[u] = BOUNCE;
                                                                goto nextp5;
							    }
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
                                ((unit[u2].order != 'm' && unit[u2].order != 'd') || result[u2])) {
				    if (!IsMultiProvince(unit[u2].loc) && 
					(unit[u2].railway_index == -1 || unit[u2].fallback == 0)) {
                                        if (result[u2] < DISLODGED)
                                                result[u2] += DISLODGED;
                                        unit[u2].status = 'r';
                                        bounce++;
				    }
                                }
                        }
                }
}

/* Check if railway move requested is to be blocked */
int RailwayMoveCut( int u)
{
    /* Moves on a railway are blocked if a non-owner unit is on the line, 
     * OR a non-owner move to destination will be successful
     */

    int diff, source_pr, dest_pr, i, u1;
    int irw = unit[u].railway_index;

    if (irw < 0) return 0;  /* wasting my time! */

    ValidRailwayProvince(irw, unit[u].loc, &source_pr);
    ValidRailwayProvince(irw, unit[u].dest, &dest_pr);

    diff = source_pr < dest_pr ? 1: -1;

    for (i = source_pr; i != dest_pr; i=i+diff) {
	if (i==source_pr) continue;  /* No need tolook at where I am! */
	
	for (u1=1; u1 <= nunit; u1++) {
	    if (u == u1) continue;  /* Don't look at me! */
	    if (unit[u].owner != unit[u1].owner && 
                  ((unit[u1].loc == rw[irw].pr[i] && 
		    unit[u1].status == ':' &&
		    unit[u1].order != 'm') ||
		   (unit[u1].dest ==  rw[irw].pr[i] && !result[u1]))) {
		/* OK; we've got a unit either here and not of mtype OR
		  going here and not of my type */

		/* So, let's order dummy unit to move there, to fake bounce */
		unit[pr[rw[irw].prov_index].unit].dest = rw[irw].pr[i];
	        unit[pr[rw[irw].prov_index].unit].order = 'm';
		unit[pr[rw[irw].prov_index].unit].owner = AUTONOMOUS; /* Just so it is calculated in */
		return 1;
	    }
	}
    }
    return 0;
}

/* Check if railway has a free province to retreat to */
int HasFreeRWProvince( int u, int *p, int set)
{
    /* Moves on a railway are blocked if a non-owner unit is on the line,
     * OR a non-owner move to destination will be successful
     */

    int diff, source_pr, dest_pr, i, u1;
    int irw = unit[u].railway_index;
    int possible_free, friendly_blocker;

    *p = -1;  /* Default, bad value */
    if (irw < 0) return 0;  /* wasting my time! */

    ValidRailwayProvince(irw, unit[u].loc, &source_pr);
    ValidRailwayProvince(irw, unit[u].dest, &dest_pr);

    diff = source_pr < dest_pr ? 1: -1;

    possible_free = 1;
    for (i = source_pr; i != dest_pr && possible_free; i=i+diff) {
        if (i==source_pr) continue;  /* No need to look at where I was going to! */
	friendly_blocker = 0;
        for (u1=1; u1 <= nunit && possible_free; u1++) {
            if (u == u1) continue;  /* Don't look at me! */
            if (((unit[u].owner != unit[u1].owner) &&
                  (unit[u1].loc == rw[irw].pr[i] && 
		   unit[u1].status == ':' &&
		   ((unit[u1].order != 'm' && unit[u1].order != 'd') || (unit[u1].order == 'm' && result[u1])))) ||
                   (unit[u1].dest ==  rw[irw].pr[i] && !result[u1]) ||
		   (unit[pr[rw[irw].prov_index].unit].dest == rw[irw].pr[i])) {
                /* OK; we've got a unit either here and not of mtype OR
                  someone tried to go here (with sucess) OR
		  the fake unit went here (indicating a detected cut)  */
                possible_free = 0;  /* No, it is occupied */
            }
	    if (possible_free && 
		unit[u].owner == unit[u1].owner &&
                unit[u1].loc == rw[irw].pr[i] &&
                unit[u1].status == ':' &&
                ((unit[u1].order != 'm' && unit[u1].order != 'd') || (unit[u1].order == 'm' && result[u1]))) {
		/* If it looks free, but power has a unit still there, mark as blocked by friendly */
		friendly_blocker = 1;
	    }
        }
	if (possible_free && !friendly_blocker) {
	    /* If still free, it is free! save free value */
	    *p = rw[irw].pr[i];
	}
	if (unit[u].owner == unit[u1-1].owner)
	    possible_free = 1;  /* same owner means not cut, carry on */
    }
    if (*p == -1)
    	return 0;
    else
    {
	if (set) {
	    unit[u].dest = *p;
	    unit[u].status = ':';
	    result[u] = 0;
	}
    }
	return 1;
}

/****************************************************************************/
int moveout(int pt)
{
/* Process movement orders. */
	int u, u2, i, index, p, igw;
	unsigned char *s, *t, c;
	int c1;
	char cbuffer[1024];
	int new_result[MAXUNIT]; 
	int rw_print = 0;

	bounce = 0;
	for (p = 1; p <= npr; p++) {
		contest[p] = 0;
		fail[p] = 0;
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
		unit[u].fallback = 0;  /* Initialise this first */
		result[u] = 0;
		/*** Not sure why this was here, but it messed up land-units
		 giving support to fleets to a costal region
		 MLM 28/09/2000
		if (unit[u].type != 'F' && unit[u].order != 't')
unit[u].dcoast = 0;***/ /* non-fleets not transforming have no coast */
		if (unit[u].type != 'F')
			unit[u].coast = 0; /* non-fleets not transforming have no coast */
			
		support[u] = unit[u].dcoast == MX ? -1 : supval(u) - 1;
		/* Now take away half if support for a 1/2 value movement */
		support[u] = unit[u].dcoast == HX ? support[u] - 0.5 : support[u]; 
		support[u] = unit[u].dcoast == LX ? support[u] - 0.5 : support[u];
	}

	for (u = 0; u < nrw; u++)
	    rw_used[u] = 0;
	/*
	 ** If we're not really processing, clear all the bogus proxy orders and
	 ** skip to the report generation.
	 */

#define unit_space(u)  (unit[u].type == ' ' ? "" : " ")


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

				fprintf(rfp, "%s: %s%s%s%s", powers[p = unit[u].owner], Stype(unit[u].stype),
					Utype(unit[u].type), unit_space(u), pr[unit[u].loc].name);

				if (unit[u].coast > XC && unit[u].coast != HX && unit[u].coast != LX)
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
						unit[u].unit_prov = unit[u2].unit_prov;
						unit[u].controller = unit[u2].controller;
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
 
	DoMoves();   /** All the logic to work out the moves **/

	/* Now recheck the railways, adjusting those that have failed */

	if (nrw > 0 && (dipent.x2flags & X2F_RAILWAYS)) {
	    /*
             * Railway moves will fail if any intermediate point is occupied or successfully attacked
             * by non-mover country
             */
	
	    for (u = 1; u <= nunit; u++) {
		support[u] = 0;  /* Zero the support as it will be redone */
		new_result[u] = 0;
	    }

	    for (u = 1; u <= nunit; u++) {
		if (unit[u].railway_index >= 0 && (result[u] % DISLODGED) != BLOCKED) {
		    /* OK, we've got a valid railway move, see if prematurely cut */
  		    if (RailwayMoveCut(u)) {
			new_result[u] = BOUNCE;
			support[u] = -1; /* Unit no longer counts for result purposes */
					 /* The railway unit instead will bounce if required */
		    }
		}
		if (gateway(unit[u].loc)) {
		    /* OK, got a gateway, see if ordering */
		    if (unit[u].order == 's') {
			/* Ordering, so see if related province will be emtpy or dislodged */
			igw = pr[unit[u].loc].type2;
			i = gw[igw].gw_prov;
			if (unit[pr[i].unit].status == 'r' ||
			    (unit[pr[i].unit].order == 'm' && !result[pr[i].unit])) {
			    new_result[u] = VOID;  /* No longer got gateway unit in place! */
			}
		    }
		}
	    }
	
	    /* OK, now reinitialise data arrays before reprocessing */
	    bounce = 0;
	    for (u=1; u <= nunit; u++) {
		unit[u].status = ':';
		result[u] = new_result[u];
            }	
	    /* Bug 273, reset fail and contest arrays */
	    for (p = 1; p <= npr; p++) {
	        contest[p] = 0;
		fail[p] = 0;
	    }
	
	    DoMoves();  /** Recalculate moves after railway failure included **/
	
	    for (u=1; u <= nunit; u++) {
		if (unit[u].railway_index >= 0 && (result[u] == BOUNCE || new_result[u] == BOUNCE)) {
                    /* Set up where railway unit will go to, if possible */
                        HasFreeRWProvince(u, &unit[u].fallback, 0);
                }
	    }
	}

    }


/* OK, let's take out any dummy railway units that were made autonomous */
    for (i = 0; i < nrw; i++)
        unit[pr[rw[i].prov_index].unit].owner = 0;  /* Now not owned, forced! */


/* Pass 6a. Convert fleets to Army/fleet if unit could embark */

        for (p = 0, u = 1; u <= nunit && (dipent.flags & F_AFRULES); u++) {
                if (unit[u].owner <= 0)
                        continue;
                if (processing || pt == unit[u].owner || pt == MASTER) {
		   if (unit[u].order == 'e' && unit[pr[unit[u].dest].unit].type == 'F' &&
		      unit[u].owner == unit[pr[unit[u].dest].unit].owner)
			unit[pr[unit[u].dest].unit].type = 'T';
		}
	}

/* Pass 6b, update unit_held: if unit moved away, set to zero */
        for (u = 1; u <= nunit && processing; u++) {
            if (!result[u]) {
                switch (unit[u].order)
                    case 'm':
                    case 'd':
                    case 'e':
                    case 'k':
                        pr[unit[u].loc].unit_held = 0;
            }
        }

/* Pass 6: Process movement, print report */
	for (p = 0, u = 1; u <= nunit; u++) {
		if (unit[u].owner <= 0)
			continue;
		if (processing || pt == unit[u].owner || pt == MASTER ||
		    (IS_DUPLEX(dipent) && unit[u].controller == pt)) {
			if (unit[u].owner != p)
				fprintf(rfp, "\n");
			if ((unit[u].owner != p) && (pt == MASTER)) {
				mast_rpt(unit[u].owner, 0);
			};
			fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
			if (unit[u].proxy != 0)
				fprintf(rfp, "%s ", owners[unit[unit[u].proxy].owner]);
			fprintf(rfp, "%s%s%s%s", Stype(unit[u].stype),
			 Utype(unit[u].type), unit_space(u), pr[unit[u].loc].name);

			if (unit[u].coast > XC)
				fprintf(rfp, " (%s)", mtype[unit[u].coast]);
			if (isNativeUnit(u)) 
			    unit[u].order = 'h';  /* Native Units always hold */

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
				fprintf(rfp, "%s %s -> %s", Utype(unit[u2].type), pr[unit[u].unit_prov].name,
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
					/*
					 * Tim Miller: convoy clean-up -- the convoy
					 * list now has provinces, not units.
					 */
					while(*s) {
						/*
						 * unum holds the unit ordinal of the unit
						 * in the province we convoy through, or 0
						 * if no unit.
						 */
						int unum = pr[*s].unit; 

						/*
						 * If this game is not blind, we can warn
						 * players about possible bad convoys.
						 */
						if(!(dipent.flags & F_BLIND))
						{
							if (!processing && !predict && unit[unum].owner == unit[u].owner &&
							 (unit[unum].order != 'c' || unit[unum].unit != u ||
							 unit[unum].dest != unit[u].dest)) {
								result[u] = BAD_CONVOY;
							}
						}
						fprintf(rfp, " -> %s", pr[*s].name);
						s++;
					}
				}
				fprintf(rfp, " -> %s", pr[unit[u].dest].name);
				if (unit[u].dcoast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);
				break;
			case 'n':
			/*
				if (unit[u].type == ' ') {
					fprintf(rfp, " HOLD");
				} else
			*/	if (unit[u].owner != AUTONOMOUS && unit[u].owner != NEUTRAL &&
			 	    unit[u].controller != AUTONOMOUS) {
					fprintf(rfp, ", No Order Processed");
					if (dipent.phase[0] != 'F' || pr[unit[u].loc].type != 'v')
						more_orders++;
				} else 
				    fprintf(rfp, " HOLD");  /* For autonomous and neutrals, to hold */

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
				 fprintf(rfp, "%s%s%s", Utype(unit[u2].type), unit_space(u2), pr[unit[u].unit_prov].name);
				else
				 fprintf(rfp, "%s %s", "Unit", pr[unit[u].unit_prov].name);
				if (unit[u].unit_coast > XC)
					fprintf(rfp, " (%s)", mtype[unit[u].unit_coast]);
				if (u2 && unit[u2].loc != unit[u].dest) {
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
				if (unit[u].status != 'r' && !fail[unit[u].loc]) {
				 unit[u].type = unit[u].new_type;
				 unit[u].coast = unit[u].new_coast;
				}
                            break;

			case 'a':
			    fprintf(rfp, " AIRLIFT ");
			    if ((i = unit[u2 = unit[u].unit].owner) != p) {
                                    if (!(dipent.flags & F_BLIND))
                                        fprintf(rfp, "%s ", owners[i]);
                                    else
                                        fprintf(rfp,"unknown ");
                                }
                                else if (!processing && !predict && unit[u2].dest != unit[u].dest)
                                        result[u] = BAD_AIRLIFT;
                                fprintf(rfp, "Army %s -> %s", pr[unit[u].unit_prov].name,
                                        pr[unit[u].dest].name);

			    break;
			case 'e':
			    fprintf(rfp, " EMBARK ");
			    if ((i = unit[u2 = unit[u].unit].owner) != p) {
                                    if (!(dipent.flags & F_BLIND))
                                        fprintf(rfp, "%s ", owners[i]);
                                    else
                                        fprintf(rfp,"unknown ");
                                }
                                fprintf(rfp, "Fleet %s", 
                                        pr[unit[u].dest].name);

			    break;
			case 'k':
			    fprintf(rfp, " DISEMBARK ");
			    fprintf(rfp, "Army %s", pr[unit[u].dest].name);
	
			    break; 
			default:
				fprintf(rfp, " INVALID ORDER (internal error)");
			}
			if (fail[unit[u].loc])
				fprintf(rfp, ". (*failed*)\n");
			else if (result[u]) {
				fprintf(rfp, ". (*%s%s*)\n", results[result[u] % DISLODGED],
					result[u] > DISLODGED ? ", dislodged" : "");
			} else {
				fprintf(rfp, ".\n");
			}
		}
	}

/* and now display the railway movements, for failing railways */
    if ((dipent.x2flags & X2F_RAILWAYS) && (processing || predict))
    for (u = 1; u <=nunit; u++) {
	if (unit[u].railway_index > - 1 ) {
	    if (result[u] && !((result[u] % DISLODGED) == BLOCKED)) { 
		/* OK, unit was not successful and not blocked: need to work out retreat */
	        if (HasFreeRWProvince(u, &p, 1)) {
		    if (!rw_print) {
		        fprintf(rfp, "\n");
			rw_print = 1;
		    }
		    if (result[u] >= DISLODGED) {
			/* If unit was marked as dislodged, it is not now it has moved elsewhere */
			result[u] -= DISLODGED;
			bounce--;
	            }
		    unit[u].dest = p;  /* Mark it to move into free province */
		    fprintf(rfp, "%s: ", powers[unit[u].owner]);
		    fprintf(rfp, "%s%s %s", Stype(unit[u].stype),
                         Utype(unit[u].type), pr[unit[u].loc].name);
                        if (unit[u].coast > XC)
                                fprintf(rfp, " (%s)", mtype[unit[u].coast]);
		    if ((s = unit[u].convoy)) {
                        while (*s) {
                            fprintf(rfp, " -> %s", pr[*s].name);
			    s++;
                        }
                    }
                    fprintf(rfp, " -> %s", pr[unit[u].dest].name);
                    if (unit[u].dcoast > XC)
                        fprintf(rfp, " (%s)", mtype[unit[u].dcoast]);
		    fprintf(rfp,".\n");
		}
	    }
        }
    }

    if (rw_print)
	fprintf(rfp, "\n");
	

/* Print out fleets forced transformed to armies */
    if (dipent.x2flags & X2F_BURN_BOATS) {
	for (p=0, u = 1; u <= nunit; u++) {
                        if (unit[u].owner <= 0)
                                continue;
			if (!(processing || predict || pt == MASTER || pt == unit[u].owner))
			    continue;  /* Not my unit, so don't show! */
	        if (CountCentres(unit[u].owner) == 0 && 
		    !result[u] && 
		    unit[u].order == 'm' &&
		    !water(unit[u].dest) &&
		    unit[u].type == 'F') {
			if (!p) {
			    fprintf(rfp,"\n");
			    p++;
			} 
                        fprintf(rfp, "%s: ", powers[unit[u].owner]);
                        if (unit[u].proxy != 0)
                                fprintf(rfp, "%s ", owners[unit[unit[u].proxy].owner]);
                        fprintf(rfp, "%s%s%s%s", Stype(unit[u].stype),
                         Utype(unit[u].type), unit_space(u), pr[unit[u].dest].name);
                        fprintf(rfp, " TRANSFORMS TO %s.\n", Utype('A'));
		        unit[u].type = 'A';
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
				 (!(dipent.xflags & XF_AUTODISBAND)) &&
				  (unit[u].owner != dipent.has_natives) &&
				  (unit[u].owner != NEUTRAL &&
				   unit[u].controller != AUTONOMOUS) &&
				  (p = *s++) ; 
				 s++) {
					if ((!contest[p] || IsMultiProvince(p))
					&& ((*s >> 4) == unit[u].coast || unit[u].type == 'W' )
				 && (*s & 0x0f) != MX
					 && (!(u2 = pr[p].unit)	/* XI: can't retreat to */
						||((unit[u2].loc != p || IsMultiProvince(p))	/* attackers origin. */
					 && unit[u2].loc != unit[u].loc))
					 && (dipent.phase[0] != 'F' || pr[p].type != 'v')
					 && AllowedGatewayRetreat(u, p)) {
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
