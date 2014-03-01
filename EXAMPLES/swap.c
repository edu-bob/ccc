#include <ccc/CC_kernel.h>

# line 1 swap.cc

/* PROCESS SPEC agent */
CC_pid CC_agent();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	int *arg1;
} 
CC_8swap_A;
#define CC_8swap_A_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	int arg1;
} 
CC_8swap_B;
#define CC_8swap_B_id 1

#define CC_agent_numtrans 2

/* PROCESS BODY agent */
CC_pid CC_agent(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("agent",CC_agent_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	for (;;){
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8swap_A_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
				{
					CC_tblock CC_tblock0;
					CC_accept(CC_8swap_A_id,&CC_tblock0,0,1, 1);
					{
						int temp ;
						{
							CC_tblock CC_tblock1;
							CC_accept(CC_8swap_A_id,&CC_tblock1,0,0, 2);
							{
								temp =*((CC_8swap_A *)CC_tblock1)->arg1 ;
								*((CC_8swap_A *)CC_tblock1)->arg1 =*((CC_8swap_A *)CC_tblock0)->arg1 ;
							}
CC_treturn1: 
							CC_treturn(1);
						}

						*((CC_8swap_A *)CC_tblock0)->arg1 =temp ;
					}
CC_treturn0: 
					CC_treturn(2);
				}

				break; 
CC_guard1_2 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8swap_B_id,2)))
					goto CC_choose1;
				goto CC_guard1_3;
			case 2:
				{
					CC_tblock CC_tblock2;
					CC_accept(CC_8swap_B_id,&CC_tblock2,0,1, 3);
					{
						int temp ;
						{
							CC_tblock CC_tblock3;
							CC_accept(CC_8swap_B_id,&CC_tblock3,0,0, 4);
							{
								temp =((CC_8swap_B *)CC_tblock3)->arg1 ;
								{
									((CC_8swap_B *)CC_tblock3)->value = ((CC_8swap_B *)CC_tblock2)->arg1 ;
									goto CC_treturn3;
								}

							}
CC_treturn3: 
							CC_treturn(3);
						}

						{
							((CC_8swap_B *)CC_tblock2)->value = temp ;
							goto CC_treturn2;
						}

					}
CC_treturn2: 
					CC_treturn(4);
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

	}
	CC_complete();
}

/* PROCESS SPEC sema */
CC_pid CC_sema();

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 value;
} 
CC_11P;
#define CC_11P_id 0

typedef int CC_headvalue4;
typedef struct {
	CC_headvalue4 value;
} 
CC_11V;
#define CC_11V_id 1

#define CC_sema_numtrans 2

/* PROCESS BODY sema */
CC_pid CC_sema(count ,
CC_priority, DD_createId)
int  count;
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("sema",CC_sema_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	for (;;){
		{
			CC_selectinit();
			goto CC_guard2_1; 
CC_choose2: 
			switch(CC_choose(2)) {
CC_guard2_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_11V_id,1)))
					goto CC_choose2;
				goto CC_guard2_2;
			case 1:
				{
					CC_tblock CC_tblock4;
					CC_accept(CC_11V_id,&CC_tblock4,0,1, 5);
CC_treturn4: 
					CC_treturn(5);
				}

				count ++;
				break; 
CC_guard2_2 :
				if ((count >0) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_11P_id,2)))
					goto CC_choose2;
				goto CC_guard2_3;
			case 2:
				{
					CC_tblock CC_tblock5;
					CC_accept(CC_11P_id,&CC_tblock5,0,1, 6);
CC_treturn5: 
					CC_treturn(6);
				}

				count --;
				break; 
CC_guard2_3 :
				if (1) CC_terminate_exists=1; 
				goto CC_guard2_4;
				break; 

			}
			goto CC_endselect2;
CC_guard2_4: 
			goto CC_choose2;
CC_endselect2:;
		}

	}
	CC_complete();
}

CC_pid printsema ;
CC_pid x ;

/* PROCESS SPEC trader */
CC_pid CC_trader();
#define CC_trader_numtrans 0

/* PROCESS BODY trader */
CC_pid CC_trader(oddp ,
CC_priority, DD_createId)
int  oddp;
int CC_priority;
unsigned DD_createId;
{

	int j ;
	typedef int  CC_shtypedef0;
	CC_shtypedef0 (*s);
	int list_A [20][2],list_B [20][2];

	{
		int CC_j,CC_n,CC_ss;
		char *CC_s;

		s = (CC_shtypedef0 *) CC_shmalloc(sizeof(*s));
	}

	if (CC_fork("trader",CC_trader_numtrans,CC_priority,DD_createId))
		return CC_childpid;
	for (j =0;j <20;j ++){
		list_A [j ][0]=oddp +2*j ;
		(*s) =list_A [j ][0];
		(CC_tcallinit(x ,CC_8swap_A_id),
		((CC_8swap_A *)CC_mytblock)->arg1= &(*s) ,
		CC_tcall(1),
		((CC_8swap_A *)CC_mytblock)->value) ;
		list_A [j ][1]=(*s) ;
	}
	for (j =0;j <20;j ++){
		list_B [j ][0]=oddp +2*j ;
		list_B [j ][1]=(CC_tcallinit(x ,CC_8swap_B_id),
		((CC_8swap_B *)CC_mytblock)->arg1= list_B [j ][0],
		CC_tcall(2),
		((CC_8swap_B *)CC_mytblock)->value) ;
	}
	(CC_tcallinit(printsema ,CC_11P_id),
	CC_tcall(3),
	((CC_11P *)CC_mytblock)->value) ;
	printf ("\nHi, I'm %s!\n",(oddp ?"odd":"even"));
	printf ("using transaction swap_A:\n");
	for (j =0;j <20;j ++)
		printf ("exchanged %d for %d\n",list_A [j ][0],list_A [j ][1]);
	printf ("using transaction swap_B:\n");
	for (j =0;j <20;j ++)
		printf ("exchanged %d for %d\n",list_B [j ][0],list_B [j ][1]);
	(CC_tcallinit(printsema ,CC_11V_id),
	CC_tcall(4),
	((CC_11V *)CC_mytblock)->value) ;
	CC_complete();
}

CC_main ()
{
	CC_pid even ,odd ;

	printsema =CC_sema (1,0, 1 );
	x =CC_agent (0, 2 );
	even =CC_trader (0,0, 3 );
	odd =CC_trader (1,0, 4 );
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
