/*
* Table definitions
*/
#define CDB
#include "../DD_kernel.h"

typedef char *string;

typedef DD_formal formal;

/*
	string name;
	unsigned line;
	unsigned space;
	int numFormals;
	formal *formals;
*/
typedef DD_occur occur;

/*
	unsigned line;
	unsigned space;
*/
typedef DD_otherOccur otherOccur;

/*
	unsigned line;
	unsigned space;
	unsigned termLine;
	unsigned immedLine;
*/
typedef DD_selectOccur selectOccur;

/*
	unsigned line;
	unsigned space;
	---- valueSize not used.
*/
typedef DD_treturnOccur treturnOccur;

/*
	string name;
	unsigned short space;
	unsigned short num;
	unsigned chain;
	unsigned short obj;
*/
typedef DD_symEntry symbol;

typedef int sock;

#define newstr(s)	(string)strcpy(malloc(strlen(s)+1),s)
#define instances(n,t)	(t *)calloc((n), sizeof (t))
#define instance(t)	instances(1,t)

