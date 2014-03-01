#include <ccc/CC_kernel.h>

# line 1 tcall.cc

/* PROCESS SPEC x */
CC_pid CC_x();

typedef float CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 10 tcall.cc
	    value;
} 
CC_8t;
#define CC_8t_id 0

#define CC_x_numtrans 1

# line 11 tcall.cc

float count 
# line 13 tcall.cc
=0.90
# line 13 tcall.cc
;

/* PROCESS BODY x */
CC_pid CC_x(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 16 tcall.cc

	if (CC_fork("x",CC_x_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 17 tcall.cc
	for (;;){
		CC_delay((double)(count 
# line 18 tcall.cc
		    ), 1);

		count +=0.01
# line 19 tcall.cc
		    ;
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
# line 20 tcall.cc
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8t_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
# line 21 tcall.cc
				{
					CC_tblock CC_tblock0;

# line 21 tcall.cc

# line 21 tcall.cc
					CC_accept(CC_8t_id,&CC_tblock0,0,1, 1);
					{
						{
							((CC_8t *)CC_tblock0)->value = 
# line 22 tcall.cc
							    count ;
							goto CC_treturn0;
						}

# line 22 tcall.cc

					}
CC_treturn0: 
					CC_treturn(1);
				}

# line 23 tcall.cc

				break; 
CC_guard1_2 :
# line 24 tcall.cc

				if (1) CC_terminate_exists=1; 
				goto CC_guard1_3;

# line 25 tcall.cc
				break; 

			}
			goto CC_endselect1;
CC_guard1_3: 
			goto CC_choose1;
CC_endselect1:;
		}

# line 26 tcall.cc

	}
	CC_complete();
}

CC_main ()

# line 31 tcall.cc
{
	CC_pid X ;
	float c 
# line 33 tcall.cc
	    ;

	X =CC_x (0, 1 )
# line 35 tcall.cc
		;
	while ((c =(CC_remember_time( 1.0),
	CC_tcallinit(X ,CC_8t_id),
	(CC_timedtcall(1) ?
	((CC_8t *)CC_mytblock)->value :
	(  -1.0))) )>=0
# line 36 tcall.cc
	    )
		printf ("I was accepted after %f seconds.\n",c )
# line 37 tcall.cc
			;
	printf ("I got impatient and timed out.\n")
# line 38 tcall.cc
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
