

#include <stdio.h>
#include "dip.h"
#include "mach.h"

/* name and place of config file */
char *CONFIG_FILE ="dip.conf";
char *CONFIG_DIR = ".";

struct dipent dipent;
FILE *log_fp;			/* General log/interlock file                   */
FILE *rfp;			/* Reply file pointer                           */
FILE *cfp;			/* Control file pointer                         */
FILE *xfp;			/* Control file pointer                         */
int nded;

char subjectline[80];

struct dedication ded[MAXUSER];

/* from mach.h */
int npown[NPROV + 1];		/* New province owner  (Machiavelli)         */
int ncown[NPROV + 1];		/* New city owner             (Machiavelli)          */

struct treasury ducats[NPOWER + 1];
struct expense expense[NPOWER + 1][4];
struct vincome vincome[MAXVINC];

unsigned char chits[NPOWER + 1][MAX_CHIT];	/* Assassination chits      */
short payments[NPOWER + 1][NPOWER + 1];		/* Who is paying whom what  */
short borrow[NPOWER + 1][2];	/* How much from bank       */
unsigned char allies[NPOWER + 1][NPOWER + 1];	/* Who is allowing whom     */

int ftab[12 - 1][12 - 1];	/* Machiavelli famine province list          */
int ptab[12 - 1][12 - 1];	/* Machiavelli plague province list          */

/* from porder.c, probably from some header file before that, i didn't
 * keep very good track -- nw Sun Oct  5 07:00:38 GMT 1997
 */
int cmap[CMAP_SIZE];
struct province pr[NPROV + 1];
int npr;			/* Actual number of provinces                 */
struct units unit[MAXUNIT];
char *mtype[] =
{"land", "landx", "coast", "north coast",
 "east coast", "west coast", "south coast", "conditional"};
char line[1024];
unsigned char *heap;
int hp, hpx, maxheap;
int nv;				/* Number of variable income cities           */
FILE *ifp;

int nunit;			/* Actual number of units in the game         */
int err;			/* Number of errors encountered               */
int pline;			/* Nonzero if we've printed input line out    */
int marker;			/* Nonzero if processing latest set of orders */
int eof, eol;			/* Nonzero if end of file or end of line      */
int processing;			/* Nonzero when processing orders             */

int need_order[NPOWER + 1];	/* Set nonzero if orders needed next phase */
int more_orders;		/* Nonzero if not all units ordered           */

/*
 * the following variables were moved here from dip.h so we don't
 * get multiple definitions
 */

/* this may need to be included in the other programs as well */
char *notifies = "*";

int aflg = 0;			/* -a: don't mess with the at queue at all      */
int Aflg = 0;			/* -A: don't take anything off the at queue     */
char *dflg;			/* -d home directory                            */
int Dflg = 0;			/* -D: output goes to stdout rather than reply  */
char *nflg;			/* Our program name                             */
int qflg = 0;			/* -q: quick mode, just process mail, not games */
int tflg = 0;			/* -t variant: test adjudication                */
char *rflg = NULL;		/* -r name: randomizer for die rolls            */
int sflg = 0;			/* -s sequence: test adjudication               */
int vflg = 0;			/* -v: verbose, issue all error messages        */
int xflg = 0;			/* -x: no incoming mail on stdin                */
char *inname = NULL;
FILE *inp;

int control = 0;

