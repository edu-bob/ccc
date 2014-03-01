#include <ccc/CC_kernel.h>

# line 1 accept.cc

/* PROCESS SPEC foo */
CC_pid CC_foo();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 value;
} 
CC_8action;
#define CC_8action_id 0

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 value;
	int arg1;
} 
CC_8fred;
#define CC_8fred_id 1

#define CC_foo_numtrans 2

/* PROCESS BODY foo */
CC_pid CC_foo(i ,x ,
CC_priority)
int  i;
float  x;
int CC_priority;
{

	int temp ,temp2 ;

	if (CC_fork("foo",CC_foo_numtrans,CC_priority))
		return CC_childpid;
	{
		CC_tblock CC_tblock0;
		CC_accept(CC_8fred_id,&CC_tblock0,0,0);
		{
			temp2 =((CC_8fred *)CC_tblock0)->arg1 ;
		}
		/*
		 *  exiting block in which the following were declared:
		 *  |name                context chain   num     obj     alias               |
		 *  |temp                0       0       6       0       ((CC_8fred *)CC_tblock0)->arg1|
		 */
CC_treturn0: 
		CC_treturn();
	}

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |c_mypid             0       25      8       3                           |
	 *  |temp                0       0       6       0                           |
	 *  |temp2               0       0       6       0                           |
	 */
	CC_complete();
}

/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |c_abort             0       25      6       0                           |
 *  |c_active            0       25      6       0                           |
 *  |c_associate         0       25      6       0                           |
 *  |c_changepriority    0       25      6       0                           |
 *  |c_completed         0       25      6       0                           |
 *  |c_getpriority       0       25      6       0                           |
 *  |c_invalid           0       25      6       0                           |
 *  |c_nullpid           0       0       5       3                           |
 *  |i                   0       0       6       0                           |
 *  |x                   0       0       6       0                           |
 *  |anytype             1       0       4       3                           |
 *  |foo                 1       0       8       3                           |
 *  |1                   8       0       6       0                           |
 *  |2                   8       0       6       0                           |
 *  |action              8       0       9       0                           |
 *  |fred                8       0       10      0                           |
 *  |0                   9       0       0       0                           |
 *  |=                   9       0       6       0                           |
 *  |0                   10      0       1       0                           |
 *  |1                   10      0       6       0                           |
 *  |=                   10      0       6       0                           |
 */
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
