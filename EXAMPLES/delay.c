#include <ccc/CC_kernel.h>

# line 1 delay.cc

CC_main ()
{
	float count 
# line 12 delay.cc
	    ;

	for (count =0.90
# line 14 delay.cc
;count <1.10
# line 14 delay.cc
;count +=0.01
# line 14 delay.cc
){
		    CC_delay((double)(count 
# line 15 delay.cc
		    ), 1);

		printf ("I was delayed for %f seconds.\n",count )
# line 16 delay.cc
			;
	}
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}
