/*---------------------------------------------------------------------------
 * This module is the front end for the concurrent debugger, cdb.  The
 * usage is 
 * 		cdb [options] rcmd
 * 
 * The currently implemented options are 
 * 
 * 	-h remotehost     	(default localhost)
 * 	-d path			(directory for remote execution)
	-i infile		read from trace file
	-o outfile		save trace file
	-O outfile		ascii trace file
	-t outfile		trace, no display

 * 
 * option to be added include
 * 
 * 	-g		start the remote program under dbx for complete debugging
 * 
 * and rcmd is the command (the file and options compiled with the -G
 * option on the remote host, default a.out).
 * 
 * 
 * 
 * William L. Lynch 		Mon Jul 28 11:00:17 1986
 * 
 *---------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------
 * The communication protocol is for the front end (this cdb process) to
 * connect to the hosts cdb port, listen on that port, then start the user
 * process and accept on that port.  In this way, the user process needs
 * to know nothing about where the front end is.
 *---------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdio.h>
#include "cdb.h"
#include "../bool.h"

#define Stdin 0

extern bool Reading;
bool TraceOnly = FALSE;
bool Run = FALSE;

struct sgttyb ttyb;		/* tty state */

string programName;
string remoteHost = "localhost\0            ";
string remoteCommand;
string remoteDir = NULL;
char prog[12];
char *progForm = "%10.10s:";



sock std, err, cdbs;	/* stdin/stdout, stderr, cdb sockets for
			 communicating with remote processes */

main (argc,argv)
int argc;
string argv[];
{

	programName = argv[0];
	argv++; argc--;	
	while ( argc > 0 ) {
		if ( argv[0][0] == '-' )
			switch ( argv[0][1] ) {
			case 'h':
				argv++; argc--;
				remoteHost = argv[0];
			        break;
			case 'd':
				argv++; argc--;
				remoteDir = argv[0];
			        break;
			case 'i':
				argv++; argc--;
				inputFile(argv[0]);
				sprintf(prog, progForm, argv[0]);
				err = std = 21;   /* illegal, don't read */
			        break;
			case 'o':
				argv++; argc--;
				outputFile(argv[0]);
			        break;
			case 'O':
				argv++; argc--;
				asciiFile(argv[0]);
			        break;
			case 't':
				argv++; argc--;
				outputFile(argv[0]);
				TraceOnly = TRUE;
			        break;
			case 'g':
				fprintf(stderr, "-g unimplemented.\n");
			        break;
			default:
				fatal(1, "%s option unknown.\n", argv[0]);
			        break;
			}
		else {
			remoteCommand = argv[0];
			sprintf(prog, progForm, argv[0]);
		}
		argv++; argc--;
	}
	if ( TraceOnly && rcdb(&remoteHost, remoteCommand, 
					remoteDir, &std, &err, &cdbs) != -1) {
		commun_TraceOnly();
	} else if ( Reading || rcdb(&remoteHost, remoteCommand,
					remoteDir, &std, &err, &cdbs) != -1 ){
		cdb_init(prog);
		commun_IO();
		cdb_end();
	} else 
		fatal(1,"Remote connection failed.\n");
}


cdb_init(prog)
string prog;
{
	int read, ready;
	
	push_devmode(0);
	set_devmode(0, 0600);
	win_init(0);
	command_init(prog);
	commun_init();
	display_init();
	static_init();
	dynamic_init();
}



cdb_end()
{
	display_end();
	commun_end();
	reset_devmode(0);
}



fatal(code, f, p0, p1, p2, p3, p4)
int code;
string f;
{
	fprintf(stderr, "%s: ", programName);
	fprintf(stderr, f, p0, p1, p2, p3, p4);
	exit(code);
}


