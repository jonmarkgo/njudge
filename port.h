/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  port.h

 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

/* TODO the getdtablesize() stuff is deprecated, and only used in dip.c */

#ifndef _PORT_H
#define _PORT_H

#if defined(SYSV) || defined(_HPUX_SOURCE) || defined(SOLARIS) || defined(sun)
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define lockfd(f,nb)  lockf(f, (nb ? F_TLOCK : F_LOCK), 0)
#define random() mrand48()
#define srandom(seed) srand48((long)seed)
#else				/* SYSV */

/* #  include <strings.h> */
#include <sys/file.h>
#define lockfd(f,nb) flock(f, LOCK_EX | (nb ? LOCK_NB : 0))

#endif				/* SYSV */

#endif
