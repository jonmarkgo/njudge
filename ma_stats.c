/*
   ** $Log$
   ** Revision 1.7  2003/07/26 12:45:58  millis
   ** Fix Bug 203
   **
   ** Revision 1.6  2003/01/05 00:06:43  millis
   ** Fix bug 90
   **
   ** Revision 1.5  2002/02/25 13:44:10  miller
   ** Fixed country ownership bug
   **
   ** Revision 1.4  2002/02/25 11:51:52  miller
   ** Various updates for Machiavelli bug fixes
   **
   ** Revision 1.2.2.1  2001/10/19 23:08:49  dema
   ** Added XF_NOMONEY handling (for Mach games with no money)
   **
   ** Revision 1.2  2001/07/01 23:19:29  miller
   ** various
   **
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

#include <stdlib.h>
#include <string.h>

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
	if (!(dipent.xflags & XF_NOMONEY)) {
	fputs("\nPower     Treasury\n", rfp);
	fputs("--------  --------\n", rfp);
	for (p = 1; p < WILD_PLAYER; p++) {
		if (dipent.pl[p] == 'x')
			continue;
                if (FindPower(p) >= dipent.n)
                    continue;
                if (dipent.players[FindPower(p)].status & SF_DEAD)
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
			if (dipent.pl[p] == 'x' ||
			    dipent.players[FindPower(p)].status & SF_DEAD )
				continue;
			s = buf;
			for (i = 0; chits[p][i]; i++) {
				if (chits[p][i] == WILD_PLAYER ||
				    dipent.players[FindPower(chits[p][i])].status & SF_DEAD )
					continue;
				sprintf(s, "%s%s", s == buf ? "" : ", ", powers[chits[p][i]]);
				while (*s)
					s++;
			}
			if (pt == MASTER) {
				fprintf(rfp, "  %s:%*s", powers[p], (int) (10 - strlen(powers[p])), "");
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

	int u=1,l=2;
	int nc[NPOWER+2];
	int first_country;
	int owned_country[NPOWER+2];
	int one_owned;
        int x;

	reb = noin = fam = sig = 0;

	fprintf(rfp,"\nCountries Controlled:\n\n");
	for (j = 1; j <= NPOWER + 1; j++) {
		one_owned = 0;
                if (dipent.pl[j] == 'x')
                        continue;
	    for (i = 1; i <= NPOWER+1; i++)
                owned_country[i] = 0;

                i = j == NPOWER + 1 ? 0 : j;

                s = buf;
                strcpy(s, powers[i]);
                while (*s)
                        s++;
                *s++ = ':';
                for (p = strlen(powers[i]) + 2; p < 12; p++)
                        *s++ = ' ';

		x = j == WILD_PLAYER ? j + 1 : j;

                for (p = 0, n = 1; n <= npr; n++) {
		    if ( pletter[dipent.variant][pr[n].home] == 
		         pletter[dipent.variant][x]) {
		           owned_country[power(pr[n].type)] = 1; 
		           one_owned = 1;
		    }
		}
		first_country = 1;
		for (i = 1; i<= NPOWER + 1; i++)
                    if ((owned_country[i] && i != WILD_PLAYER ) ||
			(i == WILD_PLAYER && first_country)) {
			if (!first_country) {
			    *s++ = ','; *s++ = ' ';
			}
			strcpy(s, powers[i]);
			while (*s)
                        s++;
			first_country = 0;
		}
                strcpy(s, ".\n");
                if (one_owned)
		    wrap(rfp, buf, 0, 11);
	}

	fprintf(rfp, "\nCities Controlled:\n\n");

	for (j = 1; j <= NPOWER + 1; j++) {
	    nc[j] = 0;
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
				if (pr[n].cown == pr[n].owner)
				    nc[j]++;  /* Person owns completely this city */
		
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
				}
				if (pr[n].type >= 'A' && pr[n].type <= 'Z') {
				    sprintf(s, "(%c)", pr[n].type);
				    while (*s) s++;
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
			    if ( 0== 0 ) /* Do always */ {
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
				}
				if (pr[n].type >= 'A' && pr[n].type <= 'Z') {
                                    sprintf(s, "(%c)", pr[n].type);
                                    while (*s) s++;
                                }
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

	if (dipent.xflags & XF_NOMONEY) {
	/* Also show building needs for no money games */

	    for (i = 1; i <= NPOWER; i++) {
                if (dipent.pl[i] == 'x')
                        continue;
                if (nc[i] > nu[i]) {
                        for (p = 1; p <= npr; p++) {
                                if (pr[p].owner == i
                                    && (pr[p].type == dipent.pl[i] || pr[p].type == 'x')
                                    && (!(u = pr[p].unit) || unit[u].loc != p)) {
                                        need_order[i]++;
                                        break;
                                }
                        }
                } else if (nc[i] < nu[i]) {
                        if (nc[i] > 0)
                                need_order[i]++;
                }

                p = strlen(powers[i]) + 1;
                fprintf(rfp, "%s:", powers[i]);
                while (p++ < GetMaxCountryStrlen() )
                        putc(' ', rfp);
                l = nc[i] >= nu[i] ? nc[i] - nu[i] : nu[i] - nc[i];
                fprintf(rfp, "%2d Supply center%s %2d Unit%s  %s %2d unit%s.",
                        nc[i], nc[i] == 1 ? ", " : "s,",
                        nu[i], nu[i] == 1 ? ": " : "s:",
                        nc[i] >= nu[i] ? "Builds " : "Removes",
                        l, l == 1 ? "" : "s");
                {
                        for (p = 0; p < dipent.n; p++)
                                if (dipent.players[p].power == i)
                                        break;       
		        if (p < dipent.n && dipent.players[p].status & SF_CD &&
                            !(dipent.flags & F_QUIET)) {
                                fprintf(rfp, "%s  (* CD *)\n", l == 1 ? " " : "");
                                if (!nc[i] && !nu[i]) {
                                        dipent.players[p].status = 0;
                                        dipent.players[p].power =
                                            *dipent.players[p].address == '*' ? -1 : OBSERVER;
                                }
                        } else if (p < dipent.n && dipent.players[p].status & SF_ABAND &&
                                   !(dipent.flags & F_QUIET)) {
                                fprintf(rfp, "%s  (* ABANDONED *)\n", l == 1 ? " " : "");
                                if (!nc[i] && !nu[i]) {
                                        dipent.players[p].status = 0;
                                        dipent.players[p].power =
                                            *dipent.players[p].address == '*' ? -1 : OBSERVER;
                                }
                        } else
                                putc('\n', rfp);
                }
	    }
	}

}
