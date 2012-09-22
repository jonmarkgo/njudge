/*
 * $Log: global.c,v $
 * Revision 1.15  2004-09-06 22:08:17  millis
 * Bug363 Allow more than one victor
 *
 * Revision 1.14  2004/05/22 08:58:22  millis
 * Bug 297: Add Intimate Diplomacy
 *
 * Revision 1.13  2004/01/04 11:34:35  millis
 * Implement Bug #262 (ExtraCentres for 1900 Steamroller)
 *
 * Revision 1.12  2003/05/14 07:59:17  nzmb
 * Fixed bug #122 -- splits dip.reply into part created before and after
 * the GM issues a "become" command.
 *
 * Revision 1.11  2003/05/13 10:59:09  millis
 * Correctly set bailout flag to initial 0 value
 *
 * Revision 1.10  2003/05/13 00:07:26  millis
 * Bug 110, move on process deadline by 24 hours on bailout recovery
 *
 * Revision 1.9  2003/05/12 23:23:46  millis
 * Fix bug 133, allow turn to process when set to manualprocess and process command sent.
 *
 * Revision 1.8  2003/01/14 13:51:36  millis
 * Merges from USTV
 *
 * Revision 1.7  2002/08/27 23:56:10  millis
 * Added better victory display (fixing Mach victory bug)
 *
 * Revision 1.6  2002/05/11 09:15:31  greg
 * Minor bug fixes
 * - fixed subjectline for absence requests
 * - fixed phase length, so it's no longer hard coded for responses
 * - partial fix for unusable builds, players with only unusable builds
 *    will no longer be flagged as having orders due, however players
 *    with some usable builds will need to waive any unusable builds,
 *    also, if one or more players have unusable builds, but no
 *    player has usable builds, the build phase will process after
 *    a short delay
 *
 * Revision 1.5  2001/10/20 12:11:11  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.4.2.2  2001/10/19 23:33:42  dema
 * Added initial_money + CVS header
 *
 *
 */    

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
short bids[NPOWER + 1][NPOWER + 1];  /* Who is bidding what for whom  */


unsigned char chits[NPOWER + 1][MAX_CHIT];	/* Assassination chits      */
short payments[NPOWER + 1][NPOWER + 1];		/* Who is paying whom what  */
short borrow[NPOWER + 1][2];	/* How much from bank       */
unsigned char allies[NPOWER + 1][NPOWER + 1];	/* Who is allowing whom     */

int ftab[12 - 1][12 - 1];	/* Machiavelli famine province list          */
int ptab[12 - 1][12 - 1];	/* Machiavelli plague province list          */
int stab[12 - 1][12 - 1];       /* Machiavelli storm  province list          */
struct p_unit permitted_units[MAXPLAYERS]; /* Mach list of permitted unit types	     */
struct p_money initial_money;   /* Mach set of initial money */

struct extra_centre extra_centres[MAXPLAYERS]; /* list of extra units for certain powers */

/* from porder.c, probably from some header file before that, i didn't
 * keep very good track -- nw Sun Oct  5 07:00:38 GMT 1997
 */
int cmap[CMAP_SIZE];
struct province pr[NPROV + 1];
int npr;			/* Actual number of provinces                 */
struct units unit[MAXUNIT];
char *mtype[] =
{"land", "landx", "landh", "landl", "coast", "north coast",
 "east coast", "west coast", "south coast", "conditional"};
char line[1024];
unsigned char *heap;
int hp, hpx, maxheap;
int nv;				/* Number of variable income cities           */

int nhk=0; 
struct hk_struct hk[MAX_POWERS];

int ngw =0;
struct gw_struct gw[MAX_GATEWAYS];

int nrw = 0;
struct rw_struct rw[MAX_RAILWAYS];

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

// int aflg = 0;		Replaced with options.dont_touch_q
// int Aflg = 0;		Replaced with options.dont_rm_q
// char *dflg;			Replaced with options.cwd
// int Dflg = 0;			Replaced with options.debug
// char *nflg;			Not longer used
// int qflg = 0;		Replaced with options.quick
// int tflg = 0;		Replaced with options.variant
// char *rflg = NULL;	Replaced with options.randomizer
// int sflg = 0;		Replaced with options.sequence
// int vflg = 0;		Replaced with options.verbose
// int xflg = 0;		Replaced with options.no_input
// char *inname = NULL; Not longer used
// FILE *inp;			Not longer used

int control = 0;
int predict = 0; 		/* =1 to predict a move for master */

int canbuild = 1;		/* can the power being reminded to move build? */
int victor = 0;			/* Set to number of victorious powers */
int victors[MAXPLAYERS];	/* Array of victors */

int process_set; 		/* Set to 1 if process command has been set */

int bailout_recovery = 0;       /* Set to 1 if recovering from a bail-out	*/

int GM_Became = 0;		/* if GM issued the become command */

int has_treasury = 0;		/* Set to 1 if the datafile supplies treasury information */
