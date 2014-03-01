#include <ccc/CC_kernel.h>

# line 1 broken_cc.cc

typedef float floatarray [][];

/* PROCESS SPEC noparam */
CC_pid CC_noparam();
#define CC_noparam_numtrans 0

# line 13 broken_cc.cc

/* PROCESS BODY noparam */
CC_pid CC_noparam(a ,
CC_priority, DD_createId)

# line 16 broken_cc.cc
a;
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("noparam",CC_noparam_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 17 broken_cc.cc
	CC_complete();
}

CC_main ()

# line 20 broken_cc.cc
{
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
