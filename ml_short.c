/*
 * $Log$
 * Revision 1.1  2000/11/14 14:27:37  miller
 * Initial revision
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.3  1997/03/16 06:53:13  rpaar
 *
 * Revision 1.2  1996/11/05 23:11:36  rpaar
 * USIT changes to fix minor bugs
 */

/*  ml_short. -- process incoming shorthand mail
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
#include <time.h>

#include "dip.h"
#include "mail.h"
#include "functions.h"
#include "conf.h"
#include "porder.h"

#define SH_PROPOSE 	1
#define SH_ACCEPT 	2
#define SH_REJECT 	3
#define SH_INFORM 	4
#define SH_DRAW 	5
#define SH_PEACE 	6
#define SH_ALLY 	7
#define SH_TARGET 	8
#define SH_DMZ		9
#define SH_NOT		10
#define SH_PROVINCE     11
#define SH_ORDER        15

/* And these do not appear in 'searchable' list */
#define SH_BLANK	12
#define SH_PHASE	13
#define SH_POWER	14
#define SH_PROV_LIST	16
#define SH_ERROR	17
#define SH_NOPOWERS     18
#define SH_NOPROV	19
#define SH_SEPARATOR	20
#define SH_MASTER	21 
#define SH_OBSERVER	22

static char *sh_prelim[] =
{"", 
 "proposes","propose",
 "accepts", "accept",
 "rejects", "reject",
 "informs", "inform",
 "draw", 
  "peace", 
  "alliance","ally", 
 "targets","target", 
  "dmz", 
 "not", "!", 
 "provinces","province", "prov",
 "orders", "order",
  ","
  };
static int sh_pvalue[] =
{0, 
 SH_PROPOSE, SH_PROPOSE,
 SH_ACCEPT, SH_ACCEPT,
 SH_REJECT, SH_REJECT,
 SH_INFORM, SH_INFORM,
 SH_DRAW, 
 SH_PEACE,
 SH_ALLY, SH_ALLY,
 SH_TARGET, SH_TARGET,
 SH_DMZ, 
 SH_NOT, SH_NOT,
 SH_PROVINCE, SH_PROVINCE, SH_PROVINCE,
 SH_ORDER, SH_ORDER,
 SH_SEPARATOR
};


#define NO_POWERS 0
#define CANBE_POWERS 1
#define MUSTBE_POWERS 2
#define NO_PROVINCE 0
#define CANBE_PROVINCE 0
#define MUSTBE_PROVINCE 2

int ProcessShSegment(char **s, char *rrret_text, int power_list, int prov_list);
/*
 * This function shoud be replaced by the phase function in phase.c
 */

int CheckShSegmentPhase(char *s, char *rret_text) 
{
	char poss_phase[7];
	rret_text[0] = '\0';
	strncpy(poss_phase, s, 6);
	poss_phase[6] = '\0';
	if (!isalnum(*s)) {
		while (!isalnum(*s) && *s != '\0' ) s++; /* Skip spaces */
        }	
	if (GetOneWord(poss_phase) == 0 ) return SH_BLANK;

	if (strlen(s) < 6 ) return SH_ERROR;
	/* Only accept annnl */
	switch (toupper(s[0]))
	{
		case 'F':
		case 'S':
			rret_text[0]=toupper(s[0]);
			break;
		default:
			return SH_ERROR;
	}

	if (!isdigit(s[1]) || !isdigit(s[2]) || !isdigit(s[3]) || !isdigit(s[4]) )
	{
		return SH_ERROR;
	}
	rret_text[1] = s[1];
	rret_text[2] = s[2];
	rret_text[3] = s[3];
	rret_text[4] = s[4];
	switch (toupper(s[5]))
	{
		case 'M':
		case 'R':
			rret_text[5] = toupper(s[5]);
			break;
		case 'B':
			/* Check if fall, as only fall has a build phase */
			if (rret_text[0] == 'F' ) {
			     rret_text[5] = toupper(s[5]);
                             break;
			}
			/* Intentional fall-through */

		default:
			rret_text[5] = '\0';
			return SH_ERROR;
	}

	rret_text[6] = '\0';  /* Null-terminate it */

	return SH_PHASE;
}

#define NOIFS 0

int mail_shorthand(char *text)
{
	char sh_line[1024];
	int ret;
	int last_ret = SH_BLANK;
	char ret_text[1024];
	int com_type = 0; /* =1 for propose, accept, reject */
	int com_class = 0; /* =1 for draw, peace, DMZ, prov, order */
        int com_dmz = 0;  /* Number of times DMZ encountered */
	int com_not = 0; /* Number of times NOT encountered */
	int prov_list = 0;
	int first_entry = 0;
	char *text1;
        char phase_text[7];
        char phase_type;

	phase_text[0]='\0';
	text1 = text;
	
	sh_line[0] = '\0';
	ret_text[0] = '\0';
	/* firstly, strip away leading blanks */
	despace(text);
	if (*text1 == '\0') {
		/* blank line, ignore */
		*text = '\0';
		return 0;
	}
	if (!dipent.pr_valid) {
		/* pr[] must be filled to run this code */
		po_init();
		gamein();
		dipent.pr_valid++;
	}

	while (!com_type) {
		ret = ProcessShSegment(&text1, ret_text, CANBE_POWERS, NO_PROVINCE);
		switch (ret)
		{
			case SH_INFORM:
			case SH_POWER:
			{
				strcat(sh_line, ret_text);
				break;
			}
			case SH_PROPOSE:
			case SH_ACCEPT:
			case SH_REJECT:
			{
			        if (last_ret == SH_INFORM) {
				    strcat(sh_line, "\nError: Inform must be followed by a powerlist.");
				    strcpy(text,sh_line);
                                    strcat(text,"\n");
				    return 1;	
				}
				strcat(sh_line,ret_text);
				com_type++;
				break;
			}
			case SH_BLANK:
			{
				if (first_entry == 0) {
				    /* blank line, return and ignore */
				    *text = 0;
				    return 0;
				}
				strcat(sh_line, "\nError: Line ended before finding Propose/Accept/Reject.");
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 1;
				break;
			}
			case SH_MASTER:
			{
				strcat(sh_line, "\nError: Master is not a valid power in shorthand power list.");
                                strcpy(text,sh_line);
                                strcat(text,"\n");
                                return 1;
			}
			default:
				strcat(sh_line,"\nError: Expected Propose/Accept/Reject not ");
				strcat(sh_line, ret_text);
				strcat(sh_line,".\n");
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 1;	/* Error return */
		}
		first_entry++;
		last_ret = ret;

	};

	/* OK, now look for a power/power list */
	ret = ProcessShSegment(&text1,ret_text, MUSTBE_POWERS, NO_PROVINCE);
	switch (ret)
	{
		case SH_POWER:
		{
			strcat(sh_line, ret_text);
			break;
		}
		case SH_BLANK:
		{
			strcat(sh_line, "\nError: Line ended before finding a power.");
			strcpy(text,sh_line);
			strcat(text,"\n");
			return 1;
			break;
		}
		case SH_MASTER:
		{
			strcat(sh_line, "\nError: Master is not a valid power in powerlist.");
                        strcpy(text,sh_line);
                        strcat(text,"\n");
                        return 1;
	        }
		case SH_OBSERVER:
		{
			strcat(sh_line, "\nError: Observer is not allowed here as a power.");
                        strcpy(text,sh_line);
                        strcat(text,"\n");
                        return 1;
		}

		default:
			strcat(sh_line,"\nError: Expected Power not ");
                        strcat(sh_line, ret_text);
                        strcat(sh_line,".\n");
                        strcpy(text,sh_line);
			strcat(text,"\n");
			return 1;       /* Error return */
	}
	/* Right, now see which command class it is */
        while (!com_class) {
                ret = ProcessShSegment(&text1, ret_text, NO_POWERS, CANBE_PROVINCE);
                switch (ret)
                {
                        case SH_DRAW:
                        case SH_PEACE:
			{
				/* These commands end here */
				strcat(sh_line, ret_text);
				if (CheckShSegmentPhase(text1, phase_text) == SH_PHASE ) {
                                        text1 += 6; /* Skip the found phase */
                                        phase_type = phase_text[5];
					strcat(sh_line," ");
                                        strcat(sh_line, phase_text);
                                        strcat(sh_line," ");
                                }
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 0;
			}
			case SH_ALLY:
				strcat(sh_line, ret_text);
				if (CheckShSegmentPhase(text1, phase_text) == SH_PHASE ) {
                                        text1 += 6; /* Skip the found phase */
                                        phase_type = phase_text[5];
                                        strcat(sh_line, phase_text);
                                        strcat(sh_line," ");
                                }
				ret = ProcessShSegment(&text1, ret_text, NO_POWERS, NO_PROVINCE);
                		switch (ret)
				{
					case SH_BLANK:
						strcpy(text,sh_line);
						strcat(text,"\n");
						return 0;
						break;

					case SH_TARGET:
						strcat(sh_line, ret_text);
                                		ret = ProcessShSegment(&text1, ret_text, MUSTBE_POWERS, NO_PROVINCE);
						switch (ret)
                                		{
							case SH_POWER:
								strcat(sh_line, ret_text);
								strcpy(text,sh_line);
								strcat(text,"\n");
								return 0;
								break;
							case SH_BLANK:
								strcat(sh_line,"\nError: Expected powerlist.");
								strcpy(text,sh_line);
								strcat(text,"\n");
								return 1;
								break;
                					case SH_MASTER:
                					{
                        					strcat(sh_line, "\nError: Master is not a valid power in powerlist.");
                        					strcpy(text,sh_line);
                        					strcat(text,"\n");
                        					return 1;
                					}
                					case SH_OBSERVER:
                					{
                        					strcat(sh_line, "\nError: Observer is not allowed here as a power.");
                        					strcpy(text,sh_line);
                        					strcat(text,"\n");
                        					return 1;
                					}
							default:
								strcat(sh_line,"\nError: Expected powerlist not ");
								strcat(sh_line,ret_text);
								strcat(sh_line, ret_text);
								strcpy(text,sh_line);
								strcat(text,"\n");
								return 1;
						}
					default:
					{
						strcat(sh_line,"\nError: Not expected ");
                                                                strcat(sh_line,ret_text);
                                                                strcat(sh_line, ret_text);
                                                                strcpy(text,sh_line);
								strcat(text,"\n");
								return 1;
					}
				}
				break; /* out of courtesy */
			case SH_DMZ:
			{
				/* Duplicate instances of DMZ are treated as an error */
				if (com_dmz != 0) {
					strcpy(sh_line,"\n:Error - Duplicate DMZ specified.");
					strcpy(text,sh_line);
					strcat(text,"\n");
					return 1;
				}
				/* else */
				com_class++;
				com_dmz++;
				prov_list++;
				strcat(sh_line, ret_text);
				if (CheckShSegmentPhase(text1, phase_text) == SH_PHASE ) {
                                        text1 += 6; /* Skip the found phase */
                                        phase_type = phase_text[5];
                                        strcat(sh_line, phase_text);
                                        strcat(sh_line," ");
                                }
				break;

			}
			case SH_ORDER:
			{
				strcat(sh_line,ret_text);
        			/* See if there is a phase first, to know how to parse the order */
            			if (CheckShSegmentPhase(text1, phase_text) == SH_PHASE ) {
                			text1 += 6; /* Skip the found phase */
			                phase_type = phase_text[5];
					strcat(sh_line, phase_text);
					strcat(sh_line," ");
			        } else {
			                phase_type = dipent.phase[5]; /* Use current phase */
					phase_text[0]='\0';
			        }
			        switch (phase_type) {
			                case 'B':
			                        if (build_syntaxcheck(text1,NOIFS, &sh_line[strlen(sh_line)]) == 0) {
			                        } else {
                                                  strcpy(ret_text, "Invalid order:");
                                                  strcat(ret_text,text1);
                                                  strcat(sh_line,ret_text);
                                                strcpy(text,sh_line);
                                                strcat(text,"\n");
                                                return 1;

                                                }

			                        break;
			                case 'R':
       			                 if (retreat_syntaxcheck(text1,NOIFS, &sh_line[strlen(sh_line)]) == 0)  {
						} else {
                                                  strcpy(ret_text, "Invalid order:");
                                                  strcat(ret_text,text1);
                                                  strcat(sh_line,ret_text);
                                                strcpy(text,sh_line);
                                                strcat(text,"\n");
                                                return 1;

                                                }

			                        break;
			
       			         	case 'M':
			                        if (move_syntaxcheck(text1,NOIFS, &sh_line[strlen(sh_line)]) == 0) {
			                        } else {
						  strcpy(ret_text, "Invalid order:");
						  strcat(ret_text,text1);
						  strcat(sh_line,ret_text);
                                                strcpy(text,sh_line);
                                                strcat(text,"\n");
                                                return 1;

						}
					
						break;
					default:
						strcat(sh_line,
						       "\nError: bad phase type\n"); 
                                                strcat(sh_line,ret_text);
                                                strcpy(text,sh_line);
                                                strcat(text,"\n");
                                                return 1;
			        }
				/* It was decoded as an order so leave! */
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 0;
			}
	
			case SH_PROVINCE:
			{
				com_class++;
                                com_dmz++;
                                prov_list++;
                                strcat(sh_line, ret_text);
				if (CheckShSegmentPhase(text1, phase_text) == SH_PHASE ) {
                                        text1 += 6; /* Skip the found phase */
                                        phase_type = phase_text[5];
                                        strcat(sh_line, phase_text);
                                        strcat(sh_line," ");
                                }
                                break;
			}
			case SH_NOT:
			{
                               /* Duplicate instances of NOT are treated as an error */
                                if (com_not != 0) {
                                        strcpy(sh_line,"\n:Error - Duplicate NOT specified.");
                                        strcpy(text,sh_line);
					strcat(text,"\n");
					return 1;
                                }
                                /* else */
                                com_not++;
                                strcat(sh_line, ret_text);
                                break;
			}
			case SH_BLANK:
			{
				strcat(sh_line, "\n:Error - Command class expected.");
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 1;
			}
			default:
				strcat(sh_line,"\nError: Expected command not ");
                        	strcat(sh_line, ret_text);
                        	strcat(sh_line,".");
                        	strcpy(text,sh_line);
				strcat(text,"\n");
				return 1;       /* Error return */
		}
	};
	/* OK, at this point, we're expecting a list of provinces only */
       
       while (prov_list != 0 && prov_list < 4) {
	   ret = ProcessShSegment(&text1, ret_text, NO_POWERS, MUSTBE_PROVINCE);
	   switch (ret)
           {
		case SH_PROV_LIST:
		{
			if (prov_list != 1) strcat(sh_line, ",");
			strcat(sh_line, ret_text);
			prov_list++;
			if (prov_list >= 4) { 
				/* Got the maximum number of provinces */
				strcpy(text,sh_line);
				strcat(text,"\n");
				return 0;
			}
			break;
		}
		case SH_BLANK:
		{
			prov_list = 0;
			strcpy(text,sh_line);
			strcat(text,"\n");
			return 0; /* That's the end! */
		}
		case SH_SEPARATOR:
		{
			break;  /* ignore separators */
		}
		default:
			prov_list = 0;
			strcat(sh_line,"\nError: Expected province not ");
                                strcat(sh_line, ret_text);
                                strcat(sh_line,".");
                                strcpy(text,sh_line);
				strcat(text,"\n");
				return 1;       /* Error return */
	     }
	};
	
	/* Should never get here! */
	strcpy(text,"**Unhandled shorthand line!**\n");
	return 1;
}
/* Prossess Shorthand single word 
 * Parameters are:
 * s : input text, from which first word is taken
 * rrret_text: returned string
 * power_list: set to 0 if a power is not possible in this context
 * prov_list: set to 0 if a province is not possible in this context
 * orderable: set to 0 if an order is not possible in this context
 */

int ProcessShSegment(char **s, char *rrret_text, int power_list, int prov_list)
{	
	static char power_text[100];
	static char in_text[150];
	char *t = power_text;
	char part_list[MAX_POWERS+1];
	int count;
	
	int p1,c1;  /* dummies for get_prov only */

	int i;
        int sh_error = 0;
	*rrret_text='\0'; /* Initialise output text */


	/* OK, first lets extract first word from 's' as it could be a power */
	strcpy(power_text, *s);
	if (GetOneWord(power_text) == 0) return SH_BLANK; /* nothing there! */
	strcpy(in_text, *s); /* Remember text! */

       if (power_list != NO_POWERS) {
        /* OK, it could be a power, let's check! */
            if (parse_powers(&t, part_list, dipent.np, &count,0) == 0 ) {
	    	    /*list_powers(rrret_text, power_text); */
		    i = 0;
		    while (power_text[i]!= 0) {
			power_text[i] = toupper(power_text[i]);
			i++;
		    }
		    strcpy(rrret_text, power_text); 
		    strcat(rrret_text," ");
	            while ( isspace(**s)== 0 && **s != '\0') (*s)++; /* skip until spaces start */		
		    while ( isspace(**s)!= 0 && **s != '\0') (*s)++; /* skip until spaces end */
		    /* The power 'M' is not allowed */
		    if (strchr(power_text, (int) 'M' ) != NULL ) return SH_MASTER;
		    /* The power 'O' is not allowed in the last powerlist */
		    if (strchr(power_text, (int) 'O') != NULL && power_list == MUSTBE_POWERS) return SH_OBSERVER;		    
		    return SH_POWER;
	    }
	    if (power_list == MUSTBE_POWERS) return SH_NOPOWERS; 
	}
	
        if (prov_list != NO_PROVINCE) {
        /* OK, it could also be a province, let's check */
		t = get_prov(*s, &p1, &c1);
		if (p1 != 0) /* indicates a province found */ {
			*s = t; /* resume AFTER match position */
			strcpy(rrret_text," ");
			strcat(rrret_text, pr[p1].name);
			return SH_PROV_LIST;
		}
		if (power_list == MUSTBE_PROVINCE) return SH_NOPROV;
        }
	*s = lookfor(*s, sh_prelim, nentry(sh_prelim), &i);

	sh_error = sh_pvalue[i];
	
	switch (sh_pvalue[i]) {
	
		case SH_PROPOSE:
			strcpy(rrret_text,"proposes ");
			break;

		case SH_ACCEPT:
			strcpy(rrret_text,"accepts ");
			break;

		case SH_REJECT:
			strcpy(rrret_text,"rejects ");
			break;

		case SH_INFORM:
			strcpy(rrret_text,"informs ");
			break;

		case SH_DRAW:
			strcpy(rrret_text,"draw");
			break;

		case SH_PEACE:
			strcpy(rrret_text,"peace");
			break;

		case SH_ALLY:
			strcpy(rrret_text,"alliance ");
			break;

		case SH_TARGET:
			strcpy(rrret_text,"target ");
			break;

		case SH_DMZ:
			strcpy(rrret_text, "DMZ ");
			break;

		case SH_NOT:
			strcpy(rrret_text, "NOT ");
			break;

		case SH_PROVINCE:
			strcpy(rrret_text, "province ");
			break;

		case SH_SEPARATOR:
			strcpy(rrret_text,", ");
			break;
		case SH_ORDER:
			strcpy(rrret_text,"order ");
			break;

		default:
			GetOneWord(in_text);
			sprintf(rrret_text,"'%s'", in_text);
			sh_error = SH_ERROR;
	}

	return sh_error;
}

