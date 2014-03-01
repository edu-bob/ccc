#include <ccc/CC_kernel.h>

# line 1 fft.cc

/* PROCESS SPEC vertex */
CC_pid CC_vertex();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 11 fft.cc
	    value;
} 
CC_8start;
#define CC_8start_id 0

typedef int CC_headvalue2;

# line 12 fft.cc
typedef struct {
	CC_headvalue2 value;
} 
CC_8compute;
#define CC_8compute_id 1

# line 12 fft.cc

#define CC_vertex_numtrans 2

# line 13 fft.cc

int numproc ;
float alpha 
# line 16 fft.cc
;

typedef float  CC_shtypedef0;
CC_shtypedef0 *A
# line 18 fft.cc

# line 18 fft.cc
;
typedef CC_pid  CC_shtypedef1;
CC_shtypedef1 *pids
# line 19 fft.cc

# line 19 fft.cc
;

/* PROCESS BODY vertex */
CC_pid CC_vertex(me ,
CC_priority, DD_createId)
int  me;
int CC_priority;
unsigned DD_createId;
{

# line 22 fft.cc

	int mask ,you 
# line 23 fft.cc
	    ;
	float temp ;

	if (CC_fork("vertex",CC_vertex_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 26 fft.cc
	{
		CC_tblock CC_tblock0;

# line 26 fft.cc

# line 26 fft.cc
		CC_accept(CC_8start_id,&CC_tblock0,0,0, 1);
CC_treturn0: 
		CC_treturn(1);
	}

# line 26 fft.cc

	for (
# line 27 fft.cc
mask =1;
# line 27 fft.cc
mask <numproc ;
# line 27 fft.cc
mask <<=1){

# line 28 fft.cc
		you =me ^mask ;
		if (
# line 29 fft.cc
		me &mask )
		{
			CC_tblock CC_tblock1;

# line 30 fft.cc

# line 30 fft.cc
			CC_accept(CC_8compute_id,&CC_tblock1,0,0, 2);
			{

# line 31 fft.cc
				temp =(alpha *A [you ]+A [me ])/(alpha +1.0);

# line 32 fft.cc
				A [me ]=(alpha *A [me ]+A [you ])/(alpha +1.0);

# line 33 fft.cc
				A [you ]=temp ;
			}
CC_treturn1: 
			CC_treturn(2);
		}

# line 34 fft.cc

		else 

# line 36 fft.cc
		(CC_tcallinit(pids [you ],CC_8compute_id),
		CC_tcall(1),
		((CC_8compute *)CC_mytblock)->value) ;
	}
	CC_complete();
}

CC_main (argc ,argv )

# line 41 fft.cc
int argc ;
char **argv 
# line 42 fft.cc
;
{
	int j ;
	float atof ()
# line 45 fft.cc
		;

	if (argc >1
# line 55 fft.cc
	    )
		numproc =atoi (argv [1])
# line 56 fft.cc
			;
	else 
	    numproc =4
# line 58 fft.cc
	    ;
	if (argc >2
# line 59 fft.cc
	    )
		alpha =atof (argv [2])
# line 60 fft.cc
			;
	else 
	    alpha =1.0
# line 62 fft.cc
	    ;
	for (j =0
# line 63 fft.cc
;j <numproc 
# line 63 fft.cc
;j ++
# line 63 fft.cc
)
		    A [j ]=j 
# line 64 fft.cc
		    ;
	for (j =0
# line 65 fft.cc
;j <numproc 
# line 65 fft.cc
;j ++
# line 65 fft.cc
)
		    pids [j ]=CC_vertex (j ,0, 1 )
# line 66 fft.cc
			;
	for (j =0
# line 67 fft.cc
;j <numproc 
# line 67 fft.cc
;j ++
# line 67 fft.cc
)
		    (CC_tcallinit(pids [j ],CC_8start_id),
		CC_tcall(2),
		((CC_8start *)CC_mytblock)->value) 
# line 68 fft.cc
			;
	c_wait ()
# line 69 fft.cc
		;
	for (j =0
# line 70 fft.cc
;j <numproc 
# line 70 fft.cc
;j ++
# line 70 fft.cc
)
		    printf ("A[%d] = %f.\n",j ,A [j ])
# line 71 fft.cc
			;
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	A = (CC_shtypedef0 *) CC_shmalloc((256) * sizeof(*A));
	pids = (CC_shtypedef1 *) CC_shmalloc((256) * sizeof(*pids));
}
