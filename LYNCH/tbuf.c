#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 "tbuf.cc"

extern struct _iobuf {
	int _cnt ;
	char *_ptr ;
	char *_base ;
	int _bufsiz ;
	short _flag ;
	char _file ;
}
_iob [20];

struct _iobuf *fopen ();
struct _iobuf *fdopen ();
struct _iobuf *freopen ();
long	 ftell ();
char *gets ();
char *fgets ();
char *sprintf ();

union untype {
	int a ;
	long  b ;
	char c ;
}
un ;

typedef struct comp {
	int a ;
	struct comp *self ;
	union untype bar ;
	union sec {
		int f ,g ;
	}
	the ;
	union sec h ,i ;

	char ch ;
}
complicated ;

complicated c5 ,c6 ;

struct u {
	int i ,j ;
	complicated k ;
	struct u *uptr ;

}
unc ;

/* PROCESS SPEC foo */
CC_pid CC_foo();

typedef complicated CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
} 
CC_13a;
#define CC_13a_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	complicated arg1;
	complicated arg2;
} 
CC_13b;
#define CC_13b_id 1

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 value;
} 
CC_13c;
#define CC_13c_id 2

#define CC_foo_numtrans 3

/* DEBUG SPEC */
typedef struct  {
	complicated  arg1;
	complicated  arg2;
	int  arg3;
} 
DD_cr_foo;
DD_cr_foo DD_create_foo;

static void 
DD_convert_foo(request, id, tblock, answer)
DD_question request;
int id;
char *tblock;
char *answer;
{
	switch ( request ) {
	case DD_askCreate:
		sprintf(answer, "{%d,%x,[union],[union],[union],[union],\'%s\'},{%d,%x,[union],[union],[union],[union],\'%s\'},%d", 

		((DD_cr_foo *)tblock)->arg1.a,
		((DD_cr_foo *)tblock)->arg1.self

		,
		DD_unctrl(((DD_cr_foo *)tblock)->arg1.ch)
		    ,

		((DD_cr_foo *)tblock)->arg2.a,
		((DD_cr_foo *)tblock)->arg2.self

		,
		DD_unctrl(((DD_cr_foo *)tblock)->arg2.ch)
		    ,
		((DD_cr_foo *)tblock)->arg3);
		return;
	case DD_askTrans: 
		switch ( id ) {
		case CC_13a_id:
			sprintf(answer, "%d,%d,%d,%d", 
			((CC_13a *)tblock)->arg1,
			((CC_13a *)tblock)->arg2,
			((CC_13a *)tblock)->arg3,
			((CC_13a *)tblock)->arg4);
			return;
		case CC_13b_id:
			sprintf(answer, "{%d,%x,[union],[union],[union],[union],\'%s\'},{%d,%x,[union],[union],[union],[union],\'%s\'}", 

			((CC_13b *)tblock)->arg1.a,
			((CC_13b *)tblock)->arg1.self

			,
			DD_unctrl(((CC_13b *)tblock)->arg1.ch)
			    ,

			((CC_13b *)tblock)->arg2.a,
			((CC_13b *)tblock)->arg2.self

			,
			DD_unctrl(((CC_13b *)tblock)->arg2.ch)
			    );
			return;
		case CC_13c_id:
			*answer =  '\0';
		}

	case DD_askTreturn: 
		switch ( id ) {
		case CC_13a_id:
			sprintf(answer, "{%d,%x,[union],[union],[union],[union],\'%s\'}", 

			((CC_13a *)tblock)->value.a,
			((CC_13a *)tblock)->value.self

			,
			DD_unctrl(((CC_13a *)tblock)->value.ch)
			    );
			return;
		case CC_13b_id:
			sprintf(answer, "%d", 
			((CC_13b *)tblock)->value);
			return;
		case CC_13c_id:
			sprintf(answer, "%d", 
			((CC_13c *)tblock)->value);
			return;
		}
	}
}
DD_ProcessTable DD_processTable_foo;

/* PROCESS BODY foo */

CC_pid CC_foo(c1,c2,i, CC_priority, DD_createId)
complicated  c1;
complicated  c2;
int  i;
int CC_priority;
unsigned DD_createId;
{

	int i ;
	complicated j ;

	DD_create_foo.arg1 = c1;
	DD_create_foo.arg2 = c2;
	DD_create_foo.arg3 = i;
	DD_createArgs = (char *)&DD_create_foo;

	if (CC_fork("foo",CC_foo_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	DD_processTable = &DD_processTable_foo;
	{
		CC_tblock CC_tblock0;
		CC_accept(CC_13a_id,&CC_tblock0,0,0, 1);
		{
			{
				((CC_13a *)CC_tblock0)->value = c1 ;
				CC_treturn(1);
				goto CC_treturn0;
			}

		}
CC_warn2:
		CC_treturn(2);
CC_treturn0:;
	}
	;

	{
		CC_tblock CC_tblock1;
		CC_accept(CC_13b_id,&CC_tblock1,0,0, 2);
		{
			((CC_13b *)CC_tblock1)->arg1 .a  =5;
			((CC_13b *)CC_tblock1)->arg1 .self  =(&((CC_13b *)CC_tblock1)->arg1 );
			((CC_13b *)CC_tblock1)->arg2 .bar  .c  ='c';
			{
				((CC_13b *)CC_tblock1)->value = 3;
				CC_treturn(3);
				goto CC_treturn1;
			}

		}
CC_warn4:
		CC_treturn(4);
CC_treturn1:;
	}
	;

	c2 .a  =5;
	c2 .self  =(&c1 );
	c2 .bar  .c  ='c';

	CC_complete();
}

/* DEBUG BODY */
static DD_formals DD_formals_foo = {
	"c1", "c2", "i"};
static DD_formals DD_acceptFormals_13a_1 = {
	"i1", "i2", "i3", "i4"};
static DD_formals DD_acceptFormals_13b_2 = {
	"c3", "c4"};
static DD_occurs DD_creates_foo = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_accepts_foo = {
	{
		"", 0, 0, 0, 0, 0	}
	,
	{
		"a", 56, sizeof(CC_13a), DD_convert_foo, DD_numAnd(DD_acceptFormals_13a_1)	}
	,
	{
		"b", 60, sizeof(CC_13b), DD_convert_foo, DD_numAnd(DD_acceptFormals_13b_2)	}
};
static DD_occurs DD_transacts_foo = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_selectOccurs DD_selects_foo = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays_foo = {
	{
		0	}
};
static DD_otherOccurs DD_terminates_foo = {
	{
		0	}
};
static DD_treturnOccurs DD_treturns_foo = {
	{
		0, 0, 0	}
	,
	{
		57, sizeof ((CC_13a *)DD_dummy)->value, DD_convert_foo	}
	,
	{
		58, sizeof ((CC_13a *)DD_dummy)->value, DD_convert_foo	}
	,
	{
		64, sizeof ((CC_13b *)DD_dummy)->value, DD_convert_foo	}
	,
	{
		65, sizeof ((CC_13b *)DD_dummy)->value, DD_convert_foo	}
};
static DD_ProcessTable DD_processTable_foo = {		
	DD_numAnd(DD_accepts_foo),
	DD_numAnd(DD_creates_foo),
	DD_numAnd(DD_transacts_foo),
	DD_numAnd(DD_selects_foo),
	DD_numAnd(DD_delays_foo),
	DD_numAnd(DD_terminates_foo),
	DD_numAnd(DD_treturns_foo)};

CC_main (argc ,argv )
int argc ;
char *argv [];
{
	CC_pid fooo ;
	float f ;
	struct _iobuf *fd ;

	fd =fopen ("tbuf.out","w");
	fprintf (fd ,"tbuf starting\n");

	printf ("Tbuf main STARTING.\n");

	fooo =CC_foo (c5 ,c5 ,9,0, 1 );

	(CC_tcallinit(fooo ,CC_13a_id),
	((CC_13a *)CC_mytblock)->arg1= 5,
	((CC_13a *)CC_mytblock)->arg2= 6,
	((CC_13a *)CC_mytblock)->arg3= 7,
	((CC_13a *)CC_mytblock)->arg4= 8,
	CC_tcall(1),
	((CC_13a *)CC_mytblock)->value) ;
	(CC_tcallinit(fooo ,CC_13b_id),
	((CC_13b *)CC_mytblock)->arg1= c6 ,
	((CC_13b *)CC_mytblock)->arg2= c6 ,
	CC_tcall(2),
	((CC_13b *)CC_mytblock)->value) ;

}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}

/* DEBUG BODY */
static DD_occurs DD_creates_main = {
	{
		"", 0, 0, 0, 0, 0	}
	,
	{
		"foo", 91, sizeof DD_create_foo, DD_convert_foo, DD_numAnd(DD_formals_foo)	}
};
static DD_occurs DD_accepts_main = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_transacts_main = {
	{
		"", 0, 0, 0, 0, 0	}
	,
	{
		"a", 93, sizeof(CC_13a), DD_convert_foo, 0, 0	}
	,
	{
		"b", 94, sizeof(CC_13b), DD_convert_foo, 0, 0	}
};
static DD_selectOccurs DD_selects_main = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays_main = {
	{
		0	}
};
static DD_otherOccurs DD_terminates_main = {
	{
		0	}
};
static DD_treturnOccurs DD_treturns_main = {
	{
		0, 0, 0	}
};
static DD_ProcessTable DD_processTable_main = {		
	DD_numAnd(DD_accepts_main),
	DD_numAnd(DD_creates_main),
	DD_numAnd(DD_transacts_main),
	DD_numAnd(DD_selects_main),
	DD_numAnd(DD_delays_main),
	DD_numAnd(DD_terminates_main),
	DD_numAnd(DD_treturns_main)};
DD_ProcessTable *DD_processTable = &DD_processTable_main;
DD_symEntry DD_SymbolTable[] = {

	{
		"_iobuf", 1, 8, 0, 1	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"comp", 1, 10, 0, 1	}
	,
	{
		"foo", 1, 13, 0, 3	}
	,
	{
		"sec", 1, 11, 0, 2	}
	,
	{
		"u", 1, 12, 0, 1	}
	,
	{
		"untype", 1, 9, 0, 2	}
	,
	{
		"a", 9, 6, 0, 14	}
	,
	{
		"b", 9, 6, 0, 9	}
	,
	{
		"c", 9, 6, 0, 12	}
	,
	{
		"a", 10, 6, 0, 14	}
	,
	{
		"self", 10, 10, 5, 1	}
	,
	{
		"bar", 10, 9, 0, 2	}
	,
	{
		"the", 10, 11, 0, 2	}
	,
	{
		"h", 10, 11, 0, 2	}
	,
	{
		"i", 10, 11, 0, 2	}
	,
	{
		"ch", 10, 6, 0, 12	}
	,
	{
		"f", 11, 6, 0, 14	}
	,
	{
		"g", 11, 6, 0, 14	}
	,
	{
		"0", 13, 3, 0, 0	}
	,
	{
		"1", 13, 10, 0, 1	}
	,
	{
		"2", 13, 10, 0, 1	}
	,
	{
		"3", 13, 6, 0, 14	}
	,
	{
		"a", 13, 14, 0, 1	}
	,
	{
		"b", 13, 15, 0, 14	}
	,
	{
		"c", 13, 16, 0, 14	}
	,
	{
		"0", 14, 4, 0, 0	}
	,
	{
		"1", 14, 6, 0, 14	}
	,
	{
		"2", 14, 6, 0, 14	}
	,
	{
		"3", 14, 6, 0, 14	}
	,
	{
		"4", 14, 6, 0, 14	}
	,
	{
		"=", 14, 10, 0, 1	}
	,
	{
		"0", 15, 2, 0, 0	}
	,
	{
		"1", 15, 10, 0, 1	}
	,
	{
		"2", 15, 10, 0, 1	}
	,
	{
		"=", 15, 6, 0, 14	}
	,
	{
		"0", 16, 0, 0, 0	}
	,
	{
		"=", 16, 6, 0, 14	}
};
int DD_symbolSize = sizeof DD_SymbolTable / sizeof DD_SymbolTable[0];
DD_globalOccurs DD_globalOccurTable[] = {
	{
		"main", &DD_processTable_main	}
	,{
		"foo", &DD_processTable_foo	}
};
int DD_numberGlobals = 2;
