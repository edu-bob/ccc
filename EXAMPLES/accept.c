#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 accept.cc

/* PROCESS SPEC a */
CC_pid CC_a();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 10 accept.cc
	    value;
	int arg1;
} 
CC_8x;
#define CC_8x_id 0

# line 10 accept.cc

#define CC_a_numtrans 1

# line 11 accept.cc

/* PROCESS SPEC b */
CC_pid CC_b();
#define CC_b_numtrans 0

# line 13 accept.cc

/* PROCESS BODY a */

CC_pid CC_a( CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 16 accept.cc

	int zzz ;

	if (CC_fork("a",CC_a_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;

# line 19 accept.cc
	{
		CC_tblock CC_tblock0;

# line 19 accept.cc

# line 19 accept.cc
		CC_accept(CC_8x_id,&CC_tblock0,0,0, 1);

# line 19 accept.cc
CC_warn1:
		CC_treturn(0);
CC_treturn0:;
	}

# line 19 accept.cc

	{
		CC_tblock CC_tblock1;

# line 20 accept.cc

# line 20 accept.cc
		CC_accept(CC_8x_id,&CC_tblock1,0,0, 2);
		{

# line 21 accept.cc
			printf ("In the second accept.\n");
			CC_treturn(2);
			goto CC_treturn1;

		}
CC_warn3:
		CC_treturn(2);
CC_treturn1:;
	}

# line 23 accept.cc

	{
		CC_tblock CC_tblock2;

# line 24 accept.cc

# line 24 accept.cc
		CC_accept(CC_8x_id,&CC_tblock2,0,0, 3);

# line 24 accept.cc
		{
			printf ("In the third accept, returning -1.\n")
# line 25 accept.cc
				;
			{
				((CC_8x *)CC_tblock2)->value =  -1
# line 26 accept.cc
				    ;
				CC_treturn(4);
				goto CC_treturn2;
			}

		}
CC_warn5:
		CC_treturn(4);
CC_treturn2:;
	}

# line 27 accept.cc

	{
		CC_tblock CC_tblock3;

# line 28 accept.cc

# line 28 accept.cc
		CC_accept(CC_8x_id,&CC_tblock3,0,0, 4);

# line 28 accept.cc
		{
			int j ;

# line 30 accept.cc
			j =((CC_8x *)CC_tblock3)->arg1 ;

# line 31 accept.cc
			printf ("In the fourth accept, we were passed %d.\n",j );
		}
CC_warn6:
		CC_treturn(5);
CC_treturn3:;
	}

# line 32 accept.cc

# line 33 accept.cc
	printf ("We are going to wait three seconds while some requests queue up.\n");
	CC_delay((double)(
# line 34 accept.cc
	3.0), 1);

# line 35 accept.cc
	printf ("Done waiting.\n");
	for (
# line 36 accept.cc
zzz =5;
# line 36 accept.cc
zzz <8;
# line 36 accept.cc
zzz ++)
	    {
		CC_tblock CC_tblock4;

# line 37 accept.cc

# line 37 accept.cc
		CC_accept(CC_8x_id,&CC_tblock4,1,0, 5);
		while(CC_acceptby((double)(
# line 37 accept.cc
		((CC_8x *)CC_tblock4)->arg1 
# line 37 accept.cc
		    )));
		{
			int j 
# line 38 accept.cc
			    ;
			j =((CC_8x *)CC_tblock4)->arg1 
# line 39 accept.cc
			    ;
			printf ("accept number %d gets arg %d.\n",zzz ,j )
# line 40 accept.cc
				;
		}
CC_warn7:
		CC_treturn(6);
CC_treturn4:;
	}

# line 41 accept.cc

	for (;;){
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
# line 43 accept.cc
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8x_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
# line 44 accept.cc
				{
					CC_tblock CC_tblock5;

# line 44 accept.cc

# line 44 accept.cc
					CC_accept(CC_8x_id,&CC_tblock5,0,1, 6);

# line 44 accept.cc
					{
						int j ;

# line 46 accept.cc
						j =((CC_8x *)CC_tblock5)->arg1 ;

# line 47 accept.cc
						printf ("and yet another accept, arg=%d.\n",j );
					}
CC_warn8:
					CC_treturn(7);
CC_treturn5:;
				}

# line 48 accept.cc

				break; 
# line 49 accept.cc
CC_guard1_2 :
				if (1) CC_terminate_exists=1; 
				goto CC_guard1_3;

# line 50 accept.cc
				break; 
# line 50 accept.cc

			}
			goto CC_endselect1;
CC_guard1_3: 
			goto CC_choose1;
CC_endselect1:;
		}

# line 51 accept.cc

	}
	CC_complete();
}

/* PROCESS BODY b */
typedef struct  {
	CC_pid  arg1;
	int  arg2;
} 
DD_cr_b;
DD_cr_b DD_create_b;

CC_pid CC_b(A,arg, CC_priority, DD_createId)
CC_pid  A;
int  arg;
int CC_priority;
unsigned DD_createId;
{

# line 56 accept.cc

	DD_create_b.arg1 = A;
	DD_create_b.arg2 = arg;
	DD_createArgs = (char *)&DD_create_b;

	if (CC_fork("b",CC_b_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;

# line 57 accept.cc
	printf ("just created, about to call A with arg=%d.\n",arg )
# line 57 accept.cc
		;
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= arg ,
	CC_tcall(1),
	((CC_8x *)CC_mytblock)->value) 
# line 58 accept.cc
		;
	CC_complete();
}

CC_main ()
{
	CC_pid A 
# line 63 accept.cc
	    ;
	int ret ;

# line 66 accept.cc
	A =CC_a (0, 1 );

# line 67 accept.cc
	printf ("calling first accept.\n");

# line 68 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(2),
	((CC_8x *)CC_mytblock)->value) ;

# line 69 accept.cc
	printf ("calling second accept.\n");

# line 70 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(3),
	((CC_8x *)CC_mytblock)->value) ;

# line 71 accept.cc
	printf ("calling third accept.\n");

# line 72 accept.cc
	ret =(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(4),
	((CC_8x *)CC_mytblock)->value) ;

# line 73 accept.cc
	printf ("third accept returned %d.\n",ret );

# line 74 accept.cc
	printf ("calling fourth accept, arg=666.\n");

# line 75 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 666,
	CC_tcall(5),
	((CC_8x *)CC_mytblock)->value) ;

# line 76 accept.cc
	printf ("creating processes to call A with differing args.\n");

# line 77 accept.cc
	CC_b (A ,444,0, 2 );
	CC_delay((double)(
# line 78 accept.cc
	0.25), 2);

# line 79 accept.cc
	CC_b (A ,333,0, 3 );
	CC_delay((double)(
# line 80 accept.cc
	0.25), 3);

# line 81 accept.cc
	CC_b (A ,222,0, 4 );
	CC_delay((double)(
# line 82 accept.cc
	0.25), 4);

# line 83 accept.cc
	CC_b (A ,111,0, 5 );
	CC_delay((double)(
# line 84 accept.cc
	0.25), 5);

# line 85 accept.cc
	printf ("created four calling tasks.\n");
	CC_delay((double)(
# line 86 accept.cc
	4.00), 6);

# line 87 accept.cc
	printf ("by now they should all be accepted...\n");

# line 88 accept.cc
	printf ("calling A with arg=9.\n");

# line 89 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9,
	CC_tcall(6),
	((CC_8x *)CC_mytblock)->value) ;

# line 90 accept.cc
	printf ("calling A with arg=99.\n");

# line 91 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 99,
	CC_tcall(7),
	((CC_8x *)CC_mytblock)->value) ;

# line 92 accept.cc
	printf ("calling A with arg=999.\n");

# line 93 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 999,
	CC_tcall(8),
	((CC_8x *)CC_mytblock)->value) ;

# line 94 accept.cc
	printf ("calling A with arg=9999.\n");

# line 95 accept.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9999,
	CC_tcall(9),
	((CC_8x *)CC_mytblock)->value) ;

# line 96 accept.cc
	printf ("all done!\n");
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
static DD_formals DD_formals_b = {
	"A", "arg"};
static DD_formals DD_acceptFormals_8x_1 = {
	"i"};
static DD_formals DD_acceptFormals_8x_2 = {
	"i"};
static DD_formals DD_acceptFormals_8x_3 = {
	"i"};
static DD_formals DD_acceptFormals_8x_4 = {
	"i"};
static DD_formals DD_acceptFormals_8x_5 = {
	"i"};
static DD_formals DD_acceptFormals_8x_6 = {
	"i"};
static DD_occurs DD_creates = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"a", 66,0, 0	}
	,
	{
		"b", 77, sizeof DD_create_b, DD_numAnd(DD_formals_b)	}
	,
	{
		"b", 79, sizeof DD_create_b, DD_numAnd(DD_formals_b)	}
	,
	{
		"b", 81, sizeof DD_create_b, DD_numAnd(DD_formals_b)	}
	,
	{
		"b", 83, sizeof DD_create_b, DD_numAnd(DD_formals_b)	}
};
static DD_occurs DD_accepts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"x", 19, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_1)	}
	,
	{
		"x", 20, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_2)	}
	,
	{
		"x", 24, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_3)	}
	,
	{
		"x", 28, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_4)	}
	,
	{
		"x", 37, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_5)	}
	,
	{
		"x", 44, sizeof(CC_8x), DD_numAnd(DD_acceptFormals_8x_6)	}
};
static DD_occurs DD_transacts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"x", 58, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 68, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 70, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 72, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 75, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 89, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 91, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 93, sizeof(CC_8x), 0, 0	}
	,
	{
		"x", 95, sizeof(CC_8x), 0, 0	}
};
static DD_selectOccurs DD_selects = {
	{
		0, 0, 0	}
	,{
		43, 50, 0	}
};
static DD_otherOccurs DD_delays = {
	{
		0	}
	,
	{
		34	}
	,
	{
		78	}
	,
	{
		80	}
	,
	{
		82	}
	,
	{
		84	}
	,
	{
		86	}
};
static DD_otherOccurs DD_terminates = {
	{
		0	}
};
static DD_treturnOccurs DD_treturns = {
	{
		0, 0	}
	,
	{
		19, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		22, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		23, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		26, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		27, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		32, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		41, sizeof ((CC_8x *)DD_dummy)->value	}
	,
	{
		48, sizeof ((CC_8x *)DD_dummy)->value	}
};
static DD_symEntry DD_SymbolTable[] = {

	{
		"a", 1, 8, 0, 3	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"b", 1, 10, 0, 3	}
	,
	{
		"x", 8, 9, 0, 14	}
	,
	{
		"0", 9, 1, 0, 0	}
	,
	{
		"1", 9, 6, 0, 14	}
	,
	{
		"=", 9, 6, 0, 14	}
	,
	{
		"1", 10, 8, 0, 3	}
	,
	{
		"2", 10, 6, 0, 14	}
};
static DD_specTable DD_SpecTable = {		
	DD_numAnd(DD_SymbolTable),
	DD_numAnd(DD_accepts),
	DD_numAnd(DD_creates),
	DD_numAnd(DD_transacts),
	DD_numAnd(DD_selects),
	DD_numAnd(DD_delays),
	DD_numAnd(DD_terminates),
	DD_numAnd(DD_treturns)};
static DD_convert(request, id, tblock, answer)
question request;
int id;
char *tblock;
char *answer;
{
	switch (request) {
	case DD_askSpec: 
		*(DD_specTable **)answer = &DD_SpecTable;
		return;
	case DD_askCreate:
		switch (id) {
		case 1:
			answer = "";
		case 2:
		case 3:
		case 4:
		case 5:
			sprintf(answer, " %d,%d",  
			((DD_cr_b *)tblock)->arg1,
			((DD_cr_b *)tblock)->arg2);
			return;
		}

	case DD_askAccept:
		id = DD_Acc(id);
	case DD_askTrans: 
		switch (id) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			case DD_Acc(1):
			case DD_Acc(2):
			case DD_Acc(3):
			case DD_Acc(4):
			case DD_Acc(5):
			case DD_Acc(6):
			sprintf(answer, " %d",  
			((CC_8x *)tblock)->arg1);
			return;
		}

	case DD_askTreturn: 
		switch (id) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			sprintf(answer, " %d",  
			((CC_8x *)tblock)->value);
			return;
		}
	}
}
