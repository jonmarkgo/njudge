/*
 * $Log$
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
    char line[1024];
    
    int max_size = sizeof(line) -1;

    if (argc != 2) {
	printf("lenlimit <filename>:\n");
	printf("\n where <filename> is file to limit to 1024 bytes line length.\n\n");
	return 1;
    }
    filename = argv[1];

    fptr = fopen(filename, "r");

    if (!fptr) {
	printf("File %s not found.\n", filename");
	return ;
    }

    while (fgets(line, max_size-1, fptr)) {
        if (strlen(line) >= max_size) {
	    line[max_size-1] = '\n';
	    line[max_size] = '\0';
	}
	fputs(line,stdout);
    };

    fclose(fptr);

    return 0;
}
