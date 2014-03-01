/* CC_fork.c -- concurrent c intrinsic functions, plus CC_fork()  */

#include <stdio.h>
#include "CC_include.h"
#include "DD_kernel.h"

extern int DD_me;


#define s_destroylock(l) shfree(l)

/*
 *  Priorities are currently ignored. Problems to be addressed:
 *	(1) What should the default priority be?
 *	(2) How do we allow users to better their priorities?
 *  A straightforward implementation would make starting priority zero,
 *  and users could only worsen their priorities. Maybe some smarter person
 *  can think of something better. Or maybe the same sort of person can
 *  add something so that we can bump our priorities up (to 0).
 */

int CC_core_dump_on_error=0;

c_core_dump_on_error(whether)
int whether;
{
	CC_core_dump_on_error=whether;
}

/*
** Common shared memory allocator
*/
char *
CC_shmalloc(bytes)
int bytes;
{
	char *result;
	result = (char *)shmalloc(bytes);
	if ( result==(char *)0 ) {
		CC_perror("Shared memory allocation of %d bytes failed, aborting",bytes);
		perror("shmalloc");
		exit(1);
	}
	return result;
}
CC_perror(s,p0,p1,p2,p3,p4)
char *s;
{
	fprintf(stderr,"###CC runtime error, PID %d (%s): ",getpid(),me->name);
	fprintf(stderr,s,p0,p1,p2,p3,p4);
	fprintf(stderr,"\n");
	if (CC_core_dump_on_error) {
		printf("*** core dump follows ***\n");
		kill(getpid(),3);
	}
}

lock makelock()
{
	lock templock;

	templock=(lock)CC_shmalloc(sizeof(*templock));
	s_init_lock(templock);
	s_unlock(templock);
	return templock;
}

CC_fork(name,number_of_transactions,prio, id)
char *name;
int number_of_transactions;
int prio;
unsigned id;
{
	pid newborn;
	int j,unixpid;

	newborn = (pid)CC_shmalloc(sizeof(*newborn));
	newborn->life=makelock();
	s_lock(pp->statelock);
	s_lock(me->statelock);
	if ((unixpid=fork())== -1) {
		CC_perror("failed fork");
		s_unlock(pp->statelock);
		s_unlock(me->statelock);
		CC_childpid=nullpid;
		s_destroylock(newborn->life);
		shfree(newborn);
		return 1;
	} else if (unixpid) {
		CC_childpid=newborn;
		CC_block(DD_WAITbORN, id);
		return 1;
	} else {
		mypid=newborn;
		gp=pp;
		pp=me;
		me=(task)CC_shmalloc(sizeof(*me));
		mypid->task=me;
		numtrans=number_of_transactions;

		me->name = name;
		me->queuelock=makelock();
		me->client=nulltask;
		me->tblock=(char *)CC_shmalloc(CC_maxtblocksize);
		me->selectcase=(int *)CC_shmalloc(numtrans * sizeof(int));

		me->unixpid=DD_me=getpid();
		me->statelock=makelock();
		me->firstchild=nulltask;
		me->completed=0;
		me->aborted=0;
		me->choseterm=0;
		me->prepared=0;
		me->numloans=0;
		me->numchildren=0;
		me->numprepared=0;
		me->numgrandchildren=0;

		me->tqueue=(queue *)CC_shmalloc(numtrans * sizeof(queue));

		for (j=0;j<numtrans;j++) {
			me->tqueue[j].count=0;
			me->selectcase[j]=0;
		}

		gp->numgrandchildren++;
		if (pp->numchildren++) {
			pp->firstchild->prevsibling=me;
			me->nextsibling=pp->firstchild;
		} else
			me->nextsibling=nulltask;
		pp->firstchild=me;

		mytblock=me->tblock;
		s_unlock(gp->statelock);
		s_unlock(pp->statelock);
		CC_unblock(pp, DD_ISbORN, id);
		return 0;
	}
}

CC_make_elder_tasks()
{

	me=(task)CC_shmalloc(sizeof(*me));
	me->statelock=makelock();
	me->firstchild=nulltask;
	me->nextsibling=nulltask;
	me->completed=0;
	me->numloans=0;
	me->numchildren=0;
	me->numprepared=0;
	me->numgrandchildren=0;
	me->unixpid=DD_me=getpid();
	me->tblock=(char *)CC_shmalloc(CC_maxtblocksize);
	mytblock=me->tblock;

	pp=(task)CC_shmalloc(sizeof(*pp));
	pp->statelock=makelock();
	pp->firstchild=me;
	pp->nextsibling=nulltask;
	pp->completed=0;
	pp->numloans=0;
	pp->numchildren=1;
	pp->numprepared=0;
	pp->numgrandchildren=0;

	mypid=(pid)CC_shmalloc(sizeof(*main_pid));
	main_task=me;
	main_pid=mypid;
	main_pid->task=main_task;
	main_pid->life=makelock();
}

CC_free(t)
task t;
{
	s_destroylock(t->statelock);
	s_destroylock(t->queuelock);
	shfree(t->tblock);
	shfree(t->selectcase);
	shfree(t->tqueue);
	shfree(t);
}

int c_unixpid(p)
pid p;
{
	return p->task->unixpid; /* NOT very safe! */
}

int c_getpriority(p,prio)
pid p;
int prio;
{
	/* not implemented */
	return 0;
}

c_changepriority(p,prio)
pid p;
int prio;
{
	/* not implemented */
}

c_associate()
{
	/* not implemented */
}

/*
int c_count(p,n)
pid p;
int n;
{
    task t;
    int count;

    if ((t=CC_checkout(p)) == nulltask)
	return 0;
    s_lock(t->queuelock);
    count=CC_queuecount(t,n);
    s_unlock(t->queuelock);
    CC_checkin(t);
    return count;
}

int c_active(p)
pid p;
{
    task t;
    int active;

    if ((t=CC_checkout(p)) == nulltask)
	return 0;
    s_lock(t->statelock);
    active= !t->aborted && !t->completed;
    s_unlock(t->statelock);
    CC_checkin(t);
    return active;
}

int c_completed(p)
pid p;
{
    task t;
    int completed;

    if ((t=CC_checkout(p)) == nulltask)
	return 0;
    s_lock(t->statelock);
    completed= !t->aborted && t->completed;
    s_unlock(t->statelock);
    CC_checkin(t);
    return completed;
}

int c_invalid(p)
pid p;
{
    task t;
    int terminated;

    if ((t=CC_checkout(p)) == nulltask)
	return 1;
    s_lock(t->statelock);
    terminated= t->aborted;
    s_unlock(t->statelock);
    CC_checkin(t);
    return terminated;
}
*/

pid c_mypid()
{
	return mypid;
}
