/*
 * $Log$
 * Revision 1.4  2002/03/14 03:12:26  nzmb
 *
 * Added fix to ml_press.c to prevent people from getting warning messages (of if nno late press is on being prohibited from) sending press when the game is over.
 *
 * Revision 1.3  2001/05/10 09:05:05  greg
 * added subjectlines
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Various changes including
 *  - Partial press between master and only one power is NOT sent to others with WATCHALL set
 *  - Rejct press in a monor phase when XF_NOMINORPRESS is set
 *  - Change press handling to allow observers to press  in no-press games f allowed (i.e. BackSeat Driver variant)
 *  - Add prevention of press from late players, if flag is set
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

/*
 *  ml_press.c -- Process 'Press' and 'Broadcast' Commands
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
 *      10 Mar 1993 Positron     Tell Master the truth about the message.
 *      31 Mar 1994 C.Marcus,Jr. Complete redesign/rewrite.
 *      28 Apr 1994 C.Marcus,Jr. Always include sender's address to Master.
 *      23 May 1994 C.Marcus,Jr. Correct parsing of press options.
 *      27 Nov 1999 M. Miller	 Prohibit partial press from late players 
 *	07 Dec 1999 M. Miller	 parse_power doesn't allow duplicates
 */

#include <stdlib.h>
#include <string.h>

#include "dip.h"
#include "mail.h"
#include "functions.h"

/*
 *  Declarations & Definitions
 */

enum {
	NONE = 0,		/* Unrecognized option */
	GREY,			/* Grey press */
	WHITE,			/* White press */
	PARTIAL,		/* Press to selected individuals */
	ALLBUT,			/* To all but selected individuals */
	FAKEB,			/* Faked as broadcast */
	FAKEP,			/* Faked as to selected individuals */
	FAKEA,			/* Faked as not to selected individuals */
	NOFAKEB,		/* Override default of faked broadcasts */
};

#define is_observer(x)      (dipent.players[x].power == OBSERVER)

/*
 *  Public Function Declarations
 */

void mail_press(char *s, int need_opts);

/*
 *  Static Function Declarations
 */

static void print_line(FILE * outf, char *line);

/*
 *  Static Data Object Definitions
 */

static char *options[] =
{"", "grey", "white", "anonymous", "unanonymous",
 "anonymously", "unanonymously", "gray",
 "-", "to all but", "+", "to",
 "fake partial to all but", "fake parital -", "fake -",
 "fake to all but", "fake partial to", "fake to",
 "fake +", "fake broadcast", "no fake broadcast",
 "fake", "no fake "};

static int values[] =
{NONE, GREY, WHITE, GREY, WHITE,
 GREY, WHITE, GREY,
 ALLBUT, ALLBUT, PARTIAL, PARTIAL,
 FAKEA, FAKEA, FAKEA,
 FAKEA, FAKEP, FAKEP,
 FAKEP, FAKEB, NOFAKEB,
 FAKEB, NOFAKEB};

/*******************************************************************************
NAME:  mail_press

DESCRIPTION
    This procedure processes a 'press' or 'broadcast' command.  It parses the
    rest of the command line (if any), checks the validity of the command, and
    if valid then sets the appropriate flags so that the main command parser
    will interpret the following lines as a press message (to be either sent or
    ignored, as necessary).

    Press commands can have up to three types of options:  a colour (white or
    grey), a partial delivery list (either players to send the message to, or
    players not to send the message to), and a 'fake' option (fake the message
    as a broadcast, fake it as going to certain players, fake it as not going
    to certain players, or override a default fake broadcast).  None of these
    options can be specified more than once.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    ?? ??? 1987 K.Lowe       Original development.
    10 Mar 1993 Positron     Tell Master the truth about the source and desti-
                             nation(s) of the press/broadcast message.
    31 Mar 1994 C.Marcus,Jr. Complete redesign/rewrite, to allow 'press to m' to
                             always work no matter what press settings are.
    28 Apr 1994 C.Marcus,Jr. Always include the sender's address in press to
                             Master.
    23 May 1994 C.Marcus,Jr. Correct parsing of press options.

INTERFACE DEFINITION
    Calling Sequence:
        mail_press(s, need_opts);

    Inputs:
        s         := Pointer to command line, following 'press' or 'broadcast'
                     keyword
        need_opts := TRUE if options must be specified, FALSE if options are
                     optional (sorry about that wording!)

    Outputs:
        broadcast (global)    := TRUE if command is good, else FALSE
        broad_part (global)   := TRUE if press is only to some players, FALSE
                                 if press is to everybody
        broad_allbut (global) := TRUE if partial list is list of players to
                                 *not* receive message, FALSE if partial list
                                 is players to receive message
        broad_read (global)   := TRUE if message is to be read and sent, else
                                 FALSE
        broad_skip (global)   := TRUE if message is to be read and discarded,
                                 else FALSE
        broad_list (global)   := List of power letters to which message is to
                                 be sent (broad_allbut = FALSE) or not sent
                                 (broad_allbut = TRUE)

ALGORITHM
    [Note:  global return variables were initialized when broadcast files were
        opened, and do not need to be initialized here.]

    Initialize local flags as necessary.
    Parse the command line options, if any, setting local flags only.
    Validate command options against game flags.
    If command is valid, write press header to broadcast and/or master's broad-
        cast file(s).
    Set global return variables according to validity of local flags.
*******************************************************************************/

void mail_press(char *s, int need_opts)
{
/*
 *  Dynamic Data Object Definitions
 */
	int bad_cmd = 0,	/* Is this a bad command? */
	 have_opts = 0;		/* Have we processed any options? */
	char fake_list[sizeof(broad_list)],	/* List of fake destination powers */
	 game_name[32],		/* " in <name>:" string buffer */
	 line[1024],		/* Line to be sent to other players */
	 mline[1024],		/* Line to be sent to Master */
	 part_list[sizeof(broad_list)];
	/* List of destination powers */
	int color = NONE,	/* Is press grey, white, or default? */
	 count = 0,		/* Number of entries in part_list */
	 fake = NONE,		/* Is this message faked? */
	 fake_count = 0,	/* Number of entries in fake_list */
	 i,			/* Return value from lookfor() */
	 partial = NONE;	/* Is this message to some only? */
	char lpower;
	int xctr;

/*
 *  End of Definitions
 */

/*  Parse the command line options, if any, setting local flags only.  */
	sprintf(subjectline, "%s:%s - %s Diplomacy Notice ml_press", JUDGE_CODE, dipent.name, dipent.phase);

	master_press = 0;
	master_only_press = 0; 

	while (*s != '\0') {
		s = lookfor(s, options, nentry(options), &i);
		if (!i) {
			while (isspace(*s))
				++s;
			if (*s != '\0') {
				fprintf(rfp, "Unrecognized press/broadcast option '%s'.\n", s);
				bad_cmd = 1;
			}
			break;
		} else {
			have_opts = 1;
			switch (values[i]) {

			case GREY:
				if (color) {
					fprintf(rfp,
						"Multiple 'colors' specified in press command.\n");
					bad_cmd = 1;
				} else {
					color = GREY;
				}
				break;

			case WHITE:
				if (color) {
					fprintf(rfp,
						"Multiple 'colors' specified in press command.\n");
					bad_cmd = 1;
				} else {
					color = WHITE;
				}
				break;

			case PARTIAL:
				if (partial) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					partial = PARTIAL;
				}
				bad_cmd |= parse_powers(&s, part_list, sizeof(part_list),
							&count,1);
				break;

			case ALLBUT:
				if (partial) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					partial = ALLBUT;
				}
				bad_cmd |= parse_powers(&s, part_list, sizeof(part_list),
							&count,1);
				break;

			case FAKEB:
				if (fake) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					fake = FAKEB;
				}
				break;

			case FAKEP:
				if (fake) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					fake = FAKEP;
				}
				bad_cmd |= parse_powers(&s, fake_list, sizeof(fake_list),
							&fake_count,1);
				break;

			case FAKEA:
				if (fake) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					fake = FAKEA;
				}
				bad_cmd |= parse_powers(&s, fake_list, sizeof(fake_list),
							&fake_count,1);
				break;

			case NOFAKEB:
				if (fake) {
					fprintf(rfp,
						"Conflicting options specified in press command.\n");
					bad_cmd = 1;
				} else {
					fake = NOFAKEB;
				}
				break;
			}
		}
	}

/*  Validate command options against game flags:  first, if options were
   required, were they specified?  */

	if (need_opts && !have_opts) {
		fprintf(rfp,
			"You have specified no options.  If you want to send a plain\n%s",
			"message to everyone, please use the BROADCAST command.\n");
		bad_cmd = 1;

/*  Master can send any kind of press he wants, no matter what the settings.  */

	} else if (dipent.players[player].power == MASTER) {
	master_press = 1;
		/*  Good command by definition, so don't do any more checking.  */
	   if (partial == PARTIAL && count == 1) {
		/* Keep this between master and sender only */
		master_only_press = 1;
	   }

/*  Anyone can send to Master (only), no matter what the settings (but it won't
   be grey press, and he better not try to fake it!).  */

	} else if ((partial == PARTIAL) && (count == 1) &&
		   (power(part_list[0]) == MASTER)) {
		color = WHITE;
		master_press = 1;
		master_only_press = 1; /* Only for master and sender */
		if (fake) {
			fprintf(rfp,
				"Shame!  Attempting to fake out the Master is not allowed.\n");
			fake = NONE;
		}
/*  Checking for normal (i.e., not from or to Master) press:  first, is press
   allowed from non-observers (i.e. players)?  */

	} else {
		if ((dipent.flags & F_NOWHITE) && !(dipent.flags & F_GREY) && !is_observer(player)) {
			fprintf(rfp, "Game '%s' does not allow press.\n", dipent.name);
			bad_cmd = 1;

/*  If this is from an Observer, is observer press allowed?  */

		} else if ((dipent.flags & F_OBNONE) && is_observer(player)) {
			fprintf(rfp, "Game '%s' does not allow observer press.\n",
				dipent.name);
			bad_cmd = 1;

/* If a minor phase and no minor press allowed, reject it */
		} else if ((dipent.xflags & XF_NOMINORPRESS) && 
			    (dipent.phase[5] == 'B' || dipent.phase[5] == 'R')) {
			fprintf(rfp, "Game '%s' does not allow press in minor phases.\n",
				dipent.name);
			bad_cmd = 1;

/*  If this is grey press, is it allowed?  */

		} else {
			if (color == GREY) {
				if (!(dipent.flags & F_GREY)) {
					fprintf(rfp, "Game '%s' does not allow grey press.\n",
						dipent.name);
					bad_cmd = 1;
				} else if ((dipent.flags & F_OBWHITE) && is_observer(player)) {
					fprintf(rfp,
						"Game '%s' does not allow grey press from observers.\n",
						dipent.name);
					bad_cmd = 1;
				}
/*  If this is white press, is it allowed?  */

			} else if (color == WHITE) {
				if (dipent.flags & F_NOWHITE) {
					fprintf(rfp, "Game '%s' does not allow white press.\n",
						dipent.name);
					bad_cmd = 1;
				}
/*  Colour not specified; set default colour (Master defaults to white press;
   observers default to white if ObWhite is set; otherwise, default
   is grey if NoWhite is set, or if Grey is set and DefWhite is NOT set, or
   white in any other case).  */

			} else {
				if (((dipent.flags & F_OBWHITE) && is_observer(player)) ||
				    (dipent.players[player].power == MASTER)) {
					color = WHITE;
				} else {
					color = (((dipent.flags & F_GREY) &&
					 !(dipent.flags & F_DEFWHITE)) ||
						 (dipent.flags & F_NOWHITE)) ? GREY : WHITE;
				}
			}

/*  If this is partial press, is it allowed?  */

			if (partial) {
			    if (is_observer(player)) {
				if (dipent.flags & (F_OBNONE | F_OBWHITE )) {
					fprintf(rfp,"Game '%s' does not allow observer partial press.\n",
						dipent.name);
					bad_cmd = 1;
				}
			    } else {
				if (dipent.flags & F_NOPARTIAL) {
					fprintf(rfp, "Game '%s' does not allow partial press.\n",
						dipent.name);
					bad_cmd = 1;
				}
			    }
			}
/*  If this is a no-fake-broadcast, is it allowed?  */

			if (fake == NOFAKEB) {
				if (!(dipent.flags & F_FAKE) && (dipent.flags & F_DEFFAKE)) {
					fprintf(rfp,
						"Game '%s' only allows partial press to fake broadcast.\n",
						dipent.name);
					bad_cmd = 1;
				}
/*  If this is a fake broadcast, is it allowed?  */

			} else if (fake == FAKEB) {
				if (!(dipent.flags & F_FAKE) && !(dipent.flags & F_DEFFAKE)) {
					fprintf(rfp, "Game '%s' does not allow fake press.\n",
						dipent.name);
					bad_cmd = 1;
				}
/*  If this is a fake partial, is it allowed?  */

			} else if (fake) {
				if (dipent.flags & F_NOPARTIAL) {
					fprintf(rfp, "Game '%s' does not allow partial press.\n",
						dipent.name);
					bad_cmd = 1;
				} else if (!(dipent.flags & F_FAKE) &&
					   (dipent.flags & F_DEFFAKE)) {
					fprintf(rfp,
						"Game '%s' only allows partial press to fake broadcast.\n",
						dipent.name);
					bad_cmd = 1;
				} else if (!(dipent.flags & F_FAKE)) {
					fprintf(rfp, "Game '%s' does not allow fake press.\n",
						dipent.name);
					bad_cmd = 1;
				}
/*  Fake option not specified; default to fake broadcast if message is partial
   and partial fakes broadcast.  */

			} else {
				if (partial && (dipent.flags & F_DEFFAKE)) {
					fake = FAKEB;
				}
			}
		}
	}


/* OK, let's see if press from late powers is allowed */
	if (!(dipent.phase[6] == 'X')) 
	{
		if (WAITING(dipent.players[player].status)) {
			/* it is not, let us see if this power is late and not pressing to master */
		    if (!((count == 1) && (power(part_list[0]) == MASTER))) {
			/* We are waiting for him, see if messag is only to master */
			if (time(NULL) > dipent.deadline) { 
		          if (dipent.xflags & XF_NOLATEPRESS) {
			  	fprintf(rfp, "Game '%s' does not allow press from late players.\n",
                                                dipent.name);
				fprintf(rfp, "You must submit moves for ALL your units before pressing.\n");
                                bad_cmd = 1;
			  } else {
			    /* you can press when late, but it's not really good so... */
			        fprintf(rfp, 
				"Note: You are marked as late and should really have\n      corrected this before sending press.\n\n");
			  }
			}
		    }
		}
	}
			
/*  If command is invalid, discard the following text.  */

	if (bad_cmd) {
		fprintf(rfp, "\nNo press/broadcast message sent, discarding text:\n\n");
		broad_skip = 1;

/*  If command is valid, write press header to broadcast and/or master's
   broadcast file(s):  first, tell the sender what we're doing.  */

	} else {
		if (partial) {
			sprintf(line, "Message sent to %s", (partial == ALLBUT) ?
				"all but " : "");
			list_powers(line, part_list);
			strcat(line, ":\n");
		} else {
			strcpy(line, "Broadcast message sent:\n");
		}
		print_line(rfp, line);

/*  Tell the recipients what we want them to believe, and tell the Master the
   truth:  if it's grey partial press (or we want them to think so), the other
   players get "Message to ", while the Master gets "Message [from <address> as
   <power>] to ".  */

		if ((partial && (fake != FAKEB)) || (fake == FAKEP) || (fake == FAKEA)) {
			if (color == GREY) {
				sprintf(subjectline, "%s:%s - %s Press to ", JUDGE_CODE, dipent.name, dipent.phase);

				strcpy(line, "Message to ");
				sprintf(mline, "Message [from %s as %s] to ",
					dipent.players[player].address,
				   powers[dipent.players[player].power]);

/*  If it's white partial press, either in a non-Gunboat game or from the
   Master, the other players and the Master get "Message from <address> as
   <power> to ".  */

			} else {
				if (!(dipent.flags & F_GUNBOAT) ||
				    (dipent.players[player].power == MASTER)) {
					lpower = dipent.pl[dipent.players[player].power];
					sprintf(subjectline, "%s:%s - %s Press from %c to ", JUDGE_CODE, dipent.name, dipent.phase, lpower);

					sprintf(line, "Message from %s as %s to ", raddr,
						powers[dipent.players[player].power]);
					sprintf(mline, "Message from %s as %s to ", raddr,
						powers[dipent.players[player].power]);

/*  If it's white partial press from an observer or player (Gunboat game), the
   other players get "Message from <power> to ", while the Master gets "Message
   from [<address> as] <power> to ".  */

				} else {
					lpower = dipent.pl[dipent.players[player].power];
					sprintf(subjectline, "%s:%s - %s Press from %c to ", JUDGE_CODE, dipent.name, dipent.phase, lpower);

					sprintf(line, "Message from %s to ",
						powers[dipent.players[player].power]);
					sprintf(mline, "Message from [%s as] %s to ",
					  dipent.players[player].address,
						powers[dipent.players[player].power]);
				}
			}

/*  If the partial press is to all but certain powers (or we wish them to think
   so), add "all but ".  */

			if ((fake == FAKEA) || (!fake && (partial == ALLBUT))) {
				strcat(subjectline, "all but ");
				strcat(line, "all but ");
				strcat(mline, "all but ");
			}
/*  List the destination (or supposed destination) powers.  */
			xctr = 0;
			while ((fake_list[xctr] = toupper(fake_list[xctr])) != '\0') xctr++;
			xctr =0;
			while ((part_list[xctr] = toupper(part_list[xctr])) != '\0') xctr++;

			strcat(subjectline, ((fake == FAKEP) || (fake == FAKEA)) ? fake_list :
				part_list);

			list_powers(line, ((fake == FAKEP) || (fake == FAKEA)) ? fake_list :
				    part_list);
			list_powers(mline, ((fake == FAKEP) || (fake == FAKEA)) ? fake_list
				    : part_list);

/*  If it's grey broadcast press (or we want them to think so), the other
   players get "Broadcast message", while the Master gets "Broadcast message
   [from <address> as <power>]".  */

		} else {
			if (color == GREY) {
				sprintf(subjectline, "%s:%s - %s Broadcast", JUDGE_CODE, dipent.name, dipent.phase);

				strcpy(line, "Broadcast message");
				sprintf(mline, "Broadcast message [from %s as %s]",
					dipent.players[player].address,
				   powers[dipent.players[player].power]);

/*  If it's white broadcast press, either in a non-Gunboat game or from the
   Master, the other players and the Master get "Broadcast message from
   <address> as <power>".  */

			} else {
				if (!(dipent.flags & F_GUNBOAT) ||
				    (dipent.players[player].power == MASTER)) {
					lpower = dipent.pl[dipent.players[player].power];
					sprintf(subjectline, "%s:%s - %s Broadcast from %c", JUDGE_CODE, dipent.name, dipent.phase, lpower);

					sprintf(line, "Broadcast message from %s as %s", raddr,
						powers[dipent.players[player].power]);
					sprintf(mline, "Broadcast message from %s as %s", raddr,
						powers[dipent.players[player].power]);

/*  If it's white broadcast press from an observer or player (Gunboat game), the
   other players get "Broadcast message from <power>", while the Master gets
   "Broadcast message from [<address> as] <power>".  */

				} else {
					lpower = dipent.pl[dipent.players[player].power];
					sprintf(subjectline, "%s:%s - %s Broadcast from %c", JUDGE_CODE, dipent.name, dipent.phase, lpower);

					sprintf(line, "Broadcast message from %s",
						powers[dipent.players[player].power]);
					sprintf(mline, "Broadcast message from [%s as] %s",
					  dipent.players[player].address,
						powers[dipent.players[player].power]);
				}
			}
		}


/*  Make sure the Master knows who it's really to.  */

		if (fake && (fake != NOFAKEB)) {
			if (partial) {
				if (partial == ALLBUT) {
					strcat(mline, " [really to all but ");
				} else {
					strcat(mline, " [really to ");
				}
				list_powers(mline, part_list);
				strcat(mline, "]");
			} else {
				strcat(mline, " [really to everyone]");
			}
		}
/*  Finish up with the game name, and print it.  */

		sprintf(game_name, " in '%s':\n", dipent.name);
		strcat(line, game_name);
		strcat(mline, game_name);
		print_line(rfp, line);
		fprintf(bfp, "%s\n", line);	/* The previous call has put in the new-
						   lines except the extra at the end, so
						   we don't have to call print_line(). */
		print_line(mbfp, mline);

/*  Set global return variables according to validity of local flags.  */

		broadcast = 1;
		broad_part = (partial != NONE);
		broad_allbut = (partial == ALLBUT);
		broad_read = 1;
		if (partial)
			strcpy(broad_list, part_list);
	}
	return;
}
/*******************************************************************************
NAME:  parse_powers

DESCRIPTION
    This procedure parses a list of power letters, checking that each letter is
    valid, and that not too many are specified, and stores them in the specified
    array.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    31 Mar 1994 C.Marcus,Jr. Original development (extracted from original
                             mail_press() procedure).

INTERFACE DEFINITION
    Calling Sequence:
        error = parse_powers(&s, &list, size, &count);

    Inputs:
        s    := Pointer to command line, at beginning of power letters list
        size := Size of list array
	put_output: =1 if to go to rfp, else not.

    Outputs:
        s     := Updated pointer, at next character after power letters list
        error := TRUE if parsing error occurred, else FALSE
        list  := Character array containing validated power letters
        count := Number of valid power letters parsed

ALGORITHM
    Initialize count and list array, and skip any spaces in command line.
    While not at end of command line or at space:
        If next character is valid power letter, and room remains in array,
            store the power letter and count it, else issue error message.
    Endwhile.
*******************************************************************************/

int parse_powers(char **s, char *list, size_t size, int *count, int put_output)
{
/*
 *  Dynamic Data Object Definitions
 */
	int error = 0;		/* Parsing error occurred? */
	char *next = list;	/* Place to store next letter */
	int power_used[MASTER+1];
        int i;
/*
 *  End of Definitions
 */

/*  Initialize count and list array, and skip any spaces in command line.  */

	for (i=0; i <= MASTER; i++) power_used[i] = 0;
	
	*count = 0;
	memset((void *) list, (int) '\0', size);
	while (isspace(**s))
		++(*s);

/*  While not at end of command line or at space:  If next character is valid
   power letter, and room remains in array, store the power letter and count
   it, else issue error message.  */

	while (**s && !isspace(**s)) {
		power_used[power(**s)]++;
		if (!power(**s)) {
			if (put_output) fprintf(rfp, "Unknown power specification '%c'.\n", **s);
			error = 1;
		} else if (next >= list + size) {
			if (put_output) fprintf(rfp, "Too many power specifications.\n");
			error = 1;
		} else if (power_used[power(**s)] == 2 ) {
			if (put_output) fprintf(rfp, "Power '%c' repeated in list. \n", **s);
			error = 1;
		} else {
			*next++ = **s;
			++(*count);
		}
		++(*s);
	}
	return error;
}
/*******************************************************************************
NAME:  list_powers

DESCRIPTION
    This procedure takes a list of power letters, and adds the corresponding
    names to the indicated line to be printed out.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    31 Mar 1994 C.Marcus,Jr. Original development (extracted from original
                             mail_press() procedure).

INTERFACE DEFINITION
    Calling Sequence:
        list_powers(&line, &list);

    Inputs:
        list := Character array containing valid power letters

    Outputs:
        line := Character string to which power names are concatenated

ALGORITHM
    For each letter in list:
        Concatenate corresponding power name to output line.
        If two or more letters remain, add a comma; else, if only one letter
            remains, add "and".
    Endloop.
*******************************************************************************/

void list_powers(char *line, char *list)
{
/*
 *  Dynamic Data Object Definitions
 */

/*
 *  End of Definitions
 */

/*  For each letter in list:  Concatenate corresponding power name to output
   line.  */

	for ( /* list pointer already set */ ; *list; ++list) {
		strcat(line, powers[power(*list)]);

/*  If two or more letters remain, add a comma; else, if only one letter
   remains, add "and".  */

		if (*(list + 1)) {
			if (*(list + 2)) {
				strcat(line, ", ");
			} else {
				strcat(line, " and ");
			}
		}
	}
	return;
}
/*******************************************************************************
NAME:  print_line

DESCRIPTION
    This procedure takes a line of output text, breaks it into pieces of a
    defined size or smaller (breaking only at spaces), and prints it to the
    specified output file.  An additional newline is added at the end of the
    line.  It is assumed that there are no imbedded newlines in the line before
    processing starts (except possibly the last character, if a following blank
    line is desired).

    Note that upon return, the newlines inserted to break up the line are still
    present, so that the line may be printed again (perhaps to a different out-
    put file) without calling print_line() a second time.  However, the termina-
    ting newline is not added to the array.

REVISION HISTORY
    DATE        NAME         REASON
    ----------- ------------ ---------------------------------------------------
    31 Mar 1994 C.Marcus,Jr. Original development (extracted from original
                             mail_press() procedure).

INTERFACE DEFINITION
    Calling Sequence:
        print_line(outf, &line);

    Inputs:
        outf := File pointer to output file
        line := Character string to be printed

    Outputs:
        line := Character string as printed, possibly with embedded newlines
        Text written to specified output file.

ALGORITHM
    Loop until length of last segment is less than cutoff point:
        Starting from cutoff point, search backward for last space, and change
            it to a newline.
    Endloop.
    Print the line (with terminating newline).
*******************************************************************************/

static void print_line(FILE * outf, char *line)
{
/*
 *  Dynamic Data Object Definitions
 */
	char *head, *tail;	/* Ptrs. to ends of string segment */
/*
 *  End of Definitions
 */

/*  Loop until length of last segment is less than cutoff point:  Starting
   from cutoff point, search backward for last space, and change it to a new-
   line.  */

	for (head = line, tail = line + CUTOFF_LENGTH;
	     strlen(head) > CUTOFF_LENGTH;
	     head = tail + 1, tail = head + CUTOFF_LENGTH) {
		while (!isspace(*tail))
			--tail;
		*tail = '\n';
	}

/*  Print the line (with terminating newline).  */

	fprintf(outf, "%s\n", line);
	return;
}

