/*
   ** $Log$
   ** Revision 1.1  1998/02/28 17:49:42  david
   ** Initial revision
   **
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */


#include <stdio.h>
#include "port.h"
#include "dip.h"

int main(int argc, char *argv[])
{
	FILE *ifd, *ofd;
	char line[255], name[255];
	int nameline, delthis, found;

	if (argc != 2) {
		printf("Usage: delgame <game>\n");
		exit(1);
	}
	++argv;
	nameline = 0;
	delthis = 0;
	found = 0;
	ifd = fopen("dip.master", "r");
	ofd = fopen("dip.master.admin", "w");
	printf("Deleting game : %s\n", *argv);
	while (!feof(ifd)) {
		fscanf(ifd, "%[^\n]%*[\n]", line);
		strcpy(name, "\0");
		if (nameline) {
			sscanf(line, "%s", name);
		}
		if (strcmp(line, "-") == 0) {
			nameline = 1;
		}
		if (nameline && (strcmp(name, *argv) == 0)) {
			delthis = 1;
			found = 1;
		}
		if (strcmp(line, "-") != 0)
			nameline = 0;
		if (!delthis)
			fprintf(ofd, "%s\n", line);
		if (strcmp(line, "-") == 0) {
			delthis = 0;
		}
	}
	fclose(ifd);
	fclose(ofd);
	if (found) {
		rename("dip.master", "dip.master.old");
		rename("dip.master.admin", "dip.master");
		printf("Game: %s removed.\n", *argv);
		printf("  To make the removal complete, please type :\n");
		printf("     rm -rf D%s\n", *argv);
		printf("  (Change the directory as appropriate - \n");
		printf("    see dip.conf for the current setting.)\n\n");
	} else
		printf("Game: %s not found.\n", *argv);

	return 0;
}
