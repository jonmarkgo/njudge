/*
 * $Log: ml_signon.c,v $
 * Revision 1.50  2010-08-18 00:19:14  chrisb
 * Adds configurable password and game name behavior
 * If enabled in dip.h with AUTH_ constants:
 *    Game names may contain non-alphanumerics.
 *    Passwords may contain non-alphanumerics.
 *    Passwords may be case sensitive.
 * "Alphanumeric" is locale dependant.
 * To allow games names and passwords foreign to the judge locale,
 * change AUTH_WEAK in dip.h from 1 to 0.
 *
 * Revision 1.49  2010-02-20 21:06:34  alange
 *
 * Bug 282. Fixes based on David Norman's notes.
 *
 * Revision 1.48  2006-06-07 10:31:58  millis
 * Fix Bug 473, specify dedsystems file.
 *
 * Revision 1.47  2005/06/09 09:45:16  alange
 * Fix bug 406. Masters and observers no longer set to wait on takeover.
 *
 * Revision 1.46  2004/09/28 20:38:40  alange
 * Bug 353 Faster deadline for first turn when NO_PRESS
 *
 * Revision 1.45  2004/08/03 21:55:08  millis
 * Fix Bug 349 and 352
 *
 * Revision 1.44  2004/07/27 23:09:44  millis
 * Signon problems fixed (Bug 91)
 *
 * Revision 1.41  2004/05/22 09:23:48  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.40  2004/05/12 23:56:56  millis
 * Fix bug 298, don't advance deadline if first turn and other players still CD
 * on takeover.
 *
 * Revision 1.39  2004/05/12 23:44:04  millis
 * Fix Bug 305 so that same player can return, even if dead
 *
 * Revision 1.38  2004/04/04 15:58:38  millis
 * Fixed bug 285 (inform extra custodians of game start)
 *
 * Revision 1.36  2004/02/14 23:11:16  millis
 * Forced ocmmit, no changes
 *
 * Revision 1.35  2003/12/25 06:43:54  nzmb
 * Fixed bug #102 and another small problem in the SET START processing
 * code.
 *
 * Revision 1.34  2003/10/12 05:02:11  nzmb
 * Fixed bug #212 (players in NMR games get warned when another player, who had
 * been in CD, returns to take control of his power).
 *
 * Revision 1.33  2003/08/27 12:04:57  millis
 * Fix bug 222
 *
 * Revision 1.32  2003/08/12 23:04:10  millis
 * Fix bug 216
 *
 * Revision 1.31  2003/07/23 00:11:43  millis
 * Bug 192
 *
 * Revision 1.30  2003/07/22 23:45:22  millis
 * Bug 135
 *
 * Revision 1.29  2003/07/15 11:42:04  millis
 * Fixed compile bug
 *
 * Revision 1.28  2003/07/14 23:35:53  millis
 * Fix bug 179 - do not allow takeover of eliminated players
 *
 * Revision 1.27  2003/05/24 23:57:15  millis
 * Bug 97, removed Generic Handling code
 *
 * Revision 1.26  2003/05/24 23:42:22  millis
 * Fix bug 32, do not show signon if game terminated
 *
 * Revision 1.25  2003/05/24 21:19:23  millis
 * Bug fix 130, allow seed.variant and report.variant file names
 *
 * Revision 1.24  2003/05/05 23:22:45  nzmb
 * Change so that the Judge now looks in GAME_DIR when deciding whether or not
 * a new game is going to duplicate an already used name.
 *
 * Revision 1.23  2003/05/02 22:52:28  millis
 * Added support for neutrals
 *
 * Revision 1.22  2003/05/01 14:41:47  millis
 * Fixed small compile bug
 *
 * Revision 1.21  2003/04/27 15:45:04  millis
 * Fixed Bug 5 (allowing game to be Force begun without all players)
 *
 * Revision 1.20  2003/02/28 20:16:45  nzmb
 * Changed the name of resignation ratio to CD ratio, to avoid confusion with
 * Doug Massey's DRR.
 *
 * Revision 1.19  2003/02/17 14:33:32  millis
 * fixed bug 6, detected returning player
 *
 * Revision 1.18  2003/01/16 22:52:24  millis
 * Removed STAT function
 *
 * Revision 1.17  2003/01/15 13:59:27  millis
 * Removed Dipstats
 *
 * Revision 1.16  2003/01/14 13:51:38  millis
 * Merges from USTV
 *
 * Revision 1.15  2002/08/27 22:27:55  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.13  2002/07/16 18:14:23  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.12  2002/04/18 04:44:33  greg
 * Added the following commands:
 * - unstart
 * - set secret
 * - set [prflist|prfrand|prfboth]
 *
 * Fixed Set Absence so that "to" is not case sensitive
 *
 * Fixed Quiet games so that new players are announced
 * before the game starts
 *
 * Fixed ascii_to_ded.c so thatit no longer generates an
 * error when compiled
 *
 * Revision 1.11  2001/11/12 11:12:54  miller
 * Fix to correct res_rat if over 1.0
 *
 * Revision 1.10  2001/08/31 02:05:22  greg
 * added "Ready to Start" subjectline for manual start games
 * and fixed subjectline problems with preference changes
 *
 * Revision 1.9  2001/07/15 09:18:34  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.8  2001/06/24 06:03:28  nzmb
 * Added functionality to enforce the new dedication systems, while keeping
 * intact Millis' previous changes.
 *
 * Revision 1.6  2001/05/09 06:38:28  greg
 * added subjectlines
 *
 * Revision 1.5  2001/04/15 21:21:22  miller
 * Only send warning for different emails if not a substring
 *
 * Revision 1.4  2001/01/06 18:48:43  davidn
 * Changes to allow game creator to be automatically made master
 *
 * Revision 1.3  2000/11/14 14:27:37  miller
 * Small changes for Blind/Wings setup data files, also handling of JK signon from
 * any email account, adjustment of takeover countries as according to game settings (deadline that is)
 * Showing of preference and absence settings on signon (the first restricted to master only)
 * Handling of BLankBoard games (where game starts with no pieces)
 *
 * Revision 1.2  1998/03/01 12:52:19  davidn
 * Correction to sort routine added
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.4  1997/03/16 06:53:13  rpaar
 *
 * Revision 1.3  1997/02/16 20:43:18  davidn
 * Additions to dipent structure and associated code, to allow duplex variants.
 * Command is "set players n".
 *
 * Revision 1.2  1996/11/05 22:45:48  rpaar
 * Changes made by NW to suppress non standard characters as input.
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/* 
 *  mail_signon.c -- process the signon commands
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 *
 *  DATE        NAME         REASON
 *  ----------- ------------ ------------------------------------------
 *  ?? ??? 1987 Ken Lowe     He wrote it
 *  16 Oct 1996 Nathan W.    See below
 *  31 Dec 1996 David Norman Changes for dipent.no_of_players 
 *  18 Nov 1999 Millis Miller Prevent ? signons to terminated but unstarted game
 *  02 Dec 1999 Millis Miller Reset deadlines to sensible values on player takeover
 *  03 Dec 1999 Millis Miller Chose different seed/report files if blind/wings
 *  26 May 2001 Mario Becroft Changes for plyrdata record and dedication
 *		Tim Miller    systems.
 */

/*
 * Wed Oct 16 20:23:05 GMT 1996
 * reformatted file to standardize coding style --nw
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "config.h"
#include "dip.h"
#include "mail.h"
#include "ml_signon.h"
#include "variant.h"
#include "functions.h"
#include "plyrdata.h"
#include "porder.h"

#define ADMINISTRATOR	'@'

static int NewGameSignon(char *password, int lmaster, int luserid, int lsiteid, int llevel);


static int GetFirstAbandonedPower(char *power_letter)
{
    int i;
    /* Try to see if there is an abandoned power */

    for (i = 0; i < dipent.n; i++) {
        if (dipent.players[i].status & SF_ABAND) {
	    *power_letter = dipent.pl[dipent.players[i].power];
	    return 1;
	}
    }
    return 0;  /* No abandoned powers found */
}


 
/* Power starting messages for game */
static void PowerStartingList(FILE *fptr, int index)
{
    int i;
    int one_done = 0;
    char *stext = "You have been selected as %s in game '%s' of Diplomacy.";

    if (!(IS_DUPLEX(dipent)) || dipent.players[index].power >= WILD_PLAYER)
       fprintf(fptr, stext, powers[dipent.players[index].power], dipent.name);
    else {
	for (i = 0; i < dipent.n; i++) {
	    if (i == index || FindPower(dipent.players[i].controlling_power) == index) {
		if (one_done)
		    fputc('\n', fptr);
		fprintf(fptr, stext, powers[dipent.players[i].power], dipent.name);
		one_done++;
	    }
	}
    }

}

/*
 * Place the correct money values in the beginning players treasury
 */
 
void SetupIntimateTreasury(void)
{
    int i,p;
    short value;

    if (has_treasury)
	    return;  /* Game datafile already has treasury */

    if (!(dipent.flags & F_INTIMATE)) return;  /* Not Intimate, so don't do anything! */

    if (atoi(dipent.seq) > 1) return; /* Not on first turn, so ignore this call */

    for (i = 0; i < dipent.n; i++) {
	p = dipent.players[i].power;

	if (dipent.players[i].power < WILD_PLAYER && 
	    dipent.players[i].controlling_power == 0) {
	    
	    if (dipent.variant == V_STANDARD) {
                /* For standard, we have fixed values:
	         *  E, F, R & T are given 20 credits, G 22, A & I 24.
	         */
	        switch (pletter[V_STANDARD][p]) {
		    case 'I':
		    case 'A':
		        value = 24;
		        break;
		    case 'G':
		        value = 22;
		        break;
		    default:
		        value = 20;
	        }
	    } else {
                /* For non-standard games, 3 * numer of powers */
	        value = dipent.np * 3;
	    }

	} else 
	    value = 0;  /* Not a player! */

	ducats[p].treasury = value;
    }
    return;
}
/*
 * Open a data file, based on variant setting
 */

FILE *OpenDataFile(char *line, char *type)
{

 FILE *fp;

 if ((dipent.flags & F_BLIND ) && (dipent.xflags & F_WINGS)) {
     /* Bug 130, try to open first with %s.name */
     sprintf(line, "data/%s_bw.%s", type, variants[dipent.variant]);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
     sprintf(line, "data/%s_bw.%d", type, dipent.variant);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
 }

 if (dipent.flags & F_WINGS) {
     /* Bug 130, try to open first with %s.name */
     sprintf(line, "data/%s_w.%s", type, variants[dipent.variant]);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
     sprintf(line, "data/%s_w.%d", type, dipent.variant);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
 }

 if (dipent.flags & F_BLIND) {
     /* Bug 130, try to open first with %s.name */
     sprintf(line, "data/%s_b.%s", type, variants[dipent.variant]);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
     sprintf(line, "data/%s_b.%d", type, dipent.variant);
     if ( (fp = fopen(line,"r")) != NULL) return fp;
 }

  /* Bug 130, try to open first with %s.name */
  sprintf(line, "data/%s.%s", type, variants[dipent.variant]);
  if ( (fp = fopen(line,"r")) != NULL) return fp;

  sprintf(line, "data/%s.%d", type, dipent.variant);
  fp = fopen(line,"r");
  if (fp == NULL) fprintf(stderr, "igame: Error opening %s file %s.\n", type, line);
  return fp;
}

/***************************************************************************/

int mail_signon(char *s)
{

	char password[AUTH_MAX_LEN];
	int i, j, n, found;
	int jj, one_done = 0;
	int lmaster = 0; // Whether the players has been auto-promoted to master
	int luserid, lsiteid, llevel, variant = V_STANDARD, flags = 0;
	char *t, *gdirname, line[150], tmp1[2];
	int one_abandoned = 0;

/*
 *  Process SIGNON command.  The format of the signon command is:
 *
 *     signon pname password
 *
 *  where 'p' = power initial, name = game name.  If p is a question mark
 *  this is a request to join a game.  If name is not specified then it
 *  defaults to the first entry in the master file.
 */
	/* Check for non-alphanumerics in game name */

	while (isspace(*s))
		s++;		/* skip whitespace before the pname */
	for (t = name; *s && !isspace(*s); s++) {
		if (AUTH_WEAK) {
			if (ispunct(*s) && t > name) {

				fprintf(rfp, "Game names must contain only letters and numbers.\n");
				fprintf(rfp, "A '%c' was found.\n", *s);
				return E_WARN;
			}
		}	

		if (t < name + sizeof(dipent.name))
			*t++ = tolower(*s);
	}
	*t = '\0';		/* terminate the name string */

	/* Check for non-alphanumerics in password */

	t = password;
	while (isspace(*s))
		s++;
	while (*s && !isspace(*s) && t < password + sizeof(password) - 1) {
		if (AUTH_WEAK) {
			if (!isalnum(*s) && t > name) {
				fprintf(rfp, "Passwords on this judge must only contain letters and numbers.\n");
				fprintf(rfp, "A '%c' was found.\n", *s);
				return E_WARN;
			}
		}
		*t++ = *s++;
		
	}

	/* Check password specified */

	*t = '\0';
	if (!password[0]) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Password must be specified.\n");
		return E_WARN;
	}
	if (*name == '?') {

		/* Join a new game */

		while (isspace(*s))
			s++;
		variant = V_STANDARD;
		flags = 0;
		while (*s) {
			s = lookforv(s, variants, NVARIANT + NVAROPTS, &i, 1);
			if (!i) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Unknown variant specified: %s\n", s);
				fputs("Valid variants are:\n", rfp);
				for (i = 1; i < NVARIANT; i++)
					fprintf(rfp, "   %s\n", variants[i]);

				fputs("\nOptions on these variants are:\n", rfp);
				while (i < NVARIANT + NVAROPTS)
					fprintf(rfp, "   %s\n", variants[i++]);
				fputs("\n", rfp);
				return E_WARN;
			}
			if (i >= NVARIANT)
				flags |= vvalue[i];
			else
				variant = vvalue[i];
		}
		if (!getuser(raddr, &luserid, &lsiteid, &llevel)) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Your address %s is not registered with the judge.\n", raddr);
			fprintf(rfp, "Use a 'get form' request for information on registering.\n");
			return E_WARN;
		}
	}
	/*
	 *  The master file has just been opened, copy to new file looking for 
	 *  this particular entry.  Rest of file will be copied and the files
	 *  will be closed back in the calling routine.
	 */

	found = 0;
	while (getdipent(mfp)) {
		if (((name[0] == '?') && !name[1] && dipent.seq[0] == 'x')
		    || !strcmp(&name[1], dipent.name)) {
			found++;
			break;
		}
		putdipent(nfp, 1);
	}

	if (!found) {
		if (name[0] == '?') {
			struct stat sbuf;
			gdirname = malloc(strlen(GAME_DIR) + strlen(name) + 1);
			if(!gdirname)
			{
				fprintf(stderr, "Memory allocation error.\n");
				bailout(E_FATAL);
			}
			sprintf(gdirname, "%s%s", GAME_DIR, &name[1]);

			if (strcmp(raddr, GAMES_MASTER) != 0 && stat("NOCREATE", &sbuf) == 0) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, the create command has been disabled.  No new\n");
				fprintf(rfp, "games are being allowed.\n");
				return E_WARN;
			}
			if (!create) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp,
					"Please use the 'create %s <password> ...' command if you really\n",
					name);
				fprintf(rfp,
					"want to create a new game with this name.  Use the 'list' command\n");
				fprintf(rfp, "if you want to see which games already exist.\n");
				return E_WARN;
			}
			/* name[0] = 'D'; */
 			if (stat(gdirname, &sbuf) == 0) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, '%s' is a reserved name.\n", &name[1]);
				return E_WARN;
			}
			name[0] = '?';
			free(gdirname);

			if (ded[luserid].r < atoi(config("CREATE_DEDICATION"))) {
				fprintf(rfp, "You must have a minimum dedication of %i to create games\non this judge.\n", atoi(config("CREATE_DEDICATION")));
				return E_WARN;
			}
			newdipent(&name[1], variant);
			dipent.variant = variant;
			dipent.flags |= flags;
			/* MLM disabled as no longer needed 
			if (dipent.flags & F_GUNBOAT)
				dipent.flags |= F_NOPARTIAL; 
			*/
 
                        // Automatically upgrade creator to master
                        if ( !strcmp(AUTO_MASTER,"yes") && strcmp(raddr,GAMES_MASTER) ) 
			{
                                lmaster++;
                                pprintf(cfp, "%s%s is now Master for game '%s'.\n", NowString(), raddr, dipent.name);
                                dipent.flags |= F_MODERATE;
                                dipent.flags &= ~F_NORATE;
                                dipent.players[0].power = MASTER;
                        }
		} else {
			if (!msg_header_done)
				msg_header(rfp);
			if (create) {
			    /* Bug 97, give a more useful error message on bad create */
			    fprintf(rfp, "To create a game, you need an initial '?' character, as in 'CREATE ?%s ...' \n", name);
			} else 
			    fprintf(rfp, "Invalid signon name '%s' given.\n", name);
			return E_WARN;
		}
	} else {
		if ((name[0] == '?')
		    && (variant != dipent.variant || (dipent.flags & F_VFLAGS) != (flags & F_VFLAGS))) 
		{
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' is not of the", dipent.name);
			for (i = NVARIANT; i < NVARIANT + NVAROPTS; i++) {
				if (flags & vvalue[i]) {
					fprintf(rfp, " %s", variants[i]);
				}
			}
			fprintf(rfp, " %s variant.", variants[variant]);

			if (dipent.variant != V_STANDARD || dipent.flags & F_VFLAGS) {
				fprintf(rfp, "  Use:\n\n    signon %c%s password", name[0], dipent.name);
				for (i = NVARIANT; i < NVARIANT + NVAROPTS; i++)
					if (dipent.flags & vvalue[i])
						fprintf(rfp, " %s", variants[i]);
				if (dipent.variant != V_STANDARD)
					fprintf(rfp, " %s", variants[dipent.variant]);
				fprintf(rfp, "\n\nif you want to sign onto this nonstandard variant.\n");
			}
			fputc('\n', rfp);
			return E_WARN;
		}
	}

	if (dipent.flags & F_GUNBOAT)
		xaddr = someone;
	else
		xaddr = raddr;

	if ((dipent.seq[0] == 'x' || dipent.phase[6] == 'X') && name[0] == '?' ) {
	    NewGameSignon(password, lmaster, luserid, lsiteid, llevel);
	} else {

	        if (name[0] == '?') {
		    /* 
		     * Player is trying to take over first abandoned power 
		     */
		    if (GetFirstAbandonedPower(&name[0]) == 0) {
			/* Unable to find one abandoned power */
			fprintf(rfp," No abandoned powers in this game for takeover.\n\n");
			return E_WARN;
		    }
		}    

		/*
		 * This is a normal signon attempt to an existing game.
		 */

		luserid = lsiteid = 0;
		n = power(name[0]);
		for (i = 0; i < dipent.n; i++) {
			if (name[0] == ADMINISTRATOR && 
				(!strcasecmp(raddr, GAMES_MASTER) ||
				!strcmp(SPECIAL_PW, password))) {
				n = MASTER;
				i = dipent.n;
				dipent.players[i].power = n;
				strcpy(dipent.players[i].password, password);
				strcpy(dipent.players[i].address, GAMES_MASTER);
				dipent.players[i].userid = 0;
				i_am_really_jk = 1;
			} else {
				i_am_really_jk = 0;
			}
			if (dipent.players[i].power != n)
				continue;

			if (!strcmp(dipent.players[RealPlayerIndex(i)].password, password) ||
			    ((n == OBSERVER) && !strcasecmp(dipent.players[i].address, raddr))) {
				player = i;
				signedon = 1;
				listflg = 0;
				if (!dipent.players[i].userid) {
					if (getuser(raddr, &luserid, &lsiteid, &llevel)) {
						dipent.players[i].userid = luserid;
						dipent.players[i].siteid = lsiteid;
					}
				}
				if (dipent.players[i].power == MASTER)
					xaddr = raddr;

				if (strcasecmp(dipent.players[i].address, raddr) ||
				    !dipent.players[i].userid) {
					if (!getuser(raddr, &luserid, &lsiteid, &llevel)) {
						if (!msg_header_done)
							msg_header(rfp);
						fprintf(rfp, "Note: Your address %s\n", raddr);
						fprintf(rfp, "has not been registered with the adjudicator.\n");
						fprintf(rfp, "Submit an 'IamAlso %s' command if\n",
							dipent.players[i].address);
						fprintf(rfp, "that is indeed the case or else use 'get form'.\n\n");
					}
					if (luserid != dipent.players[i].userid && dipent.players[i].userid) {
					        /* See if sender is part of player's email adress 
						   in case player has email1,email2 and sends from either */
						if (!(strstr(dipent.players[i].address, raddr)))
						{
						    fprintf(xfp, "Orders for %s in '%s' submitted by %s\n",
								powers[dipent.players[i].power], dipent.name, raddr);
						    fprintf(xfp, "instead of %s.\n", dipent.players[i].address);
						}
					}
				}
				/*
				 * check whether we have a miscreant who went
				 * into CD returning
				 */
				if((!(dipent.flags & F_NONMR)) && (dipent.players[i].status & SF_CD))
				{
					broad_signon = 1;
					sprintf(subjectline, "%s:%s - %s %s is no longer in CD",
						JUDGE_CODE, dipent.name, dipent.phase, powers[dipent.players[i].power]);
					fprintf(bfp, "%s has returned as %s in %s. %s is no longer in CD.",
						xaddr, powers[dipent.players[i].power], dipent.name, powers[dipent.players[i].power]);
					fprintf(mbfp, "%s has returned as %s in %s. %s is no longer in CD.",
						raddr, powers[dipent.players[i].power], dipent.name, powers[dipent.players[i].power]);
					pprintf(cfp, "%s has returned as %s in %s. %s is no longer in CD.",
						xaddr, powers[dipent.players[i].power], dipent.name, powers[dipent.players[i].power]);
				}
				break;
			}
			if (dipent.players[i].status & (SF_CD | SF_ABAND) ||
			    *dipent.players[i].address == '*') {

				long now, then;

				if (!getuser(raddr, &luserid, &lsiteid, &llevel)) {
					if (!msg_header_done)
						msg_header(rfp);
					fprintf(rfp, "Your address %s is not registered with the judge.\n", raddr);
					fprintf(rfp, "Use a 'get form' request for information on registering.\n");
					return E_WARN;
				}
				if (mail_access(i, luserid, lsiteid, llevel, &j))
					return E_WARN;

				if ((IsPlayerDead(i)) &&
				     (luserid != dipent.players[i].userid)) {
					fprintf(rfp, "Player is eliminated - takeover not allowed.\n");
					return E_WARN;
				}

				if (j >= 0)
					dipent.players[j].power = -1;	/* Remove him as observer */

				if (!(dipent.flags & F_QUIET)) 
				    broad_signon = 1;
				pcontrol++;

				for (jj = 0; jj < dipent.n; jj++) {
				    /* Loop round power and all controlled powers, setting them as taken over */
				    if (RealPlayerIndex(i) != RealPlayerIndex(jj)) continue;  /* Not for the same power */
				    /* If not a true duplex, only process for the same power */
				    if ((!IS_DUPLEX(dipent) || dipent.flags & F_INTIMATE) && i != jj) continue;
				    strcpy(baddr, dipent.players[jj].address);
				    strcpy(dipent.players[jj].address, raddr);
				    strcpy(dipent.players[jj].password, password);
				    n = dipent.players[jj].power;
				    if (dipent.players[jj].userid == luserid) {
				        /* Same player is returning, do not reset anything */
					if (!(dipent.flags & F_QUIET)) {
					    if (dipent.x2flags & X2F_SECRET) {
						if (!one_done) {
					            sprintf(subjectline, "%s:%s - %s Returning Player Signon",
					                JUDGE_CODE, dipent.name, dipent.phase);
						    fprintf(bfp, "%s has returned in game '%s'.\n",
						            xaddr, dipent.name);
						    fprintf(mbfp, "%s has returned in game '%s'.\n",
						            raddr, dipent.name);
					            pprintf(cfp, "%s%s has returned in game '%s'.\n", NowString(),
						            xaddr, dipent.name);
																		    					}
					    } else {
                                                if (!one_done) 
					            sprintf(subjectline, "%s:%s - %s Returning Player Signon: %c", 
					                    JUDGE_CODE, dipent.name, dipent.phase, dipent.pl[n]);
					        else {
						    sprintf(tmp1, "%c", dipent.pl[n]);
						    strcat(subjectline, tmp1);
						}

                                                fprintf(bfp, "%s has returned as %s in game '%s'.\n",
                                                        xaddr, powers[n], dipent.name);
                                                fprintf(mbfp, "%s has returned as %s in game '%s'.\n",
                                                        raddr, powers[n], dipent.name);
                                                pprintf(cfp, "%s%s has returned as %s in game '%s'.\n", NowString(),
                                                        xaddr, powers[n], dipent.name);
					    }
					}
				        player = RealPlayerIndex(jj);
                                        signedon = 1;
				        listflg = 0;
				        if (!msg_header_done)
                                            msg_header(rfp);
				    } else {

				        dipent.players[jj].userid = luserid;
				        dipent.players[jj].siteid = lsiteid;
				        dipent.players[jj].late_count = 0;  /* reset old late_count */
				        player = RealPlayerIndex(jj);
				        signedon = 1;
				        listflg = 0;
				        /* Reset the user's waiting flags */
				        dipent.players[player].status &= ~(SF_LATE | SF_REMIND);
				        dipent.players[player].status |= SF_WAIT;  
                		        if ((dipent.x2flags & X2F_APPROVAL) && (dipent.players[n].power <= WILD_PLAYER))
                    		            dipent.players[player].status |= SF_NOT_APPROVED;  /* New player needs approval */
				
				        if (!msg_header_done)
					    msg_header(rfp);
				        if (jj == player)
					    put_data(dipent.players[player].userid,tookover);
				        if (!one_done)
					    fprintf(rfp,"Wait has been set automatically for you - send 'set nowait' to clear.\n");
					fprintf(rfp, "Take over of abandoned %s allowed.\n", powers[n]);
				        time(&now);
				        if (dipent.deadline < now + 24 * 60 * 24 && dipent.phase[5] == 'M'
				            && !(dipent.flags & F_MODERATE) && !one_done) {
					    now = now + 48 * 60 * 60;
					    then = now + 168 * 60 * 60;
					    fprintf(rfp, "To give yourself time to communicate with the other ");
					    fprintf(rfp, "players, you may wish\n");
					    fprintf(rfp, "to submit commands to extend the current deadline ");
					    fprintf(rfp, "and/or grace periods.\n");
					    fprintf(rfp, "Suggested commands are:\n\n");
					    fprintf(rfp, "    set grace    %-6.6s 23:30\n", ctime(&then) + 4);
					    fprintf(rfp, "    set deadline %-6.6s 23:30\n\n", ctime(&now) + 4);
				        }
				        /* WAS mfprintf  1/94 BLR */
				        if (!(dipent.flags & F_QUIET)) {
				           if (dipent.x2flags & X2F_SECRET) {
					      if (!one_done) {
					          sprintf(subjectline, "%s:%s - %s New Player Signon", JUDGE_CODE, dipent.name, dipent.phase);
					          fprintf(bfp, "%s has taken over abandoned power(s)\n in game '%s'.\n",
					                  xaddr, dipent.name);
					          fprintf(mbfp, "%s has taken over abandoned power(s)\n in game '%s'.\n",
						          raddr, dipent.name);
                                                  pprintf(cfp, "%s%s has taken over abandoned power(s)\n in game '%s'.\n", NowString(),
						          xaddr, dipent.name);
					       }
				           } else {
					       if (!one_done)
				                   sprintf(subjectline, "%s:%s - %s New Player Signon: %c", JUDGE_CODE, dipent.name, dipent.phase, dipent.pl[n]);
					       else {
						   sprintf(tmp1, "%c", dipent.pl[n]);
					           strcat(subjectline, tmp1);
					       }						   

				               fprintf(bfp, "%s has taken over the abandoned\n%s in game '%s'.\n",
					               xaddr, powers[n], dipent.name);
				               fprintf(mbfp, "%s has taken over the abandoned\n%s in game '%s'.\n",
					               raddr, powers[n], dipent.name);
			    	               pprintf(cfp, "%s%s has taken over the abandoned\n%s in game '%s'.\n", NowString(),
					               xaddr, powers[n], dipent.name);
				           }
				       }
				    }
				    one_done++;
				}

				time(&now);
			
				for (j = 0; j < dipent.n; j++) {
                            	    if (dipent.players[j].power < 0 || i == j)
                                        continue;

				    if((dipent.players[j].status & (SF_CD | SF_ABAND)) && 
				       (atoi(dipent.seq) == 1 || dipent.players[j].centers)) {
				        one_abandoned++;
				    }
				}	
				
				if (!one_abandoned) { 
				    /* bump up takeover in accordance with phase settings */
				    /* but only if no other countries abandoned */
				    deadline( (sequence * ) NULL, 1);

					fprintf(rfp, "Deadline for '%s' advanced to %s.\n",
						dipent.name, ptime(&dipent.deadline));
					mfprintf(bfp, "Deadline for '%s' advanced to %s.\n",
						 dipent.name, ptime(&dipent.deadline));
					fprintf(cfp, "%sDeadline for '%s' advanced to %s.\n", NowString(),
						dipent.name, ptime(&dipent.deadline));
					fprintf(rfp, "Grace period for '%s' advanced to %s.\n\n",
						dipent.name, ptime(&dipent.grace));
					mfprintf(bfp, "Grace period for '%s' advanced to %s.\n\nAlso, a wait has been placed for all active players",
						 dipent.name, ptime(&dipent.grace));
					if (DIPENT_NO_PRESS)
					   mfprintf(bfp, " with moves to make");

					mfprintf(bfp, ".\nThis can be removed with the 'SET NOWAIT' command.\n\n");

					pprintf(cfp, "%sGrace period for '%s' advanced to %s.\n", NowString(),
						dipent.name, ptime(&dipent.grace));

			
				    for (j = 0; j < dipent.n; j++) {
                                        if (dipent.players[j].power < 0)
                                            continue;
                                        if (dipent.players[j].power == OBSERVER ||
					    dipent.players[j].power == MASTER )
                                            continue;
					
                                        if(!(IsPlayerDead(j))) {
					    if (dipent.players[RealPlayerIndex(j)].status & SF_MOVE ||
						!DIPENT_NO_PRESS)
				            dipent.players[RealPlayerIndex(j)].status |= SF_WAIT;
				        }
				    }
				}
				break;
			}
		}

		if (signedon) {
			long now;
			time(&now);
			ded[dipent.players[player].userid].ls = now;
		} else {
			if (n == OBSERVER && !(dipent.phase[6] == 'X' && dipent.seq[0] == 'x')) {
				j = dipent.seq[0] == 'x' ? '0' + dipent.np - dipent.seq[1] : 0;
				if (dipent.n < MAXPLAYERS - j) {
					if (!*raddr) {
						if (!msg_header_done)
							msg_header(rfp);
						fprintf(rfp, "Sorry, valid return address required.\n");
						return E_WARN;
					}
					dipent.players[dipent.n].power = n;
					dipent.players[dipent.n].status = 0;
					strcpy(dipent.players[dipent.n].address, raddr);
					strcpy(dipent.players[dipent.n].password, password);
					dipent.players[dipent.n].userid = luserid;
					dipent.players[dipent.n].siteid = lsiteid;
					dipent.players[dipent.n].units = 0;
					dipent.players[dipent.n].centers = 0;
					player = dipent.n;
					signedon = 1;
					listflg = 0;
					dipent.n++;

					if (!msg_header_done)
						msg_header(rfp);
					fprintf(rfp, "You are now an %s in game '%s'.\n", powers[n], dipent.name);
					/* WAS mfprintf  1/94 BLR */
					sprintf(subjectline, "%s:%s - %s New Player Signon: %c", JUDGE_CODE, dipent.name, dipent.phase, dipent.pl[n]);

					fprintf(bfp, "%s has signed on as an %s in game '%s'.\n",
					  xaddr, powers[n], dipent.name);
					fprintf(mbfp, "%s has signed on as an %s in game '%s'.\n",
					  raddr, powers[n], dipent.name);
					broad_signon = 1;
				} else {
					if (!msg_header_done)
						msg_header(rfp);
					fprintf(rfp, "Sorry, already have too many observers for '%s'.\n",
						dipent.name);
					return E_WARN;
				}
			} else {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Invalid password or power ID specified.\n");
				return E_WARN;
			}
		}

		if (dipent.seq[0] == 'x' && dipent.phase[6] != 'X') {
			signedon = -1;
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' hasn't started up yet.\n", dipent.name);
			if (!(dipent.x2flags & X2F_SECRET) || (n == MASTER))
				fprintf(rfp, "You'll be notified when %d more people sign on.\n",
				   dipent.no_of_players - (dipent.seq[1] - '0'));
			if (n== MASTER) {
			        /* It is the master, show him the current preferences */
				ShowPreferences(rfp);
			}
			return 0;
		}
		/* Show any requested absences */
		if  (dipent.seq[0] != 'x' && dipent.phase[6] != 'X') {
			display_absence(n, rfp);
		}

		/* Don't allow signon on unstarted but terminated game */
		if (dipent.phase[6]=='X' && dipent.seq[0] == 'x') {
                    fprintf(rfp,"Game '%s' is marked for termination: no signons allowed.\n", &name[1]);
                    return E_WARN;
                }
		if (dipent.phase[6] == 'X') {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' has been terminated.\n", dipent.name);
			fprintf(rfp, "Use the 'resume' command to start it back up.\n\n");
		}
		sprintf(Tfile, "%s%s/P%s", GAME_DIR, dipent.name, dipent.seq);
		if ((pfp = fopen(Tfile, "a+")) == NULL) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Error opening %s to write future orders.\n", Tfile);
			return E_FATAL;
		}
		sprintf(Tfile, "%s%s/T%s", GAME_DIR, dipent.name, dipent.seq);
		if ((ofp = fopen(Tfile, "w")) == NULL) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
			return E_FATAL;
		}
		sprintf(Mfile, "%s%s/M%s", GAME_DIR, dipent.name, dipent.seq);
		if ((tfp = fopen(Mfile, "r")) != NULL) {
			while (fgets(line, sizeof(line), tfp)) {
				if (!strcmp(line, "X-marker\n"))
					*line = 'Y';
				fputs(line, ofp);
			}
			fclose(tfp);
		}
		fputs("X-marker\n", ofp);
	}
	return 0;
}

/***************************************************************************/

int mail_access(int ignore, int luserid, int lsiteid, int llevel, int *idx)
{

/*
 *  Check to see if this person is permitted to access this game.
 *  Return value 0 = okay, 1 = already on, -1 = no way.  
 *  Observe is set to the index of an observation point if one exists.
 */

	int i, s1, s2, ret;
	float ont_rat, res_rat;
	char *temp;

	ret = 0;
	*idx = -1;

/* Check to see if this player is allowed access to this game. */
	if (!is_allowed(GAME_PLAYER)) {
		/* Find out who the master is. */
		temp = NULL;
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power == MASTER)
				temp = dipent.players[i].address;
		}
		fprintf(rfp, "The master has restricted access to this game.  You are not allowed to sign on.\n");
		if (temp)
			fprintf(rfp, "Please contact the master (%s) if you want to dispute this decision.\n", temp);
		return -1;
	}
	for (i = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if (i == ignore)
			continue;
		if ((dipent.players[i].userid == luserid &&
		     *dipent.players[i].address != '*') ||
		    !strcasecmp(dipent.players[i].address, raddr)) {
			if (dipent.players[i].power == OBSERVER) {
				*idx = i;
			} else {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "You are already signed up to play %s in game '%s'.\n\n",
					powers[dipent.players[i].power], dipent.name);
				*idx = i;
				return 1;
			}
		}
		if (dipent.access == A_DIFF && dipent.players[i].power <= WILD_PLAYER
		    && *dipent.players[i].address != '*') {
			s1 = lsiteid;
			s2 = dipent.players[i].siteid;
			if (!(s1 % 100) || !(s2 % 100)) {
				s1 /= 100;
				s2 /= 100;
			}
			if (s1 == s2) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, %s is already signed up to play\n",
					dipent.flags & F_GUNBOAT ? someone : dipent.players[i].address);
				fprintf(rfp, "%s in game '%s'.  ", powers[dipent.players[i].power],
					dipent.name);
				fprintf(rfp, "Only one person from each area, please.\n");
				ret = -1;
			}
		}
		if (dipent.access == A_SAME && dipent.players[i].power <= WILD_PLAYER
		    && *dipent.players[i].address != '*') {
			s1 = lsiteid;
			s2 = dipent.players[i].siteid;
			if (!(s1 % 100) || !(s2 % 100)) {
				s1 /= 100;
				s2 /= 100;
			}
			if (s1 != s2) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, %s is already signed up to play\n",
					dipent.flags & F_GUNBOAT ? someone : dipent.players[i].address);
				fprintf(rfp, "%s in game '%s'.\n\n", powers[dipent.players[i].power],
					dipent.name);
				fprintf(rfp, "Game '%s' is restricted to players in the ", dipent.name);
				fprintf(rfp, "same area.\n");
				return -1;
			}
		}
	}

	if (dipent.level == L_NOVICE && llevel != L_NOVICE) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Novice-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(llevel)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_INTERMEDIATE && llevel != L_INTERMEDIATE) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Intermediate-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(llevel)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_EXPERT && llevel != L_EXPERT) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Expert-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(llevel)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_ADVANCED && (!llevel || llevel == L_NOVICE)) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Advanced-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(llevel)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_AMATEUR && (!llevel ||
			       llevel == L_EXPERT || llevel == L_WINNER)) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Amateur-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(llevel)));
		free(temp);
		return -1;
	}
	if (ded[luserid].r < dipent.dedicate) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' requires a dedication of at least %d and you\n",
			dipent.name, dipent.dedicate);
		fprintf(rfp, "are currently rated at %d.  Use 'get dedsystems' for more info.\n",
			ded[luserid].r);
		return -1;
	}
	if(get_data(luserid,total) == 0)
	{
		ont_rat = -1;
		/* We're doing this to avoid divide by 0 errors. */
	}
	else
	{
		ont_rat = 1.0 * get_data(luserid,ontime) / get_data(luserid,total);
	}
	if(get_data(luserid,started) == 0 && get_data(luserid,tookover) == 0) 
	{
		res_rat = -1;
	}
	else
	{
		res_rat = 1.0 * get_data(luserid,resigned) / (get_data(luserid,started) + get_data(luserid,tookover));
	}
        /* Fix res_rat if over limit */
        if (res_rat > 1.0 ) res_rat = 1.0;

	if(ont_rat < dipent.orded && ont_rat >= 0)
	{
		if(!msg_header_done)
			msg_header(rfp);
		fprintf(rfp,"Sorry, game '%s' requires an ontime ratio of at least %.3f.\n",
			dipent.name,dipent.orded);
		fprintf(rfp,"You are currently rated %.3f\n",ont_rat);
		return -1;
	}
	if(res_rat > dipent.rrded)
	{
		if(!msg_header_done)
			msg_header(rfp);
		fprintf(rfp,"Sorry, game '%s' requires a maximum CD ratio of %.3f.\n", 
			dipent.name,dipent.rrded);
		fprintf(rfp,"You are currently rated %.3f.\n",res_rat);
		return -1;
	}
	return (ret);
}

/***************************************************************************/

void mail_igame(void)
{

/*
 *  Igame: Initialize a game.
 *
 *  The game file is created and messages are sent to each player.
 */

	char line[150],
	availpowers[WILD_PLAYER],	/* List of powers not yet randomly assigned */
	*pref[WILD_PLAYER],		/* Adjusted pref lists for players */
	assdpowers[WILD_PLAYER] = "",	/* List of powers that have been randomly assigned */
	*cptr;				/* Character pointer */
	int i, j, k, n;
	int unassigned;		/* Number of powers not yet randomly assigned */
	int randoms = 0;		/* Number of powers to be assigned randomly */
	int wp[WILD_PLAYER];	/* Whether this is a power ordinal in this variant */
	int mx[WILD_PLAYER];	/* Index to which power was assigned to this player */
	int wv[WILD_PLAYER][WILD_PLAYER];	/* Array derived from player preferences */
	int wvcopy[WILD_PLAYER][WILD_PLAYER];	/* Copy of wv */
	int assigned;		/* Number of powers assigned so far */
	int number_of_players;	/* Number of players in the game */
	int player_list[MAXPLAYERS];	/* The list of players who want powers */
	int player_to_remove;	/* Index of player to remove from the list */
	Player temp_player;	/* Temporary player record for swapping players */
	long now;
	FILE *fp, *dfp;
	sequence seq;
	int p;
	int value;

#define UNAVAILABLE_PREFERENCE INT_MAX

	/* Preference value for an already taken power */

	for (i = 0; i < WILD_PLAYER; i++)
		for (j = 0; j < WILD_PLAYER; j++)
			wv[i][j] = 0;

	j = 0;

	for (i = 1; i < WILD_PLAYER; i++) {
		if (dipent.pl[i] == 'x') {
			wp[i] = 0;
		} else {
			wp[i] = 1;
			availpowers[j++] = dipent.pl[i];
		}
	}

	availpowers[j] = '\0';

	unassigned = j;

	for (i = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if ((j = dipent.players[i].power) < WILD_PLAYER) {
			wp[j] = 0;
		}
	}

	for (i = 0; i < dipent.n; i++) {
		if (dipent.players[i].power == WILD_PLAYER) {
			pref[i] = (char *) malloc(WILD_PLAYER);

			if (dipent.x2flags & X2F_PREFRANDONLY) {
				*pref[i] = '*';
				randoms++;
			} else {
				if (dipent.x2flags & X2F_PREFRANDALLOW) {
					if (*dipent.players[i].pref == '*') {
						randoms++;
					}
					strcpy(pref[i], dipent.players[i].pref);
				} else {
					if (*dipent.players[i].pref == '*') {
						*pref[i] = '\0';
					} else {
						strcpy(pref[i], dipent.players[i].pref);
					}
				}
			}
		}
	}

	for (i = 0; i < randoms; i++) {
		j = rand() % unassigned--;

		assdpowers[i] = availpowers[j];
		memmove(&availpowers[j], &availpowers[j+1],WILD_PLAYER - j);
	}

	j = 0;

	for (i = n = 0; i < dipent.n; i++) {
		if (dipent.players[i].power == WILD_PLAYER) {
			if (*pref[i] == '*') {
				*pref[i]++ = assdpowers[j++];
				*pref[i]-- = '\0';
			} else {
				for (k = 0; k < randoms; k++) {
					cptr = strchr(pref[i], assdpowers[k]);

					if (cptr != NULL) {
						memmove(cptr, cptr + 1, WILD_PLAYER - (cptr + 1 - pref[i]));
					}
				}
			}

			chkpref(pref[i], wp, wv[n++]);
		}
	}

	if (n != dipent.np) {
		/* Copy the wv structure, as assignment() trashes it.
		 * Only necessary if more powers than players
		 */

		for (i = 0; i < n; i++) {
			for (j = 0; j < dipent.np; j++) {
				wvcopy[i][j] = wv[i][j];
			}
		}
	}
	assigned = n;

	number_of_players = n;

	assignment(wv, n, dipent.np, mx);

/* Free memory allocated for prefs */

	for (i = 0; i < dipent.n; i++) {
		if (dipent.players[i].power == WILD_PLAYER) {
			free(pref[i]);
		}
	}

	for (i = k = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if (dipent.players[i].power == WILD_PLAYER) {
			player_list[k] = i;
			for (j = 1, n = mx[k++]; j < WILD_PLAYER; j++) {
				if (wp[j] && !n--)
					break;
			}
			if (j == WILD_PLAYER) {
				fprintf(xfp, "Not enough free powers for game '%s'.\n", dipent.name);
				fflush(xfp);
				fprintf(stderr, "Unable to assign powers properly, bailing out!\n");
				bailout(E_FATAL);
			}
			dipent.players[i].power = j;
		}
		if (dipent.players[i].power < WILD_PLAYER)
		    /* If not a blankboard game, or already in a build phase
		       players can move */
		    if (!(dipent.xflags & XF_BLANKBOARD) || dipent.phase[5] == 'B')
			    dipent.players[i].status |= SF_MOVE;
	}

	/* While we haven't assigned all the players. Only happens if a
	 * "set players" command has been entered.
	 */

	value = dipent.powers;

	while (assigned < value && number_of_players != 0) {
		/* Blank out all the taken preferences in the copy of the
		 * preference array.
		 */

		for (i = 0; i < number_of_players; i++) {
			for (j = 0; j < number_of_players; j++) {
				wvcopy[i][mx[j]] = UNAVAILABLE_PREFERENCE;
			}
		}

		/* While there are more players than spots, remove a
		 * player at random.
		 */

		while (assigned + number_of_players > value) {
			player_to_remove = ((unsigned int) (rand() / 23)) % number_of_players;

			number_of_players--;

			player_list[player_to_remove] = player_list[number_of_players];

			for (i = 0; i < dipent.np; i++) {
				wvcopy[player_to_remove][i] = wvcopy[number_of_players][i];
			}
		}

		/* Copy the copy back into the original array */

		for (i = 0; i < number_of_players; i++) {
			for (j = 0; j < dipent.np; j++) {
				wv[i][j] = wvcopy[i][j];
			}
		}

		/* Repeat the assignment operation. This time, new player
		 * records will have to be created for each power assigned
		 */

		assignment(wv, number_of_players, dipent.np, mx);

		for (k = 0; k < number_of_players; k++) {
			i = player_list[k];

			for (j = 1, n = mx[k]; j < WILD_PLAYER; j++) {
				if (wp[j] && !n--)
					break;
			}
			if (j < WILD_PLAYER) {
				if (dipent.flags & F_INTIMATE) {                                                                   /* in intimate, other powers are autonomous */
				    dipent.players[dipent.n].controlling_power = AUTONOMOUS;
				    strcpy(dipent.players[dipent.n].address, NULL_EMAIL);
				    strcpy(dipent.players[dipent.n].password, "-none-");
				} else {
				    memcpy(&dipent.players[dipent.n], &dipent.players[i], sizeof(Player));
				    dipent.players[dipent.n].controlling_power = dipent.players[i].power;
				    /*strcpy(dipent.players[dipent.n].address, NULL_EMAIL);*/
				    strcpy(dipent.players[dipent.n].password, "-password-");
				}
				dipent.players[dipent.n].power = j;
				dipent.n++;
			}
		}
		assigned += number_of_players;
	}

	/* Sort the players into order by country letter */

	for (i = 0; i < dipent.n - 1; i++) {
		k = i;

		for (j = i; j < dipent.n; j++) {
			if (dipent.players[j].power == MASTER) {
				k = j;
				break;
			}
			if ((dipent.players[k].power < 0)
			    || (dipent.players[k].power >= WILD_PLAYER)) {
				k = j;
			}
			if ((dipent.players[j].power >= 0)
			    && (dipent.players[j].power < WILD_PLAYER)
			    && (dipent.pl[dipent.players[j].power] < dipent.pl[dipent.players[k].power])) {
				k = j;
			}
		}

		if (k != i) {
			memcpy(&temp_player, &dipent.players[i], sizeof(Player));
			memcpy(&dipent.players[i], &dipent.players[k], sizeof(Player));
			memcpy(&dipent.players[k], &temp_player, sizeof(Player));

			/* If we have just moved the record referenced by the variable "player",
			 * then adjust player to the new location of the record */

			if ( i == player )
			{
				player = k;
			}
			else if ( k == player )
			{
				player = i;
			}
		}
	}

	/*
	 * Set the rest of the master file information.
	 */

	strcpy(dipent.seq, "001");
	strcpy(dipent.phase, sphase[dipent.variant]);
	if (dipent.flags & F_INTIMATE) {
	    dipent.phase[5] = 'A';
	}
	seq.clock = dipent.movement.clock;
	seq.mint = dipent.movement.mint;
	seq.next = dipent.movement.next * ((DIPENT_NO_PRESS) ? 1 : 2);
	seq.grace = dipent.movement.grace;
	seq.delay = dipent.movement.delay * ((DIPENT_NO_PRESS) ? 1 : 2);
	strcpy(seq.days, dipent.movement.days);
	deadline(&seq, 1);

	/*
	 * Create a game file.
	 */

	sprintf(line, "%s%s", GAME_DIR, dipent.name);
	mkdir(line, 0777);
	sprintf(line, "%s%s/G%s", GAME_DIR, dipent.name, dipent.seq);
	if ((ofp = fopen(line, "w")) == NULL) {
		fprintf(stderr, "igame: Error opening game file %s.\n", line);
		bailout(E_FATAL);
	}
	if ((tfp = OpenDataFile(line, "seed")) == NULL){
		bailout(E_FATAL);
	}
	while (fgets(line, sizeof(line), tfp))
	{
	    i = power(line[0]);
	    p = FindPower(i);
	    if (!(dipent.xflags & XF_BLANKBOARD) || line[1] != ':')
	        if (dipent.powers == dipent.np || line[1] != ':' ||
		    p < dipent.n)
		    fputs(line, ofp);
	}

	if (dipent.x2flags & X2F_NEUTRALS)  {
	   /* We've got Neutrals so set them up */
	    for (p = 1; p <= npr; p++) {
		if (pr[p].type == 'x' ||
		    FindPower(power(pr[p].type)) >= dipent.n) {
		    sprintf(line, "=: A %s\n", pr[p].name);
		    fputs(line, ofp);
		}
	    }
	}

	
	fclose(tfp);
	fclose(ofp);

	/*
	 * Write out a report file for each player.
	 */

	pcontrol++;
	pprintf(cfp, "%sGame '%s' now has a quorum and is starting:\n", NowString(), dipent.name);
	dipent.players[dipent.n].power = OBSERVER;
	for (i = 0; i < dipent.n + 1; i++) {
		if (dipent.players[i].power < 0)
			continue;
		if (IS_DUPLEX(dipent) && dipent.players[i].controlling_power != 0)
			continue;  /* Don't send mail message to controlled powers */

		if ((dipent.flags & F_INTIMATE) && 
		     dipent.players[i].controlling_power != AUTONOMOUS && 
		     dipent.players[i].power != MASTER) 
		    dipent.players[i].status |= SF_WAIT; /* Always set a wait for players in intimate */

		if ((ofp = fopen("dip.temp", "w")) == NULL) {
			fprintf(stderr, "igame: Error opening second temporary file.\n");
			bailout(E_FATAL);
		}
		msg_header(ofp);
		PowerStartingList(ofp, i);
		
		/* Increment the started value for plyrdata. But only if the
		   game is rated and we're not an observer. */
		if((!(dipent.flags & F_NORATE)) && dipent.players[i].power != OBSERVER) {
			if(put_data(dipent.players[i].userid,started) < 0)
			{
				fprintf(stderr,"Unable to log plyrdata for %d.\n",dipent.players[i].userid);
			}
		}
		fprintf(ofp, "\n\nThe following players are in this game:\n");
		for (j = 0; j < dipent.n; j++) {
			if (dipent.players[j].power < 0)
				continue;

			fprintf(ofp, "     %s:", powers[k = dipent.players[j].power]);
			for (k = strlen(powers[k]); k < LPOWER + 1; k++)
				putc(' ', ofp);
			fprintf(ofp, "%s\n", (dipent.flags & F_GUNBOAT) && j != i
				&& dipent.players[j].power != MASTER
			   && dipent.players[i].power != MASTER ? SomeoneText(j)
				: dipent.players[j].address);
		}
		if (dipent.xflags & XF_BLANKBOARD) {
			if ((tfp = fopen("data/report.blank","r")) == NULL) {
                                bailout(E_FATAL);
                        } 
		} else {
			if ((tfp = OpenDataFile(line, "report")) == NULL) {
				bailout(E_FATAL);
			}
		}
		while (fgets(line, sizeof(line), tfp))
			fputs(line, ofp);
		fprintf(ofp, "The deadline for the first orders is %s.\n",
			ptime(&dipent.deadline));
		fclose(ofp);
		msg_header_done = 0;
		fclose(tfp);

		if (i != dipent.n) {
			sprintf(line, "%s dip.temp '%s:%s - %s Game Starting' '%s'",
				SMAIL_CMD, JUDGE_CODE, dipent.name, dipent.phase, dipent.players[i].address);

			if (execute(line)) {
				fprintf(stderr, "igame: Error sending mail to %s.\n",
					dipent.players[i].address);
				bailout(E_FATAL);
			}
			pprintf(cfp, "     %s:", powers[k = dipent.players[i].power]);
			for (k = strlen(powers[k]); k < LPOWER + 1; k++)
				putc(' ', cfp);
			pprintf(cfp, "%s\n", (dipent.flags & F_GUNBOAT)
			   && dipent.players[i].power != MASTER ? SomeoneText(i)
				: dipent.players[i].address);
		}
	}
	sprintf(line, "%s:%s - %s Game Starting", JUDGE_CODE, dipent.name, dipent.phase);
	archive("dip.temp", line);


/*
 * Record start date for the summary
 */

	sprintf(line, "%s%s/start", GAME_DIR, dipent.name);
	if ((dfp = fopen(line, "w")) == NULL) {
		fprintf(log_fp, "igame: Error opening start file.\n");
		bailout(E_FATAL);
	}
	time(&now);
	fprintf(dfp, "Game Started: %s", ctime(&now));
	fclose(dfp);

	/*  
	 * If game is standard, it gets a Boardman Number, if not
	 * it gets a Miller Number.  It also may get an EP Number
	 * Alert Custodians to game start 
	 * 
	 * ofp contains information for the Custodians 
	 */

	if ((ofp = fopen("dip.temp", "w")) == NULL) {
		fprintf(stderr, "igame: Error opening second temporary file.\n");
		bailout(E_FATAL);
	}
	msg_header(ofp);
	fprintf(ofp, "Press:   ");
	/* TODO: Redefine this for new press settings, where can have observer 
	   but no player press */
	switch (dipent.flags & (F_NOWHITE | F_GREY | F_DEFWHITE)) {
	case (F_NOWHITE | F_GREY | F_DEFWHITE):
		fprintf(ofp, "Undefined");
		break;
	case (F_NOWHITE | F_GREY):
		fprintf(ofp, "Grey");
		break;
	case (F_NOWHITE | F_DEFWHITE):
		fprintf(ofp, "Undefined");
		break;
	case (F_NOWHITE):
		fprintf(ofp, "None");
		break;
	case (F_GREY | F_DEFWHITE):
		fprintf(ofp, "White/Grey");
		break;
	case (F_GREY):
		fprintf(ofp, "Grey/White");
		break;
	case (F_DEFWHITE):
		fprintf(ofp, "Undefined");
		break;
	case (0):
		fprintf(ofp, "White");
		break;
	}

	switch (dipent.flags & (F_OBWHITE | F_OBNONE)) {
	case (F_OBWHITE | F_OBNONE):
		fprintf(ofp, ", Undefined");
		break;
	case (F_OBNONE):
		fprintf(ofp, ", No observers");
		break;
	case (F_OBWHITE):
		fprintf(ofp, ", White observers");
		break;
	case (0):
		break;
	}

	if (dipent.flags & F_NOPARTIAL)
		fprintf(ofp, ", No Partial");
	else {
		switch (dipent.flags & (F_FAKE | F_DEFFAKE)) {
		case (F_FAKE):
			fprintf(ofp, ", Partial may be Faked");
			break;
		case (F_DEFFAKE):
			fprintf(ofp, ", Partial Fakes Broadcast");
			break;
		case (0):
			fprintf(ofp, ", No Fake Broadcast");
			break;
		case (F_FAKE | F_DEFFAKE):
			fprintf(ofp, ", Partial Fakes Broadcast by default");
			break;
		}
	}
	fprintf(ofp, ".\nWinning Centers: %d.\n", dipent.vp);
	if (dipent.flags & F_NOLIST)
		fprintf(ofp, "\nThe game is unlisted.\n");
	fprintf(ofp, "\nGame '%s' now has a quorum and is starting:\n", dipent.name);
	for (i = 0; i < dipent.n; i++) {
		fprintf(ofp, "     %s:", powers[k = dipent.players[i].power]);
		for (k = strlen(powers[k]); k < LPOWER + 1; k++)
			putc(' ', ofp);
		fprintf(ofp, "%s\n", (dipent.flags & F_GUNBOAT)
			&& dipent.players[i].power != MASTER ? SomeoneText(i)
			: dipent.players[i].address);
	}

	sprintf(line, "%s%s/info", GAME_DIR, dipent.name);
	if ((fp = fopen(line, "r"))) {
		fputc('\n', ofp);
		while (fgets(line, sizeof(line), fp))
			fputs(line, ofp);
		fclose(fp);
	}
	fclose(ofp);
	msg_header_done = 0;

	InformCustodians(dipent.name,
	                 "%s '%s: Game %s starting'",
	                  dipent.variant,
	                  dipent.flags & F_GUNBOAT);

	/*
	 *  If this is a Machiavelli game we need to process the first build
	 *  phase now to get the variable income established.
	 */

	if (dipent.flags & F_MACH) {
		time(&now);
		for (i = 0; i < dipent.n; i++)
			dipent.players[i].status = 0;
		dipent.process = now;
		dipent.start = now - 24 * 60 * 60;
	}
}

/******************************************************************************/

/*
 *  Check a preference list.  Powers are specified as single characters.
 *  Equal weighting is applied for powers within square brackets.
 */

int chkpref(char *s, int wp[WILD_PLAYER], int wv[WILD_PLAYER])
{

	int i, j, n, taken[WILD_PLAYER];
	char *t = NULL;
	float v = 0.0;


	memset(taken, 0, sizeof(taken));

	i = 1;
	while (*s && *s != '\n') {
		if (*s == '[' || *s == '(') {
			for (t = ++s, n = i - 1; *s && *s != ']' && *s != ')'; s++, n++);
			v = (n + i) / 2.0;
		}
		if (*s != ']' && *s != ')') {
			v = i;
			t = s;
		}
		if (islower(*t))
			*t = toupper(*t);
		for (j = 1, n = 0; j < WILD_PLAYER; j++) {
			if (dipent.pl[j] == *t) {
				if (wv && wp && wp[j])
					*(wv + n) = v * 10000 + rand() % 1000;
				break;
			}
			if (wp && wp[j])
				n++;
		}

		if (j == WILD_PLAYER) {
			fprintf(rfp, "Invalid character '%c' in preference list.\n", *t);
			return 1;
		}
		if (taken[j]++) {
			fprintf(rfp, "Duplicate character '%c' in preference list.\n", *t);
			return 1;
		}
		t++;
		if (*t == ']' || *t == ')' || (*s != ']' && *s != ')'))
			s++;
		i++;

	}

	if (wv && i != dipent.np + 1) {
		v = (dipent.np + i) / 2.0;
		for (n = 0; n < WILD_PLAYER; n++)
			if (!*(wv + n))
				*(wv + n) = v * 10000 + rand() % 1000;
	}
	return 0;

}

int NewGameSignon(char *password, int lmaster, int luserid, int lsiteid, int llevel)
{

	int i, n;


		/*
		 * Reject signon if game is marked to be terminated.
		 */
                if (dipent.phase[6]=='X') {
		    fprintf(rfp,"Game '%s' is marked for termination: no signons allowed.\n", &name[1]);
		    return E_WARN;
                }
		/*
                 * Sign this fellow up.
                 */
		else if (dipent.seq[0] != 'x') {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' is already in progress.\n", &name[1]);
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status & (SF_ABAND | SF_CD)
				     || *dipent.players[i].address == '*')
				    && dipent.players[i].centers != 0) {
					fprintf(rfp,
						"\nor  'signon %c%s password ...' if you want to take over %s",
						dipent.pl[dipent.players[i].power], dipent.name,
					powers[dipent.players[i].power]);
				}
			}
			fprintf(rfp, "\nor 'observe %s password' if you just want to watch.\n",
				dipent.name);
			return E_WARN;
		}
		if ((i = mail_access(-1, luserid, lsiteid, llevel, &n)) < 0) {
			return E_WARN;
		}
		if (i > 0) {
			player = n;
			signedon = -1;
			listflg = 0;
			if (!msg_header_done)
				msg_header(rfp);
			if (*dipent.players[player].pref) {
				fprintf(rfp, "Preference list is currently set to %s.\n",
					dipent.players[player].pref);
			} else {
				fprintf(rfp, "No preference list has been established.\n");
			}
			return 0;
		}
		if(dipent.no_of_players - (dipent.seq[1] - '0') <= 0 )  {
                    /* Game is already full, waiting manual start?, so reject signon */
                    fprintf(rfp,"Game '%s' is already full: no new players allowed.\n", &name[1]);
                    return E_WARN;
                }

		if (!*raddr) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Sorry, valid return address required.\n");
			return E_WARN;
		}
		if (n < 0) {
			if (dipent.n + 1 >= MAXPLAYERS) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Too many observers are signed up for game '%s'.\n",
					dipent.name);
				return E_FATAL;
			}
			n = dipent.n++;
		}
		// Make the first player the master
		if ( lmaster )
		{
			name[0] = 'm';
		}

		dipent.players[n].power = power(name[0]);
		dipent.players[n].status = 0;
		dipent.players[n].units = 0;
		dipent.players[n].centers = 0;
		dipent.players[n].userid = luserid;
		dipent.players[n].siteid = lsiteid;
		dipent.players[n].late_count = 0; /* initialise late count */
		if ((dipent.x2flags & X2F_APPROVAL) && (dipent.players[n].power <= WILD_PLAYER))
		    dipent.players[n].status |= SF_NOT_APPROVED;  /* New player needs approval */
		*dipent.players[n].pref = '\0';
		strcpy(dipent.players[n].address, raddr);
		strcpy(dipent.players[n].password, password);
		player = n;
		signedon = -1;
		listflg = 0;
		time(&dipent.process);
		dipent.process += 168 * 60 * 60;

		/*
		 * Increment the number of people signed up and start the game if ready.
		 */

		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "You've been selected to play %s in game '%s'.\n",
			powers[power(name[0])], dipent.name);

		if (name[0] == '?' && ++dipent.seq[1] == '0' + dipent.no_of_players) {
		 /* Game is startable, lets see if starting is allowed */
		    if (!(dipent.xflags & XF_MANUALSTART) ) {
			strcpy(dipent.seq, "001");
			starting++;
		    } else {
			/* Tell all players waiting for master to start game */
		        fprintf(bfp, "%s has signed up to play %s in game '%s'.\n", xaddr,
                                powers[power(name[0])], dipent.name);
                        fprintf(mbfp, "%s has signed up to play %s in game '%s'.\n", raddr,
                                powers[power(name[0])], dipent.name);
			fprintf(rfp,"Game '%s' is now ready for Master to start the game.\n", dipent.name);
			mfprintf(bfp, "Game '%s' is now ready for Master to start the game.\n", dipent.name);

			sprintf(subjectline, "%s:%s - %s Ready to Start", JUDGE_CODE, dipent.name, dipent.phase);

			broad_signon = 1;
		        if (dipent.n != 1) {
                                pprintf(cfp, "%s%s has signed up to play %s in game '%s'.\n",
                                        NowString(), xaddr,
                                    powers[power(name[0])], dipent.name);
                                pprintf(cfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s"); pprintf(cfp, ".\n\n");
                                pcontrol++;
			}
		    }
		} else {
			if (dipent.seq[0] == 'x' && dipent.phase[6] != 'X' ) {
				n = dipent.no_of_players - (dipent.seq[1] - '0');

				if (!(dipent.x2flags & X2F_SECRET)) {
					fprintf(rfp, "You'll be notified when %d more player%s sign%s on.\n",
					n, n == 1 ? "" : "s", n == 1 ? "s" : "");
				}

				n = dipent.seq[1] - '0';
			} else {
				n = dipent.np;
			}
			if ( !lmaster )
			{
				sprintf(subjectline, "%s:%s - %s New Player Signon: #%d", JUDGE_CODE, dipent.name, dipent.phase, n);

				fprintf(bfp, "%s has signed up to play %s in game '%s'.\n", xaddr,
					powers[power(name[0])], dipent.name);
				fprintf(mbfp, "%s has signed up to play %s in game '%s'.\n", raddr,
					powers[power(name[0])], dipent.name);
				mfprintf(bfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s");
				mfprintf(bfp, ".\n\n");
				broad_signon = 1;
			}

			if (!(dipent.x2flags & X2F_SECRET)) {
				fprintf(rfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s");
			}
			fprintf(rfp, ".\n\n");
			if (dipent.n != 1) {
				pprintf(cfp, "%s%s has signed up to play %s in game '%s'.\n",
					NowString(), xaddr,
				    powers[power(name[0])], dipent.name);
				pprintf(cfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s");
				pprintf(cfp, ".\n\n");
				pcontrol++;
			}
		}
		return 0;
}

