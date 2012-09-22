/*
 * $Log: conf.h,v $
 * Revision 1.11  2004-05-22 08:54:43  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.10  2003/02/17 12:41:41  millis
 * Fixed Bug 108, make lines >=1024 terminate in '\n'
 *
 * Revision 1.9  2003/02/05 23:53:43  millis
 * Removed dipstats files.
 * Also added in new JUDGE_TZ variable, to specify timezone
 *
 * Revision 1.8  2003/01/13 22:29:48  millis
 * Merged from ustv
 *
 * Revision 1.7  2002/12/22 02:00:34  millis
 * Added DIE_STORMS setting for new random seed for storms
 *
 * Revision 1.6  2002/04/15 12:55:41  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.5  2001/07/17 09:55:31  greg
 * added GAME_DIR
 *
 * Revision 1.4  2001/07/08 22:57:07  miller
 * New defines
 *
 * Revision 1.3  2001/01/06 18:48:43  davidn
 * Changes to allow game creator to be automatically made master
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added new dip.conf variable. Plus allowed dip.conf path to be passed in as a parameter
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.2  1996/11/05 22:49:23  rpaar
 * NW's changes
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#ifndef CONF_H
#define CONF_H

#include <glib.h>

#define config(var)		conf_get(var)

#define JUDGE_PATH      config("JUDGE_PATH")
// No references in code #define 	RUNLISTMAP_CMD	config("RUNLISTMAP_CMD")
// No references in code #define	RUNDIPMAP_CMD 	config("RUNDIPMAP_CMD")
#define EP_CUSTODIAN    config("EP_CUSTODIAN")
#define XFORWARD        config("XFORWARD")
#define MAP_FILE        config("MAP_FILE")
// Replaced (no longer used) #define NO_CREATE       config("NO_CREATE")
// Replaced (no longer used) #define AUTO_MASTER     config("AUTO_MASTER")
// Replaced (no longer used) #define SYSLOG_FLAG	atoi(config("SYSLOG_FLAG"))
#define BLIND_CMD       config("BLIND_CMD")

/* this is the location of the judge configuration file,
 * it should probably be a full path name, as i don't think
 * that the initial working directory is known, and the function
 * that uses this may be called before the working directory is
 * set to a sane value.
 */
extern char* CONFIG_FILE;
/* Now defined as a command line opton '-C' */
extern char* CONFIG_DIR;

void   conf_destroy(void);
gchar *conf_get(gchar *var);
gint   conf_get_bool(gchar* key);
gint   conf_get_int(gchar* key);
gint   conf_init(void);
gint   conf_read_file(gchar *dir, gchar *bname);
gint   conf_set(gchar *var, gchar *val, gint init);
gint   conf_textual_set(gchar* line);

#ifdef UNITTEST
  extern GHashTable* conf_table;
#endif

#endif /* CONF_H */
