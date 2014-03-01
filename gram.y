%token IDENTIFIER STRING_LITERAL SIZEOF CONSTANT X_ASSIGN A_ASSIGN PTR_OP INC_OP

%token TYPEDEF SSC STRUCT UNION ENUM BTYPE VOID TYPE_NAME

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token ACCEPT BODY BY CREATE DELAY IMMEDIATE OR OTHERWISE PRIORITY
%token PROCESS SELECT SPEC TERMINATE TRANS TRETURN WITH WITHIN

%token LABEL SHARED IS_NEW C_COUNT

%token UNSIGNED UNSIGNEDINT UNSIGNEDCHAR UNSIGNEDSHORT UNSIGNEDLONG LONGINT DOUBLE 
%token SHORT  CHAR FLOAT INT 

%{
#include <stdio.h>
#include "common.h"
#include "btree.h"
#include "cctypes.h"
#include "gram.h"
#include "bool.h"
#define  YYSTYPE yystype
extern bool debugFlag;
/* the following are counters to produce the debuggers uniqe id numbers. */
unsigned spec_createId = 0;
unsigned spec_acceptId = 0;
unsigned spec_transId = 0;
unsigned spec_selectId = 0;
unsigned spec_terminateId = 0;
unsigned spec_delayId = 0;
unsigned spec_treturnId = 0;
%}

%type   <y_trap>	primary_expr primary_expr2 dot transaction_call
%type	<y_trap>	primary_expr2_h1
%type	<y_type>	declarator declarator2 abstract_declarator2_h1
%type	<y_type>	t_declarator t_declarator2 t_declarator3 t_declarator4
%type	<y_type>	abstract_declarator  opt_abstract_declarator
%type	<y_type>	abstract_declarator2 opt_abstract_declarator2
%type	<y_type>	postfix_expr conditional_expr timed_transaction_call
%type	<y_type>	unary_expr unary_expr_h1 cast_expr binary_expr
%type	<y_type>	assignment_expr_h1 expr_h1 create_expr type_name
%type	<y_type>	timed_transaction_call function_declarator
%type	<y_type>	shared_declarator shared_declarator2 opt_functor
%type	<y_type>	expr_abdec  opt_expr_abdec expr_abdec2 opt_expr_abdec2
%type	<y_type>	expr_type_name init_declarator_stub
%type	<y_int>		struct_or_union transaction_dot tagged_thing

%left OR_OP
%left AND_OP
%left '|'
%left '^'
%left '&'
%left EQ_OP
%left REL_OP
%left SHIFT_OP
%left '+' '-'
%left '*' '/' '%'

%start CC_file
%%

CC_file
	:	{printf("#include <ccc/CC_kernel.h>\n");
		if ( debugFlag )
			printf("#include <ccc/DD_kernel.h>\n");
		}
	  file
	;

process_body
	: T_process T_body T_identifier
		{$<y_type>$=lookup_tag(yytext,e_process);
		save(procname);
		printf("/* PROCESS BODY %s */\n",procname);
		procnum=contextof($<y_type>$);
		proc_formal_names = capture_new();
		spec_bodyStart( procname, NS_COMPLEX );
		argnum=0;
		spec_createId = 0;
		spec_acceptId = 0;
		spec_transId = 0;
		spec_selectId = 0;
		spec_terminateId = 0;
		spec_delayId = 0;
		spec_treturnId = 0;
		}
	  T_left_paren opt_param_list 
		{int realargnum;
		realargnum=contextof(internal_lookup("0",procnum));
		if ( argnum != realargnum )
			yyerror("parameters don't agree with spec");
		}
	  T_right_paren T_left_brace
		{
		printf("CC_pid CC_%s(%s ",procname, capture_string(proc_formal_names));
		capture_free(proc_formal_names);
		printf("CC_priority, DD_createId)\n");
		linesynch();
		declare_parameters();
		printf("int CC_priority;\nunsigned DD_createId;\n{\n");
		linesynch();
		stattype=PROCSTAT;
		enterblock();
		declare_var("c_mypid",NS_VAR,func($<y_type>4));
		}
	  opt_declaration_list
		{
		declare_debug_copy(procname);
		printf("\nif (CC_fork(\"%s\",CC_%s_numtrans,CC_priority,DD_createId))\n",procname,procname);
		printf("    return CC_childpid;\n");
		if ( debugFlag )
			printf("DD_processTable = &DD_processTable_%s;\n", procname);
		forcelinesynch();
		}
	  opt_statement_list T_right_brace
		{exitblock();
		procnum= -1;
		printf("CC_complete();\n}\n");
		spec_bodyEnd();
		}
	;

opt_param_list
	:
		{null_parameter_list();}
	| param_list
		{capture_addch(',', proc_formal_names);}
	;

param_list
	: T_identifier
		{capture_addstr(yytext, proc_formal_names);
		remember_first_parameter_name();
		spec_addFormal( yytext );
		argnum++;
		}
	| param_list T_comma T_identifier
		{capture_addch(',', proc_formal_names);
		capture_addstr(yytext, proc_formal_names);
		remember_another_parameter_name();
		spec_addFormal( yytext );
		argnum++;
		}
	;

/*
** PROCESS SPEC
**
** process spec FOO ( TYPE, ... )
** ---------------- -------------
**  head	    tail
*/

process_spec
	: process_spec_head process_spec_tail
	;

process_spec_head
	: T_process T_spec T_identifier
		{save(procname);
		 printf("/* PROCESS SPEC %s */\n",procname);
		}
	;

process_spec_tail
	:  T_is_new T_identifier
		{t_type type;
		 type=lookup_tag(yytext,e_process);
		 declare_var(procname,NS_COMPLEX,type); 
		 spec_start(procname, NS_COMPLEX);
		 printf("\n#define CC_%s_numtrans CC_%s_numtrans\n",procname,yytext);
		 forcelinesynch();
		}
	  T_semi
	| T_left_paren	/* begin parsing parameter types list */
		{proccontext=declare_tag(procname,e_process); 
		 spec_start(procname, NS_COMPLEX);
		 capture_pass_through(FALSE);
		 argnum=1;
		 transid=0;
		}
	  opt_proc_type_list T_right_paren
		{static t_type numtype={0,{0,0}};
		 capture_pass_through(TRUE);
		 numtype.typespec.num= argnum-1;
		 declare_var("0", proccontext, numtype);
		 printf("CC_pid CC_%s();\n",procname);
		}
	  trans_decs
		{printf("#define CC_%s_numtrans %d\n",procname,transid);
		 proccontext=0;
		 forcelinesynch();
		 spec_end();
		}
		
	;

opt_proc_type_list
	:
	| proc_type_list
	;

proc_type_list
	: proc_type_name
	| proc_type_list T_comma proc_type_name
	;

proc_type_name
	:	{proc_type_name = capture_new();}
	type_name
		{declare_var(itoa(argnum++),proccontext,$2);
		spec_addParam(capture_string(proc_type_name));
		capture_free(proc_type_name);
		}
	;

/*
** TRANSACTIONS DECLARATIONS
**
**  { transactions_declaration_list } ;
*/

trans_decs
	: T_left_brace trans_decl_list T_right_brace T_semi
	| T_semi
	;

trans_decl_list
	: trans_decl
	| trans_decl_list trans_decl
		{yyerrok;}
	| error
	| trans_decl_list error
	;

trans_decl
	: T_trans
		{printf("typedef ");}
	  trans_return_type_specifier
		{printf("CC_headvalue%d;\n",++thead);
		linesynch();
		}
	  trans_decl_tail
	;

trans_decl_tail
	: trans_declarator T_comma trans_decl_tail
	| trans_declarator T_semi 
	;

trans_declarator
	:	{$<y_type>$=type;
		printf("typedef struct {\nCC_headvalue%d ",thead);
		linesynch();
		}
	  t_declarator
		{PSN;
		transnum=newtag();
		declare_var("=",transnum,$2);		
		$2.typespec.num=transnum;
		declare_var(transname,proccontext,$2);
		spec_addTransSpec( transname, proccontext );
		strcpy(argname,"arg%d");
		argnum=1;
		}
	  T_left_paren trans_type_list T_right_paren
		{static t_type numtype={0,{0,0}};
		numtype.typespec.num= argnum-1;
		printf("} CC_%d_%s;\n",proccontext,transname);
		printf("#define CC_%d_%s_id %d\n",proccontext,transname,transid++);
		declare_var("0",transnum,numtype);		
		*argname='\0';
		type=$<y_type>1;
		linesynch();
		}
	  ;

trans_type_list
	: nonempty_type_list
	|
	;

nonempty_type_list
	: nonempty_type_list T_comma type_name
		{declare_var(itoa(argnum++),transnum,$3);
		PSN;
		linesynch();
		}
	| type_name
		{declare_var(itoa(argnum++),transnum,$1);
		PSN;
		linesynch();
		}
	;

t_declarator
	: P.star t_declarator
		{$$=ref($2);}
	| t_declarator2
		{$$=$1;}
	;

t_declarator2
	: T_identifier
		{save(transname);
		printf("value");
		$$=type;
		}
	| P.lparen t_declarator3 P.rparen
		{$$=$2;}
	| t_declarator2 P.lbrack opt_constant_expr P.rbrack
		{$$=ref($1);}
	;

t_declarator3
	: P.star t_declarator3
		{$$=ref($2);}
	| t_declarator4
		{$$=$1;}
	;

t_declarator4
	: T_identifier
		{save(transname);
		printf("value");
		$$=type;
		}
	| P.lparen t_declarator3 P.rparen
		{$$=$2;}
	| t_declarator4 P.lbrack opt_constant_expr P.rbrack
		{$$=ref($1);}
	| t_declarator4 P.lparen opt_identifier_list P.rparen
		{$$=func($1);}
	;

opt_expr
	:
	| expr
	;

declaration
	: unshared_declaration
	| typedef_declaration
	| shared_declaration
	;

unshared_declaration
	: declaration_specifiers P.semi
	| declaration_specifiers init_declarator_list P.semi
	;

typedef_declaration
	: T_typedef
		{printf("%s ",yytext);}
	  type_specifier typedef_declarator_list P.semi
	;

typedef_declarator_list
	: typedef_init_declarator
	| typedef_declarator_list P.comma typedef_init_declarator
		{yyerrok;}
	| error
	| typedef_declarator_list error
	| typedef_declarator_list error typedef_init_declarator
		{yyerrok;}
	| typedef_declarator_list P.comma error
	;

typedef_init_declarator
	: declarator
		{declare_typedef(idname,$1);
		linesynch();
		}
	;

ssc
	: SSC
		{printf("%s ",yytext);}
	;

declaration_specifiers
	: ssc type_specifier
	| ssc
		{type=Int;}
	| type_specifier
	;

init_declarator_list
	: init_declarator
	| init_declarator_list P.comma init_declarator
		{yyerrok;}
	| error
	| init_declarator_list error
	| init_declarator_list error init_declarator
		{yyerrok;}
	| init_declarator_list P.comma error
	;

init_declarator
	: init_declarator_stub P.equals initializer
	| init_declarator_stub
	;

init_declarator_stub
	: declarator
		{declare_var(idname,NS_VAR,$1);
		linesynch();
		$$=$1;
		}
	;

trans_return_type_specifier
	:	{printf("int "); type=Int;}
	| type_specifier
	;

type_specifier
	: struct_or_union_specifier
	| process_type_specifier
	| enum_specifier
	| T_type_name	{capture_printf("%s ",yytext); type=$<y_type>1;}
	| T_btype	{capture_printf("%s ",yytext); type=$<y_type>1;}
	| T_void	{capture_printf("int "); type=Int;}
	;

process_type_specifier
	: T_process T_identifier
		{capture_printf("CC_pid ");
		if (proccontext)
		    type=half_declare_tag(yytext,e_process);
		else
		    type=lookup_tag(yytext,e_process);
		}
	;

struct_or_union_specifier
	: struct_or_union
		{/* unnamed new structure (or union) */
		$<y_type>$=Basic;	/* just a blank for filling */
		$<y_type>$.typespec.obj=$1;
		$<y_type>$.typespec.num=context;
		context=declare_tag(0,$1);
		}
	  struct_or_union_declaration
		{/* restore context */
		type=$<y_type>2;
		type.typespec.num=context;
		context=$<y_type>2.typespec.num;
		}
	| struct_or_union T_identifier
		{capture_printf("%s ",yytext);
		save(idname);
		$<y_int>$=$1;
		}
	  struct_or_union_specifier_h1
	;

struct_or_union_specifier_h1
	:	{/* named new structure */
		$<y_type>$=Basic;		/* blank for filling */
		$<y_type>$.typespec.obj=$<y_int>0;
		$<y_type>$.typespec.num=context;
		context=declare_tag(idname,$<y_int>0);
		}
	  struct_or_union_declaration
		{/* restore context */
		type=$<y_type>1;
		type.typespec.num=context;
		context=$<y_type>1.typespec.num;
		}
	|	{/* non-defining occurence; mutual recursion or already declared */
		if (context)
		    type=half_declare_tag(idname,$<y_int>0);
		else
		    type=lookup_tag(idname,$<y_int>0);
		}
	;

struct_or_union_declaration
	: P.lbrace struct_declaration_list P.rbrace
	;

struct_or_union
	: T_struct
		{capture_printf("%s ",yytext);
		$$=e_struct;
		}
	| T_union
		{capture_printf("%s ",yytext);
		$$=e_union;
		}
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
		{yyerrok;}
	| error
	| struct_declaration_list error
	;

struct_declaration
	: type_specifier struct_declarator_list P.semi
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list P.comma struct_declarator
		{yyerrok;}
	| error
	| struct_declarator_list error
	| struct_declarator_list error struct_declarator
		{yyerrok;}
	| struct_declarator_list P.comma error
	;

struct_declarator
	: declarator
		{declare_var(idname,context,$1);
		 spec_addStructField(idname, context);
		}
			
	  opt_width_doodad
	| P.colon constant_expr
	;

opt_width_doodad
	:
	| P.colon constant_expr
	;

enum_specifier
	: T_enum
		{capture_printf("%s ",yytext);}
	  enum_specifier_h1
		{type=Int;}
	;

enum_specifier_h1
	: P.lbrace enumerator_list P.rbrace
	| T_identifier
		{capture_printf("%s ",yytext);}
	  P.lbrace enumerator_list P.rbrace
	| T_identifier
		{capture_printf("%s ",yytext);}
	;

enumerator_list
	: enumerator
	| enumerator_list P.comma enumerator
	;

enumerator
	: T_identifier
		{capture_printf("%s ",yytext);
		declare_var(yytext,NS_VAR,e_int);
		}
	  opt_enum_value_fixer
	;

opt_enum_value_fixer
	:
	| P.equals constant_expr /*%%% ENFORCE no trans in const expr! */
	;

declarator
	: P.star declarator
		{$$=ref($2);}
	| declarator2
		{$$=$1;}
	;

declarator2
	: T_identifier
		{capture_printf("%s ",yytext);
		save(idname);
		$$=type;
		}
	| P.lparen declarator P.rparen
		{$$=$2;}
	| declarator2 P.lbrack opt_constant_expr P.rbrack
		{$$=ref($1);}
	| declarator2 P.lparen opt_identifier_list P.rparen
		{$$=func($1);}
	;

shared_declaration
	: T_shared
		{extern int blocknum,localsharedp;
		if (blocknum)
		    localsharedp=1;
		capture_printf("typedef ");
		}
	  type_specifier
		{capture_printf(" CC_shtypedef%d;\nCC_shtypedef%d ",
		    shtypedefnum,shtypedefnum);
		linesynch();
		}
	  shared_init_declarator_list P.semi
		{shtypedefnum++;}
	;

shared_init_declarator_list
	: shared_init_declarator
	| shared_init_declarator_list P.comma shared_init_declarator
		{yyerrok;}
	| error
	| shared_init_declarator_list error
	| shared_init_declarator_list error shared_init_declarator
		{yyerrok;}
	| shared_init_declarator_list P.comma error
	;

shared_init_declarator
	:	{typetrap_i=0;
		boxnum=0;
		}
	  shared_declarator
		{declare(idname,NS_VAR,$2,(*alias ? alias : 0));
		typetrap[typetrap_i]='\0';
		switch(boxnum) {
		    int j;
		case 0:
		    fprintf(Fshared,"%s = (CC_shtypedef%d %s) ",
		        idname,shtypedefnum,typetrap);
		    fprintf(Fshared,"CC_shmalloc(sizeof(*%s));\n",idname);
		    break;
		case 1:
		    fprintf(Fshared,"%s = (CC_shtypedef%d %s) ",
		        idname,shtypedefnum,typetrap);
		    fprintf(Fshared,"CC_shmalloc((%s) * sizeof(*%s));\n",sizes[1],idname);
		    break;
		default:
		    fprintf(Fshared,"CC_n=(%s)",sizes[1]);
		    free(sizes[1]);
		    for ( j=2 ; j<boxnum ; j++ ) {
			fprintf(Fshared,"*(%s)",sizes[j]);
			free(sizes[j]);
		    }
		    fprintf(Fshared,";\nCC_ss = (%s) * sizeof(",sizes[boxnum]);
		    free(sizes[boxnum]);
		    for (j=1;j<=boxnum;j++)
			fputc('*',Fshared);
		    fprintf(Fshared,"%s);\n",idname);
		    fprintf(Fshared,"CC_s = CC_shmalloc(CC_n * CC_ss);\n");
		    fprintf(Fshared,"for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)\n(");
		    for (j=1;j<=boxnum-2;j++)
			fputc('*',Fshared);
		    fprintf(Fshared,"%s)[CC_j] = (CC_shtypedef%d %s) CC_s;\n",
		        idname,shtypedefnum,typetrap);
		    break;
		} /* end of switch */
		linesynch();
		}
	;

shared_declarator
	: F.star shared_declarator
		{$$=ref($2);}
	| shared_declarator2
		{$$=$1;}
	;

shared_declarator2
	: lookahead_identifier
		{sprintf(alias,"(*%s)",idname);
		capture_printf(alias);
		typetrap[typetrap_i++]='*';
		$$=type;
		}
	  opt_functor
		{$$=$3;}
	| lookahead_identifier T_left_bracket
		{*alias='\0';
		capture_printf("*%s",idname);
		typetrap[typetrap_i++]='*';
		}
	  boxes
		{int j=boxnum;
		$$=type;
		while (j--)
		    $$=ref($$);
		}
	| F.lparen shared_declarator F.rparen
		{$$=$2;}
	  opt_functor
		{$$=$5;}
	;

opt_functor
	:	{$$=$<y_type>0;}
	| F.lparen F.rparen
		{$$=func($<y_type>0);}
	;

boxes
	: box T_left_bracket
		{putchar('[');
		end_trapping;
		putchar(']');
		}
	  boxes
	| box
		{*tbuf=0;
		end_trapping;
		}
	;

box
	:
		{start_trapping;}
	  expr_h1 T_right_bracket
		{tbuf[tbuf_i]='\0';
		sizes[++boxnum]=newstr(tbuf);
		}
	;

F.star
	: T_star
		{putchar('*'); typetrap[typetrap_i++]='*';}
	;

F.lparen
	: T_left_paren
		{putchar('('); typetrap[typetrap_i++]='(';}
	;

F.rparen
	: T_right_paren
		{putchar(')'); typetrap[typetrap_i++]=')';}
	;

opt_constant_expr
	:
	| constant_expr
	;

opt_identifier_list
	:
	| identifier_list
	;

identifier_list
	: T_identifier
		{capture_printf("%s ",yytext);}
	| identifier_list P.comma T_identifier
		{capture_printf("%s ",yytext); yyerrok;}
	| error
	| identifier_list error
	| identifier_list error T_identifier
		{capture_printf("%s ",yytext); yyerrok;}
	| identifier_list P.comma error
	;

type_name
	: type_specifier opt_abstract_declarator
		{$$=$2;}
	;

abstract_declarator
	: P.star opt_abstract_declarator
		{$$=ref($2);}
	| abstract_declarator2
		{$$=$1;}
	;

opt_abstract_declarator
	: P.star opt_abstract_declarator
		{$$=ref($2);}
	| opt_abstract_declarator2
		{$$=$1;}
	;

opt_abstract_declarator2
	:	{printf(argname,argnum);
		$$=type;
		}
	| abstract_declarator2
		{$$=$1;}
	;

abstract_declarator2
	: opt_abstract_declarator2 P.lbrack opt_constant_expr P.rbrack
		{$$=ref($1);}
	| abstract_declarator2 P.lparen P.rparen
		{$$=func($1);}
	| P.lparen abstract_declarator2_h1
		{$$=$2;}
	;

abstract_declarator2_h1
	: T_right_paren
		{printf(argname,argnum);
		putchar(')');
		putchar('(');
		putchar(')');
		$$=func(type);
		}
	| abstract_declarator P.rparen
		{$$=$1;}
	;

initializer
	: assignment_expr
	| P.lbrace initializer_list initializer_h1
	;

initializer_h1
	: P.rbrace
	| P.comma P.rbrace
	;

initializer_list
	: initializer
	| initializer_list P.comma initializer
	;

statement
	: compound_statement
	| return_statement
	| labeled_statement
	| expression_statement
	| switch_statement
	| if_statement
	| iteration_statement
	| jump_statement
	| delay_statement
	| process_only_statement
		{if (stattype == NORMALSTAT)
		    yyerror("statement illegal outside process body");
		}
	| treturn_statement
		{if (stattype != ACCEPTSTAT)
		    yyerror("statement illegal outside accept statement");
		}
	;

process_only_statement
	: select_statement
	| terminate_statement
	| accept_statement
	;

compound_statement
	: P.lbrace compound_statement_h1 P.rbrace
	;

compound_statement_h1
	: opt_statement_list
	|	{enterblock();}
	  declaration_list opt_statement_list
		{exitblock();}
	;

opt_statement_list
	: 
	| opt_statement_list statement
	;

opt_declaration_list
	:
	| declaration_list
	;

declaration_list
	: declaration_list_h1
		{includeshared();}
	;

declaration_list_h1
	: declaration
	| declaration_list_h1 declaration
		{yyerrok;}
	| error
	| declaration error
	;

treturn_statement
	: T_treturn T_semi
		{capture_printf("CC_treturn(%d);\ngoto CC_treturn%d;\n", ++spec_treturnId, acceptnum);
		spec_addTreturn( acceptname, procnum, Linenum, spec_treturnId);
		}
	| T_treturn
		{capture_printf("{\n((CC_%d_%s *)CC_tblock%d)->value = ",
			procnum,acceptname,acceptnum);
		}
	  expr T_semi 
		{capture_printf(";\nCC_treturn(%d);\ngoto CC_treturn%d;\n}\n", ++spec_treturnId, acceptnum);
		spec_addTreturn( acceptname, procnum, Linenum, spec_treturnId);
		linesynch();
		}
	;

delay_statement
	: T_delay
	 	{capture_printf("CC_delay((double)(");
		spec_addDelay( Linenum, ++spec_delayId);
		}
			
	  expr T_semi
		{capture_printf("), %d);\n", spec_delayId);}
	;

select_statement
	: T_select T_left_brace
		{casenum=0; /* BUG: THIS WILL NOT WORK FOR NESTED T_selectS... */
		selectnum++;
		spec_addSelect( Linenum, ++spec_selectId );
		capture_printf("{\nCC_selectinit();\n");
		capture_printf("goto CC_guard%d_1; CC_choose%d: switch(CC_choose(%d)) {",
		    selectnum,selectnum, spec_selectId);
		forcelinesynch();
		}
	  alternative opt_ors opt_other T_right_brace
		{capture_printf("\n}\ngoto CC_endselect%d;\nCC_guard%d_%d: ",
		    selectnum,selectnum,casenum+1);
		capture_printf("goto CC_choose%d;\nCC_endselect%d:;\n}\n",
		    selectnum,selectnum);
		forcelinesynch();
		}
	;

alternative
	:
		{capture_printf("CC_guard%d_%d :",selectnum,++casenum);
		linesynch();
		}
	  opt_guard alternative_stat
		{capture_printf("break; ");
		linesynch();
		}
	;

opt_guard
	: T_left_paren
		{capture_printf("if ((");}
	  expr T_right_paren T_colon
		{capture_printf(") && ");}
	|	{capture_printf("if (");}
	;

alternative_stat
	: T_terminate T_semi
		{capture_printf("1) CC_terminate_exists=1; goto CC_guard%d_%d;\n",
		    selectnum,casenum+1);
		spec_addSelectTerminate( Linenum );
		forcelinesynch();
		}
	| T_immediate
		{
		capture_printf("!CC_ichoice) CC_ichoice=%d; goto CC_guard%d_%d;\ncase %d:",
		    casenum,selectnum,casenum+1,casenum);
		spec_addSelectImmediate( Linenum );
		forcelinesynch();
		}
	  statement opt_statement_list
	| T_delay
		{capture_printf("(CC_trydelay=(");}
	  expr T_semi
		{capture_printf("))>=0)\nif (!CC_dchoice || CC_bestdelay>CC_trydelay) {\n");
		capture_printf("CC_dchoice=%d;\nCC_bestdelay=CC_trydelay;\n}\n",casenum);
		capture_printf("goto CC_guard%d_%d;\ncase %d:",selectnum,casenum+1,casenum);
		spec_addDelay( Linenum, ++spec_delayId);     /*** nowhere for
								 notification */
		forcelinesynch();
		}
	  opt_statement_list
	| accept_head
		{capture_printf("(CC_accept_exists=1,CC_tryaccept(CC_%d_%s_id,%d)))\n",
		    procnum,acceptname,casenum);
		capture_printf("goto CC_choose%d;\n",selectnum);
		capture_printf("goto CC_guard%d_%d;\ncase %d:",selectnum,casenum+1,casenum);
		accept_in_select=1;
		forcelinesynch();
		}
	  accept_tail opt_statement_list
	;

opt_ors
	:
	| T_or alternative opt_ors
	;

opt_other
	: T_otherwise
		{capture_printf("CC_guard%d_%d :",selectnum,++casenum);
		capture_printf("CC_ochoice=%d; goto CC_guard%d_%d;\ncase %d:",
		    casenum,selectnum,casenum+1,casenum);
		forcelinesynch();
		}
	  statement opt_statement_list
		{capture_printf("break; ");
		forcelinesynch();
		}
	|
	;

terminate_statement
	: T_terminate T_semi
		{capture_printf("c_abort(CC_mypid);\n");
		spec_addTerminate( Linenum, ++spec_terminateId ); 
		linesynch();
		}
	;

accept_statement
	: accept_head accept_tail
	;

accept_head
	: T_accept T_identifier
		{t_type type;
		save(acceptname);
		type=lookup_var(acceptname,procnum,
		    "%s is not a transaction of this process",acceptname);
		transnum=contextof(type);
		accept_in_select=0;
		}
	;

accept_tail:
		{enterblock();
		$<y_int>$=acceptnum;
		acceptnum=acceptnumgenerator++;
		capture_printf("{\nCC_tblock CC_tblock%d;\n",acceptnum);
		argnum=0;
		maxargnum=contextof(internal_lookup("0",transnum));
		spec_addAccept( acceptname, procnum, Linenum, ++spec_acceptId );
		linesynch();
		}
	  T_left_paren opt_accept_identifier_list T_right_paren
		{if (argnum<maxargnum)
		    yyerror("too few parameters in accept statement");
		$<y_int>$=stattype;
		stattype=ACCEPTSTAT;
		forcelinesynch();
		}
	  opt_by opt_compound_statement
		{exitblock();
		spec_addTreturn( acceptname, procnum, Linenum, ++spec_treturnId);
		capture_printf("CC_warn%d:CC_treturn(%d);\nCC_treturn%d:;}\n", spec_treturnId, spec_treturnId, acceptnum);
		acceptnum=$<y_int>1;
		stattype=$<y_int>5;
		forcelinesynch();
		}
	;

opt_accept_identifier_list
	:
	| accept_identifier_list
	;

accept_identifier_list
	: accept_identifier
	| accept_identifier_list T_comma accept_identifier
	;

accept_identifier
	: T_identifier
		{sprintf(alias,"((CC_%d_%s *)CC_tblock%d)->arg%d",
			procnum,acceptname,acceptnum,++argnum);
		if (argnum <= maxargnum) {
		    declare(yytext,NS_VAR,internal_lookup(itoa(argnum),transnum),
			alias);
		    spec_addAcceptParam( yytext );
		} else if (argnum == maxargnum+1)
		    yyerror("too many parameters in accept statement");
		linesynch();
		}
	;

opt_by
	:	{capture_printf("CC_accept(CC_%d_%s_id,&CC_tblock%d,0,%d, %d);\n",
		      procnum,acceptname,acceptnum,accept_in_select, spec_acceptId);
		linesynch();
		}
	| T_by	{capture_printf("CC_accept(CC_%d_%s_id,&CC_tblock%d,1,%d, %d);\n",
		      procnum,acceptname,acceptnum,accept_in_select, spec_acceptId);
		capture_printf("while(CC_acceptby((double)(");
		linesynch();
		}
	  expr
		{capture_printf(")));\n");
		linesynch();
		}
	;

opt_compound_statement
	: T_semi
	| compound_statement
	;

labeled_statement
	: T_label
		{capture_printf("%s ",yytext);}
	  the_labeled_statement
	| T_default
		{capture_printf("%s ",yytext);}
	  the_labeled_statement
	| T_case
		{capture_printf("%s ",yytext);}
	  constant_expr the_labeled_statement
	;

the_labeled_statement
	: P.colon statement
	;

expression_statement
	: opt_expr P.semi
	;

switch_statement
	: T_switch {capture_printf("%s ",yytext);} P.lparen expr P.rparen statement
	;

if_statement
	: T_if {capture_printf("%s ",yytext);} P.lparen expr P.rparen statement else_statement
	;

else_statement
	:
	| T_else {capture_printf("%s ",yytext);} statement
	;

iteration_statement
	: while_part statement
	| T_do {capture_printf("%s ",yytext);} statement while_part
	| T_for {capture_printf("%s ",yytext);} P.lparen
	  opt_expr  P.semi
	  opt_expr  P.semi
	  opt_expr  P.rparen
	;

while_part
	: T_while {capture_printf("%s ",yytext);} P.lparen expr P.rparen
	;

jump_statement
	: T_goto {capture_printf("%s ",yytext);} T_identifier {capture_printf("%s ",yytext);} P.semi
	| T_continue {capture_printf("%s ",yytext);} P.semi
	| T_break {capture_printf("%s ",yytext);} P.semi
	;

return_statement
	: T_return
		{if (stattype==NORMALSTAT)
		    capture_printf("%s ",yytext);
		}
	  return_opt_expr T_semi
		{switch (stattype) {
		case NORMALSTAT:
		    PSN;
		    break;
		case ACCEPTSTAT:
		    yyerror("return not allowed in accept statement body");
		    break;
		case PROCSTAT:
		    puts("CC_complete();");
		    break;
		}
		linesynch();
		}
	;

return_opt_expr
	:
	| expr
		{if (stattype != NORMALSTAT) {
		    yyerror("illegally supplied return value in process body");
		    PSN;
		    }
		}
	;

file
	:	{type=Basic;}
	  external_definition
	| file
		{type=Basic;}
	  external_definition
		{yyerrok;}
	| error
	| file error
	;

external_definition
	: function_definition
	| declaration
	| process_body
	| process_spec
	;

function_definition
	: function_declarator
		{stattype=NORMALSTAT;
		}
			
	  function_body
	;

function_declarator
	: declaration_specifiers init_declarator_stub
		{$$=$2;}
	| init_declarator_stub
		{$$=$1;}
	;	

function_body
	: compound_statement
	|	{enterblock();}
	  declaration_list compound_statement
		{exitblock();}
	;

lookahead_identifier
	: T_identifier
		{strcpy(idname,yytext);
		if (cctext==yytext)
		    cctext=idname;
		}
	;

P.lbrack
	: T_left_bracket {putchar('[');}
	;
P.rbrack
	: T_right_bracket {putchar(']');}
	;
P.lparen
	: T_left_paren {putchar('(');}
	;
P.rparen
	: T_right_paren {putchar(')');}
	;
P.star
	: T_star {putchar('*');}
	;
P.comma
	: T_comma {putchar(',');}
	;
P.equals
	: T_equal {putchar('=');}
	;
P.lbrace
	: T_left_brace {putchar('{');}
	;
P.colon
	: T_colon {putchar(':');}
	;
P.rbrace
	: T_right_brace {putchar('}');}
	;
P.semi
	: T_semi {putchar(';');}
	;
expr
	: {start_trapping;} expr_h1 {end_trapping;}
	;

assignment_expr
	: {start_trapping;} assignment_expr_h1 {end_trapping;}
	;

constant_expr
	: {start_trapping;} conditional_expr {end_trapping;}
	;

  /*******************************************************************/
 /* the following, and only the following, participate in trapping. */
/*******************************************************************/

create_expr
	: T_create T_identifier
		{$<y_type>$=lookup_tag(yytext,e_process);
		pri((ntf,"CC_%s",yytext));
		TP('(');
		spec_addCreate( yytext, $<y_type>$.typespec.num, Linenum, ++spec_createId );
		}
	  T_left_paren create_opt_argument_expr_list T_right_paren opt_with_clause
		{/*** put something in here about createId ***/
		pri((ntf,", %d", spec_createId));
		TP(')');
		$$=$<y_type>3;
		}
	;

opt_with_clause
	:
		{TP('0');}
	| T_with T_priority T_left_paren
		{TP('(');}
	  expr_h1 T_right_paren
		{TP(')');}
	;

create_opt_argument_expr_list
	:
	| argument_expr_list {TP(',');}
	;

primary_expr
	: primary_expr2
		{$$=$1;}
	| dot
		{TP('.');
		pri((ntf,"%s ",dotname));
		$$.a=$1.a;
		$$.z=tbuf_i;
		$$.t=$1.t;
		}
	;

dot
	: primary_expr T_dot T_identifier
		{save(dotname);
		callprocnum=contextof($1.t);
		$$.t=lookup_var(dotname,callprocnum,
		    "nonexistent field or transaction %s",dotname);
		$$.a=$1.a;
		$$.z=$1.z;
		}
	;

primary_expr2
	: lookahead_identifier
		{$$.a=tbuf_i;
		pri((ntf,cctext));
		$$.z=tbuf_i;
		$$.t=lookup_last(idname);
		}
	| primary_expr2_h1
		{$$=$1;}
	;

primary_expr2_h1
	: T_constant
		{$$.a=tbuf_i;
		TPY;
		$$.z=tbuf_i;
		$$.t=Basic;
		}
	| T_string_literal
		{$$.a=tbuf_i;
		TPY;
		$$.z=tbuf_i;
		$$.t=ref(Basic);
		}
	| T_left_paren
		{$$.a=tbuf_i;
		TP('(');
		}
	  expr_h1 T_right_paren
		{TP(')');
		$$.z=tbuf_i;
		$$.t=$3;
		}
	| primary_expr T_left_bracket
		{TP('[');}
	  expr_h1 T_right_bracket
		{TP(']');
		$$.a=$1.a;
		$$.z=tbuf_i;
		$$.t=array($1.t,$4);
		}
	| primary_expr PTR_OP T_identifier
		{pri((ntf,"->%s",yytext));
		$$.t=lookup_var(yytext,contextof(deref($1.t)),
		    "nonexistent field or transaction %s",yytext);
		$$.a=$1.a;
		$$.z=tbuf_i;
		}
	| primary_expr2_h1 T_left_paren
		{TP('(');}
	  opt_argument_expr_list T_right_paren
		{TP(')');
		$$.a=$1.a;
		$$.z=tbuf_i;
		$$.t=defunc($1.t);
		}
	| lookahead_identifier
		{$<y_trap>$.t=half_declare_func(idname);
		$<y_trap>$.a=tbuf_i;
		pri((ntf,cctext));
		TP('(');
		}
	  T_left_paren opt_argument_expr_list T_right_paren
		{TP(')');
		$$.a=$<y_trap>2.a;
		$$.z=tbuf_i;
		$$.t=defunc($<y_trap>2.t);
		}
	| T_c_count T_left_paren
		{$<y_int>$=tbuf_i;
		pri((ntf,"c_count("));
		}
	  assignment_expr_h1 T_comma T_identifier
		{$<y_int>$=contextof($4);
		pri((ntf,"CC_%d_%s_id)",$<y_int>$,yytext));
		lookup_var(yytext,$<y_int>$,"nonexistent transaction %s",yytext);
		}
	  T_right_paren
		{$$.a=$<y_int>3;
		$$.z=tbuf_i;
		$$.t=Int;
		}
	| transaction_call
		{$$=$1;}
	;

opt_argument_expr_list
	:
	| argument_expr_list
	;

argument_expr_list
	: assignment_expr_h1
	| argument_expr_list T_comma {TP(',');} assignment_expr_h1
	;

postfix_expr
	: primary_expr
		{$$=$1.t;}
	| primary_expr INC_OP
		{TPY;
		$$=inc($1.t);
		}
	;

unary_expr
	: postfix_expr
		{$$=$1;}
	| T_inc_op
		{TPY;}
	  unary_expr
		{$$=inc($3);}
	| T_star
		{TP('*');}
	  cast_expr
		{$$=deref($3);}
	| T_amper
		{TP('&');}
	  cast_expr
		{$$=addr($3);}
	| unary_op
		{TP(' ');TPY;}
	  cast_expr
		{$$=$3;}
	| T_sizeof
		{TPY;TP(' ');}
	  unary_expr_h1
		{$$=Int;}
	| create_expr
		{$$=$1;}
	;

unary_expr_h1
	: unary_expr
	| T_left_paren
		{$<y_int>$=argnum;
		argnum=0;
		TP('(');
		}
	  expr_type_name T_right_paren
		{argnum=$<y_int>2;
		TP(')');
		}
	;

unary_op
	: T_bang
	| T_tilde
	| T_plus
	| T_minus
	;

cast_expr
	: unary_expr
		{$$=$1;}
	| T_left_paren
		{$<y_int>$=argnum;
		argnum=0;
		TP('(');
		}
	  expr_type_name T_right_paren
		{argnum=$<y_int>2;
		TP(')');
		}
	  cast_expr
		{$$=$3;
		}
	;

binary_expr
	: cast_expr
		{$$=$1;}
	| binary_expr T_plus
		{TP('+');}
	  binary_expr
		{$$=add($1,$4);}
	| binary_expr T_minus
		{TP('-');}
	  binary_expr
		{$$=subtract($1,$4);}
	| binary_expr T_or_op	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_and_op	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_or	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_hat	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_amper	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_shift_op{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_star	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_slash	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_percent	{TPY;} binary_expr {$$=arith($1,$4);}
	| binary_expr T_eq_op	{TPY;} binary_expr {$$=compare($1,$4);}
	| binary_expr T_rel_op	{TPY;} binary_expr {$$=compare($1,$4);}
	;

conditional_expr
	: binary_expr
		{$$=$1;}
	| binary_expr T_question
		{TP('?');}
	  binary_expr T_colon
		{TP(':');}
	  conditional_expr
		{$$=condition($4,$7,"type clash in conditional statement");}
	| timed_transaction_call
		{$$=$1;}
	;

transaction_call
	: transaction_dot /* BUG: T_transACTION CALLS WILL NOT NEST */
		{if (argnum < maxargnum)
		    yyerror("too few parameters in transaction call");
		spec_addTrans( transname, callprocnum, Linenum, ++spec_transId );
		pri((ntf,"CC_tcall(%d),\n", spec_transId));
		pri((ntf,"((CC_%d_%s *)CC_mytblock)->value)",
		    callprocnum,transname));
		$$.a=$1;
		$$.z=tbuf_i;
		$$.t=internal_lookup("=",transnum);	/* return type */
		}
	;

transaction_dot
	: dot
		{/* nesting will not work: look at transnum, istimed, &c. */
		strcpy(transname,dotname);
		transnum=contextof($1.t);
		argnum=0;
		maxargnum=contextof(internal_lookup("0",transnum));
		if (istimed)
		    tinsert("CC_tcallinit(",$1.a);
		else
		    tinsert("(CC_tcallinit(",$1.a);
		pri((ntf,",CC_%d_%s_id),\n",callprocnum,transname));
		}
	  T_left_paren opt_tcall_argument_expr_list T_right_paren
		{$$=$1.a;}
	;

opt_tcall_argument_expr_list
	:
	| tcall_argument_expr_list
	;

tcall_argument_expr_list
	: tcall_assignment_expr
	| tcall_argument_expr_list T_comma tcall_assignment_expr
	;

tcall_assignment_expr
	:	{argnum++;
		if (argnum > maxargnum)
		    yyerror("too many parameters in transaction call");
		pri((ntf,"((CC_%d_%s *)CC_mytblock)->arg%d=",
		    callprocnum,transname,argnum));
		$<y_type>$=internal_lookup(itoa(argnum),transnum);
		}
	  assignment_expr_h1
		{TP(',');
		TP('\n');
		assign($<y_type>1,$2,"type clash in transaction call");
		}
	;

timed_transaction_call
	: T_within
		{pri((ntf,"(CC_remember_time("));}
	  binary_expr T_question
		{pri((ntf,"),\n"));
		istimed=1;
		}
	  transaction_dot T_colon
		{istimed=0;
		if (argnum < maxargnum)
		    yyerror("too few parameters in transaction call");
		spec_addTrans( transname, callprocnum, Linenum, ++spec_transId );
		pri((ntf,"(CC_timedtcall(%d) ?\n", spec_transId));
		pri((ntf,"((CC_%d_%s *)CC_mytblock)->value :\n(",
		    callprocnum,transname));
		}
	  conditional_expr
		{pri((ntf,")))"));
		$$=condition($9,internal_lookup("=",transnum),
		    "type clash in timed transaction call alternative");
		}
	;

assignment_expr_h1
	: conditional_expr
		{$$=$1;}
	| unary_expr T_equal
		{TPY;}
	  assignment_expr_h1
		{$$=assign($1,$4,"type clash in assignment statement");}
	| unary_expr T_x_assign
		{TPY;}
	  assignment_expr_h1
		{$$=arithassign($1,$4);}
	| unary_expr T_a_assign
		{TPY;}
	  assignment_expr_h1
		{$$=addassign($1,$4);}
	;

expr_h1
	: ignoreable_commas assignment_expr_h1
		{$$=$2;}
	;

ignoreable_commas
	:
	| ignoreable_commas assignment_expr_h1 T_comma
		{TP(',');
		TP('\n');
		}
	;

expr_type_name
	: expr_type_specifier opt_expr_abdec
		{$$=$2;}
	;

expr_type_specifier
	: tagged_thing T_identifier
		{if ($1 != e_process)
		    TPY;
		if ($1 == e_int)
		    type=Int;
		else
		    type=lookup_tag(yytext,$1);
		}
	| T_type_name	{TPY; type=$<y_type>1;}
	| T_btype		{TPY; type=$<y_type>1;}
	| T_void		{pri((ntf,"int ")); type=Int;}
	;

tagged_thing
	: T_struct
		{TPY;
		$$=e_struct;
		}
	| T_union
		{TPY;
		$$=e_union;
		}
	| T_process
		{pri((ntf,"CC_pid "));
		$$=e_process;
		}
	| T_enum
		{TPY;
		$$=e_int;
		}
	;

opt_expr_abdec
	:	{$$=type;}
	| expr_abdec
		{$$=$1;}
	;

opt_expr_abdec2
	:	{$$=type;}
	| expr_abdec2
		{$$=$1;}
	;

expr_abdec
	: T_star
		{TP('*');}
	  opt_expr_abdec
		{$$=ref($3);}
	| expr_abdec2
		{$$=$1;}
	;

expr_abdec2
	: T_left_paren T_right_paren
		{$$=func(type);
		TP('(');
		TP(')');
		}
	| expr_abdec2 T_left_paren T_right_paren
		{$$=func($1);
		TP('(');
		TP(')');
		}
	| opt_expr_abdec2 T_left_bracket
		{TP('[');}
	  expr_abdec2_h1
		{TP(']');
		$$=ref($1);
		}
	| T_left_paren
		{TP('(');}
	  expr_abdec T_right_paren
		{TP(')');
		$$=$3;
		}
	;

expr_abdec2_h1
	: conditional_expr T_right_bracket
	| T_right_bracket
	;

T_a_assign:	A_ASSIGN
T_accept:	ACCEPT
T_and_op:	AND_OP
T_body:		BODY
T_break:	BREAK
T_btype:
	BTYPE		{$<y_type>$ = Basic;}
	| UNSIGNED	{$<y_type>$ = Unsigned;}
	| UNSIGNEDINT	{$<y_type>$ = Unsignedint;}
	| UNSIGNEDCHAR	{$<y_type>$ = Unsignedchar;}
	| UNSIGNEDSHORT	{$<y_type>$ = Unsignedshort;}
	| UNSIGNEDLONG	{$<y_type>$ = Unsignedlong;}
	| LONGINT	{$<y_type>$ = Longint;}
	| DOUBLE	{$<y_type>$ = Double;}
	| SHORT		{$<y_type>$ = Short;}
	| CHAR		{$<y_type>$ = Char;}
	| FLOAT		{$<y_type>$ = Float;}
	| INT		{$<y_type>$ = Int;}
	;
T_by:		BY
T_c_count:	C_COUNT
T_case:		CASE
T_constant:	CONSTANT
T_continue:	CONTINUE
T_create:	CREATE
T_default:	DEFAULT
T_delay:	DELAY
T_do:		DO
T_else:		ELSE
T_enum:		ENUM
T_eq_op:	EQ_OP
T_for:		FOR
T_goto:		GOTO
T_identifier:	IDENTIFIER
T_if:		IF
T_immediate:	IMMEDIATE
T_inc_op:	INC_OP
T_is_new:	IS_NEW
T_label:	LABEL
T_or:		OR
T_or_op:	OR_OP
T_otherwise:	OTHERWISE
T_priority:	PRIORITY
T_process:	PROCESS
T_rel_op:	REL_OP
T_return:	RETURN
T_select:	SELECT
T_shared:	SHARED
T_shift_op:	SHIFT_OP
T_sizeof:	SIZEOF
T_spec:		SPEC
T_string_literal:	STRING_LITERAL
T_struct:	STRUCT
T_switch:	SWITCH
T_terminate:	TERMINATE
T_trans:	TRANS
T_treturn:	TRETURN
T_type_name:	TYPE_NAME
T_typedef:	TYPEDEF
T_union:	UNION
T_void:		VOID
T_while:	WHILE
T_with:		WITH
T_within:	WITHIN
T_x_assign:	X_ASSIGN
T_colon:	':'
T_equal:	'='
T_amper:	'&'
T_bang:		'!'
T_tilde:	'~'
T_plus:		'+'
T_minus:	'-'
T_star:		'*'
T_or:		'|'
T_hat:		'^'
T_slash:	'/'
T_percent:	'%'
T_question:	'?'
T_left_paren:	'('
T_right_paren:	')'
T_left_bracket:	'['
T_right_bracket:']'
T_left_brace:	'{'
T_right_brace:	'}'
T_comma:	','
T_dot:		'.'
T_semi:		';'
%%
