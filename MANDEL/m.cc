#include <stdio.h>

process spec getline()
{
	trans int nextline();
};
process spec output()
{
	trans int display(int);
	trans int done();
};
process spec mandel(int, process getline, process output)
{
	trans int done();
};

int Concurrency;

main(argc,argv)
int argc;
char *argv[];
{
	int i;

	process getline g;
	process output o;
	process mandel m[128];

	if ( argc > 1 )
		Concurrency = atoi(argv[1]);
	else
		Concurrency = 5;

	g = create getline();
	o = create output();

	for ( i=0 ; i<Concurrency ; i++ )
		m[i] = create mandel(i,g,o);
	for ( i=0 ; i<Concurrency ; i++ )
		m[i].done();
}

process body getline()
{
	int next;

	next = 0;
	while (1) {
		select {
			accept nextline() {
				if ( next < 256 )
					treturn next++;
				else
					treturn -1;
			}
		or
			terminate;
		}
	}
}
process body mandel(i,get,put)
{
	int line;

	while ((line=get.nextline()) >= 0 ) {
		printf("Process %d got line %d\n",i,line);
		put.display(line);
	}
	printf("Process %d done\n",i);
	accept done();
}
process body output()
{
	int line;

	while (1) {
		select {
			accept display(line) {
				printf("putline(%d)\n",line);
			}
		or
			terminate;
		}
	}
}
