/*
 * $Log$
 * Revision 1.4  2003/06/29 21:37:43  nzmb
 * Made EOG draw entries broadcasted at the end of the game.
 *
 * Revision 1.3  2003/06/22 04:10:23  nzmb
 * Added code to allow users to record diary entries, read them, and delete them
 * if they make a mistake. The diaries will be broadcast when the games end (this
 * part is not as of now, yet written).
 *
 * Revision 1.2  2002/07/17 11:48:11  millis
 * Added Log to header
 *
 */

/*
 * tm_xpress.c -- Tim Miller's extended press functionality. Will include
 * EOG diarys and szine style press published at deadline.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "dip.h"
#include "mail.h"
#include "functions.h"

void new_diary_entry(void);
void list_entries(void);
void read_entry(int entry);
void delete_entry(int entry);
int  get_numentries(char *gamename, char pabbr);

int process_ppress(void)
{
	char pfilename[1000];
	char b;
	
	/* see if this is even allowed */
	if(!(dipent.x2flags & X2F_POSTALPRESS))
	{
		fprintf(rfp,"Postal press is not allowed. Skipping:\n");
		ppress_skip = 1;
		return 1;
	}

	/* this is cool -- open up the press file */
	sprintf(pfilename,"%s%s/ppress-%s", GAME_DIR, dipent.name, dipent.phase);
	if((ppfp = fopen(pfilename, "a")) == NULL)
	{
		fprintf(log_fp, "Error opening ppress file %s.\n", pfilename);
		bailout(E_FATAL);
	}

	ppress_read = 1;
	broadcast_master_only = 1;
	b = dipent.pl[dipent.players[player].power];
	fprintf(mbfp,"%s as %c in %s submitted press:\n", raddr,
		b, dipent.name);
	fprintf(ppfp,"\nPress from %c:\n", b);
	fprintf(rfp, "\nAppending to press file:\n");
	sprintf(subjectline, "%s:%s - %s postal press recorded", JUDGE_CODE,
		dipent.name, dipent.phase);
	return 0;
}

void process_diary(char *cmd)
{
	char *ch, *tmp;
	int read = 0;	 /* read a specified entry */
	int delete = 0;  /* delete a specified entry */
	int entry = -1;	 /* number of entry to perform action on */

	ch = strchr(cmd,'\n');
	if(ch)
		*ch = '\0'; /* remove any newline */

	ch = strchr(cmd, ' ');
	if(ch)
	{
		tmp = ch + 1;
		*ch = '\0';  /* make the first arg its own null terminated string */
	}

	if(!strcasecmp(cmd, "record"))
	{
		/* record a new diary entry */
		new_diary_entry();
		broadcast_master_only = 1;
		sprintf(subjectline, "%s:%s - %s New diary entry from %c",
			JUDGE_CODE, dipent.name, dipent.phase,
			dipent.pl[dipent.players[player].power]);
		return;
	}
	if(!strcasecmp(cmd, "list"))
	{
		/* list previously made entries */
		list_entries();
		return;
	}
	if(!strcasecmp(cmd, "read"))
		read++;
	else if(!strcasecmp(cmd, "delete"))
		delete++;
	else {
		fprintf(rfp,"Bad diary option %s.\n",cmd);
		fprintf(rfp,"Skipping following message...\n");
		diary_skip++;
		return;
	}

	if(!ch)
	{
		fprintf(rfp, "You must specify an entry number to %s.\n\n", cmd);
		return;
	}

	entry = (int)strtol(tmp,NULL,10);
	if(entry < 0)
	{
		fprintf(rfp,"Bad entry %s.\n", tmp);
		return;
	}

	if(read)
		read_entry(entry);
	if(delete)
		delete_entry(entry);
}

void new_diary_entry(void)
{
	int next_entry;
	char fname[512];
	char pabbr = dipent.pl[dipent.players[player].power];

	next_entry = get_numentries(dipent.name, pabbr);
	if(next_entry < 0)
	{
		fprintf(rfp,"Error: could not open new diary entry.\n\n");
		diary_skip++;
		return;
	}

	/* determine the new entry file & open it */
	sprintf(fname,"%s%s/diary-%c-%d", GAME_DIR, dipent.name,
		pabbr, next_entry);
	diaryfp = fopen(fname, "w");
	if(!diaryfp)
	{
		fprintf(log_fp, "Could not open diary file %s!\n", fname);
		fflush(log_fp);
		bailout(E_FATAL);
	}

	fprintf(rfp, "Recording new diary entry #%d...\n\n", next_entry);
	fprintf(diaryfp, "Diary entry from %s in %s for phase %s:\n\n\n",
		powers[dipent.players[player].power], dipent.name,
		dipent.phase);
	diary_read = 1;
}

void read_entry(int entry)
{
	char entryfname[512];
	char line[1000];
	FILE *diary_read;

	sprintf(entryfname,"%s%s/diary-%c-%d", GAME_DIR, dipent.name,
		dipent.pl[dipent.players[player].power], entry);
	diary_read = fopen(entryfname, "r");
	if(!diary_read)
	{
		fprintf(rfp, "Cannot open diary entry #%d -- ", entry);
		fprintf(rfp, "perhaps said entry doesn't exist?\n\n");
		return;
	}

	fprintf(rfp, "Reading entry %d ... \n\n", entry);
	while(fgets(line, sizeof line, diary_read))
		fprintf(rfp,"%s", line);
	fprintf(rfp, "\n\nEnd of entry\n\n");
}

void delete_entry(int entry)
{
	char pabbr, cmd[512];

	pabbr = dipent.pl[dipent.players[player].power];
	if(entry < 0 || entry >= get_numentries(dipent.name, pabbr))
	{
		fprintf(rfp,"Cannot delete diary entry %d -- no such entry.\n\n", entry);
		return;
	}

	sprintf(cmd,"rm -f %s%s/diary-%c-%d", GAME_DIR, dipent.name,
		pabbr, entry);
	system(cmd);
	fprintf(rfp, "Diary entry %d removed.\n\n", entry);
}

void list_entries(void)
{
	int nentries, i;
	char pabbr;
	struct stat statbuf;
	struct tm *mod_tm;
	char fname[1000];
	char *months[] = {"Non", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; 

	pabbr = dipent.pl[dipent.players[player].power];
	nentries = get_numentries(dipent.name, pabbr);
	if(nentries < 0)
	{
		fprintf(rfp,"Error fetching number of entries!\n");
	} else if(nentries == 0) {
		fprintf(rfp,"You haven't made any diary entries!\n");
	} else {
		fprintf(rfp,"\nExisting diary entries:\n\n");
		for(i = 0; i < nentries; i++)
		{
			sprintf(fname,"%s%s/diary-%c-%d", GAME_DIR,
				dipent.name, pabbr, i);
			if(stat(fname, &statbuf) != -1)
			{
				mod_tm = localtime(&(statbuf.st_mtime));
				fprintf(rfp, "Entry #%d: modified %s %d, %d %02d:%02d\n",
					i, months[mod_tm->tm_mon + 1],
					mod_tm->tm_mday, 1900 + mod_tm->tm_year,
					mod_tm->tm_hour, mod_tm->tm_min); 
			}
			else
			{
				nentries++;
			}
		}
		fprintf(rfp,"\n\n");
	}
}

int get_numentries(char *gamename, char pabbr)
{
	char command[1000];
	char nstring[256];
	FILE *cmdpipe;
	int nentries = -1;

	sprintf(command,"ls %s%s/diary-%c* | wc -w",
		GAME_DIR, gamename, pabbr);
	cmdpipe = popen(command, "r");
	if(cmdpipe)
	{
		fread(nstring, 1, 255, cmdpipe);
		pclose(cmdpipe);
	
		if(strstr(nstring, "No such file"))
			nentries = 0;
		else
			nentries = (int)strtol(nstring, NULL, 10);
	}

	return nentries;
}

void send_diary(void)
{
	/*
	 * send each diary out at the end of the game.
	 */
	int i, j, k, ndiaries;
	char subjectln[256];
	char diary_fl[256];
	char line[1000];
	char pabbr;
	struct stat sbuf;

	for(i = 0; i < dipent.n; i++)
	{
		/*
		 * cycle through all the players who may have
		 * left diaries
		 */
		if(dipent.players[i].power < 0)
			continue;
		pabbr = dipent.pl[dipent.players[i].power];
		ndiaries = get_numentries(dipent.name, pabbr);
		if(ndiaries < 0)
			/* error don't send anything */
			ndiaries = 0;

		for(j = 0; j < ndiaries; j++)
		{
			/* send all diaries */
			sprintf(subjectln, "NZMB:%s diary #%d from %c",
				dipent.name, j, pabbr);
			sprintf(diary_fl, "%s%s/diary-%c-%d", GAME_DIR,
				dipent.name, pabbr, j);
			if(stat(diary_fl, &sbuf) == -1)
			{
				ndiaries++;
				continue;
			}

			for(k = 0; k < dipent.n; k++)
			{
				/* to everyone */
				if(dipent.players[k].power < 0)
					continue;
				sprintf(line, "%s %s \"%s\" %s", SMAIL_CMD,
					diary_fl, subjectln, dipent.players[k].address);
				execute(line);
			}
		}
	}
}
