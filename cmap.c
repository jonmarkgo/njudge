/*
 * $Log$
 * Revision 1.8  2002/12/11 14:57:12  millis
 * Allow '\r' as blank line character
 *
 * Revision 1.7  2002/12/11 14:46:47  millis
 * Restore version 1.5
 *
 * Revision 1.6  2002/08/27 22:27:44  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.5  2002/05/16 13:11:55  miller
 * Removed warning and debug line
 *
 * Revision 1.4  2002/04/15 12:55:40  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.2.2.2  2001/10/19 23:24:36  dema
 * Added handling for inital money
 *
 * Revision 1.2.2.1  2001/10/15 00:18:30  ustv
 * Added handling of comments, Colonial settings
 *
 * Revision 1.2  2001/07/01 23:19:29  miller
 * New processing for storm stables, venice flag, duality provinces,
 * unit limits
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  po_cmap.c
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
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"
#include "dip.h"
#include "conf.h"
#include "functions.h"
#include "porder.h"
#include "mach.h"

int main(int argc, char *argv[])
{

/*
 *  Compile the map into something that can be sucked in easily.
 */

	unsigned char from = 0, to;
	int i, j, n, m, p, v, nc, np;
	char file[255], *t, dir[100];
	long t1, t2;
	struct stat sbuf;
	PFTAB(pftab);
	short cent[NPROV];
	short prov[NPROV];
	char *typetext;
	int power_index;
	char unit_list[MAX_UNIT_TYPES][200]; /* to be safe only! */
	char power_name[20];
	char *s;
	char temp_line[100];
	int money;

	switch (argc) {
	case 1:
		dir[0] = '\0';
		break;
	case 2:
		sprintf(dir, "%s", argv[1]);
		break;
	default:
		fprintf(stderr, "Usage %s [directory]\n", *argv);
		exit(1);
	}

	maxheap = 10000;
	if (!(heap = (unsigned char *) malloc(maxheap + 10))) {
		fprintf(stderr, "Unable to allocate heap.\n");
		exit(1);
	}
	for (v = 1; v < NVARIANT; v++) {
		nrw=0; ngw = 0;  /* Initialise variables */
		if (v == 1)
			sprintf(line, "%s/map", dir);
		else
			sprintf(line, "%s/map.%s", dir, variants[v]);
		if (stat(line, &sbuf)) {
			perror(line);
			exit(1);
		}
		t1 = sbuf.st_mtime;

		sprintf(file, "%s/../map.%d", dir, v);
		if (stat(file, &sbuf))
			t2 = 0;
		else
			t2 = sbuf.st_mtime;

		if (stat(*argv, &sbuf)) {
			fprintf(stderr, "stat: ");
			perror(*argv);
			exit(1);
		}
		if (t1 < t2 && sbuf.st_mtime < t2)
			continue;

		printf("Compiling new %s for %s variant.\n", file, variants[v]);
		if ((ifp = fopen(line, "r")) == NULL) {
			fprintf(stderr, "Error opening map data file %s.\n", line);
			exit(1);
		}
		hp = 0;
		npr = 0;

		/*  Process province definitions: name, type code code ...   */

		while (fgets(line, sizeof(line), ifp) && *line != '-') {
		    if (isalnum(line[0]) ) { /* Treat all others as comments */

			if (npr == NPROV) {
				fprintf(stderr, "Maximum number of provinces (%d) exceeded.\n", NPROV);
				exit(1);
			}
			pr[++npr].name = (char *) &heap[hp];
			pr[npr].move = NULL;
			pr[npr].unit = 0;
			for (t = line; *t && *t != ','; t++) {
				if (*t == '\n') {
					heap[hp++] = 0;
					fprintf(stderr, "Missing comma for province %s.\n", pr[npr].name);
					exit(1);
				}
				heap[hp++] = *t;
			}
			heap[hp++] = 0;
			heap[hp++] = npr;

			for (t++; isspace(*t); t++);
			if (*t) {
				pr[npr].type = *t++;
			} else {
				fprintf(stderr, "Missing type/abbreviations for %s.\n", pr[npr].name);
				exit(1);
			}

			pr[npr].flags = 0;
			pr[npr].type2 = pr[npr].type; /* default type2 = type */

			/* If another character after type, and not a mach one (0-7) use as type2 value */
			if (*t && !isspace(*t) &&  (*t < '0' || '7' < *t)) {
				switch (*t)
				{
				    case 'w':
				    case 'l':
					pr[npr].type2 = *t;
					break;
				    default:
					fprintf(stderr, "Bad type2 value %c for %s.\n", *t, pr[npr].name);
                                        err++;
				}
				t++;
			}

			if (*t && !isspace(*t)) {

				if (*t < '0' || '7' < *t) {
					fprintf(stderr, "Bad city value %c for %s.\n", *t, pr[npr].name);
					err++;
				}
				pr[npr].flags = (*t++ - '0');	/* Number of city points */

				if (*t == 'f' || *t == 'P')
					pr[npr].flags |= PF_FORTRESS;	/* Province is fortifiable */
				
				if (*t == 'p' || *t == 'P')
					pr[npr].flags |= PF_PORT;	/* Province is a port city */

				if (!isspace(*t)) {
				    t++;
				    if (*t == 'V')
				        pr[npr].flags |= PF_VENICE;     /* Province is like Venice */
				}

				while (*t && !isspace(*t))
					t++;

			}
			for (;;) {
				while (isspace(*t))
					t++;
				if (!*t)
					break;
				while (*t && !isspace(*t))
					heap[hp++] = *t++;

				heap[hp++] = 0;
				heap[hp++] = npr;

				if (hp > maxheap) {
					fprintf(stderr, "Heap exceeded!!  Processing %s.\n", pr[npr].name);
					exit(1);
				}
			}
		    }
		}

		heap[hp++] = 0;
		printf("%d of %d provinces used.\n", npr, NPROV);
		initial_money.enabled = 0; /* Set default, no initial money */

		/*
		 *  Process movement: prov-type: prov prov prov/coast ...
		 *  Where type is mv = Land movement
		 *                xc = Any coast
		 *                nc = North coast
		 *                sc = South coast
		 *                ec = East coast
		 *
		 *  Heap gets two-ples: province and "from coast" * 0x10 | "to coast".
		 */

		n = 0;
		while (fgets(line, sizeof(line), ifp) && *line != '-') {
		    if (isalnum(line[0])) { /* treat all others as comments */
			t = get_prov(line, &m, &i);
			if (!m || *t++ != '-') {
				fprintf(stderr, "Bad movement: %s", line);
				err++;
				continue;
			}
			if (m != n && pr[m].move) {
				fprintf(stderr, "Movement for province %s noncontiguous.\n", pr[m].name);
				err++;
			}
			n = m;

			if (!pr[n].move) {
				heap[hp++] = 0;
				pr[n].move = &heap[hp];
			}
			switch (*t) {

			case 'm':
				from = MV * 0x10;
				break;
			case 'x':
				from = XC * 0x10;
				break;
			case 'n':
				from = NC * 0x10;
				break;
			case 'e':
				from = EC * 0x10;
				break;
			case 'w':
				from = WC * 0x10;
				break;
			case 's':
				from = SC * 0x10;
				break;
			case 'c':
				from = CC * 0x10;
				break;

			default:
				fputs(line, stderr);
				fprintf(stderr, "Invalid movement %c for province %s.\n", *t, pr[n].name);
				err++;
			}

			while (*t && !isspace(*t))
				t++;
			while (isspace(*t))
				t++;
			while (*t) {
				t = get_prov(t, &m, &i);
				if (!m) {
					fprintf(stderr, "Movement from %s to unknown: %s", pr[n].name, t);
					m = 1;
					err++;
					break;
				}
				to = i ? i : from ? XC : MV;

				if (*t == '/') {
					switch (*++t) {

					case 'm':
						to = MX;
						break;	/* Only with support */
					case 'x':
						to = XC;
						break;
					case 'n':
						to = NC;
						break;
					case 'e':
						to = EC;
						break;
					case 'w':
						to = WC;
						break;
					case 's':
						to = SC;
						break;
					case 'c':
						to = CC;
						break;

					default:
						fprintf(stderr, "Movement for %s: %s/%s", pr[n].name, pr[m].name, t);
						to = 0;
						err++;
					}
					while (*t && !isspace(*t))
						t++;
					while (isspace(*t))
						t++;
				}
				heap[hp++] = m;
				heap[hp++] = from | to;

				if (hp > maxheap) {
					fprintf(stderr, "Heap exceeded!!  Processing movement %s -> %s.\n",
						pr[n].name, pr[m].name);
					exit(1);
				}
			}
		    }
		}
		heap[hp++] = 0;

		/*
		 *  Process miscellaneous section.
		 */

		nc = np = nv = 0;
		while (fgets(line, sizeof(line), ifp) && *line != '-') {
			int vi_array[7];

			switch (*line) {

				/*
				 *  Center ordering for summary report.
				 */

			case 'C':
				while (fgets(line, sizeof(line), ifp) && isspace(*line)) {
					for (t = line; isspace(*t); t++);
					while (*t) {
						t = get_prov(t, &j, &i);
						if (!(cent[nc++] = j)) {
							fprintf(stderr, "Unrecognized center in centers list: %s", t);
							t = "";
							err++;
						}
						if (nc > NPROV) {
							fprintf(stderr, "Maximum centers in center list exceeded.\n");
							err++;
						}
						if (*t == ',')
							t++;
					}
				}
				break;

				/*
				 *  Owner (province) ordering for summary report.
				 */

			case 'O':
				while (fgets(line, sizeof(line), ifp) && isspace(*line)) {
					for (t = line; isspace(*t); t++);
					while (*t) {
						t = get_prov(t, &j, &i);
						if (!(prov[np++] = j)) {
							fprintf(stderr, "Unrecognized center in province list: %s", t);
							t = "";
							err++;
						}
						if (np > NPROV) {
							fprintf(stderr, "Maximum centers in province list exceeded.\n");
							err++;
						}
						if (*t == ',')
							t++;
					}
				}
				break;

				/*
				 * HongKong table 
				 */
			case 'H':
			    if (nhk >= MAX_POWERS) {
			        fprintf(stderr,"Too many power entries in HK section.\n");
				err++;
				break;
			    }
			    i = 0; /* Counter for entries */
			    while (fgets(line, sizeof(line), ifp) &&
				   !isspace(*line) && line[0] != '#') {
				/* Each line is of the format:
				 * Powerletter: prov1 prov2 ...
                                 */
				hk[nhk].power_letter = line[0]; /* TDB, validate power */
				t = &line[2];
				while(isspace(*t)) t++; /* discount spaces */
				while (*t) {
				    s = temp_line;
				    while (*t && *t != ' ') {
				        *s = *t;
					s++; t++;
				    }
				    *s = '\0';
			            get_prov(temp_line, &p, &m);
				    if (!p) {
					fprintf(stderr,
					        "Bad province in HongKong List %s.\n", 
						temp_line);
					err++;
					break;
				    }
				    if (i >= MAX_HK_PROVINCES) {
					fprintf(stderr,
						"Too many provinces in HK List.\n");
					err++;
					break;
				    }
				    hk[nhk].pr[i] = p;
				    i++;
				    hk[nhk].np = i;
				}
				nhk++;
			    };

			    break;

				/*
				 * Gateway table
				 */
			case 'G':
			    while (fgets(line, sizeof(line), ifp) &&
				   !isspace(*line) && line[0] != '#') {
				/* This specifies gateways and has the
				 * following layout:
			         * gatway-name:gateway-prov prov1 prov2
				 */
				if (ngw >= MAX_GATEWAYS) {
				    fprintf(stderr,"Maximum gateways reached,\n");
				    err++;
				}
			        t = line;
				s = gw[ngw].name;
				while (*t && (*t != ':')) {
				    *s = *t; 
				    s++; t++;
				}
				*s = '\0';
				get_prov( gw[ngw].name, &p, &m);
				if (!p) {
                                    fprintf(stderr,"Gateway not found.\n");
                                    err++;
                                }
				pr[p].type2 = ngw; /* remember the index of gateway for the province */
				gw[ngw].prov_index = p; /* Remember this fake province index */

				t = get_prov(++t, &p, &m);
				if (!p) {
				    fprintf(stderr,"Gateway province not found.\n");
				    err++;
				}
				gw[ngw].gw_prov = p;
				

				/* OK, stuff in the controlled provinces now */
				i=0;
				while (*t) {
				    s = t;
				    while (*t && *t != ' ')
				        t++;
				    t = get_prov(s, &p, &m);
				    if (!p) {
				        fprintf(stderr, "Province not found in gateway spec.\n");
					err++;
				    }
				    if (i >= MAX_GW_PROVINCES) {
					fprintf(stderr, "Too many gatway provinces for %s specified.", gw[ngw].name);
				    }
				    gw[ngw].pr[i] = p;
				    i++;
				    gw[ngw].np = i;
				}
				ngw++;
			    };
			    break;

				/*
				 * Railway table
				 */
			case 'R':
			    while (fgets(line, sizeof(line), ifp) &&
				   !isspace(*line) && line[0] != '#') {
			    	/* Railway table
				 * Provides a list of connected provinces
				 * Format is the following:
				 * name:permitted_power_letters prov1 prov2 ...
				 */
				if (nrw >= MAX_RAILWAYS) {
				    fprintf(stderr, "Maximum %d railways already specified.<n", MAX_RAILWAYS);
				    err++;
				    break;
				}
				t = line;
				s = rw[nrw].name;
				while (*t && *t != ':') {
				    *s = *t;
				    s++; t++;
				}
				*s = '\0';
				/* fprintf(stderr,"Adding railway %s\n", s); */
				get_prov( rw[nrw].name, &p, &m);
                                if (!p) {
                                    fprintf(stderr,"Railway province not found.\n");
                                    err++;
                                }
                                pr[p].type2 = nrw; /* remember the index of railway for the province */


				t++; /* Skip the ':' */
				i = 0;
				while (*t && *t != ' ' && i < MAX_POWERS) {
				    rw[nrw].power_letter[i] = *t;
				    t++; i++;
				}
				rw[nrw].power_letter[i] = '\0'; 
				rw[nrw].prov_index = p; /* Also remember province index */
				/* Now get the railway provinces */
				t++;
				i = 0;
				while (*t && i < MAX_RW_PROVINCES) {
				    s = t;
				    while (*t && *t != ' ') t++;
				    t = get_prov(s, &p, &m);
				    if (!p) {
					fprintf(stderr,"Unknown province %s in railway.\n", s);
					err++;
					break;
				    }
				    if (pr[p].type == 'w') {
					fprintf(stderr,"Water province %s cannot be part of railway.\n", s);
					err++;
					break;
				    }
				    rw[nrw].pr[i++] = p;
				    rw[nrw].np++;
				}
				nrw++;
			    };
			    break;

				/*
				 *  Plague, Famine and Storm tables.
				 */

			case 'F':
			case 'P':
			case 'S':
				if (*line =='F' ) {
					pftab = ftab;
					typetext = "famine";
				}
				else if (*line =='P') {
					pftab = ptab;
					typetext = "plague";
				} else { 
					pftab = stab;
					typetext = "storm";
				}

				for (i = 2; i <= 12; i++) {
					if (!fgets(line, sizeof(line), ifp)) {
						fprintf(stderr, "End of input in %s table.\n", typetext);
						err++;
						break;
					}
					if (i != atoi(line)) {
						fprintf(stderr, "Column %d mismatch in %s table.\n", i, typetext);
						fprintf(stderr, "Line: %s", line);
						err++;
					}
					if (!(t = strchr(line, ':'))) {
						fprintf(stderr, "Missing colon in %s table.\n", typetext);
						fprintf(stderr, "Line: %s", line);
						err++;
					}
					t++;
					for (j = 2; j <= 12; j++) {
						while (isspace(*t))
							t++;
						if (*t == '-') {
							while (*t == '-')
								t++;
							p = 0;
						} else {
							t = get_prov(t, &p, &m);
							if (!p) {
								fprintf(stderr, 
									"Bad province %d in %s table: %s", 
									j, typetext, t);
								fprintf(stderr, "Line: %s", line);
								err++;
								break;
							}
						}
						pftab[i - 2][j - 2] = p;
					}
				}
				break;

				/*
				 *  Variable income tables.
				 */

			case 'V':
				while (fgets(line, sizeof(line), ifp) && isspace(*line)) {
					t = get_prov(line, &p, &m);
					if (!p) {
						fprintf(stderr, "Bad province in variable income: %s", line);
						err++;
						continue;
					}
					vincome[nv].prov = p;
					money = 0;
					if (sscanf(t, "%d%d%d%d%d%d%d%d", &vi_array[0], &vi_array[1],
						   &vi_array[2], &vi_array[3], &vi_array[4], &vi_array[5],
						   &vi_array[6], &money) < 7) {
						fprintf(stderr, "Bad dice list in income: %s", line);
						err++;
						continue;
					}
					vincome[nv].dice = vi_array[0];
					vincome[nv].vinc[0] = vi_array[1];
					vincome[nv].vinc[1] = vi_array[2];
					vincome[nv].vinc[2] = vi_array[3];
					vincome[nv].vinc[3] = vi_array[4];
					vincome[nv].vinc[4] = vi_array[5];
					vincome[nv].vinc[5] = vi_array[6];
					initial_money.money[p] = money;
					if (money) initial_money.enabled = 1; /* There is inital money */

					if (nv++ > MAXVINC) {
						fprintf(stderr, "Maximum number of variable income cities.\n");
						nv = 0;
						err++;
					}
				}
				break;
                        /*
                         * Special unit limits
                         */
 
                        case 'L':
                                power_index = 0;
                                while (fgets(line, sizeof(line), ifp) && isspace(*line)) {
                                    if (sscanf(line, "%s %s %s %s",
                                                power_name,
                                                unit_list[0], unit_list[1],  unit_list[2]) != MAX_UNIT_TYPES +1) {
                                      fprintf(stderr, "Bad unit line: %s ", line);
                                      err++;
                                    } else {
                                       strncpy(&permitted_units[power_index].power_letter,
                                               power_name, 1);
                                        for (i = 0; i < MAX_UNIT_TYPES; i++)
                                        {
                                             permitted_units[power_index].permissions[i] = 0;
                                            for (j = 0; j < MAX_SPEC_TYPES; j++)
                                            {
                                                switch (unit_list[i][j])
                                                {
                                                    case P_ANYTIME:                                                              
                                                        permitted_units[power_index].permissions[i] += PP_ANYTIME << (j*2);
                                                        break;
 
                                                    case P_BUILD:
                                                           permitted_units[power_index].permissions[i] += PP_BUILD << (j*2);
                                                        break;
 
                                                   case P_BUY:
                                                        permitted_units[power_index].permissions[i] += PP_BUY << (j*2);
                                              break;
 
                                                   case P_NEVER:
                                                        permitted_units[power_index].permissions[i] += PP_NEVER << (j*2);
                                                        break;
 
                                                   default:
                                                        fprintf(stderr, "Unknown option in line: %s", line);
                                                        err++;
                                                        break;
                                              }
                                          }
                                     }
                                       power_index++;
                                   }
                                   /* fprintf(stderr, "Pwer = %s, army = %s, fleet = %s\n",
                                           power_name, army_list, fleet_list); */
                                }
                                break;
                                         
			/*
			 * Comments.
			 */

			case '\n':
			case '\r':
			case '#':
			case ' ':
				break;

			default:
				fprintf(stderr, "Unrecognized code in miscellaneous section: %s", line);
				err++;
			}
		}

		printf("%d of %d heap space used.\n", hp, maxheap);

		fclose(ifp);

		/* Check movement table for consistency */
		po_chkmov();

		/*
		 *  Write the results out to the new map file.
		 */

		if (!err) {
			if (!(ifp = fopen(file, "w"))) {
				perror(file);
				exit(1);
			}
			fwrite(&npr, sizeof(npr), 1, ifp);
			fwrite(&hp, sizeof(hp), 1, ifp);
			fwrite(&nv, sizeof(nv), 1, ifp);
			for (i = 1; i <= npr; i++) {
				cmap[CMAP_NAME] = pr[i].name - (char *) heap;
				cmap[CMAP_MOVE] = pr[i].move - (unsigned char *) heap;
				cmap[CMAP_TYPE] = pr[i].type;
				cmap[CMAP_FLAG] = pr[i].flags;
				cmap[CMAP_TYPE2] = pr[i].type2;
				fwrite(cmap, sizeof(cmap[0]), CMAP_SIZE, ifp);
			}
			fwrite(heap, sizeof(unsigned char), hp, ifp);
			if (nv) {
				fwrite(vincome, nv, sizeof(vincome[0]), ifp);
				fwrite(ftab, sizeof(ftab), 1, ifp);
				fwrite(ptab, sizeof(ptab), 1, ifp);
				fwrite(stab, sizeof(stab), 1, ifp);
				fwrite(permitted_units, sizeof(permitted_units), 1, ifp);
				fwrite(&initial_money, sizeof(initial_money),1,ifp);
			}
			fwrite(&nhk, sizeof(nhk), 1, ifp);
                        if (nhk) {
                                fwrite(hk, sizeof(hk[0]), nhk, ifp);
                        }
			fwrite(&ngw, sizeof(ngw), 1, ifp);
                        if (ngw) {
                                fwrite(gw, sizeof(gw[0]), ngw, ifp);
                        }
                        fwrite(&nrw, sizeof(nrw), 1, ifp);
                        if (nrw) {
                                fwrite(rw, sizeof(rw[0]), nrw, ifp);
                        }
			/* These two are wirrten last as only sumamry.c needs them (po_init.c doesn't) */
			fwrite(&nc, sizeof(nc), 1, ifp);
			if (nc) {
				fwrite(cent, sizeof(cent[0]), nc, ifp);
			}
			fwrite(&np, sizeof(np), 1, ifp);
			if (np) {
				fwrite(prov, sizeof(prov[0]), np, ifp);
			}
			fclose(ifp);
		}
	}

	exit(err);

}

/*
 * TODO holy christmas, this function is defined both here and in
 * po_init.c  this is bad.  
 * since cmap is a separate program, i'm leaving it as is, but
 * we should really have either one definition if it's really the
 * same function, or we should rename this one
 */

void po_chkmov(void)
{

/* Check movement table for consistancy */

	int from, to, i, j;
	unsigned char *s, *t;


	for (from = 1; from <= npr; from++) {
		if (!(s = pr[from].move)) {
			if (pr[from].type != 'g' && pr[from].type != 'r')
			    fprintf(stderr, "No movement table for %s.\n", pr[from].name);
			continue;
		}
		while ((to = *s++)) {
			if (!(t = pr[to].move)) {
				fprintf(stderr, "No movement table for %s.\n", pr[to].name);
				continue;
			}
			i = *s++ & 0x0f;
			while ((j = *t++) && ((j != from) || (i != MX && i != *t >> 4)))
				t++;
			if (!j) {
				fprintf(stderr, "Can't get back to %s from %s (%s).\n",
					pr[from].name,
					pr[to].name, mtype[i]);
				err++;
			}
		}
	}
}

/****************************************************************************/
