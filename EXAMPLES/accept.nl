#include <ccc/CC_kernel.h>


/* PROCESS SPEC a */
CC_pid CC_a();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
	    value;
	int arg1;
} 
CC_8x;
#define CC_8x_id 0


#define CC_a_numtrans 1


/* PROCESS SPEC b */
CC_pid CC_b();
#define CC_b_numtrans 0


/* PROCESS BODY a */
CC_pid CC_a(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{


	int zzz ;

	if (CC_fork("a",CC_a_numtrans,CC_priority,DD_createId))
		return CC_childpid;

	{
		CC_tblock CC_tblock0;


		CC_accept(CC_8x_id,&CC_tblock0,0,0, 1);

CC_treturn0: 
		CC_treturn(1);
	}


	{
		CC_tblock CC_tblock1;


		CC_accept(CC_8x_id,&CC_tblock1,0,0, 2);
		{

			printf ("In the second accept.\n");
			goto CC_treturn1;

		}
CC_treturn1: 
		CC_treturn(2);
	}


	{
		CC_tblock CC_tblock2;


		CC_accept(CC_8x_id,&CC_tblock2,0,0, 3);

		{
			printf ("In the third accept, returning -1.\n")
				;
			{
				((CC_8x *)CC_tblock2)->value =  -1
				    ;
				goto CC_treturn2;
			}

		}
CC_treturn2: 
		CC_treturn(3);
	}


	{
		CC_tblock CC_tblock3;


		CC_accept(CC_8x_id,&CC_tblock3,0,0, 4);

		{
			int j ;

			j =((CC_8x *)CC_tblock3)->arg1 ;

			printf ("In the fourth accept, we were passed %d.\n",j );
		}
CC_treturn3: 
		CC_treturn(4);
	}


	printf ("We are going to wait three seconds while some requests queue up.\n");
	CC_delay((double)(
	3.0), 1);

	printf ("Done waiting.\n");
	for (
zzz =5;
zzz <8;
zzz ++)
	    {
		CC_tblock CC_tblock4;


		CC_accept(CC_8x_id,&CC_tblock4,1,0, 5);
		while(CC_acceptby((double)(
		((CC_8x *)CC_tblock4)->arg1 
		    )));
		{
			int j 
			    ;
			j =((CC_8x *)CC_tblock4)->arg1 
			    ;
			printf ("accept number %d gets arg %d.\n",zzz ,j )
				;
		}
CC_treturn4: 
		CC_treturn(5);
	}


	for (;;){
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8x_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
				{
					CC_tblock CC_tblock5;


					CC_accept(CC_8x_id,&CC_tblock5,0,1, 6);

					{
						int j ;

						j =((CC_8x *)CC_tblock5)->arg1 ;

						printf ("and yet another accept, arg=%d.\n",j );
					}
CC_treturn5: 
					CC_treturn(6);
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


	}
	CC_complete();
}

/* PROCESS BODY b */
CC_pid CC_b(A ,arg ,
CC_priority, DD_createId)
CC_pid  A;
int  arg;
int CC_priority;
unsigned DD_createId;
{


	if (CC_fork("b",CC_b_numtrans,CC_priority,DD_createId))
		return CC_childpid;

	printf ("just created, about to call A with arg=%d.\n",arg )
		;
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= arg ,
	CC_tcall(1),
	((CC_8x *)CC_mytblock)->value) 
		;
	CC_complete();
}

CC_main ()
{
	CC_pid A 
	    ;
	int ret ;

	A =CC_a (0, 1 );

	printf ("calling first accept.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(2),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("calling second accept.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(3),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("calling third accept.\n");

	ret =(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(4),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("third accept returned %d.\n",ret );

	printf ("calling fourth accept, arg=666.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 666,
	CC_tcall(5),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("creating processes to call A with differing args.\n");

	CC_b (A ,444,0, 2 );
	CC_delay((double)(
	0.25), 2);

	CC_b (A ,333,0, 3 );
	CC_delay((double)(
	0.25), 3);

	CC_b (A ,222,0, 4 );
	CC_delay((double)(
	0.25), 4);

	CC_b (A ,111,0, 5 );
	CC_delay((double)(
	0.25), 5);

	printf ("created four calling tasks.\n");
	CC_delay((double)(
	4.00), 6);

	printf ("by now they should all be accepted...\n");

	printf ("calling A with arg=9.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9,
	CC_tcall(6),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("calling A with arg=99.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 99,
	CC_tcall(7),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("calling A with arg=999.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 999,
	CC_tcall(8),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("calling A with arg=9999.\n");

	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9999,
	CC_tcall(9),
	((CC_8x *)CC_mytblock)->value) ;

	printf ("all done!\n");
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
