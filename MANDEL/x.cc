pragma nolinesynch;

#include <stdio.h>

#define SIZE 256

typedef struct {
	double a,b;
} comp;

/*
**------------------------------------------------------------------------
** Process specifications
*/
process spec raster()
{
	trans int new();
	trans int put(int);
	trans int get();
};

process spec display(process raster);

process spec mandel(int, process raster, comp, comp);
/*
** End of process specifications
**------------------------------------------------------------------------
*/

shared int raster[SIZE][SIZE];

int Concurrency;

char *A;
main(argc,argv)
int argc;
char *argv[];
{
	int i;
	comp min, max;

	process raster memsrvr;
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

	memsrvr = create raster();

	for ( i=0 ; i<Concurrency ; i++ )
		m[i] = create mandel(i,memsrvr,min,max);
	fprintf(stderr,"main done\n");
}

process body mandel(i,srvr,min,max)
{
	int line, r;
	int res;
	comp pt;

	while ((line=srvr.new()) >= 0 ) {
		for ( r=0 ; r<SIZE ; r++ ) {
			pt.a = min.a+(max.a-min.a)/SIZE*r;
			pt.b = min.b+(max.b-min.b)/SIZE*line;
			raster[line][r] = domandel(pt);
		}
		srvr.put(line);
	}
	fprintf(stderr,"mandel(%d) done\n",i);
}

/*
** process raster is the output server.  It accepts completed lines from the
**	computation processes and passes them off to the display server.
**
** completed lines are stored in a queue implemented as an array.  Wrap
** around is not supported since the array is long enough to hold all
** entries even if the display server stops.
*/

process body raster()
{
	int queue[SIZE], qhead, qtail;
	int next;
	int line;
	process display d;

	next = 0;
	qhead = 0;
	qtail = 0;

	d = create display(c_mypid());
	while (1) {
		select {
			accept new() {
				if ( next < SIZE )
					treturn next++;
				else
					treturn -1;
			}
		or
			accept put(line) {
				queue[qtail++] = line;
			}
		or (qhead<qtail):
			accept get() {
				treturn queue[qhead++];
			}
		or (qhead>=SIZE):
			accept get() {
fprintf(stderr,"accepted final get\n");
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

	strcpy(A,"disply");
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
		printf("\033[%d;1H%s",line, buf);
		fflush(stdout);
		
	}
fprintf(stderr,"got -1\n");
	BG_shift_in();
	fflush(stdout);
}
domandel(pt)
comp pt;
{
	int count;
	double zr, zi, newr;
return iabs((int)(pt.a*pt.b*1000.0))%1001;
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
iabs(i)
{return i<0?-i:i;}
