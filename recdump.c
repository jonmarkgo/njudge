/*
 * Recdump.c -- dumps player records into an output file -- also allows
 * user to modify a particular record.
 *
 * Created 2001 - T. Miller and M. Becroft.
 */

/*
$ID$
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "plyrdata.h"

void writetodata(void);

int main(int argc, char *argv[])
{
	int fd,i;
	char *s;
	FILE *fp;
	PLYRDATA_RECORD rec;

	if(argc == 1)
	{
		/* The user just wants a nice pretty text output. */
		if((fd = open_plyrdata()) != 0)
		{
			printf("Error opening plyrdata file\n");
			return 1;
		} else {
			if(!(fp = fopen("text.rec", "w")))
			{
				printf("Can't open text.rec file\n");
				return 1;
			}
			for(i = 0;; i++)
			{
				if(get_plyrdata_record(i,&rec) == 0)
				{
					printf("Got %d records\n",i);
					close_plyrdata();
                        		fclose(fp);
					return 0;
				} else {
					fprintf(fp,"Record %d\n",i);
					fprintf(fp,"Ontime %lu\n",get_long(rec.ontime));
					fprintf(fp,"Total %lu\n",get_long(rec.total));
					fprintf(fp,"Started %lu\n",get_long(rec.started));
					fprintf(fp,"Tookover %lu\n",get_long(rec.tookover));
					fprintf(fp,"Resigned %lu\n",get_long(rec.resigned));
				}
			}
		}
	}
	for(i = 1; i < argc; i++)
	{
		if(*argv[i] == '-')			
		{
			for(s = argv[1] + 1; *s; s++)
			{
				switch(*s)
				{
					case 'w':
						/* User wants to write to
						  the file. */
						writetodata();
						break;
					default:
						printf("Unrecognized flag '%c'\n",*s);
						return 1;
						break;
				}
			}
		}
	}
	/* Nothing else to do, might as well return an error. */
	return 1;
	
}
void writetodata(void)
{
	PLYRDATA_RECORD rec;
	int a,b,c,d;
	if(open_plyrdata() != 0)
	{
		printf("Unable to open plyrdata file.\n");
		exit(0);
	}
	printf("Which record do you want to modify?\n");
	scanf("%d", &a);

	/* Print the record for the user. */
	if (get_plyrdata_record( a, &rec ) < 1)
        {
                printf( "Could not read record #%d\n", a);
		if(a > 50000)
		{
			printf("%d is very large; really create this?\n",a);
			printf("Type 0 for no, all else for yes.\n");
			scanf("%d", &d);
			if(d == 0) { 
				exit(0);
			}
		}
		printf("Creating record.\n");
		memset(&rec,0,sizeof(PLYRDATA_RECORD));
        }
        else
        {
                printf( "Record #%d\n"
                        "Ontime = %lu\n"
                        "Total = %lu\n"
                        "Started = %lu\n"
                        "Resigned = %lu\n"
                        "Tookover = %lu\n",
                        a, get_long(rec.ontime), get_long(rec.total),
                        get_long(rec.started), get_long(rec.resigned),
                        get_long(rec.tookover));
        }               
	printf("Choose a field to modify, 1 for ontime, 2 for total, 3 for\n");
	printf("started, 4 for resigned, or 5 for tookover.\n");
	scanf("%d", &b);
	printf("What should the new value for this field be?\n");
	scanf("%d", &c);

	switch(b)
	{
		case 1:
			rec.ontime = put_long(c);
			break;

		case 2:
			rec.total = put_long(c);
			break;

		case 3:
			rec.started = put_long(c);
			break;
		
		case 4:
			rec.resigned = put_long(c);
			break;

		case 5:
			rec.tookover = put_long(c);
			break;

		default:
			printf("Invalid selection for field to modify.\n");
			exit(0);
	}
	put_plyrdata_record(a, &rec);
	if (get_plyrdata_record( a, &rec ) < 1)
        {
                printf( "Could not read record #%d\n", a);
        }
        else
        {
                printf( "Record #%d\n"
                        "Ontime = %lu\n"
                        "Total = %lu\n"
                        "Started = %lu\n"
                        "Resigned = %lu\n"
                        "Tookover = %lu\n",
                        a, get_long(rec.ontime), get_long(rec.total),
                        get_long(rec.started), get_long(rec.resigned),
                        get_long(rec.tookover) );
        }               
	close_plyrdata();
}
