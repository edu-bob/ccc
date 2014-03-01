#include <ccc/CC_kernel.h>
static char *DD_convert;

# line 1 "select.cc"

/* PROCESS SPEC queue */
CC_pid CC_queue();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
} 
CC_8get;
#define CC_8get_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	int arg1;
} 
CC_8put;
#define CC_8put_id 1

#define CC_queue_numtrans 2

CC_main ()
{
	CC_pid myqueue ;
	myqueue =CC_queue (256,0, 1 );
	myqueue =CC_queue (256,(5), 2 );
}

/* PROCESS BODY queue */

CC_pid CC_queue(i, CC_priority, DD_createId)
int  i;
int CC_priority;
unsigned DD_createId;
{

	int temp ,temp2 ;

	if (CC_fork("queue",CC_queue_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
CC_guard1_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8get_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
			{
				CC_tblock CC_tblock0;
				CC_accept(CC_8get_id,&CC_tblock0,0,1, 1);
				{
					temp ++;
				}
CC_warn1:
				CC_treturn(1);
CC_treturn0:;
			}

			break; 
CC_guard1_2 :
			if ((temp >temp2 ) && 
			    (CC_accept_exists=1,CC_tryaccept(CC_8put_id,2)))
				goto CC_choose1;
			goto CC_guard1_3;
		case 2:
			{
				CC_tblock CC_tblock1;
				CC_accept(CC_8put_id,&CC_tblock1,0,1, 2);
				{
					((CC_8put *)CC_tblock1)->arg1 --;
				}
CC_warn2:
				CC_treturn(2);
CC_treturn1:;
			}

			break; 
CC_guard1_3 :
			if (1) CC_terminate_exists=1; 
			goto CC_guard1_4;
			break; 

		}
		goto CC_endselect1;
CC_guard1_4: 
		goto CC_choose1;
CC_endselect1:;
	}

	CC_complete();
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
