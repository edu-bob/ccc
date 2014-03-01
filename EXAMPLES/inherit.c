#include <ccc/CC_kernel.h>

# line 1 inherit.cc

/* PROCESS SPEC print_character */
CC_pid CC_print_character();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 18 inherit.cc
	    value;
	char arg1;
} 
CC_8printchar;
#define CC_8printchar_id 0

# line 18 inherit.cc

#define CC_print_character_numtrans 1

# line 19 inherit.cc

/* PROCESS SPEC print_string */
*CC_pid CC_print_string();
#define CC_print_string_numtrans 0

# line 21 inherit.cc

/* PROCESS BODY print_string */
CC_pid CC_print_string(p ,s ,
CC_priority, DD_createId)
CC_pid  p;
char  s;
int CC_priority;
unsigned DD_createId;
{

# line 24 inherit.cc

	if (CC_fork("print_string",CC_print_string_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 25 inherit.cc
	while (*s 
# line 25 inherit.cc
	    )
		(CC_tcallinit(p ,CC_8printchar_id),
		((CC_8printchar *)CC_mytblock)->arg1= *s ++,
		CC_tcall(1),
		((CC_8printchar *)CC_mytblock)->value) 
# line 26 inherit.cc
			;
	CC_complete();
}

/* PROCESS SPEC print_one_way */

#define CC_print_one_way_numtrans CC_print_character_numtrans

# line 29 inherit.cc

/* PROCESS BODY print_one_way */
CC_pid CC_print_one_way(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 32 inherit.cc

	if (CC_fork("print_one_way",CC_print_one_way_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 33 inherit.cc
	for (;;)
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
# line 34 inherit.cc
CC_guard1_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8printchar_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
# line 35 inherit.cc
			{
				CC_tblock CC_tblock0;

# line 35 inherit.cc

# line 35 inherit.cc
				CC_accept(CC_8printchar_id,&CC_tblock0,0,1, 1);

# line 35 inherit.cc
				{
					putchar (((CC_8printchar *)CC_tblock0)->arg1 )
# line 36 inherit.cc
						;
				}
CC_treturn0: 
				CC_treturn(1);
			}

# line 37 inherit.cc

			break; 
CC_guard1_2 :
# line 38 inherit.cc

			if (1) CC_terminate_exists=1; 
			goto CC_guard1_3;

# line 39 inherit.cc
			break; 

		}
		goto CC_endselect1;
CC_guard1_3: 
		goto CC_choose1;
CC_endselect1:;
	}

# line 40 inherit.cc

	CC_complete();
}

/* PROCESS SPEC print_another_way */

#define CC_print_another_way_numtrans CC_print_character_numtrans

# line 47 inherit.cc

/* PROCESS BODY print_another_way */
CC_pid CC_print_another_way(
CC_priority, DD_createId)

# line 50 inherit.cc
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("print_another_way",CC_print_another_way_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 51 inherit.cc
	for (;;)
	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose(2)) {
# line 52 inherit.cc
CC_guard2_1 :
# line 52 inherit.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8printchar_id,1)))
				goto CC_choose2;
			goto CC_guard2_2;
		case 1:
# line 53 inherit.cc
			{
				CC_tblock CC_tblock1;

# line 53 inherit.cc

# line 53 inherit.cc
				CC_accept(CC_8printchar_id,&CC_tblock1,0,1, 2);
				{

# line 54 inherit.cc
					printf ("%c",((CC_8printchar *)CC_tblock1)->arg1 );
				}
CC_treturn1: 
				CC_treturn(2);
			}

# line 55 inherit.cc

			break; 
# line 56 inherit.cc
CC_guard2_2 :
			if (1) CC_terminate_exists=1; 
			goto CC_guard2_3;

# line 57 inherit.cc
			break; 
# line 57 inherit.cc

		}
		goto CC_endselect2;
CC_guard2_3: 
		goto CC_choose2;
CC_endselect2:;
	}

# line 58 inherit.cc

	CC_complete();
}

CC_main ()
{

# line 63 inherit.cc
	CC_print_string (CC_print_one_way (0, 2 ),"hi there!\n",0, 2 );

# line 64 inherit.cc
	CC_print_string (CC_print_another_way (0, 4 ),"hello!\n",0, 4 );
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
