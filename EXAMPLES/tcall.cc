/* tcall.c */

/*
 *  This program tests the timed transaction call.
 *  It found some bugs in the queue stuff (particularly CC_dequeue()).
 */
process spec x()
{
    trans float t();
};

float count=0.90;

process body x()
{
    for (;;) {
	delay count;
	count += 0.01;
	select {
	    accept t() {
		treturn count;
	    }
	or
	    terminate;
	}
    }
}

main()
{
    process x X;
    float c;

    X = create x();
    while ((c = within 1.0 ? X.t() : -1.0) >= 0)
	printf("I was accepted after %f seconds.\n",c);
    printf("I got impatient and timed out.\n");
}
