#include <ccc/CC_kernel.h>

# line 1 quicksort.cc

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

# line 4 quicksort.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 *s
# line 7 quicksort.cc

# line 7 quicksort.cc
;

minisort (aa ,zz )
int aa 
# line 10 quicksort.cc
,zz ;
{
	int temp 
# line 12 quicksort.cc
	    ;

	if (s [aa ]>s [zz ]
# line 14 quicksort.cc
	    ){
		temp =s [aa ]
# line 15 quicksort.cc
		    ;
		s [aa ]=s [zz ]
# line 16 quicksort.cc
		    ;
		s [zz ]=temp 
# line 17 quicksort.cc
		    ;
	}

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |temp                0       0       6       14                          |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       14                          |
 *  |zz                  0       0       6       14                          |
 */

int partition (aa ,az ,za ,zz )
int aa 
# line 22 quicksort.cc
,*az ,*za 
# line 22 quicksort.cc
,zz ;
{
	int r 
# line 24 quicksort.cc
	    ;

	r =s [(aa +zz )/2]
# line 26 quicksort.cc
	    ;
	while (aa <=zz 
# line 27 quicksort.cc
	    ){
		while (s [aa ]<r 
# line 28 quicksort.cc
		    )
			aa ++
# line 29 quicksort.cc
			    ;
		while (s [zz ]>r 
# line 30 quicksort.cc
		    )
			zz --
# line 31 quicksort.cc
			    ;
		if (aa <=zz 
# line 32 quicksort.cc
		    ){
			minisort (aa ,zz )
# line 33 quicksort.cc
				;
			aa ++
# line 34 quicksort.cc
			    ;
			zz --
# line 35 quicksort.cc
			    ;
		}
	}
	*az =zz 
# line 38 quicksort.cc
	    ;
	*za =aa 
# line 39 quicksort.cc
	    ;

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |r                   0       0       6       14                          |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       14                          |
 *  |az                  0       5       6       14                          |
 *  |za                  0       5       6       14                          |
 *  |zz                  0       0       6       14                          |
 */

sequential_sorter (aa ,zz )
int aa 
# line 43 quicksort.cc
,zz ;
{
	int az 
# line 45 quicksort.cc
	    ,za ;

# line 47 quicksort.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 48 quicksort.cc
	az -aa <2)

# line 49 quicksort.cc
		minisort (aa ,az );
	else 

# line 51 quicksort.cc
	sequential_sorter (aa ,az );
	if (
# line 52 quicksort.cc
	zz -za <2)

# line 53 quicksort.cc
		minisort (za ,zz );
	else 

# line 55 quicksort.cc
	sequential_sorter (za ,zz );

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |az                  0       0       6       14                          |
	 *  |za                  0       0       6       14                          |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       14                          |
 *  |zz                  0       0       6       14                          |
 */

/* PROCESS SPEC parallel_sorter */
CC_pid CC_parallel_sorter();

typedef int CC_headvalue1;

# line 60 quicksort.cc
typedef struct {
	CC_headvalue1 value;
} 
CC_9i_am_done;
#define CC_9i_am_done_id 0

# line 60 quicksort.cc

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 
# line 61 quicksort.cc
	    value;
} 
CC_9are_you_done;
#define CC_9are_you_done_id 1

#define CC_parallel_sorter_numtrans 2

# line 62 quicksort.cc

/* PROCESS BODY parallel_sorter */
CC_pid CC_parallel_sorter(aa ,zz ,parent ,makechildren ,
CC_priority, DD_createId)

# line 65 quicksort.cc
int  aa;
int  zz;
CC_pid  parent;
int  makechildren;
int CC_priority;
unsigned DD_createId;
{

	int az 
# line 66 quicksort.cc
	    ,za ;

	if (CC_fork("parallel_sorter",CC_parallel_sorter_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 68 quicksort.cc

# line 68 quicksort.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 69 quicksort.cc
	makechildren ){
		if (
# line 70 quicksort.cc
		az -aa <2)

# line 71 quicksort.cc
			minisort (aa ,az );
		else 

# line 73 quicksort.cc
		CC_parallel_sorter (aa ,az ,c_mypid (),makechildren -1,0, 1 );
		if (
# line 74 quicksort.cc
		zz -za <2)

# line 75 quicksort.cc
			minisort (za ,zz );
		else 

# line 77 quicksort.cc
		CC_parallel_sorter (za ,zz ,c_mypid (),makechildren -1,0, 2 );
		if (
# line 78 quicksort.cc
		az -aa >=2)
		{
			CC_tblock CC_tblock0;

# line 79 quicksort.cc

# line 79 quicksort.cc
			CC_accept(CC_9i_am_done_id,&CC_tblock0,0,0, 1);

			/*
			 *  exiting block in which the following were declared:
			 *  |name                context chain   num     obj     alias               |
			 */
CC_treturn0: 
			CC_treturn(1);
		}

# line 79 quicksort.cc

		if (
# line 80 quicksort.cc
		zz -za >=2)
		{
			CC_tblock CC_tblock1;

# line 81 quicksort.cc

# line 81 quicksort.cc
			CC_accept(CC_9i_am_done_id,&CC_tblock1,0,0, 2);

			/*
			 *  exiting block in which the following were declared:
			 *  |name                context chain   num     obj     alias               |
			 */
CC_treturn1: 
			CC_treturn(2);
		}

# line 81 quicksort.cc

	}
	else {

# line 83 quicksort.cc
		sequential_sorter (aa ,az );

# line 84 quicksort.cc
		sequential_sorter (za ,zz );
	}
	if (
# line 86 quicksort.cc
	parent !=c_nullpid )

# line 87 quicksort.cc
		(CC_tcallinit(parent ,CC_9i_am_done_id),
		CC_tcall(1),
		((CC_9i_am_done *)CC_mytblock)->value) ;
	else 
	    {
		CC_tblock CC_tblock2;

# line 89 quicksort.cc

# line 89 quicksort.cc
		CC_accept(CC_9are_you_done_id,&CC_tblock2,0,0, 3);

		/*
		 *  exiting block in which the following were declared:
		 *  |name                context chain   num     obj     alias               |
		 */
CC_treturn2: 
		CC_treturn(3);
	}

# line 89 quicksort.cc

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |az                  0       0       6       14                          |
	 *  |c_mypid             0       25      9       3                           |
	 *  |za                  0       0       6       14                          |
	 */
	CC_complete();
}

CC_main (argc ,argv )

# line 93 quicksort.cc
int argc ;
char *argv []
# line 94 quicksort.cc
;
{
	int j ,length 
# line 96 quicksort.cc
	    =0
# line 96 quicksort.cc
	    ,children ;
	CC_pid top_sorter 
# line 97 quicksort.cc
	    ;

	if (argc >1
# line 99 quicksort.cc
	    )
		children =atoi (argv [1])
# line 100 quicksort.cc
			;
	else 
	    children =3
# line 102 quicksort.cc
	    ;
	fprintf ((&_iob [2]),"Using %d processes\n",(2<<(children +1))-1)
# line 103 quicksort.cc
		;

	for (j =0
# line 107 quicksort.cc
;j <5000
# line 107 quicksort.cc
;j ++
# line 107 quicksort.cc
)
		    s [j ]=rand ()%10000
# line 108 quicksort.cc
		    ;
	length =5000
# line 109 quicksort.cc
	    ;

	top_sorter =CC_parallel_sorter (0,length -1,c_nullpid ,children ,0, 3 )
# line 117 quicksort.cc
		;
	(CC_tcallinit(top_sorter ,CC_9are_you_done_id),
	CC_tcall(2),
	((CC_9are_you_done *)CC_mytblock)->value) 
# line 118 quicksort.cc
		;

	for (j =0
# line 122 quicksort.cc
;j <length 
# line 122 quicksort.cc
;j ++
# line 122 quicksort.cc
)
		    printf ("s[%d]=%d.\n",j ,s [j ])
# line 123 quicksort.cc
			;

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |atoi                0       25      6       0                           |
	 *  |children            0       0       6       14                          |
	 *  |fprintf             0       25      6       0                           |
	 *  |j                   0       0       6       14                          |
	 *  |length              0       0       6       14                          |
	 *  |printf              0       25      6       0                           |
	 *  |rand                0       25      6       0                           |
	 *  |top_sorter          0       0       9       3                           |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |argc                0       0       6       14                          |
 *  |argv                0       6       6       12                          |
 */

/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |CC_main             0       25      6       0                           |
 *  |_iob                0       5       8       1                           |
 *  |aa                  0       0       6       14                          |
 *  |c_abort             0       25      6       14                          |
 *  |c_active            0       25      6       14                          |
 *  |c_associate         0       25      6       14                          |
 *  |c_changepriority    0       25      6       14                          |
 *  |c_completed         0       25      6       14                          |
 *  |c_getpriority       0       25      6       14                          |
 *  |c_invalid           0       25      6       14                          |
 *  |c_nullpid           0       0       5       3                           |
 *  |fdopen              0       46      8       1                           |
 *  |fgets               0       46      6       12                          |
 *  |fopen               0       46      8       1                           |
 *  |freopen             0       46      8       1                           |
 *  |ftell               0       25      6       9                           |
 *  |gets                0       46      6       12                          |
 *  |makechildren        0       0       6       14                          |
 *  |minisort            0       25      6       0                           |
 *  |parent              0       0       9       3                           |
 *  |partition           0       25      6       14                          |
 *  |s                   0       5       6       14                          |
 *  |sequential_sorter   0       25      6       0                           |
 *  |sprintf             0       46      6       12                          |
 *  |zz                  0       0       6       14                          |
 *  |_iobuf              1       0       8       1                           |
 *  |anytype             1       0       4       3                           |
 *  |parallel_sorter     1       0       9       3                           |
 *  |_base               8       5       6       12                          |
 *  |_bufsiz             8       0       6       14                          |
 *  |_cnt                8       0       6       14                          |
 *  |_file               8       0       6       12                          |
 *  |_flag               8       0       6       11                          |
 *  |_ptr                8       5       6       12                          |
 *  |1                   9       0       6       14                          |
 *  |2                   9       0       6       14                          |
 *  |3                   9       0       9       3                           |
 *  |4                   9       0       6       14                          |
 *  |are_you_done        9       0       11      14                          |
 *  |i_am_done           9       0       10      14                          |
 *  |0                   10      0       0       0                           |
 *  |=                   10      0       6       14                          |
 *  |0                   11      0       0       0                           |
 *  |=                   11      0       6       14                          |
 */
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	s = (CC_shtypedef0 *) CC_shmalloc((5000) * sizeof(*s));
}
