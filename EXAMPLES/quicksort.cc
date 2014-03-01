pragma map;

#include <stdio.h>

#define S_LENGTH 5000
shared int s[S_LENGTH];

minisort(aa, zz)
int aa, zz;
{
    int temp;

    if (s[aa] > s[zz]) {
	temp = s[aa];
	s[aa] = s[zz];
	s[zz] = temp;
    }
}

int partition(aa, az, za, zz)
int aa, *az, *za, zz;
{
    int r;

    r = s[(aa + zz)/2];
    while (aa <= zz) {
	while (s[aa] < r)
	    aa++;
	while (s[zz] > r)
	    zz--;
	if (aa <= zz) {
	    minisort(aa, zz);
	    aa++;
	    zz--;
	}
    }
    *az = zz;
    *za = aa;
}

sequential_sorter(aa, zz)
int aa,zz;
{
    int az, za;

    partition(aa, &az, &za, zz);
    if (az - aa < 2)
	minisort(aa, az);
    else
	sequential_sorter(aa, az);
    if (zz - za < 2)
	minisort(za, zz);
    else
	sequential_sorter(za, zz);
}

process spec parallel_sorter(int, int, process parallel_sorter, int)
{
    trans i_am_done();
    trans are_you_done();
};

process body parallel_sorter(aa, zz, parent, makechildren)
{
    int az, za;

    partition(aa, &az, &za, zz);
    if (makechildren) {
	if (az - aa < 2)
	    minisort(aa, az);
	else
	    create parallel_sorter(aa, az, c_mypid(), makechildren - 1);
	if (zz - za < 2)
	    minisort(za, zz);
	else
	    create parallel_sorter(za, zz, c_mypid(), makechildren - 1);
	if (az - aa >= 2)
	    accept i_am_done();
	if (zz - za >= 2)
	    accept i_am_done();
    } else {
	sequential_sorter(aa, az);
	sequential_sorter(za, zz);
    }
    if (parent != c_nullpid)
	parent.i_am_done();
    else
	accept are_you_done();
}

main(argc,argv)
int argc;
char *argv[];
{
    int j,length=0,children;
    process parallel_sorter top_sorter;

    if ( argc > 1 )
	children = atoi(argv[1]);
    else
	children = 3;
    fprintf(stderr,"Using %d processes\n",(2<<(children+1))-1);
/*
 *  Read in the data from stdin.
 */
    for ( j=0; j<S_LENGTH ; j++ )
	s[j] = rand() % 10000;
    length = S_LENGTH;

/*
 *  Start the first sorter process. The value of three
 *  allows 2**(3+1) - 1 or 15 processes to be started.
 *  Each process waits for its children, and we wait
 *  for the top process.
 */
    top_sorter = create parallel_sorter(0, length-1, c_nullpid, children);
    top_sorter.are_you_done();
/*
 *  Finally, print out the values.
 */
    for (j=0; j<length; j++)
	printf("s[%d]=%d.\n",j,s[j]);
}
