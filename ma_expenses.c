/*
 * $Log$
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  po_expchk.c

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

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

#define BOUNCE 		2
#define NOADJACENT	3
#define TOO_MANY	4


/*
 * Input processing of the expenditures, payments, loans, etc.
 */
int expin(char **s, int p)
{
	/* s is the Input stream */
	/* p is the Power specification */

	/*
	 * The format for the special commands is:
	 *
	 * cmd          ::= <borrow_cmd> | <give_cmd> | <exp_cmd> | <ally_cmd>
	 *
	 * borrow_cmd   ::= <borrow> <amount> <duration>
	 * give_cmd     ::= <give> { <amount> | <power> } to { <power> | "bank" }
	 * exp_cmd      ::= <expense> <amount> <etype> <type> <s-prov>
	 * ally_cmd     ::= <ally> <power>
	 */

	char c, order;
	int i, j, p1, p2, u, u1, u2, c1;
	unsigned char *bp;

	*s = get_expense(*s, &c);

	if (c == 'x') {
		/*
		   ** Return with a positive value means here
		   ** This line does not look like a expenditure
		 */
		return 1;
	}
	if (p > NPOWER) {
		errmsg("Cannot submit expense as master.  Use 'become' command.\n");
		return E_WARN;
	}
	switch (c) {
	case 'a':		/* Ally with power, power, power... */
	case 'u':		/* unally with power, power, power... */
		do {
			/* TODO get_power wants a char *, why are we passing it
			 * and int * ?, i've added a cast to -Wall shuts up, 
			 * but i think that there may be a more serious problem here
			 */
			*s = get_power(*s, &p1);
			if (!p1) {
				errmsg("Invalid power name -> %s", *s);
				return E_WARN;
			}
			allies[p][p1] = c == 'a' ? 1 : 0;
		} while (**s == ',' || isalnum(**s));
		break;

	case 'b':		/* Borrow amount for 1/2 year(s) */
		if (NO_LOANS) {
			errmsg("Game %s does not allow bank loans.\n", dipent.name);
			return E_WARN;
		}
		*s = get_amount(*s, &p1);
		if (p1 < 0) {
			errmsg("Invalid amount specified -> %s", *s);
			return E_WARN;
		}
		/*
		   ** Get the total amound borrowed sofar
		 */
		for (j = i = 0; i < 6; i++)
			j += ducats[p].loan[i];

		/*
		   ** Get the duration, either 1 or 2
		 */
		*s = get_duration(*s, &i);
		if (!i) {
			errmsg("Invalid duration for loan -> %s", *s);
			return E_WARN;
		}
		/*
		   ** Add the amount borrowed for the other duration
		 */
		j += borrow[p][1 - (--i)];

		/*
		   ** No more than maximum 
		 */
		if (p1 + j - payments[p][0] > MAX_LOAN) {
			errmsg("Maximum outstanding principle is %d ducats.\n", MAX_LOAN);
			return E_WARN;
		}
		/*
		   ** Set the new borrowed amount for this duration
		 */
		borrow[p][i] = p1;
		break;

	case 'e':		/* Expense 1-4: type prov/unit/power */

		while (isspace(**s))
			s++;
		/*
		   * TODO bug check, was *s++, but "value computed not used",
		   * i have replaced it with an equivalent expression, but
		   * the original author may have meant (*s)++
		   * i have replaced at least one similar expression elsewhere with
		   * no note, so grepping the patch to this version may turn up
		   * some other places to check for this as well
		 */
		if ((i = atoi(*s) - 1) < 0 || i > 3) {
			errmsg("Expense number must be 1-4.");
			return E_WARN;
		}
		while (isdigit(**s))
			(*s)++;
		if (**s == ':')
			(*s)++;

		*s = get_amount(*s, &p2);
		while (isspace(**s) || **s == ',')
			(*s)++;

		if (testword(s, "none")) {
			p2 = 0;
			expense[p][i].type = 0;
			return 0;
		}
		*s = get_exptype(*s, &order);

		switch (order) {
		case 'a':	/* assassinate                  */
			if (NO_ASSASSINATION) {
				errmsg("Game %s does not allow assassinations.\n",
				       dipent.name);
				return E_WARN;
			}
			*s = get_power(*s, &u);
			if (!u) {
				errmsg("Invalid power specified -> %s", *s);
				return E_WARN;
			}
			break;
		case 'b':	/* buy                          */
		case 'c':	/* counter-bribe                */
		case 'd':	/* disband                      */
		case 'g':	/* garrison to autonomous       */
			*s = get_type(*s, &c);
			if (order == 'g') {
				if (c == 'x')
					c = 'G';
				if (c != 'G') {
					errmsg("Only garrisons can be made autonomous.\n");
					return E_WARN;
				}
			}
			*s = get_prov(*s, &p1, &c1);
			if (!p1) {
				errmsg("Unrecognized province -> %s", *s);
				return E_WARN;
			}
			u1 = pr[p1].unit;
			u2 = pr[p1].gunit;
			u = c == 'G' ? u2 :
			    c != 'x' ? u1 :
			    u1 && ((order == 'c' && unit[u1].owner == p) ||
			    (order != 'c' && unit[u1].owner != p)) ? u1 :
			    u2 && ((order == 'c' && unit[u2].owner == p) ||
			    (order != 'c' && unit[u2].owner != p)) ? u2 :
			    u1 ? u1 : u2;
			if (!u) {
				errmsg("No %s present %s %s.\n", utype(c),
				water(p1) ? "in the" : "in", pr[p1].name);
				return E_WARN;
			}
			if (c != 'x' && c != unit[u].type) {
				errmsg("The unit %s %s is %s, not %s.\n",
				water(p1) ? "in the" : "in", pr[p1].name,
				       autype(unit[u].type), autype(c));
				return E_WARN;
			}
			break;

		case 'f':	/* famine relief                */
		case 'p':	/* pacify rebellion             */
		case 'r':	/* cause rebellion              */
			*s = get_prov(*s, &u, &c1);
			if (!u) {
				errmsg("Unrecognized province -> %s", *s);
				return E_WARN;
			}
			/*
			   ** Venice can't be affected by rebellions
			 */
			if ((order == 'r') && !strcmp(pr[u].name, "Venice")) {
				errmsg("No rebellion may be placed in %s", pr[u].name);
				return E_WARN;
			}
			break;

		default:
			errmsg("Invalid expenditure type -> %s", *s);
			return E_WARN;
		}

		if (p2 < 0) {
			*s = get_amount(*s, &p2);
			if (p2 < 0) {	/* Why smaller than zero? */
				switch (order) {
				case 'a':	/* Assassination */
					p2 = 12;
					break;
				case 'b':	/* Buy unit */
					p2 = unit[u].owner == AUTONOMOUS ? 9 : 18;
					break;
				case 'c':	/* Counter-bribe */
					p2 = 3;
					break;
				case 'd':	/* Disband unit */
					p2 = unit[u].owner == AUTONOMOUS ? 6 : 12;
					break;
				case 'f':	/* Famine relief */
					p2 = 3;
					break;
				case 'g':	/* Garrison to autonomous */
					p2 = 9;
					break;
				case 'p':	/* Pacify Rebellion */
					p2 = 12;
					break;
				case 'r':
					p2 = pr[u].home == pr[u].owner ? 15 : 9;
					break;
				default:
					errmsg("Invalid amount specified.", *s);
					return E_WARN;
				}

				if (bribe(order)) {
					/* 
					   ** Special units
					 */
					if (unit[u].stype == 'c' || unit[u].stype == 'p')
						p2 *= 2;
					/*
					   ** Garrisons in major cities (value > 1)
					 */
					if (is_garrison(u) && cityvalue(unit[u].loc) > 1)
						p2 *= 2;
				}
			}
		}
		expense[p][i].type = order;
		expense[p][i].amount = p2;
		expense[p][i].unit = u;
		break;


	case 'p':		/* Pay amount|chit to power */
		*s = get_amount(*s, &p1);
		if (p1 < 0) {
			/* If a power is specified it is a chit transfer */
			*s = get_power(*s, &p1);
			if (!p1) {
				errmsg("Invalid amount specified -> %s", *s);
				return E_WARN;
			}
			if (testword(s, "assassination"))
				while (isspace(**s))
					(*s)++;

			if (testword(s, "chit"))
				while (isspace(**s))
					(*s)++;

			p1 = -p1;
		}
		if (!testword(s, "to")) {
			errmsg("Missing 'to' keyword.\n");
			return E_WARN;
		}
		while (isspace(**s))
			(*s)++;

		if (p1 >= 0 && testword(s, "bank")) {
			payments[p][0] = p1;
		} else {
			*s = get_power(*s, &p2);
			if (!p2) {
				errmsg("Unrecognized power specified -> %s", *s);
				return E_WARN;
			}
			if (p1 >= 0) {	/* Payments */
				for (i = j = 1; payments[p][i];) {
					if (payments[p][i] != p2) {
						payments[p][i++] = payments[p][j++];
						payments[p][i++] = payments[p][j++];
					} else {
						i += 2;
					}
				}
				if (p1 == 0)
					p2 = 0;
				payments[p][j++] = p2;
				payments[p][j++] = p1;
			} else {	/* Chits */
				for (bp = chits[p]; *bp && *bp != -p1; bp++);
				if (!*bp) {
					errmsg("You do not have an assassination chit for %s.\n",
					       powers[-p1]);
					return E_WARN;
				}
				*bp = WILD_PLAYER;
				for (bp = chits[p2]; *bp; bp++);
				if (bp <= chits[p2] + MAX_CHIT) {
					*bp++ = -p1;
					*bp++ = 0;
				}
				if (marker && !processing) {
					fprintf(rfp, "%s assassination chit given to %s.\n\n",
						owners[-p1], powers[p2]);
				}
			}
		}
		break;

	default:
		errmsg("Unknown expense %c (internal error)\n", c);
		err++;
	}
	return 0;
}

/****************************************************************************/

void expout(int pt)
{

	/*
	 *  Process expenditure orders.
	 */

	int i, j, k, l, nl, n1, p, p1, p2, u, u1, u2, minval;
	unsigned char *bp;

	unsigned char result[WILD_PLAYER][4];
	unsigned char counter[MAXUNIT];
	unsigned char amount[WILD_PLAYER][4];
	unsigned char assassin[WILD_PLAYER];


	if (err)
		fprintf(rfp, "\n");
	fprintf(rfp, "Expenditure %s for %s of %d.  (%s.%s)\n",
		pt ? "orders" : "results",
		dipent.phase[0] == 'F' ? "Fall" :
		dipent.phase[0] == 'U' ? "Summer" : "Spring",
		atoi(&dipent.phase[1]), dipent.name, dipent.seq);

	/*
	   **  First everyone pays off their loans to the bank.
	 */

	nl = 0;
	for (p = 1; p < WILD_PLAYER; p++) {
		if (processing || pt == p || pt == MASTER) {
			if ((i = payments[p][0]) > 0) {
				if (!nl++)
					putc('\n', rfp);
				fprintf(rfp, "%s:%*s Pays %d ducat%s to bank.",
					powers[p], LPOWER - strlen(powers[p]), "", i, i == 1 ? "" : "s");
				if (i > ducats[p].treasury) {
					i = ducats[p].treasury;
					ducats[p].treasury = 0;
					fprintf(rfp, "  ** NSF = %d ducat%s", i, i == 1 ? "" : "s");
				} else {
					ducats[p].treasury -= i;
				}
				putc('\n', rfp);
				if (ducats[p].interest[0] != -1) {
					for (j = 0; i > 0 && j < 6; j++) {
						if (ducats[p].interest[j] > i) {
							ducats[p].interest[j] -= i;
							i = 0;
						} else {
							i -= ducats[p].interest[j];
							ducats[p].interest[j] = 0;
						}
						if (ducats[p].loan[j] > i) {
							ducats[p].loan[j] -= i;
							i = 0;
						} else {
							i -= ducats[p].loan[j];
							ducats[p].loan[j] = 0;
						}
					}
				}
			}
		}
	}

	/*
	   **  Now players are free to pay other players.
	 */

	memset(assassin, 0, sizeof(assassin));
	memset(counter, 0, sizeof(counter));

	for (p = 1; p < WILD_PLAYER; p++) {
		if (processing || pt == p || pt == MASTER) {
			for (j = 1; (p1 = payments[p][j++]) != 0; j++) {
				if ((i = payments[p][j]) > 0) {
					if (!nl++)
						putc('\n', rfp);
					fprintf(rfp, "%s:%*s Pays %d ducat%s to %s.",
						powers[p], LPOWER - strlen(powers[p]), "", i, i == 1 ? "" : "s", powers[p1]);

					for (k = 0; k < 6; k++) {
						if (ducats[p].loan[k] + ducats[p].interest[k] > 0) {
							fprintf(rfp, "  ** Outstanding bank loan, ignored");
							break;
						}
					}

					if (k == 6) {
						if (i > ducats[p].treasury) {
							i = ducats[p].treasury;
							ducats[p].treasury = 0;
							fprintf(rfp, "  ** NSF = %d ducat%s", i, i == 1 ? "" : "s");
						} else {
							ducats[p].treasury -= i;
						}
						counter[p1] += i;
					}
					putc('\n', rfp);

				}
			}
		}
	}

	for (p = 1; p < WILD_PLAYER; p++)
		ducats[p].treasury += counter[p];

	/*
	   **  Now they're free to borrow more to get further in debt.
	 */

	for (p = 1; p < WILD_PLAYER; p++) {
		if (processing || pt == p || pt == MASTER) {
			for (k = 0; k < 2; k++) {
				if ((i = borrow[p][k]) > 0) {
					if (!nl++)
						putc('\n', rfp);
					fprintf(rfp, "%s:%*s Borrows %d ducat%s for %d year%s",
						powers[p], LPOWER - strlen(powers[p]), "", i, i == 1 ? "" : "s",
						k + 1, k == 0 ? "" : "s");
					if (ducats[p].interest[0] < 0) {
						fprintf(rfp, ".  ** Disallowed -- prior default\n");
						continue;
					}
					for (l = j = 0; j < 6; j++)
						l += ducats[p].loan[j];

					if (l + i > MAX_LOAN) {
						fprintf(rfp, ".  ** MAX principle is %d ducats\n", MAX_LOAN);
						i = MAX_LOAN - l;
						fprintf(rfp, "%s:%*s Borrows %dd for %d year%s",
							powers[p], LPOWER - strlen(powers[p]), "", i, k + 1, k == 0 ? "" : "s");
					}
					ducats[p].loan[l = (k == 0 ? 3 : 6)] += i;
					ducats[p].interest[l] += j = (k == 0 ? (i + 4) / 5 : (i + 1) / 2);
					fprintf(rfp, ", %dd due in %s of %d.\n", i + j,
						dipent.phase[0] == 'S' ? "Spring" :
						dipent.phase[0] == 'U' ? "Summer" : "Fall",
						atoi(dipent.phase + 1) + (k == 0 ? 1 : 2));
					ducats[p].treasury += i;
				}
			}

			/*
			 * Let him know with whom he's declared alliance.
			 */

			if (!processing) {
				int n = 0;
				for (i = 1; i < WILD_PLAYER; i++) {
					if (dipent.pl[i] == 'x' || i == p || !allies[p][i])
						continue;
					if (!n++) {
						fprintf(rfp, "\n%s allowing ", powers[p]);
					} else {
						fputs(" and ", rfp);
					}
					fputs(powers[i], rfp);
				}
				if (n) {
					fputs(" to use straits.\n", rfp);
					nl++;
				}
			}
		}
	}

	/*
	 *  Now the bank collects any outstanding loans.
	 */

	for (p = 1; p < WILD_PLAYER; p++) {
		if (processing || pt == p || pt == MASTER) {
			if ((i = ducats[p].loan[0] + ducats[p].interest[0]) > 0) {
				if (!nl++)
					putc('\n', rfp);
				fprintf(rfp, "%s:%*s Bank collects %d ducat%s.",
					powers[p], LPOWER - strlen(powers[p]), "", i, i == 1 ? "" : "s");
				if (i > ducats[p].treasury) {
					i = ducats[p].treasury;
					ducats[p].treasury = 0;
					fprintf(rfp, "  ** NSF = %dd, assassinated", i);
					assassin[p]++;
					for (j = 0; j < 7; j++) {
						ducats[p].loan[j] = 0;
						ducats[p].interest[j] = -1;
					}
				} else {
					ducats[p].treasury -= i;
				}
				putc('\n', rfp);
			}
			if (ducats[p].interest[0] >= 0) {
				for (j = 1; j < 7; j++) {
					ducats[p].loan[j - 1] = ducats[p].loan[j];
					ducats[p].interest[j - 1] = ducats[p].interest[j];
				}
				ducats[p].loan[6] = 0;
				ducats[p].interest[6] = 0;
			}
		}
	}

	/*
	 *  Verify expenditures.
	 */

	memset(amount, 0, sizeof(amount));
	memset(counter, 0, sizeof(counter));
	memset(result, 0, sizeof(result));

	/*
	 *  Pass 1: Make sure everyone has enough money.
	 */

	for (p = 1; p < WILD_PLAYER; p++) {
		if (dipent.pl[p] == 'x')
			continue;
		if (processing || pt == p || pt == MASTER) {
			for (j = 0; j < 4; j++) {
				if (expense[p][j].amount <= 0)
					expense[p][j].type = 0;
				if (expense[p][j].type != 0) {
					if ((i = expense[p][j].amount) > ducats[p].treasury) {
						i = ducats[p].treasury;
					}
					ducats[p].treasury -= i;
					/* Isn't this check performed below ? */
/*          i -= i % 3; */
					amount[p][j] = i;

					/*
					   **  Verify that there is an adjacent unit to do the bribe.
					 */

					if (!NO_ADJACENCY
					    && (bribe(expense[p][j].type) || expense[p][j].type == 'c')) {
						for (bp = pr[p1 = unit[expense[p][j].unit].loc].move; *bp; bp++) {
							if (((u = pr[*bp].unit) && unit[u].owner == p) ||
							    ((u = pr[*bp].gunit) && unit[u].owner == p)) {
								break;
							}
							bp++;
						}
						if (!(*bp || ((u = pr[p1].unit) && unit[u].owner == p)
						      || ((u = pr[p1].gunit) && unit[u].owner == p))) {
							result[p][j] = NOADJACENT;
						}
					}
				}
			}
		}
	}

	/*
	   **  Pass 2: Find the largest bribe for each unit, sum counter-bribes.
	 */

	if (processing || pt == MASTER) {
		for (p1 = 1; p1 < WILD_PLAYER; p1++) {
			if (dipent.pl[p1] == 'x')
				continue;
			for (i = 0; i < 4; i++) {
				if (expense[p1][i].type == 'c') {
					counter[expense[p1][i].unit] += amount[p1][i];
				} else if (!result[p1][i] && bribe(expense[p1][i].type)) {
					for (p2 = 1; p2 < WILD_PLAYER; p2++) {
						if (dipent.pl[p2] == 'x')
							continue;
						for (j = 0; j < 4; j++) {
							if (p1 == p2 && i == j)
								continue;
							if (!result[p2][j] && bribe(expense[p2][j].type) &&
							    expense[p2][j].unit == expense[p1][i].unit) {
								if (amount[p2][j] == amount[p1][i]) {
									result[p1][i] = BOUNCE;
									result[p2][j] = BOUNCE;
								} else if (amount[p2][j] > amount[p1][i]) {
									result[p1][i] = BOUNCE;
								} else if (amount[p2][j] < amount[p1][i]) {
									result[p2][j] = BOUNCE;
								}
							}
						}
					}
				}
			}
		}
	}
	/*
	   **  Pass 3: Print out results.
	 */

	for (p = 1; p < WILD_PLAYER; p++) {
		if (processing || pt == p || pt == MASTER) {
			n1 = 0;
			for (j = 0; j < 4; j++) {
				if (expense[p][j].type != 0) {
					if (!n1++)
						putc('\n', rfp);
					nl++;
					i = expense[p][j].amount;
					u = expense[p][j].unit;
					fprintf(rfp, "%s:%*s E%d: %2dd, ",
						powers[p], LPOWER - strlen(powers[p]), "", j + 1, i);
					switch (expense[p][j].type) {
					case 'a':
						fprintf(rfp, "Assassinate %s", powers[u]);
						minval = 12;
						break;

					case 'b':
						fprintf(rfp, "Buy %s %s%s %s", owners[unit[u].owner],
							bstype(unit[u].stype), utype(unit[u].type),
						   pr[unit[u].loc].name);

						minval = unit[u].owner == AUTONOMOUS ? 9 : 18;

						/*
						   **  You can only have one special unit, don't allow another
						   **  to be purchased.
						 */

						if (unit[u].stype != 'x') {
							for (u1 = 1; u1 <= nunit; u1++) {
								if (u == u1 || unit[u1].proxy == u)
									continue;
								if (unit[u1].owner == p && unit[u1].stype != 'x') {
									result[p][j] = TOO_MANY;
								}
							}
						}
						/* 
						 * Moved block of code to part that can not be processed 
						 * multiple times for the same unit
						 */
						break;

					case 'c':
						fprintf(rfp, "Counter-Bribe ");
						if (unit[u].owner != p)
							fprintf(rfp, "%s ", owners[unit[u].owner]);
						fprintf(rfp, "%s%s %s", bstype(unit[u].stype), utype(unit[u].type),
						   pr[unit[u].loc].name);
						minval = 3;
						break;

					case 'd':
						fprintf(rfp, "Disband %s %s%s %s", owners[unit[u].owner],
							bstype(unit[u].stype), utype(unit[u].type),
						   pr[unit[u].loc].name);

						minval = unit[u].owner == AUTONOMOUS ? 6 : 12;
						break;

					case 'f':
						fprintf(rfp, "Famine Relief %s", pr[u].name);
						minval = 3;
						break;

					case 'g':
						fprintf(rfp, "%s garrison %s to autonomous", owners[unit[u].owner],
						   pr[unit[u].loc].name);
						minval = 9;
						break;

					case 'p':
						fprintf(rfp, "Pacify Rebellion %s", pr[u].name);
						minval = 12;
						break;

					case 'r':
						fprintf(rfp, "Cause Rebellion %s", pr[u].name);
						minval = pr[u].home == pr[u].owner ? 15 : 9;
						break;

					default:
						fprintf(rfp, "Unknown expense %c", expense[p][j].type);
						fprintf(log_fp, "Unknown expense %c", expense[p][j].type);
						minval = 1000;
					}

					if ((i = expense[p][j].amount % 3)) {
						fprintf(rfp, "  ** Not 3x, %d wasted\n", i);
						expense[p][j].amount -= i;
						j--;
						continue;
					}
					if (expense[p][j].amount > amount[p][j]) {
						fprintf(rfp, "  ** NSF\n");
						expense[p][j].amount = amount[p][j];
						/* Causes this expenditure to be handled for the second time ! */
						j--;
						continue;
					}
					if (result[p][j] == BOUNCE) {
						fprintf(rfp, "  ** Larger bribe, wasted\n");
						continue;
					}
					if (result[p][j] == NOADJACENT) {
						fprintf(rfp, "  ** No adjacency, wasted\n");
						continue;
					}
					if (result[p][j] == TOO_MANY) {
						fprintf(rfp, "  ** Existing special unit, wasted\n");
						continue;
					}
					if (bribe(expense[p][j].type)) {
						if (counter[u] > 0) {
							fprintf(rfp, "  ** Counter-Bribed\n");
							expense[p][j].amount -= counter[u];
							counter[u] = 0;
							j--;
							continue;
						}
						/* 
						 * Inserted old code here
						 */
						/*
						   **  If we're not processing, pretend like purchases worked so 
						   **  he'll be reminded that he needs to submit orders if he hasn't.
						 */

						if (expense[p][j].type == 'b') {
							if (!processing && p == pt) {
								p2 = unit[u].loc;
								if (has_rebellion(p2)) {
									fprintf(rfp, "Rebellion in %s liberated.\n", pr[p2].name);
									remove_rebellion(p2);
								}
								for (u1 = 1; u1 <= nunit; u1++) {
									if (unit[u1].proxy == u && unit[u1].owner == p) {
										if (is_garrison(u)) {
											pr[p2].gunit = u1;
										} else {
											pr[p2].unit = u1;
										}
										unit[u1].proxy = 0;

										for (u2 = 1; u2 <= nunit; u2++) {
											if (unit[u2].unit == u) {
												unit[u2].unit = u1;
											}
											if (unit[u2].order == 'm' && unit[u2].convoy != NULL) {
												unsigned char *s;
												for (s = unit[u2].convoy; *s; s++) {
													if (*s == u)
														*s = u1;
												}
											}
										}
										break;
									}
								}
								if (u1 > nunit) {
									unit[u].owner = p;
									unit[u].order = 'n';
								}
							}
						}
						/* End of moved code */

						if (unit[u].stype == 'c' || unit[u].stype == 'p') {
							minval *= 2;	/* Special forces units doubled */
						}
						if (unit[u].type == 'G' && cityvalue(unit[u].loc) > 1) {
							minval *= 2;	/* Garrisons in major cities doubled */
						}
					}
					if ((i = expense[p][j].amount) < minval) {
						fprintf(rfp, "  ** %dd required, wasted\n", minval);
						result[p][j] = 1;
						continue;
					}
					if (!processing && i > minval) {
						fprintf(rfp, "  ** %d+%d ducats", minval, i - minval);
					}
					if (expense[p][j].type == 'a') {
						for (bp = chits[p]; *bp && *bp != u; bp++);
						if (!*bp) {
							fprintf(rfp, "  ** No chit, wasted\n");
							result[p][j] = 1;
							continue;
						}
						i = expense[p][j].amount / 12;
						if (i > 3) {
							fprintf(rfp, "  ** 36 ducats maximum, rest wasted\n");
							expense[p][j].amount = 36;
							j--;
							continue;
						}
						*bp = WILD_PLAYER;
						if (processing || pt == MASTER) {
							die_rolls(DIE_EXPENSE + (p * 10) + j);
							if (i < (k = die(1, 6))) {
								fprintf(rfp, "  ** Die is %d, needed <%d, failed\n", k, i + 1);
								result[p][j] = 1;
							} else {
								fprintf(rfp, "  ** Die is %d, SUCCESS!\n", k);
								assassin[u]++;
							}
							continue;
						}
					}
					fprintf(rfp, ".\n");

				}
			}
		}
	}

	/*
	   **  Pass 4: Execute the successful expenditures.
	 */

	if (processing || pt == MASTER) {
		for (p = 1; p < WILD_PLAYER; p++) {
			if (dipent.pl[p] == 'x')
				continue;
			for (j = 0; j < 4; j++) {
				if (result[p][j] == 0) {
					u = expense[p][j].unit;
					switch (expense[p][j].type) {
					case 'b':
						p2 = unit[u].loc;
						if (has_rebellion(p2)) {
							fprintf(rfp, "Rebellion in %s liberated.\n", pr[p2].name);
							remove_rebellion(p2);
						}
						for (u1 = 1; u1 <= nunit; u1++) {
							if (unit[u1].proxy == u && unit[u1].owner == p) {
								if (is_garrison(u)) {
									pr[p2].gunit = u1;
								} else {
									pr[p2].unit = u1;
								}
								unit[u1].proxy = 0;
								unit[u].owner = 0;

								for (u2 = 1; u2 <= nunit; u2++) {
									if (unit[u2].unit == u) {
										unit[u2].unit = u1;
									}
									if (unit[u2].order == 'm' && unit[u2].convoy != NULL) {
										unsigned char *s;
										for (s = unit[u2].convoy; *s; s++) {
											if (*s == u)
												*s = u1;
										}
									}
								}
								break;
							}
						}
						if (u1 > nunit) {
							unit[u].owner = p;
							unit[u].order = 'n';
						}
						if (!water(p2)) {
							if (is_garrison(u)) {
								ncown[p2] = p;
								if (!pr[p2].unit) {
									npown[p2] = p;
								}
							} else {
								npown[p2] = p;
								if (!has_garrison(p2)) {
									ncown[p2] = p;
								}
							}
						}
						break;

					case 'd':
						unit[u].owner = 0;
						unit[u].order = 'n';
						if (is_sieged(p1 = unit[u].loc)) {
							remove_siege(p1);
							fprintf(rfp, "Siege on %s lifted.\n", pr[p1].name);
						}
						break;

					case 'f':
						remove_infection(u);
						break;

					case 'g':
						unit[u].owner = AUTONOMOUS;
						break;

					case 'p':
						remove_rebellion(u);
						break;

					case 'r':
						set_rebellion(u);
						break;
					}
				}
			}
		}
	}
	/*
	   **  Pass 5: Process assassinations.
	 */

	if (processing || pt == MASTER) {
		for (p = 1; p < WILD_PLAYER; p++) {
			if (dipent.pl[p] == 'x' || !assassin[p])
				continue;
			fprintf(rfp, "\n%s leader assassinated.\n", owners[p]);
			nl++;
			for (u = 1; u <= nunit; u++) {
				if (unit[u].owner == p) {
					unit[u].order = 'n';
					if (is_sieged(p1 = unit[u].loc)) {
						remove_siege(p1);
						if (unit[u].type == 'G') {
							unit[u].owner = 0;
							ncown[p1] = unit[pr[p1].unit].owner;
							pr[p1].gunit = 0;
							fprintf(rfp, "Besieged Garrison %s disbands.\n", pr[p1].name);
						} else {
							fprintf(rfp, "Siege on %s lifted.\n", pr[p1].name);
						}
					}
				}
			}

			die_rolls(DIE_ASSASSIN + p);
			fprintf(rfp, "Checking for rebellions...\n");
			fprintf(rfp, "   Die  Status     Province\n");
			for (p1 = 1; p1 <= npr; p1++) {
				if (pr[p1].owner == p) {
					k = die(1, 6);
					j = pr[p1].unit || has_garrison(p1) ? (pr[p1].home == p ? 1 : 3) :
					    (pr[p1].home == p ? 2 : 5);
					if (k <= j) {
						fprintf(rfp, "%5d   Rebellion  %s\n", k, pr[p1].name);
						set_rebellion(p1);
					} else {
						if (has_rebellion(p1)) {
							fprintf(rfp, "%5d   No change  %s\n", k, pr[p1].name);
						} else {
							fprintf(rfp, "%5d   Okay       %s\n", k, pr[p1].name);
						}
					}
				}
			}
		}
	}
	if (!nl)
		fputs("\nNone.\n", rfp);
	fputc('\n', rfp);

	/*
	   **  Make sure the map properly reflects disbanded units.
	 */

	for (p = 1; p <= npr; p++) {
		if ((u = pr[p].unit) && unit[u].owner <= 0) {
			pr[p].unit = 0;
			if ((u = has_garrison(p)) && unit[u].owner > 0) {
				npown[p] = unit[u].owner;
			}
		}
		if ((u = pr[p].gunit) && unit[u].owner <= 0) {
			pr[p].gunit = 0;
			if ((u = pr[p].unit) && unit[u].owner > 0) {
				ncown[p] = unit[u].owner;
			}
		}
	}

}

/*
 * if (first word of stream == word)
 *     skip this word and return true
 * else
 *     return false
 */
int testword(char **stream, char *word)
{
	int l = strlen(word);

	if (!strncasecmp(*stream, word, l)) {
		*stream += l;
		return 1;
	}
	return 0;
}
