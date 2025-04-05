/*
$Log: ascii2rec.c,v $
Revision 1.3  2004-09-17 18:28:47  nzmb
Fixed compile warning.

Revision 1.2  2003/12/08 18:47:52  millis
Removed compile warning

Revision 1.1  2003/03/05 02:42:09  nzmb
Added ascii2rec which will convert a text plyrdata record dump (generated
by recdump) into a binary plyrdata file.

*/

/*
Convert a recdump text.rec to binary plyrdata format.
A very quick hack by Tim Miller
*/

#include <stdio.h>
#include "plyrdata.h"

#define LINELEN 80

int main(int argc, char **argv)
{
	FILE *ifp;
	int put = 0;
	char type[LINELEN], line[LINELEN];
	unsigned long val, recno = 0, lineno = 0;
	PLYRDATA_RECORD rec;
	
	if(argc < 2)
	{
		puts("USAGE: ./ascii2rec infile");
		puts("Outfile will be called plyrdata");
		return 0;
	}

	if((ifp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Can't open %s.\n", argv[1]);
		return 0;
	}
	if(open_plyrdata() != 0)
	{
		fprintf(stderr, "Can't open plyrdata.\n");
		return 0;
	}

	while(fgets(line, sizeof(line), ifp) != NULL)
	{
		lineno++;
		put = 0;
		if(sscanf(line, "%s %lu", type, &val) < 2)
		{
			fprintf(stderr, "Error on line %lu.\n", lineno);
			break;
		}
		switch(type[0])
		{
			case 'R':
				if(type[2] == 'c')
					put = 1;
				else
					rec.resigned = put_long(val);
				break;
			case 'O':
				rec.ontime = put_long(val);
				break;
			case 'T':
				if(type[2] == 't')
					rec.total = put_long(val);
				else
					rec.tookover = put_long(val);
				break;
			case 'S':
				rec.started = put_long(val);
				break;
			default:
				fprintf(stderr, "Bad line: %s\n", line);
				break;
		}
		if(put == 1)
		{
			printf("putting record %lu.\n", recno);
			put_plyrdata_record(recno, &rec);
			recno = val;
		}
	}
	/* put the last record */
	if(put == 1)
        {
                printf("putting record %lu.\n", recno);
                put_plyrdata_record(recno, &rec);
        }


	fclose(ifp);
	close_plyrdata();
	return 0;
}
