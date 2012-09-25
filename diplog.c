
#include <glib.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "conf.h"
#include "config.h"
#include "diplog.h"

static FILE* diplog_fp = NULL;
static gint  diplog_fd = -1;
static gboolean enable_syslog = 0;

gint dip_log_error_quark(void) {

	return g_quark_from_static_string("dip_log_error_quark");

}
void diplog_close(void) {

	g_assert(diplog_fd > -1 && diplog_fp != NULL);

	unlockfd(diplog_fd, 0);
	fclose(diplog_fp);
	close(diplog_fd);

	diplog_fd = -1;
	diplog_fp = NULL;

}
void diplog_entry(gchar* fmt, ...) {

	gchar date_str[20];
	gchar* fmt_str;
	time_t now;
	va_list args;

	now = time(NULL);
	strftime(date_str, 20, "%F %T", localtime(&now));
	fmt_str = g_strdup_printf("%s: %s\n", date_str, fmt);

	va_start(args, fmt);

	vfprintf(diplog_fp, fmt_str, args);

	va_end(args);

	g_free(fmt_str);
	g_free(date_str);

}
gint diplog_open(GError** err) {

	gint rtn = 0;
	gchar* fn = conf_get("log_file");

	g_assert(diplog_fd == -1 && diplog_fp == NULL);

	if ((diplog_fd = open(fn, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0) {
		g_set_error(err, DIP_LOG_ERROR, DIP_LOG_ERROR_FILE,
				"unable to open log file - %s: %s", fn, g_strerror(errno));
		goto exit_diplog_open;
	}

	if (lockfd(diplog_fd, 0) < 0) {
		g_set_error(err, DIP_LOG_ERROR, DIP_LOG_ERROR_LOCK,
				"unable lock log file - %s: %s", fn, g_strerror(errno));
		close(diplog_fd);
		goto exit_diplog_open;
	}

	if ((diplog_fp = fdopen(diplog_fd, "a")) == NULL) {
		g_set_error(err, DIP_LOG_ERROR, DIP_LOG_ERROR_STREAM,
				"unable fdopen stream - %s: %s", fn, g_strerror(errno));
		unlockfd(diplog_fd, 0);
		close(diplog_fd);
		goto exit_diplog_open;

	}

	rtn = 1;

exit_diplog_open:

	return rtn;

}
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
