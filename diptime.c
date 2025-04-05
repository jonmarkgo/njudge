
/*
   ** $Log: diptime.c,v $
   ** Revision 1.2  2004-01-21 00:55:57  millis
   ** Allow passing of forced date/time (for testing)
   **
   ** Revision 1.1  2004/01/02 10:03:00  millis
   ** *** empty log message ***
   **
 */

/*  diptime.c
 */


#include "diptime.h"

time_t dip_time = 0;

time_t diptime(time_t *dtime)
{
    time_t now_time;
    if (dip_time == 0 ) {
	time(&now_time);
	if (dtime != NULL)
	    *dtime = now_time;
	return now_time;
    } else {
	if (dtime != NULL)
	    *dtime = dip_time;
	return dip_time;
    }
}
