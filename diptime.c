
/*
   ** $Log$
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
	*dtime = dip_time;
	return dip_time;
    }
}
