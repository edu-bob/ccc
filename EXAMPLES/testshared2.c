#include <ccc/CC_kernel.h>

# line 1 testshared2.cc

typedef int  CC_shtypedef0;
CC_shtypedef0 (*a)
# line 7 testshared2.cc
,(*b);

CC_main ()

# line 10 testshared2.cc
{
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	a = (CC_shtypedef0 *) CC_shmalloc(sizeof(*a));
	b = (CC_shtypedef0 *) CC_shmalloc(sizeof(*b));
}
