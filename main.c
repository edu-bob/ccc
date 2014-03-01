/*
** Concurrent C translator
**
** Joe Barerra, RIACS, 1985
** Bob Brown, RIACS, 1985
** Bill Lynch, RIACS, 1986
**
** Input - named .cc file on argument list (just one!)
** Output - translated programs on stdout
**
** System interface done by Bob Brown, RIACS, 1986
*/
#include <stdio.h>
#define EXTERN
#include "common.h"
#include "bool.h"
bool debugFlag;


main(argc, argv)
int argc;
char *argv[];
{
	int yyparse();
	int status;

		
	Filename = "stdin";
	while ( argc > 1 ) {
		if ( strcmp(argv[1], "-G") == 0) {
			debugFlag = TRUE;
		} else {
			if (freopen(argv[1],"r", stdin) == NULL ) {
				perror(argv[1]);
				exit(1);
			}
		Filename = argv[1];
		}
	argv++;
	argc--;
	}
	Linenum = 1;
	spec_init();
	gram_init();
	capture_init();
	status=yyparse();
	gram_end();
	flusherrors();
	if ( debugFlag )
		spec_printGlobals();
	
	return status;
}
