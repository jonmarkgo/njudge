
/*
   ** $Log$
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
#include <time.h>
#include <string.h>

#include "port.h"
#include "dip.h"

char *lookfor();

#define nentry(x) (sizeof(x)/sizeof(x[0]))

/***************************************************************************/

int mail_date(char **p, long *date, int past, FILE * rfp)
{

/*
 *  Parse a fairly free form date.  JM will parse strict unix dates.
 *
 *  If a valid date is found, the pointer is advanced.
 *  A non-zero return value indicates an error, date trashed.
 *  If non-zero, past specifies dates are past rather than future.
 */

	char *s;
	struct tm *tm, t;
	int dow, mon, dom, hour, mins, year;
	int i, j, change;
	int clockhours, clockmins;

	static char *dows[] =
	{"", "sun", "mon", "tue", "wed", "thu", "fri", "sat"};
	static char *mons[] =
	{"", "jan", "feb", "mar", "apr", "may", "jun",
	 "jul", "aug", "sep", "oct", "nov", "dec"};

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

	if (hour == -1)
		hour = past ? 0 : /*23*/ clockhours;
	if (mins == -1)
		mins = past ? 0 : /*30*/ clockmins;
	if (dom == -1 && dow == -1 && mon != -1 && past)
		dom = 1;

	time(date);
	tm = localtime(date);

	if (dow != -1) {
		if (dom != -1 || mon != -1 || year != -1) {
			fprintf(rfp, "Don't specify day of week with month, day or year.\n");
			return 1;
		}
		i = dow - tm->tm_wday;
		if (!past) {
			if (i <= 0)
				i += 7;
		} else {
			if (i >= 0)
				i -= 7;
		}
		*date += 24 * 60 * 60 * i - ((tm->tm_hour * 60 + tm->tm_min) * 60 + tm->tm_sec);
		tm = localtime(date);
		if (tm->tm_hour != 0) {		/* correct for daylight savings */
			i = 0 - tm->tm_hour;
			if (i > 10) {
				i -= 24;
			} else if (i < -10) {
				i += 24;
			}
			*date += 60 * 60 * i - (tm->tm_min * 60 + tm->tm_sec);
		}
	}
	memcpy(&t, tm, sizeof(t));

	if (year != -1 && year != tm->tm_year) {
		if (!past && year < tm->tm_year) {
			fprintf(rfp, "Invalid year.\n");
			return 1;
		}
		t.tm_year = year;
		t.tm_mon = 1 - 1;
		t.tm_mday = 1;
		t.tm_hour = 0;
	}
	if (mon != -1 && mon != t.tm_mon) {
		if (!past && mon < t.tm_mon) {
			if ((year == -1 && t.tm_mon - mon < 6) || year == tm->tm_year) {
				fprintf(rfp, "Month has already passed.\n");
				return 1;
			}
			if (year == -1)
				t.tm_year++;
		}
		if (past && mon > t.tm_mon) {
			if (year == -1 && mon - t.tm_mon < 10) {
				t.tm_year--;
			}
		}
		t.tm_mon = mon;
		t.tm_mday = 1;
		t.tm_hour = 0;
	}
	if (dom != -1 && dom != t.tm_mday) {
		if (!past && dom < t.tm_mday) {
			if (mon != -1) {
				fprintf(rfp, "Day of the month has already passed.\n");
				return 1;
			} else {
				if (t.tm_mon++ == 13 - 1) {
					if (year != -1) {
						fprintf(rfp, "Day of the month has already passed.\n");
						return 1;
					}
					t.tm_mon = 1 - 1;
					t.tm_year++;
				}
			}
		}
		t.tm_mday = dom;
		t.tm_hour = 0;
	}
	if (!past && hour < t.tm_hour) {
		fprintf(rfp, "Hour has passed.\n");
		return 1;
	}
	t.tm_hour = hour;
	t.tm_min = mins;
	t.tm_sec = 0;

	for (i = 0; i < 10 && (tm->tm_year != t.tm_year); i++) {
		*date += (t.tm_year - tm->tm_year) * 365 * 24 * 60 * 60;
		tm = localtime(date);
	}

	*date += (t.tm_mon * 31 - tm->tm_yday) * 24 * 60 * 60;
	tm = localtime(date);

	for (i = 0; i < 10 && (tm->tm_mon != t.tm_mon); i++) {
		*date += (t.tm_mon - tm->tm_mon) * 25 * 24 * 60 * 60;
		tm = localtime(date);
	}

	for (i = 0; i < 9 && (tm->tm_year != t.tm_year ||
			      tm->tm_mon != t.tm_mon ||
			      tm->tm_mday != t.tm_mday ||
			      tm->tm_hour != t.tm_hour ||
			      tm->tm_min != t.tm_min); i++) {

		*date += (j = ((((((t.tm_year - tm->tm_year) * 14L +
				   t.tm_mon - tm->tm_mon) * 26L +
				  t.tm_mday - tm->tm_mday) * 24L +
				 t.tm_hour - tm->tm_hour) * 60L +
				t.tm_min - tm->tm_min) * 60L +
			       t.tm_sec - tm->tm_sec));

		tm = localtime(date);

		if (i > 5) {
			fprintf(rfp, "Trying %2d %24.24s: ", i, ctime(date));
			fprintf(rfp, "%02d/%02d/%02d %02d:%02d - ",
				t.tm_mon + 1, t.tm_mday, t.tm_year, t.tm_hour, t.tm_min);
			fprintf(rfp, "%02d/%02d/%02d %02d:%02d = %d.\n",
				tm->tm_mon + 1, tm->tm_mday, tm->tm_year, tm->tm_hour, tm->tm_min, j);
		}
	}

	if (i == 9) {
		fprintf(rfp, "Invalid day of the month: %02d/%02d/%02d %02d:%02d.\n",
		t.tm_mon + 1, t.tm_mday, t.tm_year, t.tm_hour, t.tm_min);
		return 1;
	}
	*p = s;
	return 0;

}
