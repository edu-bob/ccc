#include <stdio.h>
#include <sys/file.h>
#include "bool.h"
#include "cdb/eventString.h"

static bool init = FALSE;

FILE *debugFile;

#define DEBUGFILE 15


DD_debug_init()
{
	char *name = "debug.out";
	init = TRUE;
	
	debugFile = fopen(name, "w");
}


DD_debug_printf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9)
char *s;
{
	char debugString[160];
	
	if ( !init )
		DD_debug_init();

	sprintf(debugString, s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	write(fileno(debugFile), debugString, strlen(debugString));
}

