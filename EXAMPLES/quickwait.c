#include <ccc/CC_kernel.h>

# line 1 quickwait.cc

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

# line 2 quickwait.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 *s
# line 5 quickwait.cc

# line 5 quickwait.cc
;

minisort (aa ,zz )
int aa 
# line 8 quickwait.cc
,zz ;
{
	int temp 
# line 10 quickwait.cc
	    ;

	if (s [aa ]>s [zz ]
# line 12 quickwait.cc
	    ){
		temp =s [aa ]
# line 13 quickwait.cc
		    ;
		s [aa ]=s [zz ]
# line 14 quickwait.cc
		    ;
		s [zz ]=temp 
# line 15 quickwait.cc
		    ;
	}
}

int partition (aa ,az ,za ,zz )
int aa 
# line 20 quickwait.cc
,*az ,*za 
# line 20 quickwait.cc
,zz ;
{
	int r 
# line 22 quickwait.cc
	    ;

	r =s [(aa +zz )/2]
# line 24 quickwait.cc
	    ;
	while (aa <=zz 
# line 25 quickwait.cc
	    ){
		while (s [aa ]<r 
# line 26 quickwait.cc
		    )
			aa ++
# line 27 quickwait.cc
			    ;
		while (s [zz ]>r 
# line 28 quickwait.cc
		    )
			zz --
# line 29 quickwait.cc
			    ;
		if (aa <=zz 
# line 30 quickwait.cc
		    ){
			minisort (aa ,zz )
# line 31 quickwait.cc
				;
			aa ++
# line 32 quickwait.cc
			    ;
			zz --
# line 33 quickwait.cc
			    ;
		}
	}
	*az =zz 
# line 36 quickwait.cc
	    ;
	*za =aa 
# line 37 quickwait.cc
	    ;
}

sequential_sorter (aa ,zz )
int aa 
# line 41 quickwait.cc
,zz ;
{
	int az 
# line 43 quickwait.cc
	    ,za ;

# line 45 quickwait.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 46 quickwait.cc
	az -aa <2)

# line 47 quickwait.cc
		minisort (aa ,az );
	else 

# line 49 quickwait.cc
	sequential_sorter (aa ,az );
	if (
# line 50 quickwait.cc
	zz -za <2)

# line 51 quickwait.cc
		minisort (za ,zz );
	else 

# line 53 quickwait.cc
	sequential_sorter (za ,zz );
}

/* PROCESS SPEC parallel_sorter */
CC_pid CC_parallel_sorter();
#define CC_parallel_sorter_numtrans 0

# line 56 quickwait.cc

/* PROCESS BODY parallel_sorter */
CC_pid CC_parallel_sorter(aa ,zz ,makechildren ,
CC_priority, DD_createId)

# line 59 quickwait.cc
int  aa;
int  zz;
int  makechildren;
int CC_priority;
unsigned DD_createId;
{

	int az 
# line 60 quickwait.cc
	    ,za ;

	if (CC_fork("parallel_sorter",CC_parallel_sorter_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 62 quickwait.cc

# line 62 quickwait.cc
	partition (aa ,&az ,&za ,zz );
	if (
# line 63 quickwait.cc
	makechildren ){
		if (
# line 64 quickwait.cc
		az -aa <2)

# line 65 quickwait.cc
			minisort (aa ,az );
		else 

# line 67 quickwait.cc
		CC_parallel_sorter (aa ,az ,makechildren -1,0, 1 );
		if (
# line 68 quickwait.cc
		zz -za <2)

# line 69 quickwait.cc
			minisort (za ,zz );
		else 

# line 71 quickwait.cc
		CC_parallel_sorter (za ,zz ,makechildren -1,0, 2 );
	}
	else {

# line 73 quickwait.cc
		fprintf ((&_iob [2]),"sequential_sorter(%d);\n",az -aa );

# line 74 quickwait.cc
		fprintf ((&_iob [2]),"sequential_sorter(%d);\n",zz -za );

# line 75 quickwait.cc
		sequential_sorter (aa ,az );

# line 76 quickwait.cc
		sequential_sorter (za ,zz );

# line 77 quickwait.cc
		fprintf ((&_iob [2]),"done\n");
	}
	CC_complete();
}

CC_main (argc ,argv )

# line 82 quickwait.cc
int argc ;
char *argv []
# line 83 quickwait.cc
;
{
	int j ,length 
# line 85 quickwait.cc
	    =0
# line 85 quickwait.cc
	    ,children ;
	CC_pid top_sorter 
# line 86 quickwait.cc
	    ;

	if (argc >1
# line 88 quickwait.cc
	    )
		children =atoi (argv [1])
# line 89 quickwait.cc
			;
	else 
	    children =3
# line 91 quickwait.cc
	    ;
	fprintf ((&_iob [2]),"Using %d processes\n",(1<<(children +1))-1)
# line 92 quickwait.cc
		;

	for (j =0
# line 96 quickwait.cc
;j <20000
# line 96 quickwait.cc
;j ++
# line 96 quickwait.cc
)
		    s [j ]=rand ()%10000
# line 97 quickwait.cc
		    ;
	length =20000
# line 98 quickwait.cc
	    ;
	if (children <0
# line 99 quickwait.cc
	    ){
		for (j =0
# line 100 quickwait.cc
;j <length 
# line 100 quickwait.cc
;j ++
# line 100 quickwait.cc
)
			    printf ("s[%d]=%d.\n",j ,s [j ])
# line 101 quickwait.cc
				;
		exit ()
# line 102 quickwait.cc
			;
	}

	top_sorter =CC_parallel_sorter (0,length -1,children ,0, 3 )
# line 111 quickwait.cc
		;
	c_wait ()
# line 112 quickwait.cc
		;

# line 118 quickwait.cc

}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	s = (CC_shtypedef0 *) CC_shmalloc((20000) * sizeof(*s));
}
