/*
 * $Log$
 * Revision 1.7  2002/03/25 23:47:55  miller
 * Fixed properly bug for ownership when unit has just moved there
 * (it wasn't having 'loc' set correctly in pr structure)
 *
 * Revision 1.5  2002/02/25 11:51:51  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.3.2.1  2001/10/19 23:37:29  dema
 * Added handling for NoMoney games
 *
 * Revision 1.3  2001/07/08 22:58:24  miller
 * Preliminary check for XF_NOMENY
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Mach2 settings and other fixes
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  porder.c
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "mail.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

static void next_phase(int);
static void newowner(void);

/************************************************************************/

static void next_phase(int power)
{
	int status;

	if (!(dipent.xflags & XF_MACH2))
		newowner();
	else
	    if (dipent.phase[0] == 'F')
		    newowner();

	balance(power, 1, 0);
	if (!(dipent.xflags & XF_MACH2))
		status = victory();
	else
	{
	    status = 0;
	    if (dipent.phase[0] == 'F')
		status = victory();
	}

	if (status == 1) {	/* VICTORY */
		dipent.phase[6] = 'X';
		return;
	}
	if (dipent.phase[0] == 'F') {
		/* Check victory conditions */
		/* Init build phase */
		fam_plag(FAMINE);
		if (!(dipent.xflags & XF_NOMONEY))
		    income(1);
		if (!(dipent.xflags & XF_NOMONEY))
		    dipent.phase[5] = 'B';
		else if (ma_init_build_basic())
		    dipent.phase[5] = 'B';
		else {
		    next_year();
		}
		
	} else {
		if (dipent.phase[0] == 'U') {	/* End of summer */
			dipent.phase[0] = 'F';
		} else {
			fam_plag(PLAGUE);
			fam_plag(STORM);
			dipent.phase[0] = 'U';	/* End of spring */
		}
		init_movement();
	}
}

static void CalculateNewOwners(void)
{
    int u,p;
       if (dipent.phase[5] == 'A') {
	    /* Adjust phase, set city owners to province owners */
	   for (p = 1; p <= npr; p++)
		pr[p].cown = pr[p].owner;
       } 
       else 
         for (u = 1; u <= nunit; u++) {
                if (unit[u].owner <= 0)
                        continue;
                p = unit[u].loc;
                if (water(p))
                        continue;
                if (unit[u].type == 'G') {
                        ncown[p] = unit[u].owner;
                        if (pr[unit[u].loc].unit == 0 || 
			    unit[pr[unit[u].loc].gunit].owner == 0 ||
			    unit[pr[unit[u].loc].gunit].owner == unit[u].owner) {
				/* Province is owned by city owner if no-one in province or
				   provice unit is owned by same power */
                                npown[p] = unit[u].owner;
                        }
                } else {
                        /* MLM change ownership to autonomous for Mach2 if other city owner */
                        if (!(dipent.xflags & XF_NO_MIXED_CONTROL) || 
			     pr[unit[u].loc].gunit == 0 || 
                             unit[pr[unit[u].loc].gunit].owner == 0)
                                npown[p] = unit[u].owner;
                        else if ((dipent.xflags & XF_NO_MIXED_CONTROL) && unit[u].owner != unit[pr[unit[u].loc].gunit].owner)
                                npown[p] = AUTONOMOUS;
			/* If the city has a value and no garrison or a dead-player's garrison, you now own it! */
                        if (cityvalue(p) && (pr[unit[u].loc].gunit == 0 || unit[pr[unit[u].loc].gunit].owner == 0)) {
                                ncown[p] = unit[u].owner;
                        }
                }
        }
}

static void SetNewOwners(void)
{
    int p;
       for (p = 1; p <= npr; p++) {
                if (npown[p])
                        pr[p].owner = npown[p];
                if (ncown[p])
                        pr[p].cown = ncown[p];
        }
}


static void newowner(void)
{

/*
   **  Transfer ownership of provinces and cities.
 */

	int i, j, u, p;
	char *t, buf[1024];

	int p_elim [WILD_PLAYER ];
	int someone_eliminated = 0;

	/* 
	   ** Update the pr[].gunit and unit info
	 */

	   for (u = 1; u <= nunit; u++) {
	       if (unit[u].owner <= 0)
                        continue;
	       if (unit[u].type == 'G') {
	          pr[unit[u].loc].gunit = u;
	       } else {
	          pr[unit[u].loc].unit = u;
	       }
	   }

	/*
	   **  Compute the new owner based on what units are around.
	 */

	CalculateNewOwners();

	/*
	   **  Display changes in province ownership.
	 */

	for (p = 1; p <= npr; p++) {
		if (npown[p] && npown[p] != pr[p].owner)
			break;
	}

	if (p <= npr) {
		fprintf(rfp, "\nOwnership of provinces changing hands:\n\n");
		for (u = 1; u < WILD_PLAYER; u++) {
			if (dipent.pl[u] == 'x')
				continue;
			t = buf;
			for (p = 1; p <= npr; p++) {
				if (pr[p].owner == u && npown[p] != 0 && npown[p] != u) {
					if (t != buf)
						*t++ = ',';
					sprintf(t, " -%s", pr[p].name);
					while (*t)
						t++;
				} else if (pr[p].owner != u && npown[p] == u) {
					if (t != buf)
						*t++ = ',';
					sprintf(t, " +%s", pr[p].name);
					while (*t)
						t++;
				}
			}
			if (t != buf) {
				sprintf(t, ".\n");
				fprintf(rfp, "%s:%*s", powers[u], (int) (9 - strlen(powers[u])), "");
				wrap(rfp, buf, 10, 11);
			}
		}
	}
	/*
	   **  Ditto for the changes in city ownership.
	 */

	for (p = 1; p <= npr; p++) {
		if (ncown[p] && ncown[p] != pr[p].cown)
			break;
	}

	if (p <= npr) {
		fprintf(rfp, "\nOwnership of cities changing hands:\n\n");
		for (u = 1; u < WILD_PLAYER; u++) {
			if (dipent.pl[u] == 'x')
				continue;
			t = buf;
			for (p = 1; p <= npr; p++) {
				if (pr[p].cown == u && ncown[p] != 0 && ncown[p] != u) {
					if (t != buf)
						*t++ = ',';
					sprintf(t, " -%s", pr[p].name);
					while (*t)
						t++;
				} else if (pr[p].cown != u && ncown[p] == u) {
					if (t != buf)
						*t++ = ',';
					sprintf(t, " +%s", pr[p].name);
					while (*t)
						t++;
				}
			}
			if (t != buf) {
				sprintf(t, ".\n");
				fprintf(rfp, "%s:%*s", powers[u], (int) (9 - strlen(powers[u])), "");
				wrap(rfp, buf, 10, 11);
			}
		}
	}
	/*
	   **  Actually transfer the ownership.
	 */
	SetNewOwners();

	/*
	   **  Check for any schmucks who lost all their home cities.
	 */

	for (i = 1; i < WILD_PLAYER; i++) {
		p_elim[i] = 0; /* Init array flags */
		if (dipent.pl[i] == 'x')
			continue;

		u = 0;
		for (p = 1; p <= npr; p++) {
			if (pr[p].home == i && pr[p].cown == i && cityvalue(p))
				break;
			if (pr[p].owner == i || pr[p].cown == i)
				u++;
		}

		if (p > npr) {
			if (u)
			    p_elim[i] = 1;
		}
	}
	/* MachMLM 28/4/01 fix for simultaeneous eliminations */
	for (i = 1; i < WILD_PLAYER; i++) {
	          if (p_elim[i]) {
			someone_eliminated = 1;
			fprintf(rfp, "\n%s has been eliminated due to having no home cities.\n",
					powers[i]);

			for (u = 1; u <= nunit; u++) {
				if (unit[u].owner == i) {
					fprintf(rfp, "%s %s%s in %s eliminated due to attrition.\n",
						owners[unit[u].owner],
						Stype(unit[u].stype), Utype(unit[u].type),
						pr[unit[u].loc].name);
					unit[u].owner = 0;
					remove_siege(unit[u].loc);
				}
			}

			for (p = 1; p <= npr; p++) {
				if (pr[p].owner == i) {
					pr[p].owner = AUTONOMOUS;
					npown[p] = AUTONOMOUS;
				}
				if (pr[p].cown == i) {
					pr[p].cown = AUTONOMOUS;
					ncown[p] = AUTONOMOUS;
				}
				if (pr[p].home == i) {
					pr[p].home = AUTONOMOUS;
				}
			}
			/* Mark dead power as DEAD */
			dipent.players[FindPower(i)].status |= SF_DEAD;

			ducats[i].treasury = 0;
			for (p = 0; p < 6; p++) {
				ducats[i].loan[p] = 0;
				ducats[i].interest[p] = 0;
			}
			chits[i][0] = 0;
		}
	}

	/*
	   **  Check for anyone who has taken over another player's original home.
	 */

	for (i = 1; i < WILD_PLAYER; i++) {
		if (dipent.pl[i] == 'x')
			continue;
		j = 0;
		for (p = 1; p <= npr; p++) {

			if (pr[p].type == dipent.pl[i]) {
				if (!j)
					j = pr[p].owner;
				else if (j != pr[p].owner)
					break;

				if (pr[p].cown != j && cityvalue(p) > 0)
					break;

				if (pr[p].home == j)
					break;
			}
		}

		if (p > npr) {
			fprintf(rfp, "\n%s has gained control of %s's original home territory.\n",
				powers[j], powers[i]);

			for (p = 1; p < npr; p++) {
				if (pr[p].type == dipent.pl[i]) {
					pr[p].home = j;
				}
			}
		}
	}

	/* Redo calculations as an elimination may have changed this */
	if (someone_eliminated) {
	    CalculateNewOwners();
	    SetNewOwners();
	}


	/* See if NoMoney, need to do a standard-style report */

	if (dipent.xflags & XF_NOMONEY) {
	    ma_ownership();
	}

}

/****************************************************************************/
/* function which returns 1 on victory */
int victory(void)
{
	int ncities[NPOWER + 1],	/* number of cities */
	 ncountries[NPOWER + 1],	/* number of countries occupied */
	 pl,			/* player counter */
	 prov,			/* province counter */
	 city;			/* city counter */
	int maxcen,		/* number of cities needed */
	 maxcou,		/* number of countries needed */
	 numwin,		/* number of player matching the criteria */
	 winner = 0;		/* Last player which matched the criteria */

	for (pl = 1; pl <= NPOWER; pl++) {
		if (dipent.pl[pl] == 'x')
			continue;

		/* count the number of cities */
		ncities[pl] = 0;
		for (prov = 1; prov <= npr; prov++) {
			if ((pr[prov].cown == pl) && cityvalue(prov))
				ncities[pl]++;
		}

		/* count the number of variable income cities which have a homecountry
		 */
		ncountries[pl] = 0;
		for (city = 0; city < nv; city++) {
			prov = vincome[city].prov;
			if (pr[prov].home == pl)
				ncountries[pl]++;
		}
	}

	maxcen = dipent.vp;
	/* formula to get the number of countries needed 
	   15 cities -> 2 countries, 23 cities -> 3 countries */
	maxcou = (maxcen + 1) / 8;
	if (dipent.xflags & XF_CITY_ONLY_VICTORY)
		maxcou = 0; /* Mach2, no need to conquer countries, only cities */
	numwin = 0;
	/* Determine the maximum number of cities a power with enough countries
	   has */
	for (pl = 1; pl <= NPOWER; pl++) {
		if (dipent.pl[pl] == 'x')
			continue;
		if ((ncities[pl] > maxcen) && (ncountries[pl] >= maxcou))
			maxcen = ncities[pl];
	}
	/* Determine how many other match this criterium as well */
	for (pl = 1; pl <= NPOWER; pl++) {
		if (dipent.pl[pl] == 'x')
			continue;
		if ((ncities[pl] >= maxcen) && (ncountries[pl] >= maxcou)) {
			numwin++;
			winner = pl;
		}
	}
	if (numwin > 1) {
		dipent.vp = maxcen + 1;
		fprintf(rfp, "\nWinning Centers changed to %d to avoid a draw.\n\n",
			dipent.vp);
		fprintf(bfp, "\nIf you want a draw than vote for it.\n");
		broadcast = 1;
	}
	if (numwin == 1) {
		fprintf(rfp, "(* VICTORY!! *) by %s\n", powers[winner]);
		return 1;
	}
	return 0;
}

/****************************************************************************/

void income(int mindie)
{

	int i, j, n, d, p, u;
	char *s;

	/* 
	 * Arrays must allow the AUTONOMOUS player to get income as well 
	 */
	int winc[AUTONOMOUS + 1], pinc[AUTONOMOUS + 1], cinc[AUTONOMOUS + 1];
	int vinc[AUTONOMOUS + 1];


	if (dipent.xflags & XF_NOMONEY) return; /* No money, no income! */
	die_rolls(DIE_INCOME);

	/*
	 *  Assign the assassination chits if this is the first season.
	 */

	if (dipent.phase[5] == 'A') {
		for (i = 1; i < WILD_PLAYER; i++) {
			if (dipent.pl[i] == 'x')
				continue;
			for (n = 0, j = 1; j < WILD_PLAYER; j++) {
				if (dipent.pl[j] == 'x' || i == j)
					continue;
				chits[i][n++] = j;
			}
			chits[i][n] = 0;
		}
		fprintf(rfp, "Welcome to game %s of Machiavelli Diplomacy!\n", dipent.name);
		newowner();
	}
	fprintf(rfp, "\nIncome phase for Winter of %4.4s.\n", dipent.phase + 1);
	ma_ownership();

	memset(winc, 0, sizeof(winc));
	memset(pinc, 0, sizeof(pinc));
	memset(cinc, 0, sizeof(cinc));
	memset(vinc, 0, sizeof(vinc));

	/* income from fleets on sea */
	for (u = 1; u <= nunit; u++) {
		if (unit[u].owner <= 0)
			continue;
		if (water(unit[u].loc)) {
			winc[unit[u].owner]++;
		}
	}

	/* income from provinces and cities */
	for (p = 1; p < npr; p++) {
		if (!water(p)) {
			/*
			   ** My explanation of the rules is:
			   ** In case of famine the income is lost.
			   ** If the province is in rebellion the income is lost,
			   ** if the city is only in rebellion the province income is not lost.
			 */
			if (!is_infected(p) && !has_prebellion(p)) {
/*              fprintf(rfp,"Province income of %s -> %s\n",  */
/*                              pr[p].name, powers[pr[p].owner]); */
				pinc[pr[p].owner]++;
			}
			/*
			   ** My explanation of the rules is:
			   ** In case of famine iwithout a garrison, the income is lost.
			   ** If a city is fortified, its income is lost if the city is in rebellion
			   ** if it is not fortified, the income depends on the province rebellion.
			 */
			if (!is_infected(p) || has_garrison(p))
				if ((has_fortcity(p) && !has_crebellion(p)) ||
				    (!has_fortcity(p) && !has_prebellion(p))) {
/*              fprintf(rfp,"City income of %s (%d) -> %s\n", */
/*                              pr[p].name, cityvalue(p), powers[pr[p].cown]); */
			   /* Only unsieged cities give income */
				    if (!is_sieged(p))
					cinc[pr[p].cown] += cityvalue(p);
				}
		}
	}

	/* income from variable income rolls */
	fprintf(rfp, "\n");
	fprintf(rfp, "Variable     Power       Die Roll  Ducats\n");
	fprintf(rfp, "----------   ---------   --------  ------\n");

	for (i = 0; i < nv; i++) {
		n = 0;
		p = vincome[i].prov;
		u = pr[p].home == 0 ? pr[p].cown : pr[p].home;
		s = line;
		for (j = 0; j < vincome[i].dice; j++) {
			if (NO_DICE) {
				d = dipent.phase[4] % 2 ? 4 : 3;
			} else {
				d = die(1, 6);
				if (j == 0 && d < mindie)
					d = mindie;
			}
			sprintf(s, "%s%d", j ? "," : "", d);
			while (*s)
				s++;
			n += vincome[i].vinc[d - 1];
		}
		/* Adjust income if fixed */
		if (GetInitialMoney(p, &n))
		    strcpy(line," Fixed ");
                 
		fprintf(rfp, "%-13.13s%-12.12s%-10.10s%4d\n",
			pr[p].name, powers[u], line, n);
		vinc[u] += n;
	}

	fputc('\n', rfp);

	fprintf(rfp,
		"Power       Treasury  Income  Water  Provinces  Cities  Variable\n");
	fprintf(rfp,
		"--------    --------  ------  -----  ---------  ------  --------\n");

	for (i = 1; i < WILD_PLAYER; i++) {
		if (dipent.pl[i] == 'x')
			continue;
		n = winc[i] + pinc[i] + cinc[i] + vinc[i];
		ducats[i].treasury += n;
		fprintf(rfp, "%-12.12s%5d%9d%7d%10d%9d%10d\n", powers[i],
			ducats[i].treasury, n, winc[i], pinc[i], cinc[i], vinc[i]);
	}

	fputc('\n', rfp);

}

void ma_process_input(int pt, char phase)
{
	char *s;
	int u, p;
	int status;

	for (u = 1; u <= nunit; u++)
		unit[u].order = 'n';
	memset(payments, 0, sizeof(payments));
	memset(borrow, 0, sizeof(borrow));
	memset(expense, 0, sizeof(expense));

	ma_init_build_basic();

	do {
		if (preprocess(&s, &p))
			continue;
		if (!eol) {
			while (*s == ' ')
				s++;

			switch (phase) {
			case 'B':
				if (dipent.xflags & XF_NOMONEY) 
				    status = ma_buildin_basic(&s, p);
				else
				    status = ma_buildin(&s, p);
				break;
			case 'M':	/* 
					   ** Return with positive value if this line does not
					   ** contain an expenditure
					 */
				status = expin(&s, p);
				if (status > 0)
					status = ma_movein(&s, p);
				break;
			case 'R':
				status = ma_retreatin(&s, p);
				break;
			default:
				status = 0;
			}
			eol = !*s || (*s == '\n');

			if (status || !eol)
				junk(&s);
		}
	} while (!eof);
}

int ma_process_output(int pt, char phase)
{
	int retreats;

	switch (phase) {
	case 'A':		/* Initial Machiavelli phase */
		income(2);
		dipent.phase[5] = 'B';

	case 'B':		/* Adjustments */
		if (dipent.xflags & XF_NOMONEY)
		    ma_buildout_basic(pt);
		else
		    ma_buildout(pt);
		if (processing) {
			balance(pt, 1, 0);
			next_year();
		}
		break;

	case 'M':		/* Movement */
		expout(pt);
		retreats = ma_moveout(pt);
		if (processing) {	/* Advance to retreats or next phase */
			if (retreats)
				init_retreats();
			else
				next_phase(pt);
		}
		break;

	case 'R':		/* Retreats */
		ma_retreatout(pt);
		if (processing)
			next_phase(pt);
		break;

	default:
		fprintf(rfp, "Unknown phase type %s.\n", dipent.phase);
		return E_FATAL;
	}
	return 1;		/* reached ? */
}
