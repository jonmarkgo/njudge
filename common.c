/*
 * $Log$
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

/****************************************************************************/

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
	for (t = pr[unit[u].loc].move; *t; t++) {
		if (*t++ == p
		    && ((unit[u].type == 'W') || 
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

void next_year(void)
{
	int dipent_year = atoi(&dipent.phase[1]) + 1;
	while (dipent_year >= 10000)
		dipent_year -= 10000;
	sprintf(dipent.phase, "S%04dM", dipent_year);
	init_movement();
}
