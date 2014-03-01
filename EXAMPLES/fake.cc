/* fake.cc */

/*
 *  This program fakes a perfectly parallelizable algorithm.
 *  It requires two parameters on the command line.
 */

#include <stdio.h>

hardwork(howmuch)
int howmuch;
{
    int j;

    for (j=0; j<howmuch; j++)
	rand();
}

process spec sharework(int, int);

process body sharework(howmuch, makechildren)
{
    if (makechildren) {
	create sharework(howmuch/2, makechildren - 1);
	create sharework(howmuch/2, makechildren - 1);
    } else
	hardwork(howmuch);
}

main(argc,argv)
int argc;
char *argv[];
{
    int howmuch,children;

    if ( argc != 3 ) {
	printf("usage : fake processdepth howmuchwork\n");
	return;
    }
    howmuch = atoi(argv[2]);
    children = atoi(argv[1]);
    create sharework(howmuch, children);
}
