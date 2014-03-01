/* Qtcall.c -- functions for making timed and untimed transaction calls. */

#include "CC_include.h"
#include <errno.h>

unsigned DD_tcallPid;
static pid    the_pid;
static task   t;
static int    n;
static float  the_time;
static int    timed;

/*
 *  CC_remember_time() is used to store the time and to remember the fact that
 *  this is a timed transaction call.
 */

CC_remember_time(time)
float time;
{
    the_time = time;
    timed = 1;
}

/*
 *  CC_tcallinit() remembers the server pid and the transaction number
 *  for later use by CC_timedtcall() (and thus CC_tcall).
 */

CC_tcallinit(p,transnum)
pid p;
int transnum;
{
    DD_tcallPid = (unsigned)(the_pid  = p);
    n = transnum;
}

/*
 *  A normal transaction call is treated as a special case of a timed transaction
 *  call, since so much logic is common to both (in particular, the case
 *  where there is an accept, either by itself or in a select statement,
 *  which is ready to accept the client process immediately). Thus
 *  CC_tcall() is simply a call to CC_timedtcall() but with no specified time.
 */

CC_tcall(id)
unsigned id;
{
    timed=0;
    CC_timedtcall(id);
}

/*
 *  CC_timedtcall() does the work for both timed and nontimed transaction calls.
 *  If someone is waiting for us, we notify them. Otherwise, if we need to
 *  know whether we have been accepted within a certain time, we wait
 *  for notification that we have been accepted. If this doesn't arive soon
 *  enough, we time out and the server looses the chance to serve us.
 *  Except in case of timeout, we will block waiting for transaction completion
 *  (even if we already slept waiting for acceptance notification).
 *  The accept call will normally place return status in our tsuccess@@@@@ field;
 *  If we time out, CC_tcall_sleep() (see below) takes care of tsuccess.
 */

int CC_timedtcall(id)
unsigned id;
{
/*
 *  First we need to check out both our task and the server task.
 *  If we can't check out our own task, then block and wait for termination.
 *  If we can't check out the server task, then return and report failure.
 *  If all goes well, we grab queuelock so we can see if anyone is waiting for us.
 */
    CC_checkout(mypid);
    if ((t=CC_checkout(the_pid))==nulltask) {
	CC_checkin(me);
	errno=ESRCH;
	if ( the_pid )
		CC_perror("failed transaction call: %d, %d",the_pid, the_pid->task);
	else 
		CC_perror("failed transaction call: %d", the_pid );
	return 0;
    }
    s_lock(t->queuelock);
    me->waiting=0;

/*
 *  If an accept (or select) statement is blocked waiting for our
 *  transaction call, we sieze the right to wake up the sleeping accept,
 *  and block on completion of the transaction call. We do not allow
 *  ourselves to time out in this case as we know we will be serviced
 *  as soon as possible and it makes the code cleaner at the accept end
 *  if we can guarantee the server that we will still be here when it wakes up.
 */
    DD_event(DD_WAITtRANS, id);
    if (t->selectedtrans==NOTRANS && t->selectcase[n]) {
	t->selectedtrans=n;
	t->newclient=me;
	s_unlock(t->queuelock);
	CC_unblock(t, DD_IMMEDsERV, id);
	CC_block(DD_NONE, id);
	DD_event(DD_TRETURNED, id);
/*
 *  otherwise, no one is blocked waiting for our transaction call; thus we
 *  get to wait to be accepted. If this is a timed transaction call, then
 *  we wait for acceptance with CC_tcall_sleep(), which takes care of
 *  detecting timeouts. Otherwise, we simply block on transaction completion.
 */
    } else {
	CC_enqueue(t,n);
	s_unlock(t->queuelock);
	if (timed)
	    CC_tcall_sleep(the_time, id);
	else {
	    CC_block(DD_NONE, id);
	    DD_event(DD_TRETURNED, id);
	}
    }
/*
 *  @@@@@@@@@@@
 */
    CC_checkin(t);
    CC_checkin(me);
    if (!me->tsuccess)
	errno=ESRCH;
    return me->tsuccess;
}

/*
 *  CC_tcall_sleep() is used to wait for a specified time for word from
 *  an accept statement that we have been accepted. If time runs out,
 *  we retract our request if we can and report a timeout...
 */

CC_tcall_sleep(time, id)
float time;
int id;
{
/*
 *  First prepare to handle signals in message mode.
 */
    CC_message_mode();
/*
 *  Invite messages of type TCALL...
 */
    me->waiting=1;
    s_unlock(t->queuelock);
/*
 *  ...and of type DELAY.
 */
    CC_settimer(time);
/*
 *  Sit and wait for a message.
 */
    CC_block(DD_WAITtIME, 0);
/*
 *  If we receive a TCALL message, or if a TCALL message is sent before we
 *  can mark ourselves as timed out (via CC_flush_TCALL), then we flush any
 *  DELAY messages and wait for the server to execute a treturn.
 *  Otherwise, we report transaction failure and return.
 */
    if (CC_message==TCALL || !CC_flush_TCALL(t,n)) {
	CC_flush_DELAY();
	CC_block(DD_NONE, id);
    } else {
	DD_event( DD_TIMEOUT, 0);
	me->tsuccess=0;
    }
	 
/*
 *  Return to using signals for blocking and unblocking processes.
 */
    CC_block_mode();
}
