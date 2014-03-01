/*-------------------------------------------------------------------------*/
/* TYPE CHASING FUNCTIONS */

#include <stdio.h>
#include "common.h"
#include "cctypes.h"

#define BTORDER 12

#define B_ERROR		1

#define b_index(a)	(a&31)
#define b_full(a)	(b_index(a)==31)
#define b_curbit(a)	(1<<b_index(a))
#define b_isptr(a)	(a>4 && ~a&b_curbit(a))
#define b_isfunc(a)	(a>4 && a&b_curbit(a))
#define b_addr(a)	(a?(a++,a&=~b_curbit(a)):(a=5))
#define b_ref(a)	(a?(a++,a+=a&~31):(a=5))
#define b_func(a)	(a?(a++,a+=a&~31,a|=32):(a=37))
#define b_de(a)		(a&=~(1<<(a&31)),a--,a=(b_index(a)>4?a:0))

isptr(x)
t_type x; 
{
	return b_isptr(x.chain);
}
isfunc(x)
t_type x; 
{
	return b_isfunc(x.chain);
}
iserror(x)
t_type x; 
{
	return x.chain==B_ERROR;
}

char *tagtypetext[4]={
	"???",
	"struct",
	"union",
	"process"
};

int NameSpace=NS_BADTAG+1; /* counter for generating new namespaces */

/* global variables for yacc use */
int map=0;
int blocknum;
char *cctext;

t_type b_error = {
	B_ERROR,{NS_BADTAG,0}
};
extern t_type Basic;

t_type addr(x)
t_type x;
{
	if (b_full(x.chain)) {
		yyerror("type too complex");
		return b_error;
	}
	if (iserror(x))
		return b_error;
	b_addr(x.chain);
	return x;
}

t_type ref(x)
t_type x;
{
	if (b_full(x.chain)) {
		yyerror("type too complex");
		return b_error;
	}
	if (iserror(x))
		return b_error;
	b_ref(x.chain);
	return x;
}

t_type func(x)
t_type x;
{
	if (b_full(x.chain)) {
		yyerror("declaration too complex");
		return b_error;
	}
	if (iserror(x))
		return b_error;
	b_func(x.chain);
	return x;
}

t_type deref(x)
t_type x;
{
	if (!isptr(x) && !iserror(x)) {
		yyerror("object is not a pointer");
		return b_error;
	}
	if (!iserror(x))
		b_de(x.chain);
	return x;
}

t_type defunc(x)
t_type x;
{
	if (!isfunc(x) && !iserror(x)) {
		yyerror("object is not a function");
		return b_error;
	}
	if (!iserror(x))
		b_de(x.chain);
	return x;
}

#define isbare(t)	(!t.chain)
#define isbasic2(t,tt)	(isbasic(t) && isbasic(tt))
#define isptr2(t,tt)	(isptr(t) && isptr(tt))

iserror2(t,tt)
t_type t,tt;
{
	return iserror(t) || iserror(tt);
}

int contextof(t)
t_type t;
{
	if (t.chain)
		return NS_BADTAG;
	else
		return t.typespec.num;
}

isprocess(t)
t_type t;
{
	return (isbare(t) && t.typespec.obj==e_process);
}

isbasic(t)
t_type t;
{
	return(isbare(t) && (t.typespec.obj==e_basic 
				|| t.typespec.obj >= e_unsigned));
}

char aritherr[]="type clash in arithmetic expression";

t_type inc(t)
t_type t;
{
	if (isptr(t) || isbasic(t) || iserror(t))
		return t;
	yyerror(aritherr);
	return b_error;
}

t_type add(t,tt)
t_type t,tt;
{
	if ((isptr(t) || isbasic(t)) && isbasic(tt))
		return t;
	if (isbasic(t) && isptr(tt))
		return tt;
	if (!iserror2(t,tt))
		yyerror(aritherr);
	return b_error;
}

t_type subtract(t,tt)
t_type t,tt;
{
	if((isptr(t) || isbasic(t)) && isbasic(tt))
		return t;
	if (!iserror2(t,tt))
		yyerror(aritherr);
	return b_error;
}

t_type arith(t,tt)
t_type t,tt;
{
	if (isbasic2(t,tt))
		return t;
	if (!iserror2(t,tt))
		yyerror(aritherr);
	return b_error;
}

t_type array(t,tt)
t_type t,tt;
{
	if (isbasic(t) || isptr(tt))
		return deref(tt);
	if (isbasic(tt) || isptr(t))
		return deref(t);
	if (!iserror2(t,tt))
		yyerror("illegal array reference");
	return b_error;
}

int typeeq(t,tt)
t_type t,tt;
{
	return (t.chain==tt.chain && t.typespec.num==tt.typespec.num);
}

t_type compare(t,tt)
t_type t,tt;
{
	if (typeeq(t,tt) || isbasic(t) && isptr(tt) || isptr(t) && isbasic(tt))
		return Basic;
	if (isprocess(t) && isprocess(tt))
		if (t.typespec.num==NS_NULLPID || tt.typespec.num==NS_NULLPID)
			return Basic;
	if (!iserror2(t,tt))
		yyerror("type clash in comparison expression");
	return b_error;
}

t_type condition(t,tt,s)
t_type t,tt;
char *s;
{
	if (isbasic(t) && (isbasic(tt) || isptr(tt)))
		return t;
	if (isptr(t) && isbasic(tt))
		return t;
	if (isprocess(t) && isprocess(tt)) {
		register c1=t.typespec.num;
		register c2=tt.typespec.num;
		if (c1==c2 || c1==NS_ANYTYPE || c2==NS_NULLPID)
			return t;
		if (c2==NS_ANYTYPE || c1==NS_NULLPID)
			return tt;
		yyerror(s);
		return b_error;
	}
	if (typeeq(t,tt))
		return t;
	if (!iserror2(t,tt))
		yyerror(s);
	return b_error;
}

t_type assign(t,tt,s)
t_type t,tt;
char *s;
{
	if (isbasic(tt) && (isbasic(t) || isptr(t)))
		return t;
	if (isfunc(tt) && isptr(t) && typeeq(deref(t),tt))
		return t;
	if (isprocess(t) && isprocess(tt)) {
		register c1=t.typespec.num;
		register c2=tt.typespec.num;

		if (c1==c2 || c1==NS_ANYTYPE || c2==NS_NULLPID)
			return t;
		if (c2==NS_ANYTYPE)
			yyerror("process type anytype is only assignable to another anytype");
		else
			yyerror(s);
		return b_error;
	}
	if (typeeq(t,tt))
		return t;
	if (!iserror2(t,tt))
		yyerror(s);
	return b_error;
}

t_type arithassign(t,tt)
t_type t,tt;
{
	if (isbasic(t) && isbasic(tt))
		return t;
	if (!iserror2(t,tt))
		yyerror("type clash in assignment expression");
	return b_error;
}
t_type addassign(t,tt)
t_type t,tt;
{
	if ((isptr(t) || isbasic(t)) && isbasic(tt))
		return t;
	if (!iserror2(t,tt))
		yyerror("type clash in assignment expression");
	return b_error;
}


struct table_tag *Tables;

#define MAXHALF 20 /* number of recursive struct, etc. defs at one time */
short lasthalf=0;
symbol *halfthings[MAXHALF];
symbol *lexer_symbol;

t_type maketype(chain,num,obj)
unsigned chain;
unsigned short num;
unsigned short obj;
{
	t_type t;

	t.chain=chain;
	t.typespec.num=num;
	t.typespec.obj=obj;
	return t;
}

int declare(name,num,t,alias)
char *name;
unsigned num;
t_type t;
char *alias;
{
	symbol *olds,*s;
	short typedefp=0,halfdec=0;

	switch (num) {
	case NS_BADTAG:
		return NS_BADTAG;
	case NS_HALFDEC:
		halfdec=lasthalf+1;
		num=NS_COMPLEX;
		break;
	case NS_TYPEDEF:
		typedefp++;
		num=NS_VAR;
		break;
	}
	s=new(symbol);
	s->name=name;
	s->halfdec=halfdec;
	s->num=num;
	s->typedefp=typedefp;
	s->t=t;
	if (alias!=NULL) {
		s->alias=(char *)malloc(1+strlen(alias));
		strcpy(s->alias,alias);
	} else
		s->alias = NULL;
	if (s == (olds=(symbol *)btinsert(Tables->table,s))) {
		if (halfdec)
			halfthings[lasthalf++]=s;
		s->name=(char *)malloc(1+strlen(name));
		strcpy(s->name,name);
		depend_s(name, num, contextof(t));	/* inserted for degubber 
							   dependency tree */
		if (num==NS_COMPLEX) {
			return NameSpace++;
		} else {
			return NS_VAR;
		}
	}
	/* <name> has already been declared in the namespace <num> */
	free(s);
	if (num==NS_COMPLEX) {
		if (halfdec)
			if (olds->t.typespec.obj == t.typespec.obj)
				return contextof(olds->t);
			else {
				yyerror("%s is not a %s tag",name,tagtypetext[t.typespec.obj]);
				return NS_BADTAG;
			}
		if (!olds->halfdec || olds->t.typespec.obj != t.typespec.obj) {
			yyerror("%s has already been declared as a %s tag",
			name,tagtypetext[olds->t.typespec.obj]);
			return NS_BADTAG;
		} else {
			/* definition of implicitly declared struct, union or process tag */
			short halfdec=olds->halfdec;
			olds->halfdec=0; /* now fully defined */
			if (halfdec != lasthalf--) /* remove from halfthings table */
				halfthings[halfdec-1]=halfthings[lasthalf];
			return olds->t.typespec.num;
		}
	} else if (t.chain==olds->t.chain && t.typespec.num==olds->t.typespec.num)
		return NS_VAR; /* consistent redefinition */
	yyerror("inconsistent redeclaration of %s",name);
	return NS_BADTAG;
}

t_type half_declare_func(name)
char *name;
{
	t_type t;

	t=lookup(name,NS_VAR);
	if (isfunc(t))
		return t;
	t=func(Basic);
	if (declare(name,NS_VAR,t,0) == NS_BADTAG)
		return b_error;
	return t;
}

t_type half_declare_tag(name,object)
char *name;
unsigned object;
{
	t_type t;

	if (lasthalf==MAXHALF) {
		yyerror("too many unresolved recursive types; %s tag %s looses",
		tagtypetext[object],name);
		return b_error;
	}
	t=maketype(0,NameSpace,object);
	t.typespec.num=declare(name,NS_HALFDEC,t,0);
	return t;
}

int declare_tag(name,object)
char *name;
unsigned object;
{
	static char fakename[6];
	
	if (!name) {
		sprintf(fakename,"%-5d",NameSpace);
		name=fakename;
	}
	return declare(name,NS_COMPLEX,maketype(0,NameSpace,object),NULL);
}

int newtag()
{
	return NameSpace++;
}

symbol *slookup(name,context)
char *name;
unsigned context;
{
	symbol s,*olds;
	struct table_tag *t=Tables;

	s.name=name;
	s.num=context;
	while (t) {
		if (&s != (olds=(symbol *)btinsert(t->table,&s))) {
			if (olds->alias)
				cctext=olds->alias;
			else
				cctext=name;
			return olds;
		}
		btdelete(t->table,&s);
		t = t->prev;
	}
	cctext=name;
	return 0;
}

t_type lookup(name,context)
char *name;
unsigned context;
{
	symbol *s;

	if(s=slookup(name,context))
		return s->t;
	return b_error;
}

t_type internal_lookup(name,context)
char *name;
unsigned context;
{
	symbol s,*olds;
	struct table_tag *t=Tables;

	s.name=name;
	s.num=context;
	while (t) {
		if (&s != (olds=(symbol *)btinsert(t->table,&s)))
			return olds->t;
		btdelete(t->table,&s);
		t = t->prev;
	}
	return b_error;
}

t_type lookup_tag(name,tagtype)
char *name;
unsigned tagtype;
{
	symbol *s;

	if((s=slookup(name,NS_COMPLEX)) && s->t.typespec.obj==tagtype)
		return s->t;
	yyerror("%s is not a %s tag",name,tagtypetext[tagtype]);
	return b_error;
}

t_type lookup_var(name,context)
char *name;
unsigned context;
{
	symbol *s;

	if(s=slookup(name,context))
		return s->t;
	yyerror("undeclared identifier %s",name);
	declare(name,NS_VAR,b_error,0);
	return b_error;
}

t_type lookup_last(name)
char *name;
{
	if(lexer_symbol)
		return lexer_symbol->t;
	yyerror("undeclared identifier %s",name);
	declare(name,NS_VAR,b_error,0);
	return b_error;
}

#include "y.tab.h"
int ident()
{
	extern yystype yylval;

	lexer_symbol=slookup(yytext,NS_VAR);
	if (!lexer_symbol || !lexer_symbol->typedefp)
		return IDENTIFIER;
	yylval.y_type=lexer_symbol->t;
	return TYPE_NAME;
}

int label()
{
	extern int trapping;
	extern int context;

	if (trapping || context)
		return ident();
	return LABEL;
}

int ident_compare(x,y)
symbol *x,*y;
/* btree.c spec requires -1,0,1 but will accept negative, zero, positive */
{
	int i;

	if (i = (x->num - y->num))
		return i;
	return strcmp(x->name,y->name);
}
/*--------------------------------------------------------------------------------*/
/* BLOCK ENTRANCE AND EXIT, INCLUDING SHARED VAR FILE SHUFFLING */
char lshared[]="/tmp/,cccl_XXXXXX";
char gshared[]="/tmp/,cccg_XXXXXX";
FILE *Fshared,*Gshared,*Lshared;
int localsharedp=0;

includefile(F)
FILE *F;
{
	register c;

	rewind(F);
	while ((c=fgetc(F)) != EOF)
		putchar(c);
	fclose(F);
}

includeshared()
{
	static char *vars="{\nint CC_j,CC_n,CC_ss;\nchar *CC_s;\n";

	if (localsharedp) {
    		puts(vars);
		includefile(Lshared);
		Lshared=fopen(lshared,"w+");
		Fshared=Lshared;
		localsharedp=0;
		puts("}");
	} else if (Fshared==Gshared) {
		puts("CC_shinit()");
		puts(vars);
		includefile(Gshared);
		puts("}");
	}
}

firstblock()
{
	Tables=new(struct table_tag);
	Tables->prev=0;
	Tables->table = (struct btree *)btnew(BTORDER,ident_compare);
	blocknum=0;
	mktemp(lshared);
	mktemp(gshared);
	Lshared=fopen(lshared,"w+");
	Gshared=fopen(gshared,"w+");
	Fshared=Gshared;
}

enterblock()
{
	Tables->next = new(struct table_tag);
	Tables->next->prev = Tables;
	Tables = Tables->next;
	Tables->table = (struct btree *)btnew(BTORDER,ident_compare);
	blocknum++;
	Fshared=Lshared;
}

exitblock()
{
	domap();
	Tables=Tables->prev;
	btfree(Tables->next->table,free);
	free(Tables->next);
	if (!--blocknum)
		Fshared=Gshared;
}

finalexit()
{
	domap();
	fclose(Lshared);
	unlink(lshared);
	includeshared();
	unlink(gshared);
}

installpredefined()
{
	extern t_type Int;
	t_type ftype;

	ftype=func(Int);
	declare_var("c_abort",		NS_VAR,ftype);
	declare_var("c_associate",	NS_VAR,ftype);
	declare_var("c_active",		NS_VAR,ftype);
	declare_var("c_completed",	NS_VAR,ftype);
	declare_var("c_invalid",	NS_VAR,ftype);
	declare_var("c_getpriority",	NS_VAR,ftype);
	declare_var("c_changepriority",	NS_VAR,ftype);
	declare_var("c_nullpid",	NS_VAR,	  maketype(0,NS_NULLPID,e_process));
	declare_var("anytype",		NS_COMPLEX,maketype(0,NS_ANYTYPE,e_process));

	/*  c_count() (a.k.a. c_transcount()) is hard-wired into the parser & lexer */

}

/*--------------------------------------------------------------------------------*/
/* GRAM_INIT() -- CALLED FROM MAIN, HANDLES ALL INITIALIZATION CODE */

printtree(s)
symbol *s;
{
	t_type t;
	t_typespec tt;

	t=s->t;
	tt=t.typespec;
	printf(" *  |%-20s%-8d%-8x%-8d%-8d%-20s|\n",
	s->name,s->num,t.chain,tt.num,tt.obj,(s->alias ? s->alias : " "));
}



domap()
{
	if (map) {
		puts("\n/*\n *  exiting block in which the following were declared:");
		printf(" *  |%-20s%-8s%-8s%-8s%-8s%-20s|\n",
		"name","context","chain","num","obj","alias");
		btprint(Tables->table,printtree);
		puts(" */");
	}
}

gram_init()
{
	depend_Init();
	firstblock();
	installpredefined();
}

gram_end()
{
	
	finalexit();
}





