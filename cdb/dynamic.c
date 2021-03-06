/*---------------------------------------------------------------------------
 * These routines implement cdb's idea of the state of the user program in
 * the form of a table of processes and their states.  The processes must
 * be accesible by any of unix pid, ccc task pointer, ccc tag (process
 * spec tag), or cdb tag (give by cdb user at process creation).  Each
 * entry is entered into four btrees to allow these accesses.  The tree
   for ccc tags will actually have to contain lists of each process, as
   the tags are not unique.
 * 
 * William L. Lynch		Mon Aug 11 12:38:51 1986
 *---------------------------------------------------------------------------
 */
/*#define Debug*/

#include "../btree.h"
#include "../DD_include.h"
#include "../objs.h"
#include "../bool.h"
#include "cdb.h"
#include "static.h"
#include "tables.h"
#include "dynamic.h"

#define NULL 0

string getTag();
symbol *static_lookup();

static symbol mainSymbol = {"",0,0,0,0};
				   
static processList *nameTable;
static process *tagTable;
static process *pidTable;
static process *taskpTable;
processList *all = NULL;		/* list of all processes */


#define strCmp(a,b)	strcmp(a,b)
#define	numCmp(a,b)	a-b
#define cmpFunc(type, cmp, tt)	int type/**/Cmp( s1, s2 )\
				tt *s1, *s2;\
				{return cmp(s1->type,s2->type);}

cmpFunc(tag,strCmp,process)
cmpFunc(pid,numCmp,process)
cmpFunc(taskp,numCmp,process)

nameCmp(s1, s2)
processList *s1, *s2;
{
	return strcmp(s1->process->name, s2->process->name);
}

dynamic_init()
{
	nameTable = (processList *)btnew(10, nameCmp);
	tagTable = (process *)btnew(10, tagCmp);
	pidTable = (process *)btnew(10, pidCmp);
	taskpTable = (process *)btnew(10, taskpCmp);
}



process *
lookupTaskp(taskp)
unsigned taskp;
{
	process *key, *p;
	
	key = instance(process);
	key->taskp = taskp;
	p = (process *)btinsert(taskpTable, key);
	if ( p == key )
		cdbStatus("Can't find process taskp %d", taskp);
	else
		free(key);
	return p;
}

process *
lookupTag(tag)
string tag;
{
	process *key, *p;
	
	key = instance(process);
	key->tag = tag;
	p = (process *)btinsert(tagTable, key);
	if ( p == key )
		cdbStatus("Can't find process tag %s.", tag);
	else 
		free(key);
	return p;
}


process *
lookupPid(pid)
unsigned pid;
{
	process *key, *p;
	
	key = instance(process);
	key->pid = pid;
	p = (process *)btinsert(pidTable, key);
	if ( p == key )
		cdbStatus("Can't find process pid %d.", pid);		
	else 
		free(key);
	return p;
}

processList *
lookupName(name)
string name;
{
	process *key;
	processList *Key, *p;
	
	key = instance(process);
	Key = instance(processList);
	key->name = name;
	Key->process = key;
	p = (processList *)btinsert(nameTable, Key);
	if ( p == Key ) {
		cdbStatus("Can't find process name %s.", name);
		strcpy(key->name, name);
	} else {
		free(key);		
		free(Key);
	}
	return p;
}


tablesInsert(pp)
process *pp;
{
	processList *plp, *oldPlP;
	process *oldPp;
	
	oldPp = (process *)btinsert(tagTable, pp);
	if ( oldPp != pp )
		cdbStatus("Replacing process tag %s.", pp->tag);
	
	oldPp = (process *)btinsert(pidTable, pp);
	if ( oldPp != pp )
		cdbStatus("Replacing process pid %d.", pp->pid);
		
	oldPp = (process *)btinsert(taskpTable, pp);
	if ( oldPp != pp )
		cdbStatus("Replacing process task %s with %s.", oldPp->tag, pp->tag);

	plp = instance(processList);
	plp->process = pp;
	plp->next = (processList *)btdelete(nameTable, plp);
	btinsert(nameTable, plp);
	
	plp = instance(processList);
	plp->process = pp;
	plp->next = all;
	all = plp;
}


process *
update(pid, newSt, newId)
unsigned pid;
state newSt;
unsigned newId;
{
	process *pp;
	
	pp = lookupPid(pid);
	pp->state = newSt;
	pp->id = newId;	
	return pp;
}



dynamic_event(event, time, pid, toPid, id, parms, taskp)
DD_eventType event;
long time;
unsigned id, pid, toPid, taskp;
string parms;
{
	static bool isMain = TRUE;
	process *pp, *pp2 = NULL;
	symbol *firstSymbol;

#ifdef Debug
	static int i=0;
	i++;
	if ( time < 0  ) debug_printf("%d time %d\n", i, time);
#endif
	if ( isMain ) {
		isMain = FALSE;
		pp = instance(process);
		pp->name = "main";
		pp->tag = "main";
		pp->pid = pid;
		pp->taskp = NULL;
		pp->state = running;
		pp->createId = 0;
		pp->symbol = &mainSymbol;
		pp->info = static_infoLookup("main");
		display_locationInit(pp);
		display_process(pp);
		tablesInsert(pp);
		points_processInit(pp);
		command_processInit(pp);
		trace_ascii(pp, pp, DD_ISbORN);
	}
	trace_eraseTreturns();
	
	switch ( event ) {
	/* p1 packets */
	case DD_WAITaCCEPT:
		pp = update(pid, waitAccept, id);
	        break;
	case DD_WAITsELECT:
		pp = update(pid, waitSelect, id);
	        break;
	case DD_DELAY:
		pp = update(pid, delay, id);
	        break;
	/* p2 packets */
	case DD_ACCEPT:
		pp = update(pid, accepting, id);
		pp->with = pp2 = lookupPid(toPid);		
		pp->numParms = pp->info->points.accept[id].numFormals;
		pp->parms = pp->info->points.accept[id].formals;
	        break;
	case DD_TRETURNED:
		pp = update(pid, running, id);
	        break;
	case DD_IMMEDsERV:
		pp = update(pid, waitTrans, id);
		pp->with = pp2 = lookupPid(toPid);		
	        break;
	/* p3 packets  */
	case DD_WAITbORN:
		pp = update(pid, waitBorn, id);
		pp->spaceParms= static_lookup(pp->info->points.create[id].name, e_complex)->num;
		pp->numParms = static_lookup("0", pp->spaceParms)->num;
		if ( pp->numParms > 0 ) {
			pp->parms = instances(pp->numParms, string);
			firstSymbol = static_lookup("1", pp->spaceParms);
			convert_parms(parms, pp->numParms, firstSymbol, pp->parms);
		}
	        break;
	case DD_WAITtRANS:
		pp = update(pid, waitTrans, id);
		pp->with = pp2 = lookupTaskp(toPid);
		pp->transactName = pp->info->points.transact[id].name;
		pp->spaceParms = static_lookup(pp->info->points.transact[id].name, pp2->symbol->num)->num;
		pp->numParms = static_lookup("0", pp->spaceParms)->num;
		if ( pp->numParms > 0 ) {
			pp->parms = instances(pp->numParms, string);
			firstSymbol = static_lookup("1", pp->spaceParms);
			convert_parms(parms, pp->numParms, firstSymbol, pp->parms);
		}
	        break;
	case DD_TRETURN:
		pp = update(pid, treturned, id);
		pp->numParms = 1;
		firstSymbol = static_lookup(pp->with->info->points.transact[pp->with->id].name, pp->symbol->num);
		firstSymbol = static_lookup("=", firstSymbol->num);
		pp->parms = instances(pp->numParms, string);
		convert_parms(parms, pp->numParms, firstSymbol, pp->parms);
		break;
	/* p4 packets */
	case DD_WAITaBORT:
		pp = update(pid, waitAbort, 0);
	        break;
	case DD_SELECTtERM:
		pp = update(pid, selectTerm, 0);
	        break;
	case DD_WAITcHILDdIE:
		pp = update(pid, waitChildDie, 0);
	        break;
	case DD_WAITtIME:
		pp = update(pid, waitTime, 0);
	        break;
	case DD_EATtIMER:
	case DD_EATtERM:
	case DD_EATtRANS:
		pp = update(pid, eating, 0);
	        break;
	case DD_EXITING:
		pp = update(pid, dead, 0);
	        break;
	case DD_TIMEOUT:
		pp = update(pid, timeOut, 0);
	        break;
	/* p5 packets */
	case DD_LASTcHILD:
	case DD_LASTuSERkILL:
	case DD_CHILDkILL:
	case DD_IMdYING:
	case DD_ABORTINGyOU:
		pp = update(pid, dying, 0);
		pp2 = lookupPid(toPid);		
	        break;
	/* p6 packets */
	case DD_ISbORN:
		pp = instance(process);
		pp2 = lookupPid(toPid);		
		pp2->state = running;
		pp->name = pp2->info->points.create[pp2->id].name;
		pp->info = static_infoLookup(pp->name);
		pp->pid = pid;
		pp->taskp = taskp;
		pp->createId = pp2->id;
		pp->state = running;
		pp->parms = pp2->parms;
		pp->id = id;
		pp->tag = getTag(pp);
		pp->symbol = static_lookup(pp->name, e_complex);
		display_locationInit(pp);
		tablesInsert(pp);
		points_processInit(pp);
		command_processInit(pp);
	        break;
	}
	trace_ascii(pp, pp2, event);
	trace(pp, pp2, event);
	breaks(pp, pp2, event);
}



dynamic_dump()
{
	processList *plp;
	
	for ( plp=all; plp != NULL; plp = plp->next ) {
		display_clear(plp->process);
		display_process(plp->process);
		switch ( plp->process->state ) {
		case accepting:
			display_drawLine( plp->process, plp->process->with);
		case waitBorn:
		case waitTrans:
			display_parms( plp->process );
			break;
		}
	}
}


