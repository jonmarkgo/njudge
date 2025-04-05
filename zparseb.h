/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Apr 2003
 * included in njudge package.
 */

#ifndef ZPARSEB_X_H
#define ZPARSEB_X_H

#include "zmacro.h"
/* --------------------------------------------------------
   Structures
*/
typedef struct {
  char* powerName;
  char* powerAdj;
  bool doUseFileIn;
  bool doUseFileOut;
  bool varHideAllCentre;
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
  e_Result_Start = 0,
  e_Result_Moves = 1,
  e_Result_Dislodged = 2,
  e_Result_Ownership = 3,
  e_Result_Builds = 4,
  e_Result_invalid      // Must be the last+1 for debugging (i.e.=5)
} eResultState;
/* --------------------------------------------------------
   Own definitions
*/
unsigned FindString (char* sub, char* str) ;
unsigned FindStringDislodged (char* sub, char* str) ;

int ParseInput (ArrayResStr* lPtr, IoOptions* ioPtr) ;

void ArrayDelete (ArrayResStr* lPtr) ;

#endif //ZPARSEB_X_H

