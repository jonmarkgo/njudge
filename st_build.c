/*
   ** $Log$
   ** Revision 1.25  2003/05/14 07:59:24  millis
   ** Antoher build problem resolved (all builds were rejected).
   **
   ** Revision 1.24  2003/05/12 21:43:04  millis
   ** Fixed bug that didn't work out correct builds.
   **
   ** Revision 1.23  2003/05/12 02:38:23  millis
   ** Bug 118 fix, correctly handle transforms for fleets.
   **
   ** Revision 1.22  2003/05/10 00:46:15  millis
   ** Bug 140 fix, display 'orders' when orders and 'results' when results
   **
   ** Revision 1.21  2003/05/04 22:39:45  millis
   ** Fixed build counting in ExtraCentres() game
   **
   ** Revision 1.20  2003/05/03 22:45:32  millis
   ** take account of ExtraCentres() if set.
   **
   ** Revision 1.19  2003/05/02 21:39:33  millis
   ** Added handling of assumed home centres
   **
   ** Revision 1.18  2003/02/09 23:07:09  millis
   ** Corrected order of GetUnit() call
   ** Also removed superfluous blank line on order output
   **
   ** Revision 1.17  2003/02/05 01:10:00  millis
   ** Fixed small bug with pending build unit count
   **
   ** Revision 1.16  2003/01/18 23:46:03  millis
   ** Integrated USTV changes.
   **
   ** Revision 1.15  2003/01/18 15:17:28  millis
   ** first intermediate checkin, with Native and Multi-province support
   **
   ** Revision 1.14  2002/12/12 01:46:31  millis
   ** Fixed Bug 51 (Suez counting as an owned province)
   **
   ** Revision 1.13  2002/06/10 22:07:02  millis
   ** Small fix to init_build(), that was counting gateway units as units
   **
   ** Revision 1.12  2002/05/17 11:34:26  miller
   ** Fixed small bug for CheckOwnedOK() returning bad value
   **
   ** Revision 1.11  2002/05/11 09:15:34  greg
   ** Minor bug fixes
   ** - fixed subjectline for absence requests
   ** - fixed phase length, so it's no longer hard coded for responses
   ** - partial fix for unusable builds, players with only unusable builds
   **    will no longer be flagged as having orders due, however players
   **    with some usable builds will need to waive any unusable builds,
   **    also, if one or more players have unusable builds, but no
   **    player has usable builds, the build phase will process after
   **    a short delay
   **
   ** Revision 1.10  2002/04/15 12:55:46  miller
   ** Multiple changes for blind & Colonial & setup from USTV
   **
   ** Revision 1.9  2001/12/28 06:21:24  nzmb
   ** Fixed bug causing segmentation fault in st_build.c .. there was an extra %s in line 402.
   **
   ** Revision 1.7  2001/10/29 23:41:34  miller
   ** Fixed reoccurring chaos bug (use centre() macro not 'x' to test province)
   **
   ** Revision 1.6  2001/10/20 12:11:14  miller
   ** Merged in changes from DEMA and USTV 
   **
   ** Revision 1.5.2.1  2001/10/20 00:52:49  dedo
   ** Remvoe compile warnings
   **
   ** Revision 1.5  2001/02/26 11:27:12  miller
   ** Fixed small bug on line 107, using [i] instead of [p]
   **
   ** Revision 1.4  2000/11/18 21:12:14  miller
   ** Fixed ONECENTRE bug
   **
   ** Revision 1.3  2000/11/16 20:42:40  miller
   ** New changes for phased move syntax check, transformations and onecentre/anycentre
   **
   ** Revision 1.2  1999/02/04 19:10:52  davidn
   ** buildout(). Altered index of mastrpt_pr to fix crashes on USTR.
   ** Array is indexed by unit[].owner, which can be 1 to NPOWER (=around 100 on the
   ** standard judge). MAXPLAYER is 50.
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
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

/*
 *  03 Dec 1999 Millis Miller  Discount blockaded centres from powers builds
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

#define PossibleHomeCentre(p1) (p1 == 'x' || \
		    (p1 >= '0' && p1 <= '9') || \
                    (p1 >= 'A' && p1 <= 'Z'))


int ExtraCentres(void) 
{
    int current_year;
    int first_year;

    if (dipent.extra_centres == 0)
        return 0;

    current_year = atoi(&dipent.phase[1]);
    first_year = atoi(&sphase[dipent.variant][1]);

    if ((current_year - first_year) <= dipent.extra_centres)
        return (dipent.extra_centres - current_year + first_year);

    return 0;
}

    

int CountHomeCentres( int p)
{
    int total = 0;
    int i;

    for ( i = 1; i <= npr; i++)
        if (pr[i].type == pletter[dipent.variant][p] )
            total++;

    return total;
}

int CountCentres( int p )
{

    int total = 0;
    int i;

    for ( i = 1; i <= npr; i++)
        if (pr[i].owner ==  p )
            total++;

    return total;
}


static int nu[NPOWER + 1], lu[NPOWER + 1];
int one_owned[NPOWER + 1];
/* See if passed location is in conditions to be built on */
int CheckOwnedOK( char type, int u, int p, int p1, int *c1)
{
        unsigned char *t;

	if (pr[p1].blockaded) {
            errmsg("Cannot transform wing as it is blockading %s.\n",
                                pr[p1].name);
                        return 0;
        }

        if (water(p1)) {
                    errmsg("Cannot transform %s while %s water.\n",
                                utype(unit[u].type),
                                unit[u].type == 'F' ? "in" : "over");
                    return 0;
        }

        if (type == 'x') {
                errmsg("Unit type must be specified.\n");
                return 0;
        }
        if (type == 'A')
                *c1 = MV;
        else if (type == 'W' ) {
                *c1 = MV;  /* Temporary for Wings */
                if (!(dipent.flags & F_WINGS)) {
                        errmsg("This game does not allow wings.\n");
                        return 0;
                }
	} else if (type == 'R') {	
		*c1 = MV;
		if (!(dipent.x2flags & X2F_ARTILLERY)) {
			errmsg("This game does not allow artillery.\n");
			return 0;
		}
        } else {
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
void init_build(void)
{
	int i=0, p;


	UpdateBlockades();

	/*  Count number of units allowed to be built.  */

	for (p = 1; p <= NPOWER; p++) {

	        /* Just in case it got changed, i.e. X2F_MORE_HOMES game! */
		if (PossibleHomeCentre(pr[p].type) && 
		    PossibleHomeCentre(pletter[dipent.variant][pr[p].home]))
		    pr[p].type = pletter[dipent.variant][pr[p].home];
		
		one_owned[p] = 0;
		nu[p] = 0;
		lu[p] = 0;
		nu[p] += ExtraCentres();
		
		for (i = 1; i <= npr; i++) {
		    if (gateway(i)) continue; /* Gateways don't count */
		    /* Just in case it got changed, i.e. X2F_MORE_HOMES game! */
                    if (PossibleHomeCentre(pr[i].type) &&
                    PossibleHomeCentre(pletter[dipent.variant][pr[i].home]))
                    pr[i].type = pletter[dipent.variant][pr[i].home];


		    if (pr[i].owner == p && 
		        pr[i].blockaded == 0 && 
			PossibleHomeCentre(pr[i].type))
				nu[p]++;
		}

		for (i = 1; i <= nunit; i++)
			if (unit[i].owner == p && !gateway(unit[i].loc))
				nu[p]--;
		if (nu[p] < 0)
			nu[p]--;
		if (nu[p] > 0)
			nu[p]++;

		for (i = 1; i <= npr; i++) {
			if (pr[i].owner == p && 
			    ((pr[i].type == dipent.pl[p]) || dipent.xflags & XF_BUILD_ANYCENTRES) && 
			    pr[i].blockaded == 0 )
				/* Was doing something here, no more! */
			if (pr[i].owner == p && pr[i].type == dipent.pl[p]) { 
				one_owned[p]++;
			}
		}
	}
}

int build_syntaxcheck(char *in_text, int precheck, char *out_string)
{
	char *s = in_text;

	char type, order;
	unsigned char *t;
	int p1, c1;
	char temp_out[256];
	char *out_text = NULL;
	temp_out[0]='\0';
	if (out_string != NULL) out_text = temp_out;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: {<order>} <type> <province>
	   **    order ::= build | remove ¦ transform
	   **    type  ::= Army | Fleet | Wing
	 */

	/* 
         * See if a precheck is possible
	 * If so, see if it is an canprocess line and assume ok if so
         */

	if (precheck) 
	{
		if (canpreprocess(s))  return 0;
	}

	s = get_order(s, &order);
	AddOrderToOrder(out_text,order);
	s = get_type(s, &type);
	s = get_prov(s, &p1, &c1);
	
	if (!p1 && order != 'w') {
		errmsg("Unrecognized province -> %s", s);
		return E_WARN;
	}
	*s = '\0'; /* End of string found, so terminate it */

	switch (order) {
	case 'b':
		AddUnitProvinceToOrder(out_text,type, p1);
		if (type == 'x') {
			errmsg("Unit type must be specified for build.\n");
			return E_WARN;
		}
		if (pr[p1].type == 'w' || pr[p1].type == 'l') {
    			errmsg("%s is not a supply centre.\n",
    				pr[p1].name);
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
		} else if (type == 'R') {
                    c1 = MV;
                    if (!(dipent.x2flags & X2F_ARTILLERY)) {
                        errmsg("This game does not allow artillery.\n");
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

		break;
	case 'w':
		break;

	case 'm':
		break;

	case 'r':
		AddUnitProvinceToOrder(out_text,type, p1);
		break;

	case 'h':  /* Home centre definition */
	case 'n':  /* Home centre definition cancellation */
		if (!(dipent.x2flags & X2F_MORE_HOMES)) {
		     errmsg("Invalid build order encountered.\n");
		    return E_WARN;
		}
		if (pr[p1].type != 'x' && 
		    !(pr[p1].type >= '0' && pr[p1].type <= '9') && 
		    !(pr[p1].type >= 'A' && pr[p1].type <= 'Z')) {
		        errmsg("Invalid province to make home centre.\n");
			return E_WARN;
		}
		AddProvinceToOrder(out_text, p1);
		break;

	case 't':
		
		AddUnitProvinceToOrder(out_text,type, p1);
                 if (!(dipent.xflags & XF_TRANS_BUILD)) {
                        errmsg("Build transformation is not enabled for this game.\n");
                        return E_WARN;
                }
		if (water(p1)) {
                    errmsg("Cannot transform while in/over water.\n");
                    return E_WARN;
                }
		if (type == 'W' ) {
                        if (!(dipent.flags & F_WINGS)) {
                                errmsg("This game does not allow wings.\n");
                                return E_WARN;
                        }
		}
		if (type == 'R') {
                    if (!(dipent.x2flags & X2F_ARTILLERY)) {
                        errmsg("This game does not allow artillery.\n");
                        return E_WARN;
		    }
                }


		break;

	default:
		errmsg("Invalid build order encountered.\n");
		return E_WARN;
	}
	if (out_text != NULL) strcpy(out_string, out_text);
	return 0;
}
int buildin(char **s, int p)
{
/*  Read build orders in from input file.  */

	char type, order;
	unsigned char *t;
	int i, j, u=0, p1, c1;

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
	if (!p1 && order != 'w' && order != 'u') {
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
	if (order == 'x' && !(dipent.xflags & XF_ALTBUILD))
		order = nu[p] >= 0 ? 'b' : 'r';

	if (!(dipent.xflags & XF_ANYDISBAND))
	if ((order == 'b' && (nu[p] <= 0  && !(dipent.xflags & XF_ALTBUILD) )) ||
	    (order == 'w' && (nu[p] <= 0  && !(dipent.xflags & XF_ALTBUILD) )) ||
	    (order == 'r' && nu[p] >= 0)) {
		errmsg("%s is not permitted to %s any units.\n",
		       powers[p], order == 'r' ? "remove" : "build");
		return E_WARN;
	}
	switch (order) {
	case 'b':
		if (((dipent.xflags & XF_BUILD_ONECENTRE) ==  XF_BUILD_ONECENTRE) && one_owned[p] < 1) {
			errmsg("%s must own at least one home centre to build.\n",
			       powers[p]);
			return E_WARN;
		}
                if (pr[p1].type == 'w' || pr[p1].type == 'l') {
                        errmsg("%s is not a supply centre.\n",
                                pr[p1].name);
                        return E_WARN;
                }
		if (pr[p1].type != dipent.pl[p] 
		    && !(dipent.xflags & XF_BUILD_ANYCENTRES)) {
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
		if (pr[p1].blockaded) {
			errmsg("Cannot build in %s as it is blockaded.\n",
				pr[p1].name);
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
		} else if (type == 'R') {
                        c1 = MV;
                        if (!(dipent.x2flags & X2F_ARTILLERY)) {
                            errmsg("This game does not allow artillery.\n");
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

		/*  FALL THROUGH  */
	case 'w':

		if ((u = pr[p1].unit)) {
			if (unit[u].status != 'b' && unit[u].exists ) {
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
			if (dipent.xflags & XF_ALTBUILD) {
			    if (nu[p] == 1) nu[p] = -2;
			}
			u = ++nunit;
		}

		if (dipent.xflags & XF_ALTBUILD) {
		    /* A Dirty fix that seems to do the trick */
		    if (!u ) {
		        nu[p]--;
                            if (nu[p] == 1) nu[p] = -2;
                            u = ++nunit;
                    }
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

	case 'u':    /* Unwaive, or remove waive orders */
		if (dipent.xflags & XF_ANYDISBAND)
		for (u = 1; u <= nunit; u++) {
		   if (unit[u].owner == p && unit[u].type == 'x' ) {
			unit[u].owner = 0; /* not owned! */
			nu[p]++;
			if (nu[p] == -1)
			    nu[p] = 2;
		    }
		}

	        break;


	case 'm':
		if (dipent.xflags & XF_ALTBUILD) {
		    if (!(u = pr[p1].unit) || unit[u].owner != p || !unit[u].exists) {
                        errmsg("%s does not own a unit %s %s to maintain.\n",
                        powers[p], mov_type(p1,u), pr[p1].name);
                        return E_WARN;
                    }
		    if (unit[u].status == 'd') {
			nu[p]--;
		        if (nu[p] == 1)
			    nu[p] = -2;
		    }
			
		    unit[u].order = 'm';
		    unit[u].status = 'm';
		}
		break;

	case 'r':
		u = GetUnitIndex(p1, p); /* Will return the next unit to disband */
		if (!u || unit[u].owner != p) {
			errmsg("%s does not own a unit %s %s to remove.\n",
			powers[p], mov_type(p1,u), pr[p1].name);
			return E_WARN;
		}
		/*
		   *  If this one's already been removed, put it back.
		 */

		if (!(dipent.xflags & XF_ANYDISBAND)) {
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
		}
		else {
		    if (dipent.xflags & XF_ALTBUILD) {
		    nu[p]++;
		    if (nu[p] == -1)
		        nu[p] = 2;  /* Funny threshold, value always avoids +/-1 */
		    }
		}

		unit[u].status = 'd';
		if (!(dipent.xflags & XF_ANYDISBAND)) {
		    unit[u].order = lu[p];
		    lu[p] = u;
		}

		break;

	case 't':
		u = pr[p1].unit; /* Find out what unit is there */

		if (!(dipent.xflags & XF_TRANS_BUILD)) {
                        errmsg("Build transformation is not enabled for this game.\n");
                        return E_WARN;
                }

	        if (type == 'x') {
                    errmsg("Unknown unit type to transform to.\n");
                    return E_WARN;
                }
                if (unit[u].type == type && type != 'F') {
                    errmsg("Error: Unit in %s is already of type %s.\n",
                    pr[p1].name, Utype(unit[u].type));
                    return E_WARN;
                }
                /* If a fleet, check that coast is different */
                if (type == 'F' && unit[u].type == 'F') {
                    if (c1 == unit[u].coast) {
                        errmsg("Fleet %s already on %s.\n", pr[p1].name, mtype[c1]);
                        return E_WARN;
                    }
                }

		/* OK, let's see if this can be transformed */

		switch (dipent.xflags & XF_TRANS_BANYW)
		{
			case 0: /* Can only transform on home centres */
		            if (pr[p1].type != dipent.pl[p] && pr[p1].type != 'x' ) {
                                 errmsg("%s is not a home province for %s.\n",
                                        pr[p1].name, powers[p]);
                                return E_WARN;
                            }
	                break;

			case XF_TRANS_BONEC: /* Need one home owned centre to transform */
			                        if (one_owned[p] < 1) {
                        errmsg("%s must own at least one home centre to build.\n",
                               powers[p]);
                        return E_WARN;
                        }

			case XF_TRANS_BANYC:  /* Can transform on any centre */
                        /* but must own the centre */
                        if (pr[p1].owner != p) {
                        errmsg("%s does not control %s.\n",
                               powers[p], pr[p1].name);
                        return E_WARN;
                        }
			    break;

			default: /*Anywhere at all */
			    break; /* Nothing to do */
		}
                u = pr[p1].unit;

 
                if (!CheckOwnedOK(type, u, p, p1, &c1)) return E_WARN;

                /*
                 *  If this one's already been maked for removal, skip it.
                 */

                if (unit[u].status == 'd' ) {
                        errmsg("%s %s %s is already marked to be removed.\n",
                        Utype(unit[u].type), mov_type(p1,u), pr[p1].name);
                        return E_WARN;
                } else if (unit[u].status == 'b' ) {
                        errmsg("%s %s %s is already marked to be built.\n",
                        Utype(unit[u].type), mov_type(p1,u), pr[p1].name);
                        return E_WARN;
                } else if (unit[u].type == type && unit[u].coast == c1) {
			/* Unit was marked to be transformed, cancel transform */
                        unit[u].status = ' ';
			unit[u].order = ' ';
		} else {
                    unit[u].status = 't';
                    unit[u].order = order;
                    unit[u].new_type = type;
                    unit[u].new_coast = c1;
                    /*lu[p] = u;*/
                }
		break;

        case 'h':  /* Home centre definition */
                if (!(dipent.x2flags & X2F_MORE_HOMES)) {
                     errmsg("Invalid build order encountered.\n");
                    return E_WARN;
                }
                if (pr[p1].type != 'x' &&
                    !(pr[p1].type >= '0' && pr[p1].type <= '9') &&
                    !(pr[p1].type >= 'A' && pr[p1].type <= 'Z')) {
                        errmsg("Invalid province to make home centre.\n");
                        return E_WARN;
                }
                /* OK, see if it is owned */
                if (pr[p1].owner != p) {
                    errmsg("Only owned centres can be made a home centre.\n");
                    return E_WARN;
                }
                if (pr[p1].type == pletter[dipent.variant][p]) {
                    errmsg("Centre is already owned by %s.\n",power(p));
                    return E_WARN;
                }
                if (CountHomeCentres(p) >= dipent.num_homes) {
                    errmsg("Power already has enough home centres.\n");
                    return E_WARN;
                }

                if (pr[p1].type == 'x')
                    pr[p1].home = p;
                else {
                    if (CountCentres(power(pr[p1].type)) > 0) {
                        errmsg("Centre is still used as home centre by %s.\n",
                                powers[power(pr[p1].type)]);
                        return E_WARN;
                   } else {
                        pr[p1].home = p;
                   }
                }
                break;


        case 'n':  /* Home centre cancel */
            if (!(dipent.x2flags & X2F_MORE_HOMES)) {
                     errmsg("Invalid build order encountered.\n");
                    return E_WARN;
            }
            pr[p1].home = pr[p1].type;
            break;


	default:
		errmsg("Invalid build order encountered.\n");
		return E_WARN;
	}
	return 0;

}

void buildout(int pt)
{
	int i, u, p, c1, p_index, counting_centres, u_diff[NPOWER + 1], p1, pp;
	char mastrpt_pr[NPOWER + 1];    // Used to be [MAXPLAYERS]. DAN 04/02/1999
        int num_units[NPOWER +1];
        int num_hc;
        int pr_found;
        int one_printed;
        int assumed[ NPOWER + 1 ];  /* Count number of centres power is assuming this turn as home centre */

	fprintf(rfp, "Adjustment %s for Winter of %d.  (%s.%s)\n\n",
		pt ? "orders" : "results",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	for ( u = 0; u <= NPOWER; u++)
		num_units[u] = 0;   /* Initialise array */
	
	if (pt == MASTER) {
		for (u = 0; u <= NPOWER; u++)    /* Used to be < MAXPLAYERS. DAN. */
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

        for (p1 = 1;  p1<= npr; p1++) {
            pr_found = 0;
            for (u = 1; u <= nunit; u++) {
                if (unit[u].loc == p1)
                    pr_found = 1;
            }
            if (!pr_found && pr[p1].type == 'x') {
                /* Unowned centre: if we have natives, build one there */
                if (dipent.has_natives &&
                        (pr[p1].owner == dipent.has_natives || !pr[p1].owner)) {
                    unit[++nunit].type = 'A';
                    unit[nunit].stype = 'x';
                    unit[nunit].owner = dipent.has_natives;
                    unit[nunit].status = 'b';
                    unit[nunit].loc = p1;
                }
            }
         }

	for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
            p_index = FindPower(p);
	    one_printed = 0;
            if (p_index >= dipent.n) continue; /* Not a valid power */
	    if (!(processing || pt == p || pt == MASTER)) continue;	
	    for (u = 1; u <= nunit; u++) {
		if (p == unit[u].owner) {

			if (unit[u].status == 'b') {

				num_units[unit[u].owner]++;
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
				one_printed++;

			} else if (unit[u].status == 'd' && unit[u].exists) {
				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Removes the %s %s %s.\n",
					utype(unit[u].type),
					mov_type(unit[u].loc,u),
					pr[unit[u].loc].name);

				unit[u].owner = 0;
				unit[u].status = ':';
				pr[unit[u].loc].unit_held = 0;
				one_printed++;


			} else if (unit[u].status == 'w') {
				num_units[unit[u].owner]++;
				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Build waived.\n");
				unit[u].owner = 0;
				one_printed++;

			} else if (unit[u].status == 't') {
				num_units[unit[u].owner]++;
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
				one_printed++;
			} else if (unit[u].status == 'm') {
			    num_units[unit[u].owner]++;
				/* only notify when explictly maintaining units */
			    if (!processing && !predict) {		
				fprintf(rfp, "%s: ", powers[p= unit[u].owner]);
				 for (i = strlen(powers[p]); i < LPOWER; i++)
                                                putc(' ', rfp);
                                        fprintf(rfp, "Maintains the %s %s %s.\n",
                                                utype(unit[u].type),
                                                mov_type(unit[u].loc,u),
                                                pr[unit[u].loc].name);
				one_printed++;
 			    }
				unit[u].status = ':';
			} else if (unit[u].status == ':') {
			    num_units[unit[u].owner]++;
			    if (dipent.xflags & XF_ALTBUILD) {
                                fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
                                for (i = strlen(powers[p]); i < LPOWER; i++)
                                                putc(' ', rfp);
                                fprintf(rfp, "No order for the %s %s %s", 
				             utype(unit[u].type),
					     mov_type(unit[u].loc,u),
					     pr[unit[u].loc].name);
                                if (!processing && !predict ) {
                                        more_orders++;
                                }
                                fprintf(rfp, " (maintain).\n");
				one_printed++;
			    }
                        }
		}
	    }
	    if (!processing && !predict && pt == MASTER)
		if (one_printed)
                    fprintf(rfp,"\n"); /* Extra blank for master */

	}

	for (p = 1; p <= NPOWER; p++) {
		assumed[p]=0;
		if  (processing || pt == p || pt == MASTER) {
		    p_index = FindPower(p);
		    if (p_index >= dipent.n) continue; /* Not a valid power */
                    counting_centres = dipent.players[p_index].centers -
                                       dipent.players[p_index].centres_blockaded;
                    u_diff[p] = num_units[p] - counting_centres - ExtraCentres();

		    if (u_diff[p] < 0) {
			fprintf(rfp, "%s: ", powers[p]);
			for (i = strlen(powers[p]); i < LPOWER; i++)
				putc(' ', rfp);
			for (i = 1; i <= npr; i++) {
				if (pr[i].owner == p && !pr[i].unit &&
				    (pr[i].type == dipent.pl[p] || (centre(i) &&
				     (dipent.xflags & XF_BUILD_ANYCENTRES))))
					break;
			}
			if (!(dipent.xflags & XF_ANYDISBAND)) {
			    if (i > npr ) {
				i = nu[p] - 1 + ExtraCentres();
				if (processing)
				    fprintf(rfp, "%d unusable build%s waived.\n", i, i == 1 ? "" : "s");
				else
				    fprintf(rfp, "%d unusable build%s pending.\n", i, i == 1 ? "" : "s");
				nu[p] = 1;
			    } else {
				i = nu[p] - 1;
	
				fprintf(rfp, "%d build%s pending.\n", i, i == 1 ? "" : "s");
			    }
			}
		    }
		    if ((dipent.xflags & XF_ANYDISBAND) && u_diff[p] != 0 ) {
		        if (!processing && !predict && !(pt == MASTER)) 
		                err++;	
		        if (u_diff[p] > 0) 
		            fprintf(rfp,"%d too many units: need to remove some.\n", u_diff[p]);
		        else
			    fprintf(rfp,"%d too few units: need to build/maintain/waive.\n", -u_diff[p]);
		    }
		}	
	}

	if (dipent.xflags & XF_ALTBUILD) {
	    if (err)
		more_orders++; /* MUST be 100% ok for these variants */

        } else {
            if (!processing && (nu[pt] < -1 || nu[pt] > 1))
                more_orders++;
        }

	for (u = 1; u <= nunit; u++) {
		if ((p = unit[u].owner) && 
	            u_diff[p] > 0 && 
		    pr[unit[u].loc].owner != p && 
		    FindPower(p) < dipent.n) {
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
				u_diff[p]--;
			}
		}
	}

 /* Now see if there are centres that have become home centres */
      for (p1 = 1;  p1<= npr && (dipent.x2flags & X2F_HOMETRANSFER); p1++) {
            if (pr[p1].unit && pr[p1].unit_held &&
                (PossibleHomeCentre(pletter[dipent.variant][pr[p1].home]) ||
		 (!pr[p1].home && pr[p1].type == 'x')) &&
                unit[pr[p1].unit].owner != pr[p1].home) {
                pr[p1].home = unit[pr[p1].unit].owner;
            }
        }

       for (pp = 1; pp <= NPOWER; pp++) { /* This loop is just to order by power */
            p_index = FindPower(pp);
            one_printed = 0;
            if (p_index >= dipent.n) continue; /* Not a valid power */
          one_printed = 0;
          for (p1 = 1;  p1<= npr; p1++) {
            p = pr[p1].home;
            if ( pr[p1].type != pletter[dipent.variant][pr[p1].home] &&
                PossibleHomeCentre(pletter[dipent.variant][pr[p1].home])) {
                 assumed[p]++;
                 if (pp == p && (processing || pt == p || pt == MASTER)) {
                    one_printed++;
                    fprintf(rfp, "\n%s: ", powers[p]);
                                for (i = strlen(powers[p]); i < LPOWER; i++)
                                        putc(' ', rfp);
                                fprintf(rfp, "Assumes %s as a home centre.",
                                        pr[p1].name);
                }
            }
          }
        }
        if (one_printed) fprintf(rfp, "\n");

        /* Now show pending moves for home centre declarations */
        one_printed = 0;
        for (p = 1; p <= NPOWER && (dipent.x2flags & X2F_MORE_HOMES); p++) {
            if (p != dipent.has_natives && (processing || pt == p || pt == MASTER )) {
                p_index = FindPower(p);
                if (dipent.players[p_index].centers <=0 &&
                    dipent.players[p_index].units <= 0)
                        continue;  /* Not an alive player, so ignore */
                num_hc = CountHomeCentres(p);
                if ( (dipent.x2flags & X2F_MORE_HOMES) &&
                     num_hc + assumed[p] < dipent.num_homes && CountCentres(p) > 0) {
                        fprintf(rfp, "\n%s: ", powers[p]);
                        one_printed++;
                        for (i = strlen(powers[p]); i < LPOWER; i++)
                                 putc(' ', rfp);
                        fprintf(rfp, "%d more Home Centre assignment%s pending.",
                                dipent.num_homes - num_hc - assumed[p],
                                dipent.num_homes - num_hc - assumed[p] > 1 ? "s" : "" );
                }
            }
        }
	
        if (one_printed) fprintf(rfp, "\n");

 /* Adjust the unit_held values */
      for (p1 = 1;  p1<= npr && processing; p1++) {
            if (pr[p1].unit )
                pr[p1].unit_held = 1;
        }
}	        
/****************************************************************************/
