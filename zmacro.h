/*
 * $Log$
 * Revision 1.1  2002/04/11 13:23:32  miller
 * First entry, from testing on USTV
 *
 */

/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Jan 2002
 * to be included in njudge package.
 */

#ifndef ZMACRO_X_H
#define ZMACRO_X_H

#include <stdio.h>
#define yLINUX

#ifdef DEBUG
#define fDebug stderr
#endif

#define nil 0
#define false 0
#define true 1

#ifdef yLINUX
void faultHandler (int thisSignalId) ;

#define ASSERTION_FALSE(y) { fprintf(stderr,"ASSERTION_FALSE:%s\n",y);faultHandler(0); }
#define ASSERTION(x,y) { if((x)!=1){fprintf(stderr,"ASSERTION:%s\n",y);faultHandler(0);} }
#else
#define ASSERTION_FALSE(y) { fprintf(stderr,"ASSERTION_FALSE:%s\n",y);exit(1); }
#define ASSERTION(x,y) { if((x)!=1){fprintf(stderr,"ASSERTION:%s\n",y);exit(1);} }
#endif

#define TRACERULE(s1,s2) printf("RULE:%s:%s\n",s1,s2)

/*
  Avoid mutually exclusive defines
 */
#ifdef TEST_LEX
#ifdef TEST_YACC
#error Test LEX and YACC both defined?
#endif
#endif
/*
  .
*/

typedef char bool;
typedef unsigned char t_uchar;

#endif

