/*
 * $Log: draw.c,v $
 * Revision 1.20  2010-02-20 21:06:34  alange
 *
 * Bug 282. Fixes based on David Norman's notes.
 *
 * Revision 1.19  2008-11-20 23:33:49  millis
 * Fix Bug 549 to allow duplex & Mach concessions
 *
 * Revision 1.18  2004/10/23 22:43:29  millis
 * Bug 363 and 368, AlliedWin and Conced/NoDias in duplex games fixes
 *
 * Revision 1.17  2004/07/25 16:01:37  millis
 * Bug 336, allow draw/concede for Intimate games
 *
 * Revision 1.16  2004/05/22 09:08:15  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.15  2004/04/04 15:58:38  millis
 * Fixed bug 285 (inform extra custodians of game start)
 *
 * Revision 1.14  2003/07/17 00:01:29  millis
 * Use MailOut to send emails
 *
 * Revision 1.13  2003/06/29 21:37:41  nzmb
 * Made EOG draw entries broadcasted at the end of the game.
 *
 * Revision 1.12  2003/06/20 23:25:37  millis
 * Fix bug 180, missing press messages (due to too much stack variables)
 *
 * Revision 1.11  2003/02/25 07:46:14  nzmb
 * Small change to make it impossible to vote for a draw or concession if the game
 * is over.
 *
 * Revision 1.10  2003/02/12 07:46:06  nzmb
 * Fixed several bugs in the concession handling code, including a severe one
 *
 * that could, in rare circumstances, lead to an undeserved concession being
 * granted
 *
 * Revision 1.9  2002/08/27 22:27:50  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.8  2002/04/06 04:23:32  nzmb
 * Fixed bug in draw.c that caused the noconcessions flag to also reject
 * draw votes.
 *
 * Revision 1.7  2001/10/22 20:09:04  nzmb
 * Cw.c draw.c added chenges and fixes so summaries for draws and concessions are properly sent to the HALL_KEEPER address.
 *
 * Revision 1.4  2001/07/15 09:14:55  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.3  2001/05/08 07:29:27  greg
 * added subjectline to draws
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added handling of new XF_:FLAGS , and absence data elements in master.dip
 * Used gerenric flags to handle variants (not specificif tests)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/03/16 06:52:44  rpaar
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*------------------------------------------------------------------*/
/*           Code to handle DIAS and non-DIAS draws                 */
/*                                                                  */
/* Written by Jonathan S. Haas, positron@eecs.umich.edu, 4 Mar 93   */
/*------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>

#include "conf.h"
#include "config.h"
#include "dip.h"
#include "functions.h"
#include "mail.h"


/* Find if any of players controlled powers can vote in a draw/concession */
int PlayerHasNoUnitsOrCentres(int r_index)
{
    int i;

    for (i = 0; i < dipent.n; i++) 
        if (i == r_index || (dipent.players[i].controlling_power == r_index && r_index > 0)) {
	    if (dipent.players[i].units || (!(dipent.flags & F_MACH) && dipent.players[i].centers)) {
		return 0;
	    }
	}

    return 1;
}

int check_can_vote(int i, int flag)
{
	char *s = NULL;
	int r_index = RealPlayerIndex(i);

	if(flag == 1)
        {
		s = "draw";
	} else if (flag == 2) {
		s = "concession";
	} else {
                fprintf(stderr,"Invalid argument %d for check_can_vote.\n",
                        flag);
                bailout(E_FATAL); /* Is this right? */
        }
	/* Don't want concessions in games where they aren't allowed. */
	if((flag == 2) && (dipent.xflags & XF_NOCONCESSIONS))
	{
		fprintf(rfp,"No concessions are permitted in this game.\n");
		return 1;
	}
	if (dipent.players[i].power == OBSERVER) {
		fprintf(rfp, "Observers are not eligible to vote on a %s.\n",s);
		return 1;
	}
	if (dipent.players[i].power == MASTER) {
		fprintf(rfp, "The Master is not eligible to vote on a %s.\n",s);
		return 1;
	}
	if (dipent.phase[0] == 'x') {
		fprintf(rfp, "You cannot declare a game which has not begun a %s!\n",s);
		return 1;
	}
	if(dipent.phase[6] == 'X') {
                fprintf(rfp, "This game is complete. You can't vote for a %s.\n",s);
                return 1;
        }
 
 	if (PlayerHasNoUnitsOrCentres(r_index) ) {
		fprintf(rfp, "You have no units or centers and cannot vote on a %s.\n",s);
		return 1;
	}
	return 0;
}

int find_player(int ordinal)
{
	int i;
	for (i = 0; i < dipent.n; i++)
		if (dipent.players[i].power == ordinal)
			return i;
	return -1;
}

int chkdraw(char *to_check)
{
	int i, j, k;
	int errcnt = 0;

	for (i = 0; i < strlen(to_check); i++) {
		to_check[i] = toupper(to_check[i]);
		if (!(j = power(to_check[i]))) {
			fprintf(rfp, "%c: invalid power\n", to_check[i]);
			errcnt++;
			continue;
		}
		if ((j = find_player(j)) == -1) {
			fprintf(rfp, "%c: invalid power\n", to_check[i]);
			errcnt++;
			continue;
		}
		if (dipent.flags & F_INTIMATE && (dipent.players[j].controlling_power != 0)) {
		        fprintf(rfp, "%c: non-playing power cannot be included in draw\n", to_check[i]);
			errcnt++;
			continue;
		}
		if (dipent.players[j].power == OBSERVER) {
			fprintf(rfp, "Observers may not participate in draws.\n");
			errcnt++;
			continue;
		}
		if (dipent.players[j].power == MASTER) {
			fprintf(rfp, "The Master is ineligible to participate in a draw.\n");
			errcnt++;
			continue;
		}
		if (!dipent.players[j].units) {
			fprintf(rfp, "%s has no units and is ineligible to participate in "
			   "a draw.\n", powers[dipent.players[j].power]);
			errcnt++;
			continue;
		}
		for (k = 0; k < i; k++)
			if (to_check[k] == to_check[i]) {
				fprintf(rfp, "%c: duplicate power\n", to_check[k]);
				errcnt++;
				continue;
			}
	}
	if (strlen(to_check) > 20) {
		fprintf(rfp, "Draws can include at most 20 powers.\n");
		errcnt++;
	}
	if (errcnt)
		fprintf(rfp, "%d %s encountered in draw list.\n", errcnt,
			(errcnt == 1) ? "error" : "errors");
	return errcnt;
}

/* Chkconc, checks a concession string for validity */

int chkconc(char *to_check)
{
	int i, j=0;
        int errcnt = 0;
                        
        for (i = 0; i < strlen(to_check); i++) {
                to_check[i] = toupper(to_check[i]);
                if (!(j = power(to_check[i]))) {
                        fprintf(rfp, "%c: invalid power\n", to_check[i]);
                        errcnt++;
                        continue;   
		}

	if ((j = find_player(j)) == -1) {
                        fprintf(rfp, "%c: invalid power\n", to_check[i]);
                        errcnt++;
                        continue;
                }

	if (dipent.players[j].power == OBSERVER) {
                        fprintf(rfp, "You may not concede the game to an observer.\n");
                        errcnt++;
                        continue;   
                }
        if (dipent.players[j].power == MASTER) {
                        fprintf(rfp, "You may not concede the game to the master.\n");
                        errcnt++;
                        continue;
                }

        if (dipent.flags & F_INTIMATE && (dipent.players[j].controlling_power != 0)) {
                   fprintf(rfp, "%c: non-playing power cannot have concession\n", to_check[i]);
	           errcnt++;
	           continue;
											                      }


	if (strlen(to_check) > 1) {
	        fprintf(rfp, "You may not concede to more than 1 player!\n");
                errcnt++;  
		}
	}


	/* Now here's the tricky part -- we only want the player to be able
	** to concede the game to the largest power.
	*/

	for(i = 0; i <= dipent.n; i++)
	{
		if(dipent.players[i].power == MASTER) continue;
		if(dipent.players[i].power == OBSERVER) continue;
		if(dipent.players[i].power == dipent.players[j].power)
			continue;
		
		if(dipent.players[j].centers <= dipent.players[i].centers &&
		   !(dipent.flags & F_INTIMATE) && !(dipent.flags & F_MACH))
		{
                        fprintf(rfp, "%s has at least as many centers as %s. You may only concede to the largest power on the board.\n",
                            powers[dipent.players[i].power],
			    powers[dipent.players[j].power]);
                        errcnt++;
		}
	}
	if (errcnt)
	{
                fprintf(rfp, "%d %s encountered in concession.\n", errcnt,
                        (errcnt == 1) ? "error" : "errors");
	}
        return errcnt;
}

int char_in_string(char the_char, char *the_string)
{
	while (*the_string)
		if (*(the_string++) == the_char)
			return 1;
	return 0;
}

/*
 * Why a linked list? Well, why not?
 */
typedef struct list {
	char draw[MAXPLAYERS + 1];
	struct list *next;
} listelem;
listelem *make_acceptable_list(char, char *);
listelem *default_acceptable_list(char);
int acceptable(char *, int, char *, char);

int process_draw(void)
{
	static char survivors[MAXPLAYERS + 1], participants[MAXPLAYERS + 1],
	 line[1024], line2[1024], *s, *t;
	int i, j, found = 0;
	long now;
	FILE *ofp, *dfp;
	listelem *head, *temp;
	int first_index, concession = 1;
/*
 * First, make up a list of all survivors. This will help later.
 */

	for (i = 0, j = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if (dipent.players[i].units > 0 &&
		    dipent.players[i].power != -1 && !(dipent.flags & F_INTIMATE &&
			dipent.players[i].controlling_power == 0))
			survivors[j++] = dipent.pl[dipent.players[i].power];
	}
	survivors[j] = '\0';

/*
 * If draws are DIAS, this is easy :-)
 */

	if (!(dipent.x2flags & X2F_NODIAS)) {
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (dipent.players[i].units > 0 &&
			    !(dipent.players[i].status & SF_DRAW))
				return 0;
		}
		strcpy(participants, survivors);
	} else {

/*
 * No-DIAS draws are trickier.
 */
		participants[0] = '\0';
		i = find_player(power(survivors[0]));
		if (dipent.players[RealPlayerIndex(i)].status & SF_DRAW)
			head =
			    make_acceptable_list(survivors[0], dipent.players[i].draw);
		else
			head = default_acceptable_list(survivors[0]);
		while (head) {
			for (s = survivors + 1; *s; s++) {
				i = find_player(power(*s));
				if (!acceptable(head->draw, dipent.players[RealPlayerIndex(i)].status & SF_DRAW,
					     dipent.players[RealPlayerIndex(i)].draw, *s))
					break;
			}
			if (!(*s) && strlen(head->draw) > strlen(participants)) {
				strcpy(participants, head->draw);
				found = 1;
			}
			temp = head;
			head = head->next;
			free(temp);
		}
		while (head) {
			temp = head;
			head = head->next;
			free(temp);
		}
		if (!found)
			return 0;
	}

/*
 * Hooray, we've got a draw.
 */

 /* See if was draw including only the same player or not */
	first_index =  RealPlayerIndex(find_player(power(*(participants+1))));
	for (s = participants+1; *s; s++) {
	    if (first_index != RealPlayerIndex(find_player(power(*s)))) 
	        concession = 0;
	}
	    
	sprintf(line, "%s%s/draw",GAME_DIR, dipent.name);
	if ((dfp = fopen(line, "w")) == NULL) {
		fprintf(log_fp, "draw: Error opening draw file.\n");
		bailout(E_FATAL);
	}
	if ((ofp = fopen("dip.temp", "w")) == NULL) {
		fprintf(log_fp, "draw: Error opening second temporary file.\n");
		bailout(E_FATAL);
	}
	msg_header(ofp);

	time(&now);
	fprintf(dfp, "%s declared: %s\n", concession ? "concession" : "draw", ctime(&now));

	sprintf(line, "Game '%s' has been %s ", dipent.name,
		concession ? "conceded to" : "declared a draw between");
	sprintf(line2, "The game was %s ",
		concession ? "conceded to" : "declared a draw between");
	for (s = participants; *s; s++) {
		strcat(line, powers[power(*s)]);
		strcat(line2, powers[power(*s)]);
		if (*(s + 1)) {
			if (*(s + 2)) {
				strcat(line, ", ");
				strcat(line2, ", ");
			} else {
				strcat(line, " and ");
				strcat(line2, " and ");
			}
		}
	}

	sprintf(subjectline, "%s:%s - %s %s: %s", JUDGE_CODE, dipent.name, dipent.phase, 
		concession ? "Game Conceded to" : "Draw Declared", participants);

	dipent.phase[6] = 'X';

	strcat(line, ". Congratulations on a game well-played.");
	strcat(line2, ".");
	for (t = s = line, i = 0; *s; s++, i++) {
		if (i > 78) {
			while (*--s != ' ');
			*s++ = '\0';
			fprintf(ofp, "%s\n", t);
			fprintf(rfp, "%s\n", t);
			mfprintf(bfp, "%s\n", t);
			pprintf(cfp, "%s%s\n", NowString(), t);
			t = s;
			i = 0;
		}
	}
	fprintf(ofp, "%s\n\n", t);
	fprintf(rfp, "%s\n\n", t);
	mfprintf(bfp, "%s\n\n", t);
	pprintf(cfp, "%s%s\n\n", NowString(), t);

	for (t = s = line2, i = 0; *s; s++, i++) {
		if (i > 78) {
			while (*--s != ' ');
			*s++ = '\0';
			fprintf(dfp, "%s\n", t);
			t = s;
			i = 0;
		}
	}
	fprintf(dfp, "%s\n", t);
	fclose(dfp);

	/*
	 * If game is standard, it has a Boardman Number, if not
	 * it has a Miller Number.  Check to see if it is an EP
	 * game.  Alert Custodians to draw
	 */

	fclose(ofp);
	msg_header_done = 0;

	{
		sprintf(line, "%s '%s: %s in game %s'", "%s",
				"%s", concession ? "Concession" : "Draw",
				"%s");
		InformCustodians(dipent.name,
				 line,
				 dipent.variant,
				 dipent.flags & F_GUNBOAT); 
	}

	/*
	 * Force regeneration of the summary file if it's a gunboat game.
	 */

	if (dipent.flags & F_GUNBOAT) {
		sprintf(line, "%s%s/msummary",GAME_DIR, dipent.name);
		remove(line);
	}
	/* 
	 * This code, ripped from mail.c, around line 1000, should force
	 * a new summary to be created nicely
	 */
	{
		char *mflg, *gflg;
		gflg = (dipent.flags & F_GUNBOAT &&
			(dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL)) ? "g" : "";
		mflg = (*gflg && dipent.players[player].power == MASTER) ? "m" : "";
		sprintf(line, "%s -C %s -%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, mflg, gflg,
			dipent.flags & F_QUIET ? "q" : "", dipent.variant, dipent.name);
		system(line);
	}

	/*  Mail summary to HALL_KEEPER */

	sprintf(line, "%s%s/summary 'HoF: %s in %s'",
		GAME_DIR, dipent.name, concession ? "Consession" : "Draw", dipent.name);
	MailOut(line, HALL_KEEPER);

	/* send eog diaries */
	send_diary();

	broadcast = 1;
	return 1;
}
int process_conc(void)
{
	/*
	** Is there a concession here? Only this function knows. First
	** we will flag the largest player (since only he can be conceded
	** to), and then see if everyone else wants to concede to him.
	*/
	int largest = -1;
	int i;
	char *s, *t;
	static char line[1024],line2[1024];
	FILE *ofp, *dfp;
	long now;

	for(i = 0; i < dipent.n; i++)
	{
		if(dipent.players[i].power == OBSERVER || dipent.players[i].power == MASTER)
			continue;
		if(largest == -1 && !(dipent.flags & F_INTIMATE))
		{
			largest = i;
			continue;
		}
		if (dipent.flags & F_INTIMATE) {
		    /* In Intimate, use the preference chosen */
		    if (dipent.players[i].status & SF_CONC) {
		        largest = FindPower(power(dipent.players[i].draw[0]));
		    }
		} else {
 		    if(dipent.players[i].centers > dipent.players[largest].centers)
			largest = i;
		}
	}

	if (largest == -1) return 0;  /* No one in conditions to concede */

	for(i = 0; i < dipent.n; i++)
	{
		if(i == largest) continue;
		if(dipent.players[i].power == OBSERVER || dipent.players[i].power == MASTER)
			continue;

		if (dipent.flags & F_INTIMATE) {
		    if (dipent.players[i].controlling_power == 0 &&
		        dipent.players[i].centers > 0) {
	                if (!(dipent.players[i].status & SF_CONC) ||
			    (FindPower(power(dipent.players[i].draw[0])) != largest)) {
				return 0;
			}
		    }
		} else {
			
		    if(dipent.players[i].centers > 0 &&
			!(dipent.players[i].status & SF_CONC))
				return 0;
		}
	}
	/* OK, if we're this far, we have a concession. This next set of 
	   code is ripped, with some modifications, from process_draw */

	sprintf(line, "%s%s/conc",GAME_DIR, dipent.name);
        if ((dfp = fopen(line, "w")) == NULL) {
                fprintf(log_fp, "conc: Error opening concession file.\n");
                bailout(E_FATAL);
        }
        if ((ofp = fopen("dip.temp", "w")) == NULL) {
                fprintf(log_fp, "draw: Error opening second temporary file.\n");
                bailout(E_FATAL);  
        }
        msg_header(ofp);   
                        
        time(&now);
        fprintf(dfp, "Concession declared: %s\n\n", ctime(&now));
	fprintf(dfp, "The game was conceded to %s.\n", powers[dipent.players[largest].power]);
	fclose(dfp);

	sprintf(line,"Game %s conceded to %s.\n",dipent.name,powers[dipent.players[largest].power]);

	sprintf(subjectline,"%s:%s - %s. Game conceded to %s",JUDGE_CODE,dipent.name,dipent.phase,
		powers[dipent.players[largest].power]);

	dipent.phase[6] = 'X';

	strcat(line, "The game is over. Thank you for playing.");
        strcat(line2, ".");   

	/* We think this code just wraps lines. */
	for (t = s = line, i = 0; *s; s++, i++) {
                if (i > 78) {
                        while (*--s != ' ');
                        *s++ = '\0';
                        fprintf(ofp, "%s\n", t);
                        fprintf(rfp, "%s\n", t);
                        mfprintf(bfp, "%s\n", t);
                        pprintf(cfp, "%s%s\n", NowString(), t);
                        t = s;
                        i = 0;
                }
        }
	fprintf(ofp, "%s\n\n", t);
        fprintf(rfp, "%s\n\n", t);  
        mfprintf(bfp, "%s\n\n", t);
        pprintf(cfp, "%s%s\n\n", NowString(), t);
                        
        for (t = s = line2, i = 0; *s; s++, i++) {
                if (i > 78) { 
                        while (*--s != ' ');
                        *s++ = '\0';
                        fprintf(dfp, "%s\n", t);
                        t = s;
                        i = 0;
                }
        }
	fclose(ofp);
	msg_header_done = 0;

	/* Now we must notify the various custodians of the concession. */
	InformCustodians(dipent.name,
	                 "%s '%s: Concession in game %s'",
		         dipent.variant,
		         dipent.flags & F_GUNBOAT);
	
	/* Regenerate the summary */
	if (dipent.flags & F_GUNBOAT) {
                sprintf(line, "%s%s/msummary",GAME_DIR, dipent.name);
                remove(line);
        }
	{  
                char *mflg, *gflg;
                gflg = (dipent.flags & F_GUNBOAT &&
                        (dipent.phase[6] != 'X' || dipent.flags & F_NOREVEAL)) ? "g" : "";
                mflg = (*gflg && dipent.players[player].power == MASTER) ? "m" : "";
                sprintf(line, "%s -C %s -%s%s%slv%d %s", SUMMARY_CMD, CONFIG_DIR, mflg,gflg,
                        dipent.flags & F_QUIET ? "q" : "", dipent.variant,dipent.name);
                system(line);
	}
	/* Now mail the summary to the hall keeper. */
	sprintf(line, "%s%s/summary 'HoF: Concession to %s in %s'",
                GAME_DIR, dipent.name,powers[dipent.players[largest].power],
		dipent.name);
	MailOut(line, HALL_KEEPER);

	/* send diaries */
	send_diary();
                        
        broadcast = 1;
	return 1;

}
/*
 * Believe it or not, the following hodgepodge of code works. It's more-
 * or-less the only good algorithm for coming up with all subsets of a
 * given set, or so saith my math major roommate.
 */

listelem *link_choice(char the_power, char *the_preflist, int flags, int cont,
		      int length, listelem * link_to)
{
	char the_draw[MAXPLAYERS + 1], *s;
	int i, c;
	listelem *new;

	the_draw[0] = '\0';
	s = the_draw;
	for (i = 1, c = 0; i < length; i <<= 1, c++)
		if (flags & i)
			*(s++) = the_preflist[c];
	*s = '\0';
	if (cont && !char_in_string(the_power, the_draw))
		return link_to;
	new = (listelem *) malloc(sizeof(listelem));
	new->next = link_to;
	strcpy(new->draw, the_draw);
	return new;
}

listelem *default_acceptable_list(char the_power)
{
	listelem *new;
	char main_power;  /* Set to letter of principle power */
	int index1;
	new = (listelem *) malloc(sizeof(listelem));
	index1 = RealPlayerIndex(find_player(power(the_power)));
	main_power = dipent.pl[dipent.players[index1].power];
	sprintf(new->draw, "%c", main_power);
	new->next = NULL;
	return new;
}

listelem *make_acceptable_list(char the_power, char *the_preflist)
{
	long unsigned int i, length;
	int containsflag;
	listelem *retvalue = NULL, *secondlist = NULL, *traverse, *new;

	containsflag = char_in_string(the_power, the_preflist);
	length = 1 << strlen(the_preflist);
	for (i = 1; i < length; i++)
		retvalue =
		    link_choice(the_power, the_preflist, i, containsflag, length, retvalue);
	if (!containsflag) {
		for (traverse = retvalue; traverse; traverse = traverse->next) {
			new = (listelem *) malloc(sizeof(listelem));
			new->next = secondlist;
			sprintf(new->draw, "%c%s", the_power, traverse->draw);
			secondlist = new;
		}
		new = (listelem *) malloc(sizeof(listelem));
		new->next = secondlist;
		sprintf(new->draw, "%c", the_power);
		secondlist = new;
		for (traverse = retvalue; traverse->next; traverse = traverse->next);
		traverse->next = secondlist;
	}
	return retvalue;
}

int acceptable(char *checking, int draw, char *list, char the_power)
{
	char *s;
        int index1, index2;
        index1 = RealPlayerIndex(find_player(power(the_power)));

	if (!draw) {
		while (*checking != '\0') {
		    index2 = RealPlayerIndex(find_player(power(*checking)));
		    if (index2 != index1)
			return 0;
		    checking++;
		}
		return 1;
	}
	if (char_in_string(the_power, list) &&
	    !char_in_string(the_power, checking))
		return 0;
	for (s = checking; *s; s++)
		if (!char_in_string(*s, list))
			return 0;
	return 1;
}
