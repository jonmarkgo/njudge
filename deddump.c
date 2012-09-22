/*
 * $Log: deddump.c,v $
 * Revision 1.5  2003-09-14 22:08:16  jaldhar
 * Included fcntl.h for symbols not defined in unistd.h
 *
 * Revision 1.4  2002/08/27 22:27:48  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.3  2001/05/12 08:05:28  greg
 * added Mario Becroft's dedication bug fix
 *
 * Revision 1.2  2001/05/07 04:30:49  greg
 * minor bug fix
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  deddump.c
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

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "conf.h"
#include "config.h"
#include "dip.h"

/*
 *  Dump/load the binary dedication file to/from a text file.
 */

static int lflg = 0;

static char *addr[MAXUSER];

int main(int argc, char **argv)
{

	int i, j, k, n, fd;
	int whosiz;
	char *s, *whobuf;
	FILE *fp;


	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			for (s = argv[i] + 1; *s; s++) {
				switch (*s) {
				case 'l':
					lflg++;
					break;

				default:
					fprintf(stderr, "Unrecognized flag '%c'.\n", *s);
					fprintf(stderr, "Usage %s [-l]\n", argv[0]);
				}
			}
		}
	}

/* Interlock the log file to ensure single threading  */

	if (lflg) {
		if ((fd = open(conf_get("log_file"), O_RDWR | O_APPEND | O_CREAT, 0600)) < 0) {
			perror(conf_get("log_file"));
			exit(E_FATAL);
		}
		if (lockfd(fd, 0)) {
			perror("FLOCKF");
			exit(E_FATAL);
		}
		if (!(log_fp = fdopen(fd, "a"))) {
			perror("fdopen");
			exit(E_FATAL);
		}
		fprintf(stderr, "Not prepared to do load option yet.\n");

	} else {

		if ((fd = open("dip.ded", O_RDONLY)) < 0) {
			perror("dip.ded");
			exit(1);
		} else {
			nded = read(fd, ded, MAXUSER * sizeof(ded[0])) /sizeof(ded[0]);
			close(fd);
		}

		fprintf(stdout, "Got %d users from binary dedication file.\n", nded);

		if ((fd = open("dip.whois", O_RDONLY)) < 0) {
			perror("dip.whois");
			exit(1);
		} else {
			struct stat sbuf;

			/*
			   **  Allocate an appropriately sized buffer and suck the whois file in.
			 */

			if (fstat(fd, &sbuf)) {
				perror("fstat");
				exit(1);
			}
			whosiz = sbuf.st_size;
			if (!(whobuf = (char *) malloc(whosiz + 100))) {
				fprintf(stderr, "Malloc failure.\n");
				exit(1);
			}
			if ((n = read(fd, whobuf, whosiz)) != whosiz) {
				fprintf(stderr, "Got %d of %d on who read.\n", n, whosiz);
				perror("read");
				exit(1);
			}
			whobuf[whosiz - 1] = '\0';

			fprintf(stdout, "Got %d bytes from whois file.\n", whosiz);

			for (s = whobuf; *s; s++) {
				while (*s && strncmp(s, "User:", 5)) {
					do {
						s++;
					} while (*s && *s != 'U');
				}
				if (!*s)
					break;

				if (s != whobuf)
					*(s - 1) = '\0';
				i = atoi(s + 5);
				if (i <= 0 || MAXUSER < i) {
					*(s + 100) = '\0';
					fprintf(stderr, "Bad user:\n%s\n", s);
					exit(1);
				}
				if (i > nded) {
					nded = i + 1;
					sscanf(s + 5, "%d%d%d", &i, &j, &k);
					ded[i].r = k;
				}
				addr[i] = s;
				do {
					s++;
				} while (*s && *s != 'U');
			}

			if (!(fp = fopen("text.ded", "w"))) {
				perror("text.ded");
				exit(1);
			}
			for (i = 0; i <= nded; i++) {
				fprintf(fp, "USER: %d %d %d %d %d\n", i,
					ded[i].r, ded[i].i1, ded[i].i1, ded[i].i2);
				fprintf(fp, "SIGN: %s", ctime(&ded[i].ls));
				fprintf(fp, "MOTD: %s", ctime(&ded[i].md));
				fprintf(fp, "DAT0: %s", ctime(&ded[i].d0));
				if (addr[i]) {
					fputs(addr[i], fp);
					fputc('\n', fp);
				}
			}
		}
	}
	return 0;
}
