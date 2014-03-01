/* fft.cc */

/*
 *  This program gives the process skeleton for an fft routine.
 *  It has the right accepts, tcalls, etc. but not the math.
 */

process spec vertex(int)
{
    trans start();
    trans compute();
};

int numproc;
float alpha;
#define MAXNUMPROC 256
shared float A[MAXNUMPROC];
shared process vertex pids[MAXNUMPROC];

process body vertex(me)
{
    int mask, you;
    float temp;

    accept start();
    for (mask=1; mask<numproc; mask <<= 1) {
	you = me ^ mask;
	if (me & mask)
	    accept compute() {
		temp  = (alpha*A[you] + A[me]) / (alpha +  1.0);
		A[me] = (alpha*A[me] + A[you]) / (alpha +  1.0);
		A[you] = temp;
	    }
	else
	    pids[you].compute();
    }
}

main(argc, argv)
int argc;
char **argv;
{
    int j;
    float atof();

/*
 *  Numproc must be a power of two.
 *
 *  Alpha=0.0 reverses values;
 *  alpha=1.0 averages values;
 *  alpha->infinity keeps values in original order.
 *  Intermediate values of alpha do intermediate sorts of things.
 */
    if (argc > 1)
	numproc = atoi(argv[1]);
    else
	numproc = 4;
    if (argc > 2)
	alpha = atof(argv[2]);
    else
	alpha = 1.0;
    for (j=0; j<numproc; j++)
	A[j] = j;
    for (j=0; j<numproc; j++)
	pids[j] = create vertex(j);
    for (j=0; j<numproc; j++)
	pids[j].start();
    c_wait();
    for (j=0; j<numproc; j++)
	printf("A[%d] = %f.\n",j,A[j]);
}
