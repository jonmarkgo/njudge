  /*
  ** $Log$
  ** Revision 1.26  2004/06/15 23:48:37  millis
  ** Bug 297: fix a problem with detecting intimate victory
  **
  ** Revision 1.25  2004/06/08 08:02:03  millis
  ** Bug 297: Fix so that only real, alive players can win
  **
  ** Revision 1.24  2004/05/22 10:28:45  millis
  ** Restored 1.22 version changes mistakenly deleted
  **
  ** Revision 1.23  2004/05/22 08:52:22  millis
  ** Bug 297: Add Intimate Diplomacy
  **
  ** Revision 1.22  2004/04/03 16:29:17  millis
  ** Fixed ExtraCentres problem (wasn't working!)
  **
  ** Revision 1.21  2004/01/04 11:34:36  millis
  ** Implement Bug #262 (ExtraCentres for 1900 Steamroller)
  **
  ** Revision 1.20  2004/01/02 08:43:36  nzmb
  ** Fix bug #219 -- advance to the build phase when a game ends in a solo.
  **
  ** Revision 1.19  2003/05/16 22:45:42  millis
  ** Fix bug 156 (ownership() was having winning condition incorrectly reset)
  **
  ** Revision 1.18  2003/05/03 16:25:46  millis
  ** Checking wrong flag for Summer setting!
  **
  ** Revision 1.17  2003/05/02 21:52:08  millis
  ** Better fix for the HOMETRANSFER flag
  **
  ** Revision 1.16  2003/05/02 18:19:21  millis
  ** More for Bug 137, Optional summer turns
  **
  ** Revision 1.15  2003/04/26 22:50:35  millis
  ** Fixed HongKongCheck() to return int not char
  **
  ** Revision 1.14  2003/02/18 14:04:44  millis
  ** Display home centres if X2F_HOMETRANSFER is set
  **
  ** Revision 1.13  2003/01/18 23:48:21  millis
  ** Don't call temporary USTV functions
  **
  ** Revision 1.12  2003/01/17 23:31:56  millis
  ** Merged from USTV
  ** Summary will now show only ALIVE powers
  **
  ** Revision 1.11  2002/12/28 00:02:54  millis
  ** Fixed bug 77, adding wrap_char() function
  **
  ** Revision 1.10  2002/12/16 23:19:04  nzmb
  ** Fixed bug that was causing the victor in games to be reported incorrectly. Also fixed a typo in ma_porder.c.
  **
  ** Revision 1.9  2002/08/27 23:56:11  millis
  ** Added better victory display (fixing Mach victory bug)
  **
  ** Revision 1.8  2002/05/11 09:15:35  greg
  ** Minor bug fixes
  ** - fixed subjectline for absence requests
  ** - fixed phase length, so it's no longer hard coded for responses
  ** - partial fix for unusable builds, players with only unusable builds
  **    will no longer be flagged as having orders due, however players
  **    with some usable builds will need to waive any unusable builds,
  **    also, if one or more players have unusable builds, but no
  **    player has usable builds, the build phase will process after
  **    a short delay
  **
  ** Revision 1.7  2002/04/15 12:55:46  miller
  ** Multiple changes for blind & Colonial & setup from USTV
  **
  ** Revision 1.6  2001/11/18 17:40:18  miller
  ** Small typo fix
  **
  ** Revision 1.5  2001/11/18 17:28:28  miller
  ** Corrected build checking for chaos-like games
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
static int HongKongCheck(int, int);
 #define IsCentre(x)  ( pr[x].type == 'x' || (pr[x].type >= 'A' && pr[x].type <= 'Z') || (pr[x].type >= '0' && pr[x].type <= '9'))
/***************************************************************************/
/*  Print out build statistics.  
   Return zero if no builds are needed
   Return positive if builds are needed
   Return negative if victory occurred            
 */
int ownership(int new_flag)
{
	int nu[NPOWER + 1], np[NPOWER + 1], i, ii, j, n, l, p, u, maxcen,
	 tmpi, numwin;
	int statusval;
	int p_count;
	static int p_list[NPOWER+1];
	char *s, buf[1024];

	fprintf(rfp, "\nOwnership of supply centers:\n\n");

	for (j = 1; j <= NPOWER + 1; j++) {
		if (dipent.pl[j] == 'x')
			continue;
		i = j == NPOWER + 1 ? 0 : j;

		np[i] = nu[i] = 0;
		for (n = 1; n <= nunit; n++)
			if (unit[n].owner == i && unit[n].type != ' ') {
				nu[i]++;
			        if (unit[n].type == 'T')
				    nu[i]++;  /* AF is two units */
			}

		s = buf;
		strcpy(s, powers[i]);
		while (*s)
			s++;
		*s++ = ':';
		for (p = strlen(powers[i]) + 2; p < GetMaxCountryStrlen() +2; p++)
			*s++ = ' ';
		for (p = 0, n = 1; n <= npr; n++) {
			if ((pr[n].type == 'x' || !islower((int) pr[n].type)) &&
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
				if (dipent.x2flags & X2F_MORE_HOMES || 
				    dipent.x2flags & X2F_HOMETRANSFER) {
				    /* As homes can be variable, show them as per Mach */
                                    if (pr[n].home == i && i != 0) {
                                        *s++ = '*';
                                    }
                                    if ((pr[n].type >= 'A' && pr[n].type <= 'Z') || 
					(pr[n].type >= '0' && pr[n].type <= '9') ||
					(pr[n].type == 'x' && pr[n].home)) {
                                        sprintf(s, "(%c)", pletter[dipent.variant][pr[n].home]);
                                       while (*s) s++;
                                    }
				}
			}
			if (pr[n].type == 'x' && !pr[n].owner && isNativePower(i) )
				np[i]++;

		}
		np[i] += ExtraCentres(j);

		if (!p)
			continue;

		strcpy(s, ".\n");
		/* Bug 77, wrap on the ',' character only */
               /* will mess up if line ends on a ',' or place has a ',' in it */
               wrap_char(rfp, buf, 0, GetMaxCountryStrlen()+1, ',');
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
		{
			numwin++;
			victor = tmpi;
		}
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

		if (dipent.xflags & XF_ALTBUILD) {
			need_order[i]++;
			if (statusval >= 0)
				statusval++;
		} else {
			if (np[i] > nu[i]) {
				if (dipent.xflags & XF_BUILD_ANYCENTRES) {
					need_order[i]++;
					if (statusval >= 0)
						statusval++;
				} else {
					if (dipent.xflags & XF_BUILD_ONECENTRE) {
						for (p = 1; p <= npr; p++) {
							if (pr[p].owner == i &&
							  pr[p].type == dipent.pl[i]) {
								need_order[i]++;
								if (statusval >= 0)
									statusval++;
								break;
							}
						}
					} else {
						for (p = 1; p <= npr; p++) {
							if (pr[p].owner == i &&
							  pr[p].type == dipent.pl[i] &&
							  (!(u = pr[p].unit) || unit[u].loc != p)) {
								need_order[i]++;
								if (statusval >= 0)
									statusval++;
								break;
							}
						}
					}
				}
			} else {
				if (np[i] < nu[i]) {
					if (np[i] > 0)
						need_order[i]++;
					if (statusval >= 0)
						statusval++;
				}
			}
		}

		p = strlen(powers[i]) + 1;
	if (np[i] != 0 || nu[i] != 0) {
		fprintf(rfp, "%s:", powers[i]);
		while (p++ < GetMaxCountryStrlen() )
			putc(' ', rfp);
		l = np[i] >= nu[i] ? np[i] - nu[i] : nu[i] - np[i];
		fprintf(rfp, "%2d Supply center%s %2d Unit%s  %s %2d unit%s.",
			np[i], np[i] == 1 ? ", " : "s,",
			nu[i], nu[i] == 1 ? ": " : "s:",
			np[i] >= nu[i] ? "Builds " : "Removes",
			l, l == 1 ? "" : "s");
		if (np[i] >= maxcen && !isNativePower(i)) {
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
	}

	/* Build transform and anydisband games always have build phase */
	if (dipent.xflags & XF_ALTBUILD)
	    if (statusval == 0)
		statusval = 1;

	/* games with chooseable home centres have builds to make if a power needs centres */
	if ((dipent.x2flags & X2F_MORE_HOMES) && statusval == 0) {
		statusval = 1;  
	/* Temporary: should do some work by seeing if any homes still left
           to declare for players alive */
	/* Raised in Bug 104 */
	}

	/* OK, let's print out the controllers of each power */

	if (IS_DUPLEX(dipent) && !(dipent.xflags & X2F_SECRET)) {
	    /* Some kind of mis-match game! */
	   putc('\n', rfp);
	   PrintTwoColTable("Power Control", "Power", "Controlled By");

	   if (!(dipent.flags & F_INTIMATE)) {
		/* OK, normal duplex game: work out the player numers */

		for (i = 0; i <= NPOWER; i++)
		    p_list[i] = 0;
				 
		/* First loop, go around the players that are playing */

		p_count = 0;
		for (i = 0; i < dipent.n; i++) {
		    p = dipent.players[i].power;
		    if (p >= WILD_PLAYER) continue;
		    if (dipent.players[i].controlling_power == 0) {
			p_count++;
			p_list[p] = p_count;
		    }
		}
		/* OK, now go through the non-playing powers, substituting player index */
		for (i = 0; i < dipent.n; i++) {
		    p = dipent.players[i].power;
		    if (p >= WILD_PLAYER) continue;
		    if (dipent.players[i].controlling_power != 0) {
		        p_list[p] = p_list[dipent.players[i].controlling_power];
		    }
		}
	   }

	   for (i = 0; i < dipent.n; i++) {
	       p = dipent.players[i].power;
	       if (p >= WILD_PLAYER) continue;
	       fprintf(rfp, "%s: ", powers[p]);
	       for (ii = strlen(powers[p]); ii < LPOWER; ii++)
	           putc(' ', rfp);
	       if (dipent.flags & F_INTIMATE) {
	           if (dipent.players[i].controlling_power == 0)
		       fprintf(rfp, "%s", powers[p]);
	           else
		       fprintf(rfp, "%s", powers[dipent.players[i].controlling_power]);
	       } else {
	           if (p_list[p])
		       fprintf(rfp, "Player %d", p_list[p]);
	       }
	       fprintf(rfp, ".\n");
	   }
	}


	if (dipent.flags & F_INTIMATE) {
	   SetupIntimateTreasury();
	   putc('\n', rfp);
	   PrintTwoColTable("Treasury Totals", "Power", "Balance");
	   /* Print out treasury changes */

	   for (i = 0; i < dipent.n; i++) {
	       if (dipent.players[i].controlling_power != 0) continue;
	       p = dipent.players[i].power;
	       if (p >= WILD_PLAYER) continue;
	       fprintf(rfp, "%s: ", powers[p]);
               for (ii = strlen(powers[p]); ii < LPOWER; ii++)
                   putc(' ', rfp);
	       fprintf(rfp, "%d", ducats[p].treasury);
	       if (new_flag) {
		   /* It is a new turn, so recalculate treasury */
                   fprintf(rfp, " + %d = %d", 
                       np[p], ducats[p].treasury + np[p]);
                   ducats[p].treasury += np[p]; 
	       } 
	       fprintf(rfp, ".\n");
	           
	   }
        }
	return statusval;
}


/* If CaptureWin is set, see if one or more players have captured all Home SCs of another */
/* Set passed status to -1 if someone has won */
void CheckCaptureWin(int *status)
{

    int p;

    for (p = 1; p <= npr; p++) {

    /* To Do!!! */

    }


}
static int OnlyOneSurvivor()
{
   /* See if only one player has survive */
    int i, win = 0, pow = 0;

    for (i = 0; i < dipent.n && win < 2; i++) {
        if (!(dipent.players[i].status & SF_DEAD) && dipent.players[i].controlling_power == 0 &&
	    dipent.players[i].power < WILD_PLAYER) {
	    win++;
	    pow = i;
	}
    }

     if (win == 1)
	return pow;
    else if (win == 0)
	return -1; /* no body left alive! */
     else
	return 0;  /* No-body won yet */
}

static int AnotherPlayersHC( int u, int possible_victor[MAXPLAYERS] )
{
    /* See if unit is occupying another players HC */

    int win = 0, owner_index, player_index;
    
    if (centre(unit[u].loc)) {
        if (pr[unit[u].loc].type != 'x') {
	    /* OK, got a powers centre, now find the player */

	    player_index = FindPower(power(pr[unit[u].loc].type));
	    owner_index = FindPower(unit[u].owner);
	    if (player_index != owner_index &&
                player_index >= 0 && player_index < dipent.n) {
	        /* If the the unit and home centres are both a real players
                 * and the province's owner is not dead, it is a match */
                if (dipent.players[player_index].controlling_power == 0 &&
                    dipent.players[owner_index].controlling_power == 0 &&
                    !(dipent.players[player_index].status & SF_DEAD)) {
	            win = unit[u].owner;
		    possible_victor[owner_index]++;
		}
	    }
	 }
    }

    return win;
}

int FindMaximum(int possible_victor[MAXPLAYERS], int limit)	
{
    /* See if there is only one in the array with a value >= limit */

    int i;
    int win = 0, count = 0;
    int maxval = 0;

    for (i = 0; i < dipent.n; i++) {
        if (possible_victor[i] > maxval) {
           win = dipent.players[i].power;
	   maxval = min(limit, possible_victor[i]);
	   count++;
        }
    }

    if (count > 1) {

       /* More than one possible winner, so zap all other entries */
       for (i = 0; i < dipent.n; i++) {
	    if (possible_victor[i] < maxval) {
	        possible_victor[i] = 0;
	    } else {
	        possible_victor[i] = maxval;
	    }
        }
        return 0;
    }
    
    return win;
}
       
static int FindRichest(int possible_victor[MAXPLAYERS])
{
    /* Find if one of the players has more money than the others */

    int i;
    int win = 0, count = 0;
    int maxval = 0;

    for (i = 0; i < dipent.n; i++) {
        if (ducats[i].treasury == maxval && maxval != 0) {
	    count++;
	} else if (ducats[i].treasury > maxval) {
	    count = 1;
	    win = i;
	    maxval = ducats[i].treasury;
	}
    }

    if (count == 1)
        return win;
    else
	return 0;
}



int CheckIntimateVictory()
{
/* OK, see if we have conditions for an intimate victory */
/* return positive if so, otherwise 0 if still playing, or -1 if all dead */
	

/* Firstly, see how many powers have a unit on another players HC */

    int occupied = 0;
    int i, u, v;
    int possible_victor[MAXPLAYERS];

    for (i=0; i < MAXPLAYERS; i++)
	possible_victor[i] = 0;

    victor = OnlyOneSurvivor();

    if (victor != 0)
	return victor;


    for (u = 1; u <= nunit; u++) {
	v = AnotherPlayersHC(u, possible_victor);
	if (v) {
            if (victor <= 0) {
                victor = v;
                occupied++;
            } else if (victor != v)
                occupied++; /* Only mark if a different person is also a victor */
	}
    }

    if (occupied == 1)
        return victor;  /* only one power is occupying */
    if (occupied == 0)
	return 0; /* No winner */

    /* OK, to be here we have more than one power occupying a home centre 
     * So now, let's see if just one person has one
     */

    victor = FindMaximum(possible_victor, 3);  /* Only count up to 3 */

    if (victor > 0)
	return victor;  /* Only one person won */

    /* OK, more than one person has occupied, find the richest one */

    victor = FindRichest(possible_victor);  

    if (victor)
	return victor;  /* There was only one richest person */

    return 0;  /* No one was in conditions to have won */
}

static void next_phase(void)
{
	int status = 0;

	if (dipent.flags & F_INTIMATE) 
	    status = CheckIntimateVictory();

	if (status != 0) 
	    dipent.phase[6] = 'X';

	if (dipent.phase[0] == 'F') {

		/* Init build phase or advance to the next spring */
		newowner();
		status = ownership(1);
		if (status >=0 && (dipent.x2flags & X2F_CAPTUREWIN))
		    CheckCaptureWin(&status);

		if (status < 0 && !(dipent.flags & F_INTIMATE)) {	/* VICTORY */
			dipent.phase[6] = 'X';
			/* fix bug 219 -- increment the phase even if the game
			 * is over, just in case somebody resumes it.
			 */
			dipent.phase[5] = 'B';
		} else {
			if (status)
				dipent.phase[5] = 'B';
			else
				next_year();
		}
	} else {
	 	if ((dipent.x2flags & X2F_SUMMER) && dipent.phase[0] != 'U') 
		    dipent.phase[0] = 'U';
		else if (dipent.phase[5] == 'A')
		    dipent.phase[5] = 'M';
		else
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

void process_input(int pt, char phase, int player)
{
	char *s;
	int u, p;
	int status;

	if (pt != MASTER && GAME_PAUSED) {
	    fprintf(rfp,"\n\n ***Game is suspended: no orders allowed until master continues it again.***\n\n");
	    err++;
	    return;
	}

	for (u = 1; u <= nunit; u++)
		unit[u].order = 'n';

	init_build();


	do {
		if (preprocess(&s, &p))
			continue;
		while (!eol) {
			while (*s == ' ')
				s++;

			if ((GAME_SETUP)) {
			    status = setupin(&s, p);
			} else {
			if ((player >= 0) && (dipent.players[player].status & SF_NOT_APPROVED)) {
                            status = 1;
                            fprintf(rfp, "You are not approved to make moves: please contact the Master.\n\n");
                        } else 
			switch (phase) {
			case 'A':
				SetupIntimateTreasury();
				status = bidin(&s, p);
				break;
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

	if (pt != MASTER && GAME_PAUSED) {
	    return E_WARN;
	}
	if (GAME_SETUP) {
	    setupout(pt);
	    return 1;
	}
	switch (phase) {

	case 'A':		/* Bids */
		bidout(pt);
		if (processing) {
		    next_phase();
		}
		break;

		
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
		{
			fprintf(log_fp,"Processing retreat.\n");
			fprintf(log_fp,"before victor: %d\n", victor);
			next_phase();
			fprintf(log_fp,"After next phase, victor %d.\n",victor);
		}
		break;

	default:
		fprintf(rfp, "Unknown phase type %s.\n", dipent.phase);
		return E_FATAL;
	}
	return 1;		/* not reached */
}

int
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
