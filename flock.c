/*
 * $Log$
 * Revision 1.2  2003/09/09 18:51:29  jaldhar
 * Got rid of port.h and replaced with some extra configure checks.  The
 * include strings.h was not carried over because it is commented out and
 * likely wouldn't work anyway.
 *
 * Revision 1.1  1998/02/28 17:49:42  david
 * Initial revision
 *
 * Revision 1.1  1996/10/20 12:29:45  rpaar
 * Morrolan v9.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "config.h"

/*
 *  Flock, theory of operation.
 *
 *  In the simple case where a command is specified, flock just calls the
 *  flock(2) routine and then invokes the command with system(3).  When the
 *  command completes flock terminates and the file is automatically unlocked.
 *
 *  In the more exciting case the first invokation of flock locks the file
 *  and a subsequent invokation at some later time will unlock it.  The user
 *  is not put into a subshell.  To accomplish this, the first invokation of
 *  flock forks off a child to do the actual locking.  When the file is
 *  locked a QUIT signal is sent back to the parent which then terminates.
 *  The pid of the child is written to a file in the /tmp directory so the
 *  second invokation of flock can find the child and send it a quit signal.
 *  The child retains the controlling terminal of the parent.  If it detects
 *  that this has been detached it will terminate.  If the parent had no
 *  controlling terminal the child will terminate when its grandparent's pid
 *  no longer exists (as detected with a zero signal).
 */

static char *lfile, cfile[20];

void xit(int sig)
{
	if (sig == SIGUSR1)
		exit(1);
	else if (sig == SIGUSR2)
		exit(2);
	exit(0);

}

void hangup(int unused)
{
	printf("\rFile %s unlocked.\r\n", lfile);
	remove(cfile);
	exit(0);
}

int main(int argc, char **argv)
{

	char *s, *tty, *pname, line[100], *ttyname();
	int i, c, fd0, fd1, pid, ppid, pppid;
	int qflg = 0, nflg = 0;


	pname = *argv;

	while (argc > 1 && argv[1][0] == '-') {
		for (i = 1; (c = argv[1][i]); i++) {
			switch (c) {
			case 'q':
				qflg++;
				break;
			case 'n':
				nflg++;
				break;
			default:
			      usage:
				fprintf(stderr, "Usage: %s [options] file          To lock a file.\n", pname);
				fprintf(stderr, "       %s                         To release above lock.\n", pname);
				fprintf(stderr, "       %s [options] command file  To lock file while executing command.\n", pname);
				fprintf(stderr, "\n");
				fprintf(stderr, "Options: -q to suppress 5 minute reminder.\n");
				fprintf(stderr, "         -n to not wait if busy, status=2.\n");
				exit(1);
			}
		}
		argv++;
		argc--;
	}

	pppid = 0;
	if (argc != 3) {
		if ((tty = ttyname(0))) {
			if ((s = strrchr(tty, '/')))
				tty = s + 1;
		} else {
			pppid = getppid();
			sprintf(line, "%d", pppid);
			tty = line;
		}
		sprintf(cfile, "/tmp/flk_%s", tty);
	}
	switch (argc) {

	case 1:

		/*
		   **  Process unlock function
		 */

		if ((fd0 = open(cfile, O_RDWR)) < 0) {
			fprintf(stderr, "No file is locked.\n");
			goto usage;
		}
		read(fd0, line, sizeof(line));
		if (!(s = strchr(line, ' '))) {
			fprintf(stderr, "Invalid format in %s file.\n", cfile);
			exit(1);
		}
		*s++ = '\0';
		pid = atoi(line);
		kill(pid, SIGHUP);
		break;

	case 2:

		/*
		 * Process lock file function
		 */

		lfile = argv[1];

		signal(SIGQUIT, xit);
		signal(SIGUSR1, xit);
		signal(SIGUSR2, xit);

		if ((pid = fork())) {
			for (;;) {
				sleep(30);
				printf("Waiting for lock on %s.\n", lfile);
			}

		} else {

			pid = getpid();
			ppid = getppid();
			if ((fd0 = open(cfile, O_RDWR | O_CREAT, 0600)) < 0) {
				perror(cfile);
				kill(ppid, SIGUSR1);
				exit(1);
			}
			if (lockfd(fd0, 1) < 0) {
				read(fd0, line, sizeof(line));
				if (!(s = strchr(line, ' '))) {
					fprintf(stderr, "Invalid %s file out there.  Bailing out.\n", cfile);
					kill(ppid, SIGUSR1);
					exit(1);
				}
				*s++ = '\0';
				fprintf(stderr, "File %s is already locked by %s. Use %s to unlock.\n",
					s, line, pname);
				kill(ppid, SIGUSR1);
				exit(1);
			}
			if ((fd1 = open(lfile, O_RDWR)) < 0) {
				perror(lfile);
				remove(cfile);
				kill(ppid, SIGUSR1);
				exit(1);
			}
			if (lockfd(fd1, nflg) < 0) {
				perror(lfile);
				remove(cfile);
				kill(ppid, SIGUSR2);
				exit(1);
			}
			sprintf(line, "%d %s", pid, lfile);
			write(fd0, line, strlen(line) + 1);
			signal(SIGHUP, hangup);
			signal(SIGTERM, hangup);
			if (!qflg && !pppid)
				printf("\rFile %s locked.  Use %s to unlock.\r\n", lfile, pname);
			kill(ppid, SIGQUIT);
			for (;;) {
				sleep(5 * 60);
				if (pppid) {
					if (kill(pppid, 0))
						hangup(0);
				} else {
					if (!isatty(1))
						hangup(0);
				}
				if (!qflg && !pppid)
					printf("\rFile %s locked.  Use %s to unlock.\r\n", lfile, pname);
			}
		}
		break;

	case 3:

		/*
		   **  Process lock command file function
		 */

		if ((fd1 = open(argv[2], O_RDWR)) < 0) {
			perror(argv[2]);
			exit(1);
		}
		if (lockfd(fd1, 1) < 0) {

			if (nflg) {
				if (!qflg)
					fprintf(stderr, "File %s busy.\n", argv[2]);
				exit(2);
			}
			if (!qflg)
				printf("File %s busy, waiting...\n", argv[2]);
			if (lockfd(fd1, 0) < 0) {
				perror(argv[2]);
				exit(1);
			}
		}
		sprintf(line, "%s %s", argv[1], argv[2]);
		system(line);
		break;

	default:
		goto usage;
	}

	exit(0);
	return 0;

}
