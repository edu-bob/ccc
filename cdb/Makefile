CFLAGS = -O
WINLIB = /u2/rlb/src/winlib


OBJS = cdb.o rcdb.o display.o command.o packet.o commun.o static.o dynamic.o \
		 ../btree.o trace.o convert.o break.o points.o debug.o
SRCS = cdb.c rcdb.c display.c command.c packet.c commun.c static.c dynamic.c \
		trace.c convert.c break.c points.c debug.c
INCS = cdb.h tables.h dynamic.h ../btree.h ../objs.h format.h command.h static.h

all:&	rcdbd cdb

rcdbd:	rcdbd.c
	cc $(CFLAGS) -o rcdbd rcdbd.c

cdb:&	$(OBJS) $(INCS) $(WINLIB)/gtrlib.a
	cc $(CFLAGS) -o cdb $(OBJS) $(WINLIB)/gtrlib.a

LIST:	$(SRCS) $(INCS) rcdbd.c Makefile
	list $?
	touch LIST

clean:	
	rm -f $(OBJS)

depend:	
	cc -M $(SRCS) $(INCS)  >makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	-rm -f eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile
	
	

# DO NOT DELETE THIS LINE

cdb.o: cdb.c
cdb.o: /usr/include/sys/types.h
cdb.o: /usr/include/sys/socket.h
cdb.o: /usr/include/sys/ioctl.h
cdb.o: /usr/include/sys/ttychars.h
cdb.o: /usr/include/sys/ttydev.h
cdb.o: /usr/include/sgtty.h
cdb.o: /usr/include/netdb.h
cdb.o: /usr/include/stdio.h
cdb.o: ./cdb.h
cdb.o: ./../DD_kernel.h
cdb.o: ./../bool.h
rcdb.o: rcdb.c
rcdb.o: /usr/include/sys/types.h
rcdb.o: /usr/include/sys/socket.h
rcdb.o: /usr/include/netinet/in.h
rcdb.o: /usr/include/stdio.h
rcdb.o: /usr/include/netdb.h
rcdb.o: /usr/include/errno.h
rcdb.o: /usr/include/sys/errno.h
display.o: display.c
display.o: /usr/include/sys/types.h
display.o: /usr/include/stdio.h
display.o: ./../bool.h
display.o: ./cdb.h
display.o: ./../DD_kernel.h
display.o: ./static.h
display.o: ./dynamic.h
display.o: ./points.h
display.o: ./stateString.h
display.o: ./format.h
display.o: ./tables.h
command.o: command.c
command.o: /usr/include/signal.h
command.o: /usr/include/stdio.h
command.o: ./../btree.h
command.o: ./../bool.h
command.o: ./cdb.h
command.o: ./../DD_kernel.h
command.o: ./format.h
command.o: ./command.h
command.o: ./static.h
command.o: ./dynamic.h
command.o: ./points.h
command.o: ./tables.h
packet.o: packet.c
packet.o: ./../DD_include.h
packet.o: /usr/include/stdio.h
packet.o: /usr/include/sys/types.h
packet.o: /usr/include/netinet/in.h
packet.o: /usr/include/sys/socket.h
packet.o: ./cdb.h
packet.o: ./../DD_kernel.h
packet.o: ./static.h
packet.o: ./tables.h
packet.o: ./../bool.h
commun.o: commun.c
commun.o: ./../DD_include.h
commun.o: /usr/include/stdio.h
commun.o: /usr/include/sys/types.h
commun.o: /usr/include/netinet/in.h
commun.o: /usr/include/sys/socket.h
commun.o: /usr/include/sys/time.h
commun.o: /usr/include/signal.h
commun.o: ./cdb.h
commun.o: ./../DD_kernel.h
commun.o: ./static.h
commun.o: ./tables.h
commun.o: ./../bool.h
commun.o: ./eventString.h
static.o: static.c
static.o: /usr/include/sys/types.h
static.o: /usr/include/sys/socket.h
static.o: /usr/include/sys/ioctl.h
static.o: /usr/include/sys/ttychars.h
static.o: /usr/include/sys/ttydev.h
static.o: /usr/include/sgtty.h
static.o: /usr/include/netdb.h
static.o: /usr/include/stdio.h
static.o: ./cdb.h
static.o: ./../DD_kernel.h
static.o: ./../btree.h
static.o: ./../objs.h
static.o: ./static.h
dynamic.o: dynamic.c
dynamic.o: ./../btree.h
dynamic.o: ./../DD_include.h
dynamic.o: ./../objs.h
dynamic.o: ./../bool.h
dynamic.o: ./cdb.h
dynamic.o: ./../DD_kernel.h
dynamic.o: ./static.h
dynamic.o: ./tables.h
dynamic.o: ./dynamic.h
dynamic.o: ./points.h
trace.o: trace.c
trace.o: ./../bool.h
trace.o: ./../DD_include.h
trace.o: ./cdb.h
trace.o: ./../DD_kernel.h
trace.o: ./static.h
trace.o: ./dynamic.h
trace.o: ./points.h
trace.o: /usr/include/stdio.h
trace.o: ./stateString.h
convert.o: convert.c
convert.o: /usr/include/ctype.h
convert.o: ./cdb.h
convert.o: ./../DD_kernel.h
convert.o: ./../objs.h
convert.o: ./../bool.h
convert.o: ./static.h
convert.o: ./dynamic.h
convert.o: ./points.h
break.o: break.c
break.o: ./cdb.h
break.o: ./../DD_kernel.h
break.o: ./../bool.h
break.o: ./../DD_include.h
break.o: ./static.h
break.o: ./dynamic.h
break.o: ./points.h
points.o: points.c
points.o: ./../bool.h
points.o: ./cdb.h
points.o: ./../DD_kernel.h
points.o: ./static.h
points.o: ./dynamic.h
points.o: ./points.h
points.o: ./tables.h
debug.o: debug.c
debug.o: /usr/include/stdio.h
debug.o: ./../bool.h
debug.o: ./cdb.h
debug.o: ./../DD_kernel.h
debug.o: ./static.h
debug.o: ./dynamic.h
debug.o: ./points.h
debug.o: ./tables.h
debug.o: ./stateString.h
cdb.o: cdb.h
cdb.o: ./../DD_kernel.h
tables.o: tables.h
dynamic.o: dynamic.h
dynamic.o: ./points.h
btree.o: ../btree.h
objs.o: ../objs.h
format.o: format.h
command.o: command.h
static.o: static.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
