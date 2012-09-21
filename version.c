/*
 * this file gutted by nw for njudge 0.8.2.  I don't think any serious
 * copyright claim can be made to what's left.  Thus this file is
 * in the public domain.
 */

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "dip.h"
#include "functions.h"

void version(FILE * rfp)
{
	int ngames, mgames, muser;
	ngames  = countgames();
	muser = MAXUSER;
	mgames = MAXGAMES;

	fprintf(rfp, "Current version is %s %s\n\n", PACKAGE, VERSION);
	fprintf(rfp, "%d users registered, maximum is %d\n", nded, muser);
	fprintf(rfp, "%d games running, maximum is %d\n", ngames, mgames);
	fprintf(rfp,"\n");
} 
