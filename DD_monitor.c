/*---------------------------------------------------------------------------
 * The debugging system for the Concurrent C compiler.  The stages of
 * development are:
 * 	trace run-time-system events on the screen.
 * 	trace RTS events to a separate monitor process.
 * 	dump compile time symbol table information to the monitor.
 * 	fork the ccc program from the debugger.
 * 	connect debugger and monitor with sockets.
 * 	graphical output of debugger.
 * 
 * The final configuration:
 * 
 * 	-----------------   ------------	-----------------
 * 	| 		|   | buffer   | socket |		|
 * 	|   monitor	|---| system + |------->|  debugger	|
 * 	|		|   | commun   |	|		|
 * 	-----^----^------   ------------	-----------------
 * 	|		|
 * 	|    RTS	|
 * 	|		|
 * 	-----------------
 * 	|		|
 *	| user program 	|
 * 	|		|
 *	-----------------
 * 
 * The debugger may reside on another machine, such as a sun workstation,
 * and will format all of the raw data from the monitor.  The debugger
 * will start the monitor and other programs on the host machine.
 * 
 * William L. Lynch		Mon Jun 30 17:04:45 1986
 * 
 * The basic hooks into the run time system consist of extra parameters on
 * CC_block,  CC_unblock, and CC_kill calls which indicate the nature of
 * the call.  There are also a couple of renegade hooks for exiting and
 * delay statements  (and for CC_shmalloc, if that is added in).  To
 * compile without the debugger, simply link with null routines for
 * DD_event*.
 * 
 * The important information about the processes consists of transaction
 * and create parameters, and most everything else.  The transaction
 * parameters will be read into a buffer at accept time from the tblock,
 * and the result will be extracted at a treturn.  The create paramters
 * are more difficult, as they bypass the RTS.  
 *---------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include "CC_include.h"
#include "DD_kernel.h"
#include "DD_buffers.h"

int DD_cdbs = 3;
int DD_me;
extern char *CC_mytblock;
extern char **CC_tblock_ptr;	/* for returns */
char *DD_createArgs;


extern DD_ProcessTable *DD_processTable;

#define Accept(id,type)		DD_processTable->accept[id].type
#define Create(id,type)		DD_processTable->create[id].type
#define Transact(id,type)	DD_processTable->transact[id].type
#define Select(id,type)		DD_processTable->select[id].type
#define Delay(id,type)		DD_processTable->delay[id].type
#define Terminate(id,type)	DD_processTable->terminate[id].type
#define Treturn(id,type)	DD_processTable->treturn[id].type





void
DD_event( event, id )
DD_eventType event;
unsigned id;
{
#ifndef DD_NULL
	if ( event != DD_NONE )
		DD_sendBuf(event, id, 0);
#endif
}



void
DD_event2( event, toPid, id )
DD_eventType event;
unsigned toPid;
unsigned id;	
{
#ifndef DD_NULL
	DD_sendBuf(event, id, toPid);
#endif
}



#ifndef DD_NULL
DD_main(argc, argv, envp)
#else
main(argc, argv, envp)
#endif
int argc;
char **argv, **envp;
{
	/*
	 *  CC_maxtblocksize should really be calculated from sizeof()s of trans structs.
	 */
	CC_maxtblocksize=512;
	CC_install_handler();
	CC_make_elder_tasks();
	CC_shinit();
	CC_main(argc, argv, envp);
	CC_complete();
}



#ifndef DD_NULL

DD_init()
{
	DD_buffers_init();
	setlinebuf(stdout);
}

DD_end()
{
	DD_buffers_end();
	
}


extern shared bool Tracing;

DD_sendBuf (event, id, to )
DD_eventType event;
unsigned id;
unsigned to;
{
	static bool first = TRUE;
	buffer *bp;
	struct timezone tz;
	extern unsigned DD_tcallPid;		/* global set in
						 CC_tcallinit (hack) */

	if ( Tracing ) {
		bp = DD_buffers_getEmpty();
		bp->event = event;
		bp->id = id;
		gettimeofday(&(bp->time), &tz);
		bp->fromId = DD_me;
		bp->toId = to;
		switch (event) {
		case DD_ISbORN:
			bp->tblock = (char *)CC_mypid;
		        break;
		case DD_WAITbORN:
			bp->tblock = DD_buffers_getTblock(Create(id, tblockSize));
			bcopy(DD_createArgs, bp->tblock, Create(id, tblockSize));
			bp->convert = Create(id, convert);
			break;
		case DD_WAITtRANS:
			bp->tblock = DD_buffers_getTblock(Transact(id, tblockSize));
			bcopy(CC_mytblock, bp->tblock, Transact(id, tblockSize));
			bp->convert = Transact(id, convert);
			bp->toId = DD_tcallPid;
			break;
		case DD_TRETURN:
			bp->tblock = DD_buffers_getTblock(Treturn(id, valueSize));
			bcopy(*CC_tblock_ptr, bp->tblock, Treturn(id, valueSize));
			bp->toId = DD_tcallPid;
			bp->convert = Treturn(id, convert);
			break;
		}
		DD_buffers_addFull(bp);
	}
	
}

#define newstr(s)	(char *)strcpy(malloc(strlen(s)+1),s)

char *
DD_unctrl(ch)
char ch;
{
	char answer[3];
	
	if ( isascii(ch) && isprint(ch) ) {
		answer[0] = ch;
		answer[1] = '\0';
	} else {
		answer[0] = '^';
		answer[1] = ch == '' ? '?' : ch & '\0177' | '@';
		answer[2] = '\0';
	}
	
	return newstr(answer);
}


main(argc, argv, envp)
int argc;
char **argv, **envp;
{
	int childPid;
	struct stat buf;

	fstat(3, &buf);
	if ( (buf.st_mode & S_IFMT) == S_IFREG ) {	/* if not started by cdb */
		Tracing = FALSE;
		DD_main(argc, argv, envp);
		CC_complete();			/* just for safety */
		exit();
	}

	DD_init();
	/* fork off a process for user program */
	if ((childPid = fork()) == -1) {
		DD_perror("failed fork\n");
	} else if (childPid == 0) {
		setpgrp(0, getpid());
		kill(getpid(), SIGTSTP);
		DD_main(argc, argv, envp);	/* does not return */
		CC_complete();			/* but just in case */
		exit();
	}
	DD_communicate(childPid);
	DD_end();
}

#endif
