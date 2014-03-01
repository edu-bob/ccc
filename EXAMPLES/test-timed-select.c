#include <ccc/CC_kernel.h>

# line 1 test-timed-select.cc

float frandom ()
{
	long  random ()
# line 10 test-timed-select.cc
		;
	static float twotothe31 =
# line 11 test-timed-select.cc
	    256.0*256.0*256.0*128.0;

	return 
# line 13 test-timed-select.cc
	    random ()/twotothe31 ;

# line 13 test-timed-select.cc

}

/* PROCESS SPEC s */
CC_pid CC_s();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 18 test-timed-select.cc
	    value;
	int arg1;
} 
CC_8t;
#define CC_8t_id 0

# line 18 test-timed-select.cc

#define CC_s_numtrans 1

# line 19 test-timed-select.cc

/* PROCESS BODY s */
CC_pid CC_s(time ,
CC_priority, DD_createId)
float  time;
int CC_priority;
unsigned DD_createId;
{

# line 22 test-timed-select.cc

	if (CC_fork("s",CC_s_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 23 test-timed-select.cc
	for (;;)
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
# line 24 test-timed-select.cc
CC_guard1_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8t_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
# line 25 test-timed-select.cc
			{
				CC_tblock CC_tblock0;

# line 25 test-timed-select.cc

# line 25 test-timed-select.cc
				CC_accept(CC_8t_id,&CC_tblock0,0,1, 1);

# line 25 test-timed-select.cc
				{
					printf ("selected accept.\n",((CC_8t *)CC_tblock0)->arg1 )
# line 26 test-timed-select.cc
						;
				}
CC_treturn0: 
				CC_treturn(1);
			}

# line 27 test-timed-select.cc

			break; 
CC_guard1_2 :
# line 28 test-timed-select.cc

			if ((CC_trydelay=(time 
# line 29 test-timed-select.cc
			    ))>=0)
				if (!CC_dchoice || CC_bestdelay>CC_trydelay) {
					CC_dchoice=2;
					CC_bestdelay=CC_trydelay;
				}
			goto CC_guard1_3;
		case 2:
# line 29 test-timed-select.cc

			printf ("selected delay.\n")
# line 30 test-timed-select.cc
				;
			break; 
CC_guard1_3 :
# line 31 test-timed-select.cc

			if (1) CC_terminate_exists=1; 
			goto CC_guard1_4;

# line 32 test-timed-select.cc
			break; 

		}
		goto CC_endselect1;
CC_guard1_4: 
		goto CC_choose1;
CC_endselect1:;
	}

# line 33 test-timed-select.cc

	CC_complete();
}

CC_main ()

# line 37 test-timed-select.cc
{
	CC_pid p ;
	float time 
# line 39 test-timed-select.cc
	    ;
	int j ;

# line 42 test-timed-select.cc
	p =CC_s (2.0,0, 1 );
	for (
# line 43 test-timed-select.cc
j =0;
# line 43 test-timed-select.cc
j <10;
# line 43 test-timed-select.cc
j ++){

# line 44 test-timed-select.cc
		time =1.5+1.0*frandom ();
		CC_delay((double)(
# line 45 test-timed-select.cc
		time ), 2);

# line 46 test-timed-select.cc
		(CC_tcallinit(p ,CC_8t_id),
		((CC_8t *)CC_mytblock)->arg1= j ,
		CC_tcall(1),
		((CC_8t *)CC_mytblock)->value) ;

# line 47 test-timed-select.cc
		printf ("That tcall was made after a delay of %f seconds.\n",time );
	}
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
