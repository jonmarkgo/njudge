/*
 * $Log$
 * Revision 1.12  2003/09/14 08:03:05  millis
 * Do not flag some errors in Mach data (just ignore them)
 *
 * Revision 1.11  2003/05/03 23:33:50  millis
 * Fix bug 150 (NO_GARRISONS flag)
 *
 * Revision 1.10  2003/05/02 22:29:16  millis
 * Added support for neutrals and artillery units
 *
 * Revision 1.9  2003/01/14 13:56:46  millis
 * Updated with ustv merged changed
 *
 * Revision 1.8  2002/08/27 22:27:56  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.7  2002/04/15 12:55:45  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.5.2.2  2001/10/19 23:45:15  dema
 * Handle NoMoney settings and initial_money
 *
 * Revision 1.5.2.1  2001/10/15 00:27:36  ustv
 * Added Honk-kong, Railway and gateway variable initialisation
 *
 * Revision 1.5  2001/07/15 09:19:36  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.4  2001/07/01 23:19:29  miller
 * Add storm table and unit limits
 *
 * Revision 1.3  2000/11/14 14:27:37  miller
 * Added blockage handling (where a wing unit doesn't caputure a SC but blockades it, rendering it unbuildable for both parties)
 *
 * Revision 1.2  1998/03/01 12:54:16  davidn
 * Second fix to allow w and x to be power letters.
 * Fix was needed in two places, and was originally only put in one.
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1997/03/12 21:33:31  davidn
 * Fix to allow w and x to be handled correctly as power letters in
 * variants, without clshing with neutral scs and water provinces.
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  init.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "dip.h"
#include "porder.h"
#include "mach.h"
#include "functions.h"

static int variant = 0;		/* The currently loaded variant */

void UpdateBlockades()
{
	int i,u;
	
	
	for (u = 0; u<dipent.np; u++) {
               	dipent.players[u].centres_blockaded = 0;
       	}

        if ((dipent.flags & F_WINGS)){
                /* Firstly, set all provinces to 'unblockaded' */
                for (i = 1; i <= npr ;i++) {
                        pr[i].blockaded = 0;
                }
                /* Now, go through all units and set to blockaded if occupied by an
                   enemy wing
                 */
                 for (u = 1; u <= nunit; u++) {
                        if (unit[u].status == ':' &&
			    pr[unit[u].loc].owner != 0 ) {
                                /* only non-retreating, existing  units */
                                if (
//				    pr[unit[u].loc].type == 'x' &&
				    (unit[u].type == 'W' ) && 
				    unit[u].owner != pr[unit[u].loc].owner) {
					i = FindPower(pr[unit[u].loc].owner);
                                       	++dipent.players[i].centres_blockaded;
                                	pr[unit[u].loc].blockaded = 1;
				}
                        }
                }
        }
}

static void CreateDummyRailwayUnits(void)
{
/* Dummy units are needed for railways. One is created per railway, with owner = 0 */
    int i;

    if (!(dipent.x2flags & X2F_RAILWAYS)) return;  /* No railways, so no dummies! */
    for (i = 0; i < nrw; i++) {
       unit[++nunit].loc = rw[i].prov_index;
       pr[rw[i].prov_index].unit = nunit;
       unit[nunit].owner = 0;   /* No owner, dummy province */
       unit[nunit].railway_index = -1;  
    }
}
static void CreateDummyGatewayUnits(void)
{
/* Dummy units are needed for gateways. One is created per gateway, with owner = 0 */
    int i;

    if (!(dipent.x2flags & X2F_GATEWAYS)) return;  /* No gateways, so no dummies! */

    for (i = 0; i < ngw; i++) {
       unit[++nunit].loc = gw[i].prov_index;
       pr[gw[i].prov_index].unit = nunit;
       unit[nunit].owner = 0;   /* No owner, dummy province */
       unit[nunit].railway_index = -1;
       unit[nunit].type = ' ';  /* no type! */
       unit[nunit].stype = 'x'; /* Not a special type */
       unit[nunit].order = 'h'; /* default hold order */
       if (pr[gw[i].gw_prov].unit)
	   unit[nunit].owner = unit[pr[gw[i].gw_prov].unit].owner;
    }
}

int po_init(void)
{

/*
 *  Initialize, read the data structures off the input files.
 */

	int i, n;
	err = 0;

	for (i = 1; i < WILD_PLAYER; i++)
		need_order[i] = 0;

	memset(ducats, 0, sizeof(ducats));

	if (hpx != 0 && variant == dipent.variant) {
		hp = hpx;
		for (i = 0; i <= npr; i++) {
			n = islower(pr[i].type) ? 0 : power(pr[i].type);
			pr[i].owner = n;
			pr[i].new_owner = n;
			pr[i].cown = n;
			pr[i].home = n;
			pr[i].unit = 0;
			pr[i].gunit = 0;
			pr[i].blockaded = 0;
			pr[i].flags &= PF_CONSTANT;
			pr[i].unit_held = 1;  /* Default, has a held unit, replaced later */
		}
	} else {

		variant = dipent.variant;
		if (hp)
			free(heap);

		sprintf(line, "map.%d", variant);
		if ((ifp = fopen(line, "r")) == NULL) {
			fprintf(rfp, "Error opening map data file %s.\n", line);
			return E_FATAL;
		}
		if (fread(&npr, sizeof(npr), 1, ifp) != 1 ||
		    fread(&hp, sizeof(hp), 1, ifp) != 1 ||
		    fread(&nv, sizeof(nv), 1, ifp) != 1) {
			fprintf(rfp, "Error reading map file npr/hp/nv, %s.\n", line);
			return E_FATAL;
		}
		maxheap = hp + 2048;
		if (!(heap = (unsigned char *) malloc(maxheap + 10))) {
			fprintf(rfp, "Unable to allocate heap.\n");
			return E_FATAL;
		}
		for (i = 1; i <= npr; i++) {
			if (fread(cmap, sizeof(cmap[0]), CMAP_SIZE, ifp) != CMAP_SIZE) {
				fprintf(rfp, "cmap read error prov = %d, %s.\n", i, line);
				return E_FATAL;
			}
			pr[i].name = (char *) &heap[cmap[CMAP_NAME]];
			pr[i].move = (unsigned char *) &heap[cmap[CMAP_MOVE]];
			pr[i].type = cmap[CMAP_TYPE];
			pr[i].flags = cmap[CMAP_FLAG];
			pr[i].type2 = cmap[CMAP_TYPE2];
			n = islower(pr[i].type) ? 0 : power(pr[i].type);
			pr[i].owner = n;
			pr[i].new_owner = n;
			pr[i].cown = n;
			pr[i].home = n;
			pr[i].unit = 0;
			pr[i].gunit = 0;
			pr[i].unit_held = 1;
			pr[i].order_index =0;
			if (pr[i].type == 'r' || pr[i].type == 'g' )
			    pr[i].move = NULL;
		}

		if ((i = fread(heap, sizeof(unsigned char), hp, ifp)) != hp) {
			fprintf(rfp, "cmap heap read error, %d of %d, %s.\n", i, hp, line);
			return E_FATAL;
		}
		if (nv > MAXVINC) {
			fprintf(rfp, "Maximum variable income exceeded.\n");
			return E_FATAL;
		}
		if (nv > 0) {
			if ((i = fread(vincome, sizeof(vincome[0]), nv, ifp)) != nv) {
				fprintf(rfp, "cmap income read error, %d of %d, %s.\n", i, nv, line);
				return E_FATAL;
			}
			if ((i = fread(ftab, sizeof(ftab), 1, ifp)) != 1) {
				fprintf(rfp, "cmap ftab read error, %d. %s\n", i, line);
				return E_FATAL;
			}
			if ((i = fread(ptab, sizeof(ptab), 1, ifp)) != 1) {
				fprintf(rfp, "cmap ptab read error, %d. %s\n", i, line);
				return E_FATAL;
			}
		 	if ((i = fread(stab, sizeof(stab), 1, ifp)) != 1) {
                                fprintf(rfp, "cmap stab read error, %d. %s\n", i, line);
                                return E_FATAL;
                        }
			if ((i = fread(permitted_units, sizeof(permitted_units), 1, ifp)) != 1) {
                                fprintf(rfp, "cmap permitted_units read error, %d. %s\n", i, line);
                                return E_FATAL;
                        }
                        if ((i = fread(&initial_money, sizeof(initial_money), 1, ifp)) != 1) {
                                fprintf(rfp, "cmap initial_money read error, %d. %s\n", i, line);
                                return E_FATAL;
                        }                                                                                                        
		}
/* Now read colonial hk, gw and rw settings */
        if (fread(&nhk, sizeof(nhk),1, ifp) != 1) {
            /* Serious error, what to do? */
            nhk = 0;
        }
        if (nhk > 0) {
            if(fread(hk, sizeof(hk[0]), nhk, ifp) != nhk) {
                fprintf(ifp, "Mismatch on number of hk records.\n");
                bailout(1);
            }
        }
        if (fread(&ngw, sizeof(ngw),1, ifp) != 1) {
            /* Serious error, what to do? */
            ngw = 0;
        }
        if (ngw > 0) {
            if(fread(gw, sizeof(gw[0]), ngw, ifp) != ngw) {
                fprintf(ifp, "Mismatch on number of gw records.\n");
                bailout(1);
            }
        }
        if (fread(&nrw, sizeof(nrw),1, ifp) != 1) {
            /* Serious error, what to do? */
            nrw = 0;
        }
        if (nrw > 0) {
            if(fread(rw, sizeof(rw[0]), nrw, ifp) != nrw) {
                fprintf(ifp, "Mismatch on number of rw records.\n");
                bailout(1);
            }
        }
/* End of colonial reading */
		fclose(ifp);

		/*
		 * Check movement table for consistancy.
		 */

		po_chkmov();
		hpx = hp;
	}
	UpdateBlockades();

	dipent.has_multi_unit_provs = CheckForMultiUnitProvinces();

	return err;
}


/****************************************************************************/

void po_chkmov(void)
{

/* Check movement table for consistancy */

	int from, to, i, j;
	unsigned char *s, *t;


	for (from = 1; from <= npr; from++) {
		if (!(s = pr[from].move)) {
			if (pr[from].type != 'r' && pr[from].type != 'g')
			    fprintf(rfp, "No movement table for %s.\n", pr[from].name);
			continue;
		}
		while ((to = *s++)) {
			if (!(t = pr[to].move)) {
				if (pr[to].type != 'r' && pr[to].type != 'g')
				    fprintf(rfp, "No movement table for %s.\n", pr[to].name);
				continue;
			}
			i = *s++ & 0x0f;
			while ((j = *t++) && ((j != from) || (i != MX && i != HX && i != LX && i != *t >> 4)))
				t++;
			if (!j) {
				/* TODO of course this may be deliberate... */
				fprintf(rfp, "Can't get back to %s from %s (%s).\n",
					pr[from].name,
					pr[to].name, mtype[i]);
				err++;
			}
		}
	}
}

int gamein(void)
{

	int i = 0, j, p1, p, c1;
	int index, counter;
	char c, phase[15], *s, *t;
	char buffer[1024];

	/* Read in current position */

	sprintf(line, "%s%s/G%s", GAME_DIR, dipent.name, dipent.seq);
	if ((ifp = fopen(line, "r")) == NULL) {
		fprintf(rfp, "Error opening game position data file %s.\n", line);
		return E_FATAL;
	}
	fgets(line, sizeof(line), ifp);
	line[strlen(line) - 1] = '\0';
	line[sizeof(phase) - 1] = '\0';
	strcpy(phase, line);
	if ((phase[0] != 'S' && phase[0] != 'F' && phase[0] != 'U') ||
	    !isdigit(phase[1]) || !isdigit(phase[2]) ||
	    !isdigit(phase[3]) || !isdigit(phase[4]) ||
	    !isalpha(phase[5])) {
		fprintf(rfp, "Invalid phase %s in game file.\n", phase);
		return E_FATAL;
	}
	if (strncmp(phase, dipent.phase, 6)) {
		if (strcmp(dipent.phase, "?1901?")) {
			fprintf(rfp, "Phase %s for '%s' does not match master file.\n",
				dipent.phase, dipent.name);
			err++;
		}
		fprintf(rfp, "Assuming game phase of %s in file %s.\n", phase, line);
		strcpy(dipent.phase, phase);
	}
	/*
	 *  Read unit positions "E: xF London".
	 */

	nunit = 0;
	memset(unit, 0, sizeof(unit));
	while (fgets(line, sizeof(line), ifp) && *line != '-') {
		if (nunit == MAXUNIT - 1) {
			fprintf(rfp, "Maximum number of units (%d) exceeded.\n", nunit);
			err++;
			break;
		}
		if (!(unit[++nunit].owner = power(*line))) {
			fprintf(rfp, "Invalid unit owner %c for unit %d.\n", *line, nunit);
			err++;
		}
		
		unit[nunit].railway_index = -1; /* not yet using a railway */
		unit[nunit].status = line[1];
		unit[nunit].exists = 1;  /* reading it in, so it must exist! */
		s = &line[3];
		unit[nunit].stype = islower(*s) ? *s++ : 'x';
		c = unit[nunit].type = *s++;
		if (c == 'A')
			unit[nunit].coast = MV;
		else if (c == 'G')
			unit[nunit].coast = MV;
		else if (c == 'F')
			unit[nunit].coast = XC;
		else if (c == 'W')
			unit[nunit].coast = MV;
		else if (c == 'T')
			unit[nunit].coast = MV;
		else if (c == 'R')
                        unit[nunit].coast = MV;
		else {
			fprintf(rfp, "Invalid type %c for unit %d.\n", c, nunit);
			err++;
		}

		s = get_prov(s, &p1, &c1);
		if (!(unit[nunit].loc = p1)) {
			fprintf(rfp, "Unknown province %s for unit %d.\n", s, nunit);
			err++;
		}
		if (c1)
			unit[nunit].coast = c1;

		/*
		 *  If retreating build list of retreat provinces in convoy chain.
		 */

		unit[nunit].convoy = NULL;
		if (unit[nunit].status == 'r') {
			counter = 0;
			unit[nunit].convoy = &heap[hp];
			while (p1 && *s++ == ',') {
				s = get_prov(s, &p1, &c1);
				if (p1)
					heap[hp++] = p1;

				/* save the coasts */
				buffer[counter++] = c1 ? c1 : 0;
			}
			heap[hp++] = 0;

			/* put the coasts on the heap */
			unit[nunit].rcoast = &heap[hp];
			for (index = 0; index < counter; index++)
				heap[hp++] = buffer[index];
			heap[hp++] = 0;
		}
		/*
		 *  Otherwise link the province back to the unit.
		 */

		else if (unit[nunit].type == 'G') {
			if (pr[p1].gunit) {
				fprintf(rfp, "More than one garrison in %s.\n", pr[p1].name);
				err++;
			} else {
				pr[unit[nunit].loc].gunit = nunit;
			}
		} else if (pr[p1].unit && !IsMultiProvince(p1)) {
			fprintf(rfp, "More than one unit in province %s.\n", pr[p1].name);
			err++;
		} else {
			pr[unit[nunit].loc].unit = nunit;
		}

		if ((dipent.x2flags & X2F_NOGARRISONS) &&
		    unit[nunit].type == 'G') {
		    /* Game does not allow garrisons, so remove it */
		    pr[unit[nunit].loc].gunit = 0;
		    nunit--;
		}
	}

	for (i= nunit+1; i < MAXUNIT; i++) 
	    unit[i].exists = 0;


	/*
	 *  Read in ownership of supply centers, one character per province.
	 */

	if (dipent.flags & F_MACH) {
		memset(allies, 0, sizeof(allies));
		for (i = 0; i < WILD_PLAYER; i++) {
			allies[i][i] = 1;
		}
		memset(npown, 0, sizeof(npown));
		memset(ncown, 0, sizeof(ncown));
	}
	fgets(line, sizeof(line), ifp);
	if (*line != '-') {
		if (line[1] == ':') {
			j = 1;
			while (fgets(line, sizeof(line), ifp) && line[1] == ':') {
				switch (*line) {

					/*
					 *  a: Alliance record: X ppppp  (Retreat phase only)
					 */

				case 'a':
					t = line + 2;
					while (isspace(*t))
						t++;
					if (!(i = power(*t))) {
						fprintf(rfp, "Unknown power %c on '%s' alliance: %s",
						  *t, dipent.name, line);
						fprintf(log_fp, "Unknown power %c on '%s' alliance: %s",
						  *t, dipent.name, line);
						/*err++;*/
					} else {
						while (*t && !isspace(*t))
							t++;
						while (isspace(*t))
							t++;
						while (*t && !isspace(*t)) {
							if (!(j = power(*t))) {
								fprintf(rfp, "Unknown power %c on '%s' alliance: %s",
									*t, dipent.name, line);
								fprintf(log_fp, "Unknown power %c on '%s' alliance: %s",
									*t, dipent.name, line);
								/*err++;*/
							} else {
								allies[i][j] = 1;
							}
							t++;
						}
					}
					break;

					/*
					 *  A: Assassination chit ownership: X ppppp
					 */

				case 'A':
					t = line + 2;
					while (isspace(*t))
						t++;
					if (!(i = power(*t))) {
						fprintf(rfp, "Unknown power %c on '%s' assassinate: %s",
						  *t, dipent.name, line);
						fprintf(log_fp, "Unknown power %c on '%s' assassinate: %s",
						  *t, dipent.name, line);
						/*err++;*/
					} else {
						while (*t && !isspace(*t))
							t++;
						while (isspace(*t))
							t++;
						for (j = 0; j < MAX_CHIT && *t && !isspace(*t); t++, j++) {
							if (!(chits[i][j] = power(*t))) {
								fprintf(rfp, "Unknown power %c on '%s' assassinate: %s",
									*t, dipent.name, line);
								fprintf(log_fp, "Unknown power %c on '%s' assassinate: %s",
									*t, dipent.name, line);
								/*err++;*/
							}
						}
						if (j == MAX_CHIT) {
							fprintf(rfp, "Too many %d assassinates (%s): %s", j, dipent.name, line);
							fprintf(log_fp, "Too many %d assassinates (%s): %s", j, dipent.name,
								line);
							err++;
						}
					}
					break;

					/*
					 *  C: City ownership
					 */

				case 'C':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						pr[i].cown = power(*t);
					break;

					/*
					 *  c: New city ownership
					 */

				case 'c':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						ncown[i] = power(*t);
					break;

					/*
					 *  D: Ducat treasury and loan information.
					 */

				case 'D':
					t = line + 2;
					while (isspace(*t))
						t++;
					if (!(i = power(*t))) {
						fprintf(rfp, "Unknown power on '%s' ducats line: %s",
						      dipent.name, line);
						fprintf(log_fp, "Unknown power on '%s' ducats line: %s",
						      dipent.name, line);
						/*err++;*/
					} else {
						while (*t && !isspace(*t))
							t++;
						sscanf(t, "%hd%hd%hd%hd%hd%hd%hd%hd%hd%hd%hd%hd%hd",
						     &ducats[i].treasury,
						       &ducats[i].loan[0], &ducats[i].interest[0],
						       &ducats[i].loan[1], &ducats[i].interest[1],
						       &ducats[i].loan[2], &ducats[i].interest[2],
						       &ducats[i].loan[3], &ducats[i].interest[3],
						       &ducats[i].loan[4], &ducats[i].interest[4],
						       &ducats[i].loan[5], &ducats[i].interest[5]);
					}
					break;

					/*
					 *  F: Province flags
					 */

				case 'F':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						pr[i].flags |= (*t - '0') << 8;
					break;

					/*
					 *  H: Province home ownership
					 */

				case 'H':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						pr[i].home = power(*t);
					break;

					/*
                                         *  S: Unit held state, '1' if unit held from last turn
					 *  			'0' if didn't or was empty
                                         */

                                case 'S':
                                        for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
                                                pr[i].unit_held = *t == '0' ? 0 : 1;
                                        break;

					/*
					 *  N: Comment, province name.  Note: OCHF must follow N.
					 */

				case 'N':
					j = i;
					break;

					/*
					 *  n: Comment, province name second and third characters.
					 */

				case 'n':
					break;

					/*
					 *  O: Province ownership.
					 */

				case 'O':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						pr[i].owner = power(*t);
					break;

					/*
					 *  o: New province ownership
					 */

				case 'o':
					for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
						npown[i] = power(*t);
					break;

				default:
					fprintf(rfp, "Unknown owner code for '%s'.\n%s", dipent.name, line);
					fprintf(log_fp, "Unknown owner code for '%s'.\n%s", dipent.name, line);
					err++;
					break;
				}
			}

		} else {	/* Old pre-Machiavelli format */

			for (t = line, i = 0; *t && !isspace(*t); t++) {
				if (i > NPROV) {
					fprintf(rfp, "Maximum provinces exceeded reading ownership line.\n");
					err++;
					break;
				}
				pr[++i].owner = power(*t);
			}
		}

		/*
		 *  Skip over player list.
		 */

		while ((t = fgets(line, sizeof(line), ifp)))
			if (*line == '-')
				break;

		if (!t) {
			fprintf(rfp, "Missing '-1' after ownership record.\n");
			err++;
		}
	}
	fclose(ifp);
      
        UpdateBlockades();

        if (!strcmp(dipent.seq, "001"))
          if (dipent.x2flags & X2F_NEUTRALS)  {
           /* We've got Neutrals so set them up */
            for (p = 1; p <= npr; p++) {
                if (pr[p].type == 'x' && !pr[p].unit) {
                   unit[++nunit].type = 'A';
		   unit[nunit].stype = 'x';
                   unit[nunit].owner = NEUTRAL;
                   unit[nunit].loc = p;
		   unit[nunit].exists = 1; 
		   unit[nunit].railway_index = -1;
                   unit[nunit].order = 'h';
		   unit[nunit].status = ':';
		   pr[p].unit = nunit;
                }
            }
          }
	CreateDummyRailwayUnits();
	CreateDummyGatewayUnits(); 
	
	for (p = 1; p <= npr; p++) {
	    if (!pr[p].unit) 
		pr[p].unit_held = 0;  /* if no units, can't be same unit holding! */
	}

	return err;
}

int gameout(void)
{

	/*  Write the next version of the game data file  */

	unsigned char *b, *rc;
	int i, j, n, p, u;
	int nu[AUTONOMOUS + 1], np[AUTONOMOUS + 1];
	char ns[80], n1[80], n2[80], n3[80], n4[80], os[80], o1[80], cs[80], c1[80],
	 hs[80], fs[80], uh[80];
#define MAX_OUTPOWER 10
        /* Outpower will allow powers with spaces in names to be used */
	char out_power[MAX_OUTPOWER];

	sprintf(dipent.seq, "%3.3d", atoi(dipent.seq) + 1);
	sprintf(line, "%s%s/G%s", GAME_DIR, dipent.name, dipent.seq);

	if ((ifp = fopen(line, "w")) == NULL) {
		fprintf(rfp, "Error opening game position output data file %s.\n", line);
		fprintf(log_fp, "Error opening game position output data file %s.\n", line);
		perror(line);
		return E_FATAL;
	}
	fprintf(ifp, "%s\n", dipent.phase);
	fprintf(log_fp, "Writing %s for %s.\n", line, dipent.phase);

	UpdateBlockades();

	/*
	 *  Write out the location of each unit.
	 */

	for (p = 1; p <= AUTONOMOUS; p++) {
		nu[p] = np[p] = 0;
		for (u = 1; u <= nunit; u++) {
			if (unit[u].type == ' ') continue;  /* dont write */
			if (unit[u].owner == p) {
				nu[p]++;
				if (unit[u].type == 'T' ) nu[p]++;  /* AF fleet occupies two units */
				if (unit[u].stype == 'x') {
					fprintf(ifp, "%c%c %c %s", dipent.pl[p],
						unit[u].status,
						unit[u].type,
						pr[unit[u].loc].name);
				} else {
					fprintf(ifp, "%c%c %c%c %s", dipent.pl[p],
						unit[u].status,
						unit[u].stype,
						unit[u].type,
						pr[unit[u].loc].name);
				}

				if (unit[u].coast > XC)
					fprintf(ifp, "/%s", mtype[unit[u].coast]);

				if (unit[u].status == 'r') {
					for (b = unit[u].convoy, rc = unit[u].rcoast; *b; b++, rc++) {
						fprintf(ifp, ", %s", pr[*b].name);
						if (*rc > XC) {
							switch (*rc) {
							case NC:
								fprintf(ifp, "/nc");
								break;
							case SC:
								fprintf(ifp, "/sc");
								break;
							case WC:
								fprintf(ifp, "/wc");
								break;
							case EC:
								fprintf(ifp, "/ec");
								break;
							}
						}
					}
				}
				putc('\n', ifp);
			}
		}
	}
	fprintf(ifp, "-1\n");

	/*
	 *  Write out the ownership of each province.
	 */

	for (n = 0, p = 1; p <= npr; p++) {
		if (!(dipent.xflags & XF_NOMONEY)) {
		    if (pr[p].type != 'r' && pr[p].type != 'g')
		        np[pr[p].owner]++;
		} 
		else if( cityvalue(p)) {
		    /* It's a mach game, see if Mach2 */
		    if (dipent.xflags & XF_MACH2) {
			if (pr[p].owner == pr[p].cown) {
			    /* In mach2, only if city & province owner same */
			    np[pr[p].cown]++;
			}
		    } 
		    else {
			np[pr[p].cown]++;
  		    } 
		} 
		ns[n] = pr[p].name[0];
		n1[n] = pr[p].name[1];
		n2[n] = pr[p].name[2];
		n3[n] = pr[p].name[3];
		n4[n] = pr[p].name[4];
		if (n1[n] == '\0' ) {
			n1[n] = n2[n] = ' '; n3[n] = ' '; n4[n] = ' '; 
		}
		if (n2[n] == '\0' ) {
                        n2[n] = n3[n] = ' '; n4[n] = ' '; 
                }
		if (n3[n] == '\0' ) {
                        n3[n] = n4[n] = ' '; 
                }
		if (n4[n] == '\0' )
			n4[n] = ' ';

		os[n] = dipent.pl[pr[p].owner];
		o1[n] = dipent.pl[npown[p]];
		cs[n] = dipent.pl[pr[p].cown];
		c1[n] = dipent.pl[ncown[p]];
		hs[n] = dipent.pl[pr[p].home];
		if (pr[p].type == 'x' && !pr[p].home)
			hs[n] = 'x';

		fs[n] = ((pr[p].flags & 0x3F00) >> 8) + '0';
		uh[n] = pr[p].unit_held == 1 ? '1' : '0';

		if (n++ == 78 - 2 || p == npr) {
			ns[n] = n1[n] = n2[n] = n3[n] = n4[n] = os[n] = o1[n] =
			    cs[n] = c1[n] = hs[n] = fs[n] = uh[n] = '\0';
			fprintf(ifp, "N:%s\nn:%s\nn:%s\nn:%s\nn:%s\nO:%s\n", ns, n1, n2, n3, n4, os);
			if (dipent.flags & F_MACH) {
				fprintf(ifp, "C:%s\nH:%s\nF:%s\n", cs, hs, fs);
				if (dipent.phase[5] == 'R') {
					fprintf(ifp, "o:%s\nc:%s\n", o1, c1);
				}
			}
			if (dipent.x2flags & X2F_MORE_HOMES ||
			    dipent.x2flags & X2F_HOMETRANSFER) {
				 fprintf(ifp, "H:%s\n",hs);
			}
			if (dipent.x2flags & X2F_HOMETRANSFER) {
				fprintf(ifp, "S:%s\n", uh);
			}

			n = 0;
		}
	}

	if (dipent.flags & F_MACH) {
		for (i = 1; i < WILD_PLAYER; i++) {
			if (dipent.pl[i] != 'x') {
				fprintf(ifp, "D: %c %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
					dipent.pl[i], ducats[i].treasury,
				ducats[i].loan[0], ducats[i].interest[0],
				ducats[i].loan[1], ducats[i].interest[1],
				ducats[i].loan[2], ducats[i].interest[2],
				ducats[i].loan[3], ducats[i].interest[3],
				ducats[i].loan[4], ducats[i].interest[4],
				ducats[i].loan[5], ducats[i].interest[5]);
				if (chits[i][0]) {
					fprintf(ifp, "A: %c ", dipent.pl[i]);
					for (j = 0; chits[i][j]; j++) {
						if (chits[i][j] == WILD_PLAYER)
							continue;
						fputc(dipent.pl[chits[i][j]], ifp);
					}
					fputc('\n', ifp);
				}
				if (dipent.phase[5] == 'R') {
					int n = 0;
					for (j = 1; j < WILD_PLAYER; j++) {
						if (i != j && allies[i][j]) {
							if (!n++)
								fprintf(ifp, "a: %c ", dipent.pl[i]);
							fputc(dipent.pl[j], ifp);
						}
					}
					if (n)
						fputc('\n', ifp);
				}
			}
		}
	}
	/*
	 *  Indicate who needs to get their orders in next phase.
	 */

	for (u = i = 0; u < dipent.n; u++) {
		if (dipent.players[u].power < 0)
			continue;

		dipent.players[u].status &= ~(SF_MOVE | SF_MOVED | SF_PART | SF_WAIT);
		if ((p = dipent.players[u].power) <= NPOWER) {
			dipent.players[u].units = nu[p];
			if (dipent.flags & F_MACH)
			    if (dipent.xflags & XF_NOMONEY)
				dipent.players[u].centers = np[p];
			    else
			        dipent.players[u].centers = ducats[p].treasury;
			else 
			    dipent.players[u].centers = np[p];

			/* Mark player as dead if he is */
			if (!(dipent.flags & F_MACH) || 
			     (dipent.xflags & XF_NOMONEY)) {
				if (dipent.players[u].centers <= 0 &&
				    dipent.players[u].units <= 0 ) 
				  dipent.players[u].status |= SF_DEAD;
			}
			if (need_order[p] && !(dipent.players[u].status & SF_DEAD))
				dipent.players[u].status |= SF_MOVE;
		} else {
			dipent.players[u].units = 0;
			dipent.players[u].centers = 0;
		}

		/* In any case, reset the Late and Remind flags */
		dipent.players[u].status &=  ~(SF_LATE | SF_REMIND);
		if ((0 < p && p <= NPOWER)
		|| (p == MASTER && (dipent.flags & F_MODERATE) && !i++)) {
		        strncpy(out_power, powers[p], MAX_OUTPOWER - 1);
                        out_power[MAX_OUTPOWER-1] = '\0';
		        for (i=0; out_power[i] != '\0' && i < MAX_OUTPOWER; i++) {
		            /* Substitute space with '_' to prevent crash on fscanf */
			     if (out_power[i] == ' ') out_power[i] = '_';
		        }

			fprintf(ifp, "%-9.9s %4x %2d %2d %3d %5d %4d %s %4d %2d\n", out_power,
			dipent.players[u].status, dipent.players[u].units,
				dipent.players[u].centers, dipent.players[u].userid,
				dipent.players[u].siteid, ded[dipent.players[u].userid].r,
				dipent.players[u].address, 
				dipent.players[u].late_count, dipent.players[u].centres_blockaded);
		}
	}
	fprintf(ifp, "-2\n");

	/*
	 *  Dump out some more info for enhanced summary reports.
	 */

	putdipent(ifp, 0);

	fclose(ifp);

	/*
	 *  Remove the summary files which may be out of date now.
	 */

	/* TODO allow for placement into a games subdir */
	/*   done - greg   :-)   */
	sprintf(line, "%s%s/summary", GAME_DIR, dipent.name);
	remove(line);
	sprintf(line, "%s%s/msummary", GAME_DIR, dipent.name);
	remove(line);
	return 0;
}
