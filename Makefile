YACC=/usr/local/bin/bigyacc
BINDIR = /usr/local/bin
LIBDIR = /usr/local/lib

YFLAGS = -d
CFLAGS = -O


CDBOBJS = DD_monitor.o DD_semaphore.o DD_buffers.o DD_commun.o DD_packet.o\
			 DD_debug.o
CDBSRCS = DD_monitor.c DD_semaphore.c DD_buffers.c DD_commun.c DD_packet.c\
			 DD_debug.c
CDBINCS = 	       DD_semaphore.h DD_buffers.h DD_commun.h

LIBOBJS = CC_accept.o CC_death.o CC_signal.o CC_queue.o CC_tcall.o CC_fork.o
LIBSRCS = CC_accept.c CC_death.c CC_signal.c CC_queue.c CC_tcall.c CC_fork.c
LIBINCS = CC_include.h 

COMOBJS = scan.o main.o btree.o cctypes.o aux.o spec.o capture.o depend.o\
	  specPrint.o
COMSRCS = gram.y scan.l main.c btree.c cctypes.c aux.c spec.c capture.c depend.c\
	  specPrint.c
COMINCS = common.h btree.h cctypes.h gram.h spec.h

all:	ccc cccom  libccd.a libccc.a

libccd.a:&	$(CDBOBJS) $(LIBOBJS)
	ar cr libccd.a $(CDBOBJS) $(LIBOBJS)
	ranlib libccd.a

libccc.a:&	$(LIBOBJS) DD_nulMonitor.o
	ar cr libccc.a $(LIBOBJS) DD_nulMonitor.o
	ranlib libccc.a

DD_nulMonitor.o: DD_monitor.c
	rm -f DD_nulMonitor.c
	ln -s DD_monitor.c DD_nulMonitor.c
	cc $(CFLAGS) -DDD_NULL -c DD_nulMonitor.c

ccc:	ccc.sh
	sed 's#\$$(LIBDIR)#$(LIBDIR)#g' ccc.sh > ccc
	chmod +x ccc

cccom:	gram.o other.o
	cc $(CFLAGS) gram.o other.o -o cccom
other.o:&$(COMOBJS)
	ld -r $(COMOBJS) -o other.o

shar:	$(LIBSRCS) $(LIBINCS) $(COMSRCS) $(COMINCS)  $(CDBSRCS) $(CDBINCS)
	shar Makefile $(LIBSRCS) $(LIBINCS) $(COMSRCS) $(COMINCS) $(CDBSRCS) $(CDBINCS) > ccc.shar

LIST:	$(CDBSRCS) $(LIBSRCS) $(COMSRCS) $(CDBINCS) $(LIBINCS) $(COMINCS) \
		ccc.sh Makefile CC_kernel.h DD_kernel.h
	list $?
	touch LIST

install:ccc.install cccom.install lib.install DD_kernel.h
	-mkdir $(DESTDIR)/usr/include/ccc
	install -c CC_kernel.h DD_kernel.h $(DESTDIR)/usr/include/ccc

ccc.install:ccc
	install -c ccc $(DESTDIR)/$(BINDIR)
cccom.install:cccom
	install -s cccom $(DESTDIR)/$(LIBDIR)
lib.install:libccc.a libccd.a
	install libccc.a $(DESTDIR)/$(LIBDIR)
	ranlib $(DESTDIR)/$(LIBDIR)/libccc.a
	install libccd.a $(DESTDIR)/$(LIBDIR)
	ranlib $(DESTDIR)/$(LIBDIR)/libccd.a
	
lib.flar:$(LIBSRCS) $(CDBSRCS)
	flar $(LIBSRCS) $(CDBSRCS) > lib.flar

clean:	
	rm -f a.out y.output $(COMOBJS) y.tab.h $(LIBOBJS) $(CDBOBJS)\
		 DD_nulMonitor.o DD_monitor.o  ccom ccc libccc.a

depend:
	grep '^#include' ${COMSRCS} ${LIBSRCS} ${CDBSRCS}\
		| sed -e '/<.*>/d' -e 's/:[^"]*"\([^"]*\)".*/: \1/' \
		| sed 's/\.c/.o/' >makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	-rm -f eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

y.tab.h:	gram.y
# DO NOT DELETE THIS LINE
gram.y: btree.h
gram.y: common.h
gram.y: btree.h
gram.y: cctypes.h
gram.y: gram.h
gram.y: bool.h
scan.l: common.h
scan.l: y.tab.h
main.o: common.h
main.o: bool.h
cctypes.o: common.h
cctypes.o: cctypes.h
cctypes.o: y.tab.h
aux.o: common.h
aux.o: cctypes.h
spec.o: cctypes.h
spec.o: spec.h
spec.o: bool.h
depend.o: btree.h
depend.o: cctypes.h
depend.o: bool.h
specPrint.o: cctypes.h
specPrint.o: spec.h
specPrint.o: DD_kernel.h
CC_accept.o: CC_include.h
CC_death.o: CC_include.h
CC_signal.o: CC_include.h
CC_queue.o: CC_include.h
CC_tcall.o: CC_include.h
CC_fork.o: CC_include.h
CC_fork.o: DD_kernel.h
DD_monitor.o: CC_include.h
DD_monitor.o: DD_kernel.h
DD_monitor.o: DD_buffers.h
DD_semaphore.o: DD_semaphore.h
DD_semaphore.o: bool.h
DD_buffers.o: DD_include.h
DD_buffers.o: bool.h
DD_buffers.o: DD_buffers.h
DD_buffers.o: DD_semaphore.h
DD_buffers.o: cdb/eventString.h
DD_commun.o: bool.h
DD_commun.o: DD_include.h
DD_commun.o: DD_kernel.h
DD_commun.o: DD_buffers.h
DD_packet.o: DD_include.h
DD_packet.o: DD_kernel.h
DD_debug.o: bool.h
DD_debug.o: cdb/eventString.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
