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
#include <netinet/in.h>

#include "plyrdata.h"

#define LE 0
#define BE 1

void set_byte_orders(uint32 magic);

int errno;
PLYRDATA_HEADER header;
static FILE *fp;
static short hostbo;
static short filebo;

FILE *create_plyrdata(void);

int open_plyrdata(void) {
    /* Open the PlyrData file. */
        
    if (fp)
    {
        fprintf(stderr, "Opening already open plyrdata file.\n");
    }

    fp = fopen(PLYRDATA_FILENAME, "r+");

    if (!fp) {
        if (errno == EACCES || errno == ENOENT) {
            if (!(fp = create_plyrdata())) {
                fprintf(stderr, "Error %d creating plyrdata.\n", errno);
                /*bailout(E_FATAL);*/
                return 1;
            }
        }
        else {
            fprintf(stderr, "Error opening plyrdata file");
            /*bailout(E_FATAL);*/
            return 1;
        }
    }

    if (fread(&header, sizeof(PLYRDATA_HEADER), 1, fp ) < 1)  
    {
        fprintf(stderr, "plyrdata header too short.\n");
        /*bailout(E_FATAL);*/
        return 1;
    }
    set_byte_orders(header.magic);
    if (
            /* remember to convert the header fields from
               network to host byte order if needed */
            get_long(header.magic) != PLYRDATA_MAGIC ||
            get_long(header.version) > PLYRDATA_VERSION ||
            get_long(header.record_size) > 400 ||
            get_long(header.header_size) > 400 ) {
        fprintf(stderr, "plyrdata header invalid.\n");
        /*bailout(E_FATAL);*/
        return 1;
    }
    return 0;
}

/********************************************************************/
void set_byte_orders(uint32 magic)
{
    if (magic == htonl(magic))
    {
        /* this is a big endian host */
        hostbo = BE;
    }
    else
        hostbo = LE;

    if (magic == PLYRDATA_MAGIC)
    {
        /* file is in network byte order */
        filebo = hostbo;
    }
    else
        filebo = !hostbo;
}

long put_long(long in)
{
    if (filebo == hostbo)
        return in;
    else
        return
            ((in >> 24) & 0xFF) |
            ((in >> 8)  & 0xFF00) |
            ((in << 8)  & 0xFF0000) |
            ((in << 24) & 0xFF000000);
}

long get_long(long in)
{
    if (filebo == hostbo)
        return in;
    else
        return 
            ((in >> 24) & 0xFF) |
            ((in >> 8)  & 0xFF00) |
            ((in << 8)  & 0xFF0000) |
            ((in << 24) & 0xFF000000);
}

/********************************************************************/
FILE *create_plyrdata(void) {
    FILE *f;

    f = fopen(PLYRDATA_FILENAME, "w+");

    if (f) {
        /*
         * store header in network byte order for extra
         * portability.
         * Tim Miller (Dec 8, 2002)
         */
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

/******************************************************************/

int get_plyrdata_record( int recno, PLYRDATA_RECORD *record)
{
    size_t record_size;
    long end;
    long pos;

    /* Check that record is within the file */
    fseek(fp, 0, SEEK_END);
    end = ftell(fp);
    pos = get_long(header.header_size) + get_long(header.record_size) * recno;
    if (pos + get_long(header.record_size) > end)
        return 0;
    if (fseek(fp, pos, SEEK_SET)) {
        return 0;
    }
    record_size = sizeof(PLYRDATA_RECORD) < get_long(header.record_size) ?
                  sizeof(PLYRDATA_RECORD) : get_long(header.record_size);
    return fread(record, record_size, 1, fp);
}
/*******************************************************************/
int put_plyrdata_record(int recno, PLYRDATA_RECORD *record)
{
    size_t record_size;
    if (fseek(fp,
            get_long(header.header_size) + get_long(header.record_size) * recno,
            SEEK_SET)) {
        return 0;
    }
    record_size = sizeof(PLYRDATA_RECORD) < get_long(header.record_size) ?
                  sizeof(PLYRDATA_RECORD) : get_long(header.record_size);
    return fwrite(record, record_size, 1, fp);
}
/********************************************************************/
uint32 get_data(int recno, PLYRDATA_TYPE type)
{
    /* This function will return data from a player's record */
    PLYRDATA_RECORD rec;

    /* First make sure we can read the record. */
    if (get_plyrdata_record(recno, &rec) < 1)
    {
        fprintf(stderr, "Could not read record #%d\n", recno);
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
            return get_long(rec.ontime);
            break;

        case total:
            return get_long(rec.total);
            break;

        case started:
            return get_long(rec.started);   
            break;

        case resigned:
            return get_long(rec.resigned);
            break;

        case tookover:
            return get_long(rec.tookover);
            break;

        default:
            fprintf(stderr,
                    "Bad request for %d in plyrdata file. Terminating.\n",
                    (int)type);
            return -1;
    }

    return -1;
}
/********************************************************************/
int put_data(int recno, PLYRDATA_TYPE type)
{

    /* First we have to find the appropriate record to put information
     ** into. If the record does not exist, we'll return a -1. 
     ** Mar 28, I've now changed this to create the record automatically
     ** if it does not exist.
     */
    PLYRDATA_RECORD rec;
    uint32 temp;

    if (get_plyrdata_record(recno, &rec) < 1)
    {
        /* OK, this record doesn't exist, better try to create it */
        memset(&rec, 0, sizeof(PLYRDATA_RECORD));
    }

    /* See what sort of of info we want to put in to the record and
       act accordingly */
    switch (type) {

        case ontime:
            temp = get_long(rec.ontime);
            temp++;
            rec.ontime = put_long(temp);
            put_plyrdata_record(recno, &rec);
            return 0;
            break;

        case total:
            temp = get_long(rec.total); 
            temp++;
            rec.total = put_long(temp);
            put_plyrdata_record(recno, &rec);
            return 0;
            break;

        case started:
            temp = get_long(rec.started); 
            temp++;
            rec.started = put_long(temp);
            put_plyrdata_record(recno, &rec);
            return 0;
            break;

        case resigned:
            temp = get_long(rec.resigned); 
            temp++;
            rec.resigned = put_long(temp);
            put_plyrdata_record(recno, &rec);
            return 0;
            break;

        case tookover:
            temp = get_long(rec.tookover); 
            temp++;
            rec.tookover = put_long(temp);
            put_plyrdata_record(recno, &rec ;
            return 0;
            break;

        default:
            fprintf(stderr, "Improper input for plyrdata.");
            return -1;
    }
}
/******************************************************************/
void close_plyrdata( void )
{
   if (fclose(fp))
   {
        fprintf(stderr, "Error closing plyrdata file");
        /*bailout(E_FATAL);*/
   }
}
  

