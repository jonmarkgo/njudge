/*
 * Revision 1.1  2002/04/11 23:49:00  miller
 * First draft.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "config.h"
#include "dip.h"

struct dedication ded[MAXUSER];

int main(int argc, char** argv) {

	gint  idx;
	gint  records;
	FILE* infp;
	FILE* outfp;
	struct dedication ded;

	if (argc < 3) {
		fprintf(stderr, "USAGE: %s infile outfile\n", argv[0]);
		return 1;
	}

	if ((infp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "error opening %s - %s\n", argv[1], strerror(errno));
		return 1;
	}
	if ((outfp = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "error opening %s - %s\n", argv[2], strerror(errno));
		return 1;
	}

	fscanf(infp, "%d\n", &records);

	printf("Converting %d records\n", records);

    for (idx = 0; idx < records; idx ++) {
    	printf("%u\r", idx);
		if (6 != fscanf(infp, "%hd %hd %hd %ld %ld %ld\n",
				&ded.r, &ded.i1, &ded.i2, &ded.ls, &ded.md, &ded.d0)) {
			fprintf(stderr, "error reading record %u - %s\n", idx + 1, strerror(errno));
			return 1;
		}
		if (!fwrite(&ded, sizeof ded, 1, outfp)) {
			fprintf(stderr, "error writing record %u - %s\n", idx + 1, strerror(errno));
			return 1;
		}

    }

    fclose(infp);
    fclose(outfp);

	printf("all done without problems\n");

	return 0;

}
