/*---------------------------------------------------------------------------
 * Routines to convert parm strings (from creates, accepts, and treturns)
 * to individual strings for printing.  Structure field tags are placed in
 * the strings, and pids are converted to cdb tags.
 * 
 * William L. Lynch		Thu Aug 14 15:08:26 1986
 *---------------------------------------------------------------------------
 */

#include <ctype.h>
#include "cdb.h"
#include "../objs.h"
#include "../bool.h"
#include "static.h"
#include "dynamic.h"

#define MAXpARMsIZE	256

extern symbol symbolTable[];

/* externals, forwards */
symbol *static_lookup();
symbol *static_symbol();
process *lookupTaskp();


/* strAdd leaves s1 pointing at the end of the string, which is more
 efficient for the next add */
strAdd( s1, s2 )
string *s1, s2;
{
	while ( *s2 )
		*(*s1)++ = *s2++;
}

convert_struct( parms, symbolp, result )
string *parms;		/* "{...}..." */
symbol *symbolp;
string *result;		/* starting place for the result */
{
	int symIndex;
	symbol *sym;
	string comma;
		
	symIndex = static_lookup("DD_first", symbolp->num)->num;
	strAdd(result, "{"); (*parms)++;
	sym = static_symbol(symIndex);
	comma = "";
	do {
		strAdd(result, comma);
		strAdd(result, sym->name);
		strAdd(result, "=");
		convert_value( parms, sym, result);
		sym = static_symbol(++symIndex);
		comma = ",";
	} while ( sym->space == symbolp->num );
}


convert_process( parms, symbolp, result )
string *parms;
symbol *symbolp;
string *result;
{
	unsigned task;

	sscanf(*parms, "%d", &task);
	while ( isdigit(*(*parms)++) );
	strAdd(result, lookupTaskp(task)->tag);
}


convert_value( parms, symbolp, result )
string *parms;		/* ".,..." */
symbol *symbolp;
string *result;
{
	if ( symbolp->chain == 0 )
		switch ( symbolp->obj ) {
		case e_struct:
			convert_struct(parms, symbolp, result);
			return;
		case e_process:
			convert_process(parms, symbolp, result);
			return;
		};
	while ( **parms != ',' )
		*(*result)++ = *(*parms)++;
	(*parms)++;
}


convert_parms( parms, num, symbols, results )
string parms;
int num;
symbol symbols[];
string results[];
{
	int i;
	char ans[MAXpARMsIZE];
	char *parmp, *ansp;
	
	strcat(parms, ",");
	parmp = parms;
	for ( i=0; i < num; i++ ) {
		if ( strcmp(parmp, "") == 0 ) {
			cdbStatus("Incomplete parameters");
			break;
		}
		ansp = ans;
		convert_value(&parmp, &symbols[i], &ansp);
		*ansp = '\0';
		results[i] = newstr(ans);
	}
}
