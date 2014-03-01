#include <stdio.h>


#define LIFE_LIMIT 10000
#define MAXP 5


process spec fork()
{
	trans	void	pick_up();
	trans	void	put_down();
};

process spec philosopher ( int, process fork, process fork );


process body philosopher( id, left, right )
{
	int	times_eaten;

	for (times_eaten = 0; times_eaten != LIFE_LIMIT; times_eaten++) {
		right.pick_up();
		left.pick_up();
		printf("Philosopher %d: **\n",id);
		left.put_down();
		right.put_down();
	}

	printf("Phil %d: See you\n",id);
}

process body fork()
{
	for(;;)
		select {
			accept pick_up();
			accept put_down();
		or
			terminate;
		}
}

main()
{
process fork f[MAXP];
int j;

for (j=0; j<MAXP; j++)
	f[j] = create fork();
for (j=0; j<MAXP;j++)
	create philosopher(j, f[j], f[(j+1) % MAXP]);

}


