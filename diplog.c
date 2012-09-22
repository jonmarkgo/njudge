/* This file defines the diplogging functions */

#include <stdlib.h>
#include <stdarg.h>
#include <glib.h>

#include "conf.h"
#include "diplog.h"
#include "functions.h"

void opendiplog( char *x, int level, int priv) 
{
    if (!conf_get_bool("enable_syslog")) return;

    openlog( x, level, priv);
#ifdef HAS_ON_EXIT
    on_exit(dipexit, 0L);
#else
    atexit(dipexit);
#endif
}

#ifdef HAS_ON_EXIT
void dipexit(int exit_code, void *ignore_me)
{
    if (!SYSLOG_FLAG) return;

    if (exit_code == 0) {
	DIPINFO("Exit with error code 0");
    } else { 
	DIPNOTICE("Exit with error code != 0"); 
    }

    CLOSEDIPLOG();
}
#else
void dipexit()
{
    if (!conf_get_bool("enable_syslog")) return;

        DIPINFO("Exit");
    CLOSEDIPLOG();
}
#endif

static void diplog(int level, char *fmt, ...)
{
    va_list args;    
    if (!conf_get_bool("enable_syslog")) return;
    
    va_start(args, fmt);
    syslog(level, fmt, args);
    va_end(args);
}

void  DIPERROR(char *fmt, ...)
{

    va_list args;
    va_start(args, fmt);
    diplog(LOG_ERR, fmt, args);
    va_end(args);
}

void DIPWARN(char *fmt, ...)
{
    va_list args;    
    va_start(args, fmt);
    diplog(LOG_WARNING, fmt, args);
    va_end(args);

}

void DIPNOTICE(char *fmt, ...)
{
    va_list args;    
    va_start(args, fmt);
    diplog(LOG_NOTICE, fmt, args);
    va_end(args);

}

void DIPINFO(char *fmt, ...)
{
    va_list args;    
    va_start(args, fmt);
    diplog(LOG_INFO, fmt, args);
    va_end(args);

}

void DIPDEBUG(char *fmt, ...)
{
    va_list args;    
    va_start(args, fmt);
    diplog(LOG_DEBUG, fmt, args);
    va_end(args);
}

