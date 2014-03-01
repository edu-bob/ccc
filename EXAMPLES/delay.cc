/* delay.cc */

/*
 *  This program was used to find a bug in the timing routines.
 *  The routine that split floats into two longs rounded the
 *  usec part, causing 1.000000 -> 0.99999999999999 to be turned into
 *  0 sec, 1000000 = 0 usec.
 */
main()
{
    float count;

    for (count=0.90; count<1.10; count+=0.01) {
	delay count;
	printf("I was delayed for %f seconds.\n",count);
    }
}
