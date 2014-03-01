/*--------------------------------------------------------------------------------*/
/* TYPE CHASING FUNCTIONS */


typedef struct {
    unsigned short num:12;	/* namespace number			*/
    unsigned short obj:4;	/* see defines below			*/
} t_typespec;

typedef struct {
    unsigned chain;		/* pointer and function chain		*/
    t_typespec typespec;	/* type specifier			*/
} t_type;

typedef struct {
    short a,z;
    t_type t;
} t_trap;

typedef union {
    t_type y_type;
    t_trap y_trap;
    int y_int;
} yystype;

#include "objs.h"

t_type ref(),func(),deref(),defunc(),addr(),maketype();
t_type arithassign(),add(),subtract(),inc();
t_type assign(),compare(),internal_lookup(),lookup();
t_type condition(),arith(),addassign(),array();
t_type lookup_tag(),lookup_var(),lookup_last();
t_type half_declare_tag(),half_declare_func();
int declare_tag();

#define declare_typedef(name,type)	declare(name,NS_TYPEDEF,type,0)
#define declare_var(name,num,t)		declare(name,num,t,0)


/*---------------------------------------------------------------------------
 * Symbol table defs.
 *---------------------------------------------------------------------------
 */



typedef struct {
	char	*name;			/* identifier name			*/
	unsigned short num:14;		/* number which identifies namespace	*/
	unsigned short typedefp:2;	/* is this a typedef declaration?	*/
	short	halfdec;		/* is this an incomplete declaration?	*/

	t_type	t;			/* type					*/
	char	*alias;			/* name printed on output		*/
} symbol;

struct table_tag {
	struct btree *table;	/* table for a particular scope level	*/
	struct table_tag *prev;	/* more global scope level		*/
	struct table_tag *next;	/* more local scope level		*/
};

symbol *slookup();
