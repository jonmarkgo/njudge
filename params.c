/*
 * $Log$
 * Revision 1.34  2003/08/18 23:16:43  millis
 * Refix bug 211
 *
 * Revision 1.33  2003/08/14 12:46:26  millis
 * Fix bug 211
 *
 * Revision 1.32  2003/08/10 15:27:52  millis
 * Fix bug 25 (Add TouchPress)
 *
 * Revision 1.31  2003/07/14 22:52:27  millis
 * Fix bug 187
 *
 * Revision 1.30  2003/05/24 22:55:44  millis
 * Bug 142, allow use of AutoCreate flag
 *
 * Revision 1.29  2003/05/09 09:44:51  millis
 * Added display of Mach2 flag
 *
 * Revision 1.28  2003/05/06 01:53:07  millis
 * Bug in displaying Summer turn
 *
 * Revision 1.27  2003/05/02 22:20:59  millis
 * Further flags (NoSummer, CaptureHome)
 *
 * Revision 1.26  2003/04/16 04:32:13  millis
 * Fix bug 65
 *
 * Revision 1.25  2003/02/28 20:16:46  nzmb
 * Changed the name of resignation ratio to CD ratio, to avoid confusion with
 * Doug Massey's DRR.
 *
 * Revision 1.24  2003/02/18 14:26:01  millis
 * Fixed small errors (missing '&')
 *
 * Revision 1.23  2003/02/18 14:03:25  millis
 * Added display of some new parameters
 *
 * Revision 1.22  2003/01/15 13:47:42  millis
 * Fixed compile error
 *
 * Revision 1.21  2003/01/14 13:56:46  millis
 * Updated with ustv merged changed
 *
 * Revision 1.20  2002/12/28 01:00:56  millis
 * Small text change
 *
 * Revision 1.19  2002/12/28 00:52:18  millis
 * Proper fix to CR 17
 *
 * Revision 1.18  2002/12/28 00:42:54  millis
 * Implement bug 17, noBcentres option (to hide others centres in blind)
 *
 * Revision 1.17  2002/11/14 00:07:49  millis
 * Trivial change to remove a space character
 *
 * Revision 1.16  2002/11/13 22:29:16  millis
 * Bug 42 fix, Storms setting not being shown
 *
 * Revision 1.15  2002/07/17 11:46:33  millis
 * Only show postal setting if set
 *
 * Revision 1.14  2002/07/16 18:14:23  nzmb
 * Many changes dealing with the addition of szine style postal press. Also fixed apparent bug in signons for games which have not started.
 *
 * Revision 1.13  2002/06/11 16:26:20  nzmb
 *
 * Added set [no]mustorder to require players to submit avalid set of orders
 * before they may send press (to be used in conjunction with set wait).
 *
 * Revision 1.12  2002/04/18 04:44:34  greg
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
 * Revision 1.11  2002/04/15 12:55:45  miller
 * Multiple changes for blind & Colonial & setup from USTV
 *
 * Revision 1.10  2002/02/25 11:51:53  miller
 * Various updates for Machiavelli bug fixes
 *
 * Revision 1.9  2001/10/20 12:11:14  miller
 * Merged in changes from DEMA and USTV 
 *
 * Revision 1.8.2.2  2001/10/20 00:52:20  dedo
 * Correctedcompile error
 *
 * Revision 1.8.2.1  2001/10/15 22:27:51  ustv
 * Merge in concession and colonial flag code
 *
 * Revision 1.8  2001/08/18 07:08:54  nzmb
 * Show concessions flag. Also new dedication params show up when either
 * has been modified (not both as before).
 *
 * Revision 1.7  2001/07/08 23:00:51  miller
 * New flags and tidy-up
 *
 * Revision 1.6  2001/07/01 23:19:29  miller
 * Coastal convoy
 *
 * Revision 1.5  2001/06/24 06:05:35  nzmb
 * Made it so new dedication settings appear in game parameters.
 *
 * Revision 1.4  2001/05/14 23:00:56  miller
 * Added NoAttackTrans flag
 *
 * Revision 1.3  2000/11/18 21:12:14  miller
 * Fixed ONECENTRE bug
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Display of all new additional parameters
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.3  1997/02/16 20:43:18  davidn
 * Additions to dipent structure and associated code, to allow duplex variants.
 * Command is "set players n".
 *
 * Revision 1.2  1996/11/13 23:16:56  rpaar
 * Changed the loop range in printlevel to 1 <= x <= nlevel instead of 0 <= x <= nlevel.
 * Changed return strcap("undefined") to return ("Undefined")
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 *
 */

/*
 *  params.c -- Print Game Parameters
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
 *      ?? Apr 1994 T.Ruelle     Add printlevel() procedure.
 *      23 May 1994 C.Marcus,Jr. Add print_params() procedure; clarify text for
 *                               press flags.
 *      29 Dec 1996 David Norman no_of_players added to dipent
 *      25 Nov 1999 M. Miller    Added display of LATECOUNT flag if set
 */

#include <stdlib.h>
#include <string.h>
#include "dip.h"
#include "functions.h"
#include "mail.h"

/*  Access types.  */

char *accesses[] =
{0, "any", "different", "same"};

int avalue[] =
{0, A_ANY, A_DIFF, A_SAME};

int naccess = nentry(accesses);

/*  Levels.  */

char *levels[] =
{0, "any", "novice", "advanced", "intermediate", "amateur", "expert"};

int lvalue[] =
{0, L_ANY, L_NOVICE, L_ADVANCED, L_INTERMEDIATE, L_AMATEUR, L_EXPERT};

int nlevel = nentry(levels);

static void print_params(FILE * outf, char *line);


void strcatf( char *out, char*in, int *first)
{
	if (*first == 1) {
		*first = 0;
	} else {
		strcat(out, ", ");
	}
	strcat(out, in);
}
	/****************************************************************************
NAME:  params

DESCRIPTION
    This procedure prints out the game parameters, either for a 'list' command
    or after a change has been made to one or more parameters.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    ?? ??? 1987 K.Lowe       Original development.
    ?? Apr 1994 T.Ruelle     Add amateur level; use printlevel() to print level.
    23 May 1994 C.Marcus,Jr. Use print_params() to print lines (to allow for
                             long lines, if many flags set); clarify text for
                             press flags.
    22 Dec 1994 L.Richardson Added the Index parameter display.

INTERFACE DEFINITION
    Calling Sequence:
        params(fp);

    Inputs:
        fp := File pointer to which parameters are to be written

    Outputs:
        Parameters written to specified file

ALGORITHM
    Write move phase processing information to file.
    Write retreat phase processing information to file.
    Write adjustment phase processing information to file.
    Write general information to file.
    Write variant information to file.
    Write flag information to file.
    Write press information to file.
    Write cataloging information and victory condition to file.
****************************************************************************/

void params(FILE * fp)
{
	char line[256];		/* Line buffer */
	char temp1[50];		/* Temp number buffer */
	char *temp;		/* Pointer to buffer to be freed */
	int first_flag;		/* say if first entry in list */
	/* Write information for move, retreat, and adjustment phases (deadlines,
	   processing days, etc.).  */

	putseq(fp, "  Move", &dipent.movement);
	putseq(fp, "  Retreat", &dipent.retreat);
	putseq(fp, "  Adjust", &dipent.builds);

	/* General information (access, level, moderated, rated, dedication). */

	sprintf(line, "   Access:  %s-site",
		(temp = strcap(accesses[dipent.access])));
	free(temp);
	sprintf(&line[strlen(line)], ", Level: %s",
		(temp = printlevel(dipent.level)));
	free(temp);
	if (dipent.flags & F_MODERATE) {
		strcat(line, ", Moderated");
	}
	if (dipent.flags & F_NORATE) {
		strcat(line, ", Unrated");
	}
	if (dipent.dedicate) {
		sprintf(&line[strlen(line)], ", Dedication: %d", dipent.dedicate);
	}
	strcat(line, ".");
        print_params(fp, line);

	if (dipent.orded != 0.000 || dipent.rrded != 1.000) {
		sprintf(line, "   DSettng:" );
		sprintf(&line[strlen(line)], 
			" Minimum ontime ratio: %.3f. Maximum CD ratio: %.3f",
			dipent.orded,dipent.rrded);
		strcat(line, ".");
		print_params(fp, line);
	}

	/* Variant & option information. */

	sprintf(line, "  Variant:  %s", (temp = strcap(variants[dipent.variant])));
	free(temp);

	if (dipent.flags & F_GUNBOAT) {
		strcat(line, ", Gunboat");
	}
	if (dipent.flags & F_NOSHOW) {
		strcat(line, ", No Show");
	}
	if (dipent.flags & F_BLIND) {
		strcat(line, ", Blind");
	}
	if (!(dipent.flags & F_MACH)) {
	    if (dipent.flags & F_AFRULES) {
		strcat(line, ", AF_Rules");
	    }
	    if (dipent.flags & F_SHORTHAND ) {
                strcat(line, ", Shorthand");
	    }
	    if (dipent.flags & F_WINGS) {
		strcat(line, ", Wings");
 	    }
	}	
	strcat(line, ".");
	print_params(fp, line);

	/*  Flag information.  */

	sprintf(line, "    Flags:  ");
	if (dipent.flags & F_NONMR) {
		strcat(line, "NoNMR");
	} else {
		strcat(line, "NMR");
	}
	if (dipent.flags & F_NOLIST) {
		strcat(line, ", NoList");
	}
	if (dipent.flags & F_QUIET) {
		strcat(line, ", Quiet");
	}
	if (dipent.flags & F_NOREVEAL) {
		strcat(line, ", NoReveal");
	}
	if (dipent.flags & F_PROXY) {
		strcat(line, ", Proxy");
	} else {
		strcat(line, ", NoProxy");
	}
	if (dipent.flags & F_NODIAS) {
		strcat(line, ", NoDIAS");
	} else {
		strcat(line, ", DIAS");
	}
	if (!(dipent.flags & F_GRACEDAYS)) {
		strcat(line, ", StrictGrace");
	}
	if (dipent.flags & F_STRWAIT) {
		strcat(line, ", StrictWait");
	}

        strcat(line, ".");
        print_params(fp, line);

	if (dipent.x2flags & X2F_COLONIAL) {
	    sprintf(line, " Colonial:  ");
	    first_flag = 1;
	    if (dipent.x2flags & X2F_HONGKONG)
		strcatf(line, "HongKong", &first_flag);
	    if (dipent.x2flags & X2F_GATEWAYS)
		strcatf(line, "Gateways", &first_flag);
	    if (dipent.x2flags & X2F_RAILWAYS)
		strcatf(line, "Railways", &first_flag);
	    strcat(line,".");
	    print_params(fp, line);
	}
	if (dipent.flags & F_MACH) {
	    if (dipent.xflags & XF_MACH2) {
	        sprintf(line, "   Mach2:   ");
	    } else {
	        sprintf(line, "    Mach:   ");
	    }

	    if (dipent.flags & F_NODICE) {
			strcat(line, "NoDice");
	    } else {
			if (dipent.flags & F_NOFAMINE) {
				strcat(line, "NoFamine");
			} else {
				strcat(line, "Famine");
			}
			if (dipent.flags & F_NOPLAGUE) {
				strcat(line, ", NoPlague");
			} else {
				strcat(line, ", Plague");
			}
			if (dipent.flags & F_NOLOANS) {
				strcat(line, ", NoLoans");
			} else {
				strcat(line, ", Loans");
			}
			if (dipent.flags & F_NOASSASS) {
				strcat(line, ", NoAssassination");
			} else {
				strcat(line, ", Assassination");
			}
	    }
		    if (dipent.flags & F_NOSPECIAL) {
		   	strcat(line, ", NoSpecial");
		    } else {
				strcat(line, ", Special");
		    }
	            if (dipent.xflags & XF_FORT) {
                        strcat(line, ", Forts");
                    } else {
                        strcat(line, ", NoForts");
                    }
		    if (dipent.xflags & XF_STORMS) {
			strcat(line,", Storms");
		    }
		    if (dipent.x2flags & X2F_NOGARRISONS) 
			strcat(line, ", NoGarrisons");

		    if (dipent.x2flags & X2F_NOSUMMER) 
			strcat(line, ", NoSummer");	
		
		if (dipent.flags & F_NOADJ) {
			strcat(line, ", NoAdjacency");
		} else {
			strcat(line, ", Adjacency");
		}
                if (dipent.xflags & XF_NOMONEY)
                    strcat(line, ", NoMoney");
            strcat(line, ".");
            print_params(fp, line);

	}
	
        /* Transform information.  */
        if (!(dipent.flags & F_MACH) && (dipent.xflags & XF_TRANS_ANYT)) {
            sprintf(line, "   Trafo:  ");

	    if (dipent.xflags & XF_TRANS_BUILD) {
		strcat(line," Build:");
                switch ((dipent.xflags & XF_TRANS_BANYW)) {
                    case XF_TRANS_BANYC:
                        strcat(line,"AnyCentre");
                        break;
                   case XF_TRANS_BONEC:
                        strcat(line,"OneCentre");
                        break;
                   case XF_TRANS_BANYW:
                        strcat(line,"Anywhere");
                        break;
                default:
                        strcat(line,"HomeCentre");
                }
	    }

	    if (dipent.xflags & (XF_TRANS_MOVE)) {
		strcat(line," Move:");
		switch ((dipent.xflags & XF_TRANS_MANYW)) {
		    case XF_TRANS_MANYC:
			strcat(line,"AnyCentre");
			break;
		   case XF_TRANS_MONEC:
			strcat(line,"OneCentre");
			break;
		   case XF_TRANS_MANYW:
			strcat(line,"Anywhere");
			break;
		default:
			strcat(line,"HomeCentre");
		}
                if (dipent.xflags & XF_NOATTACK_TRANS )
                {
                    strcat(line, " (if not attacked)");
                }
                else
                    strcat(line, " (if not dislodged)");

	    }
            strcat(line, ".");
            print_params(fp, line); 
       }
 
       /*  xFlag information.  */
         sprintf(line, "   xFlags:  ");
	first_flag = 1;
	if (!(dipent.flags & F_MACH)) {
	    if ((dipent.xflags & XF_BUILD_ONECENTRE) == XF_BUILD_ONECENTRE) {
                    strcatf(line, "OneCentre",&first_flag);
            } else {
    		if (dipent.xflags & XF_BUILD_ANYCENTRES) {
	 		strcatf(line, "AnyCentres",&first_flag);
        	} else {
			strcatf(line, "HomeCentres",&first_flag);
        	}
	    }
	}
        if (dipent.xflags & XF_LATECOUNT) {
                strcatf(line, "LateCount",&first_flag);
        }
	if (dipent.xflags & XF_MANUALPROC) {
                strcatf(line, "ManualProcess",&first_flag);
        }
	if (dipent.xflags & XF_NORESUME) {
		strcatf(line, "NoResume", &first_flag);
	}
	if (dipent.xflags & XF_AUTODISBAND) {
                strcatf(line, "AutoDisband", &first_flag);
        }

	if (dipent.xflags & XF_MANUALSTART) {
		strcatf(line, "ManualStart",&first_flag);
	}
	
	if (!(dipent.flags & F_MACH)) {
	    if (dipent.xflags & XF_ANYDISBAND) {
                strcatf(line, "AnyDisband",&first_flag);
            }
	
	    if (dipent.xflags & XF_STRCONVOY) {
                    strcatf(line, "StrictConvoy",&first_flag);
            }
            if (dipent.xflags & XF_AHCONVOY) {
                strcatf(line, "AHConvoys",&first_flag);
            }
	}
	if ((dipent.xflags & XF_AIRLIFTS) && (dipent.flags & F_WINGS))
	{
		 strcatf(line, "Airlifts",&first_flag);
        }
	if (dipent.xflags & XF_BLANKBOARD) {
                strcatf(line, "BlankBoard",&first_flag);
        }
	if (dipent.xflags & XF_WATCHPRESS) {
                strcatf(line, "WatchPress",&first_flag);
        }
	if (dipent.xflags & XF_NOMINORPRESS) {
                strcatf(line, "NoMinorPress",&first_flag);
        }
	if (dipent.xflags & XF_BLANKPRESS) {
                strcatf(line, "BlankPress",&first_flag);
        }
        if (dipent.xflags & XF_COASTAL_CONVOYS) {
                strcatf(line, "CostalConvoy", &first_flag);
        }
	if (dipent.xflags & XF_MOVEDISBAND)
		strcatf(line, "Disband", &first_flag);
	if ((dipent.xflags & XF_NOCONCESSIONS) && !(dipent.flags & F_NODIAS)){
		strcatf(line, "No concessions", &first_flag);
	} else {
		strcatf(line, "Concessions", &first_flag);
	}

	if (dipent.xflags & XF_PROV_DUALITY)
		strcatf(line, "Duality", &first_flag);

	if (first_flag != 1) {
		/* Only show xflags if there are some to show! */
		strcat(line, ", ");
        	/* print_params(fp, line); */
	}
	/* Display the current max_absence setting */
	sprintf(temp1,"Max Absence: %d.", dipent.max_absence_delay);
	strcat(line,temp1);
	print_params(fp,line);


       /*  x2Flag information.  */
        sprintf(line, "  x2Flags:  ");
	first_flag = 1;
	if (dipent.x2flags & X2F_SECRET)
                strcatf(line, "Secret", &first_flag);


	if (dipent.x2flags & X2F_MORE_HOMES) {
		sprintf(temp1,"NumHome: %d", dipent.num_homes);
		strcatf(line, temp1, &first_flag);
	}
	
	if (dipent.extra_centres != 0) {
                sprintf(temp1,"ExtraCentres: %d", dipent.extra_centres);
                strcatf(line, temp1, &first_flag);
        }

	if (dipent.x2flags & X2F_NEUTRALS) {
		strcatf(line, "Neutrals", &first_flag);
	}

	if (dipent.x2flags & X2F_RIVERS) {
		strcatf(line, "Rivers", &first_flag);
	}

	if (dipent.x2flags & X2F_HOMETRANSFER) {
		strcatf(line, "HomeTransfer", &first_flag);
	}

	if (dipent.x2flags & X2F_CAVALRY) {
		strcatf(line, "Cavalry", &first_flag);
	}

	if (dipent.x2flags & X2F_ARTILLERY) {
		strcatf(line, "Artillery", &first_flag);
	}
	
	if (dipent.x2flags & X2F_AUTOCREATE) {
                strcatf(line, "AutoCreate", &first_flag);
        }

	if (dipent.x2flags & X2F_EXTRA_UNITS ) {
		strcatf(line, "ExtraUnits", &first_flag);
	}

	if (dipent.flags & F_BLIND) {
	    if (dipent.x2flags & X2F_BLIND_CENTRES) {
                strcatf(line, "BCentres", &first_flag);
            } else {
                strcatf(line, "NoBCentres", &first_flag);
            } 
	}
	if ( !(dipent.flags & F_MACH) && (dipent.x2flags & X2F_SUMMER)) {
		strcatf(line, "Summer", &first_flag);
	}
	
	if (!first_flag) 
		strcat(line,".");

	if ((dipent.x2flags & (X2F_PRINT_OPTIONS)))
	    print_params(fp, line);
	

	/*  Press information.  */

	sprintf(line, "    Press:  ");
	switch (dipent.flags & (F_NOWHITE | F_GREY | F_DEFWHITE)) {
	case (F_NOWHITE | F_GREY | F_DEFWHITE):
		strcat(line, "Undefined");
		break;
	case (F_NOWHITE | F_GREY):
		strcat(line, "Grey");
		break;
	case (F_NOWHITE | F_DEFWHITE):
		strcat(line, "Undefined");
		break;
	case (F_NOWHITE):
		strcat(line, "None");
		break;
	case (F_GREY | F_DEFWHITE):
		strcat(line, "White/Grey");
		break;
	case (F_GREY):
		strcat(line, "Grey/White");
		break;
	case (F_DEFWHITE):
		strcat(line, "Undefined");
		break;
	case (0):
		strcat(line, "White");
		break;
	}

	 if ((dipent.flags & (F_NOWHITE | F_GREY | F_DEFWHITE)) != F_NOWHITE) { 

		if (dipent.flags & F_NOPARTIAL) {
			strcat(line, ", No Partial (except to Master)");
		} else {
			switch (dipent.flags & (F_FAKE | F_DEFFAKE)) {
			case (F_FAKE | F_DEFFAKE):
				strcat(line, ", Partial Fakes Broadcast by default");
				break;
			case (F_FAKE):
				strcat(line, ", Partial may be Faked");
				break;
			case (F_DEFFAKE):
				strcat(line, ", Partial always Fakes Broadcast");
				break;
			case (0):
				strcat(line, ", Partial Allowed, No Fake");
				break;
			}
		}
	} else {
		strcat(line, " (except to Master)");
	}

	if(dipent.x2flags & X2F_POSTALPRESS)
		strcat(line,", postal style press");

	switch (dipent.flags & (F_OBWHITE | F_OBNONE)) {
                case (F_OBWHITE | F_OBNONE):
                        strcat(line, ", Observer is Undefined");
                        break;
                case (F_OBWHITE):
                       	    strcat(line, ", White observers");
                        break;
                case (F_OBNONE):
			if ((dipent.flags & (F_NOWHITE | F_GREY | F_DEFWHITE)) != F_NOWHITE )
			if (!(dipent.flags & F_NOWHITE)) { 
				strcat(line, ", No observers (except to Master)");
			}
                        break;
                case (0):
			
			if ((dipent.flags & F_NOPARTIAL)) {
	                        strcat(line,", Partial observers");
			}
                        break;
                }

	strcat(line, ".");
	print_params(fp, line);

	first_flag = 1;
	sprintf(line, "    Press Restrictions: ");
		
	/* Note, you MUST change dip.h to add new flags to X2F_PRESS_OPTIONS definitions
	 * Otherwise, they may not appear */	
		
	if (dipent.x2flags & X2F_TOUCHPRESS) {
		strcatf(line, "TouchPress", &first_flag);
	}
	if(dipent.xflags & XF_NOLATEPRESS)
	{
		strcatf(line, "No late press", &first_flag);
	}
	if(dipent.x2flags & X2F_MUSTORDER)
	{
		strcatf(line,"Valid orders required to send press", &first_flag);
	}
	/* Did you remember to alter X2F_PRESS_OPTIONS in dip.h ??? */

	if (!first_flag) 
		strcat(line,".");
	
	if ((dipent.x2flags & (X2F_PRESS_OPTIONS)) || (dipent.xflags & XF_NOLATEPRESS))
	    print_params(fp, line);
	

	/* EP and Miller/Boardman numbers; number of centres needed for win
	 * (these are short enough not to need print_params(), so print them
	 * directly to fp).
	 */

	if (dipent.epnum[0] != '\0') {
		fprintf(fp, "  EP#  :   %s.\n", dipent.epnum);
	}
	if (dipent.bn_mnnum[0] != '\0') {
		if (strlen(dipent.bn_mnnum) > 7) {
			fprintf(fp, "  Miller Number: %s.\n", dipent.bn_mnnum);
		} else {
			fprintf(fp, "  Boardman Number: %s.\n", dipent.bn_mnnum);
		}
	}

	fprintf(fp, "  Power Assignments: ");
	if (dipent.x2flags & X2F_PREFRANDONLY) {
		fprintf(fp, "Assigned Randomly Only.\n");
	} else {
		if (dipent.x2flags & X2F_PREFRANDALLOW) {
			fprintf(fp, "By Preference List or Randomly.\n");
		} else {
			fprintf(fp, "By Preference List Only.\n");
		}
	}

	if (dipent.x2flags & X2F_CAPTUREWIN) 
	    fprintf(fp, "  Win by capturing 1 power's home centres, or all draw at %d centres.\n", dipent.vp);
        else
            fprintf(fp, "  Winning Centers: %d.\n", dipent.vp);

	/* No of centres added. DAN. */
	if ((dipent.no_of_players != dipent.np) &&
	   ((!(dipent.x2flags & X2F_SECRET) || (signedon && (dipent.players[player].power == MASTER) && !broad_params)) ||
	   (dipent.phase[6] == 'X'))) {
		fprintf(fp, "  Number of Players: %d.\n", dipent.no_of_players);
	}
	/* Added the Index value, which indicates a measure of how far along the
	   game has progressed. */

	if (!(dipent.flags & F_MACH)) {
		int squares = 0, index, centers;

		/*  Loop on the total number of players signed on to the game
		   (including observers) to ensure that we get everybody!  */
		for (index = 0; index < dipent.n; index++) {
			centers = dipent.players[index].centers;
			squares += centers * centers;
		}
		fprintf(fp, "    Index:  %-3d\n", squares / dipent.np);
	}
	return;
}

/*****************************************************************************
NAME:  printlevel

DESCRIPTION
    This procedure returns the printable word that corresponds to the constant
    'level'.  This function uses Curt Marcus' strcap() function (in lib.c), and
    thus it is the caller's responsibility to free the string returned by this
    function.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    ?? Apr 1994 T.Ruelle     Original development.

INTERFACE DEFINITION
    Calling Sequence:
        string = printlevel(level);

    Inputs:
        level := Integral value for level

    Outputs:
        string := Pointer to buffer containing capitalised string value for
            level

ALGORITHM
    Loop through list of level values:
        If level value matches given level, return capitalised string.
    Endloop.
    Return capitalised string "Undefined".
***************************************************************************/

char *printlevel(int level)
{
	int x;

	/* Loop through list of level values: If level value matches given level,
	   return capitalized string.  */

	for (x = 1; x < nlevel; ++x) {
		if (lvalue[x] == level) {
			return strcap(levels[x]);
		}
	}

	return ("Undefined");
}

/**************************************************************************
NAME:  print_params

DESCRIPTION
    This procedure takes a line of output text, breaks it into pieces of a
    defined size or smaller (breaking only at spaces), and prints it to the
    specified output file.  An additional newline is added at the end of the
    line.  It is assumed that there are no imbedded newlines in the line before
    processing starts (except possibly the last character, if a following blank
    line is desired).

    This procedure is only intended for printing parameter lists, and indents
    broken lines accordingly.  Do not use this procedure for printing anything
    else.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    23 May 1994 C.Marcus,Jr. Original development (cloned from print_line()).

INTERFACE DEFINITION
    Calling Sequence:
        print_params(outf, &line);

    Inputs:
        outf := File pointer to output file
        line := Character string to be printed

    Outputs:
        line := Possibly garbaged character string, with imbedded nulls
        Text written to specified output file.

ALGORITHM
    Loop until length of last segment is less than cutoff point:
        Starting from cutoff point, search backward for last space, print up to
            that point, and insert a newline and the necessary indentation for
            the next line.
    Endloop.
    Print the remainder of the line (with terminating newline).
*******************************************************************************/

void print_params(FILE * outf, char *line)
{
	char *head, *tail;	/* Ptrs. to ends of string segment */
	char newline[] = "\n           ";	/* Newline plus indentation for next */

/*  Loop until length of last segment is less than cutoff point:  Starting
   from cutoff point, search backward for last space, print up to that point,
   and insert a newline and the necessary indentation for the next line.  */

	for (head = line, tail = line + CUTOFF_LENGTH;
	     strlen(head) > CUTOFF_LENGTH;
	     head = tail + 1, tail = head + CUTOFF_LENGTH) {
		while (!isspace(*tail))
			--tail;
		*tail = '\0';
		fputs(head, outf);
		fputs(newline, outf);
	}

/*  Print the remainder of the line (with terminating newline).  */

	fprintf(outf, "%s\n", head);
	return;
}
