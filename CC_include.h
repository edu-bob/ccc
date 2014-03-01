/* CC_include.h -- include file for Concurrent C kernel modules */

/*
 *  Definitions for using the parallel processing library locks, etc.
 */

#include <parallel/parallel.h>
typedef slock_t *lock;
/* the following is much faster.  S_LOCK would add much to code size */
#define s_unlock(l) S_UNLOCK(l)   

/*
 *  Some definitions which allow us to drop the CC_ prefix that is
 *  used to prevent clashes with user's external symbols.
 */
#define	me		CC_me
#define mypid		CC_mypid
#define pp		CC_myparent
#define gp		CC_mygrandparent
#define mytblock	CC_mytblock
#define	message		CC_message
#define numtrans	CC_numtrans
#define signal_mode	CC_signal_mode

/*
 *  Definitions of null objects.
 */
#define nullpid		0
#define nulltask	0

/*
 *  Definition of signals used by synchronization primitives in terms
 *  of standard unix signals.
 */
#define SIGUNBLOCK	SIGHUP
#define SIGTERMINATE	SIGTERM

/*
 *  Definitions for the two-mode signal handling capability in the CC kernel.
 *  Signal_mode is one of BLOCK_MODE or MESSAGE_MODE. The definitions
 *  following are codes for messages when in MESSAGE_MODE.
 */
#define BLOCK_MODE	0
#define MESSAGE_MODE	1

#define DELAY		0
#define	TERMINATE	1
#define	RENDEZVOUS	2
#define TCALL		RENDEZVOUS
#define ACCEPT		RENDEZVOUS

/*
 *  For use with "selectedtrans" field. NOTRANS means that no transaction has yet
 *  claimed the right to wake up the sleeping server. Since transaction
 *  numbers start at zero, when a client claims wakeup rights by setting
 *  "selectedtrans" to his transaction number, it will no longer be NOTRANS.
 *  DUMMYTRANS is used by flush_ACCEPT() to fool potential clients into
 *  thinking that someone else has already woken up the server.
 */
#define	NOTRANS		(-1)
#define	DUMMYTRANS	(-2)

/*
 *  There are several places when a process needs to know how
 *  many transactions there are in its process type. This value is
 *  availible in the (unshared) global variable "numtrans".
 */
int numtrans;

/*
** type definition for boolean data type
*/

#include "bool.h"

/*
 *  Forward declaration of task.
 *  (The following structure declarations are mutually recursive.)
 */
typedef struct task_struct *task;

/*
 *  The type queue is used to hold waiting transaction calls.
 *  The only routines that use the actual queue fields are in CC_queue.c.
 */
typedef struct {
    int count;
    task head,tail;
} queue;

/*
 *  A pid corresponds to a process variable in Concurrent C.
 *  All the interesting stuff is kept in the field "task".
 *  When a process is terminated, this field is set to nulltask.
 *  The lock "life" controls access to task (ie, anyone accessing
 *  task must first do a lock on "task".) Deadlock is prevented
 *  through the use of CC_checkout() and CC_checkin(), which
 *  release the lock after marking the fact that the pid is in use.
 */
typedef struct {
    task	task;
    lock	life;
} *pid;

/*
 *  This is the task structure. This space is released when a task terminates.
 */
struct task_struct {
/*
 *  The first four fields are for the transaction queues, which clients wait on
 *  when they cannot be accepted immediately. One queue is provided for
 *  each transaction; thus there are "numtrans" queues. One lock is
 *  provided for all queues of a task; it might be better to have seperate
 *  locks for each queue as well, but there are some operations,
 *  mostly involving select statements with multiple accepts, which
 *  require varying degrees of mutual exclusion among several queues.
 *
 *  An important observation about the rendezvous mechanism is that
 *  a process can block waiting for no more than one server to accept him.
 *  This allows the task itself to be used as a queue element, instead of a
 *  structure with a task field. Thus the fields "next" and "prev" point
 *  to the tasks ahead and behind the given task, if it is on a transaction
 *  queue. When a task has been removed from a queue, "next" and "prev"
 *  are set equal to each other to mark the fact that they are not on a queue.
 */
    queue	*tqueue;	/* array[numtrans] of queues for blocked clients */
    lock	queuelock;	/* mutex for queue operations */
    task	next;		/* my sucessor in the queue I'm in */
    task	prev;		/* my predecessor */

/*
 *  The next eight fields are used for transaction call synchronization
 *  and communication. Mutual exclusion to many of these fields is
 *  provided via "queuelock" (above). When a client makes a transaction call,
 *  it first places its parameters in its transaction block ("tblock"), which
 *  the server can access when it has accepted the client. The client
 *  then checks to see if the server is sleeping waiting for a transaction
 *  call. A server indicates that it is waiting for a transaction
 *  call on transaction "n" by setting "selectedtrans" to NOTRANS and
 *  "selectcase[n]" to some nonzero value. When the server is exeuting a select
 *  statement, this nonzero value is the case label for the accept alternative
 *  corresponding to that transaction, and several elements of "selectcase"
 *  may be so used. If the server is waiting, the client sets the server's
 *  "newclient" field to point to the client, and wakes the server up.
 *  If the server is not waiting, the client places himself on a queue;
 *  if the client is executing a timed transaction call, he will set
 *  "waiting" true, which tells the server to signal the client when he has been
 *  accepted. The server sets the clients "tsuccess" field true on successful
 *  completion of the transaction call, or false on abnormal completion,
 *  and supplies the return value for the transaction call (if there is one)
 *  through the client's transaction block. The "client" field in the
 *  server names the most recently accepted task; the "prevclient"
 *  field in a client task names the task accepted just before the server
 *  accepted the client task in question. (This is to allow a server to
 *  handle nested accept statements, which requires that the server remember
 *  several clients at once.)
 */
    task	newclient;	/* task to be accepted when server is woken up */
    task	client;		/* last accepted client */
    task	prevclient;	/* task accepted before my by my server */
    bool	waiting;	/* signal me when you accept so I won't time out */
    bool	tsuccess;	/* successful transaction call if true */
    char	*tblock;	/* where I place my parameters when I call server */
    int		selectedtrans;	/* which transaction was the server woken up for? */
    int		*selectcase;	/* array[numtrans], see big comment above */

/*
 *  These next fields are used to synchronize the various forms of
 *  process death, including selecting a terminate alternative, being aborted,
 *  and terminating after completion when all children have terminated.
 *  The lock "statelock" protects most of these fields. The field "unixpid"
 *  stores the unix pid of the actual process in which the task is running.
 *  "Firstchild", "nextsibling", and "prevsibling" are used to represent
 *  the family tree. (A task's parent and grandparent are accessible through
 *  the global variables "pp" and "gp", respectively.) "Completed" tells
 *  whether a task has completed; if "aborted" is true, then an abort
 *  was attempted on the task but it was checked out at the time
 *  (but when it is all checked in, it will be aborted.) When "choseterm"
 *  is true, a task must terminate; "prepared" means that a process is
 *  in a select statement with a terminate alternative and is ready
 *  to choose that alternative if the termination conditions become true.
 *  "Numloans" is used by the checkin/checkout routines. "Numprepared"
 *  holds the number of children who have "prepared" set true; and
 *  "numchildren" and "numgrandchildren" are self-explanatory.
 */
    int		unixpid;	/* unix pid, for kill & other unix calls */
    lock	statelock;	/* lock for most of following fields */
    task	firstchild;	/* first task in my child list */
    task	nextsibling;	/* next task in parent's child list */
    task	prevsibling;	/* previous task in parent's child list */
    bool	completed;	/* true if task is not active */
    bool	aborted;	/* true if an abort is pending */
    bool	choseterm;	/* must choose termination, can't back out */
    bool	prepared;	/* willing to select terminate alternative */
    bool	sickofkids;	/* used for c_wait; last kid must wake me */
    int		numloans;	/* number of tasks who have checked me out */
    int		numprepared;	/* number of children prepared to terminate */
    int		numchildren;	/* number of children */
    int		numgrandchildren; /* number of grandchildren */
/*
 * These remaining fields are just used for debugging.  "name" is the
 * process name.
 */
    char	*name;		/* name of the process */
};

int message;
pid CC_childpid,mypid,main_pid;
task me,pp,gp,main_task;
char *mytblock;
int signal_mode;
int CC_maxtblocksize;

task CC_checkout();
task CC_topqueue();
task CC_queuesucc();

#include <signal.h>
#define SIGBIT(s) (1<<(s-1))

int errno;

#include "DD_include.h"

