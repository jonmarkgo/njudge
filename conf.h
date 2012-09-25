
#ifndef CONF_H
#define CONF_H

#include <glib.h>

#define config(var)		conf_get(var)

#define JUDGE_PATH      config("JUDGE_PATH")
#define EP_CUSTODIAN    config("EP_CUSTODIAN")
#define XFORWARD        config("XFORWARD")
#define MAP_FILE        config("MAP_FILE")
#define BLIND_CMD       config("BLIND_CMD")

extern char* CONFIG_DIR;
extern char* CONFIG_FILE;

void   conf_destroy(void);
gchar *conf_get(gchar *var);
gint   conf_get_bool(gchar* key);
gint   conf_get_int(gchar* key);
gint   conf_init(void);
gint   conf_read_file(gchar *dir, gchar *bname, GError** err);
gint   conf_set(gchar *var, gchar *val, gint init, GError** err);
gint   conf_vset(GPtrArray* arr, GError** err);
gint   conf_textual_set(gchar* line, GError** err);

#ifdef UNITTEST
  extern GHashTable* conf_table;
#endif

typedef enum {

  	DIP_CONF_ERROR_FILE = 1,
  	DIP_CONF_ERROR_VOID_VALUE,
  	DIP_CONF_ERROR_PARSE

} DipConfError;

#endif /* CONF_H */
