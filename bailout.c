/*
 * $Log$
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
#include "dip.h"
#include <sys/stat.h>

extern char *line;
extern int Dflg;

/****************************************************************************/

void bailout(int level)
{
	/*
	 * Bailout.  We have detected some sort of fatal error that will prevent
	 * things from working properly.  The first thing we do is to get rid
	 * of our .forward file so that we don't enundate the world with bogus
	 * mail messages.
	 */

	char line[150];
	extern int Dflg;
	struct stat sbuf;


	if (!Dflg) {
		if (!stat(BAILOUT_PLAN, &sbuf))
			rename(BAILOUT_PLAN, PLAN);
		/* inform_rgd(); */
		rename(FORWARD, KEEPOUT);
		rename(YFORWARD, FORWARD);
		fprintf(log_fp, "Bailout complete, %s renamed to %s.\n", FORWARD,
			KEEPOUT);
		sprintf(line, "./smail /dev/null 'Bailout executed.' '%s'",
			GAMES_MASTER);
		system(line);
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
	long now;
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
		while (fgets(line, sizeof(line), ifp))
			fputs(line, ofp);

		fclose(ifp);
	}
	fclose(ofp);

	sprintf(line, "./smail dip.temp '%s' '%s'", info_line, RGD_GATEWAY);
	system(line);
}
