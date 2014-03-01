#include "CC_kernel.h"










typedef int  CC_shtypedef0;
CC_shtypedef0 (*guard1)
,(*guard2),(*guard3)
,(*guard4),(*guard5)
;



typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
	    value;
} 
CC_8t;
#define CC_8t_id 0


#define CC_fred_numtrans 1
CC_pid CC_fred_body();



CC_pid CC_fred_body(
CC_priority)

int CC_priority;
{


	if (CC_fork(CC_fred_numtrans,CC_priority))
		return CC_childpid;

	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose()) {
CC_guard1_1 :

			if ((0 
			    ) && 
			    !CC_ichoice) CC_ichoice=1; 
			goto CC_guard1_2;
		case 1:

			printf ("this should not be printed.\n" )
				;
			break; 
CC_guard1_2 :
			CC_ochoice=2; 
			goto CC_guard1_3;
		case 2:


			printf ("this should be printed.\n" );
			break; 

		}
		goto CC_endselect1;
CC_guard1_3: 
		goto CC_choose1;
CC_endselect1:;
	}


	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose()) {
CC_guard2_1 :

			if ((1 
			    ) && 
			    !CC_ichoice) CC_ichoice=1; 
			goto CC_guard2_2;
		case 1:

			printf ("this should be printed.\n" )
				;
			break; 
CC_guard2_2 :
			CC_ochoice=2; 
			goto CC_guard2_3;
		case 2:


			printf ("this should not be printed.\n" );
			break; 

		}
		goto CC_endselect2;
CC_guard2_3: 
		goto CC_choose2;
CC_endselect2:;
	}


	for (;;){
		CC_delay((double)(
		0.25 ));


		printf ("at top of select.\n" );
		{
			CC_selectinit();
			goto CC_guard3_1; 
CC_choose3: 
			switch(CC_choose()) {
CC_guard3_1 :

				if (((*guard1) 
				    ) && 
				    !CC_ichoice) CC_ichoice=1; 
				goto CC_guard3_2;
			case 1:

				printf ("guard 1 true, " )
					;
				printf ("immediate alternative chosen.\n" )
					;
				break; 
CC_guard3_2 :

				if (((*guard2) 
				    ) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_8t_id,2)))
					goto CC_choose3;
				goto CC_guard3_3;
			case 2:
				{
					CC_tblock CC_tblock0;

					CC_accept(CC_8t_id,&CC_tblock0,0,1);

					{
						printf ("guard2 true, transaction t is being accepted.\n" )
							;
					}
CC_treturn0: 
					CC_treturn();
				}


				printf ("transaction t is done being accepted.\n" )
					;
				break; 
CC_guard3_3 :

				if (((*guard3) 
				    ) && 
				    (CC_trydelay=(10.0 
				    ))>=0)
					if (!CC_dchoice || CC_bestdelay>CC_trydelay) {
						CC_dchoice=3;
						CC_bestdelay=CC_trydelay;
					}
				goto CC_guard3_4;
			case 3:

				printf ("guard3 true, ten seconds have past.\n" )
					;
				break; 
CC_guard3_4 :

				if (((*guard4) 
				    ) && 
				    (CC_trydelay=(20.0 
				    ))>=0)
					if (!CC_dchoice || CC_bestdelay>CC_trydelay) {
						CC_dchoice=4;
						CC_bestdelay=CC_trydelay;
					}
				goto CC_guard3_5;
			case 4:

				printf ("guard4 true, twenty seconds have past.\n" )
					;
				break; 
CC_guard3_5 :

				if (((*guard5) 
				    ) && 
				    1) CC_terminate_exists=1; 
				goto CC_guard3_6;

				break; 

			}
			goto CC_endselect3;
CC_guard3_6: 
			goto CC_choose3;
CC_endselect3:;
		}


	}
	CC_complete();
}


CC_main (argc )

int argc ;
{
	CC_pid f 
	    ;

	(*guard1) = 0 
	    ;
	(*guard2) = 0 
	    ;
	(*guard3) = 0 
	    ;
	(*guard4) = 0 
	    ;
	(*guard5) = (argc > 1 )
		;
	printf ("guard5 (for terminate) is now true.\n" )
		;
	f = CC_fred_body (0)
		;
	(*guard4) = 1 
	    ;
	printf ("guard4 (for delay 2.0) is now true.\n" )
		;
	CC_delay((double)(29.5 
	    ));

	(*guard3) = 1 
	    ;
	printf ("guard3 (for delay 1.0) is now true.\n" )
		;
	CC_delay((double)(29.5 
	    ));

	(*guard2) = 1 
	    ;
	printf ("guard2 (for accept t) is now true.\n" )
		;
	CC_delay((double)(29.5 
	    ));

	(CC_tcallinit(f ,CC_8t_id),
	CC_tcall(),
	((CC_8t *)CC_mytblock)->value) 
		;
	CC_delay((double)(29.5 
	    ));

	(*guard1) = 1 
	    ;
	printf ("guard1 (for immediate) is now true.\n" )
		;
	CC_delay((double)(1.0 
	    ));

	(*guard1) = 0 
	    ;
	printf ("guard1 (for immediate) is now false again.\n" )
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss,CC_c[64];
	char *CC_s;

	guard1 = (CC_shtypedef0 *) shmalloc(sizeof(*guard1));
	guard2 = (CC_shtypedef0 *) shmalloc(sizeof(*guard2));
	guard3 = (CC_shtypedef0 *) shmalloc(sizeof(*guard3));
	guard4 = (CC_shtypedef0 *) shmalloc(sizeof(*guard4));
	guard5 = (CC_shtypedef0 *) shmalloc(sizeof(*guard5));
}
