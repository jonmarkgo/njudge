/*
 * $Log$
 * Revision 1.3  2001/02/03 10:36:16  miller
 * properly hide history in blind game
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Restrict history for blind games (so that players cannot find out more than they should!)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.3  1996/11/10 14:22:47  rpaar
 * changed the "broad#" to "broad" in the keys[] array.
 * modified the return text to include newlines.
 *
 * Revision 1.2  1996/11/05 23:06:12  rpaar
 * Added function to process history as turns not dates
 */

/*  history.c -- Extract selected portions of the history archives.
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
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "dip.h"
#include "functions.h"
#include "mail.h"

static FILE *fp;
static int lcnt;
static struct stat sbuf;

/***************************************************************************/

/* Moded by R.Paar 27 May 1996
 */

/* Function: doturns

 * Input params: 
 *       int nbflag - flag whether notices should be included (1), or Not (0)
 *       char * against - the starting turn for exclusion zone
 *       char * lastturn - ending turn for exlusion zone
 *       char * GameName - name of the game we are currently processing.
 *
 * Output: 1 on success
 *
 * Error Checking: None.  
 *                 To keep it simple:
 *                   nbflag cannot be incorrectly passed.  (Should be a BOOL)
 *                   on incorrect 'against' parameter, copies whole archive (beware!)
 *                   on incorrect 'lastturn' parameter, copies only to 'against' turn.
 *
 * Objective: Copy a history archive such that starting on turn 'against'
 *   the archive is not copied.  Ends on turn 'lastturn'.
 *   If nbflag set, exclude _ALL_ notices.  (N.B. this includes draw notices)
 *
 * N.B:  I've hacked it so it will include draws in the returned history.  
 *
 * Q: Shouldn't Draws be given a "Diplomacy results" subject line?
 */

static int doturns(nbflag, against, lastturn, GameName)
int nbflag;
char *against;
char *lastturn;
char *GameName;
/* Should probably extract the const char *'s to somewhere more useful.
 * flag variable may be overkill, however, better than executing a strstr several
 * times per line. Def of flag values:
 *              0 - ok to print to output file.
 *              1 - Do not print to output file.
 *              2 - Found the exclusion zone start, used to determine looking for exclusion
 *                      end.
 */
{
	FILE *out = rfp;
	int flag = 0;
	static char Input[150];

	while (fgets(Input, sizeof(Input), fp) != NULL) {
		if (flag == 0) {
			if (strstr(Input, "Subject:") != NULL) {
				if ((!nbflag) && (strstr(Input, "notice") != NULL)) {
					flag = 1;
				} else if (strstr(Input, against) != NULL) {
					flag = 2;
				}
			}
		}
		if ((flag == 2) && strstr(Input, lastturn) != NULL) {
			flag = 0;
		}
		if ((flag == 1) && (strstr(Input, "declared a draw between") != NULL)) {
			fprintf(out, "Subject: Diplomacy notice: %s\n\n", GameName);
			flag = 0;
		}
		if (flag == 0) {
			fprintf(out, "%s", Input);
		}
		if ((flag < 2) && (strstr(Input, "Date:") != NULL) &&
		    (strstr(Input, "\(") != NULL)) {
			flag = 0;
		}
	}

	return 1;
}

/***************************************************************************/

static void hist_date(long *date, long *pos, int print)
{

	static char line[150];
	char *s;

	for (;;) {

		if (print) {
			fputs(line, rfp);
			if (!--lcnt) {
				return;
			}
		}
		*pos = ftell(fp);
		if (!fgets(line, sizeof(line), fp)) {
			if (print)
				lcnt = -1;
			*date = sbuf.st_mtime;
			return;
		}
		if (!strncmp(line, "Date: ", 6) && (s = strchr(line, '('))) {
			*date = atol(s + 1);
			return;
		}
	}
}

/***************************************************************************/

int history(char *line, int power_type)
{

	/* 
	 * power_type is the type of the plyer requesting history
         * Set to OBSERVER if power type is unknown
         */
	
	/*
	 *  The input line consists of [name] [from date] [to date] [lines n].
	 *            or: [name] exclstart turn [exclend turn] [broad].
	 * 
	 *            'broad' option is default for exstart parameter to
	 *                        eliminate potentially long histories from being
	 *                        returned.  Users must specifically ask for notices to
	 *                        to be included if they so desire.
	 */

	/* Rp - added int flagb to flag incl/excl Broadcasts.  flagt = time/turns */
	int i, not_eof, flagb, flagt;
	char *s, name[sizeof(dipent.name)], file[sizeof(name) + 20];
	char exstart[10], exend[10];	/* 10 characters should be plenty */
	long s_date, s_pos, e_date, e_pos, n_date, n_pos, from, pos,
	 to;

	/* Rp - added exclstart#, exclend#, nb# */
	static char *keys[] =
	{"", "from#", "to#", "until#", "lines#", "for#",
	 "exclstart#", "exclend#", "broad"};
	static char value[] =
	{'x', 'f', 't', 't', 'l', 'l', 's', 'e', 'n'};

	*name = '\0';
	/* Rp - init char *s */
	*exstart = '\0';
	*exend = '\0';

	s = line;

	time(&to);
	from = to - 168 * 60 * 60;
	lcnt = 1000;
	flagb = 0;		/* Rp - define flagb = 1 include broadcast, 0 excluded */
	flagt = 1;		/* Rp - define flagt = 1 do time-wise, define flagt = 0 as exclude */


	while (isspace(*s))
		s++;

	while (*s) {
		s = lookfor(s, keys, nentry(keys), &i);

		switch (value[i]) {
		case 'x':
			if (*name) {
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				fprintf(rfp, "history %s", line);
				fprintf(rfp, "Unrecognized keyword: %s", s);
				return 1;
			}
			while (isspace(*s))
				s++;
			for (i = 0; *s && !isspace(*s) && i < sizeof(name) - 1; i++) {
				/* TODO this should allow case insensitive history commands */
				/* double check that the tolower has that effect */
				name[i] = tolower(*s);
				s++;
			}
			name[i] = '\0';
			break;

		case 'f':
			if (mail_date(&s, &from, 1, rfp)) {
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				fprintf(rfp, "history %s", line);
				fprintf(rfp, "Unparsable from date: %s", s);
				return 1;
			}
			break;

		case 't':
			if (mail_date(&s, &to, 1, rfp)) {
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				fprintf(rfp, "history %s", line);
				fprintf(rfp, "Unparsable to date: %s", s);
				return 1;
			}
			break;

		case 'l':
			while (isspace(*s))
				s++;
			for (lcnt = 0; isdigit(*s); lcnt = lcnt * 10 + *s++ - '0');
			if (lcnt == 0) {
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				fprintf(rfp, "history %s", line);
				fprintf(rfp, "Unparsable line count: %s", s);
				return 1;
			}
			break;
			/* Rp - init vars if found on History line */
		case 's':{
				flagt = 0;
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				while (isspace(*s))
					s++;
				/* if (copycheck( s, exstart)) {} */
				for (i = 0; *s && !isspace(*s) && i < sizeof(exstart) - 1; i++) {
					exstart[i] = *s++;
				}
				exstart[i] = '\0';
			}
			break;

		case 'e':{
				if ((!msg_header_done) && (!signedon))
					msg_header(rfp);
				while (isspace(*s))
					s++;
				/* if (copycheck( s, exend)) {} */
				for (i = 0; *s && !isspace(*s) && i < sizeof(exend) - 1; i++) {
					exend[i] = *s++;
				}
				exend[i] = '\0';
			}
			break;

		case 'n':
			if ((!msg_header_done) && (!signedon))
				msg_header(rfp);
			flagt = 0;
			flagb = 1;
			break;
		}

		while (isspace(*s))
			s++;

	}

	/*
	 *  Now read that archive file.
	 */

	if (!*name) {
		if (!signedon) {
			if (!msg_header_done)
				msg_header(rfp);
			fprintf(rfp, "history %s", line);
			fprintf(rfp, "Game name not specified.\n");
			return 1;
		}
		strcpy(name, dipent.name);
	} else {
		if (!signedon) {
			if ((mfp = fopen(MASTER_FILE, "r")) == NULL) {
				if (!msg_header_done)
					msg_header(rfp);
				fprintf(rfp, "Error opening master file %s.\n", MASTER_FILE);
				return 1;
			}
			while ((not_eof = getdipent(mfp))) {
				if (!strcmp(dipent.name, name)) {
					break;
				}
			}
			fclose(mfp);
		}
	}

	sprintf(file, "D%s/archive", name);
	if (stat(file, &sbuf)) {
		if ((!msg_header_done) && (!signedon))
			msg_header(rfp);
		fprintf(rfp, "history %s", line);
		fprintf(rfp, "No history available for game '%s'.\n", name);
		return 1;
	}

        /* Check if a blind game - if so, only the master can do this */
        if (dipent.flags & F_BLIND ) {
                if (dipent.phase[6] != 'X' ) {
                    /* Non-finished blind games have restrictions on history */
                    switch (power_type)
                    {
			case 0: /* not signed on */
                        case OBSERVER:
                                fprintf(rfp,"Observers are not allowed history command in blind games.\n");
                                return 0;
                        case MASTER:
                                break; /* No problem for the master! */

                        default:
                                fprintf(rfp, "Players are not allowed history command in blind games.\nContact the master for help in this.\n");
                                return 0;
                    }
               }
        }

	if (!msg_header_done)
		msg_header(rfp);
	fprintf(rfp, "History information for game '%s' ", name);
	if (flagt) {
		fprintf(rfp, "from %s\n", ptime(&from));
		fprintf(rfp, "through %s:\n", ptime(&to));
	} else {
		fprintf(rfp, "excluding turn '%s'\n", exstart);
		fprintf(rfp, "to turn '%s'.", ((*exend != '\0') ? exend : "end of game"));
	}
	fprintf(rfp, "\n");

	/*
	 *  Open the archive file and determine its start time.
	 */

	if (!(fp = fopen(file, "r"))) {
		fprintf(rfp, "Unable to open history file for '%s'.\n", name);
		return 1;
	}
	/* Rp - if (flagt) do search by date */
	if (flagt) {
		hist_date(&s_date, &s_pos, 0);

		e_date = sbuf.st_mtime;
		e_pos = sbuf.st_size;

		/*
		 *  Now do a binary search to find the intended start time.
		 */

		while (s_date < from && from < e_date && e_pos - s_pos > 2048) {
			if (Dflg)
				printf("s = %24.24s / %ld.\n", ctime(&s_date), s_pos);
			if (Dflg)
				printf("e = %24.24s / %ld.\n", ctime(&e_date), e_pos);
			pos = s_pos + (long) (((float) (e_pos - s_pos) *
					       (float) (from - s_date)) /
					      (float) (e_date - s_date));
			fseek(fp, pos, 0);
			hist_date(&n_date, &n_pos, 0);
			if (Dflg)
				printf("n = %24.24s / %ld from %ld.\n", ctime(&n_date), n_pos, pos);
			if (n_date >= from) {
				e_date = n_date;
				e_pos = pos;
			} else {
				s_date = n_date;
				s_pos = n_pos;
			}
		}

		/*
		 *  Okay, close enuf.
		 */

		if (from > e_date) {
			fprintf(rfp, "No history for specified period.\n");
		} else {

			fseek(fp, s_pos, 0);
			do {
				hist_date(&s_date, &s_pos, 0);
				if (Dflg)
					printf("Skipped to %24.24s at %ld.\n", ctime(&s_date), s_pos);
			} while (s_date < from);

			do {
				hist_date(&s_date, &s_pos, 1);
				if (Dflg)
					printf("Printed to %24.24s at %ld.\n", ctime(&s_date), s_pos);
			} while (s_date < to && lcnt > 0);

			fprintf(rfp, "\n---- History terminated due to %s.\n",
				lcnt == 0 ? "line limit exceeded" :
				lcnt == -1 ? "end of information" :
				"time range exhausted");
		}
	}
	/* end if (flagt) */
	else {
		if (doturns(flagb, exstart, exend, name)) {
			fprintf(rfp, "\n---- History terminated due to end of infomation.\n");
		}
	}

	fclose(fp);
	return 0;
}
