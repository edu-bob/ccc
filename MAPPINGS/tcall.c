#include <ccc/CC_kernel.h>
static char *DD_convert;

# line 1 "tcall.cc"

/* PROCESS SPEC myproc */
CC_pid CC_myproc();

typedef float CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	char arg1;
	short arg2;
} 
CC_8first;
#define CC_8first_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	int arg1;
} 
CC_8second;
#define CC_8second_id 1

#define CC_myproc_numtrans 2

CC_main ()
{
	CC_pid myproc ;
	myproc =CC_myproc (0, 1 );
	(CC_tcallinit(myproc ,CC_8first_id),
	((CC_8first *)CC_mytblock)->arg1= 'a',
	((CC_8first *)CC_mytblock)->arg2= 017777,
	CC_tcall(1),
	((CC_8first *)CC_mytblock)->value) ;
	(CC_remember_time( 5),
	CC_tcallinit(myproc ,CC_8second_id),
	((CC_8second *)CC_mytblock)->arg1= 26,
	(CC_timedtcall(2) ?
	((CC_8second *)CC_mytblock)->value :
	( 27))) ;
}
/* PROCESS BODY myproc */

CC_pid CC_myproc( CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

	char ch ;
	short sh ;

	if (CC_fork("myproc",CC_myproc_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	{
		CC_tblock CC_tblock0;
		CC_accept(CC_8first_id,&CC_tblock0,0,0, 1);
		{
			((CC_8first *)CC_tblock0)->arg1 =((CC_8first *)CC_tblock0)->arg2 /2;
		}
CC_warn1:
		CC_treturn(1);
CC_treturn0:;
	}

	CC_complete();
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
