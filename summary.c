/*
 * $Log$
 * Revision 1.14  2002/08/27 22:27:58  millis
 * Updated for automake/autoconf functionality
 *
 * Revision 1.13  2002/06/12 21:08:30  millis
 * second value in player record is hex, thus us %x and not %d
 *
 * Revision 1.12  2002/04/23 11:08:35  miller
 * Spelling change "Svmebody" -> "Somebody"
 *
 * Revision 1.11  2002/04/18 04:44:34  greg
 * Added the following commands:
 * - unstart
 * - set secret
 * - set [prflist|prfrand|prfboth]
 *
 * Fixed Set Absence so that "to" is not case sensitive
 *
 * Fixed Quiet games so that new players are announced
 * before the game starts
 *
 * Fixed ascii_to_ded.c so thatit no longer generates an
 * error when compiled
 *
 * Revision 1.10  2002/03/05 23:38:50  miller
 * Replaced lost changes revison 1.8
 *
 * Revision 1.9  2002/03/05 23:05:47  miller
 * Fixed Machiavelli summary problem
 *
 * Revision 1.8  2002/02/03 03:45:43  nzmb
 * Fixed bug in plyrdata.c and readded some concession
 *  handler code to summary.c
 *
 * Revision 1.7  2001/10/20 12:11:16  miller
 * Merged in changes from DEMA and USTV CVS
 *
 * Revision 1.6.2.1  2001/10/15 22:30:45  ustv
 * Added display of duality stuff
 *
 * Revision 1.6  2001/08/18 07:11:36  nzmb
 * Show concessions if applicable
 * 
 * Revision 1.5  2001/07/15 09:20:27  greg
 * added support for game directories in a sub directory
 *
 * Revision 1.4  2001/07/01 23:19:29  miller
 * Add storm table
 *
 * Revision 1.3  2001/03/16 00:06:17  miller
 * Fix 'C' parameter so that next one is not squashed
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Accept -C option to specify config_dir
 * Handle blind summaries differently (if not master)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*  summary.c -- Generate a game summary file
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

#include "config.h"
#include "dip.h"
#include "porder.h"
#include "mach.h"
#include "conf.h"
#include "functions.h"
#include "diplog.h"

static int variant = 0;		/* The currently loaded variant */

#define MAXADDR 100
#define MAXNAME 80
#define MAXTURN 200
#define MAXPHASE 10
#define YPL 10			/* Ten years per line */
#define CPL 36			/* 36 supply centers per line */

static char addrs[MAXUSER][MAXADDR], addr[MAXADDR];
static char names[MAXUSER][MAXNAME];
/* this was called "phase", but that conflicted with a function of
 * the same name, so i renamed it -- nw Sun Jun  8 00:57:19 GMT 1997 */
static char phaze[MAXTURN][MAXPHASE];
static char owner[MAXTURN][NPROV];
static char cownr[MAXTURN][NPROV];
static char homes[MAXTURN][NPROV];
static int units[MAXTURN][MASTER + 1];
static int centers[MAXTURN][MASTER + 1];
static int player[MAXTURN][MASTER + 1];
static short prov[NPROV], city[NPROV];
static int sqrs[YPL];

static FILE *tfp;


static char outc[] =
{' ', '*', '&', '+', '%', '^', '$', '#', '!', '~', '@', '(',
 ')', '=', '-', '_', '|', '\\', '/', '?', '>', '<', '[', ']',
 '{', '}', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
 'w', 'x', 'y', 'z'};
static int outn[sizeof(outc)];

/* todo: move these globals to global.c */
FILE *mfp;			/* Master File pointer */
int gflg = 0;			/* Nonzero indicates gunboat mode */
int mflg = 0;			/* Nonzero indicates master mode */
int bflg = 0;			/* Nonzero indicates blind mode  */
int main(int argc, char **argv)
{

	/*
	 *  Initialize, read the data structures off the input files.
	 */

	int i, j, k, n, m, p;
	int nprov, cs, ce, ncity, lname = 0, turn, years = 0;
	int ix[60], sy, gotent;
	char string[30], *s, *t, *name;
        char exe_name[100];
	int lflg=0;
	int tooMany = 0; /* Set to 1 if too many builds were made */

	variant = 1;
	name = NULL;
	log_fp = stderr;
	gotent = 0;

	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			for (s = argv[i] + 1; *s; s++) {
				switch (*s) {
				case 'g':
					gflg++;
					lname = mflg ? 0 : sizeof("Somebody #00");
					break;

				case 'l':
					lflg++;
					break;

				case 'm':
					mflg++;
					lname = 0;
					break;

				case 'q':
					qflg++;
					break;
	
				 case 'b':
                                        bflg++;
                                        break;

				case 'C':
                                  if (*++s)
                                        CONFIG_DIR = s;
                                  else if (i+1 < argc)
                                        CONFIG_DIR = argv[i+1];
                                  else {
                                        fprintf(stderr, "Directory must follow C option.\n");
                                        goto usage;
                                  }
                                  i++;
                                  s = " ";
                                break;


				case 'c':
					/* ignore 'c' options */
					break;


				case 'v':
					if (isdigit(*(s + 1))) {
						variant = atoi(++s);
						while (isdigit(*s))
							s++;
						s--;
					} else {
						if (++i < argc) {
							variant = atoi(argv[i]);
						} else {
							fprintf(log_fp, "sum: Missing variant number.\n");
							goto usage;
						}
					}
					break;

				default:
					fprintf(log_fp, "sum: Unknown option '%c'.\n", *s);
					goto usage;
				}
			}
		} else {
			if (name)
				goto usage;
			name = argv[i];
		}
	}

       /*
         * read in the configuration, first from config file,
         * finally from the command line
         * TODO: fold in command line processing for configuration
         * variables with regular argument processing.
         */
        conf_init();
        conf_readfile(CONFIG_DIR, CONFIG_FILE);
        conf_cmdline(argc, argv);

        if (lflg) {
	    if (!(log_fp = fopen(LOG_FILE, "a"))) {
                    fprintf(stderr, "sum: Unable to open log file.\n");
                    exit(1);
	    }
        }

        mfp = fopen(MASTER_FILE, "r");
        sprintf(exe_name,"%s-%s", JUDGE_CODE, "dip");

        OPENDIPLOG(exe_name);
        DIPINFO("Started summary");


	/*
	 *  We only want to generate the master summary if the gunboat flag
	 *  is enabled.  Otherwise the master can look at the regular players'
	 *  summary.  
	 */

	if (mflg) bflg = 0; /* Master is never blind! */

	if (mflg && gflg) {
		gflg = 0;
	} else {
		mflg = 0;
	}

	if (!variant) {
		fprintf(log_fp, "sum: Invalid numeric variant specified.\n");
		goto usage;
	}
	if (!name) {
	      usage:
		fprintf(log_fp, "sum: Usage: %s [-c<CONFIG>=<value>] [-C<directory>] [-mbglv#] game\n", argv[0]);
		fprintf(log_fp, "    -c<CONFIG>=<value>: Set <CONFIG> setting to <value>.\n");
                fprintf(log_fp, "    C: Set directory for dip.conf file.\n");
		fprintf(log_fp, "    m: Master, don't hide identities in msummary\n");
	        fprintf(log_fp, "    b: Blind game, limits output\n");
		fprintf(log_fp, "    g: Gunboat, hide identities.\n");
		fprintf(log_fp, "    l: Send errors to log file.\n");
		fprintf(log_fp, "    v: Specify alternate variant.\n");
		exit(1);
	}


	/*
	 *  Spin through the Gxxx files to find variant 
	 */

	for (turn = 1; turn < MAXTURN; turn++) {
		/* TODO: 0 flag for printf(3) is not POSIX, once someone confirms that
		 * '0' is the default pad for integers, the 0 should be gotten rid of
		 */
		sprintf(line, "%s%s/G%03d", GAME_DIR, name, turn);
		if (!(ifp = fopen(line, "r")))
			break;
		fclose(ifp);
	}
	if (turn <= 2) {
		fprintf(log_fp, "sum: Attempt to obtain summary before 1st turn: %s\n", name);
		exit(1);
	}
	/* TODO: 0 flag for printf(3) is not POSIX, once someone confirms that
	 * '0' is the default pad for integers, the 0 should be gotten rid of */
	sprintf(line, "%s%s/G%03d", GAME_DIR, name, turn - 1);
	if ((ifp = fopen(line, "r"))) {
		/*
		 *  Skip to dipent
		 */
		while (fgets(line, sizeof(line), ifp) && !(line[0] == '-' && line[1] == '2'));
		getdipent(ifp);
		fclose(ifp);
	}
	variant = dipent.variant;

	newdipent(name, variant);

	variant = dipent.variant;

/*** TODO: Make summary.c and po-init.c use the same code!!! 
     I am fixing a bug due only to this now! MLM 05/03/2002 ***/

	sprintf(line, "map.%d", variant);
	if ((ifp = fopen(line, "r")) == NULL) {
		fprintf(log_fp, "sum: Error opening map data file %s.\n", line);
		exit(1);
	}
	if (fread(&npr, sizeof(npr), 1, ifp) != 1 ||
	    fread(&hp, sizeof(hp), 1, ifp) != 1 ||
	    fread(&nv, sizeof(nv), 1, ifp) != 1) {
		fprintf(log_fp, "sum: Error reading map file npr/hp/nv, %s.\n", line);
		return E_FATAL;
	}
	maxheap = hp + 2048;
	if (!(heap = (unsigned char *) malloc(maxheap + 10))) {
		fprintf(log_fp, "sum: Unable to allocate heap.\n");
		return E_FATAL;
	}

	for (i = 1; i <= npr; i++) {
		if (fread(cmap, sizeof(cmap[0]), CMAP_SIZE, ifp) != CMAP_SIZE) {
			fprintf(log_fp, "sum: cmap read error city = %d, %s.\n", i, line);
			return E_FATAL;
		}
		pr[i].name = (char *) &heap[cmap[CMAP_NAME]];
		pr[i].move = (unsigned char *) &heap[cmap[CMAP_MOVE]];
		pr[i].type = cmap[CMAP_TYPE];
		pr[i].flags = cmap[CMAP_FLAG];
		pr[i].type2 = cmap[CMAP_TYPE2];
		n = islower(pr[i].type) ? 0 : power(pr[i].type);
		pr[i].owner = n;
		pr[i].cown = n;
		pr[i].home = n;
		pr[i].unit = 0;
		pr[i].gunit = 0;
	}

	if ((i = fread(heap, sizeof(unsigned char), hp, ifp)) != hp) {
		fprintf(log_fp, "sum: cmap heap read error, %d of %d, %s.\n", i, hp, line);
		return E_FATAL;
	}
	if (nv > MAXVINC) {
		fprintf(log_fp, "sum: Maximum variable income exceeded.\n");
		return E_FATAL;
	}
	if (nv > 0) {
		if ((i = fread(vincome, sizeof(vincome[0]), nv, ifp)) != nv) {
			fprintf(log_fp, "sum: cmap income read error, %d of %d, %s.\n", i, nv, line);
			return E_FATAL;
		}
		if ((i = fread(ftab, sizeof(ftab), 1, ifp)) != 1) {
			fprintf(log_fp, "sum: cmap ftab read error, %d. %s\n", i, line);
			return E_FATAL;
		}
		if ((i = fread(ptab, sizeof(ptab), 1, ifp)) != 1) {
			fprintf(log_fp, "sum: cmap ptab read error, %d. %s\n", i, line);
			return E_FATAL;
		}
		if ((i = fread(stab, sizeof(stab), 1, ifp)) != 1) {
                        fprintf(log_fp, "sum: cmap stab read error, %d. %s\n", i, line);
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
                exit(1);
            }
        }
        if (fread(&ngw, sizeof(ngw),1, ifp) != 1) {
            /* Serious error, what to do? */
            ngw = 0;
        }
        if (ngw > 0) {
            if(fread(gw, sizeof(gw[0]), ngw, ifp) != ngw) {
                fprintf(ifp, "Mismatch on number of gw records.\n");
                exit(1);
            }
        }
	if (fread(&nrw, sizeof(nrw),1, ifp) != 1) {
	    /* Serious error, what to do? */
	    nrw = 0;
	}
	if (nrw > 0) {
	    if(fread(rw, sizeof(rw[0]), nrw, ifp) != nrw) {
		fprintf(ifp, "Mismatch on number of rw records.\n");
		exit(1);
	    }
	}

	/*
	 *  Get the province/supply center ordering.
	 */

	if ((i = fread(&nprov, sizeof(nprov), 1, ifp)) != 1 || nprov == 0) {
		fclose(ifp);
		sprintf(line, "%s%s/%ssummary", GAME_DIR, dipent.name, mflg ? "m" : "");
		if (!(ifp = fopen(line, "w"))) {
			perror(line);
			exit(1);
		}
		fprintf(ifp, "Sorry, summary not supported for the %s variant.\n",
			variants[variant]);
		exit(0);
	}

	if (nprov > NPROV) {
		fprintf(log_fp, "sum: Too many prov records: %d of %d.\n", nprov, NPROV);
		exit(1);
	}
	if ((i = fread(prov, sizeof(prov[0]), nprov, ifp)) != nprov) {
		fprintf(log_fp, "sum: prov read error, %d of %d.\n", i, nprov);
		exit(1);
	}
	/*
	 *  Get the city ordering.
	 */

	if ((i = fread(&ncity, sizeof(ncity), 1, ifp)) != 1) {
		ncity = 0;
	}
	if (ncity > NPROV) {
		fprintf(log_fp, "sum: Too many city records: %d of %d.\n", ncity, NPROV);
		exit(1);
	}
	if ((i = fread(city, sizeof(city[0]), ncity, ifp)) != ncity) {
		fprintf(log_fp, "sum: city read error, %d of %d.\n", i, ncity);
		exit(1);
	}
	fclose(ifp);

	/*
	 * Get the real names for the players.
	 */

	if ((ifp = fopen("dip.whois", "r"))) {
		while (fgets(line, sizeof(line), ifp)) {
			for (s = line; isspace(*s); s++);	/* skip any leading whitespace */
			if (!strncmp(s, "User:", 5)) {
				sscanf(s, "User: %d", &i);
			} else if (!strncasecmp(s, "Name:", 5)) {
				for (s += 5; isspace(*s); s++);
				for (t = names[i]; t < names[i] + MAXNAME && *s && *s != '\n';
				     *t++ = *s++);
				*t = '\0';
			}
		}
	}
	/*
	 *  Spin through the Gxxx files and gather up stats.
	 */

	for (turn = 1; turn < MAXTURN; turn++) {
		/* TODO remove non-posix 0 flag */
		sprintf(line, "%s%s/G%03d", GAME_DIR, dipent.name, turn);
		if (!(ifp = fopen(line, "r")))
			break;

		/*
		 *  Get the phase.
		 */

		fgets(phaze[turn], sizeof(phaze[turn]), ifp);
		if ((s = strchr(phaze[turn], '\n')))
			*s = '\0';

		/*
		 *  Skip unit locations.
		 */

		while (fgets(line, sizeof(line), ifp) && *line != '-');

		/*
		 *  Get the supply center ownerships.
		 */

		fgets(line, sizeof(line), ifp);
		if (*line != '-') {
			if (line[1] == ':') {
				j = 1;
				while (fgets(line, sizeof(line), ifp) && line[1] == ':') {
					switch (*line) {

						/*
						 *  C: City ownership
						 */

					case 'C':
						for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
							if (!isalnum(*t))
								*t = '.';
						strcpy(cownr[turn] + j - 1, line + 2);
						break;

						/*
						 *  H: Province home ownership
						 */

					case 'H':
						for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
							if (!isalnum(*t))
								*t = '.';
						strcpy(homes[turn] + j - 1, line + 2);
						break;

						/*
						 *  N: Comment, province name.  Note: OCHF must follow N.
						 */

					case 'N':
						j = i;
						break;

						/*
						 *  O: Province ownership.
						 */

					case 'O':
						for (i = j, t = line + 2; *t && !isspace(*t); i++, t++)
							if (!isalnum(*t))
								*t = '.';
						strcpy(owner[turn] + j - 1, line + 2);
						break;
					}
				}

			} else {	/* Old pre-Machiavelli format */

				strcpy(owner[turn], line);
				fgets(line, sizeof(line), ifp);

			}

			/*
			 *  Read in player list.
			 */

			n = 0;
			while (*line != '-') {
				get_power(line, &p);
				if (!p && *line == 'M')
					p = MASTER;
				if (p) {
					n++;
					j = sscanf(line, "%s %x %d %d %d %d %d %s", string, &i,
						   &units[turn][p], &centers[turn][p], &player[turn][p], &i, &i, addr);

					if (j != 8) {
						fprintf(log_fp, "sum: Bad G%3.3d line, %d: %s", turn - 1, j, line);
					}
					if ((i = player[turn][p]) < MAXUSER) {
						if (*addr != '*')
							strcpy(addrs[i], addr);
						if ((s = strchr(addrs[i], '\n')))
							*s = '\0';
						if (!gflg || p == MASTER) {
							if ((j = strlen(names[i])) > lname)
								lname = j;
						}
					} else {
						player[turn][p] = MAXUSER - 1;
						strcpy(names[MAXUSER - 1], "Unknown");
					}

				}
				if (!fgets(line, sizeof(line), ifp))
					break;
			}

			if (!n) {
				for (i = 0; i < npr; i++) {
					if ((p = power(owner[turn][i]))) {
						centers[turn][p]++;
					}
				}
			}
			/*
			 * Get any dipent entry we run across. (only last is retained)
			 */

			if (line[0] == '-' && line[1] == '2') {
				gotent = 1;
				getdipent(ifp);
			}
		} else {

			/*
			 *  Use default supply center ownerships--'x' = unowned center, 'l' =
			 *  land non-center, & 'w' = water non-center, so don't count those.
			 */

			for (i = 0; i < npr; i++) {
				if ((pr[i + 1].type != 'x') && (pr[i + 1].type != 'l') &&
				    (pr[i + 1].type != 'w')) {
					owner[turn][i] = cownr[turn][i] = homes[turn][i] = pr[i + 1].type;
				} else {
					owner[turn][i] = cownr[turn][i] = homes[turn][i] = '.';
				}
				centers[turn][power(owner[turn][i])]++;
				units[turn][power(owner[turn][i])]++;
			}
			owner[turn][i] = cownr[turn][i] = homes[turn][i] = '\n';

		}

		fclose(ifp);

	}

	if (phaze[1][0] == 'S' && phaze[1][5] == 'M') {
		sprintf(phaze[0], "x%4.4dB", atoi(phaze[1] + 1) - 1);
	} else {
		sprintf(phaze[0], "x%4.4dx", atoi(phaze[1] + 1));
	}
	memcpy(owner[0], owner[1], sizeof(owner[0]));
	memcpy(cownr[0], cownr[1], sizeof(cownr[0]));
	memcpy(homes[0], homes[1], sizeof(homes[0]));
	memcpy(centers[0], centers[1], sizeof(centers[0]));
	memcpy(units[0], units[1], sizeof(units[0]));

	if (dipent.flags & F_MACH) {
		for (p = 1; p < WILD_PLAYER; p++) {
			if (dipent.pl[p] == 'x')
				continue;
			centers[1][p] = centers[2][p] + 3 * units[2][p];
		}
	}
	/*
	 *  Oh boy, now we write out the report.  First the player list.
	 */

	sprintf(line, "%s%s/%ssummary", GAME_DIR, dipent.name, mflg ? "m" : "");
	if (!(ifp = fopen(line, "w"))) {
		perror(line);
		exit(1);
	}
	fprintf(ifp, "Summary of game '%s' through %s.\n\n", dipent.name, phaze[turn - 2]);

	k = 1;
	for (p = MASTER; p < WILD_PLAYER || p == MASTER; p++) {
		if (dipent.pl[p] == 'x')
			continue;
		i = 0;
		for (j = 1; j < turn; j++) {
			if (qflg && gflg && !mflg) {	/* Don't show power transfer if quiet */
				if (i)
					player[j][p] = i;
			}
			if ((n = player[j][p]) && i != n) {
				if (!i) {
					while (i < j)
						player[i++][p] = n;
					fprintf(ifp, "  %s:%*s", powers[p], (int) (14 - strlen(powers[p])), "");
				} else {
					fprintf(ifp, "   from %6.6s:  ", phaze[j - 1]);
				}
				i = n;
				if (gflg && p != MASTER) {
					if (strncmp(names[i], "Somebody #", 10)) {
						strcpy(addrs[i], "someone@somewhere");
						if (dipent.x2flags & X2F_SECRET) {
							sprintf(names[i], "Somebody   ");
						} else {
							sprintf(names[i], "Somebody #%d", k++);
						}
					}
				}
				addrs[i][120 - 14 - 1 - 2 - lname - 3] = '\0';
				fprintf(ifp, "%-*s  %s\n", lname, names[i], addrs[i]);
			}
		}

		if (p == MASTER)
			p = 0;
	}

	/*
	   **  We'll output the parameters if we managed to find a dipent entry.
	   (this routine is not used at the moment, it gets the dipent from
	   the dip.master file.  The current assumption is that the most
	   recent dipent in the data files is better)
	   strcpy(name, dipent.name);
	   while (getdipent(mfp)) {
	   if (strcmp(dipent.name,name) == 0)
	   break;
	   };
	 */

	if (gotent) {
		fprintf(ifp, "\n\nGame parameters are/were as follows:\n");
		params(ifp);
	}
	fprintf(ifp, "  Judge: %s.\n", JUDGE_CODE);

	/*
	 *  Game start date
	 */

	sprintf(line, "%s%s/start", GAME_DIR, dipent.name);
	if ((tfp = fopen(line, "r"))) {
		fputs("\n", ifp);
		while (fgets(line, sizeof(line), tfp))
			fputs(line, ifp);
		fclose(tfp);
	}
	/*
	 *  Draw if there is one, is next
	 */

	sprintf(line, "%s%s/draw", GAME_DIR, dipent.name);
	if ((tfp = fopen(line, "r"))) {
		while (fgets(line, sizeof(line), tfp))
			fputs(line, ifp);
		fclose(tfp);
	}
    /*
 
     * or concession ...
 
     */
 
     sprintf(line, "%s/%s/conc", GAME_DIR, dipent.name);
 
     if((tfp = fopen(line, "r"))) {
 
         while(fgets(line, sizeof(line), tfp))
 
             fputs(line, ifp);
 
         fclose(tfp);
 
     }
	/*
	 *  Comments if any are next.
	 */

	sprintf(line, "%s%s/info", GAME_DIR, dipent.name);
	if ((tfp = fopen(line, "r"))) {
		fputs("\n", ifp);
		while (fgets(line, sizeof(line), tfp))
			fputs(line, ifp);
		fclose(tfp);
	}
	/*
	 *  Next we take care of the supply center ownership.
	 */

	/* Only display this section if not blind or for master */

	if (!bflg) for (cs = 0; cs < nprov; cs += CPL) {
		ce = nprov < cs + CPL ? nprov : cs + CPL;

		if (dipent.flags & F_MACH) {
			fprintf(ifp, "\n\nHistorical Province Summary%s\n", cs ? " (cont)" : "");
			fprintf(ifp, "---------------------------%s\n", cs ? "-------" : "");
		} else {
			fprintf(ifp, "\n\nHistorical Supply Center Summary%s\n", cs ? " (cont)" : "");
			fprintf(ifp, "--------------------------------%s\n", cs ? "-------" : "");
		}
		fprintf(ifp, "   ");
		for (i = cs; i < ce; i += 2) {
			s = pr[prov[i]].name;
			while (*s)
				s++;
			s++;
			s++;
			if (!*s)
				s = pr[prov[i]].name;
			if (islower(*s))
				*s = toupper(*s);
			fprintf(ifp, " %-3.3s", s);
		}

		fprintf(ifp, "\nYear ");
		for (i = cs + 1; i < ce; i += 2) {
			s = pr[prov[i]].name;
			while (*s)
				s++;
			s++;
			s++;
			if (!*s)
				s = pr[prov[i]].name;
			if (islower(*s))
				*s = toupper(*s);
			fprintf(ifp, " %-3.3s", s);
		}
		fputc('\n', ifp);

		years = 0;
		for (i = 0; i < turn; i++) {
			if (phaze[i][5] == 'B' || phaze[i][5] == 'A' ||
			    ((i == (turn - 1)) && (phaze[i][6] == 'X')) ||
			    ((i != 0) && (i < turn - 1) && (phaze[i + 1][4] != phaze[i][4]))) {
				fprintf(ifp, "%4.4d", atoi(phaze[i] + 1));
				for (p = cs; p < ce; p++)
					fprintf(ifp, " %c", owner[i][prov[p] - 1]);
				fputc('\n', ifp);
				ix[years++] = i;
			}
		}
	}

	/*
	 *  Similarly the city ownerships.
	 */

	if (!bflg) for (cs = 0; cs < ncity; cs += CPL) {
		ce = ncity < cs + CPL ? ncity : cs + CPL;

		fprintf(ifp, "\n\nHistorical City Summary%s\n", cs ? " (cont)" : "");
		fprintf(ifp, "-----------------------%s\n", cs ? "-------" : "");
		fprintf(ifp, "   ");
		for (i = cs; i < ce; i += 2) {
			s = pr[city[i]].name;
			while (*s)
				s++;
			s++;
			s++;
			if (!*s)
				s = pr[city[i]].name;
			if (islower(*s))
				*s = toupper(*s);
			fprintf(ifp, " %-3.3s", s);
		}

		fprintf(ifp, "\nYear ");
		for (i = cs + 1; i < ce; i += 2) {
			s = pr[city[i]].name;
			while (*s)
				s++;
			s++;
			s++;
			if (!*s)
				s = pr[city[i]].name;
			if (islower(*s))
				*s = toupper(*s);
			fprintf(ifp, " %-3.3s", s);
		}
		fputc('\n', ifp);

		for (i = 0; i < years; i++) {
			fprintf(ifp, "%4.4d", atoi(phaze[ix[i]] + 1));
			for (p = cs; p < ce; p++)
				fprintf(ifp, " %c", cownr[ix[i]][city[p] - 1]);
			fputc('\n', ifp);
		}
	}

	/*
	 *  Next we take care of supply center counts..
	 */

	if (dipent.flags & F_MACH) {
		fprintf(ifp, "\n\nHistory of Treasury for Adjustments\n");
		fprintf(ifp, "-----------------------------------\n");
	} else {
		fprintf(ifp, "\n\nHistory of Supply Center Counts\n");
		fprintf(ifp, "-------------------------------\n");
	}

	for (sy = 0; sy < years; sy += YPL) {

		fprintf(ifp, "Power    %4.4s", phaze[ix[sy]] + 1);
		for (i = 1; i < YPL && i + sy < years; i++)
			fprintf(ifp, " '%2.2s", phaze[ix[i + sy]] + 3);
		fprintf(ifp, "%*sPlayer\n", 4 * (YPL - i) + 3, "");

		for (p = 1; p < WILD_PLAYER; p++) {
			if (dipent.pl[p] == 'x' || !centers[ix[sy]][p])
				continue;
			fprintf(ifp, "%-10.10s", powers[p]);
			j = 0;
			for (i = 0; i < YPL && i + sy < years; i++) {
				if (!qflg && player[ix[i + sy]][p] != player[ix[j + sy]][p]) {
					if (player[ix[j + sy]][p]) {
						fprintf(ifp, "\\%*s%s\n%*s", 4 * (YPL - i) - 1 + 2, "",
							names[player[ix[j + sy]][p]], 10 + 4 * i, "");
					}
					j = i;
				}
				if (!centers[ix[i + sy]][p])
					break;
				m = centers[ix[i + sy]][p];
				n = (ix[i + sy] == turn - 1) || (dipent.flags & F_MACH) ?
				    m : units[ix[i + sy] + 1][p];
				if (!bflg) {
					
					if (n > m) {
					    fprintf(ifp, "%3dX", m); 
					    tooMany = 1;
					}
					else 
					    fprintf(ifp, "%3d%c", m, outc[m - n]);
				} else {
					fprintf(ifp, "%3d ", m);
				}
				if (n <= m) outn[m - n]++;
				sqrs[i] += m * m;
			}
			fprintf(ifp, "%*s%s\n", 4 * (YPL - i) + 2, "", names[player[ix[j + sy]][p]]);
		}

		if (!(dipent.flags & F_MACH)) {
			fprintf(ifp, "Index:   ");
			for (i = 0; i < YPL && i + sy < years; i++) {
				fprintf(ifp, "%4d", sqrs[i] / dipent.np);
				sqrs[i] = 0;
			}
		}
		fprintf(ifp, "\n\n");

	}

	if (!bflg) {
		if (tooMany) 
			fprintf(ifp, "X = too many build(s).\n");
		
		for (i = 1; i < sizeof(outc); i++) {
			if (outn[i]) {
				fprintf(ifp, "%c = %d unused build%s.\n", outc[i], i, i == 1 ? "" : "s");
			}
		}
	}
	/*
	 *  Next we take care of the rest of the info...
	 */

	if (!(dipent.flags & F_MACH)) {
		fprintf(ifp, "\nIndex is the sum of squares of the number of supply centers divided by the\n");
		fprintf(ifp, "number of players.  It is a measure of how far the game has progressed.\n");
	}
	fclose(ifp);
	return 0;
}
