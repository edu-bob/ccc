pragma nolinesynch;

process spec notrans(float,double);
process spec hastrans()
{
	trans float mytrans(double);
};

process body notrans(x,y)
{
}
process body hastrans()
{
}
