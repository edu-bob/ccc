#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 "matrix.cc"

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

typedef float *rowArray [10];

rowArray A ,B ;

/* PROCESS SPEC C */
CC_pid CC_C();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	int arg1;
	int arg2;
	float arg3;
} 
CC_9answer;
#define CC_9answer_id 0

#define CC_C_numtrans 1

/* DEBUG SPEC */
typedef struct  {
	int  arg1;
	int  arg2;
} 
DD_cr_C;
DD_cr_C DD_create_C;

static void 
DD_convert_C(request, id, tblock, answer)
DD_question request;
int id;
char *tblock;
char *answer;
{
	switch ( request ) {
	case DD_askCreate:
		sprintf(answer, "%d,%d", 
		((DD_cr_C *)tblock)->arg1,
		((DD_cr_C *)tblock)->arg2);
		return;
	case DD_askTrans: 
		switch ( id ) {
		case CC_9answer_id:
			sprintf(answer, "%d,%d,%g", 
			((CC_9answer *)tblock)->arg1,
			((CC_9answer *)tblock)->arg2,
			((CC_9answer *)tblock)->arg3);
			return;
		}

	case DD_askTreturn: 
		switch ( id ) {
		case CC_9answer_id:
			sprintf(answer, "%d", 
			((CC_9answer *)tblock)->value);
			return;
		}
	}
}
DD_ProcessTable DD_processTable_C;

/* PROCESS SPEC DotProd */
CC_pid CC_DotProd();
#define CC_DotProd_numtrans 0

/* DEBUG SPEC */
typedef struct  {
	int  arg1;
	int  arg2;
	int  arg3;
	int  arg4;
	CC_pid  arg5;
} 
DD_cr_DotProd;
DD_cr_DotProd DD_create_DotProd;

static void 
DD_convert_DotProd(request, id, tblock, answer)
DD_question request;
int id;
char *tblock;
char *answer;
{
	switch ( request ) {
	case DD_askCreate:
		sprintf(answer, "%d,%d,%d,%d,%d", 
		((DD_cr_DotProd *)tblock)->arg1,
		((DD_cr_DotProd *)tblock)->arg2,
		((DD_cr_DotProd *)tblock)->arg3,
		((DD_cr_DotProd *)tblock)->arg4,
		((DD_cr_DotProd *)tblock)->arg5);
		return;
	case DD_askTrans: 
		switch ( id ) {
		}

	case DD_askTreturn: 
		switch ( id ) {
		}
	}
}
DD_ProcessTable DD_processTable_DotProd;

/* PROCESS BODY C */

CC_pid CC_C(rows,cols, CC_priority, DD_createId)
int  rows;
int  cols;
int CC_priority;
unsigned DD_createId;
{

	rowArray C ;
	int i ,j ;

	DD_create_C.arg1 = rows;
	DD_create_C.arg2 = cols;
	DD_createArgs = (char *)&DD_create_C;

	if (CC_fork("C",CC_C_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	DD_processTable = &DD_processTable_C;
	shmallocMatrix (rows ,cols ,C );

	for (i =0;i <rows ;i ++){
		for (j =0;j <cols ;j ++){
			{
				CC_tblock CC_tblock0;
				CC_accept(CC_9answer_id,&CC_tblock0,0,0, 1);
				{

					C [((CC_9answer *)CC_tblock0)->arg1 ][((CC_9answer *)CC_tblock0)->arg2 ]=((CC_9answer *)CC_tblock0)->arg3 ;
					CC_treturn(1);
					goto CC_treturn0;

				}
CC_warn2:
				CC_treturn(2);
CC_treturn0:;
			}

		}
	}

	printMatrix (rows ,cols ,C );
	CC_complete();
}

/* DEBUG BODY */
static DD_formals DD_formals_C = {
	"rows", "cols"};
static DD_formals DD_acceptFormals_9answer_1 = {
	"row", "col", "num"};
static DD_occurs DD_creates_C = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_accepts_C = {
	{
		"", 0, 0, 0, 0, 0	}
	,
	{
		"answer", 70, sizeof(CC_9answer), DD_convert_C, DD_numAnd(DD_acceptFormals_9answer_1)	}
};
static DD_occurs DD_transacts_C = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_selectOccurs DD_selects_C = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays_C = {
	{
		0	}
};
static DD_otherOccurs DD_terminates_C = {
	{
		0	}
};
static DD_treturnOccurs DD_treturns_C = {
	{
		0, 0, 0	}
	,
	{
		76, sizeof ((CC_9answer *)DD_dummy)->value, DD_convert_C	}
	,
	{
		77, sizeof ((CC_9answer *)DD_dummy)->value, DD_convert_C	}
};
static DD_ProcessTable DD_processTable_C = {		
	DD_numAnd(DD_accepts_C),
	DD_numAnd(DD_creates_C),
	DD_numAnd(DD_transacts_C),
	DD_numAnd(DD_selects_C),
	DD_numAnd(DD_delays_C),
	DD_numAnd(DD_terminates_C),
	DD_numAnd(DD_treturns_C)};

/* PROCESS BODY DotProd */

CC_pid CC_DotProd(Ahigh,Alow,Brows,Bcols,C, CC_priority, DD_createId)
int  Ahigh;
int  Alow;
int  Brows;
int  Bcols;
CC_pid  C;
int CC_priority;
unsigned DD_createId;
{

	int bc ;
	int i ,j ;
	float product ;

	DD_create_DotProd.arg1 = Ahigh;
	DD_create_DotProd.arg2 = Alow;
	DD_create_DotProd.arg3 = Brows;
	DD_create_DotProd.arg4 = Bcols;
	DD_create_DotProd.arg5 = C;
	DD_createArgs = (char *)&DD_create_DotProd;

	if (CC_fork("DotProd",CC_DotProd_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	DD_processTable = &DD_processTable_DotProd;
	for (bc =0;bc <Bcols ;bc ++){
		for (i =Alow ;i <Ahigh ;i ++){

			product =0.0;
			for (j =0;j <Brows ;j ++){
				product =A [i ][j ]*B [j ][bc ];
			}
			(CC_tcallinit(C ,CC_9answer_id),
			((CC_9answer *)CC_mytblock)->arg1= i ,
			((CC_9answer *)CC_mytblock)->arg2= bc ,
			((CC_9answer *)CC_mytblock)->arg3= product ,
			CC_tcall(1),
			((CC_9answer *)CC_mytblock)->value) ;
		}
	}
	CC_complete();
}

/* DEBUG BODY */
static DD_formals DD_formals_DotProd = {
	"Ahigh", "Alow", "Brows", "Bcols", "C"};
static DD_occurs DD_creates_DotProd = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_accepts_DotProd = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_transacts_DotProd = {
	{
		"", 0, 0, 0, 0, 0	}
	,
	{
		"answer", 105, sizeof(CC_9answer), DD_convert_C, 0, 0	}
};
static DD_selectOccurs DD_selects_DotProd = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays_DotProd = {
	{
		0	}
};
static DD_otherOccurs DD_terminates_DotProd = {
	{
		0	}
};
static DD_treturnOccurs DD_treturns_DotProd = {
	{
		0, 0, 0	}
};
static DD_ProcessTable DD_processTable_DotProd = {		
	DD_numAnd(DD_accepts_DotProd),
	DD_numAnd(DD_creates_DotProd),
	DD_numAnd(DD_transacts_DotProd),
	DD_numAnd(DD_selects_DotProd),
	DD_numAnd(DD_delays_DotProd),
	DD_numAnd(DD_terminates_DotProd),
	DD_numAnd(DD_treturns_DotProd)};

shmallocMatrix (rows ,cols ,rowPtrs )
int rows ,cols ;
rowArray rowPtrs ;
{
	int i ;

	for (i =0;i <rows ;i ++){
		rowPtrs [i ]=(float*)shmalloc (sizeof (*rowPtrs [i ])*cols );
	}
}

printMatrix (rows ,cols ,matrix )
int rows ,cols ;
rowArray matrix ;
{
	int i ,j ;

	for (i =0;i <rows ;i ++){
		for (j =0;j <cols ;j ++){
			printf ("%7.3f ",matrix [i ][j ]);
		}
		printf ("\n");
	}
}

getMatrix (argc ,argv ,Arows ,Acols ,Brows ,Bcols )
int argc ;
char *argv [];
int *Arows ,*Acols ,*Brows ,*Bcols ;

{
	int i ,j ;

	*Arows =10;
	*Acols =9;
	*Brows =9;
	*Bcols =10;

	shmallocMatrix (*Arows ,*Acols ,A );
	shmallocMatrix (*Brows ,*Bcols ,B );

	for (i =0;i <*Arows ;i ++){
		for (j =0;j <*Acols ;j ++){
			A [i ][j ]=i +j ;
		}
	}

	for (i =0;i <*Brows ;i ++){
		for (j =0;j <*Bcols ;j ++){
			B [i ][j ]=i -j ;
		}
	}

}

CC_main (argc ,argv )
int argc ;
char *argv [];
{
	CC_pid C ;
	int Arows ,Acols ,Brows ,Bcols ;
	int Ahigh ;
	int rowsPerProcess ;

	getMatrix (argc ,argv ,&Arows ,&Acols ,&Brows ,&Bcols );
	C =CC_C (Arows ,Bcols ,0, 1 );

	rowsPerProcess =Arows /4+((Arows %4)>0);
	for (Ahigh =Arows ;Ahigh >0;Ahigh -=rowsPerProcess ){

		CC_DotProd (Ahigh ,Ahigh >=rowsPerProcess ?Ahigh -rowsPerProcess :0,Brows ,Bcols ,C ,0, 2 );

	}
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
		"C", 207, sizeof DD_create_C, DD_convert_C, DD_numAnd(DD_formals_C)	}
	,
	{
		"DotProd", 213, sizeof DD_create_DotProd, DD_convert_DotProd, DD_numAnd(DD_formals_DotProd)	}
};
static DD_occurs DD_accepts_main = {
	{
		"", 0, 0, 0, 0, 0	}
};
static DD_occurs DD_transacts_main = {
	{
		"", 0, 0, 0, 0, 0	}
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
		"C", 1, 9, 0, 3	}
	,
	{
		"DotProd", 1, 11, 0, 3	}
	,
	{
		"_iobuf", 1, 8, 0, 1	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"0", 9, 2, 0, 0	}
	,
	{
		"1", 9, 6, 0, 14	}
	,
	{
		"2", 9, 6, 0, 14	}
	,
	{
		"answer", 9, 10, 0, 14	}
	,
	{
		"0", 10, 3, 0, 0	}
	,
	{
		"1", 10, 6, 0, 14	}
	,
	{
		"2", 10, 6, 0, 14	}
	,
	{
		"3", 10, 6, 0, 13	}
	,
	{
		"=", 10, 6, 0, 14	}
	,
	{
		"0", 11, 5, 0, 0	}
	,
	{
		"1", 11, 6, 0, 14	}
	,
	{
		"2", 11, 6, 0, 14	}
	,
	{
		"3", 11, 6, 0, 14	}
	,
	{
		"4", 11, 6, 0, 14	}
	,
	{
		"5", 11, 9, 0, 3	}
};
int DD_symbolSize = sizeof DD_SymbolTable / sizeof DD_SymbolTable[0];
DD_globalOccurs DD_globalOccurTable[] = {
	{
		"main", &DD_processTable_main	}
	,{
		"C", &DD_processTable_C	}
	,{
		"DotProd", &DD_processTable_DotProd	}
};
int DD_numberGlobals = 3;
