/*---------------------------------------------------------------------------
 * 
 * Table manager for remembering everything about process specs.
 * 
 * The specs are kept in a btree sorted by id so that when they are
   inserted in the object c code as arrays, they are indexable by id
   number.  create, accept, treturn, and transaction specs contain pointers to the
   symbol table, used to extract the names.
 * 
 * 
 * The ordering of field entries is kept in a dynamically allocated array,
   which contains lists of field entries (symbol table pointers).  The
   array is indexed by namespace.
 * 
 * A "spec" (as in printSpec) is one of the things that has no name in the
   symbol table and is kept separately like treturns, selects, etc.
 * 
 * The printing out is done in such a fashion that the arrays are indexed
   by the id number of the occurrence.
 * 
 * William L. Lynch		Wed Jul  9 09:48:46 1986
 * 				Tue Jul 15 09:47:17 1986
 *---------------------------------------------------------------------------
 */



#include <stdio.h>
#include "cctypes.h"
#include "spec.h"
#include "bool.h"

#define newstr(s)	(char *)strcpy(malloc(strlen(s)+1),s)
#define instances(n,t)	(t *)calloc((n), sizeof (t))
#define instance(t)	instances(1,t)

extern char *parmString, *formString;	/* spec print */
extern bool debugFlag;

/*
** FORWARDS
*/

int spec_printNode(), spec_cmp(), spec_processCmp(), spec_numCmp();


/*
** GLOBALS
*/
spec_process OutsideSpec = {"", 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
spec_process *CurrentSpec = NULL;  /* used for later insertion of formals */

struct btree *AcceptTable, *mainAcceptTable;
struct btree *CreateTable, *mainCreateTable;
struct btree *TransactionTable,  *mainTransactionTable;
struct btree *SelectTable,  *mainSelectTable;
struct btree *DelayTable,  *mainDelayTable;
struct btree *TerminateTable,  *mainTerminateTable;
struct btree *TreturnTable,  *mainTreturnTable;
 
struct btree *numToProcess;

/* allows linear access to processes without knowing the names */
spec_process *process = NULL;	
spec_process *processTail;
struct btree *ProcessTable;    /* stores create formals and typestrings */

spec_structSpace *StructTable;
int bound = 100;			/* starting bound of preceding,
					 dynamically adjusted as necessary */


/*
** METHODS
*/

spec_init()
{
	ProcessTable = (struct btree *)btnew(10, spec_processCmp);
	numToProcess = (struct btree *)btnew(10, spec_numCmp);

	AcceptTable = mainAcceptTable = (struct btree *)btnew(10, spec_cmp);
	CreateTable = mainCreateTable = (struct btree *)btnew(10, spec_cmp);
	TransactionTable = mainTransactionTable =(struct btree *)btnew(10,spec_cmp);
	SelectTable = mainSelectTable = (struct btree *)btnew(10, spec_cmp);
	DelayTable = mainDelayTable = (struct btree *)btnew(10, spec_cmp);
	TerminateTable = mainTerminateTable = (struct btree *)btnew(10, spec_cmp);
	TreturnTable = mainTreturnTable = (struct btree *)btnew(10, spec_cmp);

	StructTable = instances(bound, spec_structSpace);

	formString = (char *)malloc(FORMsIZE);
	parmString = (char *)malloc(PARMsIZE);
}


spec_cmp( p1,p2 )
spec_other *p1, *p2;
{
	/* WARNING: id must be the first field in all structures passed here */
	return p1->id - p2->id;
}


spec_processCmp( p1, p2 )
spec_process *p1, *p2;
{
	return strcmp(p1->name, p2->name);
}

spec_numCmp( p1, p2 )
spec_process *p1, *p2;
{
	return p1->num - p2->num;
}


spec_start( name, context )
char *name;
int context;
{
	spec_process *pp;
	
	pp = instance( spec_process );
	pp->name = name;
	if (pp != (CurrentSpec = (spec_process *)btinsert(ProcessTable, pp)))
		free(pp);	/* this is an error */
	listAdd(process, CurrentSpec);
	CurrentSpec->name = newstr(name);
	CurrentSpec->num = slookup(name, context)->t.typespec.num;
	/* list pointers null automatically, calloc */
	btinsert(numToProcess, CurrentSpec);
}



spec_bodyStart( name, context )
char *name;
int context;
{
	spec_process *pp;
	
	
	AcceptTable = (struct btree *)btnew(10, spec_cmp);
	CreateTable = (struct btree *)btnew(10, spec_cmp);
	TransactionTable = (struct btree *)btnew(10,spec_cmp);
	SelectTable = (struct btree *)btnew(10, spec_cmp);
	DelayTable = (struct btree *)btnew(10, spec_cmp);
	TerminateTable = (struct btree *)btnew(10, spec_cmp);
	TreturnTable = (struct btree *)btnew(10, spec_cmp);

	pp = instance( spec_process );
	pp->name = name;
	if (pp != (CurrentSpec = (spec_process *)btinsert(ProcessTable, pp)))
		free(pp);	/* this not happening is an error */
}


spec_bodyEnd()
{
	if ( debugFlag )
		spec_printOccur(CurrentSpec);

	btfree(AcceptTable, free);
	AcceptTable = mainAcceptTable;
	btfree(CreateTable, free);
	CreateTable = mainCreateTable;
	btfree(TransactionTable, free);
	TransactionTable = mainTransactionTable;
	btfree(SelectTable, free);
	SelectTable = mainSelectTable;
	btfree(DelayTable, free);
	DelayTable = mainDelayTable;
	btfree(TerminateTable, free);
	TerminateTable = mainTerminateTable;
	btfree(TreturnTable, free);
	TreturnTable = mainTreturnTable;

	CurrentSpec = &OutsideSpec;		/* outside bodies (e.g., main) */
}

spec_end()
{
	int free();

	if ( debugFlag )
		spec_printConvert(CurrentSpec);
	CurrentSpec = &OutsideSpec;		/* outside bodies (e.g., main) */
}

spec_addTransSpec( name, context )
char *name;
int context;
{
	spec_field *fp;
	
	fp = instance( spec_field );
	fp->symbol = slookup( name, context );
	listAdd(CurrentSpec->trans, fp);
}


spec_addParam( typeString )	/* add the type string, during process spec */
char *typeString;
{
	spec_parameters *pp;

	pp = instance ( spec_parameters );
	pp->name = newstr(typeString);
	pp->next = NULL;
	listAdd(CurrentSpec->typeString, pp);
}



spec_addFormal( name )
char *name;
{
	spec_parameters *pp;
	
	pp = instance(spec_parameters);
	pp->name = newstr( name );
	pp->next = NULL;
	listAdd(CurrentSpec->formal, pp);
}


static spec_accept *CurrentAccept;

spec_addAcceptParam( name )
char *name;
{
	spec_parameters *pp;

	pp = instance ( spec_parameters );
	pp->name = newstr(name);
	pp->next = NULL;
	listAdd(CurrentAccept->formal, pp);
}



spec_addAccept( name, num, lineNumber, id )
char *name;
int num;
int lineNumber;
unsigned id;
{
	CurrentAccept = instance( spec_accept );
	CurrentAccept->formal = CurrentAccept->formalTail = NULL;
	CurrentAccept->lineNumber = lineNumber;
	CurrentAccept->space = CurrentSpec->num;
	CurrentAccept->id = id;
	CurrentAccept->symbol = slookup( name, num );
	btinsert(AcceptTable, CurrentAccept);
}


				
spec_addCreate( name, context, lineNumber, id )
char *name;
int context;
int lineNumber;
unsigned id;
{
	spec_create *cp;	/* create pointer */
	spec_process pp;
	
	cp = instance( spec_create );
	cp->id = id;
	cp->lineNumber = lineNumber;
	cp->space =  CurrentSpec->num;
	/* for  a create, the formal parameters are always the same. */
	pp.name = name;
	cp->params = (spec_process *)btsearch(ProcessTable, &pp);
	btinsert(CreateTable, cp);
}




spec_addTrans( name, num, lineNumber, id )
char *name;
int num;
int lineNumber;
unsigned id;
{
	spec_transact *tp;

	tp = instance( spec_transact );
	tp->id = id;
	tp->lineNumber = lineNumber;
	tp->space = CurrentSpec->num;
	tp->symbol = slookup(name, num);
	btinsert(TransactionTable, tp);
}





/*---------------------------------------------------------------------------
 *  all of the following are specs, not occurs (i.e., they have no symbol entry).
 *---------------------------------------------------------------------------
 */
static spec_select *CurrentSelect;

spec_addSelect( lineNumber, id )
int lineNumber;
unsigned id;
{
	spec_select *sp;
	
	sp = instance( spec_select );
	sp->lineNumber = lineNumber;
	sp->space = CurrentSpec->num;
	sp->id = id;
	btinsert(SelectTable, sp);
	CurrentSelect = sp;
}


spec_addSelectTerminate( lineNumber )
int lineNumber;
{
	CurrentSelect->termLine = lineNumber;
}


spec_addSelectImmediate( lineNumber )
int lineNumber;
{
	CurrentSelect->immedLine = lineNumber;
}



spec_addTerminate( lineNumber, id )
int lineNumber;
unsigned id;
{
	spec_other *tp;
	
	tp = instance( spec_other );
	tp->lineNumber = lineNumber;
	tp->space = CurrentSpec->num;
	tp->id = id;
	btinsert(TerminateTable, tp);
}


spec_addDelay( lineNumber , id)
int lineNumber;
unsigned id;
{
	spec_other *dp;
	
	dp = instance( spec_other );
	dp->lineNumber = lineNumber;
	dp->space = CurrentSpec->num;
	dp->id = id;
	btinsert(DelayTable, dp);
}



spec_addTreturn( name, num,  lineNumber, id )
char *name;
int num;
int lineNumber;
unsigned id;
{
	spec_treturn *rp;
	
	rp = instance( spec_treturn );
	rp->lineNumber = lineNumber;
	rp->id = id;
	rp->space = CurrentSpec->num;
	rp->symbol = slookup(name, num);
	btinsert(TreturnTable, rp);
}




spec_addStructField(name, context)
char *name;
int context;
{
	spec_field *sf;
	int i;

	if (context >= bound) {
		i = bound;
		bound *= 2;
		realloc(StructTable, bound*sizeof(spec_structSpace));
		for (; i < bound;i ++)
			StructTable[i].fields = NULL;
	}

	sf = instance(spec_field);
	sf->symbol = slookup(name, context);
	listAdd(StructTable[context].fields, sf);
}


spec_isOrdered(space)
unsigned space;
{
	return StructTable[space].fields != NULL;
}



spec_markProcesses()
{
	spec_process *pp;

	for ( pp = process; pp != NULL; pp = pp->next )
		markSubTree(pp->num);
}

spec_process *
spec_lookupByNum( num )
int num;
{
	spec_process key;
	
	key.num = num;
	return (spec_process *)btsearch(numToProcess, &key);
}

