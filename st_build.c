
/*
   ** $Log$
   ** Revision 1.7  2001/10/29 23:41:34  miller
   ** Fixed reoccurring chaos bug (use centre() macro not 'x' to test province)
   **
   ** Revision 1.6  2001/10/20 12:11:14  miller
   ** Merged in changes from DEMA and USTV CVS: ----------------------------------------------------------------------
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

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"



static int nu[NPOWER + 1], lu[NPOWER + 1], cnb[NPOWER + 1];
int one_owned[NPOWER + 1];
/* See if passed location is in conditions to be built on */
int CheckOwnedOK( char type, int u, int p, int p1, int *c1)
{
        char *t;

	if (pr[p1].blockaded) {
            errmsg("Cannot transform wing as it is blockading %s.\n",
                                pr[p1].name);
                        return E_WARN;
        }

        if (water(p1)) {
                    errmsg("Cannot transform %s while %s water.\n",
                                utype(unit[u].type),
                                unit[u].type == 'F' ? "in" : "over");
                    return E_WARN;
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
		one_owned[p] = 0;
		nu[p] = 0;
		lu[p] = 0;
		for (i = 1; i <= npr; i++)
			if (pr[i].owner == p && pr[i].blockaded == 0)
				nu[p]++;
		for (i = 1; i <= nunit; i++)
			if (unit[i].owner == p)
				nu[p]--;
		if (nu[p] < 0)
			nu[p]--;
		if (nu[p] > 0)
			nu[p]++;

		for (i = 1; i <= npr; i++) {
			if (pr[i].owner == p && 
			    ((pr[i].type == dipent.pl[p]) || dipent.xflags & XF_BUILD_ANYCENTRES) && 
			    pr[i].blockaded == 0 )
				cnb[p] = 0;
			if (pr[i].owner == p && pr[i].type == dipent.pl[p]) { 
				one_owned[p]++;
			}
		}
	}
}

int build_syntaxcheck(char *in_text, int precheck, char *out_string)
{
	char *s = in_text;

	char type, order, *t;
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

		break;

	default:
		fprintf(rfp, "Invalid build order encountered.\n");
		err++;
		return E_WARN;
	}
	if (out_text != NULL) strcpy(out_string, out_text);
	return 0;
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
	   **    order ::= build | remove ¦ transform
	   **    type  ::= Army | Fleet | Wing
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
			errmsg("%s does not own a unit %s %s %s to remove.\n",
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

	case 't':
                if (!(dipent.xflags & XF_TRANS_BUILD)) {
                        errmsg("Build transformation is not enabled for this game.\n");
                        return E_WARN;
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
                        unit[u].status = ':';
			unit[u].order = ' ';
		} else {
                    unit[u].status = 't';
                    unit[u].order = order;
                    unit[u].new_type = type;
                    unit[u].new_coast = c1;
                    /*lu[p] = u;*/
                }
		break;

	default:
		fprintf(rfp, "Invalid build order encountered.\n");
		err++;
		return E_WARN;
	}
	return 0;

}

void buildout(int pt)
{
	int i, u, p, c1;
	char mastrpt_pr[NPOWER + 1];    // Used to be [MAXPLAYERS]. DAN 04/02/1999

	fprintf(rfp, "Adjustment orders for Winter of %d.  (%s.%s)\n\n",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

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
				fprintf(rfp, "Removes the %s %s %s.\n",
					utype(unit[u].type),
					mov_type(unit[u].loc,u),
					pr[unit[u].loc].name);

				unit[u].owner = 0;
				unit[u].status = ':';


			} else if (unit[u].status == 'w') {

				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "Build waived.\n");
				unit[u].owner = 0;

			} else if (unit[u].status == 't') {
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
				    (pr[i].type == dipent.pl[p] || (centre(i) &&
				     (dipent.xflags & XF_BUILD_ANYCENTRES))))
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
