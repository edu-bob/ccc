
pragma nolinesynch;

#include <stdio.h>
union untype {
	int a;
	long b;
	char c;
} un;



typedef struct comp {
	int a;
	struct comp *self;
	union untype bar;
	union sec {
		int f,g;
	} the;
	union sec h,i;
	
	
	char ch;
} complicated;


complicated c5, c6;

struct u {
		int i,j;
		complicated k;
		struct u *uptr;
		
	} unc;





process spec foo(complicated, complicated, int) 
{
	trans complicated a(int,int, int, int);
	trans int b(complicated, complicated);
	trans void c();
};



process body foo(c1, c2, i)
{
	int i;
	complicated j;
	

	accept a(i1, i2, i3, i4) {
	treturn c1;		
	};
	
	accept b(c3,c4) {
	c3.a = 5;
	c3.self = (&c3);
	c4.bar.c = 'c';
	treturn 3;		
	};
	
	

	c2.a = 5;
	c2.self = (&c1);
	c2.bar.c = 'c';
	


}

main (argc,argv)
int argc;
char *argv[];
{
	process foo fooo;
	float f;
	FILE *fd;
	
	
	fd = fopen("tbuf.out", "w");
	fprintf(fd, "tbuf starting\n");
	
	printf("Tbuf main STARTING.\n");

	fooo = create foo(c5, c5, 9);
	
	fooo.a(5,6, 7, 8);
	fooo.b(c6,c6);
	
	
}


