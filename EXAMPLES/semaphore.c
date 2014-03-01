#include <ccc/CC_kernel.h>

# line 1 semaphore.cc

/* PROCESS SPEC sema */
CC_pid CC_sema();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 13 semaphore.cc
	    value;
} 
CC_8V;
#define CC_8V_id 0

typedef int CC_headvalue2;

# line 14 semaphore.cc
typedef struct {
	CC_headvalue2 value;
} 
CC_8P;
#define CC_8P_id 1

# line 14 semaphore.cc

#define CC_sema_numtrans 2

# line 15 semaphore.cc

/* PROCESS BODY sema */
CC_pid CC_sema(count ,
CC_priority, DD_createId)
int  count;
int CC_priority;
unsigned DD_createId;
{

# line 18 semaphore.cc

	if (CC_fork("sema",CC_sema_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 19 semaphore.cc
	for (;;){
		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
# line 20 semaphore.cc
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8V_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
# line 21 semaphore.cc
				{
					CC_tblock CC_tblock0;

# line 21 semaphore.cc

# line 21 semaphore.cc
					CC_accept(CC_8V_id,&CC_tblock0,0,1, 1);
CC_treturn0: 
					CC_treturn(1);
				}

# line 21 semaphore.cc

# line 22 semaphore.cc
				count ++;
				break; 
# line 23 semaphore.cc
CC_guard1_2 :
				if ((
# line 23 semaphore.cc
				count >0) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_8P_id,2)))
					goto CC_choose1;
				goto CC_guard1_3;
			case 2:
# line 24 semaphore.cc
				{
					CC_tblock CC_tblock1;

# line 24 semaphore.cc

# line 24 semaphore.cc
					CC_accept(CC_8P_id,&CC_tblock1,0,1, 2);
CC_treturn1: 
					CC_treturn(2);
				}

# line 24 semaphore.cc

# line 25 semaphore.cc
				count --;
				break; 
# line 26 semaphore.cc
CC_guard1_3 :
				if (1) CC_terminate_exists=1; 
				goto CC_guard1_4;

# line 27 semaphore.cc
				break; 
# line 27 semaphore.cc

			}
			goto CC_endselect1;
CC_guard1_4: 
			goto CC_choose1;
CC_endselect1:;
		}

# line 28 semaphore.cc

	}
	CC_complete();
}

/* PROCESS SPEC printer */
CC_pid CC_printer();
#define CC_printer_numtrans 0

# line 32 semaphore.cc

/* PROCESS BODY printer */
CC_pid CC_printer(s ,c ,
CC_priority, DD_createId)
CC_pid  s;
char  c;
int CC_priority;
unsigned DD_createId;
{

# line 35 semaphore.cc

	int j ;

	if (CC_fork("printer",CC_printer_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 38 semaphore.cc
	for (
# line 38 semaphore.cc
j =0;;
# line 38 semaphore.cc
j ++){

# line 39 semaphore.cc
		(CC_tcallinit(s ,CC_8P_id),
		CC_tcall(1),
		((CC_8P *)CC_mytblock)->value) ;

# line 40 semaphore.cc
		printf ("   %c%c%c\n",c ,c ,c );
		CC_delay((double)(
# line 41 semaphore.cc
		0.1), 1);

# line 42 semaphore.cc
		printf ("   %c%c%c\n",c ,c ,c );
		CC_delay((double)(
# line 43 semaphore.cc
		0.1), 2);

# line 44 semaphore.cc
		printf ("   %c%c%c\n",c ,c ,c );
		CC_delay((double)(
# line 45 semaphore.cc
		0.1), 3);

# line 46 semaphore.cc
		printf ("%c%c%c%c%c%c%c%c%c\n",c ,c ,c ,c ,c ,c ,c ,c ,c );
		CC_delay((double)(
# line 47 semaphore.cc
		0.1), 4);

# line 48 semaphore.cc
		printf (" %c%c%c%c%c%c%c\n",c ,c ,c ,c ,c ,c ,c );
		CC_delay((double)(
# line 49 semaphore.cc
		0.1), 5);

# line 50 semaphore.cc
		printf ("  %c%c%c%c%c\n",c ,c ,c ,c ,c );
		CC_delay((double)(
# line 51 semaphore.cc
		0.1), 6);

# line 52 semaphore.cc
		printf ("   %c%c%c\n",c ,c ,c );
		CC_delay((double)(
# line 53 semaphore.cc
		0.1), 7);

# line 54 semaphore.cc
		printf ("    %c\n",c );
		CC_delay((double)(
# line 55 semaphore.cc
		0.1), 8);

# line 56 semaphore.cc
		printf ("\n");
		if (
# line 57 semaphore.cc
		c =='a')
			CC_delay((double)(
# line 58 semaphore.cc
			0.15), 9);

# line 59 semaphore.cc
		(CC_tcallinit(s ,CC_8V_id),
		CC_tcall(2),
		((CC_8V *)CC_mytblock)->value) ;
	}
	CC_complete();
}

CC_main (argc )

# line 64 semaphore.cc
int argc ;
{
	CC_pid p1 
# line 66 semaphore.cc
	    ,p2 ;
	CC_pid s 
# line 67 semaphore.cc
	    ;

	if (argc >1
# line 69 semaphore.cc
	    )
		s =CC_sema (2,0, 1 )
# line 70 semaphore.cc
			;
	else 
	    s =CC_sema (1,0, 2 )
# line 72 semaphore.cc
		;
	p1 =CC_printer (s ,'a',0, 3 )
# line 73 semaphore.cc
		;
	p2 =CC_printer (s ,'b',0, 4 )
# line 74 semaphore.cc
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
