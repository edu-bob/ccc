#include <ccc/CC_kernel.h>

# line 1 fooltoterm.cc

/* PROCESS SPEC a */
CC_pid CC_a();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 12 fooltoterm.cc
	    value;
	int arg1;
} 
CC_8x;
#define CC_8x_id 0

# line 12 fooltoterm.cc

#define CC_a_numtrans 1

# line 13 fooltoterm.cc

/* PROCESS SPEC b */
CC_pid CC_b();
#define CC_b_numtrans 0

# line 15 fooltoterm.cc

/* PROCESS BODY a */
CC_pid CC_a(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 18 fooltoterm.cc

	int zzz ;

	if (CC_fork("a",CC_a_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 21 fooltoterm.cc
	{
		CC_tblock CC_tblock0;

# line 21 fooltoterm.cc

# line 21 fooltoterm.cc
		CC_accept(CC_8x_id,&CC_tblock0,0,0, 1);

# line 21 fooltoterm.cc
CC_treturn0: 
		CC_treturn(1);
	}

# line 21 fooltoterm.cc

	{
		CC_tblock CC_tblock1;

# line 22 fooltoterm.cc

# line 22 fooltoterm.cc
		CC_accept(CC_8x_id,&CC_tblock1,0,0, 2);
		{
			goto CC_treturn1;

		}
CC_treturn1: 
		CC_treturn(2);
	}

# line 24 fooltoterm.cc

	{
		CC_tblock CC_tblock2;

# line 25 fooltoterm.cc

# line 25 fooltoterm.cc
		CC_accept(CC_8x_id,&CC_tblock2,0,0, 3);

# line 25 fooltoterm.cc
		{
			{
				((CC_8x *)CC_tblock2)->value =  -1
# line 26 fooltoterm.cc
				    ;
				goto CC_treturn2;
			}

		}
CC_treturn2: 
		CC_treturn(3);
	}

# line 27 fooltoterm.cc

	{
		CC_tblock CC_tblock3;

# line 28 fooltoterm.cc

# line 28 fooltoterm.cc
		CC_accept(CC_8x_id,&CC_tblock3,0,0, 4);

# line 28 fooltoterm.cc
		{
			int j ;

# line 30 fooltoterm.cc
			j =((CC_8x *)CC_tblock3)->arg1 ;
		}
CC_treturn3: 
		CC_treturn(4);
	}

# line 31 fooltoterm.cc

	CC_delay((double)(
# line 32 fooltoterm.cc
	3.0), 1);

	for (
# line 33 fooltoterm.cc
zzz =5;
# line 33 fooltoterm.cc
zzz <8;
# line 33 fooltoterm.cc
zzz ++)
	    {
		CC_tblock CC_tblock4;

# line 34 fooltoterm.cc

# line 34 fooltoterm.cc
		CC_accept(CC_8x_id,&CC_tblock4,1,0, 5);
		while(CC_acceptby((double)(
# line 34 fooltoterm.cc
		((CC_8x *)CC_tblock4)->arg1 
# line 34 fooltoterm.cc
		    )));
		{
			int j 
# line 35 fooltoterm.cc
			    ;
			j =((CC_8x *)CC_tblock4)->arg1 
# line 36 fooltoterm.cc
			    ;
		}
CC_treturn4: 
		CC_treturn(5);
	}

# line 37 fooltoterm.cc

	for (;;){
		printf ("in the select loop.\n")
# line 39 fooltoterm.cc
			;
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
# line 40 fooltoterm.cc
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8x_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
# line 41 fooltoterm.cc
				{
					CC_tblock CC_tblock5;

# line 41 fooltoterm.cc

# line 41 fooltoterm.cc
					CC_accept(CC_8x_id,&CC_tblock5,0,1, 6);

# line 41 fooltoterm.cc
					{
						int j ;

# line 43 fooltoterm.cc
						j =((CC_8x *)CC_tblock5)->arg1 ;

# line 44 fooltoterm.cc
						printf ("and yet another accept, arg=%d.\n",j );
					}
CC_treturn5: 
					CC_treturn(6);
				}

# line 45 fooltoterm.cc

				break; 
# line 46 fooltoterm.cc
CC_guard1_2 :
				if (1) CC_terminate_exists=1; 
				goto CC_guard1_3;

# line 47 fooltoterm.cc
				break; 
# line 47 fooltoterm.cc

			}
			goto CC_endselect1;
CC_guard1_3: 
			goto CC_choose1;
CC_endselect1:;
		}

# line 48 fooltoterm.cc

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

# line 53 fooltoterm.cc

	if (CC_fork("b",CC_b_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 54 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= arg ,
	CC_tcall(1),
	((CC_8x *)CC_mytblock)->value) 
# line 54 fooltoterm.cc
		;
	CC_complete();
}

CC_main ()
{
	CC_pid A 
# line 59 fooltoterm.cc
	    ;
	int ret ;

# line 62 fooltoterm.cc
	A =CC_a (0, 1 );

# line 63 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(2),
	((CC_8x *)CC_mytblock)->value) ;

# line 64 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(3),
	((CC_8x *)CC_mytblock)->value) ;

# line 65 fooltoterm.cc
	ret =(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 1,
	CC_tcall(4),
	((CC_8x *)CC_mytblock)->value) ;

# line 66 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 666,
	CC_tcall(5),
	((CC_8x *)CC_mytblock)->value) ;

# line 67 fooltoterm.cc
	CC_b (A ,444,0, 2 );
	CC_delay((double)(
# line 68 fooltoterm.cc
	0.25), 2);

# line 69 fooltoterm.cc
	CC_b (A ,333,0, 3 );
	CC_delay((double)(
# line 70 fooltoterm.cc
	0.25), 3);

# line 71 fooltoterm.cc
	CC_b (A ,222,0, 4 );
	CC_delay((double)(
# line 72 fooltoterm.cc
	0.25), 4);

# line 73 fooltoterm.cc
	CC_b (A ,111,0, 5 );
	CC_delay((double)(
# line 74 fooltoterm.cc
	0.25), 5);

	CC_delay((double)(
# line 75 fooltoterm.cc
	4.00), 6);

# line 76 fooltoterm.cc
	printf ("by now they should all be accepted...\n");

# line 77 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9,
	CC_tcall(6),
	((CC_8x *)CC_mytblock)->value) ;

# line 78 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 99,
	CC_tcall(7),
	((CC_8x *)CC_mytblock)->value) ;

# line 79 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 999,
	CC_tcall(8),
	((CC_8x *)CC_mytblock)->value) ;

# line 80 fooltoterm.cc
	(CC_tcallinit(A ,CC_8x_id),
	((CC_8x *)CC_mytblock)->arg1= 9999,
	CC_tcall(9),
	((CC_8x *)CC_mytblock)->value) ;

# line 81 fooltoterm.cc
	printf ("all done!\n");
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
