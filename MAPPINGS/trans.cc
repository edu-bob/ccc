pragma nolinesynch;

process spec foo(int,float)
{
	trans action();
	trans fred(int);
};

process body foo(i, x)
{
	int temp;
	temp = i;
}
