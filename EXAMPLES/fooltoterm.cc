/* fooltoterm.cc */

/*
 *  This program was used to find a bug in the termination conditions:
 *  when the last B died, it forced A to select termination;
 *  this happened because the check for parent completion was missing.
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
	treturn;
    }
    accept x(i) {
	treturn -1;
    }
    accept x(i) {
	int j;
	j=i;
    }
    delay 3.0;
    for (zzz=5; zzz<8; zzz++)
	accept x(i) by i {
	    int j;
	    j=i;
	}
    for (;;) {
	printf("in the select loop.\n");
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
    A.x(arg);
}

main()
{
    process a A;
    int ret;

    A = create a();
    A.x(1);
    A.x(1);
    ret = A.x(1);
    A.x(666);
    create b(A, 444);
    delay 0.25;
    create b(A, 333);
    delay 0.25;
    create b(A, 222);
    delay 0.25;
    create b(A, 111);
    delay 0.25;
    delay 4.00;
    printf("by now they should all be accepted...\n");
    A.x(9);
    A.x(99);
    A.x(999);
    A.x(9999);
    printf("all done!\n");
}
