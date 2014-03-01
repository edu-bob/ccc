/* testshared.cc */

/*
 *  This program tests various shared variable declarations.
 */

process spec writer()
{
    trans write1();
    trans write2();
};

process spec reader()
{
    trans read();
};

shared int a[10];
shared int b;
shared char c[10];
shared int d[2][2][2][2];

process body writer()
{
    int j,k,l,m;

    accept write1() {
	for (j=0;j<10;j++)
	    a[j]=j;
	b=333;
	strcpy(c,"hi there!");
	for (j=0;j<2;j++)
	    for (k=0;k<2;k++)
		for (l=0;l<2;l++)
		    for (m=0;m<2;m++)
			d[j][k][l][m]=j*1000 + k*100 + l*10 + m;
    }
    accept write2() {
	for (j=0;j<10;j++)
	    a[j]=10-j;
	b= -777;
	strcpy(c,"123456789");
	for (j=0;j<2;j++)
	    for (k=0;k<2;k++)
		for (l=0;l<2;l++)
		    for (m=0;m<2;m++)
			d[j][k][l][m]=j*8 + k*4 + l*2 + m;
    }
}

process body reader()
{
    int j,k,l,m;

    accept read() {
	for (j=0;j<10;j++)
	    printf("a[%d] = %d.\n",j,a[j]);
	printf("b = %d.\n",b);
	printf("c = \"%s\"\n",c);
	for (j=0;j<2;j++)
	    for (k=0;k<2;k++)
		for (l=0;l<2;l++)
		    for (m=0;m<2;m++)
			printf("d[%d][%d][%d][%d] = %d.\n",j,k,l,m,d[j][k][l][m]);
    }
}

main()
{
    process reader r;
    process writer w;

    w = create writer();
    r = create reader();

    w.write1();
    r.read();


    r = create reader();

    w.write2();
    r.read();
}
