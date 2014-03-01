#include <ccc/CC_kernel.h>

# line 1 trans.cc

/* PROCESS SPEC foo */

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
} 
CC_8action;
#define CC_8action_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	arg1;
} 
CC_8fred;
#define CC_8fred_id 1

#define CC_foo_numtrans 2
CC_pid CC_foo();

/* PROCESS BODY foo */
CC_pid CC_foo(i ,x ,
CC_priority)
int  i;
float  x;
int CC_priority;
{

	int temp ;

	if (CC_fork("foo",CC_foo_numtrans,CC_priority))
		return CC_childpid;
	temp =i ;
	CC_complete();
}

CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
