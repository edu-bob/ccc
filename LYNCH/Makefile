#DFLAGS = -DDEBUG



.SUFFIXES:	.cc

.cc.o:
	ccc $(DFLAGS) $(CCFLAGS) $(CFLAGS) -c $<

tbuf:	tbuf.o
	ccc $(DFLAGS) $(CFLAGS) -o tbuf tbuf.o

matrix:	matrix.o
	ccc $(DFLAGS) $(CFLAGS) -o matrix matrix.o

fact:	fact.o
	ccc $(DFLAGS) $(CFLAGS) -o fact fact.o

seive:  seive.o
	ccc $(DFLAGS) $(CFLAGS) -o seive seive.o

clean:	
	rm -f *.o spec.out
