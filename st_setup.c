
/*
   ** $Log$
   ** Revision 1.2  2003/01/17 23:28:11  millis
   ** Updated from USTV
   **
   ** Revision 1.1.2.1  2003/01/13 16:04:59  millis
   ** ustv latest versions
   **
   ** Revision 1.1  2002/04/15 12:55:47  miller
   ** Multiple changes for blind & Colonial & setup from USTV
   **
   **
 */

/*  st_setup.c
   **
   **  Copyright 2002, Lowe.
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

static int ValidPower(int);


int setupin(char **s, int p)
{
/*  Read build orders in from input file.  */

	char type, order;
	unsigned char *t;
	int pow, new_nunits;
	int i, u, p1, c1;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: {<order>} <type> <province>
	   **    order ::= build | remove 
	   **    type  ::= Army | Fleet | Wing
	 */


	if (p != MASTER) {
	    errmsg("Game is in setup phase: order ignored!\n\n");
	    return E_WARN;
	}

	*s = get_order(*s, &order);
	*s = get_power(*s, &pow);
	*s = get_type(*s, &type);
	*s = get_prov(*s, &p1, &c1);
	if (!p1 && order != 'c' && order != 'z' && order != 's') {
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


	switch (order) {
	case 'b':
		if (pow == 0) {
			errmsg("Power owner must be specified for build.\n");
			return E_WARN;
		}
		if (type == 'x') {
			errmsg("Unit type must be specified for build.\n");
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

		if ((u = pr[p1].unit)) {
		} else {
			u = ++nunit;
		}

		pr[p1].unit = order == 'w' ? 0 : u;
		unit[u].owner = pow;
		unit[u].type = type;
		unit[u].stype = 'x';
		unit[u].loc = p1;
		unit[u].coast = c1;
		unit[u].status = order;
		break;

	case 'r':
		if (!(u = pr[p1].unit)) {
			errmsg("%s does not own a unit %s %s to remove.\n",
			powers[p], mov_type(p1,u), pr[p1].name);
			return E_WARN;
		}
		/*
		 *  If this one's already been removed, ignore .
		 */
		if (unit[u].status == 'd') {
			errmsg("Unit in %s already removed.\n", pr[p1].name);
                        return E_WARN;
                }

		unit[u].status = 'd';

		break;

	case 'c':  /* Clear whole board */
		for (i = 1; i <= nunit; i++)
		    unit[i].status = 'd';	
		break;

	case 'z': /* Remove armies of one power */
		if (!pow) {
		    errmsg("Invalid power encountered: must specify correctly.\n");
		    return E_WARN;
		}
		for (i = 1; i <= nunit; i++)
                {
                    if (unit[i].owner == pow)
                        unit[i].status = 'd';
                }
		break;
 
	case 's': /* Restore board to pre-setup settings */
		/* This code relies on new units always coming after existing ones */
		new_nunits = nunit;
		for (i = 1; i <= nunit; i++)
		{
		    if (unit[i].exists) 
			unit[i].status = ':';
		    else 
			new_nunits--;
		}
		nunit = new_nunits;

		/* OK, now restore province ownerships too */
		for (i=1; i <= npr; i++)
		    pr[p].new_owner = pr[p].owner;

		break;

	case 'o': /* Set owner of a centre */
	        if (pow == 0) {
                        errmsg("Power owner must be specified for ownership.\n");
                        return E_WARN;
                }
		pr[p].new_owner = p;
		break;

	default:
		fprintf(rfp, "Invalid setup order encountered.\n");
		err++;
		return E_WARN;
	}
	return 0;

}

void setupout(int pt)
{
	int i, u, p, c1, p_index, counting_centres, u_diff;
	char mastrpt_pr[NPOWER + 1];    // Used to be [MAXPLAYERS]. DAN 04/02/1999
        int num_units[NPOWER +1];

	if (!processing && pt != MASTER) return;  /* only master or processing phase interests */

	fprintf(rfp, "Setup status for Season of %d.  (%s.%s)\n\n",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	for ( u = 0; u <= NPOWER; u++)
		num_units[u] = 0;   /* Initialise array */
	
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
		if (pt == MASTER) {

			if (unit[u].status == 'b' || unit[u].status == ':') {

				num_units[unit[u].owner]++;
				fprintf(rfp, "%s: ", powers[p = unit[u].owner]);
				for (i = strlen(powers[p]); i < LPOWER; i++)
					putc(' ', rfp);
				fprintf(rfp, "%s %s", Utype(unit[u].type),
					pr[unit[u].loc].name);
				if ((c1 = unit[u].coast) > XC)
					fprintf(rfp, " (%s).\n", mtype[c1]);
				else
					fprintf(rfp, ".\n");
				unit[u].status = ':';
                        }
		}
	}

	for (p = 1; p <= NPOWER; p++) {
			p_index = FindPower(p);
                        if (!ValidPower(p)) continue;
			
			fprintf(rfp, "%s: ", powers[p]);
			for (i = strlen(powers[p]); i < LPOWER; i++)
				putc(' ', rfp);
			for (i = 1; i <= npr; i++) {
				if (pr[i].owner == p && !pr[i].unit &&
				    (pr[i].home == dipent.pl[p] || (centre(i) &&
				     (dipent.xflags & XF_BUILD_ANYCENTRES))))
					break;
			}
			counting_centres = dipent.players[p_index].centers - dipent.players[p_index].centres_blockaded;
			u_diff = num_units[p] + dipent.players[p_index].units  - counting_centres;

			if ( u_diff != 0 ) {
			    if (u_diff > 0) {
				fprintf(rfp,"%d too many units: need to remove some.\n", u_diff);
				err++;
			    } else
				fprintf(rfp,"%d too few units: need to build.\n", -u_diff);
			} else
			    fprintf(rfp,"unit count correct,\n");	
	}

}

static int ValidPower(int p)
{
/* Return 1 if power is a valid player power in this game, else 0 */

    switch (pletter[dipent.variant][p]) {

	    case '?':
	    case '&':
	    case 'M':
	    case 'O':
	    case '.':
	    case 'x':
		return 0;

	    default:
	        return 1;

    }	
}

/****************************************************************************/
