/*
 * $Log: bailout.c,v $
 * Revision 1.5  2004-12-17 21:08:34  alange
 * Bug 391: Don't rename FORWARD once KEEPOUT file exists.
 *
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

#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>

#include "conf.h"
#include "config.h"
#include "dip.h"
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


	if (!options.debug) {
		if (!stat(conf_get("bailout_plan"), &sbuf))
			rename(conf_get("bailout_plan"), conf_get("plan"));
		inform_rgd();
	/* If bail_forward already exists, we've already crashed. No need to do again. */
		if (stat(conf_get("bail_forward"), &sbuf))
		{
			rename(conf_get("forward"), conf_get("bail_forward"));
			rename(conf_get("forward_onbail"), conf_get("forward"));
		}
		if(dolog)
		{
			fprintf(log_fp, "Bailout complete, %s renamed to %s.\n", conf_get("forward"),
					conf_get("bail_forward"));
		}
		sprintf(myline, "%s /dev/null 'Bailout executed in %s at %d.' '%s'",
			conf_get("cmd_smail"), sourcename, linenum, conf_get("judge_keeper"));
		system(myline);
		diplog_syslog_entry(LOG_ERR, myline);
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
	sprintf(info_line, "%s JUDGE: Bailout at %12.12s\n\n", conf_get("judge_code"),
		ctime(&now) + 4);

	/* Append the contents of the bailout.msg file (if any). */
	if (!stat(conf_get("bailout_msg"), &sbuf)) {
		if (!(ifp = fopen(conf_get("bailout_msg"), "r"))) {
			perror(conf_get("bailout_msg"));
			fprintf(log_fp, "Unable to open %s.\n", conf_get("bailout_msg"));
			return;
		}
		while (fgets(line, (int) sizeof(line), ifp))
			fputs(line, ofp);

		fclose(ifp);
	}
	fclose(ofp);

	sprintf(line, "%s dip.temp '%s' '%s'", conf_get("cmd_smail"), info_line, conf_get("rgd_gateway"));
	system(line);
}
