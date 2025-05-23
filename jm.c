/*
 * $Log$
 * Revision 1.3  2003/02/17 15:39:21  millis
 * bug 10, prevent overlapping absences
 * Also improve text output for absences.
 *
 * Revision 1.2  2002/05/04 02:06:18  nzmb
 * Added code to display the time left until the deadline and grace at the
 * bottom of their reply whenever a player signs on.
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  jm.c
 *  Copyright 1987, Lowe.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "functions.h"

static long offset = -1L;

int jm(char *p, long *t)
{

	/* TODO: use strptime */

	/*  Compute the number of seconds since Jan 1st 1970 GMT
	 *  p = ascii date in standard format: Wed Dec 30 12:30:40 1994
	 *  
	 *  Return value is zero if everything was okay.
	 */

#define magic(m,d,y)  (d-32075L+1461L*(y+4800L+(m-14L)/12L)/4L       \
                       +367L*(m-2L-(m-14L)/12L*12L)/12L-3L*((y+4900L \
                       +(m-14L)/12L)/100L)/4L)
#define jd(m,d,y)     (magic((long)m,(long)d,(long)y)-magic(1L,1L,1970L))

	long temp;

	int mon, day, year, hour, min, sec;
	static char *months[] =
	{"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if (offset == -1L) {
		offset = 0L;
		time(&temp);
		if (jm(ctime(&temp), &offset)) {
			printf("Internal error in date format!\n");
			bailout(1);
		}
		offset = temp - offset;
	}
	for (mon = 1; mon <= 12; mon++)
		if (!strncmp(&p[4], months[mon - 1], 3))
			break;

	if (mon > 12)
		return 1;

	if (sscanf(&p[8], "%d %d:%d:%d %d", &day, &hour, &min, &sec, &year) != 5)
		return 1;

	temp = 60L * (60L * (24L * (long) jd(mon, day, year) +
			(long) hour) + (long) min) + (long) sec + offset;

	if (offset)
		switch (atoi(ctime(&temp) + 11) - hour) {	/* Adjust for Daylight Savings */
		case 0:
			break;
		case 1:
		case -23:
			temp -= 3600L;
			break;
		case -1:
		case 23:
			temp += 3600L;
			break;
		default:
			return 2;
		}
	*t = temp;
	return 0;
}

/*
 * TODO reformat this time to order the args intelligently, i.e.
 * in some sort of ISO time format
 */
char *ptime(time_t * time)
{
	/*
	 *  Return a time in format understandable as a deadline.
	 */
	static char text[32];
	struct tm *lt;

	lt = localtime(time);
	strftime(text, sizeof(text), "%a %b %d %Y %H:%M:%S %z", lt);
	return text;
}

char *abs_time(time_t * time)
{
        /*
         *  Return a time in format understandable as a deadline.
         */
        static char text[32];
        struct tm *lt;

        lt = localtime(time);
        strftime(text, sizeof(text), "%b %d %Y %H:%M:%S", lt);
        return text;
}
char *timeleft(time_t *deadline)
{
	time_t now, diff;
	int days;
	int hours;
	int minutes;
	int seconds;
	static char text[100];

	now = time(NULL);
	diff = *deadline - now;
	days = diff/(60*60*24);
	diff = diff - (days*60*60*24);
	hours = diff/(60*60);
	diff = diff - (hours*60*60);
	minutes = diff/60;
	diff = diff - (minutes*60);
	seconds = diff; /* yes this is unnecessary, but it makes to code readable :-) */

	sprintf(text,"%d day%s, %d hour%s, %d minute%s, and %d second%s",
		days, days == 1 ? "" : "s", hours, hours == 1 ? "" : "s",
		minutes, minutes == 1 ? "" : "s", seconds, seconds == 1 ? "" : "s");
	return text;
}

