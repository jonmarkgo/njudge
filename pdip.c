/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdio.h>
#include <string.h>

/*
 *  Read a mail file pass each message to dip.  Used when the judge has
 *  bailed out and mail has stacked up in the spool queue.
 */

char line[1024];

int main(int argc, char **argv)
{
	FILE *fp;

	fp = NULL;
	while (fgets(line, sizeof(line), stdin)) {
		if (!strncmp(line, "From ", 5)) {
			if (fp) {
				pclose(fp);
				fp = NULL;
			}
			if (!(fp = popen("dip -q", "w"))) {
				perror("popen");
				exit(1);
			}
			fputs(line, stdout);

		}
		if (!fp) {
			fprintf(stderr, "First line doesn't look like From.\n");
			fputs(line, stderr);
			exit(1);
		}
		fputs(line, fp);

	}

	if (fp)
		pclose(fp);

	return 0;

}
