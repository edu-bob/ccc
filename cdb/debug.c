#include <stdio.h>
#include "../bool.h"
#include "cdb.h"
#include "static.h"
#include "dynamic.h"
#include "tables.h"
static
#include "stateString.h"

extern processList *all;
FILE *fd;
bool init = FALSE;

debug_dump()
{
	processList *plp;
	process *pp;
	staticProcessInfo *info;
	int i;
		
	if (!init) debug_open();
	
	for ( plp=all; plp != NULL; plp = plp->next ) {
		pp = plp->process;
		fprintf(fd, "\n%s\nSTATE: %s, ID: %d, UNIXPID: %d\n",
			pp->tag, stateString[(int)pp->state], pp->id, pp->pid);
		fprintf(fd, "PARMS: %d in space %d\n", 
				pp->numParms, pp->spaceParms);
		fprintf(fd, "POINTS: traces, breaks\n");
		info = pp->info;
#define prPt(pt,cap, quoted)\
		fprintf(fd, "%s:\n", quoted);\
		for ( i=0; i<info->points.num/**/cap; i++ )\
			fprintf(fd, "%d, %d\n", pp->point.pt[i].traces,pp->point.pt[i].breaks)

		prPt(accept,Accepts, "accept");
		prPt(create,Creates, "create");
		prPt(transact,Transacts, "transact");
		prPt(select,Selects, "select");
		prPt(delay,Delays, "delay");
		prPt(terminate,Terminates, "terminate");
		prPt(treturn,Treturns, "treturn");
		
		

	}
}

debug_printf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9)
char *s;
{
	char debugString[160];
	
	if (!init) debug_open();
	sprintf(debugString, s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	write(fileno(fd), debugString, strlen(debugString));
}

debug_open()
{
	fd = fopen("cdebug.out", "w");
	setbuf(fd,0);
	init = TRUE;
}
