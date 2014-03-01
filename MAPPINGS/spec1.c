#include <ccc/CC_kernel.h>
static char *DD_convert;

# line 1 "spec1.cc"

/* PROCESS SPEC notrans */
CC_pid CC_notrans();
#define CC_notrans_numtrans 0

/* PROCESS SPEC hastrans */
CC_pid CC_hastrans();

typedef float CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
	double arg1;
} 
CC_9mytrans;
#define CC_9mytrans_id 0

#define CC_hastrans_numtrans 1

/* PROCESS BODY notrans */

CC_pid CC_notrans(x,y, CC_priority, DD_createId)
float  x;
double  y;
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("notrans",CC_notrans_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	CC_complete();
}

/* PROCESS BODY hastrans */

CC_pid CC_hastrans( CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("hastrans",CC_hastrans_numtrans,CC_priority,DD_createId,DD_convert))
		return CC_childpid;
	CC_complete();
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
