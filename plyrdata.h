/*
 * Plyrdata.h - for use with plyrdata system for storing records.
 * Created 2001 - T. Miller and M. Becroft.
 */

#define PLYRDATA_FILENAME "plyrdata"

#define PLYRDATA_MAGIC 0x504C5952 /* PLYR */
#define PLYRDATA_VERSION 2

typedef unsigned long uint32;

typedef struct
{
	uint32 magic;
	uint32 version;
	uint32 header_size;
	uint32 record_size;
} PLYRDATA_HEADER;

typedef struct {
	uint32 ontime;
	uint32 total;
	uint32 started;
	uint32 resigned;
	uint32 tookover;
	uint32 reserved;
} PLYRDATA_RECORD;

typedef enum{
	total = 0,
	ontime,
	started,
	resigned,
	tookover
} PLYRDATA_TYPE;

int open_plyrdata( void );
int get_plyrdata_record( int recno, PLYRDATA_RECORD *record);
int put_plyrdata_record( int recno, PLYRDATA_RECORD *record);
void close_plyrdata( void );
FILE *create_plyrdata( void );

/* Helper functions to ease interface with the code. */
uint32 get_data(int recno,PLYRDATA_TYPE type);
int put_data(int recno,PLYRDATA_TYPE type);

/* Lilliput */
long put_long( long in );
long get_long( long in );

