/* Qqueue.c -- transaction queue handling facilities */

#include "CC_include.h"

static queue_remove(qp,t)
queue *qp;
task t;
{
    if (--qp->count)
	if (qp->head == t)
	    qp->head=t->next;
	else if (qp->tail == t)
	    qp->tail=t->prev;
	else {
	    t->next->prev=t->prev;
	    t->prev->next=t->next;
	}
    t->next=t->prev;
}

task CC_queuesucc(n,t)
int n;
task t;
{
    if (t==me->tqueue[n].tail)
	return nulltask;
    else
	return t->next;
}

CC_dequeue(n,t)
int n;
task t;
{
    queue_remove(&me->tqueue[n],t);
}

int CC_remqueue(t,n)
task t;
int n;
{
    int success;

    if (success=(me->next!=me->prev))
	queue_remove(&t->tqueue[n],me);
    return success;
}

task CC_topqueue(n)
int n;
{
    return me->tqueue[n].head;
}	

CC_enqueue(t,n)
task t;
int n;
{
    queue *qp= &t->tqueue[n];

    if (qp->count++) {
	qp->tail->next=me;
	me->prev = qp->tail;
    } else {
	qp->head=me;
	me->prev=0;
    }
    qp->tail=me;
    me->next=me;	/* we want me->next != me->prev */
}

CC_flushqueues()
{
    task t,tail;
    int n;
    queue *qp;

    s_lock(me->queuelock);
    for (n=CC_numtrans-1;n>=0;n--) {
	qp= &me->tqueue[n];
	if (!qp->count)
	    continue;
	qp->count=0;
	tail=qp->tail;
	for (t=qp->head;t!=nulltask;t=(t==tail ? nulltask : t->next)) {
	    t->tsuccess=0;
	    t->prev=t->next;
	    if (t->waiting)
		CC_unblock(t, DD_IMdYING, 0);
	    CC_unblock(t, DD_IMdYING, 0);
	}
    }
    s_unlock(me->queuelock);
}

int CC_queuecount(t,n)
task t;
int n;
{
    return t->tqueue[n].count;
}	
