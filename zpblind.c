/*
 * $Log$
 */

/*
 * Coded by H. Moreira (henrique@moreira.dnsalias.net) in Jan 2002
 * to be included in njudge package.
 */

#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zparseb.h"

#ifdef yLINUX
#include <signal.h>
#endif

/* --------------------------------------------------------
   Signal - faultHandler
*/
static char myTempFilename[1000];

void faultHandler (int thisSignalId)
{
  fprintf(stderr,"Abnormal program exit! (%d)\n\
Removing file [%s]\n",thisSignalId,myTempFilename);
  if ( myTempFilename[0]!=0 ) {
    remove(myTempFilename);
  }
  exit(1);
}

/* --------------------------------------------------------
   Debug stuff
*/

/* --------------------------------------------------------
   Main function
*/
int go (IoOptions* ioPtr)
{
 unsigned iter, pos;
 int error, len;
 FILE* fIn = ioPtr->fIn;
 FILE* fOut = ioPtr->fOut;
 FILE* fTemp = ioPtr->fTemp;
 char* str;
 char aChr;
 t_uchar buf[2048];
 unsigned nEmptyLines = 0;

 static ArrayResStr lStr;

 ASSERTION(fIn!=nil,"fIn!=nil");

 ASSERTION(lStr.dblColonIdxFirst==0,"lStr.dblColonIdxFirst==0");
 ASSERTION(lStr.dblColonIdxSecond==0,"lStr.dblColonIdxSecond==0");
 ASSERTION(lStr.theLastIdx==0,"lStr.theLastIdx==0");

 ASSERTION(ioPtr->powerName!=nil && ioPtr->powerName[0]!=0,"optIO.powerName!=nil");
 ASSERTION(ioPtr->powerAdj!=nil && ioPtr->powerAdj[0]!=0,"optIO.powerAdj!=nil");

#ifdef yLINUX
 // Using temporary file xxx
 if ( fTemp==nil ) {
   ASSERTION(ioPtr->tempFilename==nil,"ioPtr->tempFilename==nil");

   sprintf(myTempFilename,"%s/%s","/tmp","zbXXXXXX");
   ioPtr->tempFilename = mktemp( myTempFilename );

   printf("CREATED TEMPORARY FILE: [%s|%s]\n",ioPtr->tempFilename,myTempFilename);
   
   ASSERTION(ioPtr->tempFilename!=nil && ioPtr->tempFilename[0]!=0,"tempFilename not alloc'd");
   fTemp = ioPtr->fTemp = fopen(ioPtr->tempFilename,"wt");
   if ( fTemp==nil ) {
     fprintf(stderr,"Cannot write temporary file %s\n",ioPtr->tempFilename);
     return 1;
   }
 }
#else
#error yLINUX must be defined!
#endif

 // The first task is pre-parsing:
 // picking only ':: Judge:' until 'The next phase of'
 ;
 // Read input
 while ( fgets(buf,2048,fIn)!=nil ) {
   str = buf;
   len = strlen(str);
   if ( len==0 )
     continue;
   ASSERTION(str[len-1]=='\n',"str[len-1]=='\\n'");
   str[--len] = 0;
   if ( len>0 && str[--len]=='\r' ) str[len] = 0;
   len--;
   // Trim right
   while ( len>=0 && (str[len]==' ' || str[len]=='\t') )
     str[len--] = 0;
   // Write temporary file
   fprintf(fTemp,"%s\n",str);
   ASSERTION(len<2048,"strlen(buf)<2048");
   lStr.size++;
   pos = FindString(":: ",str);
   if ( pos==1 ) {
     if ( lStr.dblColonIdxFirst==0 ) {
       lStr.dblColonIdxFirst = lStr.size;
     }
     else {
       lStr.dblColonIdxSecond = lStr.size;
     }
   }
   else {
     pos = FindString("The next phase of ",str);
     if ( pos==1 ) {
       lStr.theLastIdx = lStr.size;
     }
   }
 }

 if ( lStr.theLastIdx==0 )
   lStr.theLastIdx = lStr.size;

 fclose(fTemp);

 ASSERTION(ioPtr->tempFilename!=nil && ioPtr->tempFilename[0]!=0,"tempFilename!=nil");
 fTemp = ioPtr->fTemp = fopen(ioPtr->tempFilename,"rt");
 ASSERTION(fTemp!=nil,"fTemp!=nil");

 // Check first read
 if ( lStr.dblColonIdxFirst==0 ) {
   fprintf(stderr,"Cannot find '::' string.\n");
   return 1;
 }
 ASSERTION(lStr.dblColonIdxFirst>0 && lStr.dblColonIdxSecond>0,"lStr.dblColonIdxFirst>0 && lStr.dblColonIdxSecond>0");
 ASSERTION(lStr.dblColonIdxFirst+1==lStr.dblColonIdxSecond,"lStr.dblColonIdxFirst+1==lStr.dblColonIdxSecond");

 // Build the array lStr
 lStr.s = (t_uchar**)calloc( lStr.size+1, sizeof(t_uchar*) );
 ASSERTION(lStr.s!=nil,"lStr.s!=nil");
 lStr.s[0] = nil;
 for (iter=1; iter<=lStr.size; iter++) {
     fgets(buf,2048,fTemp);
     len = strlen(buf);
     ASSERTION(len>0,"len>0");
     str = buf;
     ASSERTION(str[len-1]=='\n',"str[len-1]=='\\n' <- error reading temporary file!");
     str[len-1] = 0;
     lStr.s[iter] = strdup(str);
 }
 fclose(fTemp);
 fTemp = ioPtr->fTemp = nil;

 if ( ioPtr->doDeleteTempFile )
   remove( ioPtr->tempFilename );

#ifdef DEBUG
 fprintf(fDebug,"%u lines\t:: at %u,%u (last=%u)\n%s\n%s<==\n",
	 lStr.size,
	 lStr.dblColonIdxFirst,
	 lStr.dblColonIdxSecond,
	 lStr.theLastIdx,
	 lStr.s[lStr.dblColonIdxFirst],
	 lStr.s[lStr.dblColonIdxSecond]);
 if ( lStr.theLastIdx<lStr.size )
   fprintf(fDebug,"%s<==\n",lStr.s[lStr.theLastIdx]);
 else
   fprintf(fDebug,"No next phase [%s]<==\n",lStr.s[lStr.theLastIdx]);
#endif
 
 error = ParseInput(&lStr,ioPtr);

#ifdef DEBUG_VERBOSE
 printf("ParseInput() returned %d for %u lines\n",error,lStr.size);
#endif

 // Blinded/hidden by ? and @ will not go to the output file
 for (iter=1; error==0 && iter<=lStr.size; iter++) {
     bool isEmpty;
     
     str = lStr.s[iter];
     aChr = str[0];
     isEmpty = aChr==0;

     switch ( aChr ) {
     case '?':
     case '@':
#ifdef DEBUG_VERBOSE
	 printf("!%03u %s!\n",iter,str);
#endif
	 break;
     default:
	 // Avoid printing out two consecutive empty lines
	 if ( isEmpty ) {
	     nEmptyLines++;
	     if ( nEmptyLines>=2 ) {
#ifdef DEBUG_OUTPUT
		 printf("Avoid empty line. (%u)\n",nEmptyLines);
#endif
		 continue;
	     }
	 }
	 else {
	     nEmptyLines = 0;
	 }
	 fprintf(fOut,"%s\n",str);
	 break;
     }
 }
 
 ArrayDelete(&lStr);

 return error;
}

/* --------------------------------------------------------
   Auxiliary functions
*/
int OpenFileInput (IoOptions* ioPtr, char* str)
{
 ioPtr->doUseFileIn = true;
 ASSERTION(ioPtr!=0 && str!=0 && str[0]!=0,"ioPtr!=0 && str!=0 && str[0]!=0");
 ioPtr->fIn = fopen(str,"rt");
 if ( ioPtr->fIn==nil ) {
     fprintf(stderr,"Cannot open file %s\n",str);
     return 1;
 }
#ifdef DEBUG
 fprintf(fDebug,"Using input file %s\n",str);
#endif
 ioPtr->inputFilename = str;
 return 0;
}
/* --------------------------------------------------------
   Main program
*/
int main (int argc, char* argv[], char** envp)
{
 int i, n = argc-1;
 int error = 0;

 unsigned argCount;
 char* str;

 static IoOptions optIO;

 bool hasDash, hasEqual;
 char chr;

 ASSERTION(argv!=nil,"argv!=nil");
 ASSERTION(envp!=nil,"envp!=nil");
 ASSERTION(n>=0,"n>=0");

 ASSERTION(optIO.fIn==nil,"optIO.fIn==nil");
 ASSERTION(optIO.fOut==nil,"optIO.fOut==nil");

 optIO.fIn = stdin;
 optIO.fOut = stdout;
 optIO.doPreParsing = true;
 optIO.doDeleteTempFile = true;

#ifdef yLINUX
 signal(SIGINT, faultHandler);
 signal(SIGQUIT, faultHandler);
 signal(SIGUSR1, faultHandler);
 signal(SIGALRM, faultHandler);
 signal(SIGSEGV, faultHandler);
 signal(SIGCLD, SIG_IGN);
#endif

 // Parse args
 if ( n<2 ) {
     printf("zpblind - Parse input and generate output of 'blind' variant - v0.3\n\
\n\
Usage:\n\
  zpblind [options] <power-noun> <power-adjective> [inputFile]\n\
\n\
where\n\
  <power-noun> is power name\n\
  <power-adjective> is power's possesive adjective.\n\
\n\
Options are:\n\
 -i=InputFile
 -o=OutputFile\n\
 -t=TemporaryFile\n\
");
#ifdef yLINUX
     printf("yLINUX defined.\n");
#else
     printf("non-yLINUX defined.\n");
#endif
#ifdef DEBUG
     printf("DEBUG define.\n");
#endif
     return 0;
 }

 for (i=1, argCount=0; i<=n; i++) {
     str = argv[i];
     ASSERTION(str!=nil,"str!=nil");
     hasDash = str[0]=='-';
     hasEqual = hasDash && str[2]=='=';
     chr = hasDash ? str[1] : '\0';
     if ( hasEqual ) {
	 str += 3;
	 ASSERTION(str!=nil && str[0]!=0,"str!=nil && str[0]!=0");
     }
     if ( hasEqual ) {
	 ASSERTION(str!=nil && str[0]!=0,"str!=nil && str[0]!=0");
	 switch ( chr ) {
	 case 'i':
	     if ( OpenFileInput(&optIO,str)!=0 )
		 return 1;
	     break;
	 case 'o':
	     optIO.doUseFileOut = true;
	     //
	     optIO.fOut = fopen(str,"wt");
	     if ( optIO.fOut==nil ) {
		 fprintf(stderr,"Cannot rewrite file %s\n",str);
		 return 1;
	     }
	     fclose( optIO.fOut );
#ifdef DEBUG
	     fprintf(fDebug,"Using output file %s\n",str);
#endif
	     optIO.outputFilename = str;
	     remove( str );
	     optIO.fOut = fopen(str,"wt");
	     break;
	 case 't':
	     optIO.fTemp = fopen(str,"wt");
	     if ( optIO.fTemp==nil ) {
		 fprintf(stderr,"Cannot use/write temporary file %s\n",str);
		 return 1;
	     }
	     fclose( optIO.fTemp );
	     optIO.tempFilename = str;
	     remove( str );
	     optIO.fTemp = fopen(optIO.tempFilename,"wt");
	     ASSERTION(optIO.fTemp!=nil,"optIO.fTemp!=nil");
	     optIO.doDeleteTempFile = false;
	     break;
	 default:
	     fprintf(stderr,"Bad option '%c'\n",chr);
	     break;
	 }
     }
     else {
	 argCount++;
	 switch ( argCount ) {
	 case 1:
	     optIO.powerName = str;
	     break;
	 case 2:
	     optIO.powerAdj = str;
	     break;
	 case 3:
	     if ( optIO.doUseFileIn ) {
		 fprintf(stderr,"Too many arguments.\n(Input file already specified!)\n");
		 return 1;
	     }
	     if ( OpenFileInput(&optIO,str)!=0 ) {
		 return 1;
	     }
	     break;
	 default:
	     fprintf(stderr,"Too many arguments.\n");
	     return 1;
	 }
     }
 }

 error = go(&optIO);

 /* Close all files
  */
 if ( optIO.doUseFileIn ) {
     ASSERTION(optIO.fIn!=stdin,"optIO.fIn!=stdin");
     fclose(optIO.fIn);
 }
 if ( optIO.doUseFileOut && optIO.fOut!=nil && optIO.fOut!=stdout ) {
     fclose(optIO.fOut);
 }
 if ( optIO.fTemp ) {
#ifdef DEBUG_VERBOSE
   fprintf(fDebug,"Closing temporary file %s\n",optIO.tempFilename);
#endif
   fclose(optIO.fTemp);
 }

 return error;
}

