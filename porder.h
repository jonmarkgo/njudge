/*
 * $Log$
 * Revision 1.9  2002/05/14 23:16:24  miller
 * Added 'h' province type and new_owner value for units
 *
 * Revision 1.8  2002/04/15 12:55:45  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.7  2001/10/29 23:41:34  miller
 * Fixed reoccurring chaos bug (use centre() macro not 'x' to test province)
 *
 * Revision 1.6  2001/10/20 12:11:14  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.5.2.1  2001/10/15 00:15:04  ustv
 * Added handling for 2nd type field, comments and Colonial settings
 *
 * Revision 1.5  2001/07/01 23:19:29  miller
 * Limit tables
 *
 * Revision 1.4  2001/02/23 00:19:59  miller
 * unDOSify
 *
 * Revision 1.3  2001/02/03 10:38:25  miller
 * add new structure members
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added values for blockades and transformations
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  porder.h

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

#ifndef _PORDER_H
#define _PORDER_H

#define NPROV       200		/* Maximum number of provinces on map         */
#define MAXUNIT     200		/* Maximum number of units on map     */
#define MAXVINC	    15		/* Maximum number of variable income cities */
#define MAX_CHIT    58		/* Maximum number of assassination chits    */
#define NPOWER      (WILD_PLAYER-1)
#define MV          0		/* Army/land movement                         */
#define MX          1		/* Special loeb9 water crossing             */
#define XC          4		/* Any coast                                  */
#define NC          5		/* North coast                              */
#define EC          6		/* East coast                               */
#define WC          7		/* West coast                               */
#define SC          8		/* South coast                              */
#define CC	    9		/* Special conditional movement             */
#define HX	    2		/* Half-unit movement			    */
#define LX	    3		/* Half unit movement and convoys	    */
#define PF_CONSTANT    0x00FF	/* Portion of flags that are constant         */

#define CMAP_NAME   0
#define CMAP_MOVE   1
#define CMAP_TYPE   2
#define CMAP_FLAG   3
#define CMAP_TYPE2  4
#define CMAP_SIZE   5

#define bribe(c)        (c == 'b' || c == 'd' || c == 'g')
#define water(p)	(pr[p].type == 'w' || pr[p].type == 'v' || pr[p].type == 'h')
#define water2(p)	(water(p) || pr[p].type2 == 'w')
#define highsea(p)      (pr[p].type == 'h')
#define centre(p)       ((pr[p].type == 'x') || (pr[p].type >= 'A' && pr[p].type <= 'Z') || (pr[p].type >= '0' && pr[p].type <= '9'))
#define railway(p)      (pr[p].type == 'r')
#define gateway(p)      (pr[p].type == 'g')

extern int cmap[CMAP_SIZE];

extern struct province {
	char *name;		/* Pointer into heap for official name        */
	unsigned char *move;	/* Pointer into heap for movement table       */
	int unit;		/* Ordinal of the unit here                   */
	int gunit;		/* Ordinal of the garrison (Machiavelli)      */
	int owner;		/* Owner of the province                      */
	int cown;		/* Owner of the city/fortress (Machiavelli)   */
	int home;		/* Power whose home this is part of           */
	char type;		/* 'X'=supply center, 'l'=land, 'w'=water     */				/* 'h'=High Sea				      */
	char type2;		/* if XF_PROV_DUALITY, 'l'= land, 'w'=water   */
	short flags;		/* Flags for province                         */
	int blockaded;		/* set to non-zero when centre is blockaded   */
	int gconv;		/* Mach: Non-zero if garrison converted       */
	int new_owner;		/* For setup, the new owner of province	      */
	int order_index;	/* index of next unit to be ordered	      */	
} pr[NPROV + 1];

extern int npr;			/* Actual number of provinces                 */

extern struct units {
	unsigned char owner;	/* The power that owns this unit              */
	char status;		/* ':' for normal, 'r' for retreat            */
	char type;		/* 'A' for Army, 'F' for fleet, 'W' wing      */
	char new_type;		/* New type of unit			      */
	char stype;		/* 'c' for Militia, 'm' for Mercenary, 'p'... */
	unsigned char loc;	/* Province ordinal                           */
	unsigned char coast;	/* The coast and/or movement type allowed     */
	unsigned char new_coast; /* The new coast type 			      */
	char order;		/* Move, Hold, Support or Convoy              */
	short proxy;		/* Actual unit ordinal for proxy order        */
	short unit;		/* Unit for support or convoy order           */
	short bloc;		/* Province controlling conditional orders    */
	short dest;		/* Movement or support destination            */
	unsigned char dcoast;	/* Coast specification for destination        */
	unsigned char *convoy;	/* Convoy chain to get to the destination or  */
	/*   list of possible retreats.               */
	unsigned char *rcoast;	/* For retreats, the coast to which the unit  */
	/* may retreat.                               */
	short unit_prov;	/* province of supported/convoyed unit        */
	unsigned char unit_coast; /* cost of supported/convoyed unit         */
	short railway_index;	/* Index of railway being used, else -1 */
	short exists;		/* True if unit really exists */

} unit[MAXUNIT];

extern char *mtype[];

extern char line[1024];
extern unsigned char *heap;
extern int hp, hpx, maxheap;

extern int nv;			/* Number of variable income cities           */

/* Colonial special settings */


/* HonkKong settings */

#define MAX_HK_PROVINCES 10

struct hk_struct {
int np;
char power_letter;
int pr[MAX_HK_PROVINCES];
};

extern struct hk_struct hk[MAX_POWERS];

extern int nhk;

#define MAX_GW_PROVINCES 2
#define MAX_GATEWAYS 5

struct gw_struct {
int np;
char name[40];
int prov_index; /* Index of psuedo province that is the gateway itself */
int gw_prov;    /* Index of province that is gateway in pr[] */
int pr[MAX_GW_PROVINCES];
};

extern struct gw_struct gw[MAX_GATEWAYS];

extern int ngw;

#define MAX_RAILWAYS 5
#define MAX_RW_PROVINCES 10

struct rw_struct {
int np; /* Number of provinces in local pr[] */
int prov_index; /* Index of dummy province in global pr[] */
char name[40];
char power_letter[MAX_POWERS+1];
int pr[MAX_RW_PROVINCES];
};

extern struct rw_struct rw[MAX_RAILWAYS];

extern int nrw;


extern FILE *ifp;

extern int nunit;		/* Actual number of units in the game         */
extern int err;			/* Number of errors encountered               */
extern int pline;		/* Nonzero if we've printed input line out    */
extern int marker;		/* Nonzero if processing latest set of orders */
extern int eof, eol;		/* Nonzero if end of file or end of line      */
extern int processing;		/* Nonzero when processing orders             */

extern int need_order[NPOWER + 1];	/* Set nonzero if orders needed next phase */
extern int more_orders;		/* Nonzero if not all units ordered           */


extern char *get_action();
extern char *get_amount(char *, int *);
extern char *get_duration( char *, int *);
extern char *get_expense();
extern char *get_exptype();
extern char *get_order();
extern char *get_power();
extern char *get_prov();
extern char *get_stype();
extern char *get_type();
extern char *lookfor();
extern char *get_index( char*, int *);

#endif

/***************************************************************************/
