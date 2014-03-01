/*---------------------------------------------------------------------------
 * In the following structures, id refers to the unique id given for each
 * interaction appearing in the source code.  context refers to the namespace
 * for the interaction, such as tblocks for accept and trans specs.
 *---------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------
 * the following deal with ordering structure field entries in the symbol
 * table 
 *---------------------------------------------------------------------------
 */
typedef struct spec_fieldTag  {		/* holds a structure field entry */
	symbol	*symbol;
	struct spec_fieldTag *next;
} spec_field;

typedef struct {			/* array [NameSpace] of these */
	spec_field *fields;
	spec_field *fieldsTail;
} spec_structSpace;


/*---------------------------------------------------------------------------
 * specs
 *---------------------------------------------------------------------------
 */

typedef struct parm_tag {
	char	*name;
	struct	parm_tag *next;
} spec_parameters;

typedef struct spec_processTag  {
	char	*name;
	int	num;
	spec_parameters *formal;
	spec_parameters *formalTail;
	spec_parameters *typeString;
	spec_parameters *typeStringTail;
	spec_field	*trans;
	spec_field	*transTail;
	struct spec_processTag *next;	/* used to access processes linearly */
} spec_process;	



typedef struct {			/* for accept */
	int	id;			/* unique source id number */
	unsigned lineNumber;		/* source location	*/
	unsigned space;			/* for cdb to associate with process */
	symbol	*symbol;		/* entry in the symbol table */
	spec_parameters *formal;	/* formals on create and accept */
	spec_parameters *formalTail;		
} spec_accept;

typedef struct {
	int	id;
	unsigned lineNumber;
	unsigned space;			/* for cdb to associate with process */
	spec_process *params;
} spec_create;

typedef struct {
	int	id;
	unsigned lineNumber;
	unsigned space;			/* for cdb to associate with process */
	symbol	*symbol;
} spec_transact;

typedef spec_transact spec_treturn;

typedef struct{
	int	id;
	unsigned lineNumber;
	unsigned space;			/* for cdb to associate with process */
	int	termLine;		/* location of terminate in select */
	int	immedLine;		/* location of immediate in select */
} spec_select  ;

typedef struct {			/* for delay, terminate */
	int	id;
	int	context;		/* namespace of the process	*/
	int	lineNumber;
	unsigned space;			/* for cdb to associate with process */
} spec_other;

#define PARMsIZE 1000
#define FORMsIZE 100

#define listAdd(head,entry) if (head == NULL) head = entry;\
				 else head/**/Tail->next = entry;\
				 head/**/Tail = entry;


spec_process *spec_lookupByNum();
