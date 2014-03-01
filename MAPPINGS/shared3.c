#include <ccc/CC_kernel.h>

# line 1 shared3.cc

typedef float  CC_shtypedef0;
CC_shtypedef0 *m[20][30][40];

CC_main ()
{

}
CC_shinit()
{
	int CC_j,CC_n,CC_ss,CC_c[64];
	char *CC_s;

	CC_n=(20)*(30)*(40);
	CC_ss = (50) * sizeof(****m);
	CC_s = CC_shmalloc(CC_n * CC_ss);
	for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
		(**m)[CC_j] = (CC_shtypedef0 *) CC_s;
}
