/* swap.cc */

/*
 *  This is a test of nested accepts.
 */

pragma nolinesynch;

process spec agent()
{
    trans swap_A(int *);
    trans int swap_B(int);
};

process body agent()
{
    for(;;) {
	select {
	    accept swap_A(i) {
		int temp;
		accept swap_A(j) {
		    temp = *j;
		    *j = *i;
		}
		*i = temp;
	    }
	or
	    accept swap_B(i) {
		int temp;
		accept swap_B(j) {
		    temp = j;
		    treturn i;
		}
	    treturn temp;
	    }
	or
	    terminate;
	}
    }
}

process spec sema(int)
{
    trans P();
    trans V();
};

process body sema(count)
{
    for (;;) {
	select {
	    accept V();
	    count++;
	or (count>0):
	    accept P();
	    count--;
	or
	    terminate;
	}
    }
}

process sema printsema;
process agent x;

#define COUNT 20

process spec trader(int);

process body trader(oddp)
{
    int j;
    shared int s;
    int list_A[COUNT][2],list_B[COUNT][2];

    for (j=0;j<COUNT;j++) {
	list_A[j][0] = oddp + 2*j;
	s = list_A[j][0];
	x.swap_A(&s);
	list_A[j][1] = s;
    }
    for (j=0;j<COUNT;j++) {
	list_B[j][0] = oddp + 2*j;
	list_B[j][1] = x.swap_B(list_B[j][0]);
    }
    printsema.P();
    printf("\nHi, I'm %s!\n", (oddp ? "odd" : "even"));
    printf("using transaction swap_A:\n");
    for (j=0;j<COUNT;j++)
	printf("exchanged %d for %d\n",list_A[j][0],list_A[j][1]);
    printf("using transaction swap_B:\n");
    for (j=0;j<COUNT;j++)
	printf("exchanged %d for %d\n",list_B[j][0],list_B[j][1]);
    printsema.V();
}

main()
{
    process trader even,odd;

    printsema = create sema(1);
    x = create agent();
    even = create trader(0);
    odd = create trader(1);
}
