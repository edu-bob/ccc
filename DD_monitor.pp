# 1 "DD_monitor.c"


















































# 1 "/usr/include/sys/types.h"


































typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		
typedef	unsigned int	uint;		






typedef	struct	_physadr { int r[1]; } *physadr;
typedef struct	label_t {
	int	lt_r3;
	int	lt_r4;
	int	lt_r5;
	int	lt_r6;
	int	lt_r7;
	int	lt_fp;
	int	lt_sp;
        int	lt_pc;
} label_t;



typedef	struct	_quad { long val[2]; } quad;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_long	ino_t;
typedef	long	swblk_t;
typedef	int	size_t;
typedef	int	time_t;
typedef	short	dev_t;
typedef	int	off_t;
typedef	int	bool_t;
typedef	int	spl_t;
typedef long	key_t;		

typedef	struct	fd_set { int fds_bits[1]; } fd_set;


typedef	unsigned short	Rset_t;			










typedef	unsigned char	gate_t;


typedef	struct	{
	gate_t		l_gate;
	char		l_state;
} lock_t;



# 110 "/usr/include/sys/types.h"




typedef	struct	{
	int		*s_head;
	int		*s_tail;
	short		s_count;
	gate_t		s_gate;
	char		s_flags;
} sema_t;

# 51 "DD_monitor.c"

# 1 "/usr/include/sys/stat.h"

























struct	stat
{
	dev_t	st_dev;
	ino_t	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	int	st_spare1;
	time_t	st_mtime;
	int	st_spare2;
	time_t	st_ctime;
	int	st_spare3;
	long	st_blksize;
	long	st_blocks;
	long	st_spare4[2];
};















# 52 "DD_monitor.c"

# 1 "/usr/include/stdio.h"

















extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	int	_bufsiz;
	short	_flag;
	char	_file;
} _iob[20];




























struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*gets();
char	*fgets();
char	*sprintf();
# 53 "DD_monitor.c"

# 1 "/usr/include/ctype.h"






















extern	char	_ctype_[];















# 54 "DD_monitor.c"

# 1 "./CC_include.h"







# 1 "/usr/include/parallel/parallel.h"














































typedef unsigned char	slock_t;		


















typedef struct	{
	slock_t		b_mutex;	
	unsigned char	b_limit;	
	unsigned char	b_count;	
	unsigned char	b_useno;	
} sbarrier_t;				





extern	char	*sbrk(), *shsbrk();
extern	char	*shmalloc();
























































# 137 "/usr/include/parallel/parallel.h"






extern	int	errno;

extern	int	_shm_fd;		
extern	char	*_alm_base;		
extern	int	_pgoff;			






# 8 "./CC_include.h"
typedef slock_t *lock;



























































int CC_numtrans;






# 1 "./bool.h"
typedef char bool;




# 75 "./CC_include.h"





typedef struct task_struct *task;





typedef struct {
    int count;
    task head,tail;
} queue;










typedef struct {
    task	task;
    lock	life;
} *pid;




struct task_struct {

















    queue	*tqueue;	
    lock	queuelock;	
    task	next;		
    task	prev;		




























    task	newclient;	
    task	client;		
    task	prevclient;	
    bool	waiting;	
    bool	tsuccess;	
    char	*tblock;	
    int		selectedtrans;	
    int		*selectcase;	




















    int		unixpid;	
    lock	statelock;	
    task	firstchild;	
    task	nextsibling;	
    task	prevsibling;	
    bool	completed;	
    bool	aborted;	
    bool	choseterm;	
    bool	prepared;	
    bool	sickofkids;	
    int		numloans;	
    int		numprepared;	
    int		numchildren;	
    int		numgrandchildren; 




    char	*name;		
};

int 	CC_message;
pid CC_childpid,	CC_mypid,main_pid;
task 	CC_me,	CC_myparent,	CC_mygrandparent,main_task;
char *CC_mytblock;
int CC_signal_mode;
int CC_maxtblocksize;

task CC_checkout();
task CC_topqueue();
task CC_queuesucc();


# 1 "/usr/include/signal.h"









































































int	(*signal())();





struct	sigvec {
	int	(*sv_handler)();	
	int	sv_mask;		
	int	sv_onstack;		
};




struct	sigstack {
	char	*ss_sp;			
	int	ss_onstack;		
};








struct	sigcontext {
	int	sc_onstack;		
	int	sc_mask;		
	int	sc_sp;			
	int	sc_modpsr;		
	int	sc_pc;			
};







# 124 "/usr/include/signal.h"










# 219 "./CC_include.h"


int errno;


# 1 "./DD_include.h"








# 13 "./DD_include.h"


				

typedef enum {
	DD_WAITbORN,		
	DD_ISbORN,		
	DD_WAITaBORT,		
				
	DD_SELECTtERM,		


	DD_LASTuSERkILL,	
	DD_CHILDkILL,		

				
	DD_WAITcHILDdIE,	

				
				
	DD_LASTcHILD,		

	DD_EXITING,		
	DD_IMdYING,		

				

	DD_ABORTINGyOU,		
	DD_EATtIMER,		

	DD_EATtRANS,		

	DD_EATtERM,		

	DD_IMMEDsERV,		

	DD_WAITtRANS,		


				

	DD_WAITtIME,		


	DD_TIMEOUT,		
	DD_WAITaCCEPT,		

	DD_WAITsELECT,		

	DD_DELAY,		

	DD_ACCEPT,		

	DD_TRETURN,		
	DD_TRETURNED,		

	DD_COMMUN		
} DD_eventType;
# 224 "./CC_include.h"
# 55 "DD_monitor.c"

# 1 "./DD_kernel.h"






typedef char *DD_formal;		
typedef DD_formal DD_formals[];

typedef struct {
	char *name;
	unsigned line;			
	unsigned tblockSize;
	unsigned numFormals;		
	DD_formal *formals;
} DD_occur;			


typedef struct {		
	unsigned	line;
} DD_otherOccur;

typedef struct {
	unsigned	line;
	unsigned	termLine;		
	unsigned	immedLine;		
} DD_selectOccur;

typedef struct {
	unsigned	line;
	unsigned	valueSize;
} DD_treturnOccur;
	
typedef DD_occur DD_occurs[];
typedef DD_otherOccur DD_otherOccurs[];
typedef DD_selectOccur DD_selectOccurs[];
typedef DD_treturnOccur DD_treturnOccurs[];

typedef struct {
	char	*name;
	unsigned short space:14;	
	unsigned short num:12;		
	unsigned chain;
	unsigned short obj:4;		





} DD_symEntry;  


typedef struct {
	unsigned	numSymbols;
	DD_symEntry	*symbolTable;
	unsigned	numAccepts;
	DD_occur	*accept;
	unsigned	numCreates;
	DD_occur	*create;
	unsigned	numTransacts;
	DD_occur	*transact;
	unsigned	numSelects;
	DD_selectOccur	*select;
	unsigned	numDelays;
	DD_otherOccur	*delay;
	unsigned	numTerminates;
	DD_otherOccur	*terminate;
	unsigned	numTreturns;
	DD_treturnOccur	*treturn;
} DD_specTable;

static DD_convert();

typedef enum {
	DD_askSpec, DD_askCreate, DD_askAccept, DD_askTrans, DD_askTreturn
} question;



char *DD_dummy;			
extern char *DD_createArgs;


# 56 "DD_monitor.c"

# 1 "./DD_buffers.h"

# 1 "/usr/include/sys/time.h"





























struct timeval {
	long	tv_sec;		
	long	tv_usec;	
};

struct timezone {
	int	tz_minuteswest;	
	int	tz_dsttime;	
};



























struct	itimerval {
	struct	timeval it_interval;	
	struct	timeval it_value;	
};




struct tm {
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};


extern	struct tm *gmtime(), *localtime();
extern	char *asctime(), *ctime();

# 2 "./DD_buffers.h"

typedef struct buf {
	DD_eventType event;	
	int	id;		
	int	fromId;		
	int	toId;		

	char	*tblock;	

	struct	timeval time;	
	struct buf *next;
} buffer;


void DD_buffers_addEmpty(), DD_buffers_addFull();
buffer *DD_buffers_getEmpty(), *DD_buffers_getFull();
char *DD_buffers_getTblock();
# 57 "DD_monitor.c"


# 61 "DD_monitor.c"


int DD_me;
extern char *CC_mytblock;
char *DD_createArgs;

extern int (*DD_conversion)();			 
static DD_occur		*accepts;
static DD_occur		*creates;
static DD_occur		*transacts;
static DD_selectOccur	*selects;
static DD_otherOccur	*delays;
static DD_otherOccur	*terminates;
static DD_treturnOccur	*treturns;













bool
DD_monitor_init()
{
	DD_specTable *DD_SpecTable;

	if (DD_conversion == 0)	
		return 1;
	(*DD_conversion)(DD_askSpec, 0, 0, &DD_SpecTable);
	accepts		= DD_SpecTable->accept;
	creates		= DD_SpecTable->create;
	transacts	= DD_SpecTable->transact;
	selects		= DD_SpecTable->select;
	delays		= DD_SpecTable->delay;
	terminates	= DD_SpecTable->terminate;
	treturns	= DD_SpecTable->treturn;
	return 0;
}


DD_sendBuf (event, id, to )
DD_eventType event;
unsigned id;
unsigned to;
{
	static bool first = 1;
	buffer *bp;
	struct timezone tz;
	extern unsigned DD_tcallPid;		


	if ( Tracing ) {
		if (first)
			first = DD_monitor_init();
		bp = DD_buffers_getEmpty();
		bp->event = event;
		bp->id = id;
		bp->fromId = DD_me;
		bp->toId = to;
		gettimeofday(&bp->time, &tz);
		switch (event) {
		case DD_ISbORN:
			bp->tblock = (char *)CC_mypid;
		        break;
		case DD_WAITbORN:
			bp->tblock = DD_buffers_getTblock(		creates[id]. tblockSize);
			bcopy(DD_createArgs, bp->tblock, 		creates[id]. tblockSize);
			break;
		case DD_WAITtRANS:
			bp->tblock = DD_buffers_getTblock(	transacts[id]. tblockSize);
			bcopy(CC_mytblock, bp->tblock, 	transacts[id]. tblockSize);
			bp->toId = DD_tcallPid;
			break;
		case DD_TRETURNED:
			bp->tblock = DD_buffers_getTblock(	treturns[id]. valueSize);
			bcopy(CC_mytblock, bp->tblock, 	treturns[id]. valueSize);
			bp->toId = DD_tcallPid;
			break;
		}
		DD_buffers_addFull(bp);
	}
	
}


void
DD_event( event, id )
DD_eventType event;
unsigned id;
{
# 159 "DD_monitor.c"

}



void
DD_event2( event, toPid, id )
DD_eventType event;
unsigned toPid;
unsigned id;	
{
# 172 "DD_monitor.c"

}



# 179 "DD_monitor.c"

main(argc, argv, envp)

int argc;
char **argv, **envp;
{
	


	CC_maxtblocksize=512;
	CC_install_handler();
	CC_make_elder_tasks();
	CC_shinit();
	CC_main(argc, argv, envp);
	CC_complete();
}



# 263 "DD_monitor.c"

