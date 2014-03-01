/*---------------------------------------------------------------------------
 * This code is added for the use of the debugger.  It saves a tree of
 * dependencies of the lowest level symbol table (which is the only place
 * that typedefs can occur which affect transaction and create types) so
 * that the debugger only need transfer the used portions of the symbol
 * table to the output process.
 * 
 * The method is to create a btree with two entries per record.  The first
 * is the current namespace, and the second is the namespace depending on
 * this one.  When the table is to be sent, the array "used" is marked to
 * show which symbols the debugger needs (currently, this is create and
 * transaction types).  The print routine is then run
 * which adds records to the static data at the end of the object c code.
 * 
 * William L. Lynch		Thu Jul  3 10:13:35 1986
 *---------------------------------------------------------------------------
 */


#include <stdio.h>
#include "btree.h"
#include "cctypes.h"
#include "bool.h"

#define MATCHaLL	-1
#define TREEoRDER	10
 
#define instances(n, t)		(t *)calloc((n), sizeof (t))
#define instance(t)		instances(1, t)

typedef struct {
	int space;		/* current namespace */
	int dependsOn;		/* namespace depending on preceeding */
} entry;

struct btree *depend;


	


entry *
depend_Entry( space, dependsOn )
int space, dependsOn;
{
	entry *e;
	
	e = instance(entry);
	e->space = space;
	e->dependsOn = dependsOn;
	return e;
}



depend_s( name, space, dependsOn )	/* read "depends", as in depends on */
int space, dependsOn;
char *name;

{
	if (strcmp(name, "0")) {  /* get rid of this entry, which uses the
				   num field for a transaction parameter
				   counter */
		btinsert(depend, depend_Entry(space, dependsOn));		
	}
}



int
depend_Compare( x, y )
entry *x, *y;
{
	int i;
	
	if (i = (x->space - y->space)) 
		return i;
	if (x->dependsOn == MATCHaLL || y->dependsOn == MATCHaLL ) 
		return 0;
	return (x->dependsOn - y->dependsOn);
}


bool *used;

markSubTree(nameSpace)
int nameSpace;
{
	entry *e, *newE;
	
	used[nameSpace] = TRUE;
	e = depend_Entry(nameSpace, MATCHaLL);
	while (newE = (entry *)btdelete(depend, e)) {
		used[newE->dependsOn] = TRUE;
		markSubTree(newE->dependsOn);
		cfree(newE);
	}
}


extern int NameSpace;

depend_Output( symbolTree )
struct btree *symbolTree;
{
	int maxSpaces = NameSpace;
	int depend_OutputNode();

	used = instances(maxSpaces, bool);
	used[1] = TRUE;
	spec_markProcesses();
	printf("DD_symEntry DD_SymbolTable[] = {\n");
	btprint( symbolTree, depend_OutputNode ); 
	printf("};\n");
}


 
 

depend_OutputNode( s )
symbol *s;
{
	static unsigned space = -1;  /* illegal space to start */

	if (space == s->num)	/* has already been printed by spec ordered */
		return;

	if (used[s->num]) 		
		if (spec_isOrdered(s->num)) {
			space = s->num;
			spec_printOrdered(space);
		} else 
			spec_printSymbolEntry(s);
}


depend_Init()
{
	int depend_Compare();
	
	depend = (struct btree *)btnew(TREEoRDER, depend_Compare);
}
