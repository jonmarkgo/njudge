
/*
   ** $Log$
   ** Revision 1.4.2.1  2001/10/19 23:13:17  dema
   ** Added missing include
   **
   ** Revision 1.4  2001/07/10 21:43:33  miller
   ** Fix for blind to not show summary if not master
   **
   ** Revision 1.3  2001/02/23 00:19:33  miller
   ** un-DOSify
   **
   ** Revision 1.2  2001/02/03 10:41:04  miller
   ** fix blind bug
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  po_status.c
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

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

/****************************************************************************/

void status(int pt)
{

/*
 *  Print out a status of the game.  Where the units are and to where
 *  they can retreat and who is in control of what.
 */

	char *t, *s, line[1024];
	unsigned char *b;
	int i, l, p, u;

	int not_ok_as_blind = 0;

	if ((dipent.flags & F_BLIND) && dipent.seq[6] != 'X' ) {
		if (pt != MASTER)
			not_ok_as_blind = 1;
	}

	fprintf(rfp, "Status of the %s phase for %s of %d.  (%s.%s)\n",
		dipent.phase[5] == 'M' ? "Movement" :
		dipent.phase[5] == 'R' ? "Retreat" : "Adjustment",
		dipent.phase[5] == 'B' ? "Winter" :
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);


	for (l = u = 1; u <= nunit; u++)
		l = (p = strlen(powers[unit[u].owner])) > l ? p : l;

	p = 0;
	for (u = 1; u <= nunit; u++) {
                if (pt == unit[u].owner || !not_ok_as_blind)
                {
                    /* ok to show in a blind game */

		if (p != unit[u].owner)
			putc('\n', rfp);
		p = unit[u].owner;

		t = line;
		sprintf(t, "%s:%*s", powers[p], l, "");
		t += l + 2;

		sprintf(t, "%s%s %s",
			Stype(unit[u].stype), Utype(unit[u].type), pr[unit[u].loc].name);
		while (*t)
			t++;

		if (unit[u].coast > XC) {
			sprintf(t, " (%s)", mtype[unit[u].coast]);
			while (*t)
				t++;
		}
		i = 0;
		for (b = unit[u].convoy; b != NULL && *b; b++) {
			if (!i++)
				sprintf(t, " can retreat to ");
			else
				sprintf(t, " or ");
			while (*t)
				t++;

			sprintf(t, "%s", pr[*b].name);
			while (*t)
				t++;

			if (unit[u].rcoast[i - 1] > XC) {
				sprintf(t, " (%s)", mtype[unit[u].rcoast[i - 1]]);
				while (*t)
					t++;
			}
		}

		*t++ = '.';
		*t++ = '\0';

		for (t = s = line, i = 0; *s; s++, i++)
			if (i > 79) {
				while (*--s != ' ');
				*s++ = '\0';
				fprintf(rfp, "%s\n         ", t);
				t = s;
				i = 10;
			}
		fprintf(rfp, "%s\n", t);
	  }
	}

	putc('\n', rfp);

        if (!not_ok_as_blind) {
          if (dipent.flags & F_MACH) {
                balance(pt, 0, 1);
                ma_ownership();
          } else
                ownership();
        }

	putc('\n', rfp);
	putc('\n', rfp);

}

/****************************************************************************/
