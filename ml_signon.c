/*
 * $Log$
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

#include "dip.h"
#include "mail.h"
#include "ml_signon.h"
#include ".magic.h"
#include "variant.h"
#include "functions.h"

char *generic_names[] =
{"b*", "c*", "d*", "e*", "f*", "g*",
 "h*", "i*", "j*", "k*", "l*", "m*",
 "n*", "o*", "p*", "q*", "r*", "s*",
 "t*", "u*", "v*", "w*", "x*", "y*", "z*",
 "a*"};

#define ADMINISTRATOR	'@'

/***************************************************************************/

int mail_signon(char *s)
{

	char password[20];
	int i, j, n, found;
	int userid, siteid, level, variant = V_STANDARD, flags = 0;
	char *t, line[150];

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
		if (!isalnum(*s) && t > name) {
			fprintf(rfp, "Game names must only contain alphanumeric characters.\n");
			fprintf(rfp, "A '%c' was found.\n", *s);
			return E_WARN;
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
		/* TODO: add in a check for non alphanumerics here */
		if (!isalnum(*s) && t > name) {
			fprintf(rfp, "Passwords must only contain alphanumeric characters.  Found a '%c'\n", *s);
			return E_WARN;
		}
		*t++ = tolower(*s++);
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
			s = lookfor(s, variants, NVARIANT + NVAROPTS, &i);
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
		if (!getuser(raddr, &userid, &siteid, &level)) {
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

	if (!name[1] && dipent.seq[strlen(dipent.seq) - 1] != 'x') {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp,
			"Sorry, there are no generic games forming.  Use the 'list' command\n");
		fprintf(rfp, "to get a list of the names of games that are forming.\n");
		return E_WARN;
	}
	if (!found) {
		if (name[0] == '?') {
			struct stat sbuf;

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
			lookfor(&name[1], generic_names, nentry(generic_names), &i);
			if (i) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, '%s' is a reserved name.\n", &name[1]);
				return E_WARN;
			}
			name[0] = 'D';
			if (stat(name, &sbuf) == 0) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Sorry, '%s' is a reserved name.\n", &name[1]);
				return E_WARN;
			}
			name[0] = '?';

			if (ded[userid].r < atoi(config("CREATE_DEDICATION"))) {
				fprintf(rfp, "You must have a minimum dedication of %i to create games\non this judge.\n", atoi(config("CREATE_DEDICATION")));
				return E_WARN;
			}
			newdipent(&name[1], variant);
			dipent.variant = variant;
			dipent.flags |= flags;
			if (dipent.flags & F_GUNBOAT)
				dipent.flags |= F_NOPARTIAL;
		} else {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Invalid signon name '%s' given.\n", name);
			return E_WARN;
		}
	} else {
		if ((name[0] == '?')
		    && (variant != dipent.variant || (dipent.flags & F_VFLAGS) != flags)) {
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

	if (name[0] == '?') {

		/*
		 * Sign this fellow up.
		 */

		if (dipent.seq[0] != 'x') {
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
		if ((i = mail_access(-1, userid, siteid, level, &n)) < 0) {
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
		dipent.players[n].power = power(name[0]);
		dipent.players[n].status = 0;
		dipent.players[n].units = 0;
		dipent.players[n].centers = 0;
		dipent.players[n].userid = userid;
		dipent.players[n].siteid = siteid;
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
			if (dipent.seq[2] == 'x')
				generic++;
			strcpy(dipent.seq, "001");
			starting++;
		} else {
			if (dipent.seq[0] == 'x') {
				n = dipent.no_of_players - (dipent.seq[1] - '0');
				fprintf(rfp, "You'll be notified when %d more player%s sign%s on.\n",
				n, n == 1 ? "" : "s", n == 1 ? "s" : "");
				n = dipent.seq[1] - '0';
			} else {
				n = dipent.np;
			}
			/* WAS mfprintf  1/94 BLR */
			fprintf(bfp, "%s has signed up to play %s in game '%s'.\n", xaddr,
				powers[power(name[0])], dipent.name);
			fprintf(mbfp, "%s has signed up to play %s in game '%s'.\n", raddr,
				powers[power(name[0])], dipent.name);
			fprintf(rfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s");
			fprintf(rfp, ".\n\n");
			mfprintf(bfp, "Game '%s' now has %d player%s", dipent.name, n, n == 1 ? "" : "s");
			mfprintf(bfp, ".\n\n");
			broad_signon = 1;
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

	} else {

		/*
		 * This is a normal signon attempt to an existing game.
		 */

		userid = siteid = 0;
		n = power(name[0]);
		for (i = 0; i < dipent.n; i++) {
			if (name[0] == ADMINISTRATOR && !strcasecmp(raddr, GAMES_MASTER)) {
				n = MASTER;
				i = dipent.n;
				dipent.players[i].power = n;
				strcpy(dipent.players[i].password, password);
				strcpy(dipent.players[i].address, GAMES_MASTER);
				dipent.players[i].userid = 0;
			}
			if (dipent.players[i].power != n)
				continue;

			if (!strcmp(dipent.players[i].password, password) ||
			    ((n == OBSERVER) && !strcasecmp(dipent.players[i].address, raddr))) {
				player = i;
				signedon = 1;
				listflg = 0;
				if (!dipent.players[i].userid) {
					if (getuser(raddr, &userid, &siteid, &level)) {
						dipent.players[i].userid = userid;
						dipent.players[i].siteid = siteid;
					}
				}
				if (dipent.players[i].power == MASTER)
					xaddr = raddr;

				if (strcasecmp(dipent.players[i].address, raddr) ||
				    !dipent.players[i].userid) {
					if (!getuser(raddr, &userid, &siteid, &level)) {
						if (!msg_header_done)
							msg_header(rfp);
						fprintf(rfp, "Note: Your address %s\n", raddr);
						fprintf(rfp, "has not been registered with the adjudicator.\n");
						fprintf(rfp, "Submit an 'IamAlso %s' command if\n",
							dipent.players[i].address);
						fprintf(rfp, "that is indeed the case or else use 'get form'.\n\n");
					}
					if (userid != dipent.players[i].userid && dipent.players[i].userid) {
						char *s, *t;
						if (!(s = strchr(raddr, '@')) ||
						    !(t = strchr(dipent.players[i].address, '@')) ||
						    s - raddr != t - dipent.players[i].address ||
						    strncasecmp(dipent.players[i].address, raddr, s - raddr)) {
							fprintf(xfp, "Orders for %s in '%s' submitted by %s\n",
								powers[dipent.players[i].power], dipent.name, raddr);
							fprintf(xfp, "instead of %s.\n", dipent.players[i].address);
						}
					}
				}
				break;
			}
			if (dipent.players[i].status & (SF_CD | SF_ABAND) ||
			    *dipent.players[i].address == '*') {

				long now, then;

				if (!getuser(raddr, &userid, &siteid, &level)) {
					if (!msg_header_done)
						msg_header(rfp);
					fprintf(rfp, "Your address %s is not registered with the judge.\n", raddr);
					fprintf(rfp, "Use a 'get form' request for information on registering.\n");
					return E_WARN;
				}
				if (mail_access(i, userid, siteid, level, &j))
					return E_WARN;

				if (j >= 0)
					dipent.players[j].power = -1;	/* Remove him as observer */

				broad_signon = 1;
				pcontrol++;
				strcpy(baddr, dipent.players[i].address);
				strcpy(dipent.players[i].address, raddr);
				strcpy(dipent.players[i].password, password);
				dipent.players[i].userid = userid;
				dipent.players[i].siteid = siteid;
				player = i;
				signedon = 1;
				listflg = 0;

				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Take over of abandoned %s allowed.\n\n", powers[n]);
				time(&now);
				if (dipent.deadline < now + 24 * 60 * 24 && dipent.phase[5] == 'M'
				    && !(dipent.flags & F_MODERATE)) {
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
				fprintf(bfp, "%s has taken over the abandoned\n%s in game '%s'.\n",
					xaddr, powers[n], dipent.name);
				fprintf(mbfp, "%s has taken over the abandoned\n%s in game '%s'.\n",
					raddr, powers[n], dipent.name);
				pprintf(cfp, "%s%s has taken over the abandoned\n%s in game '%s'.\n", NowString(),
					xaddr, powers[n], dipent.name);

				time(&now);

				if (dipent.deadline < now + 24 * 60 * 60) {
					dipent.deadline = now + 24 * 60 * 60;
					fprintf(rfp, "Deadline for '%s' advanced to %s.\n",
						dipent.name, ptime(&dipent.deadline));
					mfprintf(bfp, "Deadline for '%s' advanced to %s.\n",
						 dipent.name, ptime(&dipent.deadline));
					fprintf(cfp, "%sDeadline for '%s' advanced to %s.\n", NowString(),
						dipent.name, ptime(&dipent.deadline));
				}
				if (dipent.grace < now + 48 * 60 * 60) {
					dipent.grace = now + 48 * 60 * 60;
					fprintf(rfp, "Grace period for '%s' advanced to %s.\n",
						dipent.name, ptime(&dipent.grace));
					mfprintf(bfp, "Grace period for '%s' advanced to %s.\n",
						 dipent.name, ptime(&dipent.grace));
					pprintf(cfp, "%sGrace period for '%s' advanced to %s.\n", NowString(),
						dipent.name, ptime(&dipent.grace));
				}
				break;
			}
		}

		if (signedon) {
			long now;
			time(&now);
			ded[dipent.players[player].userid].ls = now;
		} else {
			if (n == OBSERVER) {
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
					dipent.players[dipent.n].userid = userid;
					dipent.players[dipent.n].siteid = siteid;
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

		if (dipent.seq[0] == 'x') {
			signedon = -1;
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' hasn't started up yet.\n", dipent.name);
			fprintf(rfp, "You'll be notified when %d more people sign on.\n",
			   dipent.no_of_players - (dipent.seq[1] - '0'));
			return 0;
		}
		if (dipent.phase[6] == 'X') {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Game '%s' has been terminated.\n", dipent.name);
			fprintf(rfp, "Use the 'resume' command to start it back up.\n\n");
		}
		sprintf(Tfile, "D%s/P%s", dipent.name, dipent.seq);
		if ((pfp = fopen(Tfile, "a+")) == NULL) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Error opening %s to write future orders.\n", Tfile);
			return E_FATAL;
		}
		sprintf(Tfile, "D%s/T%s", dipent.name, dipent.seq);
		if ((ofp = fopen(Tfile, "w")) == NULL) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
			return E_FATAL;
		}
		sprintf(Mfile, "D%s/M%s", dipent.name, dipent.seq);
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

int mail_access(int ignore, int userid, int siteid, int level, int *idx)
{

/*
 *  Check to see if this person is permitted to access this game.
 *  Return value 0 = okay, 1 = already on, -1 = no way.  
 *  Observe is set to the index of an observation point if one exists.
 */

	int i, s1, s2, ret;
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
		if ((dipent.players[i].userid == userid &&
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
			s1 = siteid;
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
			s1 = siteid;
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

	if (dipent.level == L_NOVICE && level != L_NOVICE) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Novice-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(level)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_INTERMEDIATE && level != L_INTERMEDIATE) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Intermediate-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(level)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_EXPERT && level != L_EXPERT) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Expert-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(level)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_ADVANCED && (!level || level == L_NOVICE)) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Advanced-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(level)));
		free(temp);
		return -1;
	}
	if (dipent.level == L_AMATEUR && (!level ||
			       level == L_EXPERT || level == L_WINNER)) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' is designated as Amateur-only\n",
			dipent.name);
		fprintf(rfp, "and you are registered as %s.\n",
			(temp = printlevel(level)));
		free(temp);
		return -1;
	}
	if (dipent.dedicate && ded[userid].r < dipent.dedicate) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp, "Sorry, game '%s' requires a dedication of at least %d and you\n",
			dipent.name, dipent.dedicate);
		fprintf(rfp, "are currently rated at %d.  Use 'get deadline' for more info.\n",
			ded[userid].r);
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

	char line[150];
	int i, j, k, n;
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

#define UNAVAILABLE_PREFERENCE INT_MAX
	/* Preference value for an already taken power */

	for (i = 0; i < WILD_PLAYER; i++)
		for (j = 0; j < WILD_PLAYER; j++)
			wv[i][j] = 0;

	/*
	 *  Pick a random power for those not yet specified.
	 */

	for (i = 1; i < WILD_PLAYER; i++)
		wp[i] = dipent.pl[i] != 'x' ? 1 : 0;

	for (i = 0; i < dipent.n; i++) {
		if (dipent.players[i].power < 0)
			continue;

		if ((j = dipent.players[i].power) < WILD_PLAYER) {
			wp[j] = 0;
		}
	}

	for (i = n = 0; i < dipent.n; i++) {
		if (dipent.players[i].power == WILD_PLAYER) {
			chkpref(dipent.players[i].pref, wp, wv[n++]);
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
			dipent.players[i].status = SF_MOVE;
	}

	/* While we haven't assigned all the players. Only happens if a
	 * "set players" command has been entered.
	 */

	while (assigned < dipent.np) {
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

		while (assigned + number_of_players > dipent.np) {
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
				memcpy(&dipent.players[dipent.n], &dipent.players[i], sizeof(Player));
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
	seq.clock = dipent.movement.clock;
	seq.mint = dipent.movement.mint;
	seq.next = dipent.movement.next * 2;
	seq.grace = dipent.movement.grace;
	seq.delay = dipent.movement.delay * 2;
	strcpy(seq.days, dipent.movement.days);
	dipent.deadline = 0;
	deadline(&seq, 1);

	/*
	 * Create a game file.
	 */

	sprintf(line, "D%s", dipent.name);
	mkdir(line, 0777);
	sprintf(line, "D%s/G%s", dipent.name, dipent.seq);
	if ((ofp = fopen(line, "w")) == NULL) {
		fprintf(stderr, "igame: Error opening game file %s.\n", line);
		bailout(E_FATAL);
	}
	sprintf(line, "data/seed.%d", dipent.variant);
	if ((tfp = fopen(line, "r")) == NULL) {
		fprintf(stderr, "igame: Error opening seed file %s.\n", line);
		bailout(E_FATAL);
	}
	while (fgets(line, sizeof(line), tfp))
		fputs(line, ofp);
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

		if ((ofp = fopen("dip.temp", "w")) == NULL) {
			fprintf(stderr, "igame: Error opening second temporary file.\n");
			bailout(E_FATAL);
		}
		msg_header(ofp);
		fprintf(ofp, "You have been selected as %s in game '%s' of Diplomacy.",
			powers[dipent.players[i].power], dipent.name);
		fprintf(ofp, "\n\nThe following players are in this game:\n");
		for (j = 0; j < dipent.n; j++) {
			if (dipent.players[j].power < 0)
				continue;

			fprintf(ofp, "     %s:", powers[k = dipent.players[j].power]);
			for (k = strlen(powers[k]); k < LPOWER + 1; k++)
				putc(' ', ofp);
			fprintf(ofp, "%s\n", (dipent.flags & F_GUNBOAT) && j != i
				&& dipent.players[j].power != MASTER
			   && dipent.players[i].power != MASTER ? someone
				: dipent.players[j].address);
		}
		sprintf(line, "data/report.%d", dipent.variant);
		if ((tfp = fopen(line, "r")) == NULL) {
			fprintf(stderr, "igame: Error opening data/report file %s.\n", line);
			bailout(E_FATAL);
		}
		while (fgets(line, sizeof(line), tfp))
			fputs(line, ofp);
		fprintf(ofp, "The deadline for the first movement orders is %s.\n",
			ptime(&dipent.deadline));
		fclose(ofp);
		fclose(tfp);

		if (i != dipent.n) {
			sprintf(line, "./smail dip.temp 'Diplomacy game %s starting' '%s'",
				dipent.name, dipent.players[i].address);
			if (execute(line)) {
				fprintf(stderr, "igame: Error sending mail to %s.\n",
					dipent.players[i].address);
				bailout(E_FATAL);
			}
			pprintf(cfp, "     %s:", powers[k = dipent.players[i].power]);
			for (k = strlen(powers[k]); k < LPOWER + 1; k++)
				putc(' ', cfp);
			pprintf(cfp, "%s\n", (dipent.flags & F_GUNBOAT)
			   && dipent.players[i].power != MASTER ? someone
				: dipent.players[i].address);
		}
	}
	sprintf(line, "Diplomacy game '%s' starting", dipent.name);
	archive("dip.temp", line);

/*
 * Record start date for the summary
 */

	sprintf(line, "D%s/start", dipent.name);
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
			&& dipent.players[i].power != MASTER ? someone
			: dipent.players[i].address);
	}

	sprintf(line, "D%s/info", dipent.name);
	if ((fp = fopen(line, "r"))) {
		fputc('\n', ofp);
		while (fgets(line, sizeof(line), fp))
			fputs(line, ofp);
		fclose(fp);
	}
	fclose(ofp);

	{
		if (dipent.variant != V_STANDARD || dipent.flags & F_GUNBOAT)
			sprintf(line, "./smail dip.temp 'MNC: Game %s starting' '%s'",
				dipent.name, MN_CUSTODIAN);
		else
			sprintf(line, "./smail dip.temp 'BNC: Game %s starting' '%s'",
				dipent.name, BN_CUSTODIAN);
	}
	execute(line);

	/*
	 * TODO add in a GAME_START_NOTIFY config or something 
	 * similar here.  This is where the ep custodian had been
	 * notified
	 */

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
