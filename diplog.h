#ifndef _DIPLOG_H
#define _DIPLOG_H

#ifdef HAS_ON_EXIT
void dipexit(int exit_code, void* );
#else
void dipexit(void);
#endif
void opendiplog(char *name, int, int);

#include <syslog.h>

#define OPENDIPLOG(x) opendiplog(x, 0, LOG_USER)

void DIPERROR(char *fmt, ...);
void DIPWARN(char *fmt, ...); 
void DIPNOTICE(char *fmt, ...); 
void DIPINFO(char *fmt, ...); 
void DIPDEBUG(char *fmt, ...); 
#define CLOSEDIPLOG closelog

#endif
