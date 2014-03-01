/*
** CC_accept.c -- routines for accept and select statements
*/

#include "CC_include.h"


static int the_transaction;
static int first;
static int client_on_queue;
char **CC_tblock_ptr;		/* used to be static, now used for debugger */


/*
** the CC_?choice variables only have meaning while a select is being
** processed.  Their values state which case in the generated "switch"
** state are best options for taking.
*/

int	CC_achoice;		/* an "accept" case to take		*/
int	CC_dchoice;		/* a "delay" case to take		*/
int	CC_ichoice;		/* an "immediate case to take		*/
int	CC_ochoice;		/* an "otherwise" case to take		*/
float	CC_bestdelay;
float	CC_trydelay;
bool	CC_accept_exists;
bool	CC_terminate_exists;

/*
**  This routine is called at the begining of the execution of a select statement.
**  It initializes the CC_?choice variables, checks the calling process out,
**  and freezes the transaction queues.
*/

CC_selectinit()
{
	CC_achoice = 0;
	CC_dchoice = 0;
	CC_ichoice = 0;
	CC_ochoice = 0;
	CC_bestdelay = 0.0;
	CC_accept_exists = FALSE;
	CC_terminate_exists = FALSE;
	CC_checkout(mypid);
	s_lock(me->queuelock);
	client_on_queue = 1;
}

/*
**  This routine is called when something other than an accept is selected.
**  It undoes the unwanted effects of CC_selectinit and of the evaluation
**  pass, such as leaving garbage in selectcase, leaving queuelock locked, etc.
*/

CC_select_cleanup()
{
	int j;

	for (j=0;j<CC_numtrans;j++)
		me->selectcase[j] = 0;
	s_unlock(me->queuelock);
	CC_checkin(me);
}

/*
**  CC_choose() does the actual selection if rules 1 to 4 apply. If rule
**  5 must be used, the result from CC_select() is used. Note that if
**  CC_terminateifokay() does not return if it is okay to terminate --
**  it takes care of calling CC_select_cleanup() by itself.
*/

int CC_choose(id)
int id;
{
	if (CC_achoice)
		return CC_achoice;
	if (CC_ichoice) {
		CC_select_cleanup();
		return CC_ichoice;
	}
	if (CC_terminate_exists)
		CC_terminateifokay();
	if (CC_ochoice) {
		CC_select_cleanup();
		return CC_ochoice;
	}
	if ( !(CC_accept_exists || CC_terminate_exists || CC_dchoice) )
		CC_perror("no valid choices in select");
	return CC_select(id);
}

/*
**  This routine is used during the alternative evaluation pass.
**  If there are clients waiting on this accept alternative,
**  it is chosen immediately, and selectcase is cleaned out
**  before jumping to the switch statement. If there are no clients
**  waiting on this accept, then the case label is stored in selectcase
**  and the next alternative is examined.
*/

int CC_tryaccept(n,casenum)
int n;
int casenum;
{
	int j,empty;

	if (empty=!CC_queuecount(me,n))
		me->selectcase[n] = casenum;
	else {
		for (j=0;j<CC_numtrans;j++)
			me->selectcase[j] = 0;
		CC_achoice = casenum;
	}
	return !empty;
}

unsigned DD_acceptId;		/* to pass the id for accept postlude */

CC_accept(n,local_CC_tblock_ptr,by,accept_in_select, id)
int n;
char **local_CC_tblock_ptr;
int by;
int accept_in_select;
unsigned id;
{
	DD_acceptId = id;
	the_transaction = n;
	CC_tblock_ptr = local_CC_tblock_ptr;
	if (!accept_in_select) {
		CC_checkout(mypid);
		s_lock(me->queuelock);
		client_on_queue = (CC_queuecount(me,n) > 0);
		if (!client_on_queue) {
			me->selectedtrans = NOTRANS;
			me->selectcase[n] = 1;
			s_unlock(me->queuelock);
			CC_block(DD_WAITaCCEPT, id);
			me->selectcase[n] = 0;
		}
	}
	if (client_on_queue) {
		me->newclient = CC_topqueue(n);
		if (by) {
			*CC_tblock_ptr = me->newclient->tblock;
			first = 1;
			return;
		}
		CC_dequeue(n,me->newclient);
		s_unlock(me->queuelock);
	}
	CC_accept_postlude();
}

/*
**  As explained above, CC_acceptby() is used to implement non-fifo
**  acceptance policies. Note the use of static variables to achieve
**  a coroutine-like behavior.
*/

int CC_acceptby(ae)
double ae;
{
	static task candidate,winner;
	static double lowest;

	/*
	**  Return if there was only one client, who we have already accepted.
	*/
	if (!client_on_queue)
		return 0;
	/*
	**  Update lowest and winner.
	*/
	if (first) {
		first = 0;
		lowest = ae;
		winner = me->newclient;
		candidate = winner;
	} else if (ae<lowest) {
		lowest = ae;
		winner = candidate;
	}
	/*
	**  If there are any more tasks, do another evaluation;
	**  otherwise, pick a winner, unlock the queue lock, and
	**  stop evaluating.
	*/
	if ((candidate = CC_queuesucc(the_transaction,candidate)) != nulltask) {
		*CC_tblock_ptr = candidate->tblock;
		return 1;
	} else {
		CC_dequeue(the_transaction,winner);
		me->newclient = winner;
		s_unlock(me->queuelock);
		CC_accept_postlude();
		return 0;
	}
}

/*
**  This is the clean up routine called after a client has been accepted,
**  called either from CC_accept() or CC_acceptby(). It tells the client
**  that he has been accepted, if the client is executing a timed transaction
**  call. It then makes the newly accepted client the current client, and
**  pushes the client onto a client stack which is used by CC_treturn().
*/
CC_accept_postlude()
{
	if (me->newclient->waiting)
		CC_unblock(me->newclient, DD_ACCEPT, DD_acceptId);
	else 
		DD_event2(DD_ACCEPT, (unsigned)me->newclient->unixpid, DD_acceptId);
	me->newclient->prevclient = me->client;
	me->client = me->newclient;
	*CC_tblock_ptr = me->client->tblock;
	CC_checkin(me);
}

/*
**  The treturn statement is used inside an accept statement to transfer
**  control out of the innermost accept statement and to release the client
**  from the rendezvous, supplying him with a return value in his tblock
**  if one is expected. It is translated into an assignment to the
**  the value field of the suitably casted tblock, followed by a goto
**  to a CC_treturn() call which is the last statement in the compound
**  statement generated for the accept statement. (This placement ensures
**  that a treturn is implicitly executed by "falling off" the accept
**  statement.) CC_treturn() thus deals with waking up the blocked client,
**  and popping the client stack so that the next CC_treturn will wake up
**  the proper client.
*/
CC_treturn(id)
unsigned id;
{
	me->client->tsuccess = 1;
	CC_unblock(me->client, DD_TRETURN, id);
	me->client = me->client->prevclient;
}

/*
**  CC_select is called from within CC_choose() when no choices are immediately
**  availible. 
*/
int CC_select(id)
unsigned id;
{
	int j,choice;
	/*
	**  First prepare to handle signals in message mode.
	**  This allows us to determine the type of message that we are receiving.
	*/
	CC_message_mode();
	/*
	**  Invite messages of type TERMINATE...
	*/
	if (CC_terminate_exists)
		CC_preparedtoterm();
	/*
	**  and ACCEPT...
	*/
	me->selectedtrans = NOTRANS;
	s_unlock(me->queuelock);
	/*
	**  and DELAY.
	*/
	if (CC_dchoice)
		CC_settimer(CC_bestdelay);
	/*
	**  Sit and wait for a message.
	*/
	CC_block(DD_WAITsELECT, id);
	/*
	**  Either by explicit protocol or by the nature of the
	**  message senders, we will recieve at most one message
	**  of any particular type.  However, different types of
	**  messages will not block each other and thus we must
	**  flush spurious messages. CC_flush_TERMINATE() and
	**  CC_flush_ACCEPT() will return a 0 if they actually
	**  found a message, which in some contexts means that
	**  they should still be acted upon. There is an implicit
	**  priority attached to messages, with TERMINATE as the
	**  highest priority message,
	**  
	**  There is an implicit priority attached to messages due
	**  to the order in which we are willing to flush them.
	**  TERMINATE messages have the highest priority, so that
	**  we can be sure that the ready-to-terminate conditions
	**  are not violated. (See Qdeath.c for details.) Thus if
	**  we received a TERMINATE message, or if we received
	**  some other message, but a TERMINATE message is sent
	**  before we can tell our siblings via
	**  CC_flush_TERMINATE() that we are no longer prepared to
	**  terminate, then we flush all other messages and act
	**  upon the TERMINATE message.
	*/
	if (message==TERMINATE || (CC_terminate_exists && !CC_flush_TERMINATE())) {
		CC_flush_DELAY();
		CC_flush_ACCEPT();
		CC_block_mode(); /* need to do this for selfabort to be called */
		CC_select_cleanup();
		c_abort(mypid);
	}
	/*
	**  Otherwise, the only messages that have been or will be sent are DELAY
	**  and ACCEPT. If we received an ACCEPT message, or an ACCEPT message is sent 
	**  before we can mark ourselves as timed out (via CC_flush_ACCEPT), then we
	**  flush any DELAY messages and prepare to accept the client.
	*/
	if (message==ACCEPT || (CC_achoice && !CC_flush_ACCEPT())) {
		CC_flush_DELAY();
		choice = me->selectcase[me->selectedtrans];
		for (j=0;j<CC_numtrans;j++)
			me->selectcase[j] = 0;
		client_on_queue = 0;
		/*
		**  Otherwise, we must have received a DELAY message...
		*/
	} else {
		CC_select_cleanup();
		choice = CC_dchoice;
	}
	/*
	**  Return to using signals for blocking and unblocking processes,
	**  ie as a simple form of semaphore mechanism (with queues one process big).
	*/
	CC_block_mode();
	/*
	**  Return our choice for the proper case label.
	*/
	return choice;
}
