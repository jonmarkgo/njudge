/*
   ** $Log$
   ** Revision 1.1  1996/10/20 12:29:45  rpaar
   ** Morrolan v9.0
   **
 */

/*  po_condition.c
   **
   **  Copyright 1987, Lowe.
   **
   **  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
   **  Maryland, all rights reserved; used with permission.
   **
   **  Redistribution and use in source and binary forms are permitted
   **  provided that it is for non-profit purposes, that this and the
   **  above notices are preserved and that due credit is given to Mr.
   **  Lowe.
   **
 */

#include "dip.h"
#include "functions.h"
#include "porder.h"

/****************************************************************************/

int condition(char **s)
{

/*
   **  Evaluate a conditional expression for either true or false.
   **
   **  The conditional statements are of the form:
   **
   **  expression ::= (condition {[and/or] condition})
   **  condition ::= {not} <province> {<power>} {<type>}
 */

#define C_AND    1
#define C_OR     2
#define C_THEN   3
#define C_NOT    1

	static char *conjunction[] =
	{0, "and", "or", "then"};
	static char *negation[] =
	{0, "not"};

	int op = C_AND, vp = 1, n = 0;
	int v, p, c, u, i, owner;
	char type;


	while (isspace(**s))
		(*s)++;

	while (**s && op != C_THEN) {

		*s = lookfor(*s, negation, nentry(negation), &i);
		if (i)
			n = !n;

		if (**s == '(') {
			(*s)++;
			v = condition(s);
		} else {
			type = 'x';
			owner = 0;
			p = 0;
			c = 0;
			do {
				i = 0;
				if (!owner) {
					*s = get_power(*s, &owner);
					if (owner)
						i++;
				}
				if (type == 'x') {
					*s = get_type(*s, &type);
					if (type != 'x')
						i++;
				}
				if (!p) {
					*s = get_prov(*s, &p, &c);
					if (p)
						i++;
				}
			} while (i);

			if (!p)
				errmsg("Unrecognized province -> %s", *s);


			v = (u = pr[p].unit) && unit[u].status != 'b' &&
			    (!owner || unit[u].owner == owner) &&
			    (!c || unit[u].coast == c) &&
			    (type == 'x' || unit[u].type == type);

#ifdef notdef
			printf("Looking for %s %c at %s: ", owners[owner], type, pr[p].name);
			if (!u)
				printf("No unit.\n");
			else {
				printf("Owner %d, ", (!owner || unit[u].owner == owner));
				printf("Coast %d, ", (!c || unit[u].coast == c));
				printf("Type  %d. ", (type == 'x' || unit[u].type == type));
				printf("Stats %d. ", unit[u].status);
				printf("Result is %d\n", v);
			}
#endif
		}

#ifdef undef
		printf("Result: vp = %d, op = %d, v = %d", vp, op, v);
#endif
		if (op == C_AND)
			vp = vp && (n ? !v : v);
		else
			vp = vp || (n ? !v : v);
#ifdef undef
		printf(" is %d.\n", vp);
#endif

		n = 0;
		do {
			if (**s == ')') {
				(*s)++;
				if (n)
					errmsg("]\n");
				return vp;
			}
			if (!**s || **s == '\n')
				op = C_THEN;
			else {
				*s = lookfor(*s, conjunction, nentry(conjunction), &op);
				if (!op) {
					if (!n++)
						errmsg("Discarding junk: [");
					errputc(*(*s)++, rfp);
				}
			}
		} while (!op);
		if (n)
			errmsg("]\n");
		n = 0;
	}

	return vp;

}

/****************************************************************************/
