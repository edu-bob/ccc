#include <ccc/CC_kernel.h>

# line 1 testshared.cc

/* PROCESS SPEC writer */
CC_pid CC_writer();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 10 testshared.cc
	    value;
} 
CC_8write1;
#define CC_8write1_id 0

typedef int CC_headvalue2;

# line 11 testshared.cc
typedef struct {
	CC_headvalue2 value;
} 
CC_8write2;
#define CC_8write2_id 1

# line 11 testshared.cc

#define CC_writer_numtrans 2

# line 12 testshared.cc

/* PROCESS SPEC reader */
CC_pid CC_reader();

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 
# line 16 testshared.cc
	    value;
} 
CC_11read;
#define CC_11read_id 0

#define CC_reader_numtrans 1

# line 17 testshared.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 
# line 19 testshared.cc
*a
# line 19 testshared.cc
;
typedef int  CC_shtypedef1;
CC_shtypedef1 
# line 20 testshared.cc
(*b);
typedef char  CC_shtypedef2;
CC_shtypedef2 
# line 21 testshared.cc
*c
# line 21 testshared.cc
;
typedef int  CC_shtypedef3;
CC_shtypedef3 
# line 22 testshared.cc
*d[2
# line 22 testshared.cc
][2
# line 22 testshared.cc
][2
# line 22 testshared.cc
]
# line 22 testshared.cc
;

/* PROCESS BODY writer */
CC_pid CC_writer(
CC_priority, DD_createId)

# line 25 testshared.cc
int CC_priority;
unsigned DD_createId;
{

	int j 
# line 26 testshared.cc
	    ,k ,l 
# line 26 testshared.cc
	    ,m ;

	if (CC_fork("writer",CC_writer_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 28 testshared.cc
	{
		CC_tblock CC_tblock0;

# line 28 testshared.cc

# line 28 testshared.cc
		CC_accept(CC_8write1_id,&CC_tblock0,0,0, 1);
		{
			for (
# line 29 testshared.cc
j =0;
# line 29 testshared.cc
j <10;
# line 29 testshared.cc
j ++)

# line 30 testshared.cc
				a [j ]=j ;

# line 31 testshared.cc
			(*b) =333;

# line 32 testshared.cc
			strcpy (c ,"hi there!");
			for (
# line 33 testshared.cc
j =0;
# line 33 testshared.cc
j <2;
# line 33 testshared.cc
j ++)
				    for (
# line 34 testshared.cc
k =0;
# line 34 testshared.cc
k <2;
# line 34 testshared.cc
k ++)
					    for (
# line 35 testshared.cc
l =0;
# line 35 testshared.cc
l <2;
# line 35 testshared.cc
l ++)
						    for (
# line 36 testshared.cc
m =0;
# line 36 testshared.cc
m <2;
# line 36 testshared.cc
m ++)

# line 37 testshared.cc
							d [j ][k ][l ][m ]=j *1000+k *100+l *10+m ;
		}
CC_treturn0: 
		CC_treturn(1);
	}

# line 38 testshared.cc

	{
		CC_tblock CC_tblock1;

# line 39 testshared.cc

# line 39 testshared.cc
		CC_accept(CC_8write2_id,&CC_tblock1,0,0, 2);
		{
			for (
# line 40 testshared.cc
j =0;
# line 40 testshared.cc
j <10;
# line 40 testshared.cc
j ++)

# line 41 testshared.cc
				a [j ]=10-j ;

# line 42 testshared.cc
			(*b) = -777;

# line 43 testshared.cc
			strcpy (c ,"123456789");
			for (
# line 44 testshared.cc
j =0;
# line 44 testshared.cc
j <2;
# line 44 testshared.cc
j ++)
				    for (
# line 45 testshared.cc
k =0;
# line 45 testshared.cc
k <2;
# line 45 testshared.cc
k ++)
					    for (
# line 46 testshared.cc
l =0;
# line 46 testshared.cc
l <2;
# line 46 testshared.cc
l ++)
						    for (
# line 47 testshared.cc
m =0;
# line 47 testshared.cc
m <2;
# line 47 testshared.cc
m ++)

# line 48 testshared.cc
							d [j ][k ][l ][m ]=j *8+k *4+l *2+m ;
		}
CC_treturn1: 
		CC_treturn(2);
	}

# line 49 testshared.cc

	CC_complete();
}

/* PROCESS BODY reader */
CC_pid CC_reader(
CC_priority, DD_createId)

# line 53 testshared.cc
int CC_priority;
unsigned DD_createId;
{

	int j 
# line 54 testshared.cc
	    ,k ,l 
# line 54 testshared.cc
	    ,m ;

	if (CC_fork("reader",CC_reader_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 56 testshared.cc
	{
		CC_tblock CC_tblock2;

# line 56 testshared.cc

# line 56 testshared.cc
		CC_accept(CC_11read_id,&CC_tblock2,0,0, 3);
		{
			for (
# line 57 testshared.cc
j =0;
# line 57 testshared.cc
j <10;
# line 57 testshared.cc
j ++)

# line 58 testshared.cc
				printf ("a[%d] = %d.\n",j ,a [j ]);

# line 59 testshared.cc
			printf ("b = %d.\n",(*b) );

# line 60 testshared.cc
			printf ("c = \"%s\"\n",c );
			for (
# line 61 testshared.cc
j =0;
# line 61 testshared.cc
j <2;
# line 61 testshared.cc
j ++)
				    for (
# line 62 testshared.cc
k =0;
# line 62 testshared.cc
k <2;
# line 62 testshared.cc
k ++)
					    for (
# line 63 testshared.cc
l =0;
# line 63 testshared.cc
l <2;
# line 63 testshared.cc
l ++)
						    for (
# line 64 testshared.cc
m =0;
# line 64 testshared.cc
m <2;
# line 64 testshared.cc
m ++)

# line 65 testshared.cc
							printf ("d[%d][%d][%d][%d] = %d.\n",j ,k ,l ,m ,d [j ][k ][l ][m ]);
		}
CC_treturn2: 
		CC_treturn(3);
	}

# line 66 testshared.cc

	CC_complete();
}

CC_main ()

# line 70 testshared.cc
{
	CC_pid r ;
	CC_pid w 
# line 72 testshared.cc
	    ;

	w =CC_writer (0, 1 )
# line 74 testshared.cc
		;
	r =CC_reader (0, 2 )
# line 75 testshared.cc
		;

	(CC_tcallinit(w ,CC_8write1_id),
	CC_tcall(1),
	((CC_8write1 *)CC_mytblock)->value) 
# line 77 testshared.cc
		;
	(CC_tcallinit(r ,CC_11read_id),
	CC_tcall(2),
	((CC_11read *)CC_mytblock)->value) 
# line 78 testshared.cc
		;

	r =CC_reader (0, 3 )
# line 81 testshared.cc
		;

	(CC_tcallinit(w ,CC_8write2_id),
	CC_tcall(3),
	((CC_8write2 *)CC_mytblock)->value) 
# line 83 testshared.cc
		;
	(CC_tcallinit(r ,CC_11read_id),
	CC_tcall(4),
	((CC_11read *)CC_mytblock)->value) 
# line 84 testshared.cc
		;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	a = (CC_shtypedef0 *) CC_shmalloc((10) * sizeof(*a));
	b = (CC_shtypedef1 *) CC_shmalloc(sizeof(*b));
	c = (CC_shtypedef2 *) CC_shmalloc((10) * sizeof(*c));
	CC_n=(2)*(2)*(2);
	CC_ss = (2) * sizeof(****d);
	CC_s = CC_shmalloc(CC_n * CC_ss);
	for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
		(**d)[CC_j] = (CC_shtypedef3 *) CC_s;
}
