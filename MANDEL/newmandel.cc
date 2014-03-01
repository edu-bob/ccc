pragma nolinesynch;
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

typedef char bool;
#define TRUE 1
#define FALSE 0

#define SIZE 256

/*
** TYPES...
*/

typedef struct {	/* complex numbers */
	double a,b;
} comp;

/*
**------------------------------------------------------------------------
** Process specifications
**
** TICKET - maintains a protected integer.  Transactions are
**		set - set the ticket counter to a given value.
**		value - returns the current value.
**		next - returns current value and increments.
**
** QUEUE - maintains a protected queue of integers.  Transactions are
**		put - puts the given integer on the queue.
**		get - returns the next integer from the queue.
**
** DISPLAY - runs the graphics display.  UNIX does not support atomic writes
**	     to terminal devices, so this process is the only one that writes
**	     to the display.  No transactions.  It retrieves lines numbers
**	     from the queue.
**
**  MANDEL - performs the computation.
*/
process spec ticket()
{
	trans void	set(int);
	trans int	value();
	trans int	next();
};

process spec queue(int)
{
	trans int put(int);
	trans int get();
};

process spec display(process queue);

process spec mandel(int, process ticket, process queue, comp, comp);
/*
** End of process specifications
**------------------------------------------------------------------------
*/

shared int raster[SIZE][SIZE];
shared struct timeval times[256][2];
FILE *Dump;
int Concurrency;

char *A;
main(argc,argv)
int argc;
char *argv[];
{
	int i;
	comp min, max;

	process queue queue;
	process ticket ticket;
	process mandel m[128];

	A = argv[0];
	if ( argc > 1 )
		Concurrency = atoi(argv[1]);
	else
		Concurrency = 5;
	min.a = 0-2.0;
	min.b = 0-1.25;
	max.a = 0.5;
	max.b = 1.25;

	ticket = create ticket();
	ticket.set(0);
	queue = create queue(256);
	(void) create display(queue);

	for ( i=0 ; i<Concurrency ; i++ )
		m[i] = create mandel(i,ticket,queue,min,max);
}

process body mandel(i,ticket,srvr,min,max)
{
	int line, r;
	int res;
	comp pt;
	struct rusage ru;

	while ((line=ticket.next()) < SIZE ) {
		getrusage(0,&ru);
		times[line][0] = ru.ru_utime;
		for ( r=0 ; r<SIZE ; r++ ) {
			pt.a = min.a+(max.a-min.a)/SIZE*r;
			pt.b = min.b+(max.b-min.b)/SIZE*line;
			raster[line][r] = domandel(pt);
		}
		getrusage(0,&ru);
		times[line][1] = ru.ru_utime;
		srvr.put(line);
	}
}
process body ticket()
{
	int tick;
	int i, value;

	while ( TRUE ) {
		select {
			accept set(val) {
				tick = val;
			}
		or
			accept value() {
				treturn tick;
			}
		or
			accept next() {
				treturn tick++;
			}
		or
			terminate;
		}
	}
}
/*
** process queue accepts completed lines from the
**	computation processes and passes them off to the display server.
**
** This is a minimal function queue server.
*/

process body queue(size)
{
	int *queue, qhead, qtail;
	int line;
	bool more;
	process display d;

	qhead = 0;
	qtail = 0;
	queue = calloc(size, sizeof(int));
	more = TRUE;
	while (more) {
		select {
			accept put(line) {
				queue[qtail++] = line;
			}
		or (qhead<qtail):
			accept get() {
				treturn queue[qhead++];
			}
		or (qhead>=size):
			accept get() {
				more = FALSE;
				treturn -1;
			}
		or
			terminate;
		}
	}
}
process body display(srvr)
{
	int i, line;
	char buf[SIZE+1];

	BG_push_context();
	init_patterns();
	BG_cursor_pos(0,0);
	BG_erase_display(0);
	BG_shift_out();
	fflush(stdout);
	while ( (line=srvr.get()) >= 0 ) {
		for ( i=0 ; i<SIZE ; i++ )
			if (raster[line][i] >= 1000 )
				buf[i] = '9';
			else
				buf[i] = (raster[line][i] % 9)+'0';
		buf[i] = 0;
		printf("\033[%d;1H%s",line+1, buf);
		fflush(stdout);
		
	}
	BG_shift_in();
	BG_pop_context();
	fflush(stdout);
	for ( i=0 ; i<256 ; i++ )
		fprintf(stderr, "%d %d\n",i,(times[i][1].tv_sec*1000+times[i][1].tv_usec/1000)-(times[i][0].tv_sec*1000+times[i][0].tv_usec/1000));
}
domandel(pt)
comp pt;
{
	int count;
	double zr, zi, newr;
	zr = zi = 0.0;
	for ( count=0 ; count<1000 ; count++ ) {
		if ( zi*zi+zr*zr > 4.0)
			break;
		newr = zr*zr-zi*zi+pt.a;
		zi = 2.0*zr*zi+pt.b;
		zr = newr;
	}
	return count;
}
