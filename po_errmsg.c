/*
   ** $Log: po_errmsg.c,v $
   ** Revision 1.3  2001-10-20 12:11:14  miller
   ** Merged in changes from DEMA and USTV CVS: ----------------------------------------------------------------------
   **
   ** Revision 1.2.2.1  2001/10/19 23:11:39  dema
   ** Added missing include
   **
   ** Revision 1.2  2000/11/14 14:27:37  miller
   ** small change to allow syntax checking to work with preprocessing commands
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  po_errmsg.c
   **
   **  Copyright 1987, Lowe.
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the
   **  above notices are preserved and that due credit is given to Mr.
   **  Lowe.
   **
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

extern int vflg;

/****************************************************************************/

void errmsg(char *fmt,...)
{
/*
 *  Output an error message and increment the error counter, if appropriate.
 */
	va_list args;

	if (options.verbose || (marker && !processing)) {
		if (!pline++)
			fputs(line, rfp);
		va_start(args, fmt);
		vfprintf(rfp, fmt, args);
		va_end(args);
		if (!options.verbose) // TODO: wtf ?!? - cjw
			err++;
	}
}


/****************************************************************************/

void errputc(char c, FILE * fp)
{

/*
 *  Output a character if we're processing errors.
 */

	if (marker && !processing)
		putc(c, fp);

}

/****************************************************************************/

static int alev = 0, ilev = 0, elev = 0;
#define IF      1
#define ELSEIF  2
#define ELSE    3
#define ENDIF   4

        static char *directives[] =
        {"0", "if", "else if", "else", "endif"};

int canpreprocess(char *s)
{
	int dir;
        s = lookfor(s, directives, nentry(directives), &dir);
	return dir; 
}

int preprocess(char **s, int *p)
{

/*
 *  Do various common processing for every line read from input.
 *  The return value if nonzero if the line should be skipped.
 */

	int dir;

	/*
	 *  Check for the been here before marker.  Errors only get reported
	 *  The first time through.
	 */

	eof = eol = 0;

	if (!fgets(line, sizeof(line), ifp)) {
		eof++;
		marker = 0;
		return 1;
	}
	if (!strcmp(line, "X-marker\n")) {
		marker++;
		alev = ilev = elev = 0;
		return 1;
	}
	/*  Ensure that we don't have any unterminated IF directives.  */

	if (!strcmp(line, "Y-marker\n")) {
		alev = ilev = elev = 0;
		return 1;
	}
	/*  If there's an error we want to print out the line, but only once.  */

	pline = 0;
	*s = line;

	/*  Get the source power specification  */

	if (!(*p = power(**s))) {
		errmsg("Invalid source power %c specified.\n", **s);
		return 1;
	}
	while (*(*s)++ != ':')
		if (!**s || **s == ' ' || **s == ';') {
			errmsg("Missing colon on source power.\n");
			return 1;
		}
	/*  Look for conditional processing  */

	*s = lookfor(*s, directives, nentry(directives), &dir);
	if (dir) {
		switch (dir) {
		case IF:
			if (alev == ilev) {
				elev++;
				if (condition(s))
					ilev++;
			}
			alev++;
			break;

		case ELSEIF:
			if (alev == ilev) {
				ilev--;
				elev--;
			}
			if (alev == elev)
				if (condition(s))
					ilev++;
			break;

		case ELSE:
			if (alev == ilev) {
				ilev--;
				elev--;
			}
			if (alev == elev)
				ilev++;
			break;

		case ENDIF:
			alev--;
			if (alev < ilev)
				ilev--;
			if (alev < elev)
				elev--;
			if (alev <= 0)
				alev = ilev = elev = 0;
		}
		return 1;
	}
	return (alev != ilev);

}

/****************************************************************************/
/*
 * Dump the rest of this line
 */
void junk(char **stream)
{
	char *s;

	s = *stream;
	if (*s && *s != ';' && *s != ',' && *s != '\n') {
		errmsg("Discarding junk: [");
		while (*s != ';' && *s != '\n')
			errputc(*s++, rfp);
		errputc(']', rfp);
		errputc('\n', rfp);
	}
	*stream = s;
}
