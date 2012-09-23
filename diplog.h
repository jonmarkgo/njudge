#ifndef DIPLOG_H
#define DIPLOG_H

#include <syslog.h>

void diplog_syslog_close(void);
void diplog_syslog_open(gchar* log_alias);
void diplog_syslog_entry(int level, char *fmt, ...);

#endif /* DIPLOG_H */
