/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Sep 2002
 * included in njudge package.
 */

#ifndef ZMACRO_X_H
#define ZMACRO_X_H

#include <stdio.h>

#ifndef yLINUX
#define yLINUX
#endif

#ifdef DEBUG
#define DEBUG_PARSE
#endif

#if defined(DEBUG_PARSE) || defined(DEBUG_OUTPUT) || defined(DEBUG_VAR)
#define DEBUG_any
#endif

#ifdef DEBUG_any
#ifndef fDebug
#define fDebug stderr
#endif
#endif //if DEBUG...

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
#endif //TEST_YACC
#endif
/*
  .
*/

typedef char bool;
typedef unsigned char t_uchar;

#endif //ZMACRO_X_H

