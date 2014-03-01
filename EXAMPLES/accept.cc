/* accept.cc */

/*
 *  This program tests out various types of accept statements.
 */

process spec a()
{
    trans int x(int);
};

process spec b(process a, int);

process body a()
{
    int zzz;

    accept x(i);
    accept x(i) {
	printf("In the second accept.\n");
	treturn;
    }
    accept x(i) {
	printf("In the third accept, returning -1.\n");
	treturn -1;
    }
    accept x(i) {
	int j;
	j=i;
	printf("In the fourth accept, we were passed %d.\n", j);
    }
    printf("We are going to wait three seconds while some requests queue up.\n");
    delay 3.0;
    printf("Done waiting.\n");
    for (zzz=5; zzz<8; zzz++)
	accept x(i) by i {
	    int j;
	    j=i;
	    printf("accept number %d gets arg %d.\n",zzz,j);
	}
    for (;;) {
	select {
	    accept x(i) {
		int j;
		j=i;
		printf("and yet another accept, arg=%d.\n",j);
	    }
	or
	    terminate;
	}
    }
}

process body b(A, arg)
{
    printf("just created, about to call A with arg=%d.\n",arg);
    A.x(arg);
}

main()
{
    process a A;
    int ret;

    A = create a();
    printf("calling first accept.\n");
    A.x(1);
    printf("calling second accept.\n");
    A.x(1);
    printf("calling third accept.\n");
    ret = A.x(1);
    printf("third accept returned %d.\n", ret);
    printf("calling fourth accept, arg=666.\n");
    A.x(666);
    printf("creating processes to call A with differing args.\n");
    create b(A, 444);
    delay 0.25;
    create b(A, 333);
    delay 0.25;
    create b(A, 222);
    delay 0.25;
    create b(A, 111);
    delay 0.25;
    printf("created four calling tasks.\n");
    delay 4.00;
    printf("by now they should all be accepted...\n");
    printf("calling A with arg=9.\n");
    A.x(9);
    printf("calling A with arg=99.\n");
    A.x(99);
    printf("calling A with arg=999.\n");
    A.x(999);
    printf("calling A with arg=9999.\n");
    A.x(9999);
    printf("all done!\n");
}
