/* $Header: /u2/lynch/ccc/cdb/RCS/rcdbd.c,v 1.1 86/09/11 13:46:50 lynch Exp Locker: lynch $ */
/* $Log:	rcdbd.c,v $
 * Revision 1.1  86/09/11  13:46:50  lynch
 * Initial revision
 * 
 * Revision 1.1  84/02/14  15:45:52  root
 * Initial revision
 *  */
#ifndef lint
static char sccsid[] = "@(#)rexecd.c	4.10 (Berkeley) 83/07/02";
#endif

#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>

#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <netdb.h>

typedef int sock;

extern	errno;
struct	sockaddr_in sin = { AF_INET };
struct	passwd *getpwnam();
char	*crypt(), *rindex(), *sprintf();
/* VARARGS 1 */
int	error();
int	reapchild();
/*
 * remote execute server:
 *	remoteDir\0
 *	username\0
 *	password\0
 *	command\0
 *	data
 */
main(argc, argv)
	int argc;
	char **argv;
{
	sock f;
	struct sockaddr_in from;
	struct servent *sp;

	sp = getservbyname("cdb", "tcp");
	if (sp == 0) {
		fprintf(stderr, "tcp/cdb: unknown service\n");
		exit(1);
	}
	sin.sin_port = sp->s_port;
#ifndef DEBUG
	if (fork())
		exit(0);
	for (f = 0; f < 10; f++)
		(void) close(f);
	(void) open("/", 0);
	(void) dup2(0, 1);
	(void) dup2(0, 2);
	(void) dup2(0, 3);
	{ sock t = open("/dev/tty", 2);
	  if (t >= 0) {
		ioctl(t, TIOCNOTTY, (char *)0);
		(void) close(t);
	  }
	}
#endif
	argc--, argv++;
	f = socket(AF_INET, SOCK_STREAM, 0, 0);
	if (f < 0) {
		perror("rcdbd: socket");
		exit(1);
	}
	if (bind(f, &sin, sizeof (sin), 0) < 0) {
		perror("rcdbd: bind:");
		exit(1);
	}
	signal(SIGCHLD, reapchild);
	listen(f, 10);
	for (;;) {
		sock s;
		int len = sizeof (from);

		s = accept(f, &from, &len, 0);
		if (s < 0) {
			if (errno == EINTR)
				continue;
			perror("rcdbd: accept");
			sleep(1);
			continue;
		}
		if (fork() == 0) {
			signal(SIGCHLD, SIG_IGN);
			doit(s, &from);
		}
		(void) close(s);
	}
}

reapchild()
{
	union wait status;

	while (wait3(&status, WNOHANG, 0) > 0)
		;
}

char	username[20] = "USER=";
char	homedir[64] = "HOME=";
char	shell[64] = "SHELL=";
char	*envinit[] =
	    {homedir, shell, "PATH=:/usr/ucb:/bin:/usr/bin", username, 0};
char	**environ;

struct	sockaddr_in asin = { AF_INET };

doit(f, fromp)
	sock f;
	struct sockaddr_in *fromp;
{
	char cmdbuf[NCARGS+1+5], *cp, *namep;
	char user[16], pass[16];
	struct passwd *pwd;
	sock s, cdbs;
	short port;
	int pv[2], pid, ready, readfrom, cc;
	char buf[BUFSIZ], sig;
	int one = 1;
	char	directory[50];

	(void) signal(SIGINT, SIG_DFL);
	(void) signal(SIGQUIT, SIG_DFL);
	(void) signal(SIGTERM, SIG_DFL);
#ifdef DEBUG
	{ int t = open("/dev/tty", 2);
	  if (t >= 0) {
		ioctl(t, TIOCNOTTY, (char *)0);
		(void) close(t);
	  }
	}
#endif
	dup2(f, 0);
	dup2(f, 1);
	dup2(f, 2);
	dup2(f, 3);
	(void) alarm(60);
	port = 0;
	for (;;) {
		char c;
		if (read(f, &c, 1) != 1)
			exit(1);
		if (c == 0)
			break;
		port = port * 10 + c - '0';
	}
	(void) alarm(0);
	if (port != 0) {
		s = socket(AF_INET, SOCK_STREAM, 0, 0);
		if (s < 0)
			exit(1);
		if (bind(s, &asin, sizeof (asin), 0) < 0)
			exit(1);
		(void) alarm(60);
		fromp->sin_port = htons((u_short)port);
		if (connect(s, fromp, sizeof (*fromp), 0) < 0)
			exit(1);
		(void) alarm(0);

		/* now do the cdb command stream */
		cdbs = socket(AF_INET, SOCK_STREAM, 0, 0);
		if (cdbs < 0)
			exit(1);
		if (bind(cdbs, &asin, sizeof (asin), 0) < 0)
			exit(1);
		(void) alarm(60);
		if (connect(cdbs, fromp, sizeof (*fromp), 0) < 0)
			exit(1);
		(void) alarm(0);
	}
	dup2(cdbs, 3);
	if ( cdbs != 3)
		(void) close(cdbs);

	getstr(directory, sizeof directory, "directory");
	getstr(user, sizeof(user), "username");
	getstr(pass, sizeof(pass), "password");
	strncpy(cmdbuf, "exec ", 5);
	getstr(&cmdbuf[5], sizeof(cmdbuf)-5, "command");
	setpwent();
	pwd = getpwnam(user);
	if (pwd == NULL) {
		error("Login incorrect.\n");
		exit(1);
	}
	endpwent();
	if (*pwd->pw_passwd != '\0') {
		namep = crypt(pass, pwd->pw_passwd);
		if (strcmp(namep, pwd->pw_passwd)) {
			error("Login incorrect.\n");
			exit(1);
		}
	}
	if ( *directory != '\0' )
		pwd->pw_dir = directory;
	if (chdir(pwd->pw_dir) < 0) {
		error("No remote directory %s.\n", pwd->pw_dir);
		exit(1);
	}
	(void) write(2, "\0", 1);
	if (port) {
		(void) pipe(pv);
		pid = fork();
		if (pid == -1)  {
			error("Try again.\n");
			exit(1);
		}
		if (pid) {
			(void) close(0); (void) close(1); (void) close(2);
			(void) close(f); (void) close(pv[1]);
			(void) close(3);
			readfrom = (1<<s) | (1<<pv[0]);
			ioctl(pv[1], FIONBIO, (char *)&one);
			/* should set s nbio! */
			do {
				ready = readfrom;
				(void) select(16, &ready, 0, 0, 0);
				if (ready & (1<<s)) {
					if (read(s, &sig, 1) <= 0)
						readfrom &= ~(1<<s);
					killpg(pid, sig);
				}
				if (ready & (1<<pv[0])) {
					cc = read(pv[0], buf, sizeof (buf));
					if (cc <= 0) {
						shutdown(s, 1+1);
						readfrom &= ~(1<<pv[0]);
					} else
						(void) write(s, buf, cc);
				}
			} while (readfrom);
			exit(0);
		}
		setpgrp(0, getpid());
		(void) close(s); (void)close(pv[0]);
		dup2(pv[1], 2);
	}
	pwd->pw_shell = "/bin/sh";
	(void) close(f);
/*	initgroups(pwd->pw_name, pwd->pw_gid); */
	(void) setuid(pwd->pw_uid);
	(void) setgid(pwd->pw_gid);
	environ = envinit;
	strncat(homedir, pwd->pw_dir, sizeof(homedir)-6);
	strncat(shell, pwd->pw_shell, sizeof(shell)-7);
	strncat(username, pwd->pw_name, sizeof(username)-6);
	cp = rindex(pwd->pw_shell, '/');
	if (cp)
		cp++;
	else
		cp = pwd->pw_shell;
	execl(pwd->pw_shell, cp, "-c", cmdbuf, 0);
	perror(pwd->pw_shell);
	exit(1);
}

/* VARARGS 1 */
error(fmt, a1, a2, a3)
	char *fmt;
	int a1, a2, a3;
{
	char buf[BUFSIZ];

	buf[0] = 1;
	(void) sprintf(buf+1, fmt, a1, a2, a3);
	(void) write(2, buf, strlen(buf));
}

getstr(buf, cnt, err)
	char *buf;
	int cnt;
	char *err;
{
	char c;

	do {
		if (read(0, &c, 1) != 1)
			exit(1);
		*buf++ = c;
		if (--cnt == 0) {
			error("%s too long\n", err);
			exit(1);
		}
	} while (c != 0);
}
