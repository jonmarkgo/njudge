/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

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
		return (dipent.flags & F_FORTRESS ||
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
