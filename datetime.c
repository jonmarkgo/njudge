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

time_t datetime_epoc_from_string(const gchar* str) {

	time_t epoc = 0;
	GDate* gdate;
	struct tm cdate;

	gdate = g_date_new();

	g_date_set_parse(gdate, str);
	if (!g_date_valid(gdate))
		goto exit_datetime_epoc_from_string;

	g_date_to_struct_tm(gdate, &cdate);

	epoc = datetime_epoc_utc(mktime(&cdate));

exit_datetime_epoc_from_string:

	g_date_free(gdate);

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
