/*---------------------------------------------------------------------------
 * The following pertains to the addition of the symbol table information
 * to the end of the .c file.
 *---------------------------------------------------------------------------
 */

typedef char *DD_formal;		/* for accept and create */
typedef DD_formal DD_formals[];

typedef struct {
	char	 *name;
	unsigned line;			/* source location */
#ifndef CDB
	unsigned tblockSize;
	void	(*convert)();
#endif
	unsigned numFormals;		/* number of entries in following field */
	DD_formal *formals;
} DD_occur;			


typedef struct {		/* info about an occurrence of a un-named event */
	unsigned	line;
} DD_otherOccur;

typedef struct {
	unsigned	line;
	unsigned	termLine;		/* location of a select terminate */
	unsigned	immedLine;		/* location of a select immediate */
} DD_selectOccur;

typedef struct {
	unsigned	line;
#ifndef CDB
	unsigned	valueSize;
	void		(*convert)();
#endif
} DD_treturnOccur;
	
typedef DD_occur DD_occurs[];
typedef DD_otherOccur DD_otherOccurs[];
typedef DD_selectOccur DD_selectOccurs[];
typedef DD_treturnOccur DD_treturnOccurs[];

typedef struct {
	char	*name;
	unsigned short space;		/* namespace */
	unsigned short num;		/* namespace of subelements */
	unsigned chain;
	unsigned short obj;		/* type */
} DD_symEntry;  


typedef struct {
	unsigned	numAccepts;
	DD_occur	*accept;
	unsigned	numCreates;
	DD_occur	*create;
	unsigned	numTransacts;
	DD_occur	*transact;
	unsigned	numSelects;
	DD_selectOccur	*select;
	unsigned	numDelays;
	DD_otherOccur	*delay;
	unsigned	numTerminates;
	DD_otherOccur	*terminate;
	unsigned	numTreturns;
	DD_treturnOccur	*treturn;
} DD_ProcessTable;

typedef struct {
	char *name;
	DD_ProcessTable *table;
} DD_globalOccurs;

#ifndef CDB
typedef enum {
	DD_askTable, DD_askCreate, DD_askAccept, DD_askTrans, DD_askTreturn
} DD_question;

#define DD_numAnd(x)	sizeof x / sizeof x[0], x

DD_ProcessTable *DD_processTable;

char *DD_dummy;			/* for the treturn sizeof */
extern char *DD_createArgs;
#endif
