/*
   ** $Log$
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */


/*------------------------------------------------------------------*/
/* This is like fprintf(), but it sends to the file pointer "mbfp"  */
/* as well as the normal file pointer. A bit of a kludgy fix to     */
/* allow moderators to see the real scoop on grey/faked press.      */
/*                                                                  */
/* Written by Jonathan S. Haas, positron@eecs.umich.edu, 10 Mar 93  */
/*------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include "mail.h"
#define MAXARGS 100

void mfprintf(FILE * the_file, char *fmt,...)
{
	va_list args;
	char string[200];

	va_start(args, fmt);
	vsprintf(string, fmt, args);
	va_end(args);

	fputs(string, the_file);
	fputs(string, mbfp);
}

