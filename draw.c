/*
 * $Log$
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

#include "dip.h"
#include "functions.h"
#include "mail.h"

int check_can_vote(int i)
{
	if (dipent.players[i].power == OBSERVER) {
		fprintf(rfp, "Observers are not eligible to vote on a draw.\n");
		return 1;
	}
	if (dipent.players[i].power == MASTER) {
		fprintf(rfp, "The Master is not eligible to vote on a draw.\n");
		return 1;
	}
	if (dipent.phase[0] == 'x') {
		fprintf(rfp, "You cannot declare a game which has not begun a draw!\n");
		return 1;
	}
 	
	if (!dipent.players[i].units) {
		fprintf(rfp, "You have no units and cannot vote.\n");
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
	char survivors[MAXPLAYERS + 1], participants[MAXPLAYERS + 1],
	 line[1024], line2[1024], *s, *t;
	int i, j, found = 0;
	long now;
	FILE *ofp, *dfp;
	listelem *head, *temp;
/*
 * First, make up a list of all survivors. This will help later.
 */

	for (i = 0, j = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if (dipent.players[i].units > 0 &&
		    dipent.players[i].power != -1)
			survivors[j++] = dipent.pl[dipent.players[i].power];
	}
	survivors[j] = '\0';

/*
 * If draws are DIAS, this is easy :-)
 */

	if (!(dipent.flags & F_NODIAS)) {
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
		if (dipent.players[i].status & SF_DRAW)
			head =
			    make_acceptable_list(survivors[0], dipent.players[i].pref);
		else
			head = default_acceptable_list(survivors[0]);
		while (head) {
			for (s = survivors + 1; *s; s++) {
				i = find_player(power(*s));
				if (!acceptable(head->draw, dipent.players[i].status & SF_DRAW,
					     dipent.players[i].pref, *s))
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

	sprintf(line, "D%s/draw", dipent.name);
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
	fprintf(dfp, "Draw declared: %s\n", ctime(&now));
	sprintf(line, "Game '%s' has been %s ", dipent.name,
		strlen(participants) == 1 ? "conceded to" : "declared a draw between");
	sprintf(line2, "The game was %s ",
		strlen(participants) == 1 ? "conceded to" : "declared a draw between");
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

	{
		if (dipent.variant != V_STANDARD || dipent.flags & F_GUNBOAT)
			sprintf(line, "%s dip.temp 'MNC: Draw in game %s' '%s'",
				SMAIL_CMD,
				dipent.name, MN_CUSTODIAN);
		else
			sprintf(line, "%s dip.temp 'BNC: Draw in game %s' '%s'",
				SMAIL_CMD, dipent.name, BN_CUSTODIAN);
	}
	execute(line);

	/*
	 * Force regeneration of the summary file if it's a gunboat game.
	 */

	if (dipent.flags & F_GUNBOAT) {
		sprintf(line, "D%s/msummary", dipent.name);
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
		sprintf(line, "%s -C %s -%s%s%slv%d %s", CONFIG_DIR, mflg, gflg,
			SUMMARY_CMD,
			dipent.flags & F_QUIET ? "q" : "", dipent.variant, dipent.name);
		system(line);
	}

	/*  Mail summary to HALL_KEEPER */

	sprintf(line, "%s D%s/summary 'HoF: Draw in %s' '%s'",
		SMAIL_CMD, dipent.name, dipent.name, HALL_KEEPER);
	execute(line);

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
	new = (listelem *) malloc(sizeof(listelem));
	sprintf(new->draw, "%c", the_power);
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

	if (!draw)
		return strlen(checking) == 1 && checking[0] == the_power;
	if (char_in_string(the_power, list) &&
	    !char_in_string(the_power, checking))
		return 0;
	for (s = checking; *s; s++)
		if (!char_in_string(*s, list))
			return 0;
	return 1;
}
