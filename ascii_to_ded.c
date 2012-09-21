/*
   ** $Log: ascii_to_ded.c,v $
   ** Revision 1.5  2006-03-17 02:45:04  nzmb
   ** write and close are standard under POSIX. Add unistd.h and fix bogus declaration. This fixes compile problem on HP-UX 11i
   **
   ** Revision 1.4  2002/08/27 22:27:44  millis
   ** Updated for automake/autoconf functionality
   **
   ** Revision 1.3  2002/05/14 23:44:46  miller
   ** Added simple error checking for ded.txt on fopen()
   **
   ** Revision 1.2  2002/04/18 04:44:30  greg
   ** Added the following commands:
   ** - unstart
   ** - set secret
   ** - set [prflist|prfrand|prfboth]
   **
   ** Fixed Set Absence so that "to" is not case sensitive
   **
   ** Fixed Quiet games so that new players are announced
   ** before the game starts
   **
   ** Fixed ascii_to_ded.c so thatit no longer generates an
   ** error when compiled
   **
   ** Revision 1.1  2002/04/11 23:49:00  miller
   ** First draft.
   **

  */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include "config.h"
#include "dip.h"

struct dedication ded[MAXUSER];
int main()
{
    FILE *ind;
    int outd;
    int i;
    int nded;

   printf("Reading from ded.txt, "),

   ind = fopen("ded.txt", "r");
   if (!ind) {
     fprintf(stderr, "Error opening ded.txt\n");
     return 1;
   }
  
   fscanf(ind, "%d\n", &nded);

   printf("Converting %d records\n", nded);

    for (i = 0; i < nded; i++)
    {
       fscanf(ind, "%hd %hd %hd %ld %ld %ld\n", 
	&ded[i].r, &ded[i].i1, &ded[i].i2, 
	&ded[i].ls, &ded[i].md, &ded[i].d0);
/*
	if (ded[i].r !=0)
		ded[i].r +=50;
*/
    }
    fclose(ind);

   printf("Writing to dip.oded.\n");
   outd = open("dip.oded", O_RDWR | O_CREAT, 0666);
   nded = write(outd, ded, nded * sizeof(ded[0]) );
   close(outd);


   printf("\nIf all is ok, copy dip.oded to replace dip.ded.\n\n");
   return 0;
}
