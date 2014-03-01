# 1 "DD_nulMonitor.c"


















































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
# 51 "DD_nulMonitor.c"

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
# 52 "DD_nulMonitor.c"

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


# 53 "DD_nulMonitor.c"

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
# 54 "DD_nulMonitor.c"

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










char *eventString[] = {
	"waiting for childbirth",
	"I am born",
	"waiting for abortion",
	"chose termination",
	"last user finished",
	"killing a child",
	"waiting for children to die",
	"last child dying",
	"I\'m exiting",
	"I\'m dying, you shouldn't be waiting",
	"I\'m aborting you",
	"eating a late timer signal",
	"eating a late transaction signal",
	"eating a late termination signal",
	"serve me",
	"waiting on a transaction",
	"waiting for a possible timeout",
	"tcall time out",
	"accept waiting",
	"select waiting",
	"delaying",
	"accepting a client",
	"treturn",
	"I got a return",
	"debugger ending"
};




DD_monitor_init()
{
	DD_specTable *DD_SpecTable;
	
	(*DD_conversion)(DD_askSpec, 0, 0, &DD_SpecTable);
	accepts		= DD_SpecTable->accept;
	creates		= DD_SpecTable->create;
	transacts	= DD_SpecTable->transact;
	selects		= DD_SpecTable->select;
	delays		= DD_SpecTable->delay;
	terminates	= DD_SpecTable->terminate;
	treturns	= DD_SpecTable->treturn;
	
}



void
DD_event( event, id )
DD_eventType event;
unsigned id;
{
# 160 "DD_nulMonitor.c"

}



void
DD_event2( event, toTask, id )	
DD_eventType event;
task toTask;
unsigned id;	
{
# 173 "DD_nulMonitor.c"

}



# 180 "DD_nulMonitor.c"

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



# 234 "DD_nulMonitor.c"

