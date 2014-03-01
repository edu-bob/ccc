#include <ccc/CC_kernel.h>



extern struct _iobuf {
	int _cnt ;
	char *_ptr ;
	char *_base ;
	int _bufsiz ;
	short _flag ;
	char _file ;
}
_iob [20
];

struct _iobuf *fopen ()
;
struct _iobuf *fdopen ();
struct _iobuf *freopen ()
;
long ftell ();
char *gets ()
;
char *fgets ();
char *sprintf ()
;


typedef int  CC_shtypedef0;
CC_shtypedef0 *s

;

minisort (aa ,zz )
int aa 
,zz ;
{
	int temp 
	    ;

	if (s [aa ]>s [zz ]
	    ){
		temp =s [aa ]
		    ;
		s [aa ]=s [zz ]
		    ;
		s [zz ]=temp 
		    ;
	}

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |temp                0       0       6       0                           |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       0                           |
 *  |zz                  0       0       6       0                           |
 */

int partition (aa ,az ,za ,zz )
int aa 
,*az ,*za 
,zz ;
{
	int r 
	    ;

	r =s [(aa +zz )/2]
	    ;
	while (aa <=zz 
	    ){
		while (s [aa ]<r 
		    )
			aa ++
			    ;
		while (s [zz ]>r 
		    )
			zz --
			    ;
		if (aa <=zz 
		    ){
			minisort (aa ,zz )
				;
			aa ++
			    ;
			zz --
			    ;
		}
	}
	*az =zz 
	    ;
	*za =aa 
	    ;

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |r                   0       0       6       0                           |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       0                           |
 *  |az                  0       5       6       0                           |
 *  |za                  0       5       6       0                           |
 *  |zz                  0       0       6       0                           |
 */

sequential_sorter (aa ,zz )
int aa 
,zz ;
{
	int az 
	    ,za ;

	partition (aa ,&az ,&za ,zz );
	if (
	az -aa <2)

		minisort (aa ,az );
	else 

	sequential_sorter (aa ,az );
	if (
	zz -za <2)

		minisort (za ,zz );
	else 

	sequential_sorter (za ,zz );

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |az                  0       0       6       0                           |
	 *  |za                  0       0       6       0                           |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |aa                  0       0       6       0                           |
 *  |zz                  0       0       6       0                           |
 */

/* PROCESS SPEC parallel_sorter */
CC_pid CC_parallel_sorter();

typedef int CC_headvalue1;

typedef struct {
	CC_headvalue1 value;
} 
CC_9i_am_done;
#define CC_9i_am_done_id 0


typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 
	    value;
} 
CC_9are_you_done;
#define CC_9are_you_done_id 1

#define CC_parallel_sorter_numtrans 2


/* PROCESS BODY parallel_sorter */
CC_pid CC_parallel_sorter(aa ,zz ,parent ,makechildren ,
CC_priority)

int  aa;
int  zz;
CC_pid  parent;
int  makechildren;
int CC_priority;
{

	int az 
	    ,za ;

	if (CC_fork("parallel_sorter",CC_parallel_sorter_numtrans,CC_priority))
		return CC_childpid;


	partition (aa ,&az ,&za ,zz );
	if (
	makechildren ){
		if (
		az -aa <2)

			minisort (aa ,az );
		else 

		CC_parallel_sorter (aa ,az ,c_mypid (),makechildren -1,0);
		if (
		zz -za <2)

			minisort (za ,zz );
		else 

		CC_parallel_sorter (za ,zz ,c_mypid (),makechildren -1,0);
		if (
		az -aa >=2)
		{
			CC_tblock CC_tblock0;


			CC_accept(CC_9i_am_done_id,&CC_tblock0,0,0);

			/*
			 *  exiting block in which the following were declared:
			 *  |name                context chain   num     obj     alias               |
			 */
CC_treturn0: 
			CC_treturn();
		}


		if (
		zz -za >=2)
		{
			CC_tblock CC_tblock1;


			CC_accept(CC_9i_am_done_id,&CC_tblock1,0,0);

			/*
			 *  exiting block in which the following were declared:
			 *  |name                context chain   num     obj     alias               |
			 */
CC_treturn1: 
			CC_treturn();
		}


	}
	else {

		sequential_sorter (aa ,az );

		sequential_sorter (za ,zz );
	}
	if (
	parent !=c_nullpid )

		(CC_tcallinit(parent ,CC_9i_am_done_id),
		CC_tcall(),
		((CC_9i_am_done *)CC_mytblock)->value) ;
	else 
	    {
		CC_tblock CC_tblock2;


		CC_accept(CC_9are_you_done_id,&CC_tblock2,0,0);

		/*
		 *  exiting block in which the following were declared:
		 *  |name                context chain   num     obj     alias               |
		 */
CC_treturn2: 
		CC_treturn();
	}


	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |az                  0       0       6       0                           |
	 *  |c_mypid             0       25      9       3                           |
	 *  |za                  0       0       6       0                           |
	 */
	CC_complete();
}

CC_main (argc ,argv )

int argc ;
char *argv []
;
{
	int j ,length 
	    =0
	    ,children ;
	CC_pid top_sorter 
	    ;

	if (argc >1
	    )
		children =atoi (argv [1])
			;
	else 
	    children =3
	    ;
	fprintf ((&_iob [2]),"Using %d processes\n",(2<<(children +1))-1)
		;

	for (j =0
;j <5000
;j ++
)
		    s [j ]=rand ()%10000
		    ;
	length =5000
	    ;

	top_sorter =CC_parallel_sorter (0,length -1,c_nullpid ,children ,0)
		;
	(CC_tcallinit(top_sorter ,CC_9are_you_done_id),
	CC_tcall(),
	((CC_9are_you_done *)CC_mytblock)->value) 
		;

	for (j =0
;j <length 
;j ++
)
		    printf ("s[%d]=%d.\n",j ,s [j ])
			;

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |atoi                0       25      6       0                           |
	 *  |children            0       0       6       0                           |
	 *  |fprintf             0       25      6       0                           |
	 *  |j                   0       0       6       0                           |
	 *  |length              0       0       6       0                           |
	 *  |printf              0       25      6       0                           |
	 *  |rand                0       25      6       0                           |
	 *  |top_sorter          0       0       9       3                           |
	 */
}
/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |argc                0       0       6       0                           |
 *  |argv                0       6       6       0                           |
 */

/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |CC_main             0       25      6       0                           |
 *  |_iob                0       5       8       1                           |
 *  |aa                  0       0       6       0                           |
 *  |c_abort             0       25      6       0                           |
 *  |c_active            0       25      6       0                           |
 *  |c_associate         0       25      6       0                           |
 *  |c_changepriority    0       25      6       0                           |
 *  |c_completed         0       25      6       0                           |
 *  |c_getpriority       0       25      6       0                           |
 *  |c_invalid           0       25      6       0                           |
 *  |c_nullpid           0       0       5       3                           |
 *  |fdopen              0       46      8       1                           |
 *  |fgets               0       46      6       0                           |
 *  |fopen               0       46      8       1                           |
 *  |freopen             0       46      8       1                           |
 *  |ftell               0       25      6       0                           |
 *  |gets                0       46      6       0                           |
 *  |makechildren        0       0       6       0                           |
 *  |minisort            0       25      6       0                           |
 *  |parent              0       0       9       3                           |
 *  |partition           0       25      6       0                           |
 *  |s                   0       5       6       0                           |
 *  |sequential_sorter   0       25      6       0                           |
 *  |sprintf             0       46      6       0                           |
 *  |zz                  0       0       6       0                           |
 *  |_iobuf              1       0       8       1                           |
 *  |anytype             1       0       4       3                           |
 *  |parallel_sorter     1       0       9       3                           |
 *  |_base               8       5       6       0                           |
 *  |_bufsiz             8       0       6       0                           |
 *  |_cnt                8       0       6       0                           |
 *  |_file               8       0       6       0                           |
 *  |_flag               8       0       6       0                           |
 *  |_ptr                8       5       6       0                           |
 *  |1                   9       0       6       0                           |
 *  |2                   9       0       6       0                           |
 *  |3                   9       0       9       3                           |
 *  |4                   9       0       6       0                           |
 *  |are_you_done        9       0       11      0                           |
 *  |i_am_done           9       0       10      0                           |
 *  |0                   10      0       0       0                           |
 *  |=                   10      0       6       0                           |
 *  |0                   11      0       0       0                           |
 *  |=                   11      0       6       0                           |
 */
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	s = (CC_shtypedef0 *) CC_shmalloc((5000) * sizeof(*s));
}
