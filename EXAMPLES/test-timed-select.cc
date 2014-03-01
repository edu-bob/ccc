/* test-timed-select.cc */

/*
 *  You'll never guess.
 */

float frandom()
{
    long random();
    static float twotothe31 = 256.0 * 256.0 * 256.0 * 128.0;

    return random()/twotothe31;
}

process spec s(float)
{
    trans t(int);
};

process body s(time)
{
    for (;;)
	select {
	    accept t(j) {
		printf("selected accept.\n",j);
	    }
	or
	    delay time;
	    printf("selected delay.\n");
	or
	    terminate;
	}
}

main()
{
    process s p;
    float time;
    int j;

    p = create s(2.0);
    for (j=0;j<10;j++) {
	time= 1.5 + 1.0 * frandom();
	delay time;
	p.t(j);
	printf("That tcall was made after a delay of %f seconds.\n",time);
    }
}
