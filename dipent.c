/*
 * $Log$
 * Revision 1.22  2004/05/22 09:01:45  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.21  2004/02/14 23:32:11  millis
 * Allow use of fixed time via parameter (for debugging)
 *
 * Revision 1.20  2003/07/17 22:59:29  millis
 * Bug 185
 *
 * Revision 1.19  2003/07/16 14:50:56  millis
 * Used D_X2FLAGS to allow default setting for X2FLAGS for games (if desired)
 *
 * Revision 1.18  2003/06/20 00:21:02  millis
 * Tried to fix it correctly!
 *
 * Revision 1.17  2003/06/19 23:43:18  millis
 * Added a repeat test flag.
 *
 * Revision 1.16  2003/06/19 23:27:55  millis
 * Bug 181, bailout recovery only for control game, and adjust deadlines
 * for non-control games.
 *
 * Revision 1.15  2003/05/14 19:01:22  millis
 * Don't adjust mach files in 'A' season on bailout
 *
 * Revision 1.14  2003/05/13 00:07:26  millis
 * Bug 110, move on process deadline by 24 hours on bailout recovery
 *
 * Revision 1.13  2003/05/12 23:47:59  millis
 * Fix bug 110, shift process time out on a timewarp.
 *
 * Revision 1.12  2003/04/16 04:31:32  millis
 * Fixed a bug that zapped the x/x2/flags settings on getdipent calls
 *
 * Revision 1.11  2003/01/13 22:38:51  millis
 * merged in from ustv
 *
 * Revision 1.10  2002/08/27 22:27:50  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.9  2001/10/20 12:11:11  miller
 * Merged in changes from DEMA and USTV
 *
 * Revision 1.8.2.2  2001/10/19 23:29:08  dema
 * Allow powers with spaces in their names to be used
 *
 * Revision 1.8.2.1  2001/10/15 00:00:24  ustv
 * Added reading/writing of dipent.x2flags
 *
 * Revision 1.8  2001/07/15 09:14:18  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.7  2001/07/08 22:54:49  miller
 * Set default correctly for rrded, and add TIME_TOLERANCE usage from dip.conf
 *
 * Revision 1.6  2001/07/01 23:19:29  miller
 * Default for XF_COASTAL
 *
 * Revision 1.5  2001/06/24 05:30:29  nzmb
 * Added read/write capability for new dipent variables (dedapplied, orded,
 * rrded) used in the plyrdata and new deadline systems.
 *
 * Revision 1.4  2001/05/26 11:20:34  miller
 * Do not notify time-warp for shifts < 1 minute
 *
 * Revision 1.3  2001/01/05 22:27:53  miller
 * Fix to test using '==' not '='
 * (made all games erroneously of Chaos variant when upgrading from older dip.master format.
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added handling of new XF_:FLAGS , and absence data elements in master.dip
 * Used gerenric flags to handle variants (not specificif tests)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/02/16 20:43:18  davidn
 * Additions to dipent structure and associated code, to allow duplex variants.
 * Command is "set players n".
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*
 * dipent.c *  Copyright 1987, Lowe. * *  Diplomacy is a trademark of the
 * Avalon Hill Game Company, Baltimore, *  Maryland, all rights reserved;
 * used with permission. * *  Redistribution and use in source and binary
 * forms are permitted *  provided that it is for non-profit purposes,
 * that this and the  *  above notices are preserved and that due credit
 * is given to Mr. *  Lowe. * *  DATE        NAME         REASON *
 * ----------- ------------ ----------------------------------------- *
 * ?? ??? 1987 Ken Lowe     He wrote it *  29 Dec 1996 David Norman
 * Addition of dipent.no_of_players *  29 Dec 1996 David Norman Protection 
 * against killing dip.master added 
 * 26 Nov 1999 Millis Miller Added player late_count to payer structure
 *                           Also added use of xflags structure.
 * 26 May 2001 Mario Becroft Added code for dipent.orded,dipent.rrded, 
 *	       Tim Miller    and dipent.dedapplied, all part of the new
 *			     dedication systems.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "dip.h"
#include "defaults.h"
#include "functions.h"
#include "diplog.h"

extern int Dflg;

char *ctime();
char *lookfor();

void putseq(FILE * fp, char *s, sequence * seq);
void gettime(char *line, long *time);
void getplay(char *line, Player * p);
void putplay(FILE * fp, Player * p, int dopw);

int time_warp = 0; /* Set to 1 when a time warp was detected */
/************************************************************************/

int getdipent(FILE * fp)
{

/*
 *  Read the next game entry from the master file.
 */

	int i, j, tempvp, tempplayers;
	int old_flags, old_xflags, old_x2flags, old_x3flags;  /* Remember flags settings! */
	time_t now;
	unsigned char line[1000];
	char *s; 
	char *malloc();
	static int recover_print = 0;

	memset(&dipent, 0, sizeof(dipent));
	if (!fgets(line, sizeof(line), fp))
		return 0;
	i = sscanf(line, "%s%s%s%d%d%d%d%x%d%d%x%d%x%d%x%d", dipent.name, dipent.seq, dipent.phase,
		   &dipent.access, &dipent.variant,
		   &dipent.level, &dipent.dedicate,
		   &dipent.flags, &tempvp, &tempplayers,
		   &dipent.xflags, &dipent.max_absence_delay,
		   &dipent.x2flags, &dipent.num_homes,
		   &dipent.x3flags, &dipent.powers);
	switch (i) {

	    case 7:
		dipent.flags = F_NONMR;
	
	    case 8:
		tempvp = 0;
	
	    case 9:
		tempplayers = 0;
	
	    case 10:
		dipent.xflags = 0;
		if ((dipent.variant == V_h31)
                    || (dipent.variant == V_h32)
                    || (dipent.variant == V_classical)
                    || (dipent.variant == V_chaos)) {
			dipent.xflags |= XF_BUILD_ANYCENTRES;
		}
		if (dipent.variant == V_aberration) dipent.xflags |= XF_BUILD_ONECENTRE;
	
	    case 11:
		dipent.max_absence_delay = 0;
		dipent.rrded = 1.000;
		dipent.orded = 0.000; /* Set ded settings for migrated games */

            case 12:
                if (dipent.variant == V_machiavelli)
                    dipent.xflags |= XF_COASTAL_CONVOYS;
		dipent.x2flags = 0;
	
	    case 13:
	        dipent.num_homes = 0;

	    case 14:
		dipent.x3flags = 0;
		dipent.wait = 0;
		dipent.powers = dipent.np;
		/* F_PROXY became F_INTIMATE */
	        if (dipent.flags & F_INTIMATE) {
		    dipent.flags &= ~F_INTIMATE;
		    dipent.x2flags |= X2F_PROXY;
		}
		if (dipent.flags & F_SPARE1) {
		    dipent.flags &= ~F_SPARE1;
		    dipent.x2flags |= X2F_NODIAS;
		}
		if (dipent.flags & F_SPARE2) {
		    dipent.flags &= ~F_SPARE2;
                    dipent.x2flags |= X2F_STRWAIT;
                }

	    case 15:
		dipent.powers = dipent.np;

	    case 16:
		/* All ok, exit out */
		break;  /* Dont forget to remove this if adding new cases! */

	
	    default:
		fprintf(stderr, "Bad header in master file (returned %d).\n%s\n", i ,line);
		bailout(E_FATAL);
	    
	}
	/* tempcentres will remember centres setting */
	old_flags = dipent.flags;
	old_xflags = dipent.xflags;
	old_x2flags = dipent.x2flags;
	old_x3flags = dipent.x3flags;
	SETNP(dipent.variant);
	dipent.xflags = old_xflags;
	dipent.x2flags = old_x2flags;
	dipent.x3flags = old_x3flags;
	dipent.flags = old_flags;
	dipent.has_natives = GetNativeIndex();

	if (tempvp != 0)
		dipent.vp = tempvp;

	if (dipent.powers == 0)
		dipent.powers = dipent.np;

	if (tempplayers != 0) {
		dipent.no_of_players = tempplayers;
	} else {
		dipent.no_of_players = dipent.powers;
	}

	while (fgets(line, sizeof(line), fp) && *line != '-') {
		switch (*line) {

		case 'A':
		case 'B':
			memset(&dipent.builds, 0, sizeof(sequence));
			if (getseq(stderr, line, &dipent.builds))
				bailout(E_FATAL);
			break;

		case 'C':
			line[strlen(line) - 1] = '\0';
			for (s = line; !isspace(*s); s++);
			while (isspace(*s))
				s++;
			strncpy(dipent.comment, s, sizeof(dipent.comment) - 1);
			break;

		case 'D':
			gettime(line, &dipent.deadline);
			break;

		case 'E':
			line[strlen(line) - 1] = '\0';
			for (s = line; !isspace(*s); s++);
			while (isspace(*s))
				s++;
			strncpy(dipent.epnum, s, sizeof(dipent.epnum) - 1);
			break;

		case 'G':
			gettime(line, &dipent.grace);
			break;

		case 'M':
			memset(&dipent.movement, 0, sizeof(sequence));
			if (getseq(stderr, line, &dipent.movement))
				bailout(E_FATAL);
			break;

		case 'N':
			line[strlen(line) - 1] = '\0';
			for (s = line; !isspace(*s); s++);
			while (isspace(*s))
				s++;
			strncpy(dipent.bn_mnnum, s, sizeof(dipent.bn_mnnum) - 1);
			break;

		case 'P':
			gettime(line, &dipent.process);
			break;

		case 'R':
			memset(&dipent.retreat, 0, sizeof(sequence));
			if (getseq(stderr, line, &dipent.retreat))
				bailout(E_FATAL);
			break;

		case 'S':
			gettime(line, &dipent.start);
			break;

		case 'Y':
			dipent.dedapplied = strtol( &(line[22]), NULL, 10);
			break;
		case 'O':
			sscanf( &(line[16]),"%f", &dipent.orded);
			break;

		case 'T':
			sscanf( &(line[26]),"%f", &dipent.rrded);
			break;

		case 'W':
			gettime(line, &dipent.wait);
			break;

		case '_':
			if (dipent.n > 0) {
				line[strlen(line) - 1] = '\0';
				for (s = line; !isspace(*s); s++);
				while (isspace(*s))
					s++;
				strcpy(dipent.players[dipent.n - 1].pref, s);
			}
			break;

		default:
			if (dipent.n >= MAXPLAYERS) {
				fprintf(stderr, "Too many players for game '%s'.\n", dipent.name);
				bailout(E_FATAL);
			}
			*dipent.players[dipent.n].pref = '\0';
			getplay(line, &dipent.players[dipent.n++]);
		}
	}
	if (Dflg > 1)
		fprintf(log_fp, "Getdipent returns: '%s'.\n", dipent.name);

	if (!strcmp(dipent.name, "control")) {
		time(&now);
		/* MLM 26/5/2001 only notify warp on shift more than TIME_TOLERANCE */
		if (dipent.process && (dipent.start > (now +TIME_TOLERANCE) || 
		    now > (TIME_TOLERANCE + dipent.process))) {
			fprintf(stderr, "Current date %24.24s should be between...\n", ctime(&now));
			if (!dip_time) {
			    fprintf(stderr, "Control dates %24.24s ", ctime(&dipent.start));
			    fprintf(stderr, "< %24.24s.\n", ctime(&dipent.process));
			}
			fprintf(stderr, "Time warp indicated.  GM notified.\n");
			sprintf(line, "/dev/null 'Diplomacy time warp'");
			MailOut(line, GAMES_MASTER);
			/* bailout(E_FATAL); */
			/* Try to fix time warp by advancing deadline */
			deadline(NULL, 1);
			time_warp = 1;
			DIPNOTICE("TimeWarp detected.");

			/* If recovering from bailout, notify the GAMES_MASTER */
           		if (bailout_recovery && !recover_print) {
                    	    sprintf(line, "/dev/null 'Bailout recovery initiated'");
			    MailOut(line, GAMES_MASTER);
                            recover_print = 1;
			}
			
		} else {
		    time_warp = 0;
		}
		dipent.start = now - 1;
		dipent.process = now + 168 * HRS2SECS;
		for (j = i = 0; i < dipent.n; i++)
			j += strlen(dipent.players[i].address) + 2;
		if ((s = notifies = malloc(j + 1))) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power == MASTER)
					*s++ = '+';
				strcpy(s, dipent.players[i].address);
				s += strlen(s) + 1;
			}
			*s = '\0';
		} else {
			notifies = "*";
		}

	} else {
	    /* Non control game, check for a time-warp or bailout recovery set */
                if ((time_warp || bailout_recovery) && dipent.phase[5] != 'A') {
                /* Try to fix the warp/recovery by adjusting deadline */
                /* Rather simplistic, but will do for now */
                    deadline(NULL,1);

                 /* Bug 110: Also, shift out the process date by 24 hours if sooner */
#define PROCESS_SHIFT (24*60*60)
                    if (dipent.process && (dipent.process < now + PROCESS_SHIFT))
                        dipent.process += PROCESS_SHIFT;
               }

        }
	return 1;
}

/***********************************************************************/

void putdipent(FILE * fp, int dopw)
{

/*
 * Write the current entry out to the master file. 
 */

	int i;
	char line[1000];

	fprintf(fp, "%-8.8s  %-8.8s  %-8.8s  %d %d %d %d %x %d %d %x %d %x %d %x %d\n", 
		dipent.name, dipent.seq,
		dipent.phase, dipent.access, dipent.variant,
		dipent.level, dipent.dedicate, dipent.flags, dipent.vp,
		dipent.no_of_players, dipent.xflags,
		dipent.max_absence_delay,
                dipent.x2flags, /* This indicates version 0.8.9 onwards */
		dipent.num_homes, /* This indicates version 1.1.1 onwards */
		dipent.x3flags,    /* This indicates version 1.7 onwards */
		dipent.powers	/* Also new for version 1.7 onwards */
		
	);

	if (dipent.process)
		fprintf(fp, "Process   %24.24s (%ld)\n",
			ctime(&dipent.process), dipent.process);
	if (dipent.deadline)
		fprintf(fp, "Deadline  %24.24s (%ld)\n",
			ctime(&dipent.deadline), dipent.deadline);
	if (dipent.start)
		fprintf(fp, "Start     %24.24s (%ld)\n",
			ctime(&dipent.start), dipent.start);
	if (dipent.grace)
		fprintf(fp, "Grace     %24.24s (%ld)\n",
			ctime(&dipent.grace), dipent.grace);
	if (dipent.wait)
		fprintf(fp, "Wait      %24.24s (%ld)\n",
				ctime(&dipent.wait), dipent.wait);

	fprintf(fp, "Ontime rat. min %.3f\n",dipent.orded);
        fprintf(fp, "T = resignation ratio max %.3f\n",dipent.rrded);
        fprintf (fp,"Yet_Applied_deadline? %d\n",
                                                dipent.dedapplied);
	if (*dipent.comment)
		fprintf(fp, "Comment   %s\n", dipent.comment);
	if (*dipent.epnum)
		fprintf(fp, "EP_number %s\n", dipent.epnum);
	if (*dipent.bn_mnnum)
		fprintf(fp, "Number_BM %s\n", dipent.bn_mnnum);

	putseq(fp, "Moves  ", &dipent.movement);
	putseq(fp, "Retreat", &dipent.retreat);
	putseq(fp, "Adjust ", &dipent.builds);

	for (i = 0; i < dipent.n; i++) {
		putplay(fp, &dipent.players[i], dopw);
	}
	if (fprintf(fp, "-\n") == 0) {
		fprintf(stderr, "Error writing to dip.master. Disk error suspected. Bailing out\n");
		sprintf(line, "/dev/null 'File error writing dip.master'");
		MailOut(line, GAMES_MASTER);
		bailout(E_FATAL);
	}
}

/***********************************************************************/

void newdipent(char *name, int variant)
{

/*
 * Build a new diplomacy master file entry. 
 */

	char dir[50];

	sprintf(dir, "%s%s", GAME_DIR, name);
	mkdir(dir, 0777);
	strncpy(dipent.name, name, sizeof(dipent.name));
	strcpy(dipent.seq, "x0");
	strcpy(dipent.phase, sphase[variant]);
	dipent.access = D_ACCESS;
	dipent.level = D_LEVEL;
	dipent.flags = D_FLAGS;
	dipent.xflags = D_XFLAGS;
	dipent.x2flags = D_X2FLAGS;
	dipent.x3flags = D_X3FLAGS;
	dipent.dedicate = D_DEDICATE;
	dipent.variant = variant;
	SETNP(variant);
	dipent.process = 0;
	dipent.deadline = 0;
	dipent.start = 0;
	dipent.grace = 0;
	dipent.wait = 0;
	dipent.movement.clock = D_MOVE_CLOCK;
	dipent.movement.mint = D_MOVE_MINT;
	dipent.movement.next = D_MOVE_NEXT;
	dipent.movement.grace = D_MOVE_GRACE;
	dipent.movement.delay = D_MOVE_DELAY;
	strcpy(dipent.movement.days, D_MOVE_DAYS);
	dipent.retreat.clock = D_RETREAT_CLOCK;
	dipent.retreat.mint = D_RETREAT_MINT;
	dipent.retreat.next = D_RETREAT_NEXT;
	dipent.retreat.grace = D_RETREAT_GRACE;
	dipent.retreat.delay = D_RETREAT_DELAY;
	strcpy(dipent.retreat.days, D_RETREAT_DAYS);
	dipent.builds.clock = D_BUILDS_CLOCK;
	dipent.builds.mint = D_BUILDS_MINT;
	dipent.builds.next = D_BUILDS_NEXT;
	dipent.builds.grace = D_BUILDS_GRACE;
	dipent.builds.delay = D_BUILDS_DELAY;
	strcpy(dipent.builds.days, D_BUILDS_DAYS);
	dipent.n = 0;
	dipent.orded = 0.000;
	dipent.rrded = 1.000;
	dipent.dedapplied = 0;
	dipent.no_of_players = dipent.np;
	dipent.powers = dipent.np;
	dipent.max_absence_delay = D_MAX_ABSENCE_DELAY;
        dipent.has_natives = GetNativeIndex();
}

/***********************************************************************/

void testdipent(int seq, int variant)
{

/*
 * Initialize the dipent structure for the test case. 
 */

	int i;
	long now;

	time(&now);

	newdipent("test", variant);
	sprintf(dipent.seq, "%03d", seq);
	strcpy(dipent.phase, "?1901?");

	dipent.process = now - 1;
	dipent.deadline = now - 1;
	dipent.start = now - 1;
	dipent.grace = now - 1;
	dipent.n = WILD_PLAYER;
	for (i = 0; i < dipent.n; i++) {
		dipent.players[i].power = i;
		dipent.players[i].status = 0;
		dipent.players[i].siteid = 0;
		dipent.players[i].userid = 0;
		dipent.players[i].late_count = 0;
		dipent.players[i].centres_blockaded = 0;
		strcpy(dipent.players[i].password, "spud");
		strcpy(dipent.players[i].address, "*");
	}
	dipent.players[0].power = power('o');
	strcpy(dipent.players[0].password, "spud");
	strcpy(dipent.players[0].address, GAMES_MASTER);
}

/***********************************************************************/

int getseq(FILE * fp, char *line, sequence * seq)
{

	static char *keys[] =
	{"", "clock", "c", "min", "m", "next", "n",
	 "grace", "g", "delay", "de", "days", "day", "da"};
	static char action[] =
	{'x', 'c', 'c', 'm', 'm', 'n', 'n',
	 'g', 'g', 'd', 'd', 'D', 'D', 'D'};
	int i;
	char *p, *s, *t, word[30];
	float f;

	for (s = line; !isspace(*s); s++);
	while (isspace(*s))
		s++;

	while (*s) {
		s = lookfor(s, keys, nentry(keys), &i);
		switch (action[i]) {
		case 'c':
			if (sscanf(s, "%d", &i) != 1 || i > 1440) {
				fprintf(fp, "%sBad clock specification.\n", line);
				return 1;
			}
			seq->clock = i;
			break;

		case 'm':
			if (sscanf(s, "%f", &f) != 1 || f < 0) {
				fprintf(fp, "%sBad minimum time specification.\n", line);
				return 1;
			}
			seq->mint = f;
			break;

		case 'n':
			if (sscanf(s, "%f", &f) != 1 || f < 0) {
				fprintf(fp, "%sBad next time specification.\n", line);
				return 1;
			}
			seq->next = f;
			break;

		case 'g':
			if (sscanf(s, "%f", &f) != 1 || f < 0) {
				fprintf(fp, "%sBad grace time specification.\n", line);
				return 1;
			}
			seq->grace = f;
			break;

		case 'd':
			if (sscanf(s, "%f", &f) != 1 || f < 0) {
				fprintf(fp, "%sBad delay time specification.\n", line);
				return 1;
			}
			seq->delay = f;
			break;

		case 'D':
			if (sscanf(s, "%10s", word) != 1) {
				fprintf(fp, "%s,Bad list of days specification.\n", line);
				return 1;
			}
			for (p = word, t = "SMTWTFS"; *t; t++, p++) {
				if (*p != *t && *p != tolower(*t) && *p != '-') {
					fprintf(fp, "%sBad list of days specified.\n", line);
					return 1;
				}
			}
			*p = '\0';
			strcpy(seq->days, word);
			if (!strcmp(seq->days, "-------")) {
				fprintf(fp, "%sAt least one day of the week must be allowed.\n", line);
				return 1;
			}
			break;

		default:
			sscanf(s, "%s", word);
			fprintf(fp, "Invalid keyword %s.\n", word);
			return 1;
			break;
		}

		while (*s && !isspace(*s))
			s++;
		while (isspace(*s))
			s++;
	}
	return 0;
}

/***********************************************************************/

void putseq(FILE * fp, char *s, sequence * seq)
{
	if (*seq->days)
		fprintf(fp,
			"%-9.9s clock %4d min %5.2f next %6.2f grace %6.2f delay %.2f days %s\n",
			s, seq->clock, seq->mint, seq->next, seq->grace, seq->delay, seq->days);
}

/***********************************************************************/

void gettime(char *line, long *time)
{

	char *s, *t;

	for (s = line; !isspace(*s); s++);
	while (isspace(*s))
		s++;

	for (t = s; *t && *t != '('; t++);
	if (*t == '(' && (*time = atol(t + 1)))
		return;

	if (jm(s, time)) {
		fprintf(stderr, "Error processing '%s' %s", dipent.name, line);
		bailout(E_FATAL);
	}
}

/***********************************************************************/

void getplay(char *line, Player * p)
{

	int i;
	char c;

	*p->pref = '\0';
	/* You'd better be really sure you've not messed with absence array limit! */
	i = sscanf(line, "%c%*s %x %d %d %d %d %s %s %d %d %d %ld %ld %ld %ld %ld %ld %ld %d", &c, &p->status,
		   &p->units, &p->centers, &p->userid, &p->siteid,
		   p->password, p->address, &p->late_count, &p->centres_blockaded,
		   &p->absence_count, 
		   &p->absence_start[0], &p->absence_end[0],
		   &p->absence_start[1], &p->absence_end[1],
		   &p->absence_start[2], &p->absence_end[2],
		   &p->absence_total, &p->controlling_power);
	switch (i)
	{
		case 8: {
		     /* versions prior to 0.8.7 which had no late count */
		     p->late_count = 0;  /* initialise it to something! */
		    }
		case 9: {
		     p->centres_blockaded = 0;
		}
		case 10: {
		     /* versions 0.8.7 and up are fine here! */
			p->absence_count = 0; 
                   	p->absence_start[0] = p->absence_end[0] = 0;
			p->absence_start[1] = p->absence_end[1] = 0; 
                        p->absence_start[2] = p->absence_end[2] = 0;
		    }
		case 17: {
			/* versions 0.8.7 and up are fine here! */
			p->absence_total = 0;
                    }
		case 18: {
			/* New for 1.7.0 onwards */
			p->controlling_power = 0;
		    }
		case 19: {
			/* OK, this is the latest one! */
			break;
		    }

		
		default: {
		    /* OK, we've got a problem: bailout */	
		fprintf(stderr, "Bad player entry for '%s'.  Only found %d items.\n%s\n",
			dipent.name, i, line);
		bailout(E_FATAL);
		}
	}
	/*
	 * The following 2 lines are to intended to cover the removal of the
	 * alternate from the judge code.  This code will automatically
	 * replace alternates that might still be in the dip.master file and
	 * make them observers. 
	 */
	if (c == '%')
		c = 'o';
	if (!(p->power = power(c))) {
		fprintf(stderr, "Invalid power character: %s\n", line);
		bailout(E_FATAL);
	}
}

void putplay(FILE * fp, Player * p, int dopw)
{
#define MAX_OUTPOWER 9
	char c;
/* Outpower will allow powers with spaces in names to be used */
	char out_power[MAX_OUTPOWER];
	int i;

	if (p->power >= 0) {
		strncpy(out_power, powers[p->power], MAX_OUTPOWER - 1);
		out_power[MAX_OUTPOWER-1] = '\0';
		for (i=0; out_power[i] != '\0' && i < MAX_OUTPOWER; i++) {
		    /* Substitute space with '_' to prevent crash on fscanf */
		    if (out_power[i] == ' ') out_power[i] = '_';
		}
		    
		if (isupper(c = dipent.pl[p->power]))
			c = tolower(c);
		fprintf(fp, "%c%-8s %4x %2d %2d %3d %5d %-12s %s %4d %2d %d %ld %ld %ld %ld %ld %ld %ld %d\n",
			c, &out_power[1],
		   p->status, p->units, p->centers, p->userid, p->siteid,
			dopw ? p->password : "xxx", p->address,p->late_count, p->centres_blockaded,
		   p->absence_count,
                   p->absence_start[0], p->absence_end[0],
                   p->absence_start[1], p->absence_end[1],
                   p->absence_start[2], p->absence_end[2],
		   p->absence_total, p->controlling_power);
		if (*(p->pref))
			fprintf(fp, "_pref: %s\n", p->pref);
	}
}

int countgames(void)
{
	FILE *mf;
	char line[1024];
	int gamecount = 0;
	int len;

	if (!(mf = fopen(MASTER_FILE, "r"))) {
		perror(MASTER_FILE);
	}
	do {
		fgets(line, sizeof(line), mf);
		if (feof(mf))
			break;
		/* TODO check for an error */
		len = strlen(line);
		if (len && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
		if (!strcmp("-", line)) {
			gamecount++;
		}
	} while (!feof(mf));

	fclose(mf);

	/* the control game will be counted by above */
	return gamecount - 1;
}
