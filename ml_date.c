
/*
   ** $Log$
   ** Revision 1.6  2004/07/26 23:17:24  millis
   ** Bug 340: default to 00:00 for absence start and 23:59 for absence end.
   ** All other uses of date function stay unaltered.
   **
   ** Revision 1.5  2004/07/04 03:49:40  alange
   ** Bug 323. Rewrite date calculation logic to take advantage of mktime().
   ** Semantics may be slightly different in evaluating ambiguous dates.
   **
   ** Revision 1.4  2003/09/09 18:51:31  jaldhar
   ** Got rid of port.h and replaced with some extra configure checks.  The
   ** include strings.h was not carried over because it is commented out and
   ** likely wouldn't work anyway.
   **
   ** Revision 1.3  2003/02/25 11:11:23  russblau
   ** Improved absence documentation and related Judge message strings.
   **
   ** Revision 1.2  2002/10/07 03:58:16  nzmb
   ** Changed ml_date.c so set deadline command defaults to phase clock parameter
   ** rather than 23:30 local time.
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  ml_date.c -- Parse a relatively free form date.
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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "dip.h"
#include "functions.h"

char *lookfor();

#define nentry(x) (sizeof(x)/sizeof(x[0]))

/***************************************************************************/

int mail_date(char **p, long *date, int past, FILE * rfp, int date_type)
{

/*
 *  Parse a fairly free form date.  JM will parse strict unix dates.
 *
 *  If a valid date is found, the pointer is advanced.
 *  A non-zero return value indicates an error, date trashed.
 *  If non-zero, past specifies dates are past rather than future.
 */

	char *s;
	struct tm *tm, t, now;
	time_t t1, t2;
	int dow, mon, dom, hour, mins, year;
	int i, change;
	int clockhours, clockmins;

	static char *dows[] =
	{"", "sun", "mon", "tue", "wed", "thu", "fri", "sat"};
	static char *mons[] =
	{"", "jan", "feb", "mar", "apr", "may", "jun",
	 "jul", "aug", "sep", "oct", "nov", "dec"};
	static int month_length[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	dow = mon = hour = mins = dom = year = -1;

	change = -1;
	*date = 0;

	s = *p;
	while (*s) {
		if (dow == -1) {
			s = lookfor(s, dows, nentry(dows), &dow);
			if (dow)
				change = 1;
			dow--;
		}
		if (mon == -1) {
			s = lookfor(s, mons, nentry(mons), &mon);
			if (mon)
				change = 1;
			mon--;
		}
		if (isdigit(*s)) {
			change = 1;
			for (i = 0; isdigit(*s); s++)
				i = i * 10 + *s - '0';
			if (dom == -1 && (!*s || isspace(*s) || *s == ',')
			    && 1 <= i && i <= 31) {
				dom = i;
				if (*s == ',')
					s++;
			} else if (hour == -1 && *s == ':' && i < 24) {
				hour = i;
				for (i = 0, s++; isdigit(*s); s++)
					i = i * 10 + *s - '0';
				if (i > 59) {
					fprintf(rfp, "Invalid minute specified %d%s", i, s);
					return 1;
				}
				mins = i;
				while (isdigit(*s) || *s == ':' || *s == '.')
					s++;
			} else if (year == -1 && 80 < i && i < 100) {
				year = i;
			} else if (year == -1 && i > 1900) {
				year = i - 1900;
			} else {
				fprintf(rfp, "Unparsable digits following date: %d%s", i, s);
				return 1;
			}
			while (isspace(*s))
				s++;
		}
		if (change < 0)
			return 1;
		else if (!change)
			break;

		change = 0;

	}


	switch (date_type)
	{
	    case DT_ABS_START:
		clockhours = 0;
		clockmins = 0;
		break;

	    case DT_ABS_END:
		clockhours = 23;
		clockmins = 59;
		break;

	    default:
		
	    /* Want this not to default to 23:30
	     * fix by Tim Miller on inspiration from Eugene Hung
	     */
	    switch(dipent.phase[5])
	    {
		case 'M':
			clockhours = dipent.movement.clock;
			clockmins = dipent.movement.clock % 60;
			break;
		case 'R':
			clockhours = dipent.retreat.clock;
			clockmins = dipent.retreat.clock % 60;
			break;
		case 'B':
			clockhours = dipent.builds.clock;
			clockmins = dipent.builds.clock % 60;
			break;
		default:
			/* oh oh */
			clockhours = clockmins = -1;
	    }

	    if(clockhours >= 0)
		clockhours /= 60;
	    else
	    {
		clockhours = 23;
		clockmins = 30;
	    }
	}	

	time(&t2);
	tm = localtime(&t2);

	memcpy(&now, tm, sizeof(now));

/* Default DST indicator to unknown. Populate time of day in the struct */
	t.tm_isdst = -1;
	t.tm_hour = hour != -1 ? hour : past ? 0 : clockhours;
	t.tm_min = mins != -1 ? mins : past ? 0 : clockmins;
	t.tm_sec = 0;

	if (dow != -1) /* Calculate date if day of week provided. */	
	{
		if (dom != -1 || mon != -1 || year != -1)
		{
			fprintf(rfp, "Don't specify day of week with month, day or year.\n");
			return 1;
		}
		i = dow - now.tm_wday;
		if (past && i>=0) 
			i -= 7;
		if (!past && i <= 0)
			i += 7;
		t.tm_year = now.tm_year;
		t.tm_mon = now.tm_mon;
		t.tm_mday = now.tm_mday + i;
	}
	else /* Calculate if day of year provided or only time of day. */
	{
		if (year != -1)
		{
			t.tm_year = year;
			t.tm_mon = past ? 0 : 11; /* Jan or Dec */
			t.tm_mday = past ? 1 : 31; /* Jan 1 or Dec 31 */
		}
		else
		{
			t.tm_year = now.tm_year;
		}
		if (mon != -1)
		{
			t.tm_mon = mon;
			t.tm_mday = past ? 1 : month_length[mon];
		}
		else
		{
			if (year == -1)
				t.tm_mon = now.tm_mon;
		}
		if (dom != -1)
		{
			t.tm_mday = dom;
		}
		else
		{
			if (year == -1 && mon == -1)
				t.tm_mday = now.tm_mday;
		}
	}

	t1 = mktime(&t);
	if (t1 != -1 && (t1 - t2) * (past ? -1 : 1) < 0)
	{
		if (dom == -1 && mon == -1 && year == -1)
			t.tm_mday += past ? -1 : 1;
		else
			if (mon == -1 && year == -1)
				t.tm_mon += past ? -1 : 1;
			else
				if (year == -1)
					t.tm_year += past ? -1 : 1;
		t1 = mktime(&t);
	}

	if (t1 == -1)
	{
		fprintf(rfp,"Unable to calculate date:  ");
                fprintf(rfp,"%2.2d %2.2d %4.4d %2.2d:%2.2d:%2.2d\n",
			t.tm_mday, t.tm_mon+1, t.tm_year+1900,
			t.tm_hour, t.tm_min, t.tm_sec);
                return 1;
	}

	if (!past && t1 < t2)
	{
		fprintf(rfp,"The given date (%2.2i %s %4.4i %i:%2.2i) should be in the future, but it is in the past.\n",
                    t.tm_mday, mons[t.tm_mon+1], t.tm_year+1900, t.tm_hour, t.tm_min);
                return 1;
	}

	if (!past && (year == -1 || mon == -1 || dom == -1) && t1 - t2 > 120*24*60*60)
	{
		fprintf(rfp,"The caluculated date (%2.2i %s %4.4i %i:%2.2i) is more than 120 days in the future.\nIf this is what you want, then specify all of year, month and day.\n",
                    t.tm_mday, mons[t.tm_mon+1], t.tm_year+1900, t.tm_hour, t.tm_min);
                return 1;
        }

	*date = t1;
	*p = s;
	return 0;

}
