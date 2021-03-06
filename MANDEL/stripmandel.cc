pragma nolinesynch;
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <stdio.h>

#ifdef RANDOM
#define EASYFIRST
#endif

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

#ifdef LINEDISTRIBUTION
shared struct timeval times[256][2];
#endif
#ifdef ENDINGTIMES
struct timeval T0;
#endif

FILE *Dump;
int Concurrency;

char *A;
char err[256];
int ErrOut;

main(argc,argv)
int argc;
char *argv[];
{
	int i;
	comp min, max;
#ifdef ENDINGTIMES
	struct timezone tz;
#endif
	process queue queue;
	process ticket ticket;
	process mandel m[128];

	A = argv[0];
	if ( argc > 1 )
		Concurrency = atoi(argv[1]);
	else
		Concurrency = 8;
	min.a = 0-2.0;
	min.b = 0-1.25;
	max.a = 0.5;
	max.b = 1.25;
	sprintf(err,"%s.err", A);
	ErrOut = open(err,O_WRONLY|O_APPEND|O_CREAT|O_TRUNC,0664);

#ifdef RANDOM
	permute();
#endif
#ifdef ENDINGTIMES
	gettimeofday(&T0,&tz);
#endif
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
#ifdef ENDINGTIMES
	struct timeval t0, t1;
	struct timezone tz;
#endif
#ifdef LINEDISTRIBUTION
	struct rusage ru;
#endif

#ifdef ENDINGTIMES
	gettimeofday(&t0,&tz);
#endif
	while ((line=ticket.next()) < SIZE ) {
#ifdef LINEDISTRIBUTION
		getrusage(0,&ru);
		times[line][0] = ru.ru_utime;
#endif
		for ( r=0 ; r<SIZE ; r++ ) {
			pt.a = min.a+(max.a-min.a)/SIZE*r;
			pt.b = min.b+(max.b-min.b)/SIZE*line;
			raster[line][r] = domandel(pt);
		}
#ifdef LINEDISTRIBUTION
		getrusage(0,&ru);
		times[line][1] = ru.ru_utime;
#endif
		srvr.put(line);
	}
#ifdef ENDINGTIMES
	gettimeofday(&t1,&tz);
	eprintf("%d %d %d\n",i,
	(t0.tv_sec*1000+t0.tv_usec/1000)-(T0.tv_sec*1000+T0.tv_usec/1000),
	(t1.tv_sec*1000+t1.tv_usec/1000)-(t0.tv_sec*1000+t0.tv_usec/1000));
#endif
}

/* This array lists the lines in the computation from the easiest to the hardest
*/

int Order[SIZE] = {
0, 1, 2, 251, 252, 253, 254, 255, 3, 4, 5, 6, 10, 11, 245, 246, 247, 248,
249, 250, 7, 8, 9, 12, 243, 244, 13, 14, 15, 16, 17, 18, 237, 239, 240,
241, 242, 19, 20, 236, 238, 21, 235, 24, 225, 226, 228, 23, 230, 231,
232, 233, 25, 28, 29, 30, 222, 223, 224, 227, 229, 26, 27, 31, 32, 33,
34, 221, 35, 220, 36, 219, 37, 22, 234, 214, 42, 218, 38, 215, 41, 217,
39, 216, 40, 213, 43, 61, 195, 212, 44, 60, 196, 197, 59, 58, 198, 200,
210, 56, 57, 199, 46, 211, 45, 47, 203, 207, 209, 49, 53, 202, 54, 55,
201, 208, 48, 50, 206, 51, 52, 204, 205, 62, 194, 63, 193, 64, 192, 65,
191, 66, 190, 67, 189, 188, 68, 69, 187, 70, 186, 72, 184, 75, 71, 181,
185, 78, 178, 76, 180, 79, 177, 74, 77, 73, 182, 179, 183, 80, 176, 81,
175, 82, 174, 83, 173, 87, 169, 84, 172, 88, 85, 168, 171, 86, 170, 89,
167, 90, 166, 91, 165, 92, 164, 98, 97, 96, 158, 160, 159, 95, 161, 94,
162, 99, 93, 157, 163, 100, 156, 101, 155, 102, 154, 103, 153, 104, 152,
150, 106, 151, 105, 107, 149, 108, 148, 109, 110, 147, 146, 111, 145,
112, 144, 116, 113, 115, 143, 140, 114, 141, 142, 117, 120, 139, 136,
119, 118, 137, 138, 135, 121, 122, 134, 123, 133, 124, 132, 126, 130,
125, 131, 127, 129, 128
};

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
#ifdef EASYFIRST
				treturn tick >= SIZE ? SIZE : Order[tick];
#else
#ifdef HARDFIRST
				treturn tick >= SIZE ? SIZE : Order[SIZE-1-tick];
#else
				treturn tick;
#endif
#endif
			}
		or
			accept next() {
#ifdef EASYFIRST
				treturn tick >= SIZE ? SIZE : Order[tick++];
#else
#ifdef HARDFIRST
				treturn tick >= SIZE ? SIZE : Order[SIZE-1-tick++];
#else
				treturn tick++;
#endif
#endif
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
#ifdef LINEDISTRIBUTION
	for ( i=0 ; i<256 ; i++ )
		eprintf("%d %d\n",i,(times[i][1].tv_sec*1000+times[i][1].tv_usec/1000)-(times[i][0].tv_sec*1000+times[i][0].tv_usec/1000));
#endif
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
eprintf(f,p0,p1,p2,p3,p4,p5,p6)
char *f;
int p0,p1,p2,p3,p4,p5,p6;
{
	char buf[256];
	sprintf(buf,f,p0,p1,p2,p3,p4,p5,p6);
	write(ErrOut,buf,strlen(buf));
}
#ifdef RANDOM
permute()
{
	int i, j, tmp;
	srand(getpid());
	for ( i=0; i<SIZE-1 ; i++ ) {
		j = (rand() % 13541) % SIZE;
		tmp = Order[i];
		Order[i] = Order[j];
		Order[j] = tmp;
	}
}
#endif
