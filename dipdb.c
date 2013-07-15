/*
 * dipdb.c
 *
 *  Created on: Jul 15, 2013
 *      Author: cjw
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>

#include "dipdb.h"
#include "dipdb_mysql.h"

static void setup_mysql(void);

gboolean (*dipdb_init)(GError** gerr);

static GQuark dipdb_error_quark(void) {

	return g_quark_from_static_string("DIPDB");

}
gboolean dipdb_interface_init(GError** gerr) {

	gboolean result = FALSE;

	if (!strcasecmp(conf_get("db_interface"), "mysql")) setup_mysql();
	else if (!strcasecmp(conf_get("db_interface"), "newdip")) {
		g_set_error(gerr, DIPDB, DIPDB_IF_NOT_IMPLEMENTED,
				"database interface not yet implemented");
		goto exit_dipdb_interface_init;
	} else {
		g_set_error(gerr, DIPDB, DIPDB_UNKNOWN_IF,
				"unknown database interface: %s", conf_get("db_interface"));
		goto exit_dipdb_interface_init;
	}

	result = TRUE;

exit_dipdb_interface_init:

	return result;

}
static void setup_mysql(void) {

	dipdb_init = dipdb_mysql_init;

}
