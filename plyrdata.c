/*
 * plyrdata.c. Experimental file to log player statistics on the Judge
 * for a new ratio based dedication system and other monitoring.
 * Created first by Tim Miller, Feb 16, 2001. Preliminary structure
 * completed on Mar 3, 2001.
 *
 * Coding by Tim Miller and Mario Becroft.
 */
/*
$ID$
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "plyrdata.h"

int errno;

PLYRDATA_HEADER header;

static FILE *fp;

FILE *create_plyrdata( void );

int open_plyrdata( void ) {
	/* Open the xPlyrData file. */
	
	if (fp)
	{
		fprintf(stderr, "Opening already open xplyrdata file.\n");
	}
	
	fp = fopen(PLYRDATA_FILENAME, "r+");

	if (!fp) {
		if (errno == EACCES || errno == ENOENT) {
			if (!(fp = create_plyrdata())) {
				fprintf(stderr, "Error %d creating xplyrdata.\n", errno);
				/*bailout(E_FATAL);*/
				return 1;
			}
		}
		else {
			fprintf(stderr, "Error opening, xPlyrData file");
			/*bailout(E_FATAL);*/
			return 1;
		}
	}

	if (fread( &header, sizeof( PLYRDATA_HEADER ), 1, fp ) < 1)  
	{
        	fprintf(stderr, "plyrdata header too short.\n");
		/*bailout(E_FATAL);*/
		return 1;
	}
	if (
		header.magic != PLYRDATA_MAGIC ||
		header.version > PLYRDATA_VERSION ||
		header.record_size > 400 ||
		header.header_size > 400 ) {
		fprintf(stderr, "plyrdata header invalid.\n");
		/*bailout(E_FATAL);*/
		return 1;
	}
	return 0;
}

/********************************************************************/
FILE *create_plyrdata( void ) {
	FILE *f;
	
	f = fopen(PLYRDATA_FILENAME, "w+");
	
	if (f) {
		memset(&header, 0, sizeof(PLYRDATA_HEADER));
		header.magic = put_long(PLYRDATA_MAGIC);
		header.version = put_long(PLYRDATA_VERSION);
		header.header_size = put_long(sizeof(PLYRDATA_HEADER));
		header.record_size = put_long(sizeof(PLYRDATA_RECORD));
		if (fwrite(&header, sizeof(PLYRDATA_HEADER), 1, f) < 1)
		{
		 	fprintf(stderr, "could not write header to plyrdata.\n");
			/*bailout(E_FATAL);*/
			return NULL;
		}
		fseek( f, 0, SEEK_SET );
	}
	else
	{
	 	fprintf(stderr, "could not create plyrdata.\n");
		/*bailout(E_FATAL);*/
		return NULL;
	}
	
	return f;
}
/***********************************************************************/
long put_long( long in )
{
#ifdef LITTLE_ENDIAN
	return
	((in >> 24) & 0xFF) ||
	((in >> 8)  & 0xFF00) ||
	((in << 8)  & 0xFF0000) ||
	((in << 24) & 0xFF000000);
#else
	return in;
#endif
}
/******************************************************************/
long get_long( long in )
{
#ifdef LITTLE_ENDIAN
	return
	((in >> 24) & 0xFF) ||
	((in >> 8)  & 0xFF00) ||
	((in << 8)  & 0xFF0000) ||
	((in << 24) & 0xFF000000);
#else
	return in;
#endif
}

/********************************************************************/
int get_plyrdata_record( int recno, PLYRDATA_RECORD *record)
{
	size_t record_size;
	long end;
	long pos;
	
	/* Check that record is within the file */
	fseek( fp, 0, SEEK_END );
	end = ftell( fp );
	pos = header.header_size + header.record_size * recno;
	if (pos + header.record_size > end)
		return 0;
	if (fseek(fp, pos, SEEK_SET)) {
		return 0;
	}
	record_size = sizeof(PLYRDATA_RECORD) < header.record_size ?
		sizeof(PLYRDATA_RECORD) : header.record_size;
	return fread(record, record_size, 1, fp);
}
/*******************************************************************/
int put_plyrdata_record( int recno, PLYRDATA_RECORD *record)
{
	size_t record_size;
	if (fseek(fp, header.header_size + header.record_size * recno,
	SEEK_SET)) {
		return 0;
	}
	record_size = sizeof(PLYRDATA_RECORD) < header.record_size ?
		sizeof(PLYRDATA_RECORD) : header.record_size;
	return fwrite(record, record_size, 1, fp);
}
/********************************************************************/
uint32 get_data(int recno,PLYRDATA_TYPE type)
{
	/* This function will return data from a player's record */
	PLYRDATA_RECORD rec;

	/* First make sure we can read the record. */
	if (get_plyrdata_record( recno, &rec ) < 1)
                {
                fprintf(stderr, "Could not read record #%d\n", recno );
		/*
		** If we can't read this, no data has been entered yet
		** so we can just return 0 (since all fields will
		** be 0 for new users.
		*/
		return 0;
                }

	/* Now see what data we need to retrieve and set it to output */
  	switch (type) {

    	case ontime:
		return rec.ontime;
		break;

	case total:
                return rec.total;
                break;
     
	case started:
		return rec.started;	
		break;

	case resigned:
		return rec.resigned;
		break;
	
	case tookover:
		return rec.tookover;
		break;

     default:
	fprintf(stderr, "Bad request for %d in XPlyrData file. Terminating.\n",
  		(int)type);
	return -1;
    }
   
   return -1;
}
/********************************************************************/
int put_data(int recno,PLYRDATA_TYPE type)
{

	/* First we have to find the appropriate record to put information
	** into. If the record does not exist, we'll return a -1. 
	** Mar 28, I've now changed this to create the record automatically
	** if it does not exist.
	*/
	PLYRDATA_RECORD rec;

	if (get_plyrdata_record( recno, &rec ) < 1)
        {
		/* OK, this record doesn't exist, better try to create it */
		memset( &rec, 0, sizeof(PLYRDATA_RECORD));
        }


	/* See what sort of of info we want to put in to the record and
	   act accordingly */
	switch (type){
  
		case ontime:
			rec.ontime++;
			put_plyrdata_record( recno, &rec);
			return 0;
			break;

		case total:
			rec.total++;
			put_plyrdata_record( recno, &rec);
			return 0;
			break;

		case started:
	                rec.started++;
			put_plyrdata_record( recno, &rec);
			return 0;
			break;

		case resigned:
	                rec.resigned++;
        	        put_plyrdata_record( recno, &rec );
			return 0;
			break;

		case tookover:
                       	rec.tookover++;
                       	put_plyrdata_record( recno, &rec );
                       	return 0;
                        break;
	
		default:
			fprintf(stderr, "Improper input for xplyrlog.");
			return -1;
	}
}
/******************************************************************/
void close_plyrdata( void )
{
   if(fclose(fp))
   {
	fprintf(stderr, "Error closing XPlyrData file");
	/*bailout(E_FATAL);*/
   }
}
  

