
#define YYDEBUG 1
#define save(x)		strcpy(x,yytext)
#define newstr(s)	(char *)strcpy(malloc(strlen(s)+1),s)

int casenum;
int argnum;
int procnum= -1;
int proccontext=0;
int callprocnum;
int maxargnum;
int istimed=0;
int selectnum=0;
int transid;
int thead=0;
int boxnum;
int shtypedefnum=0;
int accept_in_select;
yystring procname;
yystring argname={'\0'};
yystring alias;
yystring acceptname;
yystring idname;
yystring transname;
yystring dotname;
yystring errorstring;
unsigned context=0;
unsigned transnum;
unsigned acceptnum;
unsigned acceptnumgenerator=0;

int proc_type_name;
int proc_formal_names;
char typetrap[128];
int typetrap_i;

int yyerror();
extern char *cctext;
extern FILE *Fshared;

/* statement types for context sensitive statement restrictions */
#define NORMALSTAT	0
#define PROCSTAT	1
#define ACCEPTSTAT	2
int stattype=NORMALSTAT;

/* for installing numbers as names */
char *itoa(i)
int i;
{
    static char value[10];

    sprintf(value,"%d",i);
    return value;
}


/*--------------------------------------------------------------------------------*/
/* PRINTING AND TRAPPING */

char *sizes[64];
char tbuf[1024];
char ntf[128];
int tbuf_i=0;
char *trap_t;
char ps[]="%s",pss[]="%s ";
int trapping=0;

#define start_trapping	{tbuf_i=0;trapping=1;linesynch();}
#define end_trapping	{tbuf[tbuf_i]=0;printf(ps,tbuf);trapping=0;linesynch();}
#define TP(x)	tbuf[tbuf_i++]=x
#define TPY	{trap_t=yytext;while(tbuf[tbuf_i++]= *trap_t++);tbuf[--tbuf_i]=0;}
#define pri(x)	{sprintf x ;trap_t=ntf;while(tbuf[tbuf_i++]= *trap_t++);\
		tbuf[tbuf_i-1]=' ';}

#define P2(a,b)	{putchar(a);putchar(b);}
#define PSN	{putchar(';'); putchar('\n');}

tinsert(s,i)
char *s;
int i;
{
    int l=tbuf_i-i;
    int k=strlen(s);
    char *u= &tbuf[tbuf_i];
    char *v=u+k;
while(l--)
    *--v = *--u;
while(*s)
    *u++ = *s++;
tbuf_i+=k;
}
/*--------------------------------------------------------------------------------*/
/* TYPE CHASING FUNCTIONS */

t_type type;
extern int blocknum;
t_type Basic =		{0,{NS_BADTAG,e_basic}};
t_type Unsigned =	{0,{NS_BADTAG,e_unsigned}};
t_type Unsignedint =	{0,{NS_BADTAG,e_unsignedint}};
t_type Unsignedchar =	{0,{NS_BADTAG,e_unsignedchar}};
t_type Unsignedshort =	{0,{NS_BADTAG,e_unsignedshort}};
t_type Unsignedlong =	{0,{NS_BADTAG,e_unsignedlong}};
t_type Longint =	{0,{NS_BADTAG,e_longint}};
t_type Double =		{0,{NS_BADTAG,e_double}};
t_type Short =		{0,{NS_BADTAG,e_short}};
t_type Char =		{0,{NS_BADTAG,e_char}};
t_type Float =		{0,{NS_BADTAG,e_float}};
t_type Int =		{0,{NS_BADTAG,e_int}};

/*--------------------------------------------------------------------------------*/
/* PROCESS FORMAL PARAMETER NAME STORAGE AND DECLARATION */

int null_parameter_list();
int remember_first_parameter_name();
int remember_another_parameter_name();
int declare_parameters();
/*--------------------------------------------------------------------------------*/
/* SHARED VARIABLE DECLARATION HANDLING */

int shinit_var(),sh_array(),shinit_array(),box();
