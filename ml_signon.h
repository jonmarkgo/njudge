/*
   ** $Log: ml_signon.h,v $
   ** Revision 1.1  1998-02-28 17:51:14  david
   ** Initial revision
   **
   ** Revision 1.2  1996/11/05 23:23:54  rpaar
   ** AddeLog message comments to header
   **
 */

#ifndef _ml_signon_h
#define _ml_signon_h

/* for WILD_PLAYER */
#include "dip.h"

int mail_signon(char *s);
int mail_access(int ignore, int userid, int siteid, int level, int *idx);
void mail_igame(void);
int chkpref(char *s, int wp[WILD_PLAYER], int wv[WILD_PLAYER]);

#endif
