/*--------------------------------------------------------------------------------*/
#include <stdio.h>
#include "common.h"
#include "cctypes.h"

/*--------------------------------------------------------------------------------*/
/* PROCESS FORMAL PARAMETER NAME STORAGE AND DECLARATION */

extern int procnum;
extern char procname[];

struct t_param {
	char *name;
	struct t_param *next;
} *param_head,*param_tail;

null_parameter_list()
{
	param_head = 0;
}

remember_first_parameter_name()
{
	param_tail = new(struct t_param);
	param_tail->name = (char *)malloc(strlen(yytext)+1);
	strcpy(param_tail->name,yytext);
	param_head = param_tail;
	param_tail->next=0;
}

remember_another_parameter_name()
{
	param_tail = (param_tail->next = new(struct t_param));
	param_tail->name = (char *)malloc(strlen(yytext)+1);
	strcpy(param_tail->name,yytext);
	param_tail->next=0;
}

declare_parameters()
{
	struct t_param *current=param_head;
	int t_argnum=1;

	while(current){
		printf("%s %s;\n",spec_get_param_type(procname,t_argnum-1),current->name);
		declare_var(current->name,NS_VAR,lookup(itoa(t_argnum++),procnum));
		current=current->next;
	}
}


extern char debugFlag;

declare_debug_copy(procname)
{
	struct t_param *current=param_head, *trash;
	int t_argnum=0; 

	if ( !debugFlag || !current )		/* not debugging, or no parms */
		return;
	while ( current ) {
		printf("DD_create_%s.arg%d = %s;\n", procname, ++t_argnum, current->name);
		trash=current;
		current=current->next;
		free(trash); 
	}
	printf("DD_createArgs = (char *)&DD_create_%s;\n", procname);
}



/*--------------------------------------------------------------------------------*/
/* LEX FUNCTIONS */

void fixCC_()
{
	int j;

	for (j=strlen(yytext);j>=0;j--)
		yytext[j+3]=yytext[j];
	yytext[0]='C';
	yytext[1]='C';
	yytext[2]='_';
}

int linesynchp=1; /* resynch linenum on every linesynchp calls to linesynch */
int column = 0;
char lastline[800];
#define MAXLINEERR 10
int numerrors=0,toomanyerrors=0,toomanycolumn;
yystring errors[MAXLINEERR];
int errorcolumns[MAXLINEERR];

nextline()
{
	if (numerrors)
		doerror();
	Linenum++;
	column=0;
}

nextchar(c)
char c;
{
	if (c=='\n')
		nextline();
	else
		lastline[column++]=c;
}

yywrap()
{
	return(1);
}

flusherrors()
{
	char c;
 
	if (numerrors) {
		do {
			nextchar(c=exportinput());
		} 
		while (c != '\n' && c);
		if (!c) {
			column--;
			doerror();
		}
	}
}

comment() /* echo comments? don't store all this useless stuff? */
{
	char c;

	column+=2;
	do {
		do {
			nextchar(c=exportinput());
		} 
		while (c != '*' && c);
		nextchar(c=exportinput());
	} 
	while (c != '/' && c);
}

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		nextchar(yytext[i]);
}

yyerror(s,a1,a2,a3,a4,a5)
char *s,*a1,*a2,*a3,*a4,*a5;
{
	if (numerrors==MAXLINEERR) {
		toomanyerrors=1;
		toomanycolumn=column;
		return;
	}
	sprintf(errors[numerrors],s,a1,a2,a3,a4,a5);
	errorcolumns[numerrors]=column-1;
	numerrors++;
}

doerror()
{
	int j;

	lastline[column]='\0';
	for (j=0;j<numerrors;j++)
		fprintf(stderr,"\"%s\", line %d, char %d: %s\n",Filename,Linenum,errorcolumns[j]+1,errors[j]);
	if (toomanyerrors)
		fprintf(stderr,"\"%s\", line %d, char %d: too many errors on this line\n",Filename,Linenum,toomanycolumn);
	toomanyerrors=0;
	numerrors=0;
}

/* YUCK!!! This is really ugly and ad-hoc... */
forcelinesynch()
{
	if (linesynchp)
		printf("\n# line %d \"%s\"\n",Linenum,Filename);
}

linesynch()
{
	static int neglect=0;

	if (linesynchp && ++neglect>linesynchp) {
		printf("\n# line %d \"%s\"\n",Linenum,Filename);
		neglect=0;
	}
}
