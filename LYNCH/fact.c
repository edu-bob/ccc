#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 "fact.cc"

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

/* PROCESS SPEC parent */
CC_pid CC_parent();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	double arg1;
} 
CC_9result;
#define CC_9result_id 0

#define CC_parent_numtrans 1

/* PROCESS SPEC product */
CC_pid CC_product();
#define CC_product_numtrans 0

/* PROCESS BODY parent */
typedef struct  {
	long int  arg1;
} 
DD_cr_parent;
DD_cr_parent DD_create_parent;

CC_pid CC_parent(n, CC_priority, DD_createId)
long int  n;
int CC_priority;
unsigned DD_createId;
{

	int started ;
	double factorial ;
	long int start ;
	int stride ;

	DD_create_parent.arg1 = n;
	DD_createArgs = (char *)&DD_create_parent;

	if (CC_fork("parent",CC_parent_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	start =n ;
	stride =4;

	for (started =0;started <4;started ++){
		CC_product (start --,stride ,c_mypid (),0, 1 );
	}

	factorial =1;
	for (;started >0;started --){
		{
			CC_tblock CC_tblock0;
			CC_accept(CC_9result_id,&CC_tblock0,0,0, 1);
			{

				fprintf ((&_iob [2]),"Returning child: %.0f\n",((CC_9result *)CC_tblock0)->arg1 );

				factorial *=((CC_9result *)CC_tblock0)->arg1 ;
				CC_treturn(1);
				goto CC_treturn0;

			}
CC_warn2:
			CC_treturn(2);
CC_treturn0:;
		}

	}
	printf ("%1d factorial is %.0f.\n",n ,factorial );
	CC_complete();
}

/* PROCESS BODY product */
typedef struct  {
	long int  arg1;
	long int  arg2;
	CC_pid  arg3;
} 
DD_cr_product;
DD_cr_product DD_create_product;

CC_pid CC_product(start,stride,mom, CC_priority, DD_createId)
long int  start;
long int  stride;
CC_pid  mom;
int CC_priority;
unsigned DD_createId;
{

	long int i ;
	double answer ;

	DD_create_product.arg1 = start;
	DD_create_product.arg2 = stride;
	DD_create_product.arg3 = mom;
	DD_createArgs = (char *)&DD_create_product;

	if (CC_fork("product",CC_product_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	fprintf ((&_iob [2]),"starting child, %ld, %ld\n",start ,stride );

	answer =1.0;
	for (i =start ;i >0;i -=stride ){
		answer *=i ;
	}
	(CC_tcallinit(mom ,CC_9result_id),
	((CC_9result *)CC_mytblock)->arg1= answer ,
	CC_tcall(1),
	((CC_9result *)CC_mytblock)->value) ;
	CC_complete();
}

CC_main (argc ,argv )
int argc ;
char *argv [];
{
	long int n ;

	if (argc >1){
		sscanf (argv [1],"%d",&n );
		CC_parent (n ,0, 2 );
	}
	else {
		fprintf ((&_iob [2]),"Usage: fact n\n");
	}
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
static DD_formals DD_formals_parent = {
	"n"};
static DD_formals DD_formals_product = {
	"start", "stride", "mom"};
static DD_formals DD_acceptFormals_9result_1 = {
	"p"};
static DD_occurs DD_creates = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"product", 52, sizeof DD_create_product, DD_numAnd(DD_formals_product)	}
	,
	{
		"parent", 102, sizeof DD_create_parent, DD_numAnd(DD_formals_parent)	}
};
static DD_occurs DD_accepts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"result", 58, sizeof(CC_9result), DD_numAnd(DD_acceptFormals_9result_1)	}
};
static DD_occurs DD_transacts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"result", 90, sizeof(CC_9result), 0, 0	}
};
static DD_selectOccurs DD_selects = {
	{
		0, 0, 0	}
};
static DD_otherOccurs DD_delays = {
	{
		0	}
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
		65, sizeof ((CC_9result *)DD_dummy)->value	}
	,
	{
		66, sizeof ((CC_9result *)DD_dummy)->value	}
};
static DD_symEntry DD_SymbolTable[] = {

	{
		"_iobuf", 1, 8, 0, 1	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"parent", 1, 9, 0, 3	}
	,
	{
		"product", 1, 11, 0, 3	}
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
		"1", 9, 6, 0, 9	}
	,
	{
		"result", 9, 10, 0, 14	}
	,
	{
		"0", 10, 1, 0, 0	}
	,
	{
		"1", 10, 6, 0, 10	}
	,
	{
		"=", 10, 6, 0, 14	}
	,
	{
		"1", 11, 6, 0, 9	}
	,
	{
		"2", 11, 6, 0, 9	}
	,
	{
		"3", 11, 9, 0, 3	}
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
		case 2:
			sprintf(answer, "%ld", 
			((DD_cr_parent *)tblock)->arg1);
			return;
		case 1:
			sprintf(answer, "%ld,%ld,%d", 
			((DD_cr_product *)tblock)->arg1,
			((DD_cr_product *)tblock)->arg2,
			((DD_cr_product *)tblock)->arg3);
			return;
		}

	case DD_askAccept:
		id = DD_Acc(id);
	case DD_askTrans: 
		switch (id) {
		case 1:
			case DD_Acc(1):
			sprintf(answer, "%g", 
			((CC_9result *)tblock)->arg1);
			return;
		}

	case DD_askTreturn: 
		switch (id) {
		case 1:
			sprintf(answer, "%d", 
			((CC_9result *)tblock)->value);
			return;
		}
	}
}
