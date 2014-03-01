#include <ccc/CC_kernel.h>

# line 1 shared4.cc

CC_main ()
{
	typedef float  CC_shtypedef0;
	CC_shtypedef0 *x;
	{
		int CC_j,CC_n,CC_ss;
		char *CC_s;

		x = (CC_shtypedef0 *) CC_shmalloc((10) * sizeof(*x));
	}
	return ;

}
sub ()
{
	typedef double  CC_shtypedef1;
	CC_shtypedef1 (*y),*z[10];
	{
		int CC_j,CC_n,CC_ss;
		char *CC_s;

		y = (CC_shtypedef1 *) CC_shmalloc(sizeof(*y));
		CC_n=(10);
		CC_ss = (20) * sizeof(**z);
		CC_s = CC_shmalloc(CC_n * CC_ss);
		for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
			(z)[CC_j] = (CC_shtypedef1 *) CC_s;
	}
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
