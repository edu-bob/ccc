#include <ccc/CC_kernel.h>

# line 1 bettersort.cc

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

# line 8 bettersort.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 *s
# line 11 bettersort.cc

# line 11 bettersort.cc
;

minisort (aa ,zz )
int aa 
# line 14 bettersort.cc
,zz ;
{
	int temp 
# line 16 bettersort.cc
	    ;

	if (s [aa ]>s [zz ]
# line 18 bettersort.cc
	    ){
		temp =s [aa ]
# line 19 bettersort.cc
		    ;
		s [aa ]=s [zz ]
# line 20 bettersort.cc
		    ;
		s [zz ]=temp 
# line 21 bettersort.cc
		    ;
	}
}

int partition (aa ,az ,za ,zz )
int aa 
# line 26 bettersort.cc
,*az ,*za 
# line 26 bettersort.cc
,zz ;
{
	int r 
# line 28 bettersort.cc
	    ;

	r =(s [aa ]+s [aa +1]+s [zz -1]+s [zz ])/4
# line 31 bettersort.cc
	    ;
	while (aa <=zz 
# line 32 bettersort.cc
	    ){
		while (s [aa ]<r 
# line 33 bettersort.cc
		    )
			aa ++
# line 34 bettersort.cc
			    ;
		while (s [zz ]>r 
# line 35 bettersort.cc
		    )
			zz --
# line 36 bettersort.cc
			    ;
		if (aa <=zz 
# line 37 bettersort.cc
		    ){
			minisort (aa ,zz )
# line 38 bettersort.cc
				;
			aa ++
# line 39 bettersort.cc
			    ;
			zz --
# line 40 bettersort.cc
			    ;
		}
	}
	*az =zz 
# line 43 bettersort.cc
	    ;
	*za =aa 
# line 44 bettersort.cc
	    ;
}

sequential_sorter (aa ,zz )
int aa 
# line 48 bettersort.cc
,zz ;
{
	int az 
# line 50 bettersort.cc
	    ,za ;

# line 52 bettersort.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 53 bettersort.cc
	az -aa <2)

# line 54 bettersort.cc
		minisort (aa ,az );
	else 

# line 56 bettersort.cc
	sequential_sorter (aa ,az );
	if (
# line 57 bettersort.cc
	zz -za <2)

# line 58 bettersort.cc
		minisort (za ,zz );
	else 

# line 60 bettersort.cc
	sequential_sorter (za ,zz );
}

/* PROCESS SPEC parallel_sorter */
CC_pid CC_parallel_sorter();
#define CC_parallel_sorter_numtrans 0

# line 63 bettersort.cc

/* PROCESS BODY parallel_sorter */
CC_pid CC_parallel_sorter(aa ,zz ,makechildren ,
CC_priority, DD_createId)

# line 66 bettersort.cc
int  aa;
int  zz;
int  makechildren;
int CC_priority;
unsigned DD_createId;
{

	int az 
# line 67 bettersort.cc
	    ,za ;

	if (CC_fork("parallel_sorter",CC_parallel_sorter_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 69 bettersort.cc

# line 69 bettersort.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 70 bettersort.cc
	makechildren ){
		if (
# line 71 bettersort.cc
		az -aa <2)

# line 72 bettersort.cc
			minisort (aa ,az );
		else 

# line 74 bettersort.cc
		CC_parallel_sorter (aa ,az ,makechildren -1,0, 1 );
		if (
# line 75 bettersort.cc
		zz -za <2)

# line 76 bettersort.cc
			minisort (za ,zz );
		else 

# line 78 bettersort.cc
		CC_parallel_sorter (za ,zz ,makechildren -1,0, 2 );
	}
	else {

# line 84 bettersort.cc
		sequential_sorter (aa ,az );

# line 85 bettersort.cc
		sequential_sorter (za ,zz );

	}
	CC_complete();
}

CC_main (argc ,argv )

# line 93 bettersort.cc
int argc ;
char *argv []
# line 94 bettersort.cc
;
{
	int j ,length 
# line 96 bettersort.cc
	    =0
# line 96 bettersort.cc
	    ,children ;
	CC_pid top_sorter 
# line 97 bettersort.cc
	    ;

	if (argc >1
# line 99 bettersort.cc
	    )
		children =atoi (argv [1])
# line 100 bettersort.cc
			;
	else 
	    children =3
# line 102 bettersort.cc
	    ;
	fprintf ((&_iob [2]),"Using %d processes\n",(1<<(children +1))-1)
# line 103 bettersort.cc
		;

	for (j =0
# line 107 bettersort.cc
;j <20000
# line 107 bettersort.cc
;j ++
# line 107 bettersort.cc
)
		    s [j ]=rand ()%10000
# line 108 bettersort.cc
		    ;
	length =20000
# line 109 bettersort.cc
	    ;
	if (children <0
# line 110 bettersort.cc
	    ){
		for (j =0
# line 111 bettersort.cc
;j <length 
# line 111 bettersort.cc
;j ++
# line 111 bettersort.cc
)
			    printf ("s[%d]=%d.\n",j ,s [j ])
# line 112 bettersort.cc
				;
		exit ()
# line 113 bettersort.cc
			;
	}

	top_sorter =CC_parallel_sorter (0,length -1,children ,0, 3 )
# line 122 bettersort.cc
		;
	c_wait ()
# line 123 bettersort.cc
		;

	for (j =0
# line 127 bettersort.cc
;j <length 
# line 127 bettersort.cc
;j ++
# line 127 bettersort.cc
)
		    printf ("s[%d]=%d.\n",j ,s [j ])
# line 128 bettersort.cc
			;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	s = (CC_shtypedef0 *) CC_shmalloc((20000) * sizeof(*s));
}
