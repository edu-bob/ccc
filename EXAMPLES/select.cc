/* select.cc */

/*
 *  This program tests various select alternatives.
 *  The exact times can be fiddled with to look at various aspects
 *  of the select statement.
 */

shared int guard1, guard2, guard3, guard4, guard5;

process spec fred()
{
    trans t();
};

process body fred()
{
    select {
	(0):
	    immediate
	    printf("this should not be printed.\n");
	otherwise
	    printf("this should be printed.\n");
    }
    select {
	(1):
	    immediate
	    printf("this should be printed.\n");
	otherwise
	    printf("this should not be printed.\n");
    }
    for (;;) {
	delay 0.25;
	printf("at top of select.\n");
	select {
	    (guard1):
		immediate
		printf("guard 1 true, ");
		printf("immediate alternative chosen.\n");
	or
	    (guard2):
		accept t() {
		    printf("guard2 true, transaction t is being accepted.\n");
		}
		printf("transaction t is done being accepted.\n");
	or
	    (guard3):
		delay 10.0;
		printf("guard3 true, ten seconds have past.\n");
	or
	    (guard4):
		delay 20.0;
		printf("guard4 true, twenty seconds have past.\n");
	or
	    (guard5):
		terminate;
	}
    }
}

main(argc)
int argc;
{
    process fred f;

    guard1 = 0;
    guard2 = 0;
    guard3 = 0;
    guard4 = 0;
    guard5 = (argc > 1);
    printf("guard5 (for terminate) is now true.\n");
    f = create fred();
    guard4=1;
    printf("guard4 (for delay 2.0) is now true.\n");
    delay 29.5;
    guard3=1;
    printf("guard3 (for delay 1.0) is now true.\n");
    delay 29.5;
    guard2=1;
    printf("guard2 (for accept t) is now true.\n");
    delay 29.5;
    f.t();
    delay 29.5;
    guard1=1;
    printf("guard1 (for immediate) is now true.\n");
    delay 1.0;
    guard1=0;
    printf("guard1 (for immediate) is now false again.\n");
}
