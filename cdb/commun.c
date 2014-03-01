/*---------------------------------------------------------------------------
 * Communication for the front end, cdb.
 * The basic routine is commun_IO, which works in an obvious fashion
   except for  the throttle.  The only way to receive a packet while in
   running (!single) mode is for the timer to expire and turn on the cdb
   bit in readfrom.  Select calls return a -1 on interrupt, so you can't
   hang.  You have to go through the select twice on an interrupt, 
 * lest the interrupt occurs
   right after a valid return from select and trashes the ready value.
 * 
 * William L. Lynch		Fri Aug  8 17:13:45 1986
 *---------------------------------------------------------------------------
 */

#include "../DD_include.h"
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <signal.h>
#include "cdb.h"
#include "static.h"
#include "tables.h"
#include "../bool.h"

#include "eventString.h"

#define Stdin	0
#define Stdout	1
#define Stderr	2

#define bit(x)	(1 << (x))
#define sigbit(x) bit((x)-1)
#define doCmd(s, cmd) if (cc <= 0) {shutdown(s, 2); readfrom &= ~bit(s);} else cmd
#define trans(s, cmd) 	{cc = read(s, b, sizeof b); doCmd(s,cmd);}

extern sock std, err, cdbs;

static unsigned long clockTime = 0;
static bool channelOpen = TRUE;
static bool timerOccurred = FALSE;

unsigned long
addTime(deltaTime)
unsigned deltaTime;
{
	clockTime += deltaTime;
	display_clock(clockTime);
	return clockTime;
}



packetRecv(ev)
u_short ev;
{
	long time;
	unsigned long pid, toPid, task;
	unsigned id;
	string parms;
	DD_eventType event;

	event = (DD_eventType)ntohs(ev);

	switch ( event ) {
	case DD_WAITaCCEPT:
	case DD_WAITsELECT:
	case DD_DELAY:
		packet_recvP1(&time, &pid, &id);
		dynamic_event(event, time, pid, 0, id);
		break;
	case DD_ACCEPT:
	case DD_TRETURNED:
	case DD_IMMEDsERV:
		packet_recvP2(&time, &pid, &toPid, &id);
		dynamic_event(event, time, pid, toPid, id);
	        break;
	case DD_WAITbORN:
	case DD_WAITtRANS:
	case DD_TRETURN:
		packet_recvP3(&time, &pid, &id, &toPid, &parms);
		dynamic_event(event, time, pid, toPid, id, parms);
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
		packet_recvP4(&time, &pid);
		dynamic_event(event, time, pid, 0);
		break;
	case DD_LASTcHILD:
	case DD_LASTuSERkILL:
	case DD_CHILDkILL:
	case DD_IMdYING:
	case DD_ABORTINGyOU:
		packet_recvP5(&time, &pid, &toPid);
		dynamic_event(event, time, pid, toPid);
		break;
	case DD_ISbORN:
		packet_recvP6(&time, &pid, &toPid, &task);
		dynamic_event(event, time, pid, toPid, 0, 0, task);
		break;
	case DD_COMMUN:
		cdbStatus("Communication finished.");
		channelOpen = FALSE;
		break;
	default:
		cdbStatus("Undefined packet received.");
	        break;
	}
	(void)addTime(time);
}


static unsigned readfrom;
static unsigned ready;
static bool finished = FALSE;
static bool single = TRUE;
static unsigned long throttled = 1;

commun_IO()
{
	char c;
	char b[85];
	int cc;
	
	readfrom = bit(std) | bit(err) | bit(Stdin);
	do {
		do {
			timerOccurred = FALSE;
			sigsetmask(sigblock(0) & ~sigbit(SIGALRM));
			ready = readfrom;
			cc = select(16, &ready, 0, 0, 0);
			sigblock(sigbit(SIGALRM));
		} while ( cc == -1 || timerOccurred );
		
		if ( ready & bit(std) ) 
			trans(std, userOut(b, cc);)
		if ( ready & bit(err) )
			trans(err, userErr(b, cc);)

		if ( ready & bit(cdbs) ) {
			u_short event;

			cc = get(cdbs, &event, sizeof event);
			doCmd(cdbs, packetRecv(event));

			if ( single ) 
				commun_recvPacket(FALSE);
			else
				commun_readyPacket();
		}

		if ( ready & bit(Stdin) ) 
			parse();

	} while ( !finished );
}


extern int outFile;

commun_TraceOnly()
{
	char b[85];
	int cc;
	static unsigned ready;
	bool first = TRUE;

	readfrom =  bit(std) | bit(err) | bit (cdbs) | bit(Stdin);
	do {
		ready = readfrom;
		select(16, &ready, 0, 0, 0);
		
		if ( ready & bit(Stdin) )
			trans(Stdin, write(cdbs, b, cc);)
		if ( ready & bit(std) ) 
			trans(std, write(Stdout, b, cc);)
		if ( ready & bit(err) )
			trans(err, write(Stdout, b, cc);)
		if ( ready & bit(cdbs) ) {
			trans(cdbs, write(outFile, b, cc);)
			if ( first ) {
				first = FALSE;
				commun_signal(SIGCONT);
			}
		}
	} while ( (readfrom & ~(unsigned)bit(Stdin)) != 0 );
}


commun_send( c )
char c;
{
	if ( channelOpen )
		write(std, &c, 1);
}


commun_finished( flag )
bool flag;
{
	finished = flag;
}

commun_readyPacket()
{
	setTimer(throttled);
}

commun_recvPacket( flag )
bool flag;
{
	if ( flag )
		readfrom |= bit(cdbs);
	else
		readfrom &= ~bit(cdbs);
}

commun_single( flag )
bool flag;
{
	single = flag;
}


commun_throttle( inc )
int inc;
{
	if ( inc == 0 )
		setTimer(0);
	else if ( (long)throttled + inc < 1 )
		throttled = 1;
	else
		throttled += inc;
}



setTimer( usec )
unsigned long usec;
{
	struct itimerval value;
	
	value.it_interval.tv_sec  = value.it_interval.tv_usec = 0;
	value.it_value.tv_usec = usec;
	value.it_value.tv_sec = 0;
	while ( value.it_value.tv_usec >= 1000000 ) {
		value.it_value.tv_usec -= 1000000;
		value.it_value.tv_sec++;
	}
	setitimer(ITIMER_REAL, &value, 0);
}


sigTimer( sig )
int sig;
{
	if ( !single ) {
		commun_recvPacket(TRUE);
		ready = readfrom;
		timerOccurred = TRUE;
	}
}

commun_signal(sig)
int sig;
{
	if ( channelOpen )
		send(err, &sig, 1, 0);
}

commun_init()
{
	signal(SIGALRM, sigTimer);
}

commun_end()
{
	struct itimerval value;
	
	timerclear(&value.it_value);
	setitimer(ITIMER_REAL, &value, 0);
}

