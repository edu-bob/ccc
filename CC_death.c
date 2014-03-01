/*  Qdeath.c -- functions concerning aborts, termination, and completion. */

#include "CC_include.h"


/*
 *  should I use a variant of memory marking here?
 *  we should probably do something like that because c_invalid() uses this
 *  routine and would die on a pid value like ((pid)17), which a check
 *  for validity should not do.
 */

/*
 *  if checking out self and already aborted, then we wait here to die.
 *  ((I hope [not checked out] => [in block mode]))!
 *  otherwise nulltask on checking out already aborted pid.
 */
task CC_checkout(p)
pid p;
{
    task t;

    if (p==nullpid)
	return nulltask;
    s_lock(p->life);
    t=p->task;
    if (!t) {
	s_unlock(p->life);
	if (p==mypid)
	    CC_block(DD_WAITaBORT, 0); /* wait to be aborted, *** no id */
	else
	    return nulltask;
    }
    s_lock(t->statelock);
    if (t->completed || t->aborted)
	t=nulltask;
    else
	t->numloans++;
    s_unlock(p->task->statelock);
    s_unlock(p->life);
    if (!t && p==mypid)
	CC_block(DD_WAITaBORT, 0); /* wait to be aborted */
    else
	return t;
}

CC_checkin(t)
task t;
{
    int shouldkill;

    s_lock(t->statelock);
    shouldkill=(!--t->numloans && t->aborted);
    s_unlock(t->statelock);
    if (!shouldkill)
	return;
    else if (t==me)
	CC_selfabort();
    else
	CC_kill(t, DD_LASTuSERkILL, 0);
}

CC_children_select_terminate(t)
task t;
{
    task ch;

    if (t->numchildren)
	for (ch=t->firstchild;ch!=nulltask;ch=ch->nextsibling) {
	    ch->choseterm=1;
	    if (ch != me)
		CC_kill(ch, DD_CHILDkILL, 0);
	}
}

CC_terminateifokay()
{
    int selected;

    s_lock(pp->statelock);
    s_lock(me->statelock);
    if (selected=(pp->completed && pp->numchildren==pp->numprepared+1
    && !pp->numgrandchildren)) {
	me->prepared=1;
	pp->numprepared++;
	CC_children_select_terminate(pp);
    }
    s_unlock(me->statelock);
    s_unlock(pp->statelock);
    if (selected) {
	CC_select_cleanup();
	c_abort(mypid);
	CC_block(DD_SELECTtERM, 0);
    }
}

/*
 *  CC_preparedtoterm() is called only by CC_select(), while in message mode.
 *  Thus the CC_kill() sends a message rather than actually forcing an abort.
 */
CC_preparedtoterm()
{
    int selected;

    s_lock(pp->statelock);
    s_lock(me->statelock);
    me->prepared=1;
    pp->numprepared++;
    if (selected=(pp->completed && pp->numchildren==pp->numprepared
    && !pp->numgrandchildren))
	CC_children_select_terminate(pp);
    s_unlock(me->statelock);
    s_unlock(pp->statelock);
    if (selected)
	CC_kill(me, DD_SELECTtERM, 0);
}

/*
 *  CC_selfabort() is called by a process when it receives an abort signal.
 *  At this point I am all checked in.
 *
 *  things like the following indicate that there should be an anonymous
 *  parent ... and grandparent!... for "main" (the initial process).
 */


CC_selfabort()
{
    task ch;

    if (me == main_task) {
	s_lock(me->statelock);
	if (me->numchildren) {	/* abort any children */
	    for (ch=me->firstchild;ch!=nulltask;ch=ch->nextsibling)
		CC_abort(ch);
	    CC_block(DD_WAITcHILDdIE, 0);/* wait for last child to die, *** no id */
	}
	s_unlock(me->statelock);
	DD_event( DD_EXITING, 0 );	/*** no id */
	exit();
    }
    CC_flushqueues();
    CC_flushclients();
    s_lock(pp->statelock);
    s_lock(me->statelock);
    me->completed=1;		/* force completion */
    if (me != main_task)
	s_unlock(pp->statelock);
    if (me->numchildren) {	/* abort any children */
	for (ch=me->firstchild;ch!=nulltask;ch=ch->nextsibling)
	    CC_abort(ch);
	CC_block(DD_WAITcHILDdIE, 0);	/* wait for last child to die, *** no id */
    }
    s_unlock(me->statelock);
    s_lock(gp->statelock);	/* order is important to prevent deadlock */
    s_lock(pp->statelock);
    s_lock(me->statelock);
    
    /* remove ourselves from child list. */
    if (pp->firstchild==me)
	pp->firstchild=me->nextsibling;
    else
	me->prevsibling->nextsibling=me->nextsibling;

    /* detach our task from our pid */
    mypid->task=nulltask;

    /* update various counts */
    gp->numgrandchildren--;
    pp->numchildren--;
    if (me->choseterm)
	pp->numprepared--;

    /* inform anyone who is interested of our death and release locks */
    if ((pp->completed || pp->sickofkids) && !pp->numchildren)
	CC_unblock(pp, DD_LASTcHILD, 0);	/*** no id */
    else if (pp->completed && pp->numchildren==pp->numprepared
	&& !pp->numgrandchildren)
	CC_children_select_terminate(pp);
    s_unlock(me->statelock);
    s_unlock(pp->statelock);
    s_unlock(gp->statelock);

    /* deallocate storage bound to our task and exit */
    CC_free(me);
    DD_event( DD_EXITING, 0 );	/*** no id */
    exit();
}

CC_flushclients()
{
    task t;

    for (t=me->client;t!=nulltask;t=t->prevclient) {
	t->tsuccess=0;
	CC_unblock(t, DD_IMdYING, 0);	/*** no id */
    }
}

c_abort(p)
pid p;
{
    task t;
    int shouldkill;

    s_lock(p->life);
    t=p->task;
    if (!t) {
	s_unlock(p->life);
	return;
    }
    s_lock(t->statelock);
    shouldkill=(!t->aborted && !t->numloans);
    t->aborted=1;
    s_unlock(t->statelock);
    s_unlock(p->life);
    if (shouldkill)
	if (t==me)
	    CC_selfabort();
	else
	    CC_kill(t, DD_ABORTINGyOU, 0);
}

CC_abort(t)
task t;
{
    int shouldkill;

    s_lock(t->statelock);
    shouldkill=(!t->aborted && !t->numloans);
    t->aborted=1;
    s_unlock(t->statelock);
    if (shouldkill)
	if (t==me)
	    CC_selfabort();
	else
	    CC_kill(t, DD_CHILDkILL, 0);
}

CC_complete()
{
    /* shouldn't we use checkout/checkin? */

    s_lock(pp->statelock);
    s_lock(me->statelock);
    me->completed=1;
    if (me->numchildren) {	/* if there are any children, notify them */
	if (me->numchildren==me->numprepared && !me->numgrandchildren)
	    CC_children_select_terminate(me);
	s_unlock(me->statelock);
	s_unlock(pp->statelock);
	CC_block(DD_WAITcHILDdIE, 0);  /* wait for last child to say all are dead */
    } else {
	s_unlock(me->statelock);
	s_unlock(pp->statelock);
    }
    c_abort(mypid);		/* selfabort() when all checked out */
    CC_block(DD_WAITaBORT, 0); 	/* wait to be aborted */
}

c_wait()
{
    CC_checkout(mypid);
    s_lock(pp->statelock);
    s_lock(me->statelock);
    if (me->numchildren) {	/* if there are any children */
	me->sickofkids=1;
	s_unlock(me->statelock);
	s_unlock(pp->statelock);
	CC_block(DD_WAITcHILDdIE, 0);  /* wait for last child to say all are dead */
	me->sickofkids=0;
    } else {
	s_unlock(me->statelock);
	s_unlock(pp->statelock);
    }
    CC_checkin(me);
}
