/*
 * $Log$
 * Revision 1.4  2004/06/16 07:54:49  nzmb
 * A couple of changes to improve the interaction between rdip and bailout, and
 * make rdip a bit more fault tolerant:
 * (1) rdip now handles SIGPIPE and bails the judge out gracefully should it
 *     occur (previously, things died in mysterious and unpleasant ways).
 * (2) Fixed segfaults that occured when rdip calls bailout.
 *
 * Revision 1.3  2002/08/27 22:27:44  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Small change to log bailout error before bailing out
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*
 *  bailout.c
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
#include <time.h>
#include "config.h"
#include "dip.h"
#include <sys/stat.h>
#include "diplog.h"

extern char line[1024];
extern int Dflg;

void inform_rgd(void);

/****************************************************************************/

void real_bailout(int level, char *sourcename, int linenum, int dolog)
{
	/*
	 * Bailout.  We have detected some sort of fatal error that will prevent
	 * things from working properly.  The first thing we do is to get rid
	 * of our .forward file so that we don't enundate the world with bogus
	 * mail messages.
	 */

	char myline[150];
	/*extern int Dflg;*/
	struct stat sbuf;


	if (!Dflg) {
		if (!stat(BAILOUT_PLAN, &sbuf))
			rename(BAILOUT_PLAN, PLAN);
		inform_rgd();
	/* If KEEPOUT already exists, we've already crashed. No need to do again. */
		if (stat(KEEPOUT, &sbuf))
		{
			rename(FORWARD, KEEPOUT);
			rename(YFORWARD, FORWARD);
		}
		if(dolog)
		{
			fprintf(log_fp, "Bailout complete, %s renamed to %s.\n", FORWARD,
				KEEPOUT);
		}
		sprintf(myline, "%s /dev/null 'Bailout executed in %s at %d.' '%s'",
			SMAIL_CMD, sourcename, linenum, GAMES_MASTER);
		system(myline);
		DIPERROR(myline);
	}
	exit(level);
}

/*
 * This isn't a bad idea, but it may be better to inform the judge-maint
 * list instead
 */

void inform_rgd(void)
{
	/*
	 * Send mail to the dipl-l gateway with a message stating the date and
	 * time that we bailed out.
	 */
	char info_line[100];
	struct stat sbuf;
	time_t now;
	FILE *ofp, *ifp;

	/* Open the temporary file. */
	if (!(ofp = fopen("dip.temp", "w"))) {
		perror("dip.temp");
		fprintf(log_fp, "Unable to open temporary file.\n");
		return;
	}
	time(&now);
	sprintf(info_line, "%s JUDGE: Bailout at %12.12s\n\n", JUDGE_CODE,
		ctime(&now) + 4);

	/* Append the contents of the bailout.msg file (if any). */
	if (!stat(BAILOUT_MSG, &sbuf)) {
		if (!(ifp = fopen(BAILOUT_MSG, "r"))) {
			perror(BAILOUT_MSG);
			fprintf(log_fp, "Unable to open %s.\n", BAILOUT_MSG);
			return;
		}
		while (fgets(line, (int) sizeof(line), ifp))
			fputs(line, ofp);

		fclose(ifp);
	}
	fclose(ofp);

	sprintf(line, "%s dip.temp '%s' '%s'", SMAIL_CMD, info_line, RGD_GATEWAY);
	system(line);
}
