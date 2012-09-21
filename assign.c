/*
 * $Log: assign.c,v $
 * Revision 1.1  1998-02-28 17:46:58  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 *
 */

/*
 *  assign.c -- Algorithm for the Assignment Problem (Rectangular Matrices)
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
 *      ----------- ------------ -----------------------------------------------
 *      ?? ??? 1987 K.Lowe       Original development.
 *      21 Mar 1994 C.Marcus,Jr. Restructure as 'normal' C code.
 */

/*
 *  Include Files
 */

#include "dip.h"

/*
 *  Declarations & Definitions
 */

#define MAXN        WILD_PLAYER	/* Maximum value of first subscript */
#define MAXM        WILD_PLAYER	/* Maximum value of second subscript */
#define INIT        (-1)	/* Initial value to indicate 'no subscript'
				   (can't use 0, it's a valid subscript) */

/*
 *  Public Function Declarations
 */

void assignment(int a[MAXN][MAXM], int n, int m, int x[MAXN]);

/* 
   #pragma title "void assignment(int a[MAXN][MAXM], int n, int m, int x[MAXN])"
   #pragma page
 */
/*******************************************************************************
NAME:  assignment

DESCRIPTION
    This procedure uses an ACM algorithm (Algorithm 415) to assign powers to the
    players, according to their preference lists, their order of signing on to
    the game, and (unfortunately) the order that the powers are listed in the
    power letters list for the variant.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    ?? ??? 1987 K.Lowe       Original development.
    21 Mar 1994 C.Marcus,Jr. Restructure as 'normal' C code, and document as
                             much as possible; slight change made in the logic
                             for the case n=m, so that excessive reduction of
                             the initial array isn't done.

INTERFACE DEFINITION
    Calling Sequence:
        assignment(&a, n, m, &x);

    Inputs:
        a[][] := Array of preferences:  a[i][j] is the preference assigned by
                 Player i for Position j (the lower the value, the more Player i
                 prefers Position j)
        n     := The number of Players (= number of rows in array a = number of
                 entries in array x)
        m     := The number of Positions (= number of columns in array a)

    Outputs:
        x[]   := Positions assigned:  x[i] is the Position assigned to Player i.
        a[][]    Values have been trashed during calculations.

ALGORITHM
    I'm sorry, go get the ACM's Collected Algorithms if you want to TRY to
    understand this.  My comments below are only approximately correct (as
    close as I could get to what's going on here).
*******************************************************************************/

void assignment(int a[MAXN][MAXM], int n, int m, int x[MAXN])
{
/*
 *  Dynamic Data Object Definitions
 */
	int c[MAXM],		/* Columns (Positions) that are not yet assigned */
	 cb[MAXM],		/* Balanced copy of c[] */
	 lambda[MAXM],		/* ? */
	 mu[MAXN],		/* ? */
	 r[MAXN],		/* Rows (Players) that are not yet assigned */
	 y[MAXM];		/* Columns (Positions) that are already assigned */
	int cbl,		/* Index into cb[] */
	 cl,			/* Index into c[] */
	 cl0,			/* Starting (?) index into c[] */
	 count = 0,		/* Number of Positions that are already assigned */
	 i,			/* General row-based index */
	 j,			/* General column-based index */
	 k,			/* Secondary row-based index (when i is busy) */
	 l,			/* Secondary column-based index (when j is busy) */
	 min_value,		/* Temp. holder for minimum value in a row/column */
	 rl,			/* Index into r[] */
	 rs,			/* Starting (?) index into r[] */
	 sw,			/* State indicator */
	 temp;			/* Temporary holder during swap */
/*
 *  End of Definitions
 */

/*  Unless there are more rows than columns, reduce each value in row a[i][] to
   be relative to the row's minimum value (i.e., a[i][j] is replaced by 
   a[i][j] - minimum across row a[i][]).  */

	if (n <= m) {
		for (i = 0; i < n; i++) {
			min_value = a[i][0];
			for (j = 1; j < m; j++) {
				if (a[i][j] < min_value)
					min_value = a[i][j];
			}
			for (j = 0; j < m; j++)
				a[i][j] -= min_value;
		}
	}
/*  If there are more rows than colums, reduce each value in column a[][j] to
   be relative to the column's minimum value (i.e., a[i][j] is replaced by
   a[i][j] - minimum across column a[][j]).  */

	else {
		for (j = 0; j < m; j++) {
			min_value = a[0][j];
			for (i = 1; i < n; i++) {
				if (a[i][j] < min_value)
					min_value = a[i][j];
			}
			for (i = 0; i < n; i++)
				a[i][j] -= min_value;
		}
	}

/*  At this point, each player i's most-preferred position is indicated by a 0
   in row i.  */

/*  Initialize the assigned-players and assigned-positions arrays.  */

	for (i = 0; i < n; i++)
		x[i] = INIT;
	for (j = 0; j < m; j++)
		y[j] = INIT;

/*  Now, check for positions that can be immediately assigned:  the first
   player to have a 0 for position j gets it.  */

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			if ((a[i][j] == 0) && (x[i] == INIT) && (y[j] == INIT)) {
				x[i] = j;
				y[j] = i;
				++count;
			}
		}
	}

/*  At this point, the only positions not assigned are those for which either
   no player had a 0 in that column, or the only player(s) that had 0(s) in
   that column had already been assigned other position(s).  */

/*  Count the number of players that don't have a position assigned them.  If
   the number of assigned positions equals the smaller of the two dimensions,
   we're finished.  */

	while (count < ((n < m) ? n : m)) {
		sw = 1;
		rl = cl = rs = 0;
		for (i = 0; i < n; i++) {
			mu[i] = INIT;
			if (x[i] == INIT) {
				r[rl++] = mu[i] = i;
			}
		}

/*  Initialize the lambda array.  */

		for (j = 0; j < m; j++)
			lambda[j] = INIT;

/*  From this point on, it's BFM--"magic of the blackest kind".  */

		while (sw != 2) {
			while (rs < rl) {
				i = r[rs++];
				for (j = 0; j < m; j++) {
					if ((a[i][j] == 0) && (lambda[j] == INIT)) {
						lambda[j] = i;
						c[cl++] = j;
						if (y[j] == INIT)
							break;
						r[rl++] = y[j];
						mu[y[j]] = i;
					}
				}
				if ((j < m) && (y[j] == INIT))
					break;
			}
			if ((j < m) && (y[j] == INIT))
				break;

/*  Renormalize the arrays [original comment].  */

			cl0 = cl;
			cbl = 0;
			for (j = 0; j < m; j++) {
				if (lambda[j] == INIT) {
					cb[cbl++] = j;
				}
			}

			min_value = a[r[0]][cb[0]];
			for (k = 0; k < rl; k++) {
				for (l = 0; l < cbl; l++) {
					if (a[r[k]][cb[l]] < min_value)
						min_value = a[r[k]][cb[l]];
				}
			}

			for (i = 0; i < n; i++) {
				if (mu[i] == INIT) {
					for (l = 0; l < cl0; l++) {
						a[i][c[l]] += min_value;
					}
				} else {
					for (l = 0; l < cbl; l++) {
						a[i][cb[l]] -= min_value;
						if ((sw == 1) && (a[i][cb[l]] == 0) &&
						(lambda[cb[l]] == INIT)) {
							lambda[cb[l]] = i;
							if (y[cb[l]] == INIT) {
								j = cb[l];
								sw = 2;
							} else {
								c[cl++] = cb[l];
								r[rl++] = y[cb[l]];
							}
						}
					}
				}
			}

			if (sw != 2) {
				for (i = cl0; i < cl; i++)
					mu[y[c[i]]] = c[i];
			}
		}

/*  Mark new column and permute [original comment].  */

		y[j] = i = lambda[j];
		while (x[i] != INIT) {
			temp = j;
			j = x[i];
			x[i] = temp;
			y[j] = i = lambda[j];
		}
		x[i] = j;
		++count;
	}
	return;
}
