/*
 * datetime.c
 *
 *  Created on: Nov 18, 2012
 *      Author: cjw
 */

#include <glib.h>
#include <time.h>
#include <stdio.h>

static const char* abmon[12] = {
		"jan", "feb", "mar", "apr", "may", "jun",
		"jul", "aug", "sep", "oct", "nov", "dec"
};

time_t datetime_epoc_from_string(const char* str) {

	guint     epoc = 0;
	guint     idx;
	guint     parsed;
	gchar*    str_month[4];
	gchar*    cptr = abmon;
	struct tm date = {0};

	parsed = sscanf(str, "%d %3s %d", date.tm_mday, str_month, date.tm_year);
	if (parsed < 3)
		goto exit_str_to_epochdate;

	for (idx = 0; idx < 12; idx ++) {
		if (!strcasecmp(abmon[idx], str_month)) {
			date.tm_mon = idx;
		}
	}

	if (idx == 12)
		goto exit_str_to_epocdate;

	epoc = mktime(&date);

exit_str_to_epocdate:

	return epoc;

}
