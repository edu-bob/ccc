pragma nolinesynch;

process spec myproc()
{
	trans float first(char,short);
	trans void second(int);
};
main()
{
	process myproc myproc;
	myproc = create myproc();
	myproc.first('a',017777);
	within 5 ? myproc.second(26) : 27;
}
process body myproc()
{
	char ch;
	short sh;

	accept first(ch,sh) {
		ch = sh / 2;
	}
}
