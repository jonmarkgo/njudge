/*
 * $Log$
 */

/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Jan 2002
 * to be included in njudge package.
 */

#ifndef ZPARSEB_X_H
#define ZPARSEB_X_H

#ifdef DEBUG
#define DEBUG_PARSE
#else
#define fDebug stdout
#endif

#include "zmacro.h"
/* --------------------------------------------------------
   Structures
*/
typedef struct {
  char* powerName;
  char* powerAdj;
  bool doUseFileIn;
  bool doUseFileOut;
  FILE* fIn;
  FILE* fOut;
  FILE* fTemp;
  char* inputFilename;
  char* outputFilename;
  char* tempFilename;
  bool doPreParsing;
  bool doDeleteTempFile;
} IoOptions;
//typedef myIoOptions IoOptions;

typedef struct {
  unsigned size;
  t_uchar** s;
  unsigned dblColonIdxFirst;
  unsigned dblColonIdxSecond;
  unsigned theLastIdx;
} ArrayResStr;

typedef enum {
  e_Result_Start,
  e_Result_Moves,
  e_Result_Dislodged,
  e_Result_Ownership,
  e_Result_Builds
} eResultState;
/* --------------------------------------------------------
   Own definitions
*/
unsigned FindString (char* sub, char* str) ;

int ParseInput (ArrayResStr* lPtr, IoOptions* ioPtr) ;

void ArrayDelete (ArrayResStr* lPtr) ;

#endif

