/*
   ** $Log$
   ** Revision 1.4  2001/10/20 12:11:15  miller
   ** Merged in changes from DEMA and USTV CVS: ----------------------------------------------------------------------
   **
   ** Revision 1.3.2.1  2001/10/15 00:28:29  ustv
   ** Added check for HongKong flag
   **
   ** Revision 1.3  2001/04/15 21:21:22  miller
   ** Add use of MaxCountryStrlen()
   **
   ** Revision 1.2  2000/11/14 14:27:37  miller
   ** Small changes for blockade calculations
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.2  1996/11/07 08:01:09  rpaar
   ** Changed fprintf(bfp, "If you want a draw,..." to fprintf(rfp,...
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  porder.c
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

#include <string.h>
#include "dip.h"
#include "functions.h"
#include "mail.h"
#include "porder.h"

static void newowner(void);
static void next_phase(void);
static char HongKongCheck(int, int);

#define IsCentre(x)  ( pr[x].type == 'x' || (pr[x].type >= 'A' && pr[x].type <= 'Z') || (pr[x].type >= '0' && pr[x].type >= '9'))

/***************************************************************************/
/*  Print out build statistics.  
   Return zero if no builds are needed
   Return positive if builds are needed
   Return negative if victory occurred            
 */
int ownership(void)
{
	int nu[NPOWER + 1], np[NPOWER + 1], i, j, n, l, p, u, maxcen,
	 tmpi, numwin;
	int statusval;
	char *s, buf[1024];

	fprintf(rfp, "\nOwnership of supply centers:\n\n");

	for (j = 1; j <= NPOWER + 1; j++) {
		if (dipent.pl[j] == 'x')
			continue;
		i = j == NPOWER + 1 ? 0 : j;

		np[i] = nu[i] = 0;
		for (n = 1; n <= nunit; n++)
			if (unit[n].owner == i)
				nu[i]++;

		s = buf;
		strcpy(s, powers[i]);
		while (*s)
			s++;
		*s++ = ':';
		for (p = strlen(powers[i]) + 2; p < GetMaxCountryStrlen() +2; p++)
			*s++ = ' ';
		for (p = 0, n = 1; n <= npr; n++) {
			if ((pr[n].type == 'x' || !islower(pr[n].type)) &&
			    (pr[n].owner == i || (i == 0 && pr[n].owner == AUTONOMOUS))) {
				np[i]++;
				if (p++) {
					*s++ = ',';
					*s++ = ' ';
				}
				if (pr[n].blockaded) {
					sprintf(s, "(%s)", pr[n].name);
					np[i]--; /* Blockaded provinces don't count */
				} else {
					strcpy(s, pr[n].name);
				}
				while (*s)
					s++;
			}
		}

		if (!p)
			continue;

		strcpy(s, ".\n");
		wrap(rfp, buf, 0, GetMaxCountryStrlen()+1);
	}

	statusval = 0;
	putc('\n', rfp);

	maxcen = dipent.vp;
	numwin = 0;
	for (tmpi = 1; tmpi <= NPOWER; tmpi++) {
		if (dipent.pl[tmpi] == 'x')
			continue;
		if (np[tmpi] > maxcen)
			maxcen = np[tmpi];
	}
	for (tmpi = 1; tmpi <= NPOWER; tmpi++) {
		if (dipent.pl[tmpi] == 'x')
			continue;
		if (np[tmpi] >= maxcen)
			numwin++;
	}
	if (numwin > 1) {
		dipent.vp = maxcen + 1;
		fprintf(rfp, "\nWinning Centers changed to %d to avoid a draw.\n\n", dipent.vp);
		fprintf(rfp, " If you want a draw, then vote for it.\n");
		broadcast = 1;
	}
	for (i = 1; i <= NPOWER; i++) {
		if (dipent.pl[i] == 'x')
			continue;
		if (np[i] > nu[i]) {
			for (p = 1; p <= npr; p++) {
				if (pr[p].owner == i
				    && (pr[p].type == dipent.pl[i] || IsCentre(p))
				    && (!(u = pr[p].unit) || unit[u].loc != p)) {
					need_order[i]++;
					if (statusval >= 0)
						statusval++;
					break;
				}
			}
		} else if (np[i] < nu[i]) {
			if (np[i] > 0)
				need_order[i]++;
			if (statusval >= 0)
				statusval++;
		}
		p = strlen(powers[i]) + 1;
		fprintf(rfp, "%s:", powers[i]);
		while (p++ < GetMaxCountryStrlen() )
			putc(' ', rfp);
		l = np[i] >= nu[i] ? np[i] - nu[i] : nu[i] - np[i];
		fprintf(rfp, "%2d Supply center%s %2d Unit%s  %s %2d unit%s.",
			np[i], np[i] == 1 ? ", " : "s,",
			nu[i], nu[i] == 1 ? ": " : "s:",
			np[i] >= nu[i] ? "Builds " : "Removes",
			l, l == 1 ? "" : "s");
		if (np[i] >= maxcen) {
			/* Can't handle draws just yet - see above */
			if ((numwin == 1) /* || (dipent.flags & SF_DRAW) */ ) {
				fprintf(rfp, "%s  (* VICTORY!! *)\n", l == 1 ? " " : "");
				statusval = -1;
			} else
				putc('\n', rfp);
		} else {
			for (p = 0; p < dipent.n; p++)
				if (dipent.players[p].power == i)
					break;
			if (p < dipent.n && dipent.players[p].status & SF_CD &&
			    !(dipent.flags & F_QUIET)) {
				fprintf(rfp, "%s  (* CD *)\n", l == 1 ? " " : "");
				if (!np[i] && !nu[i]) {
					dipent.players[p].status = 0;
					dipent.players[p].power =
					    *dipent.players[p].address == '*' ? -1 : OBSERVER;
				}
			} else if (p < dipent.n && dipent.players[p].status & SF_ABAND &&
				   !(dipent.flags & F_QUIET)) {
				fprintf(rfp, "%s  (* ABANDONED *)\n", l == 1 ? " " : "");
				if (!np[i] && !nu[i]) {
					dipent.players[p].status = 0;
					dipent.players[p].power =
					    *dipent.players[p].address == '*' ? -1 : OBSERVER;
				}
			} else
				putc('\n', rfp);
		}
	}

	return statusval;
}

static void next_phase(void)
{
	int status;

	if (dipent.phase[0] == 'F') {

		/* Init build phase or advance to the next spring */
		newowner();
		status = ownership();
		if (status < 0)	/* VICTORY */
			dipent.phase[6] = 'X';
		else {
			if (status)
				dipent.phase[5] = 'B';
			else
				next_year();
		}
	} else {
		dipent.phase[0] = 'F';
		init_movement();
	}
}

static void newowner(void)
{
	int n;
	int p;

	for (n = 1; n <= nunit; n++) {
		if (unit[n].owner && pr[p = unit[n].loc].type != 'l' && !water(p) && unit[n].type != 'W') {
			pr[p].owner = HongKongCheck(unit[n].owner,p);
		}
	}
}

void process_input(int pt, char phase)
{
	char *s;
	int u, p;
	int status;

	for (u = 1; u <= nunit; u++)
		unit[u].order = 'n';

	init_build();


	do {
		if (preprocess(&s, &p))
			continue;
		while (!eol) {
			while (*s == ' ')
				s++;

			switch (phase) {
			case 'B':
				status = buildin(&s, p);
				break;
			case 'M':
				status = movein(&s, p);
				break;
			case 'R':
				status = retreatin(&s, p);
				break;
			default:
				status = 0;
			}
			if (status)
				junk(&s);

			eol = !*s || *s++ == '\n';
		}
	} while (!eof);
}

int process_output(int pt, char phase)
{
	int retreats;

	switch (phase) {
	case 'B':		/* Adjustments */
		buildout(pt);
		if (processing) {
			next_year();
		}
		break;

	case 'M':		/* Movement */
		retreats = moveout(pt);
		if (processing) {	/* Advance to retreats or next phase */
			if (retreats)
				init_retreats();
			else
				next_phase();
		}
		break;

	case 'R':		/* Retreats */
		retreatout(pt);
		if (processing)
			next_phase();
		break;

	default:
		fprintf(rfp, "Unknown phase type %s.\n", dipent.phase);
		return E_FATAL;
	}
	return 1;		/* not reached */
}

char 
HongKongCheck(int power, int prov_index)
{
    int i,j;
    char power_letter = dipent.pl[power];

    if (!(dipent.x2flags & X2F_HONGKONG))
	return power;  /* No HonkKong flag set, so return power */

    for (i = 0; i < nhk;i++) {
	if (hk[i].power_letter == power_letter) {
 	    for (j = 0; j < hk[i].np; j++) {
		if (hk[i].pr[j] == prov_index)
		    return AUTONOMOUS; /* Pretend unowned */
	    }
	}
    }
    return power;
}
