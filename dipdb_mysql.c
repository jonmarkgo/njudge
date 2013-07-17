/*
 * dipdb_mysql.c
 *
 *  Created on: Jul 15, 2013
 *      Author: cjw
 */

#include <glib.h>
#include <stdio.h>

#include <my_global.h>
#include <mysql.h>

#include "dipdb_mysql.h"
#include "conf.h"

static MYSQL* mydb = NULL;

GQuark dipdb_mysql_error_quark(void) {

	return g_quark_from_static_string("DIPDB_MYSQL");

}

gboolean dipdb_mysql_init(GError** gerr) {

	gboolean result = FALSE;

	if (!(mydb = mysql_init(NULL))) {
		g_set_error(gerr, DIPDB_MYSQL, MYSQL_INIT_ERROR,
				"mysql init error: %s", mysql_error(mydb));
		goto exit_dipdb_mysql_init;
	}

	if (!mysql_real_connect(mydb, conf_get("db_server"), conf_get("db_user"),
			conf_get("db_pass"), conf_get("db_name"), conf_get_int("db_server_port"),
			NULL,0)) {
		g_set_error(gerr, DIPDB_MYSQL, MYSQL_CONNECT_ERROR,
				"mysql connect error: %s", mysql_error(mydb));
		mysql_close(mydb);
		goto exit_dipdb_mysql_init;
	}

	result = TRUE;

exit_dipdb_mysql_init:

	return result;

}
