/*
 * $Log$
 */

/* 
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Jan 2002
 * to be included in njudge package.
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

/* --------------------------------------------------------
   Main parsing stuff
*/
int ParseInput (ArrayResStr* lPtr, IoOptions* ioPtr)
{
 int i, len;
 int error = 0;
 int loLimit, hiLimit;
 t_uchar lineBuffer[2048];
 t_uchar* str;
 unsigned pos, nOccurr;
 eResultState state = e_Result_Start;

 t_uchar powerNameColon[100];
 t_uchar powerNameColonUpcase[100];
 t_uchar powerAdjBlank[100];
 t_uchar powerAdjBlankUpcase[100];

 int countDislodged = 0;
 int lineNrDislodged = -1;

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

 // Firstly we will hide other movements
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
	 
	 if ( state==e_Result_Ownership )
	     continue;
	 
	 pos = FindString(powerNameColonUpcase,lineBuffer);
	 if ( pos==1 )
	     continue;
	 
	 str[0] = '@';
	 
#ifdef DEBUG_PARSE
	 fprintf(fDebug,"%s (STATE=%d)\n",str,(int)state);
#endif
     }// end IF (: was found)
     else {
	 if ( state==e_Result_Dislodged ) {
	     pos = FindString(powerAdjBlankUpcase,lineBuffer);
	     if ( pos==0 ) {
		 str[0] = '?';
	     }
	     else {
#ifdef DEBUG_PARSE
		 countDislodged++;
		 fprintf(fDebug,"PARSE: Dislodged [%s]\n",lineBuffer);
		 fprintf(fDebug,"PARSE: Dislodged count=%d\n",countDislodged);
#endif
	     }
	 }
#ifdef DEBUG_PARSE
	 fprintf(fDebug,"%s (STATE=%d)\n",str,(int)state);
#endif
     }// end ELSE (: was not found)
 }// end FOR line iterations

 if ( lineNrDislodged>0 ) {
     if ( countDislodged==0 ) {
	 // Remove line
	 lPtr->s[lineNrDislodged][0] = '?';
     }
 }
 
 return error;
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

