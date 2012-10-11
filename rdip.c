/*
 * $Log: rdip.c,v $
 * Revision 1.9  2005-09-14 11:54:20  millis
 * Bug 436, fix gcc 4.0 compile errors.
 *
 * Revision 1.8  2004/06/17 06:12:58  nzmb
 * Made rdip handle an early death of the dip program much more gracefully.
 * Instead of bailing out, it assumes the judge couldn't process the message
 * and skips it.
 *
 * Revision 1.6  2003/09/14 22:08:19  jaldhar
 * Included fcntl.h for symbols not defined in unistd.h
 *
 * Revision 1.5  2003/09/09 18:51:33  jaldhar
 * Got rid of port.h and replaced with some extra configure checks.  The
 * include strings.h was not carried over because it is commented out and
 * likely wouldn't work anyway.
 *
 * Revision 1.4  2003/04/08 09:58:31  millis
 * Fix bug 131 (problems with dip.lock2 lock)
 *
 * Revision 1.3  2003/03/25 23:35:33  millis
 * Comment out "Exit" line, as meses up on exim installations
 *
 * Revision 1.2  2000/11/14 14:27:37  miller
 * Passing of config_dir setting to dip, use of syslogging
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "conf.h"
#include "config.h"
#include "functions.h"
#include "diplog.h"

/* 
 *  The purpose of rdip is to read in mail and copy it to a file for 
 *  later processing, it is invoked by incoming mail.  The flow is: 
 *
 *    1) Obtain a lock on the "dip.lock1" file.
 *    2) Append mail to dip.mail (if no -x flag).
 *    3) If dip.input exists (attempted create), exit -- another rdip running.
 *    4) Release sendmail (if -b flag).
 *    5) Obtain a lock on the "dip.lock2" file.
 *    6) Rename dip.mail to dip.input.
 *    7) Release interlock on dip.lock1.
 *    8) For each mail message on dip.input invoke "dip -q". 
 *    9) Reobtain the interlock on dip.lock1.
 *   10) If dip.mail exists loop back up to step 6.
 *   11) Release interlock on dip.lock1. 
 *   12) Invoke "dip -x".
 *   13) Reobtain the interlock on dip.lock1.
 *   14) If dip.mail exists loop back up to step 6.
 *   15) Remove the dip.input file. 
 *   16) Release interlock on dip.lock1. 
 *   17) Release interlock on dip.lock2. 
 */

#define LOCK1	"dip.lock1"
#define LOCK2	"dip.lock2"
#define MAIL	"dip.mail"
#define INPUT	"dip.input"
#define LOGo	"dip.rlog"
#define LOGe	"dip.log"

#define E_FATAL -2
#define E_WARN  -1

char line[1024];
char line1[1024];
static struct stat sbuf;
static short badmail = 0;

int Aflg, xflg, bflg;
//int Dflg;

struct opts_s options = {0};

/*
 * If we receive a SIGPIPE it means that the Judge couldn't handle the mail
 * (usually a bad From:) and died early. If so, we set the bad mail flag
 * and keep on truckin'.
 */
void handle_sigpipe(int signo)
{
	fprintf(stderr, "rdip caught signal %d -- assuming bad mail and skipping over it.\n", signo);
	badmail = 1;
}	

/* Will retry the lock several times before giving up */
int retry_lockfd(int n, int m)
{
#define MAX_RETRIES 5
#define SLEEP_WAIT 30
/* Following error is the dreaded 'Resource temporarily unavailable' */
#define TEMPORARY_ERROR 11 
    int attempt=0;
    int ret;
    char errtext[50];

    do
    {
	attempt++;
        ret = lockfd(n, m);
	if (errno == TEMPORARY_ERROR) {
	   sprintf(errtext,"retrying %d", attempt); 
	   perror(errtext);
	   sleep(SLEEP_WAIT);
	}
    }
    while (ret && errno == TEMPORARY_ERROR && attempt < MAX_RETRIES);

    return ret;
}

int main(int argc, char **argv)
{

	int i, n;
	char *s;
	gchar opt_cfgdir = NULL;
	int fd1, fd2;
	FILE *fp, *pfp;
	long now;
	int dipstat;
	char temp_text[50];
	nice(10);

/*	conf_init();*/  /* to get path for dip exe */
/*	conf_readfile(CONFIG_FILE);
*/

/*
	sprintf(temp_text,"%s-%s", JUDGE_CODE, "rdip");
 	OPENDIPLOG(temp_text);
        DIPINFO("Started rdip");
*/	

	/* we ought to use sigaction, but it's late and this is already hacky... */
	signal(SIGPIPE, handle_sigpipe);

	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			for (s = argv[i] + 1; *s; s++) {
				switch (*s) {
				case 'A':
					Aflg++;
					break;

				case 'b':
					bflg++;
					break;

				case 'd':
					if (++i < argc) {
						if (chdir(argv[i])) {
							perror(argv[i]);
							exit(1);
						}
						/*
						 * stash away into opt_cfgdir to override after config init
						 * FIXME: this is ugly
						 */
						opt_cfgdir = argv[i];
					} else {
						fprintf(stderr, "-d option must specify directory.\n");
						exit(1);
					}
					break;

				case 'x':
					xflg++;
					break;

				default:
					fprintf(stderr, "Usage: %s [-Abx] [-d directory]\n", argv[0]);
					fprintf(stderr, "   A: pass 'A' flag through to 'dip'\n");
					fprintf(stderr, "   b: Execute in background\n");
					fprintf(stderr, "   d: Directory for adjudicator\n");
					fprintf(stderr, "   x: No mail to process on stdin\n");
					exit(1);
				}
			}
		}
	}

      conf_init();  /* to get path for dip exe */
      conf_read_file(CONFIG_DIR, CONFIG_FILE, NULL);
      if (opt_cfgdir) CONFIG_DIR = opt_cfgdir;

        sprintf(temp_text,"%s-%s", conf_get("judge_code"), "rdip");
        diplog_syslog_open(temp_text);
        diplog_syslog_entry(LOG_INFO, "Started rdip");


        options.debug = 0;

	/*
	 *  1) Obtain a lock on the "dip.lock1" file.
	 */

	fd1 = open(LOCK1, O_RDWR | O_CREAT, 0666);
	if (fd1 < 0) {
		perror(LOCK1);
		exit(1);
	}
	if (lockfd(fd1, 0)) {
		perror(LOCK1);
		exit(1);
	}
	/*
	 *  2) Append mail to dip.mail (if no -x flag).
	 */
	if (!xflg) {
		if (!(fp = fopen(MAIL, "a"))) {
			perror(MAIL);
			exit(1);
		}
		n = 0;
		while (fgets(line, sizeof(line), stdin)) {
			if (bflg && n++ && !strncmp(line, "From ", 5)) {
				fputc('>', fp);
			}
			fputs(line, fp);
		}
		fclose(fp);
	}
	/*
	 *  3) If dip.input exists, exit -- another rdip running.
	 */

	if ((fd2 = open(INPUT, O_RDWR | O_CREAT | O_EXCL, 0666)) < 0) {
		/*printf("Exit, %s exists.\n", INPUT);*/
		exit(0);
	}
	close(fd2);

	/*
	 *  4) Release sendmail (if -b flag).
	 */

	if (bflg) {
		close(0);
		close(1);
		close(2);
		if (fork())
			exit(0);
		open("/dev/null", O_RDONLY);	/* stdin  */
		open(LOGo, O_RDWR | O_APPEND | O_CREAT, 0600);	/* stdout */
		open(LOGe, O_RDWR | O_APPEND | O_CREAT, 0600);	/* stderr */
	}
	/*
	 *  5) Obtain a lock on the "dip.lock2" file.
	 */

	fd2 = open(LOCK2, O_RDWR | O_CREAT, 0666);
	if (fd2 < 0) {
		perror(LOCK2);
		exit(1);
	}
	if (retry_lockfd(fd2, 1)) {
		perror(LOCK2);
		exit(1);
	}
	time(&now);
	printf("%12.12s: Rdip starting.\n", ctime(&now) + 4);

	/*
	 *  6) Rename dip.mail to dip.input.
	 */

      step6:
	if (rename(MAIL, INPUT)) {	
		/*
	        fprintf(stderr,"mv %s -> ",MAIL);
	        perror(INPUT);                               
		*/
		goto step11;
	}
	time(&now);
	printf("%12.12s: Processing new input file.\n", ctime(&now) + 4);

	/*
	 *  7) Release interlock on dip.lock1.
	 */

	close(fd1);

	/*
	 *  8) For each mail message on dip.input invoke "dip -q". 
	 */

	if (!(fp = fopen(INPUT, "r"))) {
		perror(INPUT);
		exit(1);
	}
	pfp = NULL;

	sprintf(line1,"%s -C %s -q", conf_get("cmd_dip"), CONFIG_DIR);
	while (fgets(line, sizeof(line), fp)) {
		if (!strncmp(line, "From ", 5)) {
			if (pfp) {
				if(!badmail)
					pclose(pfp);
				pfp = NULL;
			}
			if (!(pfp = popen(line1, "w"))) {
				perror("popen");
				exit(1);
			}
			fputs(line, stdout);
			badmail = 0;	/* reset badmail flag */
		}
		if (!pfp) {
			fprintf(stderr, "First line doesn't look like From.\n");
			fputs(line, stderr);
			exit(1);
		}
		if(!badmail)
			fputs(line, pfp);
	}

	fclose(fp);
	if (pfp && !badmail) {
		dipstat = pclose(pfp);
		if (dipstat >> 8) {
			if(!badmail)
			{
				rdip_bailout(E_FATAL);
				exit(dipstat >> 8);
			}
		}
		pfp = (FILE *) NULL;
	}
	/*
	 *  9) Reobtain the interlock on dip.lock1.
	 */

	fd1 = open(LOCK1, O_RDWR | O_CREAT, 0666);
	if (fd1 < 0) {
		perror(LOCK1);
		exit(1);
	}
	if (lockfd(fd1, 0)) {
		perror(LOCK1);
		exit(1);
	}
	/*
	 * 10) If dip.mail exists loop back up to step 6.
	 */

	if (!stat(MAIL, &sbuf)) {
		goto step6;
	}
	/*
	 * 11) Release interlock on dip.lock1. 
	 */
      step11:
	close(fd1);

	/*
	 * 12) Invoke "dip -x".
	 */
	time(&now);
	printf("%12.12s: Processing master file.\n", ctime(&now) + 4);

	sprintf(line, "%s -C %s -%sx ", conf_get("cmd_dip"), CONFIG_DIR, Aflg ? "A" : "");
	if (!(pfp = popen(line, "r"))) {
		perror("popen");
		exit(1);
	}
	while (fgets(line, sizeof(line), pfp));
	pclose(pfp);

	/*
	 * 13) Reobtain the interlock on dip.lock1.
	 */

	fd1 = open(LOCK1, O_RDWR | O_CREAT, 0666);
	if (fd1 < 0) {
		perror(LOCK1);
		exit(1);
	}
	if (lockfd(fd1, 0)) {
		perror(LOCK1);
		exit(1);
	}
	/*
	 * 14) If dip.mail exists loop back up to step 6.
	 */

	if (!stat(MAIL, &sbuf)) {
		goto step6;
	}
	/*
	 * 15) Remove the dip.input file.
	 */

	remove(INPUT);

	/*
	 * 16) Release interlock on dip.lock1.
	 */

	close(fd1);

	/*
	 * 17) Release interlock on dip.lock2.
	 */

	close(fd2);

	time(&now);
	printf("%12.12s: Rdip complete.\n", ctime(&now) + 4);

	exit(0);

}
