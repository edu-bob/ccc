#include <ccc/CC_kernel.h>
#include <ccc/DD_kernel.h>

# line 1 "mandel.cc"

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

typedef char bool ;

typedef struct {
	double a ,b ;
}
comp ;

/* PROCESS SPEC raster */
CC_pid CC_raster();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
} 
CC_10new;
#define CC_10new_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	int arg1;
} 
CC_10put;
#define CC_10put_id 1

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 value;
} 
CC_10get;
#define CC_10get_id 2

#define CC_raster_numtrans 3

/* PROCESS SPEC display */
CC_pid CC_display();
#define CC_display_numtrans 0

/* PROCESS SPEC mandel */
CC_pid CC_mandel();
#define CC_mandel_numtrans 0

typedef int  CC_shtypedef0;
CC_shtypedef0 *raster[256];

int Concurrency ;

char *A ;
CC_main (argc ,argv )
int argc ;
char *argv [];
{
	int i ;
	comp min ,max ;

	CC_pid memsrvr ;
	CC_pid m [128];

	A =argv [0];
	if (argc >1)
		Concurrency =atoi (argv [1]);
	else 
	    Concurrency =5;
	min .a  =0-2.0;
	min .b  =0-1.25;
	max .a  =0.5;
	max .b  =1.25;

	memsrvr =CC_raster (0, 1 );

	for (i =0;i <Concurrency ;i ++)
		m [i ]=CC_mandel (i ,memsrvr ,min ,max ,0, 2 );
}

/* PROCESS BODY mandel */
typedef struct  {
	int  arg1;
	CC_pid  arg2;
	comp  arg3;
	comp  arg4;
} 
DD_cr_mandel;
DD_cr_mandel DD_create_mandel;

CC_pid CC_mandel(i,srvr,min,max, CC_priority, DD_createId)
int  i;
CC_pid  srvr;
comp  min;
comp  max;
int CC_priority;
unsigned DD_createId;
{

	int line ,r ;
	int res ;
	comp pt ;

	DD_create_mandel.arg1 = i;
	DD_create_mandel.arg2 = srvr;
	DD_create_mandel.arg3 = min;
	DD_create_mandel.arg4 = max;
	DD_createArgs = (char *)&DD_create_mandel;

	if (CC_fork("mandel",CC_mandel_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	while ((line =(CC_tcallinit(srvr ,CC_10new_id),
	CC_tcall(1),
	((CC_10new *)CC_mytblock)->value) )>=0){
		for (r =0;r <256;r ++){
			pt .a  =min .a  +(max .a  -min .a  )/256*r ;
			pt .b  =min .b  +(max .b  -min .b  )/256*line ;
			raster [line ][r ]=domandel (pt );
		}
		(CC_tcallinit(srvr ,CC_10put_id),
		((CC_10put *)CC_mytblock)->arg1= line ,
		CC_tcall(2),
		((CC_10put *)CC_mytblock)->value) ;
	}
	CC_complete();
}

/* PROCESS BODY raster */

CC_pid CC_raster( CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

	int queue [256],qhead ,qtail ;
	int next ;
	int line ;
	bool more ;
	CC_pid d ;

	if (CC_fork("raster",CC_raster_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	next =0;
	qhead =0;
	qtail =0;

	d =CC_display (c_mypid (),0, 3 );
	more =1;
	while (more ){
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_10new_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
				{
					CC_tblock CC_tblock0;
					CC_accept(CC_10new_id,&CC_tblock0,0,1, 1);
					{
						if (next <256)
						{
							((CC_10new *)CC_tblock0)->value = next ++;
							CC_treturn(1);
							goto CC_treturn0;
						}

						else 
						    {
							((CC_10new *)CC_tblock0)->value =  -1;
							CC_treturn(2);
							goto CC_treturn0;
						}

					}
CC_warn3:
					CC_treturn(3);
CC_treturn0:;
				}

				break; 
CC_guard1_2 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_10put_id,2)))
					goto CC_choose1;
				goto CC_guard1_3;
			case 2:
				{
					CC_tblock CC_tblock1;
					CC_accept(CC_10put_id,&CC_tblock1,0,1, 2);
					{
						queue [qtail ++]=((CC_10put *)CC_tblock1)->arg1 ;
					}
CC_warn4:
					CC_treturn(4);
CC_treturn1:;
				}

				break; 
CC_guard1_3 :
				if ((qhead <qtail ) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_10get_id,3)))
					goto CC_choose1;
				goto CC_guard1_4;
			case 3:
				{
					CC_tblock CC_tblock2;
					CC_accept(CC_10get_id,&CC_tblock2,0,1, 3);
					{
						{
							((CC_10get *)CC_tblock2)->value = queue [qhead ++];
							CC_treturn(5);
							goto CC_treturn2;
						}

					}
CC_warn6:
					CC_treturn(6);
CC_treturn2:;
				}

				break; 
CC_guard1_4 :
				if ((qhead >=256) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_10get_id,4)))
					goto CC_choose1;
				goto CC_guard1_5;
			case 4:
				{
					CC_tblock CC_tblock3;
					CC_accept(CC_10get_id,&CC_tblock3,0,1, 4);
					{
						more =0;
						{
							((CC_10get *)CC_tblock3)->value =  -1;
							CC_treturn(7);
							goto CC_treturn3;
						}

					}
CC_warn8:
					CC_treturn(8);
CC_treturn3:;
				}

				break; 
CC_guard1_5 :
				if (1) CC_terminate_exists=1; 
				goto CC_guard1_6;
				break; 

			}
			goto CC_endselect1;
CC_guard1_6: 
			goto CC_choose1;
CC_endselect1:;
		}

	}
	CC_complete();
}

/* PROCESS BODY display */
typedef struct  {
	CC_pid  arg1;
} 
DD_cr_display;
DD_cr_display DD_create_display;

CC_pid CC_display(srvr, CC_priority, DD_createId)
CC_pid  srvr;
int CC_priority;
unsigned DD_createId;
{

	int i ,line ;
	char buf [256+1];

	DD_create_display.arg1 = srvr;
	DD_createArgs = (char *)&DD_create_display;

	if (CC_fork("display",CC_display_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	BG_push_context ();
	init_patterns ();
	BG_cursor_pos (0,0);
	BG_erase_display (0);
	BG_shift_out ();
	fflush ((&_iob [1]));
	while ((line =(CC_tcallinit(srvr ,CC_10get_id),
	CC_tcall(3),
	((CC_10get *)CC_mytblock)->value) )>=0){
		for (i =0;i <256;i ++)
			if (raster [line ][i ]>=1000)
				buf [i ]='9';
			else 
			    buf [i ]=(raster [line ][i ]%9)+'0';
		buf [i ]=0;
		printf ("\033[%d;1H%s",line +1,buf );
		fflush ((&_iob [1]));

	}
	BG_shift_in ();
	BG_pop_context ();
	fflush ((&_iob [1]));
	CC_complete();
}

domandel (pt )
comp pt ;
{
	int count ;
	double zr ,zi ,newr ;
	zr =zi =0.0;
	for (count =0;count <1000;count ++){
		if (zi *zi +zr *zr >4.0)
			break ;
		newr =zr *zr -zi *zi +pt .a  ;
		zi =2.0*zr *zi +pt .b  ;
		zr =newr ;
	}
	return count ;

}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	CC_n=(256);
	CC_ss = (256) * sizeof(**raster);
	CC_s = CC_shmalloc(CC_n * CC_ss);
	for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
		(raster)[CC_j] = (CC_shtypedef0 *) CC_s;
}
static DD_formals DD_formals_display = {
	"srvr"};
static DD_formals DD_formals_mandel = {
	"i", "srvr", "min", "max"};
static DD_formals DD_acceptFormals_10put_2 = {
	"line"};
static DD_occurs DD_creates = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"raster", 60,0, 0	}
	,
	{
		"mandel", 63, sizeof DD_create_mandel, DD_numAnd(DD_formals_mandel)	}
	,
	{
		"display", 103, sizeof DD_create_display, DD_numAnd(DD_formals_display)	}
};
static DD_occurs DD_accepts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"new", 107, sizeof(CC_10new), 0, 0	}
	,
	{
		"put", 114, sizeof(CC_10put), DD_numAnd(DD_acceptFormals_10put_2)	}
	,
	{
		"get", 118, sizeof(CC_10get), 0, 0	}
	,
	{
		"get", 122, sizeof(CC_10get), 0, 0	}
};
static DD_occurs DD_transacts = {
	{
		"", 0, 0, 0, 0	}
	,
	{
		"new", 72, sizeof(CC_10new), 0, 0	}
	,
	{
		"put", 78, sizeof(CC_10put), 0, 0	}
	,
	{
		"get", 142, sizeof(CC_10get), 0, 0	}
};
static DD_selectOccurs DD_selects = {
	{
		0, 0, 0	}
	,{
		106, 127, 0	}
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
		109, sizeof ((CC_10new *)DD_dummy)->value	}
	,
	{
		111, sizeof ((CC_10new *)DD_dummy)->value	}
	,
	{
		112, sizeof ((CC_10new *)DD_dummy)->value	}
	,
	{
		116, sizeof ((CC_10put *)DD_dummy)->value	}
	,
	{
		119, sizeof ((CC_10get *)DD_dummy)->value	}
	,
	{
		120, sizeof ((CC_10get *)DD_dummy)->value	}
	,
	{
		124, sizeof ((CC_10get *)DD_dummy)->value	}
	,
	{
		125, sizeof ((CC_10get *)DD_dummy)->value	}
};
static DD_symEntry DD_SymbolTable[] = {

	{
		"9    ", 1, 9, 0, 1	}
	,
	{
		"_iobuf", 1, 8, 0, 1	}
	,
	{
		"anytype", 1, 4, 0, 3	}
	,
	{
		"display", 1, 14, 0, 3	}
	,
	{
		"mandel", 1, 15, 0, 3	}
	,
	{
		"raster", 1, 10, 0, 3	}
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
		"a", 9, 6, 0, 10	}
	,
	{
		"b", 9, 6, 0, 10	}
	,
	{
		"0", 10, 0, 0, 0	}
	,
	{
		"get", 10, 13, 0, 14	}
	,
	{
		"new", 10, 11, 0, 14	}
	,
	{
		"put", 10, 12, 0, 14	}
	,
	{
		"0", 11, 0, 0, 0	}
	,
	{
		"=", 11, 6, 0, 14	}
	,
	{
		"0", 12, 1, 0, 0	}
	,
	{
		"1", 12, 6, 0, 14	}
	,
	{
		"=", 12, 6, 0, 14	}
	,
	{
		"0", 13, 0, 0, 0	}
	,
	{
		"=", 13, 6, 0, 14	}
	,
	{
		"0", 14, 1, 0, 0	}
	,
	{
		"1", 14, 10, 0, 3	}
	,
	{
		"0", 15, 4, 0, 0	}
	,
	{
		"1", 15, 6, 0, 14	}
	,
	{
		"2", 15, 10, 0, 3	}
	,
	{
		"3", 15, 9, 0, 1	}
	,
	{
		"4", 15, 9, 0, 1	}
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
			return;
		case 3:
			sprintf(answer, "%d", 
			((DD_cr_display *)tblock)->arg1);
			return;
		case 2:
			sprintf(answer, "%d,%d,{%g,%g},{%g,%g}", 
			((DD_cr_mandel *)tblock)->arg1,
			((DD_cr_mandel *)tblock)->arg2,

			((DD_cr_mandel *)tblock)->arg3.a,
			((DD_cr_mandel *)tblock)->arg3.b
			    ,

			((DD_cr_mandel *)tblock)->arg4.a,
			((DD_cr_mandel *)tblock)->arg4.b
			    );
			return;
		}

	case DD_askAccept:
		id = DD_Acc(id);
	case DD_askTrans: 
		switch (id) {
		case 1:
			case DD_Acc(1):
			answer = "";
		case 2:
			case DD_Acc(2):
			sprintf(answer, "%d", 
			((CC_10put *)tblock)->arg1);
			return;
		case 3:
			case DD_Acc(3):
			case DD_Acc(4):
			answer = "";
		}

	case DD_askTreturn: 
		switch (id) {
		case 1:
			sprintf(answer, "%d", 
			((CC_10new *)tblock)->value);
			return;
		case 2:
			sprintf(answer, "%d", 
			((CC_10put *)tblock)->value);
			return;
		case 3:
			sprintf(answer, "%d", 
			((CC_10get *)tblock)->value);
			return;
		}
	}
}
