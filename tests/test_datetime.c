/*
 * test_datetime.c
 *
 *  Created on: Nov 18, 2012
 *      Author: cjw
 */

#include <stdio.h>
#include <glib.h>

#include "test_main.h"

#include "../datetime.h"

void test_datetime_epoc_from_string(void) {

	g_assert(!datetime_epoc_from_string("test"));

	g_assert(datetime_epoc_from_string("5 Jun 1980") == 329004000);
	g_assert(datetime_epoc_from_string("13 Aug, 1980") == 334965600);

	g_assert(datetime_epoc_from_string("Jun 5 1980") == 329004000);
	g_assert(datetime_epoc_from_string("Aug 13, 1980") == 334965600);

}

tests_t tests_datetime[] = {
		{"/datetime/datetime_epoc_from_string",			test_datetime_epoc_from_string},
		{NULL}
};
