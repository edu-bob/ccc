#include <ccc/CC_kernel.h>

# line 1 fake.cc

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

# line 9 fake.cc

hardwork (howmuch )
int howmuch 
# line 12 fake.cc
;
{
	int j ;

	for (
# line 16 fake.cc
j =0;
# line 16 fake.cc
j <howmuch ;
# line 16 fake.cc
j ++)

# line 17 fake.cc
		rand ();
}

/* PROCESS SPEC sharework */
CC_pid CC_sharework();
#define CC_sharework_numtrans 0

# line 20 fake.cc

/* PROCESS BODY sharework */
CC_pid CC_sharework(howmuch ,makechildren ,
CC_priority, DD_createId)

# line 23 fake.cc
int  howmuch;
int  makechildren;
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("sharework",CC_sharework_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 24 fake.cc
	if (
# line 24 fake.cc
	makechildren ){

# line 25 fake.cc
		CC_sharework (howmuch /2,makechildren -1,0, 1 );

# line 26 fake.cc
		CC_sharework (howmuch /2,makechildren -1,0, 2 );
	}
	else 

# line 28 fake.cc
	hardwork (howmuch );
	CC_complete();
}

CC_main (argc ,argv )

# line 32 fake.cc
int argc ;
char *argv []
# line 33 fake.cc
;
{
	int howmuch ,children 
# line 35 fake.cc
	    ;

	if (argc !=3
# line 37 fake.cc
	    ){
		printf ("usage : fake processdepth howmuchwork\n")
# line 38 fake.cc
			;
		return ;

	}

# line 41 fake.cc
	howmuch =atoi (argv [2]);

# line 42 fake.cc
	children =atoi (argv [1]);

# line 43 fake.cc
	CC_sharework (howmuch ,children ,0, 3 );
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
