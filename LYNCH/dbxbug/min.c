typedef char *CC_tblock;
typedef int CC_void;
typedef struct CC_pid_struct *CC_pid;
typedef struct CC_task_struct *CC_task;

#define c_nullpid ((CC_pid)0)

extern float CC_bestdelay,CC_trydelay;
extern int CC_achoice,CC_dchoice,CC_ichoice,CC_ochoice;
extern int CC_terminate_exists,CC_accept_exists;

extern CC_pid CC_mypid,CC_childpid;
extern CC_task CC_me;
extern CC_tblock CC_mytblock;

char *CC_shmalloc();

static char *DD_convert;

# line 1 min.cc

# line 1 /usr/include/stdio.h

extern struct _iobuf {
	int _cnt ;
	char *_ptr ;
	char *_base ;
	int _bufsiz ;
	short _flag ;
	char _file ;
}
_iob [20
# line 26 /usr/include/stdio.h
];

struct _iobuf *fopen ()
# line 55 /usr/include/stdio.h
;
struct _iobuf *fdopen ();
struct _iobuf *freopen ()
# line 57 /usr/include/stdio.h
;
long	 ftell ();
char *gets ()
# line 59 /usr/include/stdio.h
;
char *fgets ();
char *sprintf ()
# line 61 /usr/include/stdio.h
;

# line 2 min.cc

/* PROCESS SPEC foo */
CC_pid CC_foo();
#define CC_foo_numtrans 0

/* PROCESS BODY foo */

CC_pid CC_foo( CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("foo",CC_foo_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	CC_complete();
}

CC_main ()
{
	setbuf ((&_iob [1]),0);
	printf ("main starting\n");
	CC_delay((double)((10)), 1);

	CC_foo (0, 1 );
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
