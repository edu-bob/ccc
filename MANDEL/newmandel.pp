# 1 "newmandel.cc"

# 1 "/usr/include/stdio.h"

















extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	int	_bufsiz;
	short	_flag;
	char	_file;
} _iob[20];




























struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*gets();
char	*fgets();
char	*sprintf();
# 2 "newmandel.cc"

typedef char bool;





typedef struct {
	double a,b;
} comp;





process spec ticket(int)
{
	trans int	next();
};

process spec queue()
{
	trans int put(int);
	trans int get();
};

process spec display(process queue);

process spec mandel(int, process ticket, process queue, comp, comp);





shared int raster[256][256];

int Concurrency;

char *A;
main(argc,argv)
int argc;
char *argv[];
{
	int i;
	comp min, max;

	process queue memsrvr;
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
	memsrvr = create queue();

	for ( i=0 ; i<Concurrency ; i++ )
		m[i] = create mandel(i,ticket,memsrvr,min,max);
}

process body mandel(i,ticket,srvr,min,max)
{
	int line, r;
	int res;
	comp pt;

	while ((line=ticket.next()) < 256 ) {
		for ( r=0 ; r<256 ; r++ ) {
			pt.a = min.a+(max.a-min.a)/256*r;
			pt.b = min.b+(max.b-min.b)/256*line;
			raster[line][r] = domandel(pt);
		}
		srvr.put(line);
	}
}
process body ticket()
{
	int tick;
	int i;

	tick = 0;
	while ( 1 ) {
		select {
			accept next() {
				treturn tick++;
			}
		or
			terminate;
		}
	}
}









process body queue()
{
	int queue[256], qhead, qtail;
	int next;
	int line;
	bool more;
	process display d;

	next = 0;
	qhead = 0;
	qtail = 0;

	d = create display(c_mypid());
	more = 1;
	while (more) {
		select {
			accept put(line) {
				queue[qtail++] = line;
			}
		or (qhead<qtail):
			accept get() {
				treturn queue[qhead++];
			}
		or (qhead>=256):
			accept get() {
				more = 0;
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
	char buf[256+1];

	BG_push_context();
	init_patterns();
	BG_cursor_pos(0,0);
	BG_erase_display(0);
	BG_shift_out();
	fflush((&_iob[1]));
	while ( (line=srvr.get()) >= 0 ) {
		for ( i=0 ; i<256 ; i++ )
			if (raster[line][i] >= 1000 )
				buf[i] = '9';
			else
				buf[i] = (raster[line][i] % 9)+'0';
		buf[i] = 0;
		printf("\033[%d;1H%s",line+1, buf);
		fflush((&_iob[1]));
		
	}
	BG_shift_in();
	BG_pop_context();
	fflush((&_iob[1]));
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
