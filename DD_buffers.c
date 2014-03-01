/*---------------------------------------------------------------------------
 * This codes provides communication between the user processes and the
 * debug process by means of a circulating queue of buffers, locked with
   semaphores.  Transmit order is preserved in terms of buffer request
   order.
 *  
   The buffers are shuffled in the queues to try
 * to prevent mass write-flushing by the snoopy caches.
 * 
 * If there are fewer buffers than processes, than there can be a slight
   race condition as the empty pointer circulates completely around the
   buffer and two processes are accessing the same semaphore.
 *  
 * William L. Lynch		Thu Jul 10 13:11:13 1986
 *---------------------------------------------------------------------------
 */

/*#define Debug*/

#include "DD_include.h"
#include "bool.h"
#include "DD_buffers.h"
#include "DD_semaphore.h"

#define BUFFERS		50

#ifdef Debug
#include <stdio.h>
#endif
 

#define shInstances(n, t)	(t *)shmalloc((n) * sizeof (t))
#define shInstance(t)		shInstances(1, t)

#define circle(bufInt)	((bufInt)%BUFFERS)
#define pred(bufInt)	((bufInt) == 0 ? BUFFERS-1 : (bufInt)-1)
#define succ(bufInt)	((bufInt) == BUFFERS-1 ? 0 : (bufInt)+1)



static shared struct bufTag {
	buffer *buf;
	semaphore fullSem, emptySem;
} buffers[BUFFERS];

static slock_t	*emptyLock, *fullLock;
static shared int full = 0, empty = 0;	/* points to next available buffer */



DD_buffers_init()
{
	int i, start, bufr;
	
	DD_semaphore_init();
	
	emptyLock = shInstance(slock_t);s_init_lock(emptyLock); s_unlock(emptyLock);
	fullLock = shInstance(slock_t); s_init_lock(fullLock);  s_unlock(fullLock);

	for (i=0, bufr=start=0; i < BUFFERS; i++) {
		buffers[bufr].buf = shInstance(buffer);
		buffers[bufr].buf->bufNum = bufr;
		buffers[bufr].fullSem = DD_sem_create(0);
		buffers[bufr].emptySem = DD_sem_create(1);
		bufr = circle(bufr + 7);
		if ( bufr >= BUFFERS )
			bufr = ++start;
	}
}

DD_buffers_forceEnd()
{
	buffers[empty].buf->event = DD_COMMUN;
	DD_buffers_addFull(buffers[empty].buf);
}


DD_buffers_end()
{
	int i;
	shfree(emptyLock);
	shfree(fullLock);

	for ( i=0; i < BUFFERS; i++ ) {
		shfree(buffers[i].fullSem);
		shfree(buffers[i].emptySem);
		shfree(buffers[i].buf);
	}
}



buffer *
DD_buffers_getEmpty()
{
	int myEmpty;

	s_lock(emptyLock);
	myEmpty = empty;
	empty = succ(empty);
	s_unlock(emptyLock);

	DD_sem_ask(buffers[myEmpty].emptySem);

	return buffers[myEmpty].buf;
}



buffer *
DD_buffers_getFull()
{
	int myFull;

	s_lock(fullLock);
	myFull = full;
	full = succ(full);
	s_unlock(fullLock);

	DD_sem_ask(buffers[myFull].fullSem);

	return buffers[myFull].buf;
}


void
DD_buffers_addFull( buf )
buffer *buf;
{
	DD_sem_tell(buffers[buf->bufNum].fullSem);
}

void
DD_buffers_addEmpty( buf )
buffer *buf;
{
	DD_sem_tell(buffers[buf->bufNum].emptySem);
}


char *
DD_buffers_getTblock(size)
int size;
{
	return shmalloc(size);		/* this may be inefficient */
}



DD_buffers_freeTblock(tp)
char *tp;
{
	shfree(tp);
}

/*
static
#include "cdb/eventString.h"

DD_buffers_dump(s)
char *s;
{
	int i;
	waiting *wait;

	s_lock(fullLock); s_lock(emptyLock);
	for ( i=0; i<BUFFERS; i++ ) {
#define BP buffers[i]
	DD_debug_printf("%d told %d, full%d, time %d, event %s, from %d to %d id %d\n", 
		i, BP.told, BP.full, BP.buf->time.tv_usec,
		eventString[(int)BP.buf->event], BP.buf->fromId, BP.buf->toId,
		BP.buf->id);
	}
	DD_debug_printf("%s full %d, empty %d, count %d, %d.\n", s, full, empty, fullSem->count, emptySem->count);
	for ( wait = emptySem->head; wait != NULL; wait= wait->next ) {
		DD_debug_printf("waiting %d\n", wait->pid);
	}
	s_unlock(fullLock); s_unlock(emptyLock);
}

*/