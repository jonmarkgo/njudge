/*
 * this file gutted by nw for njudge 0.8.2.  I don't think any serious
 * copyright claim can be made to what's left.  Thus this file is
 * in the public domain.
 */

#include <stdio.h>

#include "dip.h"

void version(FILE * rfp)
{
	fprintf(rfp, "Current version is njudge %s\n\n", JVERSION);
}
