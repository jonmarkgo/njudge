/*
 * $Log$
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "functions.h"
#include "port.h"

/* TODO rewrite this in perl */

/*
 *  Run through the whois file and straighten it out so it looks nice.
 */

char line[256];

int sflg = 0, showsite = -1;

int main(int argc, char **argv)
{

	int i, n, fd;
	char *s, *t;
	FILE *fp1, *fp2;


	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			for (s = argv[i] + 1; *s; s++) {
				switch (*s) {
				case 's':
					sflg++;
					break;

				default:
					fprintf(stderr, "Usage: %s [-s]\n", *argv);
					fprintf(stderr, "  -s: Show all site codes.\n");
					exit(1);
				}
			}
		} else {
			fprintf(stderr, "Usage: %s [-s]\n", *argv);
			fprintf(stderr, "  -s: Show all site codes.\n");
			exit(1);
		}
	}


	if ((fd = open("dip.log", O_RDWR)) < 0) {
		perror("dip.log");
		exit(1);
	}
	if (lockfd(fd, 0)) {
		perror("dip.log");
		exit(1);
	}
	if (!(fp1 = fopen("dip.whois", "r"))) {
		perror("dip.whois");
		exit(1);
	}
	if (!(fp2 = fopen("dip.whois.new", "w"))) {
		perror("dip.whois.new");
		exit(1);
	}
	while (fgets(line, sizeof(line), fp1)) {
		for (s = line + strlen(line) - 1; isspace(*s); s--);
		*++s = '\n';
		*++s = '\0';
		for (n = 0, s = t = line; *s && *s != ':'; s++, n++);
		if (strncmp(line, "User", 4) && *s) {
			if (showsite != -1 && !strncasecmp(line, "Site", 4)) {
				fprintf(stdout, "%8d %s", showsite, line);
				showsite = -1;
			}
			*s++ = '\0';
			for (t = line; isspace(*t); t++, n--);
			if (islower(*t))
				*t = toupper(*t);
			fprintf(fp2, "%s:", t);
			while (isspace(*s))
				s++;
			if (*s) {
				if (n > 10)
					n = 9;
				fprintf(fp2, "%.*s%s", 10 - n, "          ", s);
			} else {
				fputc('\n', fp2);
			}
		} else {
			if ((showsite = atoi(line + 10)) % 100 == 0) {
				fputs(line, stderr);
			} else if (!sflg) {
				showsite = -1;
			}
			fputs(line, fp2);
		}
	}

	fclose(fp1);
	if (ferror(fp2)) {
		fprintf(stderr, "Error writing new whois file.\n");
		exit(1);
	}
	fclose(fp2);

	rename("dip.whois.new", "dip.whois");

	return 0;
}
