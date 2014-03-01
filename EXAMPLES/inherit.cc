/* inherit.cc */

/*
 *  This program demonstrates the process spec inheritence extension of
 *  this implementation of Concurrent C. This feature allows library
 *  process body definitions which depend on any of several possible
 *  user-supplied process body definitions. This program is
 *  actually rather lame but it gets the point across.
 */

/*
 *  This is library code -- a default print_character process is defined.
 */

process spec print_character()
{
    trans printchar(char);
};

process spec print_string(process print_character, char *);

process body print_string(p, s)
{
    while (*s)
	p.printchar(*s++);
}

process spec print_one_way is new print_character;

process body print_one_way()
{
    for (;;)
	select {
	    accept printchar(c) {
		putchar(c);
	    }
	or
	    terminate;
	}
}

/*
 *  Here is the user's code -- he defines a different character-printing process.
 */

process spec print_another_way is new print_character;

process body print_another_way()
{
    for (;;)
	select {
	    accept printchar(c) {
		printf("%c",c);
	    }
	or
	    terminate;
	}
}

main()
{
    create print_string(create print_one_way(), "hi there!\n");
    create print_string(create print_another_way(), "hello!\n");
}
