/*
 * $Log$
 * Revision 1.5  2003/01/14 13:56:46  millis
 * Updated with ustv merged changed
 *
 * Revision 1.4.2.1  2003/01/13 16:04:55  millis
 * ustv latest versions
 *
 * Revision 1.4  2001/12/11 12:08:36  miller
 * Added extra "trafo to" action
 *
 * Revision 1.3  2001/07/01 23:19:29  miller
 * Change function signature
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Added support for wing units
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  p_get.c

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

/*
 *  02 Dec 1999 Millis Miller Added 'wing' ptions for F_WINGS
 *  04 Dec 1999 Millis Miller Added 'transform' option for builds
 */


#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"

char *get_action(char *l, char *a)
{

	/*  Look for an appropriate action word for an order  */

	int i;
	static char *words[] =
	{"0",
	 "trafo to", "trafo", "transform to", "transforms to", "transforms", "transform", "tr#",
	 "besieges", "besiege", "sieges", "siege", "b#",
	 "convoys", "convoy", "transports", "transport", "t",
	 "fast ferry", "ferry", "ff", "f",
	 "disbands", "disband",
	 "debarks to", "disembarks to", "deboards to",
	 "debarks", "disembarks", "deboards",
	 "debark to", "disembark to", "deboard to",
	 "debark", "disembark", "deboard", "dk",
	 "embarks", "emboards", "boards",
	 "embark", "emboard", "board", "em", "e",
	 "holds", "hold", "stands", "stand", "h",
	 "lift siege", "lifts siege", "lifts", "lift", "ls", "l#",
	 "->", "-", "move to#", "moves to#", "moves", "move", "m",
	 "proxy to#", "give proxy to#", "proxy", "p#",
	 "supports", "support", "s",
	 "convert to#", "converts to#", "converts", "convert",
	 "conversion", "conversion to#",
	 "d", "c",
	 "air", "airlift", "airlifts", "air lift", "air lifts", "a"
	};

	static char reply[] =
	{'x',
	  't', 't', 't', 't', 't', 't', 't',
	  'b', 'b', 'b', 'b', 'b',
	 'c', 'c', 'c', 'c', 'c',
	 'c', 'c', 'c', 'c',
	 'd', 'd',
	 'k', 'k', 'k',
	 'k', 'k', 'k',
	 'k', 'k', 'k',
	 'k', 'k', 'k', 'k',
	 'e', 'e', 'e',
	 'e', 'e', 'e', 'e', 'e',
	 'h', 'h', 'h', 'h', 'h',
	 'l', 'l', 'l', 'l', 'l', 'l',
	 'm', 'm', 'm', 'm', 'm', 'm', 'm',
	 'p', 'p', 'p', 'p',
	 's', 's', 's',
	 'v', 'v', 'v', 'v',
	 'v', 'v',
	 'D', 'C',
	 'a', 'a', 'a', 'a', 'a', 'a'
	};


	l = lookfor(l, words, nentry(words), &i);
	*a = reply[i];
	if (*a == 'C') {
		if (dipent.flags & F_MACH) {
			*a = 'v';
		} else {
			*a = 'c';
		}
	}
	if (*a == 'D') {
		if (dipent.phase[5] == 'R') {
			*a = 'd';
		} else {
			*a = 'k';
		}
	}
	return l;
}

/*
 * TODO figure out what the -99999 assignment to a char is doing,
 * and why we immediately terminate the string pointed to by d
 */

char *get_amount(char *l, int *d)
{

	/*  Look for an amount of money  */

	char *s;
	int i, j;

	static char *words[] =
	{"0", "d#", "ducats#", "ducat#"};

	s = l;
	*d = 0;
	do {
		j = 1;
		if (*s == '+')
			s++;
		if (*s == '-') {
			j = -1;
			s++;
		}
		while (isspace(*s))
			s++;
		*d += atoi(s) * j;
		for (j = 0; isdigit(*s); s++)
			j++;
		s = lookfor(s, words, nentry(words), &i);
		while (isspace(*s))
			s++;
	} while (*s == '-' || *s == '+');

	if (j == 0) {
		*d = -99999;
		return l;
	}
	return s;

}

char *get_duration(char *l, int *d)
{

	/*
	 * Look for a duration of one or two years for a bank loan.
	 */

	int i;

	static char *words[] =
	{"0", "for", "1 year", "one year", "one", "1",
	 "at", "20 %", "20 percent", "20",
	 "2 years", "two years", "two", "2",
	 "50 %", "50 percent", "50"};

	static unsigned char reply[] =
	{0, 200, 1, 1, 1, 1,
	 200, 1, 1, 1,
	 2, 2, 2, 2,
	 2, 2, 2};

	do {
		l = lookfor(l, words, nentry(words), &i);
	} while (reply[i] == 200);
	*d = reply[i];
	return l;

}

char *get_expense(char *l, char *e)
{

	/* Look for an appropriate action word for an expense order */

	int i;

	static char *words[] =
	{"0", "pay", "give", "loan",
	 "borrow",
	 "ally", "allies", "alliance", "allow",
	 "unally", "not allies", "unalliance",
	 "don't allow", "dont allow",
	 "expense", "exp", "e#"};

	static char reply[] =
	{'x', 'p', 'p', 'p',
	 'b',
	 'a', 'a', 'a', 'a',
	 'u', 'u', 'u',
	 'u', 'u',
	 'e', 'e', 'e'};


	l = lookfor(l, words, nentry(words), &i);
	*e = reply[i];
	return l;

}

char *get_exptype(char *l, char *e)
{

	/* Look for an appropriate action word for an expense order */

	int i;

	static char *words[] =
	{"0", "famine relief", "fr#", "famine", "relief",
	 "pacify rebellion", "pr#", "pacify",
	 "counter-bribe", "cb#", "counter",
	 "disband", "d#",
	 "buy", "purchase", "b#",
	 "garrison to autonomous", "gta", "g#",
	 "autonomous", "to autonomous", "to a#",
	 "cause rebellion", "cr#", "cause", "rebellion",
	 "assassinate", "a#"};
	static char reply[] =
	{'x', 'f', 'f', 'f', 'f',
	 'p', 'p', 'p',
	 'c', 'c', 'c',
	 'd', 'd',
	 'b', 'b', 'b',
	 'g', 'g', 'g',
	 'g', 'g', 'g',
	 'r', 'r', 'r', 'r',
	 'a', 'a'};


	l = lookfor(l, words, nentry(words), &i);
	*e = reply[i];
	return l;

}

char *get_order(char *l, char *o)
{

	/* Look for an appropriate action word for a build order  */

	int i;

	static char *words[] =
	{"0", "build", "b#", "debuild",
	 "remove", "r#", "disband", "d#",
	 "maintain", "m#",
	 "waive", "w#",
	 "transform", "trafo", "t#",
	 "home", "nohome", "no home",
	 "erase", "zap", "restore",
	 "own" };
	static char reply[] =
	{'x', 'b', 'b', 'r',
	 'r', 'r', 'r', 'r',
	 'm', 'm',
	 'w', 'w',
	 't', 't', 't',
	 'h', 'n', 'n',
	 'c', 'z', 's',
	 'o'
	};


	l = lookfor(l, words, nentry(words), &i);
	*o = reply[i];
	return l;

}

char *get_power(char *l, int *o)
{

	/*
	 * Look for an appropriate name for a power.
	 */

	char *s;
	int i, n;


	*o = 0;
	while (isspace(*l))
		l++;
	for (n = 0, s = l; isalnum(*s) || *s == '-'; s++, n++);
	if (n < 2)
		return l;
	for (i = 1; i < WILD_PLAYER; i++) {
		if (dipent.pl[i] != 'x' &&
		    (!strncasecmp(l, owners[i], n) || !strncasecmp(l, powers[i], n))) {
			*o = i;
			while (isspace(*s))
				s++;
			return s;
		}
	}
	return l;

}

char *get_stype(char *l, char *t)
{

	/*
	 * Check for special unit type identifiers.
	 */

	int i;
	static char *words[] =
	{"0", "citizen's militia", "militia", "cm#",
	 "elite mercenary", "mercenary",
	 "elite mercenaries", "mercenaries", "em#",
	 "elite professional", "professional", "ep#"};

	static char reply[] =
	{'x', 'c', 'c', 'c', 'm', 'm', 'm', 'm', 'm', 'p', 'p', 'p'};

	l = lookfor(l, words, nentry(words), &i);
	*t = reply[i];
	return l;

}

char *get_type(char *l, char *t)
{

	/*
	 * Check input for optional unit type of 'a', 'army', 'f' or 'fleet', 'w' or 'wing'.
	 *
	 * Exit: t = Unit type or 'x' if invalid.
	 */

#define F_ANY 0x01

	int i;
	char *s;

	static char *words[] =
	{"0", "army / fleet", "a/f#", "af#",
	 "wing", "w#", 
	 "army", "a#",
	 "fleet", "f#",
	 "garrison", "g#",
	 "artillery", "cannon", "r#",
	 "unit",
	 "spy"};

	static char reply[] =
	{'x', 'T', 'T', 'T',
	 'W', 'W', 
	 'A', 'A',
	 'F', 'F',
	 'G', 'G',
	 'R', 'R', 'R', 
	 'U',
	 'S'};

	static int flags[] =
	{0, F_AFRULES, F_AFRULES, F_AFRULES,
	 F_WINGS, F_WINGS,
	 F_ANY, F_ANY,
	 F_ANY, F_ANY,
	 F_MACH, F_MACH,
	 F_ANY, F_ANY, F_ANY,
	 F_ANY,
	 F_BLIND};

	s = lookfor(l, words, nentry(words), &i);
	if (flags[i] & (dipent.flags | F_ANY)) {
		*t = reply[i];
		return s;
	} else {
		*t = reply[0];
		return l;
	}

}

