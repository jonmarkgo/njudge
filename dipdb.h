/*
 * dipdb.h
 *
 *  Created on: Jul 15, 2013
 *      Author: cjw
 */

#ifndef DIPDB_H_
#define DIPDB_H_

#define DIPDB		dipdb_error_quark()

typedef enum dipdb_error_e {

	DIPDB_IF_NOT_IMPLEMENTED = 1,
	DIPDB_UNKNOWN_IF

} DipDBError;

gboolean dipdb_interface_init(GError** gerr);

gboolean (*dipdb_init)(GError** gerr);

#endif /* DIPDB_H_ */
