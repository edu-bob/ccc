#include <ccc/CC_kernel.h>

# line 1 bettor.cc

# line 1 /usr/include/stdio.h

extern struct _iobuf {
	int _cnt ;
	char *_ptr ;
	char *_base ;
	int _bufsiz ;
	short _flag ;
	char _file ;
}
_iob [20
# line 26 /usr/include/stdio.h
];

struct _iobuf *fopen ()
# line 55 /usr/include/stdio.h
;
struct _iobuf *fdopen ();
struct _iobuf *freopen ()
# line 57 /usr/include/stdio.h
;
long	 ftell ();
char *gets ()
# line 59 /usr/include/stdio.h
;
char *fgets ();
char *sprintf ()
# line 61 /usr/include/stdio.h
;

# line 10 bettor.cc

/* PROCESS SPEC bettor */
CC_pid CC_bettor();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 15 bettor.cc
	    value;
	CC_pid arg1;
	int arg2;

# line 15 bettor.cc
	int arg3;
} 
CC_9init;
#define CC_9init_id 0

# line 15 bettor.cc

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 
# line 16 bettor.cc
	    value;
	int arg1;
} 
CC_9placebet;
#define CC_9placebet_id 1

# line 16 bettor.cc

#define CC_bettor_numtrans 2

# line 17 bettor.cc

/* PROCESS BODY bettor */
CC_pid CC_bettor(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 20 bettor.cc

	int i_am_first ,mylimit 
# line 21 bettor.cc
	    ;
	int mynextbet =
# line 22 bettor.cc
	    1,oplastbet 
# line 22 bettor.cc
	    =1
# line 22 bettor.cc
	    ;
	CC_pid opponent ;

	if (CC_fork("bettor",CC_bettor_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 25 bettor.cc
	{
		CC_tblock CC_tblock0;

# line 25 bettor.cc

# line 25 bettor.cc

# line 25 bettor.cc
		CC_accept(CC_9init_id,&CC_tblock0,0,0, 1);

# line 25 bettor.cc
		{
			opponent =((CC_9init *)CC_tblock0)->arg1 
# line 26 bettor.cc
			    ;
			i_am_first =((CC_9init *)CC_tblock0)->arg2 
# line 27 bettor.cc
			    ;
			mylimit =((CC_9init *)CC_tblock0)->arg3 
# line 28 bettor.cc
			    ;
		}
CC_treturn0: 
		CC_treturn(1);
	}

# line 29 bettor.cc

	if (i_am_first 
# line 30 bettor.cc
	    ){
		(CC_tcallinit(opponent ,CC_9placebet_id),
		((CC_9placebet *)CC_mytblock)->arg1= mynextbet ,
		CC_tcall(1),
		((CC_9placebet *)CC_mytblock)->value) 
# line 31 bettor.cc
			;
		printf ("I begin the game by betting $%d.\n",mynextbet )
# line 32 bettor.cc
			;
	}
	while (mynextbet >0&&oplastbet >0
# line 34 bettor.cc
	    ){
		{
			CC_tblock CC_tblock1;

# line 35 bettor.cc

# line 35 bettor.cc
			CC_accept(CC_9placebet_id,&CC_tblock1,0,0, 2);
			{

# line 36 bettor.cc
				oplastbet =((CC_9placebet *)CC_tblock1)->arg1 ;
			}
CC_treturn1: 
			CC_treturn(2);
		}

# line 37 bettor.cc

		if (
# line 38 bettor.cc
		oplastbet >0){

# line 39 bettor.cc
			mynextbet =oplastbet +1+rand ()%100;
			if (
# line 40 bettor.cc
			i_am_first )

# line 41 bettor.cc

				printf ("I just got a bet of $%d, so I will bet $%d.\n",oplastbet ,mynextbet );
			if (
# line 43 bettor.cc
			mynextbet >1000){

# line 44 bettor.cc
				mynextbet =0;
				if (
# line 45 bettor.cc
				i_am_first )

# line 46 bettor.cc
					printf ("but that's too much so I lose.\n");
			}

# line 48 bettor.cc
			(CC_tcallinit(opponent ,CC_9placebet_id),
			((CC_9placebet *)CC_mytblock)->arg1= mynextbet ,
			CC_tcall(2),
			((CC_9placebet *)CC_mytblock)->value) ;
		}
	}
	if (
# line 51 bettor.cc
	i_am_first ){
		if (
# line 52 bettor.cc
		mynextbet >0)

# line 53 bettor.cc
			printf ("I won; last bet %d\n",mynextbet );
		else 

# line 55 bettor.cc
		printf ("I lost; last bet %d\n",oplastbet );
	}
	CC_complete();
}

CC_main ()

# line 60 bettor.cc
{
	CC_pid a ,b 
# line 61 bettor.cc
	    ;

	a =CC_bettor (0, 1 )
# line 63 bettor.cc
		;
	b =CC_bettor (0, 2 )
# line 64 bettor.cc
		;
	printf ("May the bettor process win. Good luck!\n")
# line 65 bettor.cc
		;
	(CC_tcallinit(a ,CC_9init_id),
	((CC_9init *)CC_mytblock)->arg1= b ,
	((CC_9init *)CC_mytblock)->arg2= 1,
	((CC_9init *)CC_mytblock)->arg3= 1000,
	CC_tcall(3),
	((CC_9init *)CC_mytblock)->value) 
# line 66 bettor.cc
		;
	(CC_tcallinit(b ,CC_9init_id),
	((CC_9init *)CC_mytblock)->arg1= a ,
	((CC_9init *)CC_mytblock)->arg2= 0,
	((CC_9init *)CC_mytblock)->arg3= 1000,
	CC_tcall(4),
	((CC_9init *)CC_mytblock)->value) 
# line 67 bettor.cc
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
