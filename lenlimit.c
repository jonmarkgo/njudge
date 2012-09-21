/*
 * $Log: lenlimit.c,v $
 * Revision 1.5  2003-05-07 22:38:22  millis
 * Fixed bug (it didn't do anything!)
 *
 * Revision 1.4  2003/03/19 22:24:22  millis
 * Small bug fix
 *
 * Revision 1.3  2003/03/18 16:13:57  millis
 * Use fputs instead of puts to avoid extra '\n'
 *
 * Revision 1.2  2003/03/18 13:53:24  millis
 * Added checking for filename exists or not
 *
 * Revision 1.1  2003/02/17 12:41:41  millis
 * Fixed Bug 108, make lines >=1024 terminate in '\n'
 *
 *
 */

/*  lenlimit.c
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* Passed filename to check */

    char *filename;
    FILE *fptr;
    char line[512];
    
    int mmsize = sizeof(line) -1;

    if (argc != 2) {
	printf("lenlimit <filename>:\n");
	printf("\n where <filename> is file to limit to %d bytes line length.\n\n", mmsize-2);
	return 1;
    }
    filename = argv[1];

    fptr = fopen(filename, "r");

    if (!fptr) {
	printf("File %s not found.\n", filename);
	return -1;
    }

    while (fgets(line, mmsize-1, fptr)) {
        if (strlen(line) >= mmsize-2) {
	    line[mmsize-2] = '\n';
	    line[mmsize-1] = '\0';
	}
	fputs(line,stdout);
    };

    fclose(fptr);

    return 0;
}
