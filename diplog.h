#ifndef DIPLOG_H
#define DIPLOG_H

#include <syslog.h>

#define DIP_LOG_ERROR	dip_log_error_quark()

typedef enum {

	DIP_LOG_ERROR_FILE = 1,
	DIP_LOG_ERROR_LOCK,
	DIP_LOG_ERROR_STREAM

} DipLogError;

gint dip_log_error_quark(void);

void diplog_close(void);
void diplog_entry(gchar* fmt, ...);
gint diplog_open(gchar* logr, GError** err);
void diplog_syslog_close(void);
void diplog_syslog_open(gchar* log_alias);
void diplog_syslog_entry(int level, char *fmt, ...);

#endif /* DIPLOG_H */
