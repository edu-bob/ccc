#include "CC_kernel.h"



typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
} 
CC_8service;
#define CC_8service_id 0


#define CC_grandchild_numtrans 1
CC_pid CC_grandchild_body();




typedef CC_pid CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
} 
CC_10givechild;
#define CC_10givechild_id 0


#define CC_child_numtrans 1
CC_pid CC_child_body();


CC_pid CC_grandchild_body(
CC_priority)
int CC_priority;
{


	if (CC_fork(CC_grandchild_numtrans,CC_priority))
		return CC_childpid;
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose()) {
CC_guard1_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8service_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
			{
				CC_tblock CC_tblock0;
				CC_accept(CC_8service_id,&CC_tblock0,0,1);
				{
					{
						((CC_8service *)CC_tblock0)->value = 1 ;
						goto CC_treturn0;
					}

				}
CC_treturn0: 
				CC_treturn();
			}

			break; 
CC_guard1_2 :
			if (1) CC_terminate_exists=1; 
			goto CC_guard1_3;
			break; 

		}
		goto CC_endselect1;
CC_guard1_3: 
		goto CC_choose1;
CC_endselect1:;
	}

	CC_complete();
}


CC_pid CC_child_body(
CC_priority)
int CC_priority;
{

	CC_pid ch ;


	if (CC_fork(CC_child_numtrans,CC_priority))
		return CC_childpid;
	ch = CC_grandchild_body (0);
	{
		CC_tblock CC_tblock1;
		CC_accept(CC_10givechild_id,&CC_tblock1,0,0);
		{
			{
				((CC_10givechild *)CC_tblock1)->value = ch ;
				goto CC_treturn1;
			}

		}
CC_treturn1: 
		CC_treturn();
	}

	CC_complete();
}


CC_main ()
{
	CC_pid ch ;
	CC_pid gch ;

	ch = CC_child_body (0);
	gch = (CC_tcallinit(ch ,CC_10givechild_id),
	CC_tcall(),
	((CC_10givechild *)CC_mytblock)->value) ;
	CC_delay((double)(1.0 ));

	(CC_tcallinit(gch ,CC_8service_id),
	CC_tcall(),
	((CC_8service *)CC_mytblock)->value) ;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss,CC_c[64];
	char *CC_s;

}
