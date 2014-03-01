/*---------------------------------------------------------------------------
 * These routines are the communication link between the front/back end of
 * the debugger, which resides on the ouput host, and the user processes.
 * Buffers are received over  circulating queues, and translated into
 * front end messages, which are sent to the front end over a socket.
 * 
 * There is a very subtle bug here, which occurs because ccc uses pointers
   as its pids, and cdb uses unix pid numbers.  These cannot be
   translated here because the task structure could already have been
   deleted and the process died.  So, a lookup table must be kept either
   here or in the remote debugger (kept there, at the moment).  But, if an
   out of order request should happen to contain a pointer to an old task
   structure, which has been overwritten by a create which happened to
   create to the same memory location, the debugger cannot interpret this
   correctly.  This bug could be overcome with conversion in the monitor
   routine, but this would add greatly to the monitor routine complexity,
   which now simply bcopys the tblocks.
 * 
 * William L. Lynch		Fri Jul 11 10:37:20 1986
 *---------------------------------------------------------------------------
 */

/*#define Debug*/
/*#define Debug2*/


#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/wait.h>
#include "bool.h"
#include "DD_include.h"
#include "DD_kernel.h"
#include "DD_buffers.h"

#define RESOLUTION 10000		/* of timer */

#define instances(n,t)	(t *)calloc((n), sizeof (t))
#define instance(t)	instances(1,t)

#ifdef Debug
extern char *eventString[];
#endif

struct timeval DD_tv;		/* global starting time */

extern int DD_cdbs;
shared bool Tracing = TRUE;

static int sigGroup;

static bool finished;
static bool first = TRUE;

static int (*conversion)();
static DD_occur		*accepts;
static DD_occur		*creates;
static DD_occur		*transacts;
static DD_selectOccur	*selects;
static DD_otherOccur	*delays;
static DD_otherOccur	*terminates;
static DD_treturnOccur	*treturns;
static DD_symEntry	*symbols;
static int		symbolSize, acceptSize, transactSize, selectSize, delaySize;
static int		terminateSize, treturnSize, createSize;

static long seconds;				/* time of last buffer */
static long microSeconds;


DD_perror(s, p0, p1, p2, p3, p4, p5)
char *s;
{
	fprintf(stderr, "CDB ERROR: ");
	fprintf(stderr, s, p0, p1 , p2, p3, p4, p5);
}


DD_commun_sigPipe(sig)
int sig;
{
	buffer *bp;

	killpg(sigGroup, SIGCONT);
	killpg(sigGroup, SIGTERM);
	sleep(2);
	killpg(sigGroup, SIGKILL);
	sleep(1);
#ifdef Debug
	fprintf(stderr, "sigpipe\n");
#endif
	DD_buffers_forceEnd();
}


DD_commun_sigTstp(sig)
int sig;
{
	killpg(sigGroup, SIGSTOP);
}

DD_commun_sigCont(sig)
int sig;
{
	killpg(sigGroup, SIGCONT);
}


DD_commun_sigChld(sig)
int sig;
{
	union wait status;
	
#ifdef Debug
	fprintf(stderr, "sigchild");
#endif
	if ( wait3(&status, WNOHANG, 0) != 0 && WIFEXITED(status) )
		DD_commun_sigPipe(sig);
}

DD_commun_init()
{
	struct timezone tz;
		
	gettimeofday(&DD_tv, &tz);
	seconds = DD_tv.tv_sec;
	microSeconds = DD_tv.tv_usec;

	signal(SIGPIPE, DD_commun_sigPipe);
	signal(SIGTSTP, DD_commun_sigTstp);
	signal(SIGCONT, DD_commun_sigCont);
	signal(SIGCHLD, DD_commun_sigChld);

	sendTables();

}



#define sendTable(size, table,type) \
	DD_packet_sendSize(size);\
	for ( i=0; i < size; i++) DD_packet_send/**/type(table[i])



extern int DD_symbolSize;
extern DD_symEntry DD_SymbolTable[];
extern int DD_numberGlobals;
extern DD_globalOccurs DD_globalOccurTable[];

static
sendTables()
{
	int i;
	
	sendTable(DD_symbolSize, DD_SymbolTable,Symbol);
	DD_packet_sendSize(DD_numberGlobals);
	for ( i=0; i < DD_numberGlobals; i++ ) {
		DD_packet_sendName(DD_globalOccurTable[i].name);
		sendProcessTables(DD_globalOccurTable[i].table);
	}
}




static
sendProcessTables( pt )
DD_ProcessTable *pt;
{
	int i;

	sendTable(pt->numAccepts, pt->accept,Occur);
	sendTable(pt->numCreates, pt->create,Occur);
	sendTable(pt->numTransacts, pt->transact,Occur);
	sendTable(pt->numSelects, pt->select,SelectOccur);
	sendTable(pt->numDelays, pt->delay,OtherOccur);
	sendTable(pt->numTerminates, pt->terminate,OtherOccur);
	sendTable(pt->numTreturns, pt->treturn,TreturnOccur);
}


DD_communicate(processGroup)
int processGroup;
{
	buffer *bp;

	sigGroup = processGroup;
	DD_commun_init();
	
	while ( !finished ) {
		bp = DD_buffers_getFull();
		if ( Tracing )
			DD_commun_send(bp);

		switch ( bp-> event ) {
		case DD_WAITbORN:
		case DD_WAITtRANS:	
		case DD_TRETURN:
			DD_buffers_freeTblock(bp->tblock);
		        break;
		case DD_COMMUN:
			finished = TRUE;
		        break;
		}

		DD_buffers_addEmpty(bp);
	}
}



long
DD_commun_timeStamp(time)
struct timeval time;
{
	long sec, microSec;
		
	sec = time.tv_sec - seconds;
	microSec = time.tv_usec - microSeconds;

	seconds = time.tv_sec;
	microSeconds = time.tv_usec;

	return sec * 1000000/RESOLUTION + microSec / RESOLUTION;
}

int
DD_timeStamp()
{
	struct timeval tv;
	struct timezone tz;
	long sec, microSec;
	
	gettimeofday( &tv, &tz );
	sec = tv.tv_sec - DD_tv.tv_sec;
	microSec = tv.tv_usec - DD_tv.tv_usec;
	
	return (int)(sec * 1000000/RESOLUTION + microSec / RESOLUTION);
}


char *
DD_commun_parms(bp)
buffer *bp;
{
	static char answer[200];
	
	switch ( bp->event ) {		/* id-1 converts to CC_%d_%s_id
					 defines in program */
	case DD_WAITbORN:
		(*bp->convert)(DD_askCreate, bp->id-1, bp->tblock, answer);
	        break;
	case DD_WAITtRANS:
		(*bp->convert)(DD_askTrans, bp->id-1, bp->tblock, answer);
	        break;
	case DD_TRETURN:
		(*bp->convert)(DD_askTreturn, bp->id-1, bp->tblock, answer);
	        break;
	}
	return answer;
}


DD_commun_send(bp)
buffer *bp;
{
	switch (bp->event) {
	case DD_WAITaCCEPT:
	case DD_WAITsELECT:
	case DD_DELAY:
#ifdef Debug
		DD_debug_printf("sending P1: %s, from: %d, id: %d...",
				eventString[(int)bp->event], bp->fromId, bp->id);
#endif
		DD_packet_sendP1(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId, 
					bp->id);
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_ACCEPT:
	case DD_TRETURNED:
	case DD_IMMEDsERV:
#ifdef Debug
		DD_debug_printf("sending P2: %s, from: %d, to:%d, id: %d...",
			eventString[(int)bp->event], bp->fromId, bp->toId, bp->id);
#endif
		DD_packet_sendP2(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId, 
					bp->toId,
					bp->id);
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_WAITbORN:
	case DD_WAITtRANS:
	case DD_TRETURN:
#ifdef Debug
		DD_debug_printf("sending P3: %s, from: %d, to:%d, id: %d...",
			eventString[(int)bp->event], bp->fromId, bp->toId, bp->id);
#endif
		DD_packet_sendP3(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId, 
					bp->id, 
					bp->toId,
					DD_commun_parms(bp));
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_WAITaBORT:
	case DD_SELECTtERM:
	case DD_WAITcHILDdIE:
	case DD_WAITtIME:
	case DD_EATtIMER:
	case DD_EATtRANS:
	case DD_EATtERM:
	case DD_EXITING:
	case DD_TIMEOUT:
#ifdef Debug
		DD_debug_printf("sending P4: %s, from: %d...",
				eventString[(int)bp->event], bp->fromId);
#endif
		DD_packet_sendP4(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId);
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_LASTcHILD:
	case DD_LASTuSERkILL:
	case DD_CHILDkILL:
	case DD_IMdYING:
	case DD_ABORTINGyOU:
#ifdef Debug
		DD_debug_printf("sending P4: %s, from: %d, to:%d...",
			eventString[(int)bp->event], bp->fromId, bp->toId);
#endif
		DD_packet_sendP5(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId, 
					bp->toId);
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_ISbORN:
#ifdef Debug
		DD_debug_printf("sending P6: %s, from: %d, to: %d...",
				eventString[(int)bp->event], bp->fromId, bp->toId);
#endif
		DD_packet_sendP6(	(unsigned)bp->event,
					DD_commun_timeStamp(bp->time), 
					bp->fromId, 
					bp->toId,
					(unsigned)bp->tblock);
#ifdef Debug
		DD_debug_printf("sent\n");
#endif
		break;
	case DD_COMMUN:
		DD_packet_sendCommun((unsigned)bp->event);
		break;
	default:
		DD_perror("Commun: undefined buffer.\n");
	        break;
	}
}


