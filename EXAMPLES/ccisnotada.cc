/* ccisnotada.cc */

/*
 *  This program illustrates the fact that while the termination conditions
 *  work for Ada, when they are applied to Concurrent C they do not
 *  work perfectly because Concurrent C treats processes differently.
 *  

process spec grandchild()
{
    trans int service();
};

process spec child()
{
    trans process grandchild givechild();
};

process body grandchild()
{
    select {
	accept service() {
	    treturn 1;
	}
    or
	terminate;
    }
}

process body child()
{
    process grandchild ch;

    ch = create grandchild();
    accept givechild() {
	treturn ch;
    }
}

main()
{
    process child ch;
    process grandchild gch;

    ch = create child();
    gch = ch.givechild();
    delay 1.0;
    gch.service();
}
