/*
   ** $Log$

  */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include "dip.h"

int write();
int close();

struct dedication ded[MAXUSER];
int main()
{
    FILE *ind;
    int outd;
    int i;
    int nded;

   printf("Reading from ded.txt, "),

   ind = fopen("ded.txt", "r");
   fscanf(ind, "%d\n", &nded);

   printf("Converting %d records\n", nded);

    for (i = 0; i < nded; i++)
    {
       fscanf(ind, "%d %d %d %ld %ld %ld\n", 
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
