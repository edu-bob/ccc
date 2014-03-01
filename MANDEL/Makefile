CCC=../ccc

.SUFFIXES:.cc
.cc.o:
	$(CCC) $(CCFLAGS) $(CFLAGS) -c $<
#------------------------------------------------------------------------
CFLAGS=-O -DENDINGTIMES
CCFLAGS=-G

newmandel:newmandel.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o newmandel newmandel.o patterns.o primitives.o
easyfirst:easyfirst.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o easyfirst easyfirst.o patterns.o primitives.o
easyfirst.o:protomandel.cc
	$(CCC) $(CCFLAGS) $(CFLAGS) -DEASYFIRST -c protomandel.cc
	mv protomandel.o easyfirst.o

hardfirst:hardfirst.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o hardfirst hardfirst.o patterns.o primitives.o
hardfirst.o:protomandel.cc
	$(CCC) $(CCFLAGS) $(CFLAGS) -DHARDFIRST -c protomandel.cc
	mv protomandel.o hardfirst.o

inorder:inorder.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o inorder inorder.o patterns.o primitives.o
inorder.o:protomandel.cc
	$(CCC) $(CCFLAGS) $(CFLAGS) -c protomandel.cc
	mv protomandel.o inorder.o

random:random.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o random random.o patterns.o primitives.o
random.o:protomandel.cc
	$(CCC) $(CCFLAGS) $(CFLAGS) -DRANDOM -c protomandel.cc
	mv protomandel.o random.o

mandel:&mandel.o patterns.o primitives.o 
	$(CCC) $(CCFLAGS) $(CFLAGS) -o mandel mandel.o patterns.o primitives.o
netmandel:netmandel.o
	$(CCC) $(CCFLAGS) $(CFLAGS) -o netmandel netmandel.o
