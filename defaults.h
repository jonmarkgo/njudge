/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  defaults.h
   **
   **  Copyright 1987, Lowe.
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the 
   **  above notices are preserved and that due credit is given to Mr.
   **  Lowe.
   **
   **  The following parameters should be modified to fit your scene.
   **  See the file dip.h for possible values for the defaults.
 */

#ifndef _DEFAULTS_H
#define _DEFAULTS_H

#include "dip.h"

#define D_ACCESS         A_DIFF	/* A_* */
#define D_LEVEL          L_ANY	/* L_* */
#define D_FLAGS          F_NORATE|F_NONMR|F_NOPLAGUE|F_GRACEDAYS	/* F_* */
#define D_DEDICATE       -10
#define D_MOVE_CLOCK     1410
#define D_MOVE_MINT      12.0
#define D_MOVE_NEXT      72.0
#define D_MOVE_GRACE     168.0
#define D_MOVE_DELAY     0.5
#define D_MOVE_DAYS      "--TWTF-"
#define D_RETREAT_CLOCK  -1
#define D_RETREAT_MINT   0.0
#define D_RETREAT_NEXT   24.0
#define D_RETREAT_GRACE  72.0
#define D_RETREAT_DELAY  0.5
#define D_RETREAT_DAYS   "--TWTF-"
#define D_BUILDS_CLOCK   -1
#define D_BUILDS_MINT    0.0
#define D_BUILDS_NEXT    24.0
#define D_BUILDS_GRACE   72.0
#define D_BUILDS_DELAY   0.5
#define D_BUILDS_DAYS    "--TWTF-"

#endif
