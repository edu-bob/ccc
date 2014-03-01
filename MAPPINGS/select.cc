pragma nolinesynch;

process spec queue(int)
{
	trans int get();
	trans put(int);
};

main()
{
	process queue myqueue;
	myqueue = create queue(256);
	myqueue = create queue(256) with priority (5);
}

process body queue(i)
{
	int temp, temp2;
	select {
		accept get() {
			temp++;
		}
	or (temp>temp2):
		accept put(temp2) {
			temp2 --;
		}
	or
		terminate;
	}
}
