/* This file defines the diplogging functions */

#include "diplog.h"
#include <stdlib.h>
#include "functions.h"
#include "conf.h"
#include <stdarg.h>

void opendiplog( char *x, int level, int priv) 
{
    if (!SYSLOG_FLAG) return;

    openlog( x, level, priv);
    on_exit(dipexit, 0L);
}

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

static void diplog(int level, char *fmt, ...)
{
    va_list args;    
    if (!SYSLOG_FLAG) return;
    
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

