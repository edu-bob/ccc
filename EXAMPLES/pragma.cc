process spec doof()
{
    trans me();
};

process body doof()
{
    select {
	accept me();
    }
pragma linesynch;
    select {
	accept me();
    }
    select {
	accept me();
    }
pragma map;
    {
	int h,i,j,k;
	{
	float j,k,l;
	}
    }
pragma yydebug;
    select {
	accept me();
    }
pragma noyydebug;
    select {
	accept me();
    }
    select {
	accept me();
    }
}

main()
{
    printf("Don't look at the executable; look at the compilation\n");
    printf("and at pragma.c.\n");
}
