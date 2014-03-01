/* bettor.cc */

/*
 *  This was an example in the Concurrent C papers.
 *  I have stuck in some print statements so that you can
 *  see what is going on.
 */

#include <stdio.h>
#define BETTING_LIMIT    1000

process spec bettor()
{
    trans void init(process bettor, int, int);
    trans void placebet(int);
};

process body bettor()
{
    int i_am_first, mylimit;
    int mynextbet = 1, oplastbet = 1;
    process bettor opponent;

    accept init(other_player, first_or_not, limit) {
	opponent = other_player;
	i_am_first = first_or_not;
	mylimit = limit;
    }
    if (i_am_first) {
	opponent.placebet(mynextbet);
	printf("I begin the game by betting $%d.\n",mynextbet);
    }
    while (mynextbet > 0 && oplastbet > 0) {
	accept placebet(bet) {
	    oplastbet = bet;
	}
	if (oplastbet > 0) {
	    mynextbet = oplastbet + 1 + rand()%100;
	    if (i_am_first)
		printf("I just got a bet of $%d, so I will bet $%d.\n",
		oplastbet,mynextbet);
	    if (mynextbet > BETTING_LIMIT) {
		mynextbet=0;
		if (i_am_first)
		    printf("but that's too much so I lose.\n");
	    }
	    opponent.placebet(mynextbet);
	}
    }
    if (i_am_first) {
        if (mynextbet > 0)
	    printf("I won; last bet %d\n", mynextbet);
	else
	    printf("I lost; last bet %d\n", oplastbet);
    }
}

main()
{
    process bettor a, b;

    a = create bettor();
    b = create bettor();
    printf("May the bettor process win. Good luck!\n");
    a.init(b, 1, BETTING_LIMIT);
    b.init(a, 0, BETTING_LIMIT);
}
