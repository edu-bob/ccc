/*---------------------------------------------------------------------------
 * William L. Lynch 		Fri Jun 27 10:49:09 1986
 *---------------------------------------------------------------------------
 */
/*#define Debug
#define Debug2*/

#include <stdio.h>
#include <signal.h>
#include "DD_semaphore.h"
#include "bool.h"

#define SIGSEM	SIGUSR1
#define SIGALL 0xffffffff

#define listAdd(x, el) \
		if ( (x)->head == NULL )  (x)->head = (el);\
		else (x)->tail->next = (el);\
		(x)->tail = (el)

#define listRm(x, el)\
		(el) = (x)->head;\
		if ( (x)->head == (x)->tail )	(x)->head = (x)->tail = NULL;\
		else (x)->head = (x)->head->next
 

#define shInstances(n, t)	(t *)shmalloc((n) * sizeof (t))
#define shInstance(t)		shInstances(1, t)


static bool sigsem = FALSE;

DD_sem_sig( sig )
int sig;
{
#ifdef Debug
	DD_debug_printf("signalled %d\n", getpid());
#endif
	sigsem = TRUE;
}

DD_semaphore_init()
{
	signal(SIGSEM, DD_sem_sig);
}


semaphore
DD_sem_create( initialValue )
int initialValue;
{
	semaphore sem;
	
	sem = shInstance(Semaphore);
	sem->count = initialValue;
	sem->lock = shInstance(slock_t);
	s_init_lock(sem->lock); s_unlock(sem->lock);
	sem->head = sem->tail = NULL;
#ifdef Debug
	DD_debug_printf("semcreate 0x%x\n", sem);
#endif
	return sem;
}


void
DD_sem_ask( sem )
semaphore sem;
{
	static waiting *wait;
	int mask;
	
#ifdef Debug
	DD_debug_printf("semask 0x%x\n", sem);
#endif
	mask = sigblock(sigmask(SIGSEM));
	s_lock(sem->lock);
	if ( --sem->count < 0 ) {
		wait = shInstance(waiting);
		wait->pid = getpid();
		wait->next = NULL;
		
		listAdd(sem, wait);
		
		s_unlock(sem->lock);

		sigsem = FALSE;
		while ( !sigsem )
			sigpause(mask & ~sigmask(SIGSEM));
/*SIGALL & ~(sigmask(SIGSEM) | sigmask(SIGINT) | sigmask(SIGSEGV)));*/

		shfree(wait);
	} else
		s_unlock(sem->lock);
}



void
DD_sem_tell( sem )
semaphore sem;
{
	waiting *wait;

#ifdef Debug
	DD_debug_printf("semtell 0x%x\n", sem);
#endif
	s_lock(sem->lock);
	if ( sem->count++ < 0 ) {
		listRm(sem, wait);
		kill(wait->pid, SIGSEM);
#ifdef Debug2
	DD_debug_printf("signalling %d\n", wait->pid);
#endif
	}
	s_unlock(sem->lock);
}


void
DD_sem_tell_num( sem, number )
semaphore sem;
int number;
{
	int i;
	
	for ( i=0; i < number; i++ )
		DD_sem_tell(sem);
}



