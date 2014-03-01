#include <stdio.h>

#define S_LENGTH 20000
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

process spec parallel_sorter(int, int, int);

process body parallel_sorter(aa, zz, makechildren)
{
    int az, za;

    partition(aa, &az, &za, zz);
    if (makechildren) {
	if (az - aa < 2)
	    minisort(aa, az);
	else
	    create parallel_sorter(aa, az, makechildren - 1);
	if (zz - za < 2)
	    minisort(za, zz);
	else
	    create parallel_sorter(za, zz, makechildren - 1);
    } else {
	fprintf(stderr,"sequential_sorter(%d);\n",az-aa);
	fprintf(stderr,"sequential_sorter(%d);\n",zz-za);
	sequential_sorter(aa, az);
	sequential_sorter(za, zz);
	fprintf(stderr,"done\n");
    }
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
    fprintf(stderr,"Using %d processes\n",(1<<(children+1))-1);
/*
 *  Read in the data from stdin.... Well, actually, we just create random data.
 */
    for ( j=0; j<S_LENGTH ; j++ )
	s[j] = rand() % 10000;
    length = S_LENGTH;
    if (children < 0) {
	for (j=0; j<length; j++)
	    printf("s[%d]=%d.\n",j,s[j]);
	exit();
    }

/*
 *  Start the first sorter process. The value of three
 *  allows 2**(3+1) - 1 or 15 processes to be started.
 *  Each process waits for its children, and we wait
 *  for the top process.
 */
    top_sorter = create parallel_sorter(0, length-1, children);
    c_wait();
/*
 *  Finally, print out the values.
 */
#ifdef notdef
    for (j=0; j<length; j++)
	printf("s[%d]=%d.\n",j,s[j]);
#endif
}
