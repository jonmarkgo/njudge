/*
   ** $Log: ded_to_ascii.c,v $
   ** Revision 1.3  2006-03-17 02:45:04  nzmb
   ** write and close are standard under POSIX. Add unistd.h and fix bogus declaration. This fixes compile problem on HP-UX 11i
   **
   ** Revision 1.2  2002/08/27 22:27:47  millis
   ** Updated for automake/autoconf functionality
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
    int ind;
    FILE *outd;
    int i;
    int nded;

   printf("Reading from dip.ded ");
    ind = open ("dip.ded", O_RDONLY);
   nded = read(ind, ded, MAXUSER * sizeof(ded[0])) / sizeof(ded[0]);
   close(ind);

   printf("Converting out %d nded records\n", nded);

   printf("Writing to ded.txt");
   outd = fopen("ded.txt", "w");

	fprintf(outd,"%d\n", nded);
   for (i=0; i < nded; i++)
   {
       if (ded[i].r != 0 )
	   ded[i].r += 50;
       fprintf(outd,"%d %d %d %ld %ld %ld\n", 
        ded[i].r, ded[i].i1, ded[i].i2, ded[i].ls, ded[i].md, ded[i].d0);
   }
   fclose(outd);
   printf("\n\nUse ded.txt to consult or use with ascii_to_ded to convert back.\n");
   return 0;
}
