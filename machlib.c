/*
 * $Log$
 * Revision 1.3  2001/10/20 12:11:13  miller
 * Merged in changes from DEMA and USTV
 *
 * Revision 1.2.2.1  2001/10/19 23:09:49  dema
 * Add check function for initial money
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Forst and unit type limits
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdlib.h>
#include "dip.h"
#include "porder.h"
#include "mach.h"

/*
 * Test if a fortress exist.
 * This means a fortified city, or a fortress when they are used.
 */
int has_fortress(int prov)
{
	if (pr[prov].flags & PF_FORTRESS)
		return (dipent.xflags & XF_FORT ||
			pr[prov].flags & PF_VALUE);
	else
		return 0;
}

/*
 * Test if the province contains a fortified city.
 * This means the province should have a fortress and the cityvalue should
 * be positive
 */
int has_fortcity(int prov)
{
	return (pr[prov].flags & PF_FORTRESS && pr[prov].flags & PF_VALUE);
}

/* 
 * Affect this province or city by rebellion 
 */
void set_rebellion(int prov)
{
	if (has_fortress(prov) && !pr[prov].gunit)
		set_crebellion(prov);
	/*
	   ** This assumes rebellion always affects the province
	 */
	set_prebellion(prov);
}

/*
 * See if power has initial money, return 1 if so, 0 if not or none defined
 */

int GetInitialMoney(int p, int *money)
{
    if (!(initial_money.enabled)) return 0; /* No inital money defined */

    if (atoi(dipent.seq) >1 ) return 0; /* Not on initial turn */ 

    *money = initial_money.money[p];
    return 1;
}

/*
 * PermittedMachUnit - see if power is allowed unit type requested
 */

int PermittedMachUnit(int power, char type, char stype, int mode)
{
    int i, power_index = 0;
    int unit_type, unit_stype;
    int mask;

    if (!(dipent.xflags & XF_LIMIT_TYPES))
	return 1; /* No unit limits enabled, always ok */

/* Firstly, find powe in permitted array */

    
    for (i = 0; i < MAXPLAYERS && !power_index; i++)
    {
	if (dipent.pl[power] == permitted_units[i].power_letter)
	    power_index = i;
    }
/* Now, convert type and stype to my defines */

    switch (tolower(type))
    {
	case 'a':
	    unit_type = P_ARMY;
	    break;
	case 'f':
	    unit_type = P_FLEET;
	    break;
	default:
	    unit_type = P_GARRISON;
    }
    switch (tolower(stype))
    {
	case 'c':
	    unit_stype = P_CITIZEN;
	    break;
        case 'm':
            unit_stype = P_MERC;
	    break;
	case 'p':
	    unit_stype = P_PROF;
	    break;
	default:
	    unit_stype = P_NORMAL;
    }
/* OK, let's see if this unit is permitted or not */

   mask = (permitted_units[power_index].permissions[unit_type]);
   mask &=  (0x3 << unit_stype * 2);
   mask = mask >>  (unit_stype * 2);
   mask &= mode;

/*   mask = ((permitted_units[power_index].permissions[unit_type] & (0xFF << unit_stype * 2)) >> (unit_stype * 2)) & mode;
*/
    return mask;
}
