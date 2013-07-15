/*
 * dipdb_mysql.h
 *
 *  Created on: Jul 15, 2013
 *      Author: cjw
 */

#ifndef DIPDB_MYSQL_H_
#define DIPDB_MYSQL_H_

#include <glib.h>
#include <stdio.h>

typedef enum dipdb_mysql_error_e {

	MYSQL_INIT_ERROR = 1,
	MYSQL_CONNECT_ERROR

} DipDBMySQLError;

#define DIPDB_MYSQL		dipdb_mysql_error_quark()

GQuark   dipdb_mysql_error_quark(void);
gboolean dipdb_mysql_init(GError** gerr);

#endif /* DIPDB_MYSQL_H_ */
