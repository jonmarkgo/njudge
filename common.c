/*
 * $Log$
 * Revision 1.8  2004/05/22 08:53:14  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.6  2003/05/02 22:23:39  millis
 * Added valid_artillery_move()
 *
 * Revision 1.5  2003/01/13 22:38:51  millis
 * merged in from ustv
 *
 * Revision 1.4.2.1  2003/01/13 16:04:47  millis
 * ustv latest versions
 *
 * Revision 1.4  2002/04/15 12:55:41  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.2.2.1  2001/10/19 23:25:18  dema
 * Initialse need-order array
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added handling for Wing units
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/01/26 16:55:29  rpaar
 * Applied Kevin's year patch to remove some bugs in year handling.
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdlib.h>

#include "dip.h"
#include "porder.h"
#include "functions.h"

/****************************************************************************/
int valid_artillery_move(int u, int p, int *c, int *b)
{
    /* Return valid_move() if not an artillery unit */
    /* Return 0 if artillery but can't get there */
    /* return 1 if artillery and can get there */

    int ok = 0;
    unsigned char *t, fc;

    ok = valid_move(u, p, c, b);

    if (unit[u].type != 'R')
	return ok;  /* Non artillery can only support in the same place */

/* Create a dummy unit */
	nunit++;
	unit[nunit].coast = MV;  /* Artillery moves as a land unit */
	

        for (t = pr[unit[u].loc].move; *t && !ok; t++) {
	    unit[nunit].loc = *t++;
	    fc = *t >> 4;
	    if (fc == MV ) {
	        ok = valid_move(nunit, p, c, b);
	    }
	}

    nunit--;  /* Delete dummy unit */

    return ok;  
}

int valid_move(int u, int p, int *c, int *b)
{

	/*
	 *  Verify that the specified unit can move to the specified coast of
	 *  the specified province.
	 *
	 *  If the unit is on the XC coast, movement from any coast is okay.
	 *
	 *  If the destination coast is not specified, the destination coast is
	 *  set to the first one found that the unit can get to.
	 *
	 *  If the destination coast is CC (conditional) then the next entry in
	 *  the movement table contains the province that controls the straits.
	 *   
         *  Additionally, if the unit is a wing, it can do anything!
	 */

	unsigned char *t, fc;
	int ret;

	*b = 0;
	if (u <= 0) return 0; /* Invalid unit index */
	if (!pr[unit[u].loc].move) 
	    return 0;  /* Province has no valid moves, so return 0 */
	for (t = pr[unit[u].loc].move; *t; t++) {
		if (*t++ == p)
		  if 
		     (((unit[u].type == 'W') || (unit[u].type == 'T') || 
			 ((unit[u].coast == (fc = (*t >> 4)) || (unit[u].coast == XC && fc))
		          && (*c == (*t & 0x0F) || !*c)))) {
			if (!*c)
				*c = (*t & 0x0F);
			if (*c == CC) {
				*b = *++t;
				*c = XC;
			}
			ret = 1;
			return ret;
		}
	}
	ret = 0;
	return ret;
}


int lookup(unsigned char *name)
{
	unsigned char *s, *t;

	/*
	 * Do a linear search on the heap for a specified province abbreviation
	 *
	 * The structure of the heap is name, 0, number, name, 0, number ...
	 */

	t = (char *) heap;
	while (*t) {
		s = name;
		while (*t && toupper(*s) == toupper(*t)) {
			while (*++s == '.' || isspace(*s));
			while (*++t == '.' || isspace(*t));
		}
		if (!*s && !*t)
			return (int) *++t;
		while (*t++);
		t++;
	}
	return 0;
}


void init_retreats(void)
{
	int i;

	dipent.phase[5] = 'R';

	for (i=0; i < NPOWER+1; i++)
	    need_order[i] = 0;  /* Initialise array */
	
	for (i = 1; i <= nunit; i++)
		if (unit[i].status == 'r')
			if (unit[i].owner < WILD_PLAYER)
				need_order[unit[i].owner]++;
}

void init_movement(void)
{
	int i;

	dipent.phase[5] = 'M';

	for (i=0; i < NPOWER+1; i++)
            need_order[i] = 0;  /* Initialise array */                                  

	for (i = 1; i <= nunit; i++)
		if (unit[i].owner < WILD_PLAYER)
			need_order[unit[i].owner]++;
}

int HasOneMercenaryAlive(void)
{
    int i;

    for (i = 0; i < dipent.n; i++) {
        if (dipent.players[i].controlling_power != 0 &&
	    dipent.players[i].centers > 0 )
	    return 1;
    }

    return 0;  /* No-one found alive! */
}
void next_year(void)
{
	int dipent_year = atoi(&dipent.phase[1]) + 1;
	while (dipent_year >= 10000)
		dipent_year -= 10000;
	sprintf(dipent.phase, "S%04dM", dipent_year);
	if (dipent.flags & F_INTIMATE && HasOneMercenaryAlive())
		dipent.phase[5] = 'A'; /* Intimate adjustment phase */
	else 
	    init_movement();
}
