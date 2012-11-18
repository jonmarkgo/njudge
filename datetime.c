/*
 * datetime.c
 *
 *  Created on: Nov 18, 2012
 *      Author: cjw
 */

#include <glib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "datetime.h"

static const char* abmon[12] = {
		"jan", "feb", "mar", "apr", "may", "jun",
		"jul", "aug", "sep", "oct", "nov", "dec"
};

time_t datetime_epoc_from_string(const char* str) {

	time_t    epoc = 0;
	guint     month;
	guint     parsed;
	gchar*    cptr;
	gchar*    cpy_str;
	gchar     str_month[4];
	struct tm date = {0};

	cpy_str = g_strdup(str);

	/* remove comma to make parsing a bit more simple */
	if ((cptr = strchr(cpy_str, ',')) != NULL) *cptr = ' ';

	parsed = sscanf(cpy_str, "%d %3s %d", &date.tm_mday, str_month, &date.tm_year);
	if (parsed != 3) {
		parsed = sscanf(cpy_str, "%3s %d %d", str_month, &date.tm_mday, &date.tm_year);
		if (parsed != 3)
			goto exit_datetime_epoc_from_string;
	}

	for (month = 0; month < 12; month ++) {
		if (!strcasecmp(abmon[month], str_month)) {
			date.tm_mon = month;
			break;
		}
	}
	if (month == 12) /* not found */
		goto exit_datetime_epoc_from_string;

	date.tm_year -= 1900;

	epoc = datetime_epoc_utc(mktime(&date));

exit_datetime_epoc_from_string:

	g_free(cpy_str);

	return epoc;

}
time_t datetime_epoc_utc(time_t time) {

	static gint offset = G_MAXINT;
	struct tm ltime;
	struct tm utime;

	if (offset == G_MAXINT) {
		memcpy(&utime, gmtime(&time), sizeof(struct tm));
		memcpy(&ltime, localtime(&time), sizeof(struct tm));
		offset = mktime(&ltime) - mktime(&utime);
	}

	return time - offset;

}
