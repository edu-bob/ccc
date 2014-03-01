#include <ccc/CC_kernel.h>

# line 1 select.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 (*guard1)
# line 10 select.cc
,(*guard2),(*guard3)
# line 10 select.cc
,(*guard4),(*guard5)
# line 10 select.cc
;

/* PROCESS SPEC fred */
CC_pid CC_fred();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 14 select.cc
	    value;
} 
CC_8t;
#define CC_8t_id 0

#define CC_fred_numtrans 1

# line 15 select.cc

/* PROCESS BODY fred */
CC_pid CC_fred(
CC_priority, DD_createId)

# line 18 select.cc
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("fred",CC_fred_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 19 select.cc
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
# line 19 select.cc
CC_guard1_1 :
# line 19 select.cc

			if ((0
# line 20 select.cc
			    ) && 
			    !CC_ichoice) CC_ichoice=1; 
			goto CC_guard1_2;
		case 1:
# line 21 select.cc

			printf ("this should not be printed.\n")
# line 22 select.cc
				;
			break; 
CC_guard1_2 :
			CC_ochoice=2; 
			goto CC_guard1_3;
		case 2:
# line 23 select.cc

# line 24 select.cc
			printf ("this should be printed.\n");
			break; 
# line 25 select.cc

		}
		goto CC_endselect1;
CC_guard1_3: 
		goto CC_choose1;
CC_endselect1:;
	}

# line 25 select.cc

	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose(2)) {
# line 26 select.cc
CC_guard2_1 :
# line 26 select.cc

			if ((1
# line 27 select.cc
			    ) && 
			    !CC_ichoice) CC_ichoice=1; 
			goto CC_guard2_2;
		case 1:
# line 28 select.cc

			printf ("this should be printed.\n")
# line 29 select.cc
				;
			break; 
CC_guard2_2 :
			CC_ochoice=2; 
			goto CC_guard2_3;
		case 2:
# line 30 select.cc

# line 31 select.cc
			printf ("this should not be printed.\n");
			break; 
# line 32 select.cc

		}
		goto CC_endselect2;
CC_guard2_3: 
		goto CC_choose2;
CC_endselect2:;
	}

# line 32 select.cc

	for (;;){
		CC_delay((double)(
# line 34 select.cc
		0.25), 1);

# line 35 select.cc
		printf ("at top of select.\n");
		{
			CC_selectinit();
			goto CC_guard3_1; 
CC_choose3: 
			switch(CC_choose(3)) {
# line 36 select.cc
CC_guard3_1 :
# line 36 select.cc

				if (((*guard1) 
# line 37 select.cc
				    ) && 
				    !CC_ichoice) CC_ichoice=1; 
				goto CC_guard3_2;
			case 1:
# line 38 select.cc

				printf ("guard 1 true, ")
# line 39 select.cc
					;
				printf ("immediate alternative chosen.\n")
# line 40 select.cc
					;
				break; 
CC_guard3_2 :
# line 41 select.cc

				if (((*guard2) 
# line 42 select.cc
				    ) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_8t_id,2)))
					goto CC_choose3;
				goto CC_guard3_3;
			case 2:
# line 43 select.cc
				{
					CC_tblock CC_tblock0;

# line 43 select.cc
					CC_accept(CC_8t_id,&CC_tblock0,0,1, 1);

# line 43 select.cc
					{
						printf ("guard2 true, transaction t is being accepted.\n")
# line 44 select.cc
							;
					}
CC_treturn0: 
					CC_treturn(1);
				}

# line 45 select.cc

				printf ("transaction t is done being accepted.\n")
# line 46 select.cc
					;
				break; 
CC_guard3_3 :
# line 47 select.cc

				if (((*guard3) 
# line 48 select.cc
				    ) && 
				    (CC_trydelay=(10.0
# line 49 select.cc
				    ))>=0)
					if (!CC_dchoice || CC_bestdelay>CC_trydelay) {
						CC_dchoice=3;
						CC_bestdelay=CC_trydelay;
					}
				goto CC_guard3_4;
			case 3:
# line 49 select.cc

				printf ("guard3 true, ten seconds have past.\n")
# line 50 select.cc
					;
				break; 
CC_guard3_4 :
# line 51 select.cc

				if (((*guard4) 
# line 52 select.cc
				    ) && 
				    (CC_trydelay=(20.0
# line 53 select.cc
				    ))>=0)
					if (!CC_dchoice || CC_bestdelay>CC_trydelay) {
						CC_dchoice=4;
						CC_bestdelay=CC_trydelay;
					}
				goto CC_guard3_5;
			case 4:
# line 53 select.cc

				printf ("guard4 true, twenty seconds have past.\n")
# line 54 select.cc
					;
				break; 
CC_guard3_5 :
# line 55 select.cc

				if (((*guard5) 
# line 56 select.cc
				    ) && 
				    1) CC_terminate_exists=1; 
				goto CC_guard3_6;

# line 57 select.cc
				break; 

			}
			goto CC_endselect3;
CC_guard3_6: 
			goto CC_choose3;
CC_endselect3:;
		}

# line 58 select.cc

	}
	CC_complete();
}

CC_main (argc )

# line 63 select.cc
int argc ;
{
	CC_pid f 
# line 65 select.cc
	    ;

	(*guard1) =0
# line 67 select.cc
	    ;
	(*guard2) =0
# line 68 select.cc
	    ;
	(*guard3) =0
# line 69 select.cc
	    ;
	(*guard4) =0
# line 70 select.cc
	    ;
	(*guard5) =(argc >1)
# line 71 select.cc
		;
	printf ("guard5 (for terminate) is now true.\n")
# line 72 select.cc
		;
	f =CC_fred (0, 1 )
# line 73 select.cc
		;
	(*guard4) =1
# line 74 select.cc
	    ;
	printf ("guard4 (for delay 2.0) is now true.\n")
# line 75 select.cc
		;
	CC_delay((double)(29.5
# line 76 select.cc
	    ), 4);

	(*guard3) =1
# line 77 select.cc
	    ;
	printf ("guard3 (for delay 1.0) is now true.\n")
# line 78 select.cc
		;
	CC_delay((double)(29.5
# line 79 select.cc
	    ), 5);

	(*guard2) =1
# line 80 select.cc
	    ;
	printf ("guard2 (for accept t) is now true.\n")
# line 81 select.cc
		;
	CC_delay((double)(29.5
# line 82 select.cc
	    ), 6);

	(CC_tcallinit(f ,CC_8t_id),
	CC_tcall(1),
	((CC_8t *)CC_mytblock)->value) 
# line 83 select.cc
		;
	CC_delay((double)(29.5
# line 84 select.cc
	    ), 7);

	(*guard1) =1
# line 85 select.cc
	    ;
	printf ("guard1 (for immediate) is now true.\n")
# line 86 select.cc
		;
	CC_delay((double)(1.0
# line 87 select.cc
	    ), 8);

	(*guard1) =0
# line 88 select.cc
	    ;
	printf ("guard1 (for immediate) is now false again.\n")
# line 89 select.cc
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	guard1 = (CC_shtypedef0 *) CC_shmalloc(sizeof(*guard1));
	guard2 = (CC_shtypedef0 *) CC_shmalloc(sizeof(*guard2));
	guard3 = (CC_shtypedef0 *) CC_shmalloc(sizeof(*guard3));
	guard4 = (CC_shtypedef0 *) CC_shmalloc(sizeof(*guard4));
	guard5 = (CC_shtypedef0 *) CC_shmalloc(sizeof(*guard5));
}
