/*
 * $Log$
 */

/*  size_check.c
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
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    /* Passed filename to check */

    char *filename;
    FILE *fptr;
    int ss, si,sl;
    struct stat sbuf;

    if (argc != 2) {
	printf("size_check <filename>:\n");
	printf("\n where <filename> is file to check number byte sizes.\n\n");
	return 1;
    }
    filename = argv[1];

    if (stat(filename, &sbuf)) {
	fptr = fopen(filename, "w");
	fprintf(fptr, "%d %d %d", sizeof(short), sizeof( int), sizeof (long));
	fclose(fptr);
    } else {
	fptr = fopen(filename, "r");
	fscanf( fptr, "%d %d %d", &ss, &si, &sl);
	fclose(fptr);
	if (ss != sizeof(short) || si != sizeof(int) || sl != sizeof(long)) {
	    printf("Size mismatch:\n");
	    printf("Expected: short=%d bytes, int=%d bytes, long=%d bytes.\n",
		   sizeof(short), sizeof(int), sizeof(long));
	    printf("but got : short=%d bytes, int=%d bytes, long=%d bytes.\n",
		   ss, si, sl);
	    return 1;
	}
    }
    return 0;
}
