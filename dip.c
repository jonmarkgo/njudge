/*  dip.c
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
 *  Portions of this file are public domain by nathan wagner.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "config.h"
#include "dip.h"
#include "mail.h"
#include "conf.h"
#include "functions.h"
#include "diplog.h"
#include "plyrdata.h"

#define		DIP_INIT_ERROR		dip_init_error_quark()

#define pcontrol if (!(dipent.flags & (F_NOLIST|F_QUIET))) control
#define pprintf  if (!(dipent.flags & (F_NOLIST|F_QUIET))) fprintf

/* Comment the next line out if you wish the Judge to apply D_LATE right
   at the deadline rather than 24 hours later. */
/*#define NORMDED */

void CheckRemindPlayer(int player, long one_quarter);
void inform_party_of_blind_turn(int player_index, char *turn_text, char*);
static gint init(int argc, char** argv, GError** err);
gint parse_cmdline(gint argc, gchar** argv, GPtrArray** cl_cfg, GError** err);
void phase_pending(void);	/* defined in phase.c */
void print_usage(void);
static gint terminate(void);

static char* judge_tz = NULL; /* holds the TZ environment variable */
extern int time_warp;  /* Set to 1 if a time-warp was detected */
struct opts_s options = {0};
static FILE* lock_file = NULL;

static GQuark dip_init_error_quark(void) {

	return g_quark_from_static_string("dip-init-error-quark");

}

int main(int argc, char** argv) {

	gint    exit_s = 0;
	gchar*  tcptr;
	gchar*  syslog_alias = NULL;
    struct  stat buf;
    GError* err = NULL;

	if (!init(argc, argv, &err)) {
		if (err && err->code == DIP_INIT_ERROR_HAS_CRASHED) {
			bailout(err->code);
		}
		goto exit_main;
	}

 	syslog_alias = g_strdup_printf("%s-%s", conf_get("judge_code"), "dip");

	diplog_syslog_open(syslog_alias);
	diplog_syslog_entry(LOG_INFO, "Started dip");

	/* Check if xforward file exists, indicating a bailout-recovery situation */
	if (!stat(XFORWARD, &buf)) {
	    bailout_recovery = 1;
	}

	if(open_plyrdata() != 0) {
		diplog_entry("unable to open plyrdata file.");
	}
	put_data(0, total);
	
	control = ded[0].d0;

	if (!options.variant) {
		if (!options.no_input) {
			diplog_syslog_entry(LOG_DEBUG, "Processing mail");
			mail();	/* Process mail message on stdin  */
		}
		if (!options.quick) {
			diplog_syslog_entry(LOG_DEBUG, "Looking for events");
			master();	/* Process any events pending     */
		}
	} else {
		rfp = stdout;
		options.debug++;
		testdipent(options.sequence, options.variant);
		process();
	}

	if (control > 1000) {
		control = 0;
	}

	ded[0].d0 = control;

	if (nded > 0) {
		int fd;
		fd = open("dip.ded", O_RDWR | O_CREAT, 0666);
		write(fd, ded, nded * sizeof(ded[0]));
		remove("dip.dedicate");
		close(fd);
	}
	close_plyrdata();

	/* If block file exists, remove it */
	tcptr = conf_get("block_file");
	if (*tcptr) {
	    remove(tcptr);
	}

exit_main:

	if (err) {
		if (!diplog_syslog_entry(LOG_ERR, "%s", err->message)) {
			fprintf(stderr, "e> %s\n", err->message);
			exit_s = err->code;
		}
		g_error_free(err);
	}

	diplog_syslog_entry(LOG_INFO, "Ended dip");
	diplog_syslog_close();

	if (syslog_alias) g_free(syslog_alias);

	terminate();

	return exit_s;

}
void inform_party_of_blind_turn( int player_index, char *turn_text, char *in_file) {

	/*
	 * This function will tell all players that a blind turn has happened
	 * The text will be varied depending on who is being told
	 */

	char line[150];

/* ito be made configureable */
    char *out_file = "dip.rreply";

    sprintf(line, "%s %s %s %s -i=%s -o=%s", "./zpblind",
			powers[dipent.players[player_index].power],
			owners[dipent.players[player_index].power],
			dipent.x2flags & X2F_BLIND_CENTRES ? "" : "-v=HideAll",
            in_file,
            out_file);

	system(line);

	if (!(dipent.players[player_index].status & SF_RESIGN)) {
		sprintf(line, "%s '%s:%s - %s Blind Results'",
			out_file, conf_get("judge_code"), dipent.name, turn_text);
		if (*(dipent.players[player_index].address) != '*' && !options.debug) {
			MailOut(line, dipent.players[player_index].address);
		}
	}

}
void gotalarm(int unused) {

	diplog_entry("Acquire runtime lock timed-out.");

	savemail();

	bailout(0);

}
void savemail(void) {

	/*
	 *  We've run into a problem.  Save our mail input.
	 */

	FILE *fp;
	char line[1024];
	time_t now;

	if (!(fp = fopen(conf_get("mail_spooler"), "a"))) {
		perror(conf_get("mail_spooler"));
		exit(E_FATAL);
	}
	if (lockfd(fileno(fp), 0)) {
		perror("FLOCKF1");
		exit(E_FATAL);
	}
	while (fgets(line, (int) sizeof(line), options.input)) {
		fputs(line, fp);
	}

	time(&now);

	diplog_entry("message saved on %s.", conf_get("mail_spooler"));

	return;

}
static gint init(int argc, char** argv, GError** err) {

	gint       fd;
	gint       idx;
	gint       rtn = 0;
	gint*      dedic;
	gchar*     tcptr;		// Temp char pointer
	FILE*      block_fp;
	time_t     now;
	GPtrArray* cl_cfg = NULL;
	struct stat sbuf;

	g_assert(err != NULL && *err == NULL);

	g_set_prgname(g_basename(argv[0]));

	/* Reset dipent */
	memset(&dipent, 0, sizeof dipent);

	/* Set default config file */
	CONFIG_DIR  = g_memdup(".\0dip.conf", 11);
	CONFIG_FILE = CONFIG_DIR + 2;

	/* Set options defaults */
	options.input = stdin;

	/* Reset the random seed */
	time(&now);
	srand(now);

	/* Setup config values */
	if (!conf_init())
		goto exit_init;
	if (!parse_cmdline(argc, argv, &cl_cfg, err))
		goto exit_init;
	if (!conf_read_file(CONFIG_DIR, CONFIG_FILE, err))
		goto exit_init;
	if (cl_cfg) { // Parse values stashed away while reading command line
		idx = conf_vset(cl_cfg, err);
		g_ptr_array_unref(cl_cfg);
		if (!idx) goto exit_init;
	}

	/* Change cwd to judge_path */
	if (chdir(conf_get("judge_path")) < 0) {
		g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_VOID_DIR,
				"judge_dir error - %s: %s", conf_get("judge_dir"), g_strerror(errno));
		goto exit_init;
	}

	/* Open log session */
	if (!diplog_open("dip", err)) {
		goto exit_init;
	}

	/* Change the judge timezone, if set */
	if (*(tcptr = conf_get("judge_tz"))) {
		judge_tz = g_strdup_printf("TZ=%s", tcptr);
	    putenv(judge_tz);
	    tzset();
	}

	subjectline[0] = '\0';

	if (!stat(conf_get("bail_forward"), &sbuf)) {
		savemail();
		g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_HAS_BAILED,
				"has bailed error, saved mail");
		goto exit_init;
	}

	/*
	 * Acquire file lock on lock_file to ensure single process
	 * TODO: Change to POSIX sigaction
	 */

	if ((lock_file = fopen((tcptr = conf_get("lock_file")), "w")) == NULL) {
		g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_LOCK_FILE_FAIL,
				"unable to open lock file: %s", tcptr);
		goto exit_init;
	}

	signal(SIGALRM, gotalarm);
	alarm(conf_get_int("lock_timeout"));

	lockfd(fileno(lock_file), 0);

	alarm(0);

	/* OK, now see if we're supposed to use block file */
	if (*(tcptr = conf_get("block_file"))) {
	    if (!stat(tcptr, &sbuf)) {
	    	g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_HAS_CRASHED,
	    			"block file exists - %s: previous dip has crashed", tcptr);
	    	goto exit_init;
	    }
	    if ((block_fp = fopen(tcptr,"w")) == NULL) {
	    	g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_BLOCK,
	    			"unable to create block file - %s: %s", tcptr, g_strerror(errno));
	    	goto exit_init;
	    }
	    time(&now);
	    fprintf(block_fp, ctime(&now));
	    fclose(block_fp);
	}

	time(&now);
	diplog_entry("options: %c%c%c.", options.dont_rm_q ? 'A' : ' ',
			options.quick ? 'q' : ' ', options.no_input ? 'x' : ' ');

	if ((fd = open("dip.ded", O_RDONLY)) < 0)
		if ((fd = open("dip.dedicate", O_RDONLY)) < 0)
			nded = 0;
		else {
			dedic = g_alloca(MAXUSER);
			nded = read(fd, dedic, MAXUSER * sizeof(int)) / sizeof(int);
			for (idx = 0; idx <= nded; idx ++) {
				ded[idx].r = dedic[idx];
			}
			close(fd);
	} else {
		nded = read(fd, ded, MAXUSER * sizeof(ded[0])) / sizeof(ded[0]);
		close(fd);
	}

	if (!(cfp = fopen("dip.control", "a"))) {
		g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_CONTROL_FILE,
				"unable to open dip.control: %s", g_strerror(errno));
		goto exit_init;
	}
	if (!(xfp = fopen("dip.xcontrol", "a"))) {
		g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_XCONTROL_FILE,
				"unable to open dip.xcontrol: %s", g_strerror(errno));
		goto exit_init;
	}

	rtn = 1;

exit_init:

	return rtn;

}
gint terminate(void) {

	if (judge_tz != NULL) g_free(judge_tz);
	if (lock_file != NULL) fclose(lock_file);

	return 1;

}
void master(void) {

/*  Process the master file.  Perform any duties and set next time to look. */

	char *s, line[150];
	time_t now, next_time;
	FILE* ifp;
	FILE* ofp;
	FILE* msg_file;
	struct stat sbuf;
    int one_quarter = conf_get_int("reminder_time") * 60 * 60; /* Set to 8 hours */
	int i;

#define TWO_HOURS 2*60*60

	if ((ifp = fopen(conf_get("master_db"), "r")) == NULL) {
		perror(conf_get("master_db"));
		printf("Unable to open master file %s.\n", conf_get("master_db"));
		exit(E_FATAL);
	}
	if ((ofp = fopen(conf_get("master_db_tmp"), "w")) == NULL) {
		printf("Unable to open new master file temp.master.\n");
		exit(E_FATAL);
	}
	time(&now);
	next_time = now + 365 * 24 * 60 * 60;
	while (getdipent(ifp)) {

                if (now < dipent.deadline) {
                    if (one_quarter >= TWO_HOURS && (now > (dipent.deadline - one_quarter))) {
                        for ( i = 0; i < dipent.n; i++) {
				if (dipent.phase[6] != 'X' ) 
	                                CheckRemindPlayer(i, dipent.deadline - now);
                        }
                    }
                }

		if (time_warp) {
			/* A global time-warp was detected: inform all found masters of this */
			msg_file = fopen(conf_get("warp_file"), "w");
			if (msg_file != NULL) {
			    fprintf(msg_file, "A time warp was detected on the %s judge. Please check that",conf_get("judge_code"));
			    fprintf(msg_file, " the deadline for %s is correct, and inform players accordingly.\n",dipent.name);
			    fclose(msg_file);

			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0) continue;
				if (dipent.players[i].power == MASTER &&
						!(dipent.players[i].status & SF_RESIGN)) {
					sprintf(line,
							"%s '%s:%s - %s Diplomacy time-warp'",
							conf_get("warp_file"), conf_get("judge_code"), dipent.name,
							 dipent.phase);
				MailOut(line, dipent.players[i].address);
			  }
  
			    }
			}
		}

		if (bailout_recovery) {
		    /* Judge is recovering from a bailout: inform all found masters of this */
		    msg_file = fopen(conf_get("warp_file"), "w");
		    if (msg_file != NULL) {
				fprintf(msg_file, "The %s judge is recovering from a bailout. Please check that", conf_get("judge_code"));
				fprintf(msg_file, " the deadline for %s is correct, and inform players accordingly.\n",dipent.name);
				fclose(msg_file);

				for (i = 0; i < dipent.n; i++)  {
					if (dipent.players[i].power < 0) continue;
					if (dipent.players[i].power == MASTER &&
							!(dipent.players[i].status & SF_RESIGN)) {
						sprintf(line, "%s '%s:%s - %s Bailout recovery'",
								conf_get("warp_file"), conf_get("judge_code"), dipent.name, dipent.phase);
						MailOut(line, dipent.players[i].address);
				}
			}
		 }
	}

		if (dipent.process <= now) {
			if (dipent.phase[6] == 'X' || dipent.n <= 0) {
				if ((dipent.flags & (F_GUNBOAT | F_NOREVEAL)) == (F_GUNBOAT | F_NOREVEAL)) {
					dipent.process = now + 365 * 24 * 60 * 60;
				} else {
					continue;
				}
			} else if (GAME_PAUSED && (dipent.wait < now)) {
			    msg_file = fopen(conf_get("warp_file"), "w");
                            if (msg_file != NULL) {
                                fprintf(msg_file, 
				"Game is paused - master must send the 'resume' command to resume it or 'terminate' to finish.\n\n");
                                fclose(msg_file);

                               for (i = 0; i < dipent.n; i++) {
                                if (dipent.players[i].power < 0)
                                    continue;
				if (dipent.players[i].controlling_power != 0) continue;
                                if (!(dipent.players[i].status & SF_RESIGN)) {
                                    sprintf(line,
                                        "%s 'Game pause reminder: %s'",
                                        conf_get("warp_file"), dipent.name);
				    MailOut(line, dipent.players[i].address);
				}
                              }
			      dipent.process = now + 3 * 24 * 60 * 60; /* Remind every 3 days that game is paused */
			      dipent.wait = dipent.process;
			    }

        		} else {
				process();
			}
		}
		if (next_time > dipent.process)
			next_time = dipent.process;

		putdipent(ofp, 1);
	}
	fclose(ifp);
	ferrck(ofp, 1001);
	fclose(ofp);
	if (rename(conf_get("master_db_tmp"), conf_get("master_db"))) {
		diplog_entry("Error renaming %s to %s.\n", conf_get("master_db_tmp"), conf_get("master_db"));
		fprintf(stderr, "Error renaming %s to %s.\n", conf_get("master_db_tmp"),conf_get("master_db"));
		bailout(E_FATAL);
	}
	/*
	   **  Compute the next time and throw ourselves onto the AT queue.
	 */

	next_time += 60;	/* Round up to next minute */
	time(&now);
	if (now + (15 * 60) > next_time)
		next_time = now + (15 * 60);
	s = ctime(&next_time);
	if (!strncmp(s + 11, "14:00", 5))
		s[15] = '1';	/* Prevent 2pm schedules */
	sprintf(line, "%s %s %2.2s%2.2s %6.6s %4.4s", conf_get("cmd_atrun"),
		options.dont_rm_q ? "norm" : "dorm", s + 11, s + 14, s + 4, s + 20);
	if (!options.dont_touch_q)
		execute(line);

	/*
	   **  Process any info on the control file.
	 */

	if (control < 1000)
		fclose(cfp);

        if (control && control < 1000 && ded[0].md < now) {
                stat("dip.control", &sbuf);
                if (sbuf.st_size > 0) {
                        s = notifies;
                        while (*s && *s != '*') {
                                if (*s == '+')
                                        s++;
                                sprintf(line,
                                        "%s dip.control 'Diplomacy control information' '%s'",
                                        conf_get("cmd_smail"), s);
                                execute(line);
                                while (*s++);
                        }
                }
                remove("dip.control");
                ded[0].md = now + conf_get_int("min_control");
        }
        fclose(xfp);
        stat("dip.xcontrol", &sbuf);
        if (sbuf.st_size > 0) {
                s = notifies;
                while (*s && *s != '*') {
                        if (*s == '+') {
                                s++;
                                sprintf(line,
                                        "%s dip.xcontrol 'Diplomacy xcontrol information' '%s'",
                                        conf_get("cmd_smail"), s);
                                execute(line);
                        }
                        if (control >= 1000)
                                break;
                        while (*s++);
                }
        }
        remove("dip.xcontrol");
        control = 0;
}

/* See if the passed player need to make a move, 
   and send reminder message if not yet made */
void CheckRemindPlayer(int player, long one_quarter) {
    int num_hours;
    char *temp_file = "dip.temp";
    char line[150];
    char *pchar;
    time_t now;

    if(dipent.phase[6] == 'X') return; 
    if (!WAITING(dipent.players[player].status) ) return; /* Not waiting for a move */

    if (dipent.players[player].status & SF_REMIND) return; /* Already been reminded */

    if (dipent.players[player].controlling_power != 0) return; /* only players */

    num_hours = (int)  (one_quarter / (60 * 60));
    num_hours++;
    if (num_hours != 1) 
	pchar = "s ";
    else
	pchar = " ";

    if ((rfp = fopen(temp_file, "w")) == NULL) {
	 perror(temp_file);
         exit(E_FATAL);
    }

	msg_header(rfp);

/*
 *	code added to fix bug where error messages
 *	for other players are displayed if there have been
 *	no intervening signons
 */

	sprintf(Tfile, "%s/%s/T%s", conf_get("game_dir"), dipent.name, dipent.seq);
	if ((ofp = fopen(Tfile, "w")) == NULL) {
		fprintf(rfp, "Error opening %s to write orders.\n", Tfile);
	}
	sprintf(Mfile, "%s/%s/M%s", conf_get("game_dir"), dipent.name, dipent.seq);
	if ((tfp = fopen(Mfile, "r")) != NULL) {
		while (fgets(line, sizeof(line), tfp)) {
			if (!strcmp(line, "X-marker\n"))
				*line = 'Y';
			fputs(line, ofp);
		}
		fclose(tfp);
	}

	fputs("X-marker\n", ofp);
	fclose(ofp);

	if (rename(Tfile, Mfile)) {
		fprintf(rfp, "Error renaming %s to %s.\n", Tfile, Mfile);
	}

/*
 * end added code
 */

	porder('M', player, 0); /* Call this to send copy of player's actual orders */
	fprintf(rfp,"\n\nThere is less than %d hour%sto the deadline", num_hours, pchar);

	if ((dipent.players[player].status) &  SF_PART) {
		fprintf(rfp, " but you\nneed to clear your error status with a valid message.\n");
	} else {
		fprintf(rfp," and you\nhave not yet placed complete and valid moves.\n");
	}

	fprintf(rfp,"\nPlease do so before you are marked as late.\n\n");
	now=time(NULL);

        if(dipent.phase[6] != 'X')
        {
		if(now < dipent.deadline)
	   		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
		if(now < dipent.grace)
			fprintf(rfp, "Time to grace period expiration: %s.\n",timeleft(&dipent.grace));
	}
	fclose(rfp);
	msg_header_done = 0;  /* Bug 282, header will need to be redone */

	sprintf(line, "%s '%s:%s - %s Reminder'",
	  temp_file, conf_get("judge_code"), dipent.name, dipent.phase);

	dipent.players[player].status |= SF_REMIND;

	if (*(dipent.players[player].address) != '*' &&
	    !(dipent.players[player].status & SF_RESIGN)) {
		MailOut(line, dipent.players[player].address);
		sprintf(line,"Move reminder sent to %s in game %s", dipent.players[player].address, dipent.name);
		diplog_syslog_entry(LOG_INFO, line);
	}
}
int process(void) {

	int i, n;
/*	int latecnt = 0;*/
	int dedtest; /* Buffer variable for dipent.dedapplied. */
	time_t now, then;
	FILE *dfp, *gfp,
	*lfp, *mlfp; /* late file pointers */
	char line[150];
	char pppath[70];
	char ppline[200];
	char title_text[150];
	char phase[sizeof(dipent.phase)];
	static char *dedfmt = "Adding %d to user %d's dedication to yield %d.\n";
	char late[51];
	struct stat ppinfo;
	int one_printed = 0;
	int vic, vic_count;
	static char vic_string[MAXPLAYERS+1];
	char temps[2];
	int just_now_abandoned = 0;  /* Set to non-zero if at least one power hsa just abandoned */

	if (GAME_PAUSED) return 0;  /* game is paused, do not process! */

	diplog_entry("processing game '%s'.", dipent.name);
	rfp = NULL;
	lfp = NULL;
	mlfp = NULL;
	time(&now);

	/* Set dedtest equal to dipent.dedapplied. This keeps dipent.dedapplied
	** from being updated until the Judge finishes processing the game.
	** At that point dipent.dedapplied will be set to dedtest.
	*/
	dedtest = dipent.dedapplied;


	if (now < dipent.grace) {
		for (n = 0, i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;
			if ((dipent.flags & F_INTIMATE) && dipent.players[i].controlling_power != 0)
				continue;  /* Don't warn about non players in intimate */

			if (now < dipent.deadline && (dipent.players[RealPlayerIndex(i)].status & SF_WAIT)) {
				dipent.process = dipent.deadline;
				return 0;
			}
			if (WAITING(dipent.players[RealPlayerIndex(i)].status)) {
				if (now < dipent.deadline) {
					dipent.process = dipent.deadline;
					return 0;
				}
				dipent.process = dipent.grace;
				while ((then = dipent.process - 24 * 60 * 60) > now)
					dipent.process = then;

				if ((dipent.players[RealPlayerIndex(i)].status & SF_PART) &&
				  now >= dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60)) {
					dipent.players[RealPlayerIndex(i)].status |= SF_MOVED;
				} else {
					if (!n++) {
						if (options.debug) {
							lfp = stdout;
							mlfp = stdout;
						}
						else {
							if (!(lfp = fopen("dip.late", "w"))) {
								perror("dip_process: dip.late");
								bailout(1);
							} else {
								msg_header(lfp);

								if (dipent.x2flags & X2F_SECRET) {
									fprintf(lfp, "Diplomacy game '%s' is waiting for orders from one or more powers",
										dipent.name);
									one_printed++;
								}
							}

							if (!(mlfp = fopen("dip.mlate", "w"))) {
								perror("dip_process: dip.mlate");
								bailout(1);
							} else {
								msg_header(mlfp);
							}
						}
					}

					if (!(dipent.x2flags & X2F_SECRET)) {
						fprintf(lfp, "Diplomacy game '%s' is waiting for %s's orders",
							dipent.name, dipent.flags & F_QUIET ? "some power"
							: powers[dipent.players[i].power]);
						one_printed++;
					}

					fprintf(mlfp, "Diplomacy game '%s' is waiting for %s's orders",
						dipent.name, powers[dipent.players[i].power]);


					late[n-1] = (dipent.flags & F_QUIET ? '?' : dipent.pl[dipent.players[i].power]);

					if (!(dipent.players[RealPlayerIndex(i)].status & SF_LATE)) {
					    dipent.players[i].status |= SF_LATE;
					    dipent.players[i].late_count++; /* bump up the late count */
					}
					if (dipent.xflags & XF_LATECOUNT) {
						if (!(dipent.x2flags & X2F_SECRET))
							fprintf(lfp, ": %d time%s late",
								dipent.players[RealPlayerIndex(i)].late_count,
								dipent.players[RealPlayerIndex(i)].late_count == 1 ? "" : "s" );
						fprintf(mlfp, ": %d time%s late",
							dipent.players[RealPlayerIndex(i)].late_count,
							dipent.players[RealPlayerIndex(i)].late_count == 1 ? "" : "s" );
					}
					if (one_printed) {
					    fprintf(lfp,".\n");
					    one_printed = 0;
					}
					fprintf(mlfp,".\n");
				}
			}
		}

		if (n) {
			late[n] = '\0';
			then = dipent.grace - (dipent.flags & F_NONMR ? 0 : 24 * 60 * 60);

			if (dipent.x2flags & X2F_SECRET) {
				fputs("\nThese powers", lfp);
				fputs(now < then ? " will be" : " are now", lfp);
			} else {
				fputs(n == 1 ? "\nThis power" : "\nThese powers", lfp);
				fputs(now < then ? " will be" : n == 1 ? " is now" : " are now", lfp);
			}

			fputs(" considered abandoned and free for takeover", lfp);

			fputs(n == 1 ? "\nThis power" : "\nThese powers", mlfp);
			fputs(now < then ? " will be" :
			      n == 1 ? " is now" : " are now", mlfp);
			fputs(" considered abandoned and free for takeover", mlfp);

			if (now >= then) {
				if (dipent.flags & F_NONMR) {
					fputs(".\n", lfp);
					fputs(".\n", mlfp);
				} else {
					if (dipent.x2flags & X2F_SECRET) {
						fputs(".\nThey", lfp);
					} else {
						fputs(n == 1 ? ".\nIt" : ".\nThey", lfp);
					}

					fputs(" will be considered in civil disorder if orders are ", lfp);
					fprintf(lfp, "not received\nby %s.\n", ptime(&dipent.grace));

					fputs(n == 1 ? ".\nIt" : ".\nThey", mlfp);
					fputs(" will be considered in civil disorder if orders are ", mlfp);
					fprintf(mlfp, "not received\nby %s.\n", ptime(&dipent.grace));
				}
				n = -1;
			} else {
				fprintf(lfp, "\nif orders are not received by %s.\n", ptime(&then));
				fprintf(mlfp, "\nif orders are not received by %s.\n", ptime(&then));
				n = now < dipent.deadline + 24 * 60 * 60 ? 1 : 0;
			}


			if (!options.debug) {
				fclose(lfp);
				fclose(mlfp);
				msg_header_done = 0;
			}

			for (i = 0; i < dipent.n; i++) {
				register int d, w;

				if (dipent.players[i].power < 0)
					continue;

				if ((w = WAITING(dipent.players[RealPlayerIndex(i)].status)) || n) {
					if (w && n == -1) {
						if (!(dipent.players[i].status & SF_ABAND)) {
							dipent.players[i].status |= SF_ABAND;
							just_now_abandoned++;
							if (!(dipent.flags & F_NORATE) &&
							    dipent.players[i].controlling_power == 0) {
								ded[dipent.players[i].userid].r += conf_get_int("points_abandon");
								put_data(dipent.players[i].userid,resigned);
								diplog_entry(dedfmt, conf_get_int("points_abandon"), dipent.players[i].userid,
										ded[dipent.players[i].userid].r);
							}
						}
						pprintf(cfp, "%s%s in game '%s' is now considered abandoned ",
							NowString(), powers[dipent.players[i].power], dipent.name);
						pprintf(cfp, "(%s, %d of %d units).\n", dipent.phase,
							dipent.players[i].units, dipent.players[i].centers);
						pcontrol++;
					}
					if (dipent.players[i].controlling_power == 0) {
#ifdef NORMDED
					    if (!(dipent.flags & F_NORATE)) {
						d = n == 1 ?
						    (!w && (dipent.players[i].status & SF_MOVE) ? D_ONTIME : 0)
						    : (w ? D_LATE : 0);
						ded[dipent.players[i].userid].r += d;
						fprintf(log_fp, dedfmt, d, dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					    }

					    if(n == 1 && dipent.dedapplied == 0 && (!(dipent.flags & F_NORATE)))
					    {
						if(w)
						{
							put_data(dipent.players[i].userid,total);
						}
						else
						{
							put_data(dipent.players[i].userid,ontime);
							put_data(dipent.players[i].userid,total);
						}
						dedtest = 1;
					    }
#else
					    d = 0;
					    if(dipent.dedapplied == 0 && n == 1)
					    {
						if(dipent.players[i].status & SF_MOVE)
						{
							if(w)
							{
								d = conf_get_int("points_late");
								put_data(dipent.players[i].userid,total);
							}
							else
							{
								d = conf_get_int("points_ontime");
								put_data(dipent.players[i].userid,ontime);
								put_data(dipent.players[i].userid,total);
							}
							dedtest = 1;
						}
						ded[dipent.players[i].userid].r += d;
						diplog_entry(dedfmt, d,dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					    }
					}
#endif
					if (*(dipent.players[i].address) != '*' &&
					    !(dipent.players[i].status & SF_RESIGN)) {
						if (dipent.players[i].power == MASTER) {
							sprintf(line, "dip.mlate '%s:%s - %s Late Notice: %s'",
									conf_get("judge_code"), dipent.name, dipent.phase, late);
							MailOut(line, dipent.players[i].address);
						} else {
							sprintf(line, "dip.late '%s%s:%s - %s Late Notice: %s'",
								 w ? "[You are late!] " : "", conf_get("judge_code"), dipent.name, dipent.phase,
									(dipent.x2flags & X2F_SECRET) ? "?" : late);
							MailOut(line, dipent.players[i].address);
						}
					}
				}
			}
			if (n) {
				sprintf(line, "%s:%s - %s Late Notice: %s", conf_get("judge_code"), dipent.name, dipent.phase, (dipent.x2flags & X2F_SECRET) ? "?" : late);
				archive("dip.late", line);
			}
			if (n == -1) {
				pprintf(cfp, "%sGrace period for '%s' expires %s.\n",
					NowString(), dipent.name, ptime(&dipent.grace));
			}
			dipent.dedapplied = dedtest;
			return 0;
		}
	}
	if (dipent.seq[0] == 'x') {
		if (options.debug)
			rfp = stdout;
		else if (!(rfp = fopen("dip.result", "w"))) {
			perror("dip_process: dip.result");
			bailout(1);
		} else {
			msg_header(rfp);
		}
		i = dipent.powers + '0' - dipent.seq[1];
		if ((dipent.xflags & XF_MANUALSTART) && i <= 0) {
			sprintf(subjectline, "%s:%s - Waiting for Master to Start", conf_get("judge_code"), dipent.name);
			fprintf(rfp, "Diplomacy game '%s' is still waiting for master to start it.\n", dipent.name );
			pprintf(cfp, "%sDiplomacy game '%s' is still waiting for master to start it.\n", NowString(), dipent.name);
			sprintf(title_text, "Diplomacy game %s startup waiting", dipent.name);
		} else {
			sprintf(subjectline, "%s:%s - Waiting for More Players", conf_get("judge_code"), dipent.name);
			if (dipent.x2flags & X2F_SECRET) {
			    sprintf(subjectline, "%s:%s - Waiting for More Player(s)", conf_get("judge_code"), dipent.name);
			    fprintf(rfp, "Diplomacy game '%s' is still waiting for some player(s) to sign on.\n", dipent.name);
			} else {	
			    sprintf(subjectline, "%s:%s - Waiting for %d More Player%s", conf_get("judge_code"), dipent.name, i, i == 1 ? "" : "s");
			    fprintf(rfp, "Diplomacy game '%s' is still waiting for %d player%s to sign on.\n", dipent.name, i, i == 1 ? "" : "s");
			}
			pprintf(cfp, "%sDiplomacy game '%s' is still waiting for %d player%s to sign on.\n", NowString(), dipent.name, i, i == 1 ? "" : "s");
			sprintf(title_text, "Diplomacy game %s signup waiting", dipent.name);
		}
		if (!options.debug)
		{
			now=time(NULL);   
			if(dipent.phase[6] != 'X')
			{
                		if(now < dipent.deadline)
                        		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                		if(now < dipent.grace)
                 	       		fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
			}
			fclose(rfp);
		        msg_header_done = 0;  /* Bug 282, header will need to be redone */

		}
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

			if (*(dipent.players[i].address) != '*' &&
			    !(dipent.players[i].status & SF_RESIGN) &&
			     RealPlayerIndex(i) == i) {
				sprintf(line, "dip.result '%s'", subjectline);
				MailOut(line, dipent.players[i].address);
			}
		}
		dipent.process = now + 168 * 60 * 60;

	} else if (now > dipent.process) {

		if (options.debug)
			rfp = stdout;
		else if (!(rfp = fopen("dip.result", "w"))) {
			perror("dip_process: dip.result");
			bailout(1);
		} else {
			msg_header(rfp);
		}

		if (now >= dipent.grace && dipent.grace > dipent.deadline) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;
		/* Dietmar Kulsch change 10/10/2000 to avoid players going
		 * abandoned when they submit error orders
		 */
				if ((!(dipent.players[RealPlayerIndex(i)].status & SF_PART)) && WAITING(dipent.players[RealPlayerIndex(i)].status)) {
					dipent.players[i].status |= SF_CD;
					just_now_abandoned++;
					if (!(dipent.flags & F_NORATE) &&
					    RealPlayerIndex(i) == i) {
						put_data(dipent.players[i].userid,resigned);
						ded[dipent.players[i].userid].r += conf_get_int("points_cd");
						diplog_entry(dedfmt, conf_get_int("points_cd"), dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
					}
					if (!(dipent.flags & F_NONMR)) {
						pprintf(cfp, "%s%s in game '%s' is now considered in civil disorder ",
							NowString(), powers[dipent.players[i].power], dipent.name);
						pprintf(cfp, "(%s, %d of %d units).\n", dipent.phase,
							dipent.players[i].units, dipent.players[i].centers);
						pcontrol++;
					}
				}
			}
		}
		if (!(dipent.flags & F_NORATE) && now < dipent.deadline + 60 * 60 && dipent.dedapplied == 0) {
			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[i].status & SF_MOVE) &&
				    !(dipent.players[i].status & SF_CD) &&
				    RealPlayerIndex(i) == i) {
					put_data(dipent.players[i].userid,ontime);
					put_data(dipent.players[i].userid,total);
					ded[dipent.players[i].userid].r += conf_get_int("points_ontime");
					diplog_entry(dedfmt, conf_get_int("points_ontime"), dipent.players[i].userid,
							ded[dipent.players[i].userid].r);
				}
			}
		}
		if (dipent.flags & F_NONMR) {
			for (n = i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if ((dipent.players[RealPlayerIndex(i)].status &
				     (SF_MOVE | SF_MOVED | SF_PART)) == SF_MOVE) {
				    /* If intimate, only show real players abandoned */
				    if (!((dipent.flags & F_INTIMATE) && (dipent.players[i].controlling_power != 0))) {
					if (!(dipent.x2flags & X2F_SECRET)) {
						fprintf(rfp, "Diplomacy game '%s' is waiting for someone ", dipent.name);
						fprintf(rfp, "to take over the abandoned %s.\n",
						  dipent.flags & F_QUIET ? "power" :
						  powers[dipent.players[i].power]);
					}
					pprintf(cfp, "%sDiplomacy game '%s' is waiting for someone ", NowString(), dipent.name);
					pprintf(cfp, "to take over the abandoned\n");
					pprintf(cfp, "%s with %d of %d units on the board (%s).\n",
						powers[dipent.players[i].power], dipent.players[i].units,
						dipent.players[i].centers, dipent.phase);

					if (!(dipent.x2flags & X2F_SECRET) || n == 0) {
					    late[n] = ((dipent.flags & F_QUIET) ||
					    		(dipent.x2flags & X2F_SECRET) ? '?' : dipent.pl[dipent.players[i].power]);
					    n++;
					}
				    }
				}
			}

			/* Only send warning if process time passed */
			if (n && dipent.process < now ) {

			    dipent.process = now + 48 * 60 * 60;

			    /* Only send warnings if two days since last one */
			    /* or if at least one player has just abandoned */
			    if (dipent.wait < now || just_now_abandoned) {

				late[n] = '\0';

				if (dipent.x2flags & X2F_SECRET) {
					fprintf(rfp, "Diplomacy game '%s' is waiting for someone ", dipent.name);
					fprintf(rfp, "to take over the abandoned power(s).\n");
				}

				dipent.wait = dipent.process;

				if (!options.debug)
				{
					if(signedon && (dipent.phase[6] != 'X'))
					{
						now=time(NULL);   
			                	if(now < dipent.deadline)
                        				fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                				if(now < dipent.grace)
                        				fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
					}
					fclose(rfp);
				        msg_header_done = 0;  /* Bug 282, header will need to be redone */

				}
				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;

					if (*(dipent.players[RealPlayerIndex(i)].address) != '*' && !options.debug &&
					    !(dipent.players[RealPlayerIndex(i)].status & SF_RESIGN) &&
					    RealPlayerIndex(i) == i) {
						sprintf(line, "dip.result '%s:%s - %s %s Waiting for Replacements: %s'",
								conf_get("judge_code"), dipent.name, dipent.phase,
							(dipent.flags & F_NOLIST) ? "NoList" : "",
							(dipent.x2flags & X2F_SECRET) ? "?" : late);
						MailOut(line, dipent.players[i].address);
					}
				}
			    }
			    dipent.dedapplied = dedtest;
			    return 0; /* Bug 347, always return even if no warnings are sent! */
			}
		}
		/*
		 * See if manual processing is enabled, and block if so
         */
		if (dipent.xflags & XF_MANUALPROC) {
			if (!(dipent.players[0].status & SF_PROCESS)) {
				fprintf(rfp, "Game '%s' is waiting for master to process turn.\n", dipent.name);
				if (!options.debug) {
				    fclose (rfp);
			            msg_header_done = 0;  /* Bug 282, header will need to be redone */
				}

				for (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;
					sprintf(line, "dip.result '%s:%s - %s Turn Waiting'",
							conf_get("judge_code"), dipent.name, dipent.phase);

					if (*(dipent.players[RealPlayerIndex(i)].address) != '*' &&
					    !(dipent.players[RealPlayerIndex(i)].status & SF_RESIGN) &&
					      RealPlayerIndex(i) == i)
						MailOut(line, dipent.players[RealPlayerIndex(i)].address);
				}
				dipent.process = now + 24 *60 *60;  /* Remind each day */
				dipent.dedapplied = dedtest;
				return 0;
			}
		}

		/*
		   **  This is something we can actually process!
		 */

		strcpy(phase, dipent.phase);

		if ((i = porder('M', -1, 0))) {
			fprintf(rfp, "Error %d processing orders.\n", i);
			fprintf(stderr, "Error %d processing orders.\n", i);
			if (!options.debug) {
				fclose(rfp);
        			msg_header_done = 0;  /* Bug 282, header will need to be redone */
			}
			sprintf(line, "dip.result 'Diplomacy error'");
			MailOut(line, conf_get("judge_keeper"));
			bailout(1);
		}

		pprintf(cfp, "%sPhase %s processed for game '%s'.\n", NowString(), phase, dipent.name);

		/*
		   **  Remove any draw/win information (for summary) if it exists
		 */

		sprintf(line, "%s/%s/draw", conf_get("game_dir"), dipent.name);
		remove(line);

		if (dipent.phase[6] == 'X') {
			sprintf(line, "%s/%s/draw", conf_get("game_dir"), dipent.name);
			if ((dfp = fopen(line, "w")) == NULL) {
				diplog_entry("error opening draw file.");
				bailout(E_FATAL);
			}
			if ((ofp = fopen("dip.temp", "w")) == NULL) {
				diplog_entry("error opening second temporary file.");
				bailout(E_FATAL);
			}
			if ((gfp = fopen("dip.victory", "w")) == NULL) {
				diplog_entry("error opening victory file.");
				bailout(E_FATAL);
			}

			msg_header(ofp);

			pprintf(cfp, "%sGame '%s' completed.\n", NowString(), dipent.name);
			fprintf(rfp, "\nThe game is over.  Thank you for playing.\n");
			fprintf(ofp, "Game '%s' has ended in a victory for ", dipent.name);
			time(&now);
			fprintf(dfp, "Game won: %s\n", ctime(&now));
			fprintf(dfp, "The game was won by ");
			fprintf(gfp, "The game was won by ");

			/*  Find out who has won */

		
			vic_string[0] = '\0';
			for (vic_count = 0; vic_count < victor; vic_count++)
			{
			    vic = victors[vic_count];
			    if (vic_count > 0 && vic_count + 1 < victor ) {
		                fprintf(ofp, ", ");
                                fprintf(dfp, ", ");
                                fprintf(gfp, ", ");
			    } else if ( vic_count > 0 && vic_count + 1 >= victor) {
				fprintf(ofp, " and ");
				fprintf(dfp, " and ");
	                        fprintf(gfp, " and ");
			    }
			    fprintf(ofp, "%s", powers[vic]);
			    fprintf(dfp, "%s", powers[vic]);
			    fprintf(gfp, "%s", powers[vic]);
			    sprintf(temps, "%c", dipent.pl[vic]);
			    strcat(vic_string, temps);
			}
			fprintf(ofp, ".\n\n");
                        fprintf(dfp, ".\n");
                        fprintf(gfp, ".\n");


			fprintf(gfp, "Congratulations on a well deserved victory!\n");

			/*  If game is standard, it has a Boardman Number, if not
			   it has a Miller Number.  Check to see if it is an EP
			   game.  Alert Custodians to victory */
			fclose(ofp);
			msg_header_done = 0;
			fclose(dfp);
			fclose(gfp);

			for (i = 0; i < dipent.n; i++) {
				if (dipent.players[i].power < 0)
					continue;

				if (*(dipent.players[i].address) != '*' && !options.debug &&
				    RealPlayerIndex(i) == i ) {
					sprintf(line, "dip.victory '%s:%s - %s Victory: %s'",
							conf_get("judge_code"), dipent.name, phase, vic_string);
					MailOut(line, dipent.players[i].address);
				}
			}

			{
				InformCustodians(dipent.name,
						 "%s '%s: Victory in game %s'",
						 dipent.variant, 
						 dipent.flags & F_GUNBOAT);
			}

			/*
			 * This seems as good a place as any to send out EOG diaries
			 */
			send_diary();

			/*
			 * Force regeneration of the summary file if it's a gunboat game.
			 */

			if (dipent.flags & F_GUNBOAT) {
				sprintf(line, "%s/%s/msummary", conf_get("game_dir"), dipent.name);
				remove(line);
			}
			/*  This code, ripped from mail.c, around line 1000, should force
			   a new summary to be created nicely */
			{
				char *mflg, *gflg;
				gflg = ((dipent.flags & F_GUNBOAT) && (dipent.phase[6] != 'X' ||
				  (dipent.flags & F_NOREVEAL))) ? "g" : "";
				mflg = (*gflg && dipent.players[player].power == MASTER)
				    ? "m" : "";
				sprintf(line, "%s -C %s -%s%s%slv%d %s", conf_get("cmd_summary"), CONFIG_DIR, mflg, gflg,
					dipent.flags & F_QUIET ? "q" : "", dipent.variant, dipent.name);
				system(line);
			}

			/*  Mail summary to HALL_KEEPER */

			sprintf(line, "%s/%s/summary 'HoF: Victory in %s'",
					conf_get("game_dir"), dipent.name, dipent.name);
			MailOut(line, conf_get("hall_keeper"));

		} else {
			deadline((sequence *) NULL, 1);

			fprintf(rfp, "\nThe next phase of '%s' will be %s for %s of %4.4s.\n",
				dipent.name,
				dipent.phase[5] == 'M' ? "Movement" :
				dipent.phase[5] == 'R' ? "Retreats" : 
				dipent.phase[5] == 'A' ? "Bids" : "Adjustments",
				dipent.phase[5] == 'A' ? "Year" :
				dipent.phase[5] == 'B' ? "Winter" :
				dipent.phase[0] == 'F' ? "Fall" :
				dipent.phase[0] == 'U' ? "Summer" : "Spring", dipent.phase + 1);
			if (broadcast_absence_adjust)
			{
			    fprintf(rfp,"Requested absence(s) activated.\n");
			    broadcast_absence_adjust = 0;
			}
			fprintf(rfp, "The deadline for orders will be %s.\n",
				ptime(&dipent.deadline));
			if ((dipent.phase[5] == 'B' && 
				 (dipent.x2flags & X2F_MORE_HOMES)) ||
				 (dipent.phase[5] == 'A')) {
			    /* game allows home centre assignments */
			    /* Thus set active players in a wait state */
			    for  (i = 0; i < dipent.n; i++) {
					if (dipent.players[i].power < 0)
						continue;
					if (dipent.players[RealPlayerIndex(i)].power < WILD_PLAYER &&
					  !(dipent.players[RealPlayerIndex(i)].status & SF_DEAD) &&
					    dipent.players[RealPlayerIndex(i)].controlling_power == 0) {
					/* A real player, set wait status */
						dipent.players[RealPlayerIndex(i)].status |= SF_WAIT;
					}
			    }
			}
		}

		if (!options.debug)
		{
			if(signedon && (dipent.phase[6] != 'X'))
			{
				now=time(NULL);   
                		if(now < dipent.deadline)
                        		fprintf(rfp, "\nTime to deadline: %s.\n", timeleft(&dipent.deadline));
                		if(now < dipent.grace)
		                        fprintf(rfp, "Time to grace period expiration: %s.\n", timeleft(&dipent.grace));
			}
			fclose(rfp);
		        msg_header_done = 0;  /* Bug 282, header will need to be redone */

		}
		for (i = 0; i < dipent.n; i++) {
			if (dipent.players[i].power < 0)
				continue;

/*
 * This looks as good a place as any to clear draw flags...
 *    (P�ositron, 11 Mar 1993)
 * And concession flags (Tim Miller 12 Aug 2001)
 */
			dipent.players[i].status &= ~SF_CONC;
			dipent.players[i].status &= ~SF_DRAW;

			if (!(dipent.flags & F_BLIND) || (dipent.players[i].power == MASTER)) 
			{
				sprintf(line, "dip.result '%s:%s - %s Results'",
						conf_get("judge_code"), dipent.name, phase);
			}

		        /* let's do postal press */
			/* TODO: tidy this code up */ 
			sprintf(pppath, "%s/%s/ppress-%s", conf_get("game_dir"), dipent.name, phase);
		        if((dipent.x2flags & X2F_POSTALPRESS) && (stat(pppath, &ppinfo) != -1))
	    		    sprintf(ppline, "%s/%s/ppress-%s '%s:%s - %s game press'",
	    		    		conf_get("game_dir"), dipent.name, phase,
	        		conf_get("judge_code"), dipent.name, phase);

			if (*(dipent.players[i].address) != '*' && !options.debug &&
					    !(dipent.players[i].status & SF_RESIGN) &&
					    RealPlayerIndex(i) == i) 
			{
			    if(!(dipent.flags & F_BLIND) || dipent.players[i].power == MASTER)
				MailOut(line, dipent.players[i].address);
			    if((dipent.x2flags & X2F_POSTALPRESS) && (stat(pppath, &ppinfo) != -1))
				MailOut(ppline, dipent.players[i].address);
			}

			if (dipent.flags & F_BLIND) {
			    /* Ooops, it's a blind variant */
			    /* Special routine to work out what to tell who */
			    inform_party_of_blind_turn(i, phase,"dip.result");
			}
		}

		if (!strcmp(dipent.seq, "002") && !(dipent.flags & F_BLIND)) {
			sprintf(line, "dip.result 'Diplomacy results %s %s'",
				dipent.name, phase);
			MailOut(line, conf_get("games_opener"));
		}
		sprintf(line, "%s:%s - %s Results", conf_get("judge_code"), dipent.name, phase);
		archive("dip.result", line);

		phase_pending();
		deadline((sequence *) NULL, 0);
	}
	
	return 0;		/* reached ? */
}
gint parse_cmdline(gint argc, gchar** argv, GPtrArray** cl_cfg, GError** err) {

	gint   rtn = 0;
	gchar* tcptr;
	gchar  opt;

	opterr = 0; /* mute getopt */

	while ((opt = getopt(argc, argv, ":AaC:c:Dd:hi:qs:T:t:vx")) > -1) {

		switch(opt) {
			case 'A':
				options.dont_rm_q = 1;
				break;
			case 'a':
				options.dont_touch_q = 1;
				break;
			case 'C':
				options.cfg_file = optarg;
				/*
				 * Code below are for compatibility reasons only.
				 * TODO: Remove ASAP...
				 */
				CONFIG_DIR = g_strdup(optarg);
				/* Disabled since rdip uses this to forward config dir and no file name is given
				 * *(tcptr = strrchr(CONFIG_DIR, PATH_SEP)) = 0;
				 * CONFIG_FILE = ++tcptr;
				 */
				break;
			case 'c':
				/*
				 * We stash these values for later processing, since we'll need
				 * to read the config file first; complying to overriding rules
				 */
				if (!*cl_cfg) *cl_cfg = g_ptr_array_new_with_free_func(NULL);
				g_ptr_array_add(*cl_cfg, optarg);
				break;
			case 'D':
				options.debug = 1;
				break;
			case 'd':
				conf_set("judge_path", optarg, 0, NULL);
				break;
			case 'i':
				options.input = fopen(optarg, "r");
				if (NULL == options.input) {
					g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_OPEN_STREAM,
							"e> couldn't open input stream - %s: %s", optarg, g_strerror(errno));
					goto exit_parse_cmdline;
				}
				break;
			case 'h':
				print_usage();
				goto exit_parse_cmdline;
				break;
			case 'q':
				options.quick = 1;
				break;
			case 'r':
				options.randomizer = optarg;
				break;
			case 's':
				if (!sscanf(optarg, "%u", &options.sequence)) {
					g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_ARG_TYPE,
							"option -%c requires an integer argument", opt);
					goto exit_parse_cmdline;
				}
				if (!options.variant) {
					options.variant = V_STANDARD;
				}
				break;
			case 'T':
				options.datetime = optarg;
				if (mail_date(&options.datetime, &dip_time, 1, stderr, 0)) {
					g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_ARG_TYPE,
							"invalid date-time specified - %s", optarg);
				    goto exit_parse_cmdline;
				}
				break;
			case 't':
				if (!sscanf(optarg, "%u", &options.variant)) {
					g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_ARG_TYPE,
							"option -%c requires an integer argument", opt);
					goto exit_parse_cmdline;
				}
				if (!options.sequence) {
					options.sequence = 1;
				}
				break;
			case 'v':
				options.verbose ++;
				break;
			case 'x':
				options.no_input = 1;
				break;
			case ':':
				g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_MISSING_ARG,
						"option requires an argument: -%c", optopt);
				goto exit_parse_cmdline;
			case '?':
				g_set_error(err, DIP_INIT_ERROR, DIP_INIT_ERROR_VOID_OPTION,
						"unknown option: -%c", optopt);
				goto exit_parse_cmdline;
			default:
				goto exit_parse_cmdline;
		}

	}

	rtn = 1;

exit_parse_cmdline:

	return rtn;

}
void print_usage(void) {

	fprintf(stderr, "Usage: [/directory/]%s [-C <directory>] [-c<CONFIG>=<value>] [-d<date>] "
			"[-aADqvx] [-sseq] [-tvar] [-d directory] [-i<filename>] [-r name]\n", g_get_prgname());
	fprintf(stderr, "  The directory specifies where we'll find our data.\n");
	fprintf(stderr, "  -a Don't mess with the at queue.\n");
	fprintf(stderr, "  -A Don't remove anything from the at queue.\n");
	fprintf(stderr, "  -C <sudirectory> Directory where dip.conf is.\n");
	fprintf(stderr, "  -c<CONFIG>=<value> Set <CONFIG> variable to <value>.\n");
	fprintf(stderr, "  -D increments the debugging flag.\n");
	fprintf(stderr, "  -d change current directory.\n");
	fprintf(stderr, "  -i Use <filename> for input.\n");
	fprintf(stderr, "  -q quick mode, just process mail.\n");
	fprintf(stderr, "  -r Original name for randomizer (Machiavelli).\n");
	fprintf(stderr, "  -T Used <date-time> string as current date & time.\n");
	fprintf(stderr, "  -t force variant var.\n");
	fprintf(stderr, "  -v Verbose, issue all error messages.\n");
	fprintf(stderr, "  -x no input, don't read stdin for mail.\n");
	fprintf(stderr, "  -s specifies test mode and sequence.\n");
	fprintf(stderr, "  -t specifies test mode and variant.\n");

}
