
/*
 * $Log$
 * Revision 1.4  2001/05/30 21:16:46  miller
 * better lookfor fix
 *
 * Revision 1.3  2001/05/30 21:03:48  miller
 * Lookfor will terminate search only if string has really ended
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Lots of changes, including
 *  - get_die_magic() Get DIE_MAGIC value from .magic.dat (or cerate if not found)
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.2  1996/11/05 23:09:52  rpaar
 * USIT changes to fix minor bugs
 */

/*  lib.c
 *  Copyright 1987, Lowe.
 *
 *  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
 *  Maryland, all rights reserved; used with permission.
 *
 *  Redistribution and use in source and binary forms are permitted
 *  provided that it is for non-profit purposes, that this and the 
 *  above notices are preserved and that due credit is given to Mr.
 *  Lowe.
 *
 *  Version History
 *
 *  Version Author          Date      Comments
 * ------------------------------------------------------------------------
 *  1       Ken Lowe        1987      Original Code
 *  2       David Norman    19/05/96  Unobfuscate get_prov
 *                                    Fix get_prov to cope with a province 
 *                                    "New York" and an abbreviation "New"
 *                                    for a different province
 *                                    Fix get_prov to differ between "Laos"
 *                                    and "Lao s"
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "dip.h"
#include "functions.h"
#include "porder.h"
/* .magic.h no longer used */
/*#include ".magic.h"*/
#include "mail.h"

/* TODO make this static to the function, rather than global */
char nowString[20];
static long DIE_MAGIC;
/****************************************************************************/

void archive(char *file, char *subject)
{

	/*
	 *  Archive will copy the specified file to the archive file for this
	 *  particular game.
	 */

	long now;
	char name[60];
	FILE *ofp, *ifp;

	sprintf(name, "D%s/archive", dipent.name);
	if (!(ofp = fopen(name, "a"))) {
		perror(name);
		fprintf(log_fp, "Unable to append to archive file for '%s'.\n", dipent.name);
		bailout(1);
	}
	if (!(ifp = fopen(file, "r"))) {
		perror(file);
		fprintf(log_fp, "Unable to archive %s to %s.\n", file, name);
		bailout(1);
	}
	time(&now);
	fprintf(ofp, "Date:    %24.24s (%ld)\n", ctime(&now), now);
	fprintf(ofp, "Subject: %s\n\n", subject);

	while (fgets(line, sizeof(line), ifp)) {
		if (strncmp(line, "::", 2) != 0)
			fputs(line, ofp);
	}

	fputc('\n', ofp);

	fclose(ifp);
	fclose(ofp);

}

/***********************************************************************/


int die(int n, int s)
{

	/*
	 *  Generate the results of "n" dice with "s" sides each.
	 */

	register int r = 0;

	while (n-- > 0)
		r += ((rand() >> 1) % s) + 1;
	return r;

}
/****** */
long get_die_magic(void)
{
	/*
	 * This function will see if a file called .magic.dat exists
	 * (assumed previosuly created). If so, it will set DIE_MAGIC to
	 * the value inside. If not, it will write a randon six-digit value
	 * to this file 
 	 */

    /* First, make sure value is generated only once */
    static int first_pass = 0;
    FILE *rand_ptr = NULL;
    int n = 0;

    if (!first_pass) { 

    first_pass = 1;

    rand_ptr = fopen("./.magic.dat", "r");
    if (rand_ptr) {
	n = fscanf(rand_ptr, "%ld", &DIE_MAGIC);
	fclose(rand_ptr);
    }

    if (!rand_ptr || n != 1) { 
    /* read errors, generate and write a random number */
        rand_ptr = fopen("./.magic.dat", "w");
	if (rand_ptr) {
	    DIE_MAGIC = rand() % 1000000;
	    n = fprintf(rand_ptr, "%6.6ld\n", DIE_MAGIC);
	    fclose(rand_ptr);
	}
	if (!rand_ptr || n != 6) {
		fprintf(log_fp, "Unable to write magic data to ./.magic.dat.\n");
                bailout(1);
	}
    }
  }
  return DIE_MAGIC;
}



	
/***********************************************************************/

void die_rolls(int seed)
{

	/*
	 *  Establish a starting seed for random number generation.  The seed 
	 *  is set to a predictable value to allow deterministic die rolls for 
	 *  replayability.  The "DIE_MAGIC" value is defined outside and can be 
	 *  any number as long as it is a constant and kept secret.  The purpose 
	 *  of it is so the players can't predict (by writing a little program 
	 *  at home) what the die rolls are going to be.
	 */

	char *s;
	extern char *rflg;

	for (s = rflg ? rflg : dipent.name; *s; s++)
		seed += *s;

	for (s = dipent.phase; *s; s++)
		seed += *s;

	srand(seed + get_die_magic());

}

/****************************************************************************/

int execute(char *command)
{

	/*
	 *  Execute the command and verify that it really happened -- a bug
	 *  somewhere was causing "system()" to return without actually
	 *  executing anything so outgoing mail got lost.  The executed command
	 *  must remove the "dip.xfail" file so that we know it did its thing.
	 */

	int i, status;
	FILE *fp;
	long now;

	for (i = 0;; i++) {
		time(&now);
		fprintf(log_fp, "%12.12s: Xcute: %s\n", ctime(&now) + 4, command);

		if (!(fp = fopen("dip.xfail", "w"))) {
			fprintf(log_fp, "Unable to create dip.xfail.\n");
			perror("dip.xfail");
			bailout(E_FATAL);
		}
		fclose(fp);

		if ((status = system(command))) {
			fprintf(log_fp, "System error %x executing: %s\n", status, command);
			if (i < 3) {
				fprintf(log_fp, "Will try again...\n");
				sleep(30);
				continue;
			} else {
				bailout(E_FATAL);
			}
		}
		break;
	}

	if ((fp = fopen("dip.xfail", "r"))) {
		fprintf(log_fp, "dip.xfail not deleted properly.\n");
		bailout(E_FATAL);
	}
	return 0;

}

/****************************************************************************/

void ferrck(FILE * fp, int n)
{

	/*
	 *  Check for an error status on an output file before renaming over
	 *  the top of the master copy of the file.  This prevents disk full
	 *  and such from destroying all our data.
	 */

	char *fmt;

	fmt = "Disk full error number %d.  Bailing out!\n";

	if (ferror(fp)) {
		fprintf(log_fp, fmt, n);
		fprintf(stderr, fmt, n);
		bailout(E_FATAL);
	}
}

/****************************************************************************/

char *get_prov(search_province, province_number, coast)
char *search_province;		/* The province to search for */
int *province_number;		/* OUTPUT: The number of the province */
int *coast;			/* OUTPUT: The specified coast */

{

	char *heap_walker;	/* Pointer to walk the province name heap */
	char *search_province_walker;	/* Pointer to walk the search province string */

	char *search_province_remains = search_province;
	/* The part of the search province which was not used */
	int heap_white_space;	/* Flag to indicate white space in the heap */
	int province_white_space;	/* Flag to indicate white space in the search province */


	/*
	 *  Scan the input line for a suitable province name.
	 *
	 *  Exit: province_number = 0 if invalid province, otherwise province ordinal
	 *        coast           = coast specification if present.
	 *        Return          = the part of the search province which was not used
	 */

	static char *words[] =
	{"0", "(north coast)", "(nc)", "/north coast", "/nc",
	 "(east coast)", "(ec)", "/east coast", "/ec",
	 "(west coast)", "(wc)", "/west coast", "/wc",
	 "(south coast)", "(sc)", "/south coast", "/sc"};

	static int coasts[] =
	{0, NC, NC, NC, NC, EC, EC, EC, EC,
	 WC, WC, WC, WC, SC, SC, SC, SC};

	/* Initialise the province number to 0 */

	*province_number = 0;

	/* Initialise the heap walker to the beginning of the heap */

	heap_walker = (char *) heap;

	/* While we have not found the end of the heap */

	while (*heap_walker) {
		/* Initialise the search province walker to the start of the search province string */

		search_province_walker = search_province;

		/* Initialise the white space flags */

		heap_white_space = 0;
		province_white_space = 0;

		/* Skip any spaces at the start of the search province */

		while (isspace(*search_province_walker))
			search_province_walker++;

		/* 
		 * While the characters in the search province and the heap match,
		 * and any spaces in the search province and the heap match up in
		 * locn, if not number or type, and the end of the heap is not
		 * found
		 */

		while (*heap_walker
		       && heap_white_space == province_white_space
		       && toupper(*search_province_walker) == toupper(*heap_walker)) {
			/* Reset the white space flags */

			heap_white_space = 0;
			province_white_space = 0;

			/* Skip any space characters or .'s in the search province */

			while (*++search_province_walker == '.' || isspace(*search_province_walker)) {
				/* Set the province white space flag */

				province_white_space = 1;
			}

			/* Skip and space characters or .'s in the heap */

			while (*++heap_walker == '.' || isspace(*heap_walker)) {
				/* Set the heap white space flag */

				heap_white_space = 1;
			}
		}

		/* If we stopped due to finding the end of the name in the heap, and at the time, the next
		   character in the search province was one of ( ) - / ; , \n or a space character */

		if (!*heap_walker && (!*search_province_walker || strchr("(-/);,\n", *search_province_walker) || isspace(*(search_province_walker - 1)))) {
			/* We have a match */

			/* If it is the best match so far (best = longest) */
			/*
			 * It does not matter that we have not yet considered coasts in
			 * search_province_walker, as if a coast was found in the best
			 * match so far, then it will always be a better match anyway
			 */

			if (search_province_walker > search_province_remains) {
				/* Read the province number out of the heap */

				*province_number = *((unsigned char *) ++heap_walker);

				/* Check if the next part of the search province is a coast, and if so, pick up which one */

				search_province_walker = lookfor(search_province_walker, words, nentry(words), coast);

				/* If there is a coast, decode its coast code */

				if (*coast)
					*coast = coasts[*coast];

				/* Store a pointer to the unused part of the search province string */

				search_province_remains = search_province_walker;
			}
		}
		/* Skip any NULs in the heap */

		while (*heap_walker++);

		/* Step on one more character */

		heap_walker++;
	}

	/* Return the part of the search string which was not used */

	return search_province_remains;
}

char *lookfor(char *l, char *w[], int len, int *n)
{
    return lookforv(l, w, len, n, 0);
}
char *lookforv(char *l, char *w[], int len, int *n, int exact_word)
{

	/*
	 *  Look for a phrase in a table of phrases. The phrases are lower case
	 *  with a single blank where blanks are allowed or an octothorp (#)
	 *  where blanks (or parenthesis) are required.
	 *
	 *  Entry: len = number of phrases
	 *  Exit:  n = index into phrases, or zero if not found.
	 */

	int i;
	char *t, *s;

	for (i = 1; i < len; i++) {
		t = w[i];
		s = l;
		while (isspace(*s))
			s++;
		while (*t && *t == tolower(*s)) {
			t++;
			s++;
			if (*t == ' ') {
				while (isspace(*s))
					s++;
				t++;
			} else if (*t == '#') {
				if (!isspace(*s) && !isdigit(*s) && !strchr("(;/,)", *s))
					break;
				while (isspace(*s))
					s++;
				t++;
			}
		}

                if (!*t && (!exact_word || ((!*s || isspace(*s) || strchr("(;/,)",*s))))) {
			*n = i;
			while (isspace(*s))
				s++;
			return s;
		}
	}
	*n = 0;
	return l;
}

int power(char c)
{

	/*
	 *  Return the ordinal of the specified power character.
	 */

	int i;

	if (islower(c))
		c = toupper(c);
	for (i = 1; i < MASTER + 1; i++)
		if (c == dipent.pl[i])
			return (i);

	return 0;
}

char *astype(char c, char cu)
{
	if (c == 'x')
		if (cu == 'A')
			return "an";
		else
			return "a";
	else if (c == 'c')
		return ("a citizen's militia");
	else if (c == 'm')
		return ("an elite mercenary");
	else if (c == 'p')
		return ("an elite professional");
	else
		return ("an unknown");
}

char *autype(char c)
{
	if (c == 'A')
		return ("an army");
	else if (c == 'F')
		return ("a fleet");
	else if (c == 'G')
		return ("a garrison");
	else if (c == 'W')
		return ("a wing");
	else if (c == 'T')
		return ("an army/fleet");
	else if (c == 'S')
		return ("a spy");
	else
		return ("a unit");
}

char *bstype(char c)
{
	if (c == 'x')
		return ("");
	else if (c == 'c')
		return ("citizen's militia ");
	else if (c == 'm')
		return ("elite mercenary ");
	else if (c == 'p')
		return ("elite professional ");
	else
		return ("unknown ");
}

char *Stype(char c)
{
	if (c == 'x')
		return ("");
	else if (c == 'c')
		return ("Citizen's Militia ");
	else if (c == 'm')
		return ("Elite Mercenary ");
	else if (c == 'p')
		return ("Elite Professional ");
	else
		return ("Unknown ");
}

char *utype(char c)
{
	if (c == 'A')
		return ("army");
	else if (c == 'F')
		return ("fleet");
	else if (c == 'G')
		return ("garrison");
	else if (c == 'W')
                return ("wing");
	else if (c == 'T')
		return ("army/fleet");
	else if (c == 'S')
		return ("spy");
	else
		return ("unit");
}

/****************************************************************************/

char *Utype(c)
char c;
{
	if (c == 'A')
		return ("Army");
	else if (c == 'F')
		return ("Fleet");
	else if (c == 'G')
		return ("Garrison");
	else if (c == 'W')
                return ("Wing");
	else if (c == 'T')
		return ("Army/Fleet");
	else if (c == 'S')
		return ("Spy");
	else
		return ("Unit");
}

/*
 ** mov_type - return a string proper for unit type/province type
 ** i.e. A Wing is 'over Spain' or 'over the Baltic'
 ** A fleet in 'in Spain' or 'in the Baltic'
 ** an army is 'in Spain' only
 */

char *mov_type( int prov_index, int unit_index)
{
	static char line[20];
	char prov_type; 
	char unit_type;

	line[0] = '\0';
	prov_type = pr[prov_index].type;
	unit_type = unit[unit_index].type;

	if (unit_type == 'W' )  {
		strcpy(line, "over");
		if (prov_type =='w' || prov_type == 'v' ) {
			strcat(line, " the");
		}
	} else {
		if (prov_type == 'w' || prov_type == 'v') {
			strcpy(line, "in the");
		} else {
			strcpy(line, "in");
		}
	}
	return line;
}
	
	
void wrap(FILE * fp, char *buf, int pos, int indent)
{

	char *s, *t;

	for (t = s = buf; *s; s++, pos++) {
		if (pos > 78) {
			while (*--s != ' ');
			*s = '\0';
			fputs(t, fp);
			fprintf(fp, "\n%*s", indent, "");
			*s++ = ' ';
			t = s;
			pos = indent;
		}
	}
	fputs(t, fp);

}

/*
 *  Capitalize a string.
 */

char *strcap(char *string)
{
	char *copy;
	int i;

	/*
	 * Allocate a string buffer.  It's the CALLER's responsibility to
	 * free() the string when he's finished with it.
	 */

	copy = (char *) malloc(strlen(string) + 1);

	/*
	 * Copy the original string to it, capitalizing all lower-case letters
	 * that (1) are the first character; (2) follow a whitespace character;
	 * or (3) follow punctuation.  DON'T use 'toupper()', as that changes
	 * the character in the original string.  
	 */

	if (copy) {
		for (i = 0; i < strlen(string); ++i) {
			if (islower(string[i]) && ((i == 0) || isspace(string[i - 1]) ||
					       ispunct(string[i - 1]))) {
				copy[i] = string[i] - 'a' + 'A';
			} else {
				copy[i] = string[i];
			}
		}
		copy[i] = '\0';
	}
	return copy;
}

char *NowString(void)
{
	long now;

	time(&now);

	sprintf(nowString, "%12.12s: ", ctime(&now) + 4);
	return (nowString);
}

int FindPower(int power_index)
{
	int i;
	for (i = 0; i < dipent.n; i++)
	{
		if (dipent.players[i].power == power_index) return i;
	}
	return i;
}


int IsBlank(char *text)
{
        char *t = text;
        int non_blank = 0;
        while (*t != '\0' && !non_blank) {
            if (0 == isspace(*t)) non_blank = 1;
            t++;
        }
        return !non_blank;
}

int GetOneWord(char *text)
{
        char *t = text;
        int non_blank = 0;
        while (*t != '\0') {
            if (non_blank == 1 && 0 != isspace(*t)) *t = '\0'; /*terminate at 1st non-blank*/
            if (0 == isspace(*t)) non_blank = 1;
            t++;
        }
        return non_blank;
}

int deadline_recursive( sequence *seq, int new, int *rec_count);

#define DIPENT_NO_PRESS ((dipent.flags & F_NOWHITE) && !(dipent.flags & F_GREY))
 
/****************************************************************************/
/* Look and see if any requested absences */
/* and adjust deadline parameter accordingly if so */

int absence_adjust(long *deadline)
{
        int ret = 0; /* Set to 1 if adjusted times */
        int i,j;

        for (i =0; i < dipent.n; i++ )
        {
                for ( j=0; j < MAX_ABSENCES; j++)
                {
                        if (dipent.players[i].absence_start[j] < *deadline ) {
                                if (dipent.players[i].absence_end[j] > *deadline ) {
				    /* Check if a player has a pending move and (no press or strictwait)
				       or game is quiet
				       or is the master before adjusting the deadline */
				    if ((dipent.players[i].status & SF_MOVE &&
 					(dipent.flags & F_STRWAIT || DIPENT_NO_PRESS)) || 
				        (!(dipent.flags & F_STRWAIT) && !DIPENT_NO_PRESS) ||
					dipent.flags & F_QUIET ||
					dipent.players[i].power == MASTER)
				    {
                                        /* OK, we're in a requested deadline period */
                                        /* so boot up the deadline accordingly      */
                                        *deadline = dipent.players[i].absence_end[j];
                                        ret = 1; /* Changed deadline */
				    }
                                }
                        }
                }
        }
        /* OK, now delete passed deadlines */
        for (i =0; i < dipent.n; i++ )
        {
                for ( j=0; j < MAX_ABSENCES; j++)
                {
                    if (dipent.players[i].absence_start[j] != 0L ) {

                        if (dipent.players[i].absence_start[j] <= *deadline ) {
                                if (dipent.players[i].absence_end[j] <= *deadline ) {
                                        /* OK, This deadline is no longer needed */
					/* but bump up total anyway */
					dipent.players[i].absence_total += 
					    (dipent.players[i].absence_end[j] - 
					    dipent.players[i].absence_end[j]);
                                        dipent.players[i].absence_start[j] = 0L;
                                        dipent.players[i].absence_end[j] = 0L;
                                        dipent.players[i].absence_count--;
                                }
                        }
                    }
                }
        }
        return ret;
}

/*************************************************************************/

int deadline(sequence *seq, int new)
{
        int rec_count = 0;
        return deadline_recursive(seq, new, &rec_count);
}


/****************************************************************************/


int deadline_recursive(sequence * seq, int new, int *rec_count)
{

/*
 *  Compute a new deadline for this dip entry.
 */

        int i, k;
        long now, temp;
        struct tm *tm, *localtime();
        int ret = 0;

        (*rec_count)++;
        if (*rec_count > 50) return 0; /* safety code */

        time(&now);
        if (dipent.phase[6] == 'X') {
                dipent.process = now + 168 * HRS2SECS;
                return 0;
        }
        if (!seq) {
                if (!(seq = dipent.phase[5] == 'M' ? &dipent.movement :
                      dipent.phase[5] == 'R' ? &dipent.retreat :
                      dipent.phase[5] == 'B' ? &dipent.builds : NULL)) {
                        fprintf(stderr, "Invalid phase [%s] in deadline for '%s'.\n",
                                dipent.phase, dipent.name);
                        fprintf(log_fp, "Invalid phase [%s] in deadline for '%s'.\n",
                                dipent.phase, dipent.name);
                        return E_FATAL;
                }
        }
        /*
           **  If the new flag is indicated, we assume a move has just completed
           **  and we establish the next deadline.  We don't want to set the new
           **  deadline earlier than the old one though.
         */

        if (new) {
                temp = now + (int) (seq->next * HRS2SECS);

                if (temp < dipent.deadline)
                        temp = dipent.deadline;
                /* OK, adjust the time requested according to pending absences */
                ret = absence_adjust(&temp);

                if (seq->clock >= 0) {
                        tm = localtime(&temp);
                        i = seq->clock * 60 - ((tm->tm_hour * 60 + tm->tm_min) * 60 + tm->tm_sec);
                        if (i < 0)
                                i += 24 * 60 * 60;
                        temp += i;
                }
                for (k = 0; k < 8; k++) {
                        tm = localtime(&temp);
                        if (seq->days[tm->tm_wday] == '-')
                                temp += 24 * 60 * 60;
                        else if (islower(seq->days[tm->tm_wday]) && tm->tm_hour < 12)
                                temp += (12 - tm->tm_hour) * 60 * 60 - tm->tm_min * 60 - tm->tm_sec;
                        else
                                break;
                }

                /* If old deadline was huge, keep it */
		dipent.deadline = max(temp,  dipent.deadline) ;
                dipent.process = temp;
                temp += (int) (seq->grace * HRS2SECS);
                if (dipent.flags & F_GRACEDAYS) {
                        for (k = 0; k < 8; k++) {
                                tm = localtime(&temp);
                                if (seq->days[tm->tm_wday] == '-')
                                        temp += 24 * 60 * 60;
                                else if (islower(seq->days[tm->tm_wday]) && tm->tm_hour < 12)
                                        temp += (12 - tm->tm_hour) * 60 * 60 - tm->tm_min * 60 - tm->tm_sec;
                                else
                                        break;
                        }
                }
		/* If old grace was huge, keep it */
                dipent.grace =  max(temp,  dipent.grace);
                dipent.start = now + (int) (seq->mint * HRS2SECS);
        }
        /*
           **  Figure out if we can bump up the process time.
         */

        temp = now + (int) (seq->delay * HRS2SECS);
        for (i = 0; i < dipent.n; i++) {
                if (dipent.players[i].power < 0)
                        continue;

                if (dipent.players[i].status & SF_PROCESS) {
                        temp = now - 1 * HRS2SECS;
                        dipent.players[i].status &= ~SF_PROCESS;
                }
                if (dipent.players[i].status & SF_WAIT && now < dipent.deadline) {
                        temp = dipent.deadline;
                        break;
                }
                if (WAITING(dipent.players[i].status)) {
                        temp = now < dipent.deadline ? dipent.deadline :
                            now > dipent.grace && dipent.flags & F_NONMR ? dipent.process :
                            dipent.grace;
                        break;
                }
        }

        /*
         *  We don't want to advance the process time beyond the deadline
         *  here.  That will be done in 'process' when a reminder is sent
         *  out to those who haven't gotten their orders in yet.
         */

        if ((dipent.process < dipent.deadline && temp < dipent.deadline) ||
            temp < dipent.process)
                dipent.process = max(dipent.start, temp);

        if (ret == 1) {
                /* We have adjusted the deadline due to an absence */
                /* Recursively call to re-adjust if necessary */
                deadline_recursive(seq, new, rec_count);
                /* set the mailing flag so we can advise an absence adjust */
                broadcast_absence_adjust = 1;
        }
        return 0;

}


/***** */
/* This following function will remove all leading 
 * spaces and convert double (or more) spaces to only one
 */

int despace(char *intext)
{
	char temp[350];
	int space_count = 0;
	char *s = intext;
	char *outtext = temp;

	/* First take away leading spaces */
	while (isspace(*s)) s++;

	while (*s) {
	    if (isspace(*s)) {
		space_count++;
		if (space_count == 1) *outtext++ = ' ';
	    }
	    else {
		*outtext++ = *s;
		space_count = 0;
	    }
	    s++;
	}
	*outtext = '\0'; /* terminate the string */
	strcpy(intext,temp);
	return strlen(temp);
}


/***** */
/* This function will display the absences for 
 * a particular power (passsed as a parameter)
 * Normal powers will see their absences only
 * Master will see all absences
 * Observer will see none
 */

void display_one_absences(int pindex, FILE *fptr)
{
	int i = pindex;
	int k;

        if (dipent.players[i].absence_count > 0) {
           for (k = 0; k < MAX_ABSENCES;k++) {
                if (dipent.players[i].absence_start[k] != 0) {
                    fprintf(fptr, "%s: Absence requested from \n%s to ",
                         powers[dipent.players[i].power], 
			 ptime(&dipent.players[i].absence_start[k]));
                    fprintf(fptr,"%s\n", ptime(&dipent.players[i].absence_end[k]));
                }
            }
            fprintf(rfp,"\n");
        }
        return;
}
void display_absence(int pnum, FILE *fptr)
{

	int i;

	switch (pnum) {

	    case OBSERVER:
	    case WILD_PLAYER:
		/* Do nothing */
		break;   

	    case MASTER:
		for (i = 0; i < MAXPLAYERS; i++ ) {
		    display_one_absences(i, fptr);
		}
		break;

	    default:
		    display_one_absences(FindPower(pnum), fptr);
	}
	return;
}

/***********************************************************************/


void AddOrderToOrder(char *text, int order)
{
    if (text == NULL) return;

    switch (order)
    {
	case 'a':
	    strcat(text,"AIRLIFT ");
            break;
	case 'c':
	    strcat(text,"CONVOY ");
	    break;
	case 'm':
	    strcat(text, "-> ");
	    break;
	case 't':
	    strcat(text,"TRANSFORM ");
	    break;
	case 'p':
	    strcat(text, "PROXY TO ");
	    break;
	case 'd':
	    strcat(text, "DISBANDS ");
	    break;
	case 'h':
	    strcat(text, "HOLD ");
	    break;
	case 's':
	    strcat(text, "SUPPORT ");
	    break;
	case 'r':
	    strcat(text, "REMOVE ");
	    break;
	case 'b':
	    strcat(text,"BUILD ");
	    break;
	case 'w':
	    strcat(text, "WAIVE ");
	    break;
	case 'x':
	    break;
	default:
	   strcat(text,"*unknown order*");
    }
}

void AddCoastToOrder(char *text, int c1)
{
    char temp[50];
    if (text == NULL) return;
    sprintf(temp, "(%s) ", mtype[c1]);
    strcat(text, temp);
}

void AddUnitToOrder(char *text, int unit)
{
    if (text == NULL) return;
    strcat(text, Utype(unit));
    strcat(text," ");
}

void AddPlaceToOrder(char *text, int place)
{
    if (text == NULL) return;
    if (place <= 0) return;
    strcat(text, pr[place].name);
    strcat(text," ");
}
void AddProvinceToOrder(char *text, int place)
{
	return AddPlaceToOrder(text,place);
}
void AddUnitProvinceToOrder(char *text, int unit, int place)
{
	AddUnitToOrder(text, unit);
	AddPlaceToOrder(text,place);
}
void AddPowerToOrder(char *text, int power)
{
	if (text == NULL) return;
	strcat(text,powers[power]);
}
