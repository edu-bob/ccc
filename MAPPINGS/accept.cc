pragma nolinesynch;
pragma map;

process spec foo(int,float)
{
	trans action();
	trans fred(int);
};

process body foo(i, x)
{
	int temp, temp2;
	accept fred(temp) {
		temp2 = temp;
	}
}
