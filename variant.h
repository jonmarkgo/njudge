/*
 * $Log$
 * Revision 1.3  2001/04/15 21:28:30  miller
 * Added new variants
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Specify for each variant if it is a homeCentre, OneCentre or AnyCentre game.
 * This makes it clearer for the player (who will see this always in a game listing) as well as removing specific tests in the code. Plus now any variant uses this only as a default - a game setup can change this if required/wanted.
 *
 * Revision 1.1  1998/02/28 17:51:14  david
 * Initial revision
 *
 * Revision 1.2  1997/01/01 22:10:12  davidn
 * Extra marker lines added for use with AddMap.
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*
 *  variant.h -- Definitions for Different Diplomacy Variants
 *
 *  Copyright (C) 1987, Ken Lowe
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 *
 *  REVISION HISTORY
 *      DATE        NAME         REASON
 *      ----------- ------------ ----------------------------------------------
 *      ?? ??? 1987 K.Lowe       Original development.
 *      31 Mar 1994 C.Marcus,Jr. Make sure all variant arrays are 0-based.
 *      11 May 1994 Nick F.      Added Asian Diplomacy
 *      24 Oct 1994 Nick F.      Added Modern Diplomacy
 *      29 Dec 1996 David N.     Added extra markers for AddMap
 *      09 Dec 1999 M. Miller    Added any_centres flag setting 
 */

/*
 *  At various points throughout this file, there are lines which begin with
 *  a / followed by ** . These lines are used by David Normans AddMap program
 *  to locate the various points at which it must alter the file in order to
 *  add a new variant to the file. Please do not move or remove any of these
 *  lines, or add any new lines with a / followed by ** at the beginning.
 */

#ifndef _VARIANT_H
#define _VARIANT_H

int GetMaxCountryStrlen(void); /* Set to make name of a country string */

/*
 *  Declarations & Definitions
 */

/* TODO is this legal, i thought an enum needed a tag name -- nw */

enum {
	NO_VARIANT = 0,		/* Don't use 0 as a variant number   */
	V_STANDARD,		/*  1 Standard variant               */
	V_youngstown,		/*  2 Youngstown variant             */
	V_loeb9,		/*  3 Loeb 9 player variant          */
	V_pure,			/*  4 Pure variant                   */
	V_chaos,		/*  5 34 player nonsense             */
	V_britain,		/*  6 Great Britain variant          */
	V_1898,			/*  7 1898 variant                   */
	V_crowded,		/*  8 Eleven player variant          */
	V_machiavelli,		/*  9 Machiavelli 1454               */
	V_fleet_rome,		/* 10 Standard with a fleet in rome  */
	V_shift_right,		/* 11 Shift right variant            */
	V_shift_left,		/* 12 Shift left variant             */
	V_rootz,		/* 13 Root Z Diplomacy               */
	V_aberration,		/* 14 The way things might have been */
	V_wraparound,		/* 15 Doughnut shaped nonsense       */
	V_chromatic,		/* 16 5-Italy version with colours   */
	V_milan,		/* 17 Milan Diplomacy                */
	V_asia,			/* 18 Asian Diplomacy                */
	V_modern,		/* 19 Modern Diplomacy               */
	V_colonial,		/* 20 Colonial Diplomacy             */
	V_h31,			/* 21 Hundred Years War Diplomacy    */
	V_h32,			/* 22 Hundred Years War Diplomacy    */
	V_classical,		/* 23 Classical Medit. Diplomacy     */
	V_empire,		/* 24 Empire version		     */
	V_african2,		/* 25 African2			     */
	V_wraparound2,		/* 26 2nd version of wraparound	     */
	V_shift_around,		/* 27 Shift around verison	     */
	V_layered,		/* 28 Layered			     */
        V_sailho_crowded,       /* 29 SailHo crowded                 */
	V_sailho,		/* 30 SailHo			     */
	/** ADD NEW VARIANTS ABOVE THIS LINE! **/
	NVARIANT		/* Last variant + 1                  */
};

/** UPDATE DEFINES AS NECESSARY BELOW **/
#define NVAROPTS       4	/* Number of variant options (blind/gunboat/shorthand/a/f) */
#define MAX_POWERS    36	/* Max. powers in a single game (letters+digits) */
#define LPOWER        24	/* Length of the longest power name              */

/* Do not exceed byte limit as these are used in char arrays */
#define WILD_PLAYER  252  /* First non-player player ordinal               */
#define AUTONOMOUS   253  /* Non-player ordinal                            */
#define OBSERVER     254  /* Non-player ordinal                            */
#define MASTER       255  /* Last Non-player ordinal                       */

/** UPDATE DEFINES AS NECESSARY ABOVE **/

/*  Macro to set the number of players (dipent.np) and number of centres to
   win (dipent.vp) for the appropriate variant.  */

#define SETNP(variant) \
  dipent.pl = pletter[variant];                                         \
  if (dipent.pl[AUTONOMOUS] == '&') dipent.flags |= F_MACH;             \
   dipent.xflags &= ~XF_BUILD_ANYCENTRES; 				\
   dipent.xflags &= ~XF_BUILD_ONECENTRE;				\
  switch (variant) {                                                    \
    case V_STANDARD:    dipent.np =  7; dipent.vp = 18; break;          \
    case V_youngstown:  dipent.np = 10; dipent.vp = 37; break;          \
    case V_loeb9:       dipent.np =  9; dipent.vp = 20; break;          \
    case V_pure:        dipent.np =  7; dipent.vp =  4; break;          \
    case V_chaos:       dipent.np = 34; dipent.vp = 18; dipent.xflags |= XF_BUILD_ANYCENTRES; break;          \
    case V_britain:     dipent.np =  7; dipent.vp = 19; break;          \
    case V_1898:        dipent.np =  7; dipent.vp = 18; break;          \
    case V_crowded:     dipent.np = 11; dipent.vp = 18; break;          \
    case V_machiavelli: dipent.np =  8; dipent.vp = 15; break;          \
    case V_fleet_rome:  dipent.np =  7; dipent.vp = 18; break;          \
    case V_shift_right: dipent.np =  7; dipent.vp = 18; break;          \
    case V_shift_left:  dipent.np =  7; dipent.vp = 18; break;          \
    case V_rootz:       dipent.np = 14; dipent.vp = 28; break;          \
    case V_aberration:  dipent.np =  9; dipent.vp = 27; dipent.xflags |= XF_BUILD_ONECENTRE; break;          \
    case V_wraparound:  dipent.np =  7; dipent.vp = 19; break;          \
    case V_chromatic:   dipent.np =  5; dipent.vp = 11; break;          \
    case V_milan:       dipent.np =  7; dipent.vp = 18; break;          \
    case V_asia:        dipent.np =  7; dipent.vp = 21; break;          \
    case V_modern:      dipent.np = 10; dipent.vp = 33; break;          \
    case V_colonial:    dipent.np =  7; dipent.vp = 30; break;          \
    case V_h31:         dipent.np =  3; dipent.vp =  9; dipent.xflags |= XF_BUILD_ANYCENTRES; break;          \
    case V_h32:         dipent.np =  3; dipent.vp =  9; dipent.xflags |= XF_BUILD_ANYCENTRES; break;          \
    case V_classical:   dipent.np =  5; dipent.vp =  18; dipent.xflags |= XF_BUILD_ANYCENTRES; break;         \
    case V_empire:	dipent.np = 10; dipent.vp = 29; break; \
    case V_african2:	dipent.np = 6; dipent.vp = 19; break; \
    case V_wraparound2:	dipent.np = 7; dipent.vp = 19; break; \
    case V_shift_around: dipent.np = 7; dipent.vp = 18; break; \
    case V_layered: 	dipent.np = 7; dipent.vp = 35; break; \
    case V_sailho:	dipent.np = 4; dipent.vp = 9; break; \
    case V_sailho_crowded: dipent.np = 6; dipent.vp = 10; break; \
/* ADD NEW VARIANTS ABOVE THIS LINE! */                                 \
    default:                                                            \
      fprintf(stderr,"Bad variant %d for %s.\n",variant,dipent.name);   \
      exit(1);                                                          \
  }

/*
 *  External Data Object Declarations
 */

/*  Variant and option names.  */

extern char *variants[];

/*  Variant and option values.  */

extern int vvalue[];

/*  Power names.  */

extern char *powers[];

/*  Owner names (i.e., possessive form of power names).  */

extern char *owners[];

/*  Power abbreviations based on variant ('x' means corresponding power isn't
   in this variant).  The last five characters should always be '?xOM', except
   for Machiavelli where it's '?&OM'.  Likewise, 'O' and 'M' should not be
   used for anything other than Observer and Master, respecitively.  */

extern char *pletter[];

/*  Starting phase.  */

extern char *sphase[];

#endif				/* _VARIANT_H */
