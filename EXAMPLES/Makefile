.SUFFIXES:	.cc

.cc.o:
	ccc $(CCFLAGS) $(CFLAGS) -c $<

bettor:	bettor.o
	cc -o bettor bettor.o
