/* zpblind.c
   - Diplomacy Blind Parsing - blind code - Version 0.7
 */

/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Dec 2002
 * included in njudge package.
 */

#include <string.h>
#include <stdlib.h>
#include "zparseb.h"

/* --------------------------------------------------------
   Utilities
*/
char UpChar (char chr)
{
 if ( chr>='a' && chr<='z' ) return chr-32;
 return chr;
}

bool IsDigit (char chr)
{
 return chr>='0' && chr<='9';
}

unsigned FindString (char* sub, char* str)
{
 char *ptr;
 if ( str==NULL || sub==NULL ) return 0;
 ptr = strstr(str,sub);
 if ( ptr ) return ptr-str+1;
 return 0;
}

unsigned FindChrOccurr (char aChr, char* str, unsigned* nOccurr)
{
 unsigned iter = 0, pos = 0;
 char chr;

 *nOccurr = 0;
 while ( (chr = *(str++))!=0 ) {
   iter++;
   if ( chr==aChr ) {
     if ( (*nOccurr)==0 )
       pos = iter;
     (*nOccurr)++;
   }
 }
 return pos;
}

void CopyStringUp (char* inStr, char* resStr)
{
 char chr;
 if ( inStr==nil )
   return;
 while ( (chr = *(inStr++))!=0 ) {
   *resStr = UpChar(chr);
   resStr++;
 }
 *resStr = 0;
}

#ifdef DEBUG_any
static char* stateToString[e_Result_invalid]={
  "START",
  "MOVE",
  "DISL.",
  "OWNER",
  "BUILDS"
};
#endif

/* --------------------------------------------------------
   Variant functions
*/
bool VarHideDislodged (IoOptions* ioPtr)
{
 return false;
}

bool VarHideOwnership (IoOptions* ioPtr)
{
 return ioPtr->varHideAllCentre;
}

#ifdef DEBUG_UNUSED
bool VarHideBuilds (IoOptions* ioPtr)
{
 return ioPtr->varHideAllCentre;
}
#endif

/* --------------------------------------------------------
   Main parsing stuff
*/
int ParseInput (ArrayResStr* lPtr, IoOptions* ioPtr)
{
 int i, k, len;
 int nLenNon0;
 int loLimit, hiLimit;
 t_uchar lineBuffer[2048];
 t_uchar* str;
 t_uchar* pResultStr;
 unsigned pos, nOccurr;
 eResultState state = e_Result_Start;

 t_uchar powerNameColon[100];
 t_uchar powerNameColonUpcase[100];
 t_uchar powerAdjBlank[100];
 t_uchar powerAdjBlankUpcase[100];

 int countDislodged = 0;
 int lineNrDislodged = -1;

 // Other useful vars initialised here
 char* lastStr;
 int lastLen;
 t_uchar lastChr, lastChrLineBefore;
 bool wasLastLineCut;

 //
 ASSERTION(lPtr!=nil,"lPtr!=nil");
 ASSERTION(ioPtr!=nil,"ioPtr!=nil");
 //

 sprintf(powerNameColon,"%s: ",ioPtr->powerName);
 CopyStringUp(powerNameColon,powerNameColonUpcase);

 sprintf(powerAdjBlank," %s ",ioPtr->powerAdj);
 CopyStringUp(powerAdjBlank,powerAdjBlankUpcase);

 loLimit = lPtr->dblColonIdxSecond;
 hiLimit = lPtr->theLastIdx;

 // Just checking no weird characters are present!
 for (i=1; i<=lPtr->size; i++) {
     str = lPtr->s[i];
#ifdef DEBUG_VERBOSE
     printf("Line%3d/%3d [%s]\n",i,lPtr->size,str);
#endif
     ASSERTION(str && str[0]!='@' && str[0]!='?',"Weird characters!");
     ASSERTION(str[0]!='\n' && (str[0]==0 || str[0]>=' '),"Weird character: NewLine?");
 }

 // Check what kind of results: Adjustments (builds F9999B)? or Movements?
 // E.g.:
 //   :: Deadline: F1912B Mon Dec 03 2001 02:27:06 +0100!
 //   !
 //   Adjustment orders for Winter of 1912. (groggy.051)!

 pResultStr = 0;
 for (i=loLimit+1, k=1, nLenNon0=0; i<hiLimit && k<=3; i++, k++) {
     str = lPtr->s[i];
     len = strlen(str);
     if ( len>0 ) {
       pResultStr = str;
       nLenNon0++;
     }
 }
 ASSERTION(nLenNon0==1 && pResultStr!=0,"nLenNon0==1");
 pos = FindString("Adjustment orders for ",pResultStr);
 if ( pos==1 ) {
     state = e_Result_Builds;
 }

 // Main stuff: will hide other movements
 // (careful with dislodged and ownerships)

 for (i=loLimit+1; i<hiLimit; i++) {
     str = lPtr->s[i];
     len = strlen(str);

     if ( len==0 ) {
       if ( state==e_Result_Ownership ) {
	 state = e_Result_Builds;
#ifdef DEBUG_PARSE
	 fprintf(fDebug,"STATE now Builds_\n");
#endif
       }
       continue;
     }

     //
     // We are interested in lines with something to parse...
     ASSERTION(len>0,"len>0");

     //
     // Start parsing this line now!
     //
     CopyStringUp( str, lineBuffer );
     pos = FindChrOccurr(':',lineBuffer,&nOccurr);
     // If there is no character in front of colon (:),
     // or if there are more than two colons, then continue!
     if ( nOccurr>2 ) continue;
     if ( pos==len ) {
       pos = FindString("Ownership ",str);
       if ( pos==1 ) {
	 ASSERTION(state<e_Result_Ownership,"state<e_Result_Ownership");
	 i++;
	 ASSERTION(i+1<hiLimit,"i+1<hiLimit");
	 state = e_Result_Ownership;
#ifdef DEBUG_PARSE
	 fprintf(fDebug,"STATE now Ownership_\n");
#endif
       }
       else {
	 pos = FindString("The following ",str);
	 if ( pos==1 ) {
	   lineNrDislodged = i;
	   i++;
	   ASSERTION(i+1<hiLimit,"i+1<hiLimit");
	   state = e_Result_Dislodged;
#ifdef DEBUG_PARSE
	   fprintf(fDebug,"STATE now Dislodged_\n");
#endif
	 }
       }// end IF pos==1
       continue;
     }// end IF pos==len

     if ( pos>0 ) {
       ASSERTION(FindString(": ",str)>0,str);

       // We have something like 'Italy: Fleet Naples -> Ionian Sea.'
       // or 'Austria: Budapest, Bulgaria.'
       //
       // In order to distinguish them
       // (both must be reported!)
       // we use the state:
       //    Before e_Result_Ownership : report IF...
       //    During e_Result_Ownership : report ALL
       //    During e_Result_Builds    : report IF...

       pos = FindString(powerNameColonUpcase,lineBuffer);
       switch ( state ) {
       case e_Result_Start:
       case e_Result_Moves:
       case e_Result_Dislodged:
	 if ( pos==1 ) continue;
	 break;
       case e_Result_Ownership:
	 if ( VarHideOwnership(ioPtr) ) {
	   if ( pos==1 ) continue;
	 }
	 else {
	   continue;
	 }
	 break;
       case e_Result_Builds:
	 if ( pos==1 ) continue;
	 break;
       default:
	 ASSERTION_FALSE("Invalid state");
       }// end SWITCH-CASE

       str[0] = '@';

#ifdef DEBUG_PARSE
       fprintf(fDebug,"Line %03d:Z: %s (STATE=%s)\n",
	       i,
	       str,
	       stateToString[state]);
#endif
     }// end IF (: was found)
     else {
	 //
	 // Here, character : was not found
	 // We have to distinguish when somebody is dislodged,
	 // or if we are in some other state.
	 //
	 switch ( state ) {
	 case e_Result_Start:
	     state = e_Result_Moves;
	     break;
	 case e_Result_Moves:
	     // Care must be taken in this case in order to
	     // avoid previous line from being accepted here.
#ifdef DEBUG_VAR
	     fprintf(stderr,"Line %03d:M: %s\n",i,str);
#endif
	     lastChr = str[0];
	     if ( i>2 && lastChr==' ' ) {
		 lastStr = lPtr->s[i-1];
		 wasLastLineCut = lastStr[0]=='@';
		 if ( wasLastLineCut )
		     str[0] = '@';
	     }
	     break;
	 case e_Result_Dislodged:
	     pos = FindString(powerAdjBlankUpcase,lineBuffer);
	     if ( pos==0 || VarHideDislodged(ioPtr)==true ) {
		 str[0] = '?';
	     }
	     else {
		 countDislodged++;
#ifdef DEBUG_VAR
		 fprintf(fDebug,"PARSE: Dislodged [%s]\n",str);
		 fprintf(fDebug,"PARSE: Dislodged count=%d\n",countDislodged);
#endif
	     }
	     break;
	 case e_Result_Ownership:
	     // Care must be taken in this case in order to
	     // avoid previous line from being accepted here.
	     lastChr = str[len-1];
	     if ( i<=2 )
	       break;
	     lastStr = lPtr->s[i-1];
	     wasLastLineCut = lastStr[0]=='@';
	     lastLen = strlen(lastStr);
	     if ( lastLen<1 )
	       break;
	     lastChrLineBefore = lastStr[lastLen-1];
	     switch ( lastChr ) {
	     case '.':
	       // OK, check here if the last line was cut
	       // and it contains , at the end of the string
	       //
	       // E.g.:
	       //   @ustria:  Moscow, St Petersburg,
	       //   @         Trieste, Venice,
	       //          ==>Vienna, Warsaw.<==
	       //
	     case ',':
	       // E.g.:
	       //   @ustria:  Moscow, St Petersburg,
	       //          ==>Trieste, Venice,<==
	       //             Vienna, Warsaw.
	       //
	       if ( wasLastLineCut && lastChrLineBefore==',' )
		 str[0] = '@';
	       break;
	     default:
	       break;
	     }// end SWITCH-CASE during ownership (last-chr)
	     break;
	 case e_Result_Builds:
	     break;
	 default:
	     break;
	 }// end SWITCH-CASE (state)

	 //
	 // OK, nothing more to do if character ':' was not found
	 //
#ifdef DEBUG_PARSE
	 fprintf(fDebug,"Line %03d:N: %s (STATE=%s)\n",
		 i,
		 str,
		 stateToString[state]);
#endif
     }// end ELSE (: was not found)

     // End of major loop
 }// end FOR line iterations

 //
 // Check whether there are any dislodged units or not
 //
 if ( lineNrDislodged>0 ) {
     if ( countDislodged==0 ) {
	 // Remove line
	 lPtr->s[lineNrDislodged][0] = '?';
     }
 }

 // and return 0 always
 return 0;
}

/* --------------------------------------------------------
   ArrayResStr
*/
void ArrayDelete (ArrayResStr* lPtr)
{
 unsigned i, n = lPtr->size;
 lPtr->s[0] = nil;
 for (i=1; i<=n; i++)
   free( lPtr->s );
}

