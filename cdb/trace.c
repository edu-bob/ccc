/*---------------------------------------------------------------------------
 * These routines determine what gets traced and displayed by cdb.
   treturns get added to a list and removed from display at the next step,
   as they have no reply event to clear them.
 * 
 * William L. Lynch		Tue Aug 12 10:42:31 1986
 *---------------------------------------------------------------------------
 */

#include "../bool.h"
#include "../DD_include.h"
#include "cdb.h"
#include "static.h"
#include "dynamic.h"

#define NULL 0

#define bit(x)	(1 << (x))

static bool tracingFlag = TRUE;

tracing( flag )
bool flag;
{
	tracingFlag = flag;
}


trace( pp, pp2, event )
process *pp, *pp2;
DD_eventType event;
{
	if ( !tracingFlag )
		return;

	switch ( event ) {
	case DD_TRETURN:
		display_eraseLine(pp->with, pp);
		if ( !points_treturn(pp, TRACE) )
			return;
		display_parms(pp);
		trace_addTreturn(pp);
		break;
	case DD_TRETURNED:
		display_eraseParms(pp);
	        break;
	case DD_ISbORN:
		if ( !points_create(pp2, TRACE) )
			return;
		display_eraseParms(pp2);
		display_process(pp2);
	        break;
	case DD_WAITbORN:
		if ( !points_create(pp, TRACE) )
			return;
		display_parms(pp);
		break;
	case DD_WAITtRANS:
		if ( !points_transact(pp, TRACE) )
			return;
		display_parms(pp);
		break;
	case DD_IMMEDsERV:
		if ( !points_transact(pp, TRACE) )
			return;
		break;
	case DD_ACCEPT:
		if ( !points_accept(pp, TRACE) || !points_transact(pp2, TRACE) )
			return;
		display_parms(pp);
		display_drawLine(pp, pp2);
	        break;
	case DD_WAITsELECT:
		if ( !points_select(pp, TRACE) )
			return;
	        break;
	case DD_DELAY:
		if ( !points_delay(pp, TRACE) )
			return;
	        break;
/* terminate trace points not implemented. ********
	case DD_TERMINATE:
		if ( !points_terminate(pp, TRACE) )
			return;
	        break;
*/
	default:
	        break;
	}

	display_process(pp);
}




static processList *treturnsList = NULL;

trace_clearTreturns()
{
	processList *plp;
	
	while ( treturnsList ) {
		plp = treturnsList;
		treturnsList = treturnsList->next;
		free(plp);
	}
}

trace_addTreturn( pp )
process *pp;
{
	processList *plp;
	
	
	if ( !tracingFlag )
		return;
	plp = instance(processList);
	plp->next = treturnsList;
	plp->process = pp;
	treturnsList = plp;
}

trace_eraseTreturns()
{
	processList *tmp;

	if ( !tracingFlag )
		return;
	while ( treturnsList != NULL ) {
		display_eraseParms(treturnsList->process);
		tmp = treturnsList;
		treturnsList = treturnsList->next;
		free(tmp);
	}
}


/*---------------------------------------------------------------------------
 * Ascii tracing
 *---------------------------------------------------------------------------
 */

#include <stdio.h>
static
#include "stateString.h"

static FILE *ascii = NULL;

asciiFile( fileName )
string fileName;
{
	ascii =  fopen(fileName, "w");
	if ( ascii == NULL )
		fatal(1, "Can't open %s.\n", fileName);
	stateString[(int)treturned] = "running, treturned";
}


trace_ascii( pp, pp2, event )
process *pp, *pp2;
DD_eventType event;
{
	if ( ascii == NULL || event == DD_IMMEDsERV )
		return;

	asciiProcess(pp);
	switch ( event ) {
	case DD_ISbORN:
		fprintf(ascii, " from %s pid=%d", pp2->tag, pp->pid);
	        break;
	case DD_TRETURN:
	case DD_WAITbORN:
		fprintf(ascii, " %s ", pp->info->points.create[pp->id].name);
		asciiParms(pp);
		break;
	case DD_WAITtRANS:
		fprintf(ascii, " on %s.%s line %d ", pp->with->tag,
				pp->info->points.transact[pp->id].name,
				pp->info->points.transact[pp->id].line);
		asciiParms(pp);
	        break;
	case DD_ACCEPT:
		fprintf(ascii, " %s from %s line %d ",
					pp->info->points.accept[pp->id].name, 
					pp->with->tag,
					pp->info->points.accept[pp->id].line );
		asciiParms(pp);
	        break;
	}
	fprintf(ascii, "\n");
}

asciiParms( pp )
process *pp;
{
	int i;
	string comma;
	
	comma = "";
	fprintf(ascii, "(");
	for ( i=0; i < pp->numParms; i++ ) {
		fprintf(ascii, "%s%s", comma, pp->parms[i]);
		comma = ",";
	}
	fprintf(ascii, ")");
}

asciiProcess( pp )
process *pp;
{
	fprintf(ascii, "%d %s is %s", addTime(0), pp->tag, stateString[(int)pp->state]);
}

