/*
$LOG$ 
 */

/*
 * tm_xpress.c -- Tim Miller's extended press functionality. Will include
 * EOG diarys and szine style press published at deadline.
 */

#include <stdio.h>

#include "dip.h"
#include "mail.h"
#include "functions.h"

int process_ppress(void)
{
	char pfilename[1000];
	char b;
	
	/* see if this is even allowed */
	if(!(dipent.x2flags & X2F_POSTALPRESS))
	{
		fprintf(rfp,"Postal press is not allowed. Skipping:\n");
		ppress_skip = 1;
		return 1;
	}

	/* this is cool -- open up the press file */
	sprintf(pfilename,"%s%s/ppress-%s", GAME_DIR, dipent.name, dipent.phase);
	if((ppfp = fopen(pfilename, "a")) == NULL)
	{
		fprintf(log_fp, "Error opening ppress file %s.\n", pfilename);
		bailout(E_FATAL);
	}

	ppress_read = 1;
	broadcast_master_only = 1;
	b = dipent.pl[dipent.players[player].power];
	fprintf(mbfp,"%s as %c in %s submitted press:\n", raddr,
		b, dipent.name);
	fprintf(ppfp,"\nPress from %c:\n", b);
	fprintf(rfp, "\nAppending to press file:\n");
	sprintf(subjectline, "%s:%s - %s postal press recorded", JUDGE_CODE,
		dipent.name, dipent.phase);
	return 0;
}
