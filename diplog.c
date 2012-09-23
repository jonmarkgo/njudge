
#include <glib.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>

#include "conf.h"
#include "diplog.h"

static gboolean enable_syslog = 0;

void diplog_syslog_close(void) {

	if (!enable_syslog) return;

	diplog_syslog_entry(LOG_INFO, "Exit");

	closelog();

}
void diplog_syslog_open(gchar* log_alias) {

	if (!conf_get_bool("enable_syslog")) return;

	enable_syslog = 1;

	openlog(log_alias, 0, LOG_USER);

	// TODO: work to remove this solution, close syslog at main
	atexit(diplog_syslog_close);

}
void diplog_syslog_entry(int level, char *fmt, ...) {

	va_list args;

	if (!conf_get_bool("enable_syslog")) return;
    
    va_start(args, fmt);

    syslog(level, fmt, args);

    va_end(args);

}
