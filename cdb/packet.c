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
 *  * a: used for taskp on waitTrans
 * 
 * William L. Lynch		Tue Aug  5 15:58:56 1986
 *---------------------------------------------------------------------------
 */

#include "../DD_include.h"
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "cdb.h"
#include "static.h"
#include "tables.h"
#include "../bool.h"	

extern sock cdbs;

bool Reading = FALSE;		/* reading from a file */
bool Saving = FALSE;		/* saving to a file */
int outFile;
 
saving( flag )
bool flag;
{
	Saving = flag;
}

outputFile( fileName )
string fileName;
{
	FILE *out;
	
	Saving = TRUE;
	out =  fopen(fileName, "w");
	if ( out == NULL )
		fatal(1, "Can't open %s.\n", fileName);
	outFile = fileno(out);
}

inputFile( fileName )
string fileName;
{
	FILE *in;
	
	Reading = TRUE;
	in = fopen(fileName, "r");
	if ( in == NULL )
		fatal(1, "Can't open %s.\n", fileName);
	cdbs = fileno(in);
}


static int
message(d, buf, size)
int d;
char *buf;
int size;
{
	int cc, left;

	left = size;
	while ( left > 0 ) {
		cc = read(d, buf, size);
		if ( cc != 0 ) {
			left -= cc;			
			buf += cc;
		} else {
			size = 0;
			left = 0;
		}
	}
	return size;
}



get(socket, msg, size)
sock socket;
char *msg;
int size;
{
	int returnVal;

	returnVal = message(socket, msg, size);
	if ( Saving )
		write(outFile, msg, size);
	return returnVal;
}

static
recvShort(msg)
unsigned *msg;
{
	u_short message;

	get(cdbs, &message, sizeof(u_short));
	*msg = (unsigned)ntohs(message);
}


static
recvLong(msg)
unsigned *msg;
{
	u_long message;

	get(cdbs, &message, sizeof(u_long));
	*msg = (unsigned)ntohl(message);
}


#define RECVsTRINGsIZE	20

static
recvString(str)
string *str;
{	
	char msg;
	string buf;
	char *bp;
	int bufSize = RECVsTRINGsIZE;
	
	buf = bp = (char *)malloc(RECVsTRINGsIZE);
	do {
		if ( bp > buf + bufSize ) {
			buf = (char *)realloc(buf, 2*bufSize);
			bp = buf + bufSize + 1;
			bufSize *= 2;
		}
		get(cdbs, &msg, sizeof msg);	
		*bp++ = msg;
	} while ( msg != '\0' );
	*str = newstr(buf);
	free(buf);
}



packet_recvP1(time, pid, id)
u_long *time;
unsigned *pid;
unsigned *id;
{
	recvLong(time);
	recvShort(pid);
	recvShort(id);
}



packet_recvP2(time, pid, toPid, id)
u_long *time;
unsigned *pid;
unsigned *toPid;
unsigned *id;
{
	recvLong(time);
	recvShort(pid);
	recvShort(toPid);
	recvShort(id);
}

packet_recvP3(time, pid, id, toPid,  parms)
u_long *time;
unsigned *pid;
unsigned *id;
unsigned *toPid;
string *parms;
{
	recvLong(time);
	recvShort(pid);
	recvShort(id);
	recvLong(toPid);		/* might be taskp, use long */
	recvString(parms);
}

packet_recvP4(time, pid)
u_long *time;
unsigned *pid;
{
	recvLong(time);
	recvShort(pid);
}

packet_recvP5(time, pid, toPid)
u_long *time;
unsigned *pid, *toPid;
{
	recvLong(time);
	recvShort(pid);
	recvShort(toPid);
}

packet_recvP6(time, pid, toPid, task)
u_long *time;
unsigned *pid, *toPid, *task;
{
	recvLong(time);
	recvShort(pid);
	recvShort(toPid);
	recvLong(task);
}


/*---------------------------------------------------------------------------
 * The following are used for the initialization of the symbol an
 * occurrence tables in cdb
 *---------------------------------------------------------------------------
 */


packet_recvSize(size)
unsigned *size;
{
	recvShort(size);
}

	
packet_recvSymbol(sym)
symbol *sym;
{
	recvString(&sym->name);
	recvShort(&sym->space);
	recvShort(&sym->num);
	recvLong(&sym->chain);
	recvShort(&sym->obj);
}

packet_recvOccur(ocr)
occur *ocr;
{
	unsigned i;

	recvString(&ocr->name);
	recvShort(&ocr->line);
	packet_recvSize(&ocr->numFormals);
	ocr->formals = instances(ocr->numFormals, formal);
	for ( i=0; i < ocr->numFormals; i++ ) 
		recvString(&ocr->formals[i]);
}

packet_recvOtherOccur(occur)
otherOccur *occur;
{
	recvShort(&occur->line);
}

packet_recvSelectOccur(occur)
selectOccur *occur;
{
	recvShort(&occur->line);
	recvShort(&occur->termLine);
	recvShort(&occur->immedLine);
}

packet_recvTreturnOccur(occur)
treturnOccur *occur;
{
	recvShort(&occur->line);
}

packet_recvName( name )
string *name;
{
	recvString(name);
}

