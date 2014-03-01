pragma nolinesynch;

/*---------------------------------------------------------------------------
 * This factorial uses a given number of processes.  The parent
 * process divides the multiplication evenly among the  other processes,
 * then waits (with accepts) for all
 * the processes to respond with their answers, which are multiplied
 * into the final result as they are received.
 * 
 * Fri Jun 20 09:54:28 1986 Added strides for equalizing magnitude of
   answers, and doubles for helping with size.

 * William L. Lynch		Wed Jun 18 15:03:49 1986
 *---------------------------------------------------------------------------
 */

#define DEBUG 1

#include <stdio.h>

#define PROCESSES	4


process spec
parent(long int)
{
	trans void result(double);
};



process spec
product(long int, long int, process parent);	/* start, stride */




/* parent will create process products, and receive their results by trans */
process body 
parent(n)
{
	int started;	/* count of number of children */
	double factorial;
	long int start;
	int stride;
	
	start=n;	/* start of stride */
	stride = PROCESSES;
	
	for (started = 0; started < PROCESSES; started++) {
		create product(start--, stride, c_mypid());
	}

	/* Now, wait for the answers to come rolling in */
	factorial = 1;
	for (; started > 0; started--) {
		accept result(p) {

#ifdef DEBUG
			fprintf(stderr,"Returning child: %.0f\n",p);
#endif			

			factorial *= p;
			treturn;
		}
	}
	printf("%1d factorial is %.0f.\n", n, factorial);
}

	


/* product takes the product of all integers between upper and lower,
   inclusive. */
process body 
product(start, stride, mom)
{
	long int i;
	double answer;
	
#ifdef DEBUG
	fprintf(stderr,"starting child, %ld, %ld\n", start, stride);
#endif	

	answer = 1.0;
	for (i=start; i > 0; i -= stride) {
		answer *= i;
	}
	mom.result(answer);
}


main (argc,argv)
int argc;
char *argv[];
{
	long int n;
	
	if (argc>1) {
		sscanf(argv[1], "%d", &n);
		create parent(n);
	} else {
		fprintf(stderr,"Usage: fact n\n");
	}
}

