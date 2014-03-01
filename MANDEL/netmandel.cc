pragma nolinesynch;

#include <stdio.h>

typedef char bool;
#define TRUE 1
#define FALSE 0


typedef struct {
	float a,b;
} complex;

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

process spec mandel(int, process raster, complex, complex);
/*
** End of process specifications
**------------------------------------------------------------------------
*/

#define MAXSIZE 512
shared short Raster[MAXSIZE][MAXSIZE];

int Concurrency = 8;
complex Min = { -2.0, -1.25 };
complex  Max = { 0.5, 1.25 };
int Size = 256;
int Threshold = 1000;

char *A;
main(argc,argv)
int argc;
char *argv[];
{
	int i;

	process raster memsrvr;
	process mandel m[128];

	A = argv[0];
	procargs(argc,argv);
	printf("%d %d %g %g %g %g\n",Size,Threshold,Min.a,Min.b,Max.a,Max.b);
	fflush(stdout);

	memsrvr = create raster();

	for ( i=0 ; i<Concurrency ; i++ )
		m[i] = create mandel(i,memsrvr,Min,Max);
}

process body mandel(i,srvr,min,max)
{
	int line, r;
	int res;
	complex pt;

	while ((line=srvr.new()) >= 0 ) {
		for ( r=0 ; r<Size ; r++ ) {
			pt.a = min.a+(max.a-min.a)/Size*r;
			pt.b = min.b+(max.b-min.b)/Size*line;
			Raster[line][r] = domandel(pt);
		}
		srvr.put(line);
	}
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
	int queue[MAXSIZE], qhead, qtail;
	int next;
	int line;
	bool more;
	process display d;

	next = 0;
	qhead = 0;
	qtail = 0;

	d = create display(c_mypid());
	more = TRUE;
	while (more) {
		select {
			accept new() {
				if ( next < Size )
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
		or (qhead>=Size):
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
	char buf[MAXSIZE+1];

	while ( (line=srvr.get()) >= 0 ) {
		printf("%d %d\n",line,Size);
		fwrite(&Raster[line][0],sizeof Raster[line][0],Size,stdout);
		fflush(stdout);
		
	}
}
domandel(pt)
complex pt;
{
	int count;
	double zr, zi, newr;
	zr = zi = 0.0;
	for ( count=0 ; count<Threshold ; count++ ) {
		if ( zi*zi+zr*zr > 4.0)
			break;
		newr = zr*zr-zi*zi+pt.a;
		zi = 2.0*zr*zi+pt.b;
		zr = newr;
	}
	return count;
}
/*
 *===================================================
 * procargs - unified argument processing procedure
 *===================================================
 *
 * This procedure contains the logic for converting the UNIX argument
 * list into global variables
 */
procargs(argc, argv)
int argc;
char *argv[];
{
	int arg, skip, unswitched, more;
	/*
	 * put default values for arguments here
	 */
	unswitched = 0;
	for ( arg=1 ; arg<argc ; arg ++ ) {
		if ( argv[arg][0] == '-' ) {
			switch ( argv[arg][1] ) {
			case 'p':
			case 'c':
				Concurrency = atoi(argv[++arg]);
				break;
			case 'r':
				sscanf(argv[++arg],"%f",&Min.a);
				sscanf(argv[++arg],"%f",&Max.a);
				break;
			case 'i':
				sscanf(argv[++arg],"%f",&Min.b);
				sscanf(argv[++arg],"%f",&Max.b);
				break;
			case 's':
				Size = atoi(argv[++arg]);
				break;
			case 't':
				Threshold = atoi(argv[++arg]);
				break;
			default:
				usagexit(argv[0]);
			}
		} else { /* there's no dash in front */
			switch ( unswitched++ ) {
			default:
				usagexit(argv[0]);
			}
		}
	}
}
usagexit(pgm)
char *pgm;
{
	fprintf(stderr,"usage: %s \n",pgm);
	exit(1);
}
