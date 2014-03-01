/*---------------------------------------------------------------------------
 * These routines determine at what points breakpointing occurs.  They use
 * the same points routines as the trace functions, because break and
 * trace points are exactly the same.
 * 
 * William L. Lynch		Fri Sep  5 12:39:07 1986
 *---------------------------------------------------------------------------
 */

#include "cdb.h"
#include "../bool.h"
#include "../DD_include.h"
#include "static.h"
#include "dynamic.h"


extern int cdbCycle;

static bool breakingFlag = TRUE;

breaking( flag )
bool flag;
{
	breakingFlag = flag;
}


breaks( pp, pp2, event )
process *pp, *pp2;
DD_eventType event;
{
	if ( !breakingFlag )
		return;

	switch ( event ) {
	case DD_TRETURN:
		if ( !points_treturn(pp, BREAK) )
			return;
		cdbStatus("break %s treturn", pp->tag);
		break;
	case DD_WAITbORN:
		if ( !points_create(pp, BREAK) )
			return;
		cdbStatus("break %s create", pp->tag);
		break;
	case DD_WAITtRANS:
		if ( !points_transact(pp, BREAK) )
			return;
		cdbStatus("break %s transaction", pp->tag);
		break;
	case DD_ACCEPT:
		if ( !points_accept(pp, BREAK) )
			return;
		cdbStatus("break %s accept", pp->tag);
	        break;
	case DD_WAITsELECT:
		if ( !points_select(pp, BREAK) )
			return;
		cdbStatus("break %s select", pp->tag);
	        break;
	case DD_DELAY:
		if ( !points_delay(pp, BREAK) )
			return;
		cdbStatus("break %s delay", pp->tag);
	        break;
	default:
		return;
	        break;
	}
	command_cdb(cdbCycle, 1, 0);	/* stop */
}
