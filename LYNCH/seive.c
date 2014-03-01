#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 seive.cc

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

# line 26 seive.cc

typedef int factor ;

typedef int  CC_shtypedef0;
CC_shtypedef0 
# line 40 seive.cc
*seive
# line 40 seive.cc
;

/* PROCESS SPEC Crossout */
CC_pid CC_Crossout();
#define CC_Crossout_numtrans 0

# line 43 seive.cc

/* PROCESS BODY Crossout */
struct DD_create_Crossout_tag {
	factor  arg1;
} 
DD_create_Crossout;

CC_pid CC_Crossout(fact, CC_priority, DD_createId)

# line 48 seive.cc
factor  fact;
int CC_priority;
unsigned DD_createId;
{

	CC_pid newFactor 
# line 49 seive.cc
	    ;
	int i ;
	int nextCross 
# line 51 seive.cc
	    ;

	DD_create_Crossout.arg1 = fact;
	DD_createArgs = (char *)DD_create_Crossout;

	if (CC_fork("Crossout",CC_Crossout_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;

# line 57 seive.cc
	fprintf ((&_iob [2]),"A new Crossout factor: %d\n",fact )
# line 57 seive.cc
		;

	nextCross =fact 
# line 61 seive.cc
	    ;
	for (i =fact 
# line 62 seive.cc
;i <=10
# line 62 seive.cc
;i ++
# line 62 seive.cc
){
		    if (i ==nextCross 
# line 63 seive.cc
		    ){
			seive [i ]=0
# line 64 seive.cc
			    ;
			nextCross +=fact 
# line 65 seive.cc
			    ;
		}
		else {
			if (seive [(i )]!=0
# line 67 seive.cc
			    ){
				newFactor =CC_Crossout (i ,0, 1 )
# line 68 seive.cc
					;

				if (newFactor ==c_nullpid 
# line 71 seive.cc
				    ){
					fprintf ((&_iob [2]),"problem with process creation.\n")
# line 72 seive.cc
						;
				}

			}
		}

	}

	for (i =nextCross 
# line 82 seive.cc
;i <101
# line 82 seive.cc
;i +=fact 
# line 82 seive.cc
){
		    seive [i ]=0
# line 83 seive.cc
		    ;
	}

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |c_mypid             0       25      9       3                           |
	 *  |fprintf             0       25      6       0                           |
	 *  |i                   0       0       6       14                          |
	 *  |newFactor           0       0       9       3                           |
	 *  |nextCross           0       0       6       14                          |
	 */
	CC_complete();
}

CC_main ()
{

	CC_pid firstFactor 
# line 91 seive.cc
	    ;
	int i ,delayed 
# line 92 seive.cc
	    ;

	for (i =2
# line 95 seive.cc
;i <101
# line 95 seive.cc
;i ++
# line 95 seive.cc
){
		    seive [i ]=i 
# line 96 seive.cc
		    ;
	}
	firstFactor =CC_Crossout (2,0, 2 )
# line 98 seive.cc
		;

	delayed =0
# line 101 seive.cc
	    ;
	while ( !c_invalid (firstFactor )
# line 102 seive.cc
	    ){
		CC_delay((double)(1
# line 103 seive.cc
		    ), 1);

		delayed ++
# line 104 seive.cc
		    ;
	}
	printf ("Delayed %d seconds.\n")
# line 106 seive.cc
		;

	printf ("Primes through %1d: ",101-1)
# line 110 seive.cc
		;
	for (i =2
# line 111 seive.cc
;i <101
# line 111 seive.cc
;i ++
# line 111 seive.cc
){
		    if (seive [(i )]!=0
# line 112 seive.cc
		    ){
			printf ("%1d, ",i )
# line 113 seive.cc
				;
		}
	}

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |delayed             0       0       6       14                          |
	 *  |firstFactor         0       0       9       3                           |
	 *  |i                   0       0       6       14                          |
	 *  |printf              0       25      6       0                           |
	 */
}

/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |CC_main             0       25      6       0                           |
 *  |_iob                0       5       8       1                           |
 *  |c_abort             0       25      6       14                          |
 *  |c_active            0       25      6       14                          |
 *  |c_associate         0       25      6       14                          |
 *  |c_changepriority    0       25      6       14                          |
 *  |c_completed         0       25      6       14                          |
 *  |c_getpriority       0       25      6       14                          |
 *  |c_invalid           0       25      6       14                          |
 *  |c_nullpid           0       0       5       3                           |
 *  |fact                0       0       6       14                          |
 *  |factor              0       0       6       14                          |
 *  |fdopen              0       46      8       1                           |
 *  |fgets               0       46      6       12                          |
 *  |fopen               0       46      8       1                           |
 *  |freopen             0       46      8       1                           |
 *  |ftell               0       25      6       9                           |
 *  |gets                0       46      6       12                          |
 *  |seive               0       5       6       14                          |
 *  |sprintf             0       46      6       12                          |
 *  |Crossout            1       0       9       3                           |
 *  |_iobuf              1       0       8       1                           |
 *  |anytype             1       0       4       3                           |
 *  |_base               8       5       6       12                          |
 *  |_bufsiz             8       0       6       14                          |
 *  |_cnt                8       0       6       14                          |
 *  |_file               8       0       6       12                          |
 *  |_flag               8       0       6       11                          |
 *  |_ptr                8       5       6       12                          |
 *  |1                   9       0       6       14                          |
 */
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	seive = (CC_shtypedef0 *) CC_shmalloc((101) * sizeof(*seive));
}
static DD_formals DD_formals_Crossout = {
	"fact"};
static DD_occurs DD_creates = {
	{
		"", 0, 0, 0	}
	,
	{
		"Crossout", 68, sizeof DD_create_Crossout, DD_formals_Crossout	}
	,
	{
		"Crossout", 98, sizeof DD_create_Crossout, DD_formals_Crossout	}
};
static DD_occurs DD_accepts = {
	{
		"", 0, 0, 0	}
};
static DD_occurs DD_transacts = {
	{
		"", 0, 0, 0	}
};
static DD_selectOccurs DD_selects = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays = {
	{
		0	}
	,
	{
		103	}
};
static DD_otherOccurs DD_terminates = {
	{
		0	}
};
static DD_otherOccurs DD_treturns = {
	{
		0	}
};
static DD_symEntry DD_SymbolTable[] = {

	{
		"Crossout", 1, 9, 0, 3	}
	,
	{
		"_iobuf", 1, 8, 0, 1	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"_cnt", 8, 6, 0, 14	}
	,
	{
		"_ptr", 8, 6, 5, 12	}
	,
	{
		"_base", 8, 6, 5, 12	}
	,
	{
		"_bufsiz", 8, 6, 0, 14	}
	,
	{
		"_flag", 8, 6, 0, 11	}
	,
	{
		"_file", 8, 6, 0, 12	}
	,
	{
		"1", 9, 6, 0, 14	}
};
static DD_specTable DD_SpecTable = {		
	sizeof DD_SymbolTable / sizeof(DD_symEntry),
	DD_SymbolTable,
	DD_accepts,
	DD_creates,
	DD_transacts,
	DD_selects,
	DD_delays,
	DD_terminates,
	DD_treturns};
static DD_convert(request, id, tblock, answer)
question request;
int id;
char *tblock;
char **answer;
{
	switch (request) {
	case DD_askSpec: 
		*answer = (char *)&DD_SpecTable;
		return;
	case DD_askCreate:
		switch (id) {
		case 1:
		case 2:
			sprintf(*answer, " %d",  
			((struct DD_create_Crossout_tag *)DD_createArgs)->arg1);
			return;
		}

	case DD_askAccept:
		id = DD_Acc(id);
	case DD_askTrans: 
		switch (id) {
		}

	case DD_askTreturn: 
		switch (id) {
		}
	}
}
