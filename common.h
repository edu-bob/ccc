/*
** Concurrent C
**
** Common declarations
**
** Constants
** ---------
*/

#ifndef YYLMAX
#define YYLMAX 200	/* as in lex.yy.c	*/
#endif

/*
** Inline procedures
** -----------------
*/

#define new(t) (t *)malloc(sizeof(t))

/*
** Types
** -----
*/

extern char yytext[];
typedef char yystring[YYLMAX];

/*
** Globals
** -------
*/

#ifndef EXTERN
#define EXTERN extern
#endif

char	*Filename;
int	Linenum;

/*
** Procedures
** ----------
*/

int ident();
int label();
int fix_CC();
int comment();
int linesynch();
int nextline();
