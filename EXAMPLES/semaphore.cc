/* semaphore.cc */

/*
 *  This program demonstrates how to implement semaphores in Concurrent C.
 *  It prints cute pattern (arrows) when the semaphores are enabled.
 *  When the semaphores are disabled (by supplying an argument on the
 *  command line), the 'a' arrows and the 'b' arrows get mixed up.
 */

process spec sema(int)
{
    trans V();
    trans P();
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

process spec printer(process sema,char);

process body printer(s,c)
{
    int j;

    for (j=0;;j++) {
	s.P();
	printf("   %c%c%c\n",c,c,c);
	delay 0.1;
	printf("   %c%c%c\n",c,c,c);
	delay 0.1;
	printf("   %c%c%c\n",c,c,c);
	delay 0.1;
	printf("%c%c%c%c%c%c%c%c%c\n",c,c,c,c,c,c,c,c,c);
	delay 0.1;
	printf(" %c%c%c%c%c%c%c\n",c,c,c,c,c,c,c);
	delay 0.1;
	printf("  %c%c%c%c%c\n",c,c,c,c,c);
	delay 0.1;
	printf("   %c%c%c\n",c,c,c);
	delay 0.1;
	printf("    %c\n",c);
	delay 0.1;
	printf("\n");
	if (c=='a')
	    delay 0.15;
	s.V();
    }
}

main(argc)
int argc;
{
    process printer p1,p2;
    process sema s;

    if (argc > 1)
	s = create sema(2);	/* no mutual exclusion if arg on command line */
    else
	s = create sema(1);	/* otherwise, semaphores work as they should */
    p1 = create printer(s, 'a');
    p2 = create printer(s, 'b');
}
