/*
 * $Log$
 * Revision 1.2.2.1  2001/10/19 23:09:27  dema
 * Extra values for initial money settings (as in Mach2)
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * Lots of fixes
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*
 *  The following manifest constants are used to seed the random number
 *  generator to produce deterministic die rolls for the Machiavelli
 *  variants to allow replayable seasons.
 */

/* These are all defined in conf.h now. */
/* #define DIE_ASSASSIN 382204
   #define DIE_EXPENSE  148741
   #define DIE_FAMPLAG  995816   
   #define DIE_INCOME   666144   */

/* this include is only needed for NPROV and NPOWER */
#include "porder.h"

#define FAMINE	1
#define PLAGUE	2
#define STORM 3
#define MAX_LOAN	25	/* Maximum amount to loan */

#define PF_unused2     0x2000	/* Unused flag number 2 (maxflag, see init)   */
#define PF_unused1     0x1000	/* Unused flag number 1                       */
#define PF_SIEGE       0x0800	/* Siege is going on in province              */
#define PF_CREBELLION  0x0400	/* Province's city is rebelling as well       */
#define PF_REBELLION   0x0200	/* Province is under rebellion                */
#define PF_FAMINE      0x0100	/* Province is famined                        */
#define PF_PORT        0x0080	/* Province has a port city                   */
#define PF_FORTRESS    0x0040	/* City is fortified or has fortress          */
#define PF_VENICE      0x0020   /* Province is Venice (or like Venice)        */
#define PF_VALUE       0x0007	/* Number of points for major city (mask)     */
#define PF_CONSTANT    0x00FF	/* Portion of flags that are constant         */

extern int npown[NPROV + 1];	/* New province owner  (Machiavelli)          */
extern int ncown[NPROV + 1];	/* New city owner      (Machiavelli)          */

struct treasury {
	short treasury;		/* Number of ducats in the treasury           */
	short loan[6 + 1];	/* Amount of each loan for next two years     */
	short interest[6 + 1];	/* Amount of interest due on each loan        */
};

struct expense {
	char type;		/* Type of expense                            */
	short amount;		/* Amount to spend on expense                 */
	short unit;		/* Unit or province for action                */
	unsigned char result;	/* Result of expense                         */
};

struct vincome {
	unsigned char prov;	/* Major home city for power's income        */
	unsigned char dice;	/* Number of die rolls                        */
	unsigned char vinc[6];	/* Income for die roll                        */
};

extern struct treasury ducats[NPOWER + 1];
extern struct expense expense[NPOWER + 1][4];
extern struct vincome vincome[MAXVINC];

extern unsigned char chits[NPOWER + 1][MAX_CHIT];	/* Assassination chits      */
extern short payments[NPOWER + 1][NPOWER + 1];	/* Who is paying whom what  */
extern short borrow[NPOWER + 1][2];	/* How much from bank       */
extern unsigned char allies[NPOWER + 1][NPOWER + 1];	/* Who is allowing whom     */


/* the permitted unit types in Mach, see section 'L' in map file */
#define P_ARMY          0
#define P_FLEET         1
#define P_GARRISON	2
#define MAX_UNIT_TYPES  (P_GARRISON +1)  /* Max types allowed in machiavelli */

/* The unit type types, used as bit masks */
#define P_NORMAL        0
#define P_CITIZEN       1
#define P_MERC          2
#define P_PROF          3
#define MAX_SPEC_TYPES  (P_PROF +1)

/* The various permitted ownership times of unit type types */
#define P_ANYTIME       'x'  /* This unit type can be built or bribed freely */
#define P_BUILD         'b'  /* This unit type can only be built             */
#define P_BUY           'p'  /* This unit type can only be purchased        */
#define P_NEVER         '.'  /* This unit type can never be owned            */

/* THe equivalent masks for the above */
#define PP_ANYTIME      0x3
#define PP_BUILD	0x2
#define PP_BUY 		0x1
#define PP_NEVER 	0x0

struct p_unit {
char power_letter;
int permissions[MAX_UNIT_TYPES];
};

extern int ftab[12 - 1][12 - 1];	/* Machiavelli famine province list           */
extern int ptab[12 - 1][12 - 1];	/* Machiavelli plague province list           */
extern int stab[12 - 1][12 - 1];        /* Machiavelli storm province list            */
extern struct p_unit permitted_units[MAXPLAYERS]; /* Mach permitted units for a country 	      */

struct p_money
{
    int enabled;
    int money[NPROV+1];
}; /* Initial money for city */

extern struct p_money initial_money;

#define PFTAB(name) int (*name)[12-1]

#define is_garrison(u)			(unit[u].type == 'G')
#define has_garrison(p)			(pr[p].gunit)

#define has_rebellion(p)		(pr[p].flags & (PF_REBELLION|PF_CREBELLION))
#define has_crebellion(p)		(pr[p].flags & PF_CREBELLION)
#define has_prebellion(p)		(pr[p].flags & PF_REBELLION)
#define cityvalue(p)			(pr[p].flags & PF_VALUE)
#define has_port(p)				(pr[p].flags & PF_PORT)
#define is_infected(p)			(pr[p].flags & PF_FAMINE)
#define is_sieged(p)			(pr[p].flags & PF_SIEGE)
#define is_venice(p)                    (pr[p].flags & PF_VENICE)

#define remove_rebellion(p)		(pr[p].flags &= ~(PF_REBELLION|PF_CREBELLION))
#define set_crebellion(p)		(pr[p].flags |= PF_CREBELLION)
#define remove_crebellion(p)	(pr[p].flags &= ~PF_CREBELLION)
#define set_prebellion(p)		(pr[p].flags |= PF_REBELLION)
#define remove_prebellion(p)	(pr[p].flags &= ~PF_REBELLION)
#define set_infection(p)		(pr[p].flags |= PF_FAMINE)
#define remove_infection(p)		(pr[p].flags &= ~PF_FAMINE)
#define set_siege(p)			(pr[p].flags |= PF_SIEGE)
#define remove_siege(p)			(pr[p].flags &= ~PF_SIEGE)

#define NO_SPECIAL_UNITS		(dipent.flags & F_NOSPECIAL)
#define NO_ADJACENCY			(dipent.flags & F_NOADJ)
#define NO_LOANS				(dipent.flags & F_NOLOANS)
#define NO_ASSASSINATION		(dipent.flags & F_NOASSASS)
#define NO_PLAGUE				(dipent.flags & F_NOPLAGUE)
#define NO_FAMINE				(dipent.flags & F_NOFAMINE)
#define NO_DICE					(dipent.flags & F_NODICE)
#define NO_STORMS			(!(dipent.xflags & XF_STORMS))
