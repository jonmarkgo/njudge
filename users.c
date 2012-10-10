/*
 * $Log: users.c,v $
 * Revision 1.18  2011-02-07 19:42:38  chrisb
 * Applied Philippe Lalande patch for Bug 260
 *
 * Revision 1.17  2010-08-18 00:25:07  chrisb
 * Anti-phishing patch.
 * Retrieve a registration for whois only if just one user matches.
 *
 * Revision 1.16  2003-12-02 18:46:34  millis
 * Reconsidered change, restoring old version
 *
 * Revision 1.15  2003/12/02 18:43:06  millis
 * Improved checking for Bug 250, namely not allowing blank addresses
 *
 * Revision 1.14  2003/11/18 03:20:10  nzmb
 * improved cmpaddr so it (should) return 0 on a blank address or one that has
 * a non alphanumeric character as its first character -- partial fix to bug
 * number 250.
 *
 * Revision 1.13  2003/10/12 06:16:20  nzmb
 * Fixed bugs 240 & 241. You now cannot iamalso yourself to more than one
 * registration. Also if you try to do iamalso address1,address2 the Judge
 * will tell you to only iamalso yourself to one place.
 *
 * Revision 1.12  2003/02/28 20:16:47  nzmb
 * Changed the name of resignation ratio to CD ratio, to avoid confusion with
 * Doug Massey's DRR.
 *
 * Revision 1.11  2002/11/24 23:22:23  millis
 * Removed unused function "checklist"
 *
 * Revision 1.10  2002/11/08 21:59:26  millis
 * Fixed bug 36, to check if player is really blacklisted or simply trying
 * on a restricted judge
 *
 * Revision 1.9  2002/08/27 22:27:58  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.8  2002/06/23 22:56:29  nzmb
 * Cosmetic change to infoplayer/getdedication display.
 *
 * Revision 1.7  2001/12/29 20:38:04  nzmb
 *
 * Added infoplayer, record commands. Put judge version to 1.0.0 as we think it is
 * stable.
 *
 * Revision 1.6  2001/07/15 09:20:56  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.5  2001/06/24 06:08:18  nzmb
 * Added code so players may now see their plyrdata records via the "get
 * dedication" command.
 *
 * Revision 1.4  2001/05/08 07:47:47  greg
 * minor fix to allow whogame command by players after a gunboat game ends, unless it's noreveal
 *
 * Revision 1.3  2001/01/06 18:20:42  davidn
 * Correction to parsing of players.DENY to only read lines with = at the
 * start. This stops it reading lines which are commented out.
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Minor changes only
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  users.c
 *  Copyright 1989, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "conf.h"
#include "config.h"
#include "dip.h"
#include <diplog.h>
#include "mail.h"
#include "functions.h"
#include "plyrdata.h"

/*  Max. number of lines in which necessary information must appear  */

#define MAXLINE 30

/*  Defines for registration commands  */

#define PHONE    1
#define EMAIL    2
#define SITE     3
#define LEVEL    4
#define USER     5
#define COUNTRY  6
#define _PACKAGE  7

/*  Defines for "Package:" options  */

#define DFT_SEND 0		/* Default:  check for Package.Autosend file */
#define SEND     1		/* Definitely send the package               */
#define NO_SEND  2		/* Definitely don't send the package         */

/*
 *  Keep track of a database of users for Diplomacy.
 */

/*
 *  newuser(addr).  Add or replace an entry to the user database.  More
 *  user information is on the input file until an "end" command is found.
 */

int newuser(char *addr, FILE * fp)
{

	int i, j, k, n, userid, siteid, level, got, sded = nded, new_entry,
	 send_pkg = DFT_SEND;
	char *s, *t;
	char line[1024], *lines[MAXLINE], *lookfor();
	char *phone = NULL, *mail = NULL, *site = NULL, *country = NULL,
	*not_eof;
	FILE *fp1, *fp2;
	struct stat sbuf;

	static char *keys[] =
	{"", "phone:", "email:", "e-mail:", "site:", "level:",
	 "user:", "country:", "package:"};
	static int vals[] =
	{0, PHONE, EMAIL, EMAIL, SITE, LEVEL, USER, COUNTRY,
	 _PACKAGE};

	static char *levs[] =
	{"", "novice", "advanced", "intermediate",
	 "amateur", "expert", "winner"};
	static int lval[] =
	{L_AMATEUR, L_NOVICE, L_ADVANCED, L_INTERMEDIATE,
	 L_AMATEUR, L_EXPERT, L_WINNER};
	static char *pkgs[] =
	{"", "yes", "send", "no", "don't send", "don't"};
	/* TODO
	 * pval is unused... unlike most it looks important, so i'm leaving it
	 * in, but commented out -- nw Sat Jun  7 22:18:24 GMT 1997
	 */
	/* static int pval[] = {DFT_SEND, SEND, SEND, NO_SEND, NO_SEND, NO_SEND}; */
	n = got = level = 0;
	while ((not_eof = fgets(line, sizeof(line), fp))) {
		diplog_entry(line);
		fputs(line, ifp);
		if (!strchr(line, ':')) {
			not_eof = NULL;
			break;
		}
		if (!(lines[n++] = (char *) malloc(strlen(line) + 1))) {
			fprintf(rfp, "Unable to malloc space in newuser.\n");
			return E_FATAL;
		}
		strcpy(lines[n - 1], line);
		s = lookfor(lines[n - 1], keys, nentry(keys), &i);
		switch (vals[i]) {
		case PHONE:
			got |= 1;
			phone = s;
			break;

		case COUNTRY:
			got |= 16;
			country = s;
			break;

		case EMAIL:
			got |= 2;
			mail = s;
			break;

		case SITE:
			got |= 4;
			site = s;
			break;

		case LEVEL:
			got |= 8;
			lookfor(s, levs, nentry(levs), &i);
			if (!i)
				fprintf(rfp, "Unrecognized level specified, assuming %s.\n",
					levs[lval[i]]);
			level = lval[i];
			break;

		case USER:
			free(lines[--n]);	/* Don't save 'User:' line */
			break;

		case _PACKAGE:
			printf("found package: %s\n", s);
			lookfor(s, pkgs, nentry(pkgs), &send_pkg);
			free(lines[--n]);	/* Don't save 'Package:' line */
			break;
		}
		if ((n >= MAXLINE) || (got == (1 | 2 | 4 | 8 | 16)))
			break;
	}

	if ((got & (1 | 2 | 4 | 16)) != (1 | 2 | 4 | 16)) {
		fprintf(rfp, "Your phone number, email addresses, country and the ");
		fprintf(rfp, "name of your site must\n");
		fprintf(rfp, "appear within the first %d lines of the registration ", MAXLINE);
		fprintf(rfp, "data and you must supply\n");
		fprintf(rfp, "a colon following the keyword on each line.\n");
		return E_WARN;
	}
	if (!(fp1 = fopen("dip.addr", "r"))) {
		perror("addresses");
		fprintf(rfp, "Unable to open addresses.\n");
		return E_FATAL;
	}
	userid = 1;
	while ((s = fgets(line, sizeof(line), fp1))) {
		i = atoi(line + 1);
		if (i >= userid)
			userid = i + 1;
		s = strchr(line, '=');
		if (!s) {
			fprintf(rfp, "Address data is corrupted.\n");
			return E_FATAL;
		}
		s++;
		if (cmpaddr(s, addr))
			break;
		if (cmpaddr(s, mail))
			break;
	}
	fclose(fp1);

	/*
	 *  Set the site code based on the phone number.
	 */

	if (s) {
		new_entry = 0;
		sscanf(line + 1, "%d %d", &userid, &siteid);
	} else {
		new_entry = 1;
		s = phone;
		while (*s && !isdigit(*s))
			s++;
		siteid = atoi(s) * 100;
	}

	/*
	 *  Attempt to get a better site code based on the country.
	 */

	if (country) {
		while (isspace(*country))
			country++;
		s = country + strlen(country) - 1;
		while (isspace(*s))
			*s-- = '\0';
		i = -1;
		if ((fp1 = fopen("data/countries.x", "r"))) {
			while (fgets(line, sizeof(line), fp1)) {
				if ((s = strchr(line, ':'))) {
					*s++ = '\0';
					if (!strcasecmp(country, line)) {
						if ((i = atoi(s)))
							siteid = i * 100;
						break;
					}
				}
			}
			fclose(fp1);
		}
		if (i == -1) {
			fprintf(xfp, "Unknown country name: [%s].\n", country);
		}
		strcat(country, "\n");
	}
	/* if the siteid is 0, don't process the registration. */
	if (!siteid) {
		fprintf(rfp, "Your Site ID resolved to a 0.  Please verify your phone ");
		fprintf(rfp, "number and your country.\nIf you think there is a problem ");
		fprintf(rfp, "on the judge's end, please contact the JK (%s).\n", conf_get("judge_keeper"));
		return E_WARN;
	}
	if (userid >= MAXUSER) {
		fprintf(rfp, "Too many people registered.  Increase MAXUSER.\n");
		return E_FATAL;
	}
	/*
	 *  Build a new whois database.  Check for other people at this site.
	 */

	if (!(fp1 = fopen("dip.whois", "r"))) {
		perror("whois");
		fprintf(rfp, "Unable to open whois database.\n");
		return E_FATAL;
	}
	if (!(fp2 = fopen("dip.whois.new", "w"))) {
		perror("whois.new");
		fprintf(rfp, "Unable to create whois database.\n");
		return E_FATAL;
	}
	while (fgets(line, sizeof(line), fp1)) {
		if (!strncmp(line, "User:", 5)) {
			sscanf(line + 5, "%d%d%d", &i, &j, &k);
			if (i >= sded) {
				if (i > MAXUSER - 1 || i < 0) {
					fprintf(rfp, "Whois database corrupted.  Bad userid.\n");
					return E_FATAL;
				}
				if (i >= nded)
					nded = i + 1;
				ded[i].r = k;
			}
			if (i == userid) {
				while ((s = fgets(line, sizeof(line), fp1)) && strncmp(line, "User:", 5));
				if (!s)
					break;
				sscanf(line + 5, "%d%d%d", &i, &j, &k);
				if (i >= sded) {
					if (i > MAXUSER - 1 || i < 0) {
						fprintf(rfp, "Whois database corrupted.  Bad userid.\n");
						return E_FATAL;
					}
					if (i >= nded)
						nded = i + 1;
					ded[i].r = k;
				}
			}
		}
		if (!strncasecmp(line, "Site:", 5)) {
			for (s = line + 5; isspace(*s); s++);
			if (!strcasecmp(s, site))
				siteid = j;
		}
		fputs(line, fp2);
	}
	fclose(fp1);

	fprintf(xfp, "Registration request processed:\n");

	fprintf(xfp, "User: %4d %d %d\n", userid, siteid, ded[userid].r);
	fprintf(fp2, "User: %4d %d %d\n", userid, siteid, ded[userid].r);
	for (i = 0; i < n; i++) {
		fputs(lines[i], xfp);
		fputs(lines[i], fp2);
	}

	while ((not_eof = fgets(line, sizeof(line), fp))) {
		diplog_entry(line);
		fputs(line, ifp);
		if (!strchr(line, ':'))
			break;
		s = lookfor(line, keys, nentry(keys), &i);
		switch (i) {
		case USER:
			/* Ignore 'User:' if entered */
			break;

		case _PACKAGE:
			lookfor(s, pkgs, nentry(pkgs), &send_pkg);
			/* Don't write 'Package:' to dip.whois file */
			break;

		default:
			fputs(line, xfp);
			fputs(line, fp2);
		}
	}

	for (s = line; isspace(*s); s++);
	if (*s && strncasecmp("end", line, 3)) {
		fprintf(rfp, "Warning, end of registration command not found.\n");
		fprintf(rfp, "Discarding: %s", s);
	}
	fprintf(xfp, "\n");

	ferrck(fp2, 4001);
	fclose(fp2);

	/*
	 *  Rebuild the addresses file.
	 */

	if (!(fp1 = fopen("dip.addr", "r"))) {
		perror("addresses");
		fprintf(rfp, "Unable to open address file.\n");
		return E_FATAL;
	}
	if (!(fp2 = fopen("dip.addr.new", "w"))) {
		perror("address.new");
		fprintf(rfp, "Unable to create new address file.\n");
		return E_FATAL;
	}
	while (fgets(line, sizeof(line), fp1)) {
		/* Loop and put lines to new file until we hit current player */
		if (atoi(line + 1) == userid) {
			if (*line != '+')
				continue;
			s = strchr(line, '=');
			if (!s) {
				fprintf(rfp, "Address data is corrupted.\n");
				return E_FATAL;
			}
			s++;
			if (cmpaddr(s, addr))
				continue;
			if (cmpaddr(s, mail))
				continue;
		}
		fputs(line, fp2);
	}
	fprintf(fp2, "+%d %d %d =%s\n", userid, siteid, level, addr);
	s = mail;
	while (*s) {
		t = s;
		while (*t && !isspace(*t) && *t != ',')
			t++;
		if (*t)
			*t++ = '\0';
		fprintf(fp2, "*%d %d %d =%s\n", userid, siteid, level, s);
		s = t;
		while (isspace(*s))
			s++;
	}
	fclose(fp1);
	ferrck(fp2, 4002);
	fclose(fp2);

	/*
	 *  Rename the files and get rid of excess baggage.
	 */

	rename("dip.whois.new", "dip.whois");
	rename("dip.addr.new", "dip.addr");
	for (i = 0; i < n; i++)
		free(lines[i]);

	/*
	 *  If this is a new registration, check whether to send the package.
	 */

	if (new_entry) {
		if ((send_pkg == SEND) ||
		    ((send_pkg == DFT_SEND) && !stat("Package.Autosend", &sbuf))) {
			fprintf(rfp, "Newuser package being sent separately.\n");
			send_package(addr);
		} else {
			fprintf(rfp, "Newuser package is not being sent.  To get it, send ");
			fprintf(rfp, "'get package' to the Judge.\n");
		}
	} else {
		fprintf(rfp, "This is an update to an existing registration; newuser ");
		fprintf(rfp, "package is not sent.\n");
		fprintf(rfp, "To get it, send 'get package' to the Judge.\n");
	}

	fprintf(rfp, "Registration processed.\n");

	return 0;

}

/***************************************************************************/

/*
 *  Send Newuser Package of Files (upon registration, or upon request)
 */

void send_package(char *addr)
{
	char line[150], cmd[150], *eol;
	FILE *pfp;
	struct stat sbuf;

	if (!(pfp = fopen("data/package", "r"))) {
		fprintf(rfp, "Package list is not available.  Please request individual ");
		fprintf(rfp, "files separately.\n");
	} else {
		while (fgets(line, sizeof(line), pfp)) {
			for (eol = line; *eol && !isspace(*eol); ++eol);
			*eol = '\0';
			if (stat(line, &sbuf)) {
				fprintf(rfp, "Package file %s is not available.\n", line);
			} else {
				sprintf(cmd, "%s %s 'Diplomacy file %s' '%s'", conf_get("cmd_smail"),line, line, addr);
				execute(cmd);
				fprintf(rfp, "Package file %s sent.\n", line);
			}
		}
		fclose(pfp);
		fprintf(rfp, "\nPackage send completed.\n");
	}
	return;
}

/***************************************************************************/

/*
 *  I am Also: Add a new entry to the addresses file.
 */

int iamalso(char *addr, char *oldaddr)
{

	int userid, siteid, level;
	char *s, line[200];
	FILE *fp1, *fp2;

	/* do some basic checking of addr (if it has a , or ; or : the 
	   user may be trying to register more than 1 address */
	if(strchr(oldaddr,':') || strchr(oldaddr,',') || strchr(oldaddr,';'))
	{
		s = strchr(oldaddr,'\n');
		if(s)
			*s = '\0';
		fprintf(rfp,"IAmAlso: bad address '%s'.\n", oldaddr);
		fprintf(rfp,"It looks like you may be trying to IAmAlso your address to more than 1 existing addresses.\n");
		return 1;
	}

	if (!(fp1 = fopen("dip.addr", "r"))) {
		perror("address");
		bailout(1);
	}
	if (!(fp2 = fopen("dip.addr.new", "w"))) {
		perror("address.new");
		bailout(1);
	}
	userid = 0;
	while (fgets(line, sizeof(line), fp1)) {
		s = strchr(line, '\n');
		if(s)
			*s = '\0';
		s = strchr(line, '=');
		if (s && !strcasecmp(s + 1, addr)) {
			fprintf(rfp, "IamAlso ignored, %s already registered.\n", addr);
			fclose(fp1);
			fclose(fp2);
			remove("dip.addr.new");
			return 0;
		}
		if (s && cmpaddr(s + 1, oldaddr))
			sscanf(line + 1, "%d %d %d", &userid, &siteid, &level);
		fprintf(fp2, "%s\n", line);
	}

	if (!userid) {
		fprintf(rfp, "IamAlso %sAddress not found as registered.\n", oldaddr);
		fclose(fp1);
		fclose(fp2);
		remove("dip.addr.new");
		return 1;
	}
	fprintf(rfp, "%s registered as user #%d.\n", addr, userid);
	fprintf(xfp, "%s registered as user #%d.\n", addr, userid);
	fprintf(fp2, "+%d %d %d =%s\n", userid, siteid, level, addr);
	fclose(fp1);
	ferrck(fp2, 4003);
	fclose(fp2);
	rename("dip.addr.new", "dip.addr");
	return 0;

}

/***************************************************************************/

 /*
  * Get user: search the database to find the user and site ids.
  */

int getuser(char *addr, int *userid, int *siteid, int *level)
{

	char *s, line[200];
	FILE *fp;

	if (!(fp = fopen("dip.addr", "r"))) {
		perror("users.c-getuser");
		bailout(1);
	}
	while (fgets(line, sizeof(line), fp)) {
		s = strchr(line, '=');
		if (s && cmpaddr(s + 1, addr)) {
			sscanf(line + 1, "%d %d %d", userid, siteid, level);
			return 1;
		}
	}
	return 0;
}


/***************************************************************************/

 /*
  * Whogame: search dip.master for a game, and whois the players.
  */

void whogame(int f)
{
	int x;
        char *addresses, *address;

	if ((dipent.flags & F_GUNBOAT)
	    && (!signedon || dipent.players[player].power != MASTER)
	    && (dipent.phase[6] != 'X'))
		fprintf(rfp, "Game '%s' is a gunboat game.\n", dipent.name);
	else
		if ((dipent.flags & F_GUNBOAT) && (dipent.flags & F_NOREVEAL)
		    && (!signedon || dipent.players[player].power != MASTER))
			fprintf(rfp, "Game '%s' is a gunboat noreveal game.\n", dipent.name);
		else
		for (x = 0; x < dipent.n; x++)
			if (dipent.players[x].power < 0)
				continue;
			else if (!f && (dipent.players[x].power == AUTONOMOUS ||
				    dipent.players[x].power == OBSERVER))
				continue;
			else {
				fprintf(rfp, "%s in game '%s':\n\n",
					powers[dipent.players[x].power], dipent.name);
				if (!strcmp(dipent.players[x].address, "*"))
					fprintf(rfp, "%s is abandoned.  Address unknown.\n\n",
					powers[dipent.players[x].power]);
				else {
                                        addresses = strdup(dipent.players[x].address);
                                        address = strtok(addresses,", ");
                                        whois(address);       
                                        while ( (address = strtok(NULL,", ")) )
                                                whois(address);

                                        free(addresses);
                                        //whois(dipent.players[x].address);
			}
               }
	putc('\n', rfp);
	return;
}

/***************************************************************************/

/*
 * Whois: dump user info to the reply file.
 */

void whois(char *t)
{

	int users[MAXUSER], user, i, n = 0, len;
	char *s, *p, *q, line[200];
	FILE *fp;

	if (!*t) {
		fprintf(rfp, "Who is whom?\n\n");
		return;
	}
	fprintf(rfp, "Whois %s", t);
	if (!(fp = fopen("dip.addr", "r"))) {
		fprintf(rfp, "Whois: can't open address file.\n\n");
	} else {
		n = 0;
		while (fgets(line, sizeof(line), fp)) {
			user = atoi(line + 1);
			for (i = 0; i < n; i++)
				if (user == users[i])
					break;
			if (i < n)
				continue;
			if (!(s = strchr(line, '=')))
				continue;
			p = t;
			while (*p) {
				for (len = 0, q = p; *q && !isspace(*q); q++, len++);
				if (!strncasecmp(s + 1, p, len)) {
					users[n++] = user;
					break;
				}
				p = q;
				while (isspace(*p))
					p++;
			}
			if (n >= MAXUSER)
				break;
		}
		fclose(fp);
	}

	if (n>1) { n=0; }

	if (!n) {
		fprintf(rfp, "No matching Email addresses found.\n\n");
	} else {
		if (!(fp = fopen("dip.whois", "r"))) {
			fprintf(rfp, "Whois: can't open whois file.\n\n");
		} else {
			i = 0;
			while (fgets(line, sizeof(line), fp)) {
				if (!strncasecmp(line, "User:", 5)) {
					user = atoi(line + 5);
					for (i = 0; i < n; i++)
						if (user == users[i])
							break;
					if (i < n) {
						putc('\n', rfp);
					}
				}
				if (i < n)
					fputs(line, rfp);
			}
			putc('\n', rfp);
			fclose(fp);
		}
	}
}

/***************************************************************************/

 /*
  * send_dedication: Send the user's current dedication
  */

int send_dedication(char *raddr)
{
	int userid, siteid, level;
	float orat,rrat;

	if (!getuser(raddr, &userid, &siteid, &level)) {
		if (!msg_header_done)
			msg_header(rfp);
		fprintf(rfp,
		   "The address %s is not registered with the judge.\n",
			raddr);
		fprintf(rfp,
			"Use a 'get form' request for information on registering.\n");
		return E_WARN;
	}
	fprintf(rfp, "User ID: %d\n\n", userid);
	fprintf(rfp, "Current dedication: %d\n", ded[userid].r);
	fprintf(rfp, "Turns ontime: %lu\n",get_data(userid,ontime));
	fprintf(rfp, "Turns total: %lu\n",get_data(userid,total));
	fprintf(rfp, "Games started: %lu\nPositions taken over: %lu\n",
		get_data(userid,started),get_data(userid,tookover));
	fprintf(rfp, "Resignations: %lu\n",get_data(userid,resigned));
	if(get_data(userid,total) == 0)
	{
		fprintf(rfp, "Played 0 turns; a perfect timeliness record.\n");
	}
	else
	{
		orat = 1.0 * get_data(userid,ontime)/get_data(userid,total);
		fprintf(rfp, "Ontime ratio: %.3f\n", orat);
	}
	if(get_data(userid,started) == 0 && get_data(userid,tookover) == 0)
	{
		fprintf(rfp, "No started games; perfect resignation record.\n\n");
	}
	else
	{
		rrat = 1.0 * get_data(userid,resigned) / (get_data(userid,started) + get_data(userid,tookover));
		fprintf(rfp, "CD ratio: %.3f\n\n", rrat);
	}
	/* TODO i'm not sure what to return here, was no return at all -- nw */
	return 0;
}

/*  DEDICATION CODE, WILL BE ACTIVE IN THE FUTURE  */
/*  Contact Travis C. Ruelle (ruelle@isp.nwu.edu) for details  */

/***************************************************************************/

/* TODO figure out what we wanted to do here.  the next two functions
 * were commented out, i changed that to an #if out */

#if 0
 /*
  * Getded: Dump dedication information to a reply file.
  */

getded(t)
char *t;
{
	fprintf(rfp, "getded: Dedicate %s\n\n", t);
}


/****************************************************************************/

 /*
  * Dedgame: search dip.master for a game, and dedicate the players
  */

dedgame(int f)
{
	int x;

	if ((dipent.flags & F_GUNBOAT)
	    && (!signedon || dipent.players[player].power != MASTER))
		fprintf(rfp, "Game '%s' is a gunboat game.\n", dipent.name);
	else
		for (x = 0; x < dipent.n; x++)
			if (dipent.players[x].power < 0)
				continue;
			else if (!f && (dipent.players[x].power == WILD_PLAYER ||
				 dipent.players[x].power == AUTONOMOUS ||
				    dipent.players[x].power == OBSERVER))
				continue;
			else {
				fprintf(rfp, "%s in game '%s':\n\n",
					powers[dipent.players[x].power], dipent.name);
				getded(dipent.players[x].address);
			}
	putc('\n', rfp);
	return;
}

#endif


/***************************************************************************/

 /*
  * Set a particular site code to a non-generic one.
  */

int setsite(char *s)
{
	int i, j, k, n = 0, siteid, userid, users[MAXUSER], sded = nded;
	char *t, line[1024];
	FILE *fp1, *fp2, *fp3;


	if ((siteid = atoi(s))) {
		while (isdigit(*s))
			s++;
		while (isspace(*s))
			s++;
		fprintf(rfp, "Setting siteid to %d for %s", siteid, s);

		/*
		 * First run through and figure out which users we're talking about.
		 */

		if (!(fp1 = fopen("dip.whois", "r"))) {
			perror("whois");
			bailout(1);
		}
		n = 0;
		while (fgets(line, sizeof(line), fp1)) {
			if (!strncasecmp(line, "User:", 5))
				userid = atoi(line + 5);
			else if (!strncasecmp(line, "Site:", 5)) {
				for (t = line + 5; isspace(*t); t++);
				if (!strcasecmp(t, s)) {
					if (n >= MAXUSER) {
						fprintf(rfp, "Can't change siteid, too many users.\n");
						return 0;
					}
					users[n++] = userid;
				}
			}
		}

		/*
		 *  Then copy the file while changing their site id.
		 */

		rewind(fp1);
		if (!(fp2 = fopen("dip.whois.new", "w"))) {
			perror("whois.new");
			bailout(1);
		}
		while (fgets(line, sizeof(line), fp1)) {
			if (!strncasecmp(line, "User:", 5)) {
				sscanf(line + 5, "%d%d%d", &userid, &j, &k);
				if (userid >= sded) {
					if (userid > MAXUSER - 1 || userid < 0) {
						fprintf(rfp, "Whois database corrupted.  Bad userid: %s", line);
						return E_FATAL;
					}
					if (userid >= nded)
						nded = userid + 1;
					ded[userid].r = k;
				}
				for (i = 0; i < n; i++) {
					if (userid == users[i]) {
						j = siteid;
						break;
					}
				}
				fprintf(fp2, "User: %4d %d %d\n", userid, j, ded[userid].r);
				continue;
			}
			fputs(line, fp2);
		}

		fclose(fp1);
		ferrck(fp2, 4004);
		fclose(fp2);

		/*
		 *  Then copy the addresses file.
		 */

		if (!(fp1 = fopen("dip.addr", "r"))) {
			perror("addresses");
			bailout(1);
		}
		if (!(fp2 = fopen("dip.addr.new", "w+"))) {
			perror("address.new");
			bailout(1);
		}
		while (fgets(line, sizeof(line), fp1)) {
			userid = atoi(line + 1);
			for (i = 0; i < n; i++)
				if (userid == users[i])
					break;
			if (i != n) {
				t = line + 1;
				while (isdigit(*t))
					t++;	/* Skip over userid */
				while (isspace(*t))
					t++;
				while (isdigit(*t))
					t++;	/* Skip over siteid */
				fprintf(fp2, "%c%d %d%s", *line, userid, siteid, t);
				fprintf(rfp, "%c%d %d%s", *line, userid, siteid, t);
				continue;
			}
			fputs(line, fp2);
		}

		fclose(fp1);
		ferrck(fp2, 4005);
		fflush(fp2);
		fp3 = fp2;

		rename("dip.whois.new", "dip.whois");
		rename("dip.addr.new", "dip.addr");

	} else {
		if (!(fp3 = fopen("dip.addr", "r"))) {
			perror("addresses");
			bailout(1);
		}
	}

	/*
	 *  Then fix up the master file.
	 */

	if (!(fp1 = fopen(conf_get("master_db"), "r"))) {
		perror(conf_get("master_db"));
		bailout(1);
	}
	if (!(fp2 = fopen(conf_get("master_db_tmp"), "w"))) {
		perror(conf_get("master_db_tmp"));
		bailout(1);
	}
	while (getdipent(fp1)) {
		for (i = 0; i < dipent.n; i++) {
			if (!dipent.players[i].userid) {
				rewind(fp3);
				while (fgets(line, sizeof(line), fp3)) {
					t = strchr(line, '=') + 1;
					if (cmpaddr(dipent.players[i].address, t)) {
						sscanf(line + 1, "%d%d", &dipent.players[i].userid,
						       &dipent.players[i].siteid);
						fprintf(rfp, "Setting userid for %s in '%s' to %d, %s.\n",
							powers[dipent.players[i].power], dipent.name,
							dipent.players[i].userid, dipent.players[i].address);
						break;
					}
				}
			}
			for (j = 0; j < n; j++)
				if (dipent.players[i].userid == users[j])
					break;
			if (j != n) {
				dipent.players[i].siteid = siteid;
				fprintf(rfp, "Setting siteid for %s in '%s' to %d, %s.\n",
					powers[dipent.players[i].power], dipent.name,
					dipent.players[i].siteid, dipent.players[i].address);
			}
		}
		putdipent(fp2, 1);
	}

	fclose(fp1);
	ferrck(fp2, 4006);
	fclose(fp2);
	fclose(fp3);
	rename(conf_get("master_db_tmp"), conf_get("master_db"));

	/* TODO check this return value.  there was no return at all originally,
	 * but there is a return of zero above on what appears to be an error
	 * of some kind.  so i figured that returning one here would be safe.
	 * -- nw Sat Jun  7 22:29:58 GMT 1997 */

	return 1;
}

/***************************************************************************
 *
 *	ALLOW/DENY Functionality
 *	Jan 1995:  Larry Richardson
 *
 **************************************************************************/

/*
 * is_allowed( int type_flag )
 *  where:   type_flag = GLOBAL_PLAYER - global player list
 *                     = GLOBAL_MASTER - global master list
 *                     = GAME_PLAYER   - game-specific player list
 *                     = GAME_MASTER   - included for completeness, not
 *                                       used
 * returns:  TRUE  - player is allowed at the level specified
 *           FALSE - player is NOT allowed at the level specified
 */
#define ALLOW  ".ALLOW"
#define DENY   ".DENY"

int is_allowed(int type_flag)
{
        char fname[40], uname[40];
        int ret_val = 1;
        FILE *fp;

        switch (type_flag) {
        case GLOBAL_MASTER:
                strcpy(fname, "masters");
                break;
        case GAME_PLAYER:
                if (strlen(dipent.name) == 0) {
                        perror("nogame");
                        return 0;
                }
                sprintf(fname, "%s%s/players", conf_get("game_dir"), dipent.name);
                break;
        case GAME_MASTER:
                 if (strlen(dipent.name) == 0) {
                        perror("nogame");
                        return 0;
                }
                sprintf(fname, "%s%s/masters", conf_get("game_dir"), dipent.name);
                break;
        case GLOBAL_PLAYER:
        default:
                strcpy(fname, "players");
                break;
        }
/* Pass #1.  Look for a file named fname.ALLOW.  If it exists, check to see
   if the name is in there and set the return value accordingly. */
        sprintf(uname, "%s%s", fname, ALLOW);
        fp = fopen(uname, "r");
        if (fp != (FILE *) NULL) {
                ret_val = new_checklist(fp, raddr);
                fclose(fp);
        }
/* Pass #2.  Look for a file named fname.DENY.  If it exists, check to see
   if the name is in there and set the return value accordingly. */
        else {
                sprintf(uname, "%s%s", fname, DENY);
                fp = fopen(uname, "r");
                if (fp != (FILE *) NULL) {
                        ret_val = !new_checklist(fp, raddr);
                        fclose(fp);
                }
        }

/* Pass #3.  If neither the .ALLOW or the .DENY file exists, it must be ok,
   so set the return value, and return. */
        return ret_val;
}

/*
 * is_disallowed( int type_flag )
 *  where:   type_flag = GLOBAL_PLAYER - global player list
 *                     = GLOBAL_MASTER - global master list
 *                     = GAME_PLAYER   - game-specific player list
 *                     = GAME_MASTER   - included for completeness, not
 *                                       used
 * returns:  TRUE  - player is specifically denied at the level specified
 *           FALSE - player is NOT specifically denied (but may still not be allowed)
 *
 * Note: This function should only be called after calling is_allowed() to deterimine
 *       why player is not allowed
 */

int is_disallowed(int type_flag)
{
	char fname[40], uname[40];
	int ret_val = 0;
	FILE *fp;

	switch (type_flag) {
	case GLOBAL_MASTER:
		strcpy(fname, "masters");
		break;
	case GAME_PLAYER:
		if (strlen(dipent.name) == 0) {
			perror("nogame");
			return 0;
		}
		sprintf(fname, "%s%s/players", conf_get("game_dir"), dipent.name);
		break;
	case GAME_MASTER:
		 if (strlen(dipent.name) == 0) {
                        perror("nogame");
                        return 0;
                }
                sprintf(fname, "%s%s/masters", conf_get("game_dir"), dipent.name);
                break;
	case GLOBAL_PLAYER:
	default:
		strcpy(fname, "players");
		break;
	}
/* Pass #1.  Look for a file named fname.DENY.  If it exists, check to see
   if the name is in there and set the return value accordingly. */
	{
		sprintf(uname, "%s%s", fname, DENY);
		fp = fopen(uname, "r");
		if (fp != (FILE *) NULL) {
			ret_val = new_checklist(fp, raddr);
			fclose(fp);
		}
	}

/* Pass #2.  If not in .DENY file, player is not banned 
   so set the return value, and return. */
	return ret_val;
}

/*
 *  new_checklist: See if the user's name is in the passed file
 *
 *  new_checklist( FILE *fp, char *raddr )
 *   where:  fp - the file pointer to use
 *           raddr - the addres to look up
 *
 *   returns:   TRUE if the address is in the file
 *           FALSE if the address is NOT in the file
 */

int new_checklist(FILE * fp, char *addr)
{
	char *s, line[200];

	while (fgets(line, sizeof(line), fp)) {
	        if (*(s = line) != '=')
			continue;
		if (cmpaddr(s + 1, addr))
			return 1;
	}
	return 0;
}

/*
 *  add_player: Add/Remove a player's name to one of the ALLOW/DENY files
 *
 *  add_player( char *name, char *file, char addflag )
 *   where:     name    - the name to be added/removed
 *              file    - the file to be operated on
 *              addflag - = 0, remove the name
 *                        = 1, add the name
 *
 *   returns:   None.
 */

void add_player(char *player_name, char *file, char addflag)
{
	char line[200], *s;
	FILE *in_fp, *out_fp;

	if (addflag) {
		/* First, check that this player isn't already in the file. */
		in_fp = fopen(file, "r");
		if ((in_fp != (FILE *) NULL) &&
		    new_checklist(in_fp, player_name)) {
			fprintf(rfp, "Player %s is already in the %s file",
				player_name, file);
			if (!strcmp(dipent.name, "control"))
				fprintf(rfp, ".\n");
			else
				fprintf(rfp, "\nfor Game '%s'.\n",
					dipent.name);
		} else {
			/* S/He isn't, so go ahead and add him/her. */
			if (in_fp != (FILE *) NULL)
				fclose(in_fp);
			in_fp = fopen(file, "a");
			fprintf(in_fp, "=%s\n", player_name);

			fprintf(rfp, "Player %s is now in the %s file",
				player_name, file);
			if (!strcmp(dipent.name, "control"))
				fprintf(rfp, ".\n");
			else
				fprintf(rfp, "\nfor Game '%s'.\n",
					dipent.name);
		}
		fclose(in_fp);
	} else {
		/* First, check that this player is already in the file. */
		in_fp = fopen(file, "r");
		if ((in_fp == (FILE *) NULL) ||
		    !new_checklist(in_fp, player_name)) {
			if (in_fp != (FILE *) NULL)
				fclose(in_fp);
			fprintf(rfp, "Player %s is not in the %s file",
				player_name, file);
			if (!strcmp(dipent.name, "control"))
				fprintf(rfp, ".\n");
			else
				fprintf(rfp, "\nfor Game '%s'.\n",
					dipent.name);
		} else {
			struct stat file_stat;

			rewind(in_fp);
			out_fp = fopen("dip.allowdeny", "w");
			while (fgets(line, sizeof(line), in_fp)) {
				s = strchr(line, '=');
				if (s == 0)
					continue;
				if (!cmpaddr(s + 1, player_name))
					fputs(line, out_fp);
			}
			fclose(out_fp);
			fclose(in_fp);

			/* Move the temporary file to the real file after
			   saving the original file. */
			sprintf(line, "%s.bak", file);
			rename(file, line);
			rename("dip.allowdeny", file);

			fprintf(rfp, "Player %s is no longer in the %s file",
				player_name, file);
			if (!strcmp(dipent.name, "control"))
				fprintf(rfp, ".\n");
			else
				fprintf(rfp, "\nfor Game '%s'.\n",
					dipent.name);

			/* As a last step, see if the file is empty,
			   if it is, remove it. */
			if (!stat(file, &file_stat)) {
				if (!file_stat.st_size)
					remove(file);
			}
		}
	}
}

/***************************************************************************/

 /*
  * Compare an address to see if it's one we've got.
  */

int cmpaddr(char *addr, char *list)
{

	register char *s, *t, c = 0, d = 0, k;

	/* do some sanity checking on the inputs */
	if(addr[0] == '\0' || list[0] == '\0')
                return 0;
	if(!isalnum(addr[0]) || !isalnum(list[0]))
		return 0;

	s = list;
	while (*s) {
		t = addr;
		k = 0;
		while (*t) {
			c = *s++;
			d = *t++;
			if (islower(c))
				c = toupper(c);
			if (islower(d))
				d = toupper(d);
			if (c == '%' || c == '@' || c == '.')
				c = ++k;
			if (d == '%' || d == '@' || d == '.')
				d = k;
			if (c != d)
				break;
		}
		if (c == d)
			return 1;
		if (!c || isspace(c) || c == ',')
			c = 0;
		if (!d || isspace(d) || d == ',')
			d = 0;
		if ((!c && !d) ||
		    (!c && d == k && k > 1) ||
		    (!d && c == k && k > 1)) {
			return 1;
		}
		while (*s && *s++ != ',');
		while (isspace(*s))
			s++;
	}
	return 0;
}
