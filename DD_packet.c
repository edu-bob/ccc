/*---------------------------------------------------------------------------
 * These routines define the link between the communication package and
 * the cdb front end.
 * 
 * There are six types of packets, corresponding to six types of
 * events.  The first type is a simple block.  A first-type packet (P1)
 * contatins an event specifier, a time stamp, the pid of the process, and
 * the source occurrence id of the event.   The second type of event is an
 * unblock.  A P2 packet contains all the same information as a P1 packet,
 * but also includes a destination pid for the signal.  The third type of
 * event is one in which paramters are going to be sent to cdb.
 * Currently, these events are creates, accepts, and treturns.  The P3
 * packet is a P1 packet, but includes a string of the parameters.
 * The P4 packet does not correspond directly to source events, and
   consists of only an event specifier and timestamp.  The P5 packet is a
   P4 packet with a destination pid.  The P6 packet is for DD_ISbORN
   signals, where cdb will need to know both the p5 information and the
   task pointer.
 * 
 * 	event	time	pid	topid	id	parms	task *
 * P1	y	y	y		y
 * P2	y	y	y	y	y
 * P3	y	y	y	a	y	y
 * P4	y	y	y
 * P5	y	y	y	y
 * P6	y	y	y	y			y
 * 
 * a: used for taskp on waitTrans
 * 
 * William L. Lynch		Tue Aug  5 15:58:56 1986
 *---------------------------------------------------------------------------
 */

/*#define Debug
#define Debug2*/

#include "DD_include.h"
#include "DD_kernel.h"
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>

typedef char *string;

extern int DD_cdbs;

#define IOVMAX	6
static struct iovec iov[IOVMAX];
static int ioveclen = 0;

static
sendShort( msg )
u_short *msg;
{
	*msg = htons(*msg);
	iov[ioveclen].iov_base = (char *)msg;
	iov[ioveclen++].iov_len = sizeof(u_short);
#ifdef Debug
	DD_debug_printf("sendShort %u\n", (long)*msg);
#endif
}
	

static
sendLong( msg )
u_long *msg;
{
	*msg = htonl(*msg);
	iov[ioveclen].iov_base = (char *)msg;
	iov[ioveclen++].iov_len = sizeof(u_long);
#ifdef Debug
	DD_debug_printf("sendLong %lu\n", *msg);
#endif
}

	

static
sendString( msg )
string msg;
{
	iov[ioveclen].iov_base = msg;
	iov[ioveclen++].iov_len = strlen( msg ) + 1;
#ifdef Debug
	DD_debug_printf("sendString %s\n", msg);
#endif
}

static
send()
{
	int i, cc, total;
	struct iovec *iovp;
	
	iovp = iov;
	do {
		cc = writev(DD_cdbs, iovp, ioveclen);
		if ( cc == -1 )
			break;
		for ( i=total=0; i < ioveclen; i++ )
			total += iovp[i].iov_len;
		if ( total == cc )
			break;
#ifdef Debug2
	DD_debug_printf("sending %d of %d\n", cc, total);
#endif
		do
			ioveclen--;
		while ( (cc -= iovp++->iov_len) > 0 );
#ifdef Debug2
	for ( i=total=0; i < ioveclen; i++ )
		total += iovp[i].iov_len;
	DD_debug_printf("resending %d\n", total);
#endif
	} while ( ioveclen > 0 );
	
	ioveclen = 0;
}

DD_packet_sendP1( event, time, pid, id )
unsigned event;
u_long time;
unsigned pid;
unsigned id;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	sendShort(&id);
	send();
}



DD_packet_sendP2( event, time, pid, toPid, id )
unsigned event;
u_long time;
unsigned pid;
unsigned toPid;
unsigned id;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	sendShort(&toPid);
	sendShort(&id);
	send();
}


DD_packet_sendP3( event, time, pid, id, toPid, parms )
unsigned event;
u_long time;
unsigned pid;
unsigned id;
unsigned toPid;
string parms;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	sendShort(&id);
	sendLong(&toPid);	/* might be a taskp, so use long */
	sendString(parms);
	send();
}


DD_packet_sendP4( event, time, pid)
unsigned event;
u_long time;
unsigned pid;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	send();
}

DD_packet_sendP5( event, time, pid, toPid )
unsigned event;
u_long time;
unsigned pid, toPid;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	sendShort(&toPid);
	send();
}

DD_packet_sendP6( event, time, pid, toPid, task )
unsigned event;
u_long time;
unsigned pid, toPid, task;
{
	sendShort(&event);
	sendLong(&time);
	sendShort(&pid);
	sendShort(&toPid);
	sendLong(&task);
	send();
}

DD_packet_sendCommun( event )
unsigned event;
{
	sendShort(&event);
	send();
}
	
/*---------------------------------------------------------------------------
 * The following are used for the initialization of the symbol and
 * occurrence tables in cdb
 *---------------------------------------------------------------------------
 */


DD_packet_sendSize( size )
unsigned size;
{
	sendShort(&size);
	send();
}

DD_packet_sendSymbol( sym )
DD_symEntry sym;
{
	sendString(sym.name);
	sendShort(&sym.space);
	sendShort(&sym.num);
	sendLong(&sym.chain);
	sendShort(&sym.obj);
	send();
}


DD_packet_sendOccur( occur )
DD_occur occur;
{
	int i, number;
	
	sendString(occur.name);
	sendShort(&occur.line);
	number = occur.numFormals;	/* might be rearranged by sendShort */
	sendShort(&occur.numFormals);
	for ( i=0; i < number; i++ )  {
		if ( ioveclen == IOVMAX )
			send();
		sendString(occur.formals[i]);
	}
	send();
}

DD_packet_sendOtherOccur( occur )
DD_otherOccur occur;
{
	sendShort(&occur.line);
	send();
}

DD_packet_sendSelectOccur( occur )
DD_selectOccur occur;
{
	sendShort(&occur.line);
	sendShort(&occur.termLine);
	sendShort(&occur.immedLine);
	send();
}

DD_packet_sendTreturnOccur( occur )
DD_treturnOccur occur;
{
	sendShort(&occur.line);
	send();
}

DD_packet_sendName( name ) 
string name;
{
	sendString(name);
}

