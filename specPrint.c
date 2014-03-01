/*---------------------------------------------------------------------------
 * 
 * Table manager for remembering everything about process specs.
 * 
 * The specs are kept in a btree sorted by id so that when they are
   inserted in the object c code as arrays, they are indexable by id
   number.  create, accept, and transaction specs contain pointers to the
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
#include "DD_kernel.h"


#define newstr(s)	(char *)strcpy(malloc(strlen(s)+1),s)
#define instances(n,t)	(t *)calloc((n), sizeof (t))
#define instance(t)	instances(1,t)

#define BASEsIZE 80		/* size of structure name */

/*
** FORWARDS
*/

int spec_printNode(), spec_cmp(), spec_processCmp(), spec_printTreturnNode();


/*
** EXTERNS
*/

char *itoa();

extern spec_process *CurrentSpec;  /* used for later insertion of formals */

extern struct btree *AcceptTable;
extern struct btree *CreateTable;
extern struct btree *TransactionTable;
extern struct btree *SelectTable;
extern struct btree *DelayTable;
extern struct btree *TerminateTable;
extern struct btree *TreturnTable;

extern struct btree *ProcessTable;    /* stores create formals and typestrings */

extern spec_structSpace *StructTable;
extern int bound;			/* starting bound of preceeding,
					 dynamically adjusted as necessary */

extern struct table_tag *Tables;	/* in cctypes.c */
extern spec_process *process;
/*
** METHODS
*/


/*---------------------------------------------------------------------------
 * The following deals with output, rather than input
 *---------------------------------------------------------------------------
 */


spec_printOccur( pp )
spec_process *pp;
{
	int spec_printNode();
	int spec_printSelectNode();
	int spec_printCreateFormals();
	int spec_printAcceptFormals();
	

	printf("\n/* DEBUG BODY */\n");
	spec_printCreateFormals(pp);
	btprint(AcceptTable, spec_printAcceptFormals);

	spec_printCreateOccurs(pp->name);
	spec_printAcceptOccurs(pp->name);
	spec_printTransactOccurs(pp->name);
	spec_printSelectOccurs(pp->name);

	spec_printSpecs("delays", DelayTable, spec_printNode, pp->name);
	spec_printSpecs("terminates", TerminateTable, spec_printNode, pp->name);

	spec_printTreturnOccurs(pp->name);

	spec_printProcessTable(pp->name);
}


/* the use of the leading empty spec in printspecs serves three functions.
 First, it simplifies commas in the printNodes routines; second, it
 simplifies what to do if there are no occurrences of a spec; and third,
 it offsets the arrays properly for indexing with id numbers. */

spec_printSpecs( name, table, printFunction, procname )
char *name;
struct btree *table;
int (*printFunction)();
char *procname;
{
	printf("static DD_otherOccurs DD_%s_%s = {\n", name, procname);
	printf("{0}");
	btprint(table, printFunction);
	printf("};\n");
}

spec_printNode( s )
spec_other *s;
{
	printf(",\n{%d}", s->lineNumber );
}


spec_printFormalList( pp )
spec_parameters *pp;
{
	while ( pp ) {
		printf(" \"%s\"%c", pp->name, pp->next ? ',' : '}');
		pp = pp->next;
	}
		printf(";\n");
}

			
spec_printCreateFormals( pp )
spec_process *pp;
{
	if ( pp->formal ) {
		printf("static DD_formals DD_formals_%s = {\n", pp->name);
		spec_printFormalList(pp->formal);
	}
}


spec_printAcceptFormals( ap )
spec_accept *ap;
{
	if ( ap->formal ) {
		printf("static DD_formals DD_acceptFormals_%d%s_%d = {\n",
				 ap->symbol->num, ap->symbol->name, ap->id);

		spec_printFormalList(ap->formal);
	}
}


spec_printCreateOccur( sp )
spec_create *sp;
{
	printf(",\n{\"%s\", %d,", 
				sp->params->name, sp->lineNumber ); 
	if ( sp->params->formal )
		printf(" sizeof DD_create_%s, DD_convert_%s, DD_numAnd(DD_formals_%s)}",
			sp->params->name, sp->params->name, sp->params->name);
	else
		printf(" 0, DD_convert_%s, 0, 0}", sp->params->name);
}

spec_printCreateOccurs( procname )
char *procname;
{
	printf("static DD_occurs DD_creates_%s = {\n", procname ); 
	printf("{\"\", 0, 0, 0, 0, 0}");
	btprint(CreateTable, spec_printCreateOccur);
	printf("};\n");
}


spec_printAcceptOccur( ap )
spec_accept *ap;
{
	printf(",\n{\"%s\", %d, sizeof(CC_%d_%s), DD_convert_%s,", 
			ap->symbol->name, ap->lineNumber,
			ap->symbol->num, ap->symbol->name,
			spec_lookupByNum(ap->symbol->num)->name);
	if ( ap->formal )
		printf(" DD_numAnd(DD_acceptFormals_%d%s_%d)}", 
					 ap->symbol->num, ap->symbol->name, ap->id);
	else 
		printf(" 0, 0}\n");
}


spec_printAcceptOccurs( procname )
char *procname;
{
	printf("static DD_occurs DD_accepts_%s = {\n", procname);
	printf("{\"\", 0, 0, 0, 0, 0}");
	btprint(AcceptTable, spec_printAcceptOccur);
	printf("};\n");
}


spec_printTreturnOccur(s)
spec_treturn *s;
{
	printf(",\n{%d, sizeof ((CC_%d_%s *)DD_dummy)->value, DD_convert_%s}",
			s->lineNumber, s->symbol->num, s->symbol->name, 
			spec_lookupByNum(s->symbol->num)->name);
}

spec_printTreturnOccurs( procname )
char *procname;
{
	printf("static DD_treturnOccurs DD_treturns_%s = {\n", procname);
	printf("{0, 0, 0}");

	btprint(TreturnTable, spec_printTreturnOccur);
	printf("};\n");
}

spec_printTransactOccur( tp )
spec_transact *tp;
{
	printf(",\n{\"%s\", %d, sizeof(CC_%d_%s), DD_convert_%s, 0, 0}",
				tp->symbol->name, tp->lineNumber,
				tp->symbol->num, tp->symbol->name,
				spec_lookupByNum(tp->symbol->num)->name);
}

spec_printTransactOccurs( procname )
char *procname;
{
	printf("static DD_occurs DD_transacts_%s = {\n", procname);
	printf("{\"\", 0, 0, 0, 0, 0}");
	btprint(TransactionTable, spec_printTransactOccur);
	printf("};\n");
}


spec_printSelectOccur(s)
spec_select *s;
{
	printf(",{%d, %d, %d}",
				s->lineNumber, s->termLine, s->immedLine);
}

spec_printSelectOccurs( procname )
char *procname;
{
	printf("static DD_selectOccurs DD_selects_%s = {\n", procname);
	printf("{0, 0, 0}");
	btprint(SelectTable, spec_printSelectOccur);
	printf("};\n");
}


spec_printProcessTable( pn ) 
char *pn;	/* procname */
{
	printf("static DD_ProcessTable DD_processTable_%s = {\
		DD_numAnd(DD_accepts_%s),\n\
		DD_numAnd(DD_creates_%s),\n\
		DD_numAnd(DD_transacts_%s),\n\
		DD_numAnd(DD_selects_%s),\n\
		DD_numAnd(DD_delays_%s),\n\
		DD_numAnd(DD_terminates_%s),\n\
		DD_numAnd(DD_treturns_%s)};\n",
		pn, pn, pn, pn, pn, pn, pn, pn);
}



spec_printSymbolEntry(s)
symbol *s;
{
	t_type t;
	t_typespec tt;
	static char *comma = "";

	t=s->t;
	tt=t.typespec;
	printf( "%s\n{\"%s\", %d, %d, %x, %d}", comma,
		s->name, s->num, tt.num, t.chain, tt.obj);
	comma = ",";
}


spec_printOrdered( space )
unsigned space;
{
	spec_field *fp;
	
	fp = StructTable[space].fields;
	while ( fp ) {
		spec_printSymbolEntry(fp->symbol);
		fp = fp->next;
	}
}

spec_printGlobalProcessTable()
{
	spec_process *pp;
	int i=0;
	char *comma;
	
	printf("DD_globalOccurs DD_globalOccurTable[] = {");
	comma = "";
	for ( pp=process; pp != NULL; pp = pp->next ) {
		i++;
		printf("%s{\"%s\", &DD_processTable_%s}\n", comma, pp->name, pp->name);
		comma = ",";
	}
	printf("};");
	printf("int DD_numberGlobals = %d;\n", i);
}

spec_printGlobals()
{
	spec_process p;
	
/* if main exists in this file, test for main in symbol table or for occurences
*  in the external table */
	p.name = "main";
	p.num = -1;
	p.formal = NULL; p.typeString = NULL; p.trans = NULL;
	spec_printOccur(&p);
	printf("DD_ProcessTable *DD_processTable = &DD_processTable_main;\n");
	p.next = process;
	process = &p;
	
	depend_Output(Tables->table);
	printf("int DD_symbolSize = sizeof DD_SymbolTable / sizeof DD_SymbolTable[0];\n");
	spec_printGlobalProcessTable();
	process = p.next;
}

/*---------------------------------------------------------------------------
** The following deal with printing out the conversion routines for
** tblocks.   formOutput implements unlimited length strings for the format
** string and  parameter list for the printf statement in the convert
** routines appearing in a -G compiled program.  formValue decides
** what to add to these strings based on the symbol table entry, and uses
** a base entry for all of the preceding structure refrences, etc.
**---------------------------------------------------------------------------
**/

static unsigned form, parm;
char *formString;
char *parmString;
	
spec_formStart()
{
	form = 0;
	*formString = '\0';
	parm = 0;
	*parmString = '\0';
}


static char *comma = "";

/* add to the end of the format and paramter strings, reallocating if necessary */
spec_formOutput(formatString, parameterString)
char *formatString;
char *parameterString;
{
	static formBound = FORMsIZE;
	static parmBound = PARMsIZE;
	
	if ( form + strlen(formatString) + 1 >= formBound ) 
		formString = (char *)realloc(formString, formBound *= 2);
	if ( parm + strlen(parameterString) + 2 >= parmBound ) 
		parmString = (char *)realloc(parmString, parmBound *= 2);

	sprintf(&formString[form], "%s%s", comma, formatString);
	form += strlen(formatString)  + strlen(comma);
	sprintf(&parmString[parm], "%s\n%s", comma, parameterString);
	parm += strlen(parameterString) + 1 + strlen(comma);
	comma = ",";
}


spec_formValue(base, sp) 
char *base;		/* the text which selects this object */
symbol *sp;
{
	spec_field *field;
	char *newBase;
	

	if ( sp->t.chain ) {  /* is a pointer */
		spec_formOutput( "%x", base );
		return;
	}	
	
	switch ( sp->t.typespec.obj ) {
	case e_struct: 
		newBase = (char *)malloc(strlen(base) + BASEsIZE);
		spec_formOutput("{","");
		comma = "";
		for (field = StructTable[sp->t.typespec.num].fields;
					field != NULL; field = field->next) {
			sprintf(newBase, "%s.%s", base, field->symbol->name);
			spec_formValue(newBase, field->symbol);
		}
		free(newBase);
		comma = "";
		spec_formOutput("}","");
	        break;
	case e_union:
		comma = "";
		spec_formOutput(",[union]", "");
	        break;
	case e_process:
	case e_int:
	case e_short:
		spec_formOutput( "%d", base );
	        break;
	case e_unsigned:
	case e_unsignedint:
	case e_unsignedchar:
	case e_unsignedshort:
		spec_formOutput( "%u", base);
	        break;
	case e_unsignedlong:
		spec_formOutput( "%lu", base );
	        break;
	case e_longint:
		spec_formOutput( "%ld", base );
	        break;
	case e_double:
	case e_float:
		spec_formOutput( "%g", base );
	        break;
	case e_char:
		newBase = (char *)malloc(2*strlen(base) + 8);
		sprintf(newBase, "DD_unctrl(%s)", base);
		spec_formOutput( "\\\'%s\\\'", newBase );
	        break;
	}
}


spec_printConvertCreate( pp )
spec_process *pp;
{
	int tblockContext;
	int paramNumber;
	symbol *param;
	char *base[BASEsIZE];
	
	spec_formStart();
	tblockContext = slookup(pp->name, NS_COMPLEX)->t.typespec.num;
	paramNumber = 0;
	comma = "";
	while ( param = slookup(itoa(++paramNumber), tblockContext) ) {
		sprintf(base, "((DD_cr_%s *)tblock)->arg%d", pp->name, paramNumber);
		spec_formValue(base, param);
	}
	if ( strcmp(parmString, "") != 0 )
		printf("sprintf(answer, \"%s\", %s);\nreturn;\n", formString, parmString );
	else 
		printf("*answer = \'\\0\';\nreturn;\n");
}


spec_printConvertTrans( pp )
spec_process *pp;
{
	spec_field *tp;		/* transaction pointer */
	int paramNumber;
	symbol *param;
	char *base[BASEsIZE];	
	int tblockContext;

	printf("switch ( id ) {\n");
	for ( tp = pp->trans; tp != NULL; tp = tp->next ) {
		spec_formStart();
		printf("case CC_%d_%s_id:\n", tp->symbol->num, tp->symbol->name);
		tblockContext = tp->symbol->t.typespec.num;
		comma = "";
		paramNumber = 0;
		while (param = slookup(itoa(++paramNumber), tblockContext)){
			sprintf(base, "((CC_%d_%s *)tblock)->arg%d",
					tp->symbol->num, tp->symbol->name,
					paramNumber);
			spec_formValue(base, param);
		}
		if ( strcmp(parmString, "") != 0 )
		printf("sprintf(answer, \"%s\", %s);\nreturn;\n",
						formString, parmString );
		else 
			printf("*answer =  \'\\0\';\n");
	}
	printf("}\n");		/* end of switch */
}

spec_printConvertTreturn( pp )
spec_process *pp;
{
	spec_field *tp;		/* transaction pointer */
	char base[BASEsIZE];	
	int tblockContext;

	printf("switch ( id ) {\n");
	for ( tp = pp->trans; tp != NULL; tp = tp->next ) {
		spec_formStart();
		printf("case CC_%d_%s_id:\n", tp->symbol->num, tp->symbol->name);
		tblockContext = tp->symbol->t.typespec.num;
		comma = "";
		sprintf(base, "((CC_%d_%s *)tblock)->value", 
						tp->symbol->num, tp->symbol->name);
		spec_formValue(base, slookup("=", tblockContext));
		printf("sprintf(answer, \"%s\", %s);\nreturn;\n",
						formString, parmString );
	}
	printf("}\n");		/* end of switch */
}


spec_printConvert( pp ) 
spec_process *pp;
{
	printf("\n/* DEBUG SPEC */\n");
	spec_printCreateStructure(pp);
	printf("\nstatic void \nDD_convert_%s(request, id, tblock, answer)\n",
								pp->name);
	printf("DD_question request;\nint id;\n char *tblock;\nchar *answer;\n{\n");
	printf("switch ( request ) {\n");
/*	printf("case DD_askTable: *(DD_ProcessTable **)answer = &DD_SpecTable;\nreturn;");*/
	printf("case DD_askCreate:\n");
	spec_printConvertCreate(pp);
	printf("case DD_askTrans: \n");
	spec_printConvertTrans(pp);
	printf("\ncase DD_askTreturn: \n");	
	spec_printConvertTreturn(pp);
	printf("}\n}\n");
	printf("DD_ProcessTable DD_processTable_%s;\n", pp->name);
}
	

spec_printCreateStructure( pp )
spec_process *pp;
{
	int i, argnum;
	char *spec_get_param_type();
	
	argnum = contextof(internal_lookup("0", pp->num));
	
	if ( argnum == 0 )
		return;		

	printf("typedef struct  {\n", pp->name);
	for ( i=0; i < argnum; i++ )
		printf("%s arg%d;\n", spec_get_param_type(pp->name,i), i+1 );
	printf("} DD_cr_%s;\n", pp->name);
	printf("DD_cr_%s DD_create_%s;\n", pp->name, pp->name);
}


/*
** Retrieval routines
**
** These are relatively inefficient because they are strictly random access,
** they do not first say which process, then ask for the next-parameter.
*/

char *
spec_get_param_type( proc, pnum )
char *proc;
int pnum;
{
	spec_process prp;
	spec_parameters *pp;
	int i;

	prp.name = proc;
	pp = ((spec_process *)btsearch(ProcessTable, &prp))->typeString;
	
	for ( i=0; i<pnum ; i++ )
		if (pp)
			pp = pp->next;
		else {
			fprintf(stderr, "More formals than types: %s.\n", proc);
			return "int ";
		}
	return pp->name;
}



