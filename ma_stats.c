/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

#include <stdlib.h>

#include "functions.h"
#include "dip.h"
#include "mail.h"
#include "porder.h"
#include "mach.h"

/****************************************************************************/

void balance(int pt, int next, int listflg)
{

/*
 *  Print out treasury and loan statistics for each power.
 */

	int i, p, s, y;
	static char *seasons[3] =
	{"Spring", "Summer", "Fall"};


	y = atoi(dipent.phase + 1);
	s = dipent.phase[0] == 'S' ? 0 : dipent.phase[0] == 'U' ? 1 : 2;
	if (dipent.phase[5] != 'M' || next) {
		if (s++ == 2) {
			s = 0;
			y++;
		}
	}
	fputs("\nPower     Treasury\n", rfp);
	fputs("--------  --------\n", rfp);
	for (p = 1; p < WILD_PLAYER; p++) {
		if (dipent.pl[p] == 'x')
			continue;
		if (p == pt || pt == MASTER || processing || listflg) {
			fprintf(rfp, "%-10.10s%5dd", powers[p], ducats[p].treasury);
			if (ducats[p].interest[0] == -1) {
				fprintf(rfp, ", Prior loan default");
			} else {
				for (i = 0; i < 6; i++) {
					if (ducats[p].loan[i] || ducats[p].interest[i]) {
						fprintf(rfp, ", %d+%dd due %s %d",
							ducats[p].loan[i], ducats[p].interest[i],
							seasons[(i + s) % 3], y + (i + s) / 3);
					}
				}
			}
			fputc('\n', rfp);
		}
	}


	if (!NO_ASSASSINATION && pt != OBSERVER && !processing) {
		char *s, buf[1024];
		fputs("\nAssassination chits owned:\n", rfp);
		if (pt == MASTER) {
			i = 1;
			y = WILD_PLAYER;
		} else {
			i = pt;
			y = pt + 1;
		}
		for (p = i; p < y; p++) {
			if (dipent.pl[p] == 'x')
				continue;
			s = buf;
			for (i = 0; chits[p][i]; i++) {
				if (chits[p][i] == WILD_PLAYER)
					continue;
				sprintf(s, "%s%s", s == buf ? "" : ", ", powers[chits[p][i]]);
				while (*s)
					s++;
			}
			if (pt == MASTER) {
				fprintf(rfp, "  %s:%*s", powers[p], 10 - strlen(powers[p]), "");
				i = 13;
			} else {
				fputs("  ", rfp);
				i = 2;
			}
			if (s == buf)
				strcpy(s, "None.\n");
			else
				strcpy(s, ".\n");
			wrap(rfp, buf, i, 2);
		}
	}
}

/***************************************************************************
* Show city and province ownership with the famine, 
* rebellion and siege attributes.
*/
void ma_ownership(void)
{
	int nu[NPOWER + 1], np[NPOWER + 1], i, j, n, p, reb, noin, fam,
	 sig;
	char *s, buf[1024];

	reb = noin = fam = sig = 0;

	fprintf(rfp, "\nCities Controlled:\n\n");

	for (j = 1; j <= NPOWER + 1; j++) {
		if (dipent.pl[j] == 'x')
			continue;
		i = j == NPOWER + 1 ? 0 : j;

		s = buf;
		strcpy(s, powers[i]);
		while (*s)
			s++;
		*s++ = ':';
		for (p = strlen(powers[i]) + 2; p < 12; p++)
			*s++ = ' ';
		for (p = 0, n = 1; n <= npr; n++) {
			if ((pr[n].cown == i || (i == 0 && pr[n].cown == AUTONOMOUS))
			    && cityvalue(n)) {
				if (p++) {
					*s++ = ',';
					*s++ = ' ';
				}
				strcpy(s, pr[n].name);
				while (*s)
					s++;
				if (has_crebellion(n)) {
					*s++ = '+';
					reb++;
				}
				if (!has_fortcity(n) && has_prebellion(n)) {
					*s++ = '&';
					noin++;
				}
				if (is_infected(n) && !pr[n].gunit) {
					*s++ = '%';
					fam++;
				}
				if (is_sieged(n)) {
					*s++ = '$';
					sig++;
				}
				if (pr[n].home == i && i != 0) {
					*s++ = '*';
					need_order[i]++;
				} else if (pr[n].home != AUTONOMOUS && pr[n].home != 0) {
					sprintf(s, " (%s)", powers[pr[n].home]);
					while (*s)
						s++;
				}
			}
		}

		if (!p)
			continue;

		strcpy(s, ".\n");
		wrap(rfp, buf, 0, 11);

	}

	fprintf(rfp, "\nProvinces Controlled:\n\n");

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
		for (p = strlen(powers[i]) + 2; p < 12; p++)
			*s++ = ' ';
		for (p = 0, n = 1; n <= npr; n++) {
			if ((pr[n].type == 'x' || !islower(pr[n].type)) &&
			    (pr[n].owner == i || (i == 0 && pr[n].owner == AUTONOMOUS))) {
				np[i]++;
				if (p++) {
					*s++ = ',';
					*s++ = ' ';
				}
				strcpy(s, pr[n].name);
				while (*s)
					s++;
				if (has_prebellion(n)) {
					*s++ = '+';
					reb++;
				}
				if (is_infected(n)) {
					*s++ = '%';
					fam++;
				}
				if (pr[n].home == i && i != 0) {
					*s++ = '*';
				} else if (pr[n].home != AUTONOMOUS && pr[n].home != 0) {
					sprintf(s, " (%s)", powers[pr[n].home]);
					while (*s)
						s++;
				}
			}
		}

		if (!p)
			continue;

		strcpy(s, ".\n");
		wrap(rfp, buf, 0, 11);
	}

	fputs("\n* = Home city/province", rfp);
	if (reb)
		fputs(", + = Rebellion", rfp);
	if (fam)
		fputs(", % = Famine", rfp);
	if (sig)
		fputs(", $ = Besieged", rfp);
	if (noin)
		fputs(", & = No income (Province in rebellion)", rfp);
	fputs(".\n\n", rfp);
}
