/*---------------------------------------------------------------------------
 * Routines to implement the points for breaking and tracing.  Commands
   are set up with the menus such that the value of the menu is one more
   than the value of the tracepoint, and that the argument to toggle the
   point is the pointType array of points.  In this way, the command menus
   don't have to look up the referencing point array.
 * 
 * William L. Lynch		Mon Sep  8 10:27:53 1986
 *---------------------------------------------------------------------------
 */

#include "../bool.h"
#include "cdb.h"
#include "static.h"
#include "dynamic.h"
#include "tables.h"

#define pointFunc(type)\
bool \
points_/**/type( pp, kind ) \
process *pp;\
unsigned short kind;\
{\
	pointType p;\
\
	p = pp->point.type[pp->id];\
	return	kind == TRACE ? p.traces == ON : p.breaks == ON;\
}

pointFunc(accept)
pointFunc(create)
pointFunc(transact)
pointFunc(delay)
pointFunc(terminate)
pointFunc(select)
pointFunc(treturn)


points_pointsInit( field, number )
pointType **field;
int number;
{
	int i;
	
	*field = instances(number, pointType);
	for ( i=0; i < number; i++ ) {
		(*field)[i].traces = ON;
		(*field)[i].breaks = OFF;
	}
}

points_processInit( pp )
process *pp;
{
	staticProcessInfo *infop;
	
	infop = static_infoLookup(pp->name);

	points_pointsInit(&pp->point.accept, infop->points.numAccepts);
	points_pointsInit(&pp->point.create, infop->points.numCreates);
	points_pointsInit(&pp->point.transact, infop->points.numTransacts);
	points_pointsInit(&pp->point.delay, infop->points.numDelays);
	points_pointsInit(&pp->point.terminate, infop->points.numTerminates);
	points_pointsInit(&pp->point.select, infop->points.numSelects);
	points_pointsInit(&pp->point.treturn, infop->points.numTreturns);

}

