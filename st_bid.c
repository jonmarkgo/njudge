/*
   ** $Log$
   ** Revision 1.4  2004/06/11 17:27:48  millis
   ** Bug 297: Minor change to not show zero bids in results
   **
   ** Revision 1.3  2004/06/09 22:05:09  millis
   ** More fixes for Bug 297, Intimate Diplomacy
   **
   ** Revision 1.2  2004/06/05 09:00:30  millis
   ** Bug 297 : problems with calculating effective bids fixed
   **
   ** Revision 1.1  2004/05/22 08:50:14  millis
   ** Bug 297: Add Intimate Diplomacy
   **

 */

/*  st_bid.c
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the
   **  above notices are preserved.
   **
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"


/****************************************************************************/
static int power_bid_total[NPOWER+1];      /* total of a power's bids */
static int power_overbid[NPOWER+1];  /* list of powers that overbid */
static int power_overbidden[NPOWER+1]; /* list of powers that were overbidden */
static int final_bid[NPOWER+1][NPOWER+1];  /* final list of bids */
static int result[NPOWER+1][NPOWER+1];	  /* Result of the bidding */
static int new_controller[NPOWER+1];

#define BID_SUCCESS 0
#define BID_FAILURE 1


/* Work out which bids have won */
static void CalculateWinningBids( void ) {
	int p, p1, p_index, bid_total, bid_power;

	for (p = 1; p <= NPOWER; p++)
	    for (p1 = 1; p1 <= NPOWER; p1++)
	        result[p][p1] = BID_SUCCESS;
	
	for (p1 = 1; p1 <= NPOWER; p1++) { /* This loop is just to order by power */
	    p_index = FindPower(p1);
	    bid_total = 0;
	    bid_power = 0;
	    if (p_index >= dipent.n) continue; /* Not a valid power */
	    new_controller[p1] = AUTONOMOUS;  /* no-one yet controls this one */

            for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
	    p_index = FindPower(p);
	    if (p_index >= dipent.n) continue; /* Not a valid power */ 
            if (dipent.players[p_index].controlling_power != 0) continue;  /* not a real player */
	    if (final_bid[p][p1] == bid_total) {
                    /* Same bid, null out both */
                    final_bid[p][p1] = 0;
		    final_bid[bid_power][p1] = 0;
		    result[p][p1] = BID_FAILURE;
		    result[bid_power][p1] = BID_FAILURE;
		    bid_power = 0; 
	            new_controller[p1] = AUTONOMOUS;
		}
                if (final_bid[p][p1] > bid_total) {
                    /* New higher bid */
                    bid_total = final_bid[p][p1];
		    final_bid[bid_power][p1] = 0;
		    result[bid_power][p1] = BID_FAILURE;
	            bid_power = p;
		    new_controller[p1] = p;
                }
		if (final_bid[p][p1] < bid_total) {
		    /* Bid too small */
		    final_bid[p][p1] = 0;
		    result[p][p1] = BID_FAILURE;
		}
	    }
	}
}


static int OverbidValue( int bid )
{
	int value;

	value = (bid + 1) / 2;

	return value;
}

/* Work out each power's cost for bids placed */
static int TotaliseWinningBids( void ) {

	int p, p1, p_index, one_overbid = 0;
	static int my_power_overbidden[NPOWER+1];

	for (p1 = 1; p1 <= NPOWER; p1++)
	    my_power_overbidden[p1] = 0;

        for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
             p_index = FindPower(p);
             power_bid_total[p] = 0;
             if (p_index >= dipent.n) continue; /* Not a valid power */
	     if (dipent.players[p_index].controlling_power != 0) continue; /* Not a real player */
             for (p1 = 1; p1 <= NPOWER; p1++) {
		  if (power_overbidden[p1]) {
                      power_bid_total[p] += OverbidValue(final_bid[p][p1]); 
		  } else {
		      power_bid_total[p] += final_bid[p][p1];
		  }
	     }
	     if (power_bid_total[p] > ducats[p].treasury)  {
	         one_overbid++;
		 power_overbid[p]++;
		 for (p1 = 1; p1 <= NPOWER; p1++)
		     if (bids[p][p1]) 
			my_power_overbidden[p1]++;
	     }
	}

	for (p1 = 1; p1 <= NPOWER; p1++)
	    if (my_power_overbidden[p1]) 
	        power_overbidden[p1]++;

	return one_overbid;
}
											      
											      
/* Mark who has overbid and disqualify their bids */
static void DisqualifyOverBids(void)
{
	int p, p_index, p1;

	
   for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
        p_index = FindPower(p);
        if (!power_overbid[p]) continue; /* Not an overbid power */
            for (p1 = 1; p1 <= NPOWER; p1++) {
            final_bid[p][p1] = 0;
	}
   }
	
}
/* Return the controlling power of passed power */
static int PowerControlling(int p)
{
	int i, ret = -1;

	for (i = 0; i < dipent.n && ret == -1; i++) {
	    if (dipent.players[i].power == p)
	       ret = dipent.players[i].controlling_power;
	}
	return ret;
}

int bid_syntaxcheck(char *in_text, int precheck, char *out_string)
{
	char *s = in_text;

	char temp_out[256];
	char *out_text = NULL;
	temp_out[0]='\0';
	if (out_string != NULL) out_text = temp_out;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: <power-letter> <bid>>
	 */

	/* 
         * See if a precheck is possible
	 * If so, see if it is an canprocess line and assume ok if so
         */

	if (precheck) 
	{
		if (canpreprocess(s))  return 0;
	}

	bidin(&s, 0, 1);

	return 0;
}

int bidin(char **s, int p, int syntaxcheck)
{
/*  Read build orders in from input file.  */

	int i, p1, bid_power=0, pow;

	/*
	   **  Process lines of the form:
	   **
	   **    cmd   ::= <power>: <power-letter> <bid>
	 */


	if (p == MASTER) {
	    return 0;  /* Ignore bids from the master */
	    errmsg("Master cannot bid: use 'become' command if necessary.\n"); 
	    err--;  /* take away error that will be created, as master can't have errors! */
	    return E_WARN;
	}

	get_power(*s, &pow);

	if (!pow) {
	    pow = power(**s);
	    if (pow <= 0 || pow >= WILD_PLAYER) {
                errmsg("Unrecognized power -> %s", *s);
	        return E_WARN;
	    }
	    *s+= 2;  /* Skip power letter and space */
	} else {
	    *s = get_power(*s, &i);  /* Consume the power text (if there) */
	}


	for (i = 0; i < dipent.n && !bid_power && !syntaxcheck; i++) {
	    if (dipent.players[i].power == pow ) {
                if (dipent.players[i].controlling_power == 0) {
                    errmsg("Cannot bid to control %s", *s);
                    return E_WARN;
                } else {
                    bid_power = i;
		}
            }
        }
	
        if (!bid_power && !syntaxcheck) {
            errmsg("Unable to find power %s", *s);
            return E_WARN;
	}

	*s = get_amount(*s, &p1);
	if (p1 < 0) {
	    errmsg("Invalid amount specified -> %s", *s);
            return E_WARN;
        }

	if (!syntaxcheck)
	    bids[p][pow] = p1;

	return 0;

}

void bidout(int pt)
{
	int i, ii, p, p_index, p1, bid_total;
        int one_printed, one_overbid, one_bid_ok = 0;

	fprintf(rfp, "Bid %s for Year of %d.  (%s.%s)\n\n",
		pt ? "orders" : "results",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	/* Initialise data structure */
	for (p = 1; p <= NPOWER; p++) {
	    power_overbid[p] = 0;
	    power_overbidden[p] = 0;
	    for (p1 = 1; p1 <= NPOWER; p1++) {
		final_bid[p][p1] = bids[p][p1];
	    }
	}

	if (processing || predict) {

	    /* Work out the bid results */

	    /* 
	     * Firstly, see who had the largest bid for each power
	     * equal bids cancel out each other 
	     */

	    CalculateWinningBids();
	
	    /*
	     * Now go through, totalling all bids for each power
	     */
	
	    one_overbid = TotaliseWinningBids();

	    /*
	     * Now see if a power overbid, resetting bids if so
	     */

	    while (one_overbid) {
                /* Reinitialise data structure */
	        for (p = 1; p <= NPOWER; p++) 
	            for (p1 = 1; p1 <= NPOWER; p1++) 
		        final_bid[p][p1] = bids[p][p1];

		DisqualifyOverBids();

		/* 
	 	* Now rework out the bids
	 	*/

		CalculateWinningBids();
		one_overbid = TotaliseWinningBids();
	    }
	}


        for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
            p_index = FindPower(p);
            one_printed = 0;
            bid_total = 0;
            if (p_index >= dipent.n) continue; /* Not a valid power */
            if (!(processing || pt == p || pt == MASTER)) continue;
            if (PowerControlling(p) != 0) continue; /* Don't show powers that can't control */
            for (p1 = 1; p1 <= NPOWER; p1++) { /* This loop is just to order by power */

                for (i = 0; i < dipent.n; i++) {
                    if (dipent.players[i].power == p1 &&
                        (dipent.players[i].controlling_power != 0 || (dipent.x2flags & X2F_SECRET))&& 
			bids[p][p1] >= 0) {

	                if (bids[p][p1] > 0 || (!predict && !processing)) {
			    fprintf(rfp, "%s: ", powers[p]);
			    for (i = strlen(powers[p]); i < LPOWER; i++)
			        putc(' ', rfp);
			    if (bids[p][p1] == 0 && !predict && !processing)
		                fprintf(rfp, "No bid made for %s.",  powers[p1]);
			    else 
			        fprintf(rfp, "Bid for control of %s is %d.",
		                    powers[p1], bids[p][p1]);
			}

			if (bids[p][p1] > 0) {
		            if (processing || predict) {
			        if (result[p][p1] && bids[p][p1] > 0) {
				    fprintf(rfp, " (*failed*)");
				}
			    }
			}
			if ((!predict && !processing) || bids[p][p1] > 0) {
			    putc('\n', rfp);
                            one_printed++;
			    bid_total += bids[p][p1];
			}
                    }
                }
            }

            fprintf(rfp, "%s: ", powers[p]);
            for (i = strlen(powers[p]); i < LPOWER; i++)
                 putc(' ', rfp);
            fprintf(rfp,"Bid total = %d", bid_total);
	    if (!processing && !predict)
	        fprintf(rfp, " from treasury of %d.\n", ducats[p].treasury);
	    else
		fprintf(rfp, ".\n");
	    if (bid_total > ducats[p].treasury && p == pt) 
	        fprintf(rfp, " (*Warning: Bids exceed treasury total*)");
	    fprintf(rfp,"\n\n");
        }

    if (processing || predict ) {
	PrintTwoColTable("Successful Bid Totals", "Power", "Bids Total");
	for (p = 1; p <= NPOWER; p++) { /* This loop is just to order by power */
           p_index = FindPower(p);
	   if (p_index >= dipent.n) continue; /* Not a valid power */
	   if (PowerControlling(p) != 0) continue; /* Don't show powers that can't control */
	   fprintf(rfp, "%s: ", powers[p]);
	   for (ii = strlen(powers[p]); ii < LPOWER; ii++)
	       putc(' ', rfp);
	   if (power_overbid[p])
	      power_bid_total[p] = ducats[p].treasury;

	   fprintf(rfp, "%d.", power_bid_total[p]);

	   if (power_overbid[p]) 
	       fprintf(rfp, " (*Overbid*)");
	   else
	       one_bid_ok++;

	   putc('\n', rfp);
	}

	if (processing || predict) {

	    fprintf(rfp, "\n\nPower control statement\n");
	    fprintf(rfp,   "-----------------------\n");

	    for (i = 0; i < dipent.n; i++) {
	        if (dipent.players[i].controlling_power == 0) continue;
		p = dipent.players[i].power;
		fprintf(rfp, "%s: ", powers[p]);
		for (ii = strlen(powers[p]); ii < LPOWER; ii++)
		     putc(' ', rfp);
               	fprintf(rfp, "is now controlled by %s.\n",
		    powers[new_controller[p]]);
		if (processing)
			dipent.players[i].controlling_power = new_controller[p];
	    }
		
	    putc('\n', rfp);

	    PrintTreasury(pt, power_bid_total, processing, predict);

	}
    }

    if (processing && !one_bid_ok) {
        /* all players went over budget: force a game draw end */
	/* TODO */
    }

}	        
/****************************************************************************/
