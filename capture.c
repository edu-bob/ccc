#include <stdio.h>
#include <ctype.h>
/*
** capture - capture scanned text
**
** This module provides a capability for capturing and recalling multiple
** strings of text simultaneously.  The interface is:
**
** capture_init() - initialization.  Resets all structures.
** capture_new()  - starts a new capture service. Returns an integer that
**		refers to the new capture area.
** capture_addch(ch,i) - add character "ch" to capture area "i".
** capture_addstr(s,i) - add string "s" to capture area "i".
** capture_addchall(ch) - add character "ch" to all capture areas.
** capture_addstrall(s) - add string "s" to all capture areas.
** capture_string(i) - returns pointer to captured string for capture area "i",
**		This storage must be copied if saved.
** capture_free(i) - release all storage associated with capture area i.
**
** capture_extend(str) - like _addstrall, but makes sure that a separator
**		exists between words.
**
** Bob Brown/RIACS/March 1986
*/

#ifndef TRUE
#define TRUE 1
#define FALSE 0
typedef char bool;
#endif

/*
** CONSTS
*/

#define CAPTUREINC 4
#define STRINGINC 10

/*
** TYPES
*/

struct capture {
	char *string;
	int length;
	int sizeofstring;
	bool used;
};

/*
** VARS
*/

static struct capture *Capture;
static int CaptureSize;
static bool PassThrough = TRUE;

/*
** METHODS
*/

capture_init()
{
	CaptureSize = 0;
}
capture_new()
{
	int i, free;

	for ( i=0 ; i<CaptureSize ; i++ )
		if ( !Capture[i].used )
			break;
	if ( i >= CaptureSize ) {
		free = CaptureSize;
		CaptureSize += CAPTUREINC;
		Capture = (struct capture *)realloc(Capture, CaptureSize * sizeof(struct capture));
		for ( i=0 ; i<CAPTUREINC ; i++ )
			Capture[CaptureSize-i-1].used = FALSE;
	} else
		free = i;
	Capture[free].used = TRUE;
	Capture[free].string = (char *)malloc(STRINGINC);
	Capture[free].sizeofstring = STRINGINC;
	Capture[free].length = 0;
	*Capture[free].string = 0;
	return free;
}

capture_addch(ch,i)
char ch;
int i;
{
	if ( i >= CaptureSize || !Capture[i].used )
		return;
	if ( Capture[i].length+1 >= Capture[i].sizeofstring ) {
		Capture[i].sizeofstring += STRINGINC;
		Capture[i].string = (char *)realloc(Capture[i].string, Capture[i].sizeofstring*sizeof(char));
	}
	Capture[i].string[Capture[i].length++] = ch;
	Capture[i].string[Capture[i].length] = 0;
}
capture_addstr(str,i)
char *str;
int i;
{
	while ( *str )
		capture_addch(*str++, i);
}
capture_addchall(ch)
char ch;
{
	int i;
	for ( i=0 ; i<CaptureSize ; i++ )
		if ( Capture[i].used )
			capture_addch(ch,i);
}
capture_addstrall(str)
char *str;
{
	int i;
	for ( i=0 ; i<CaptureSize ; i++ )
		if ( Capture[i].used )
			capture_addstr(str,i);
}
char *
capture_string(i)
int i;
{
	if ( i >= CaptureSize || !Capture[i].used )
		return NULL;
	return Capture[i].string;
		
}
capture_free(i)
int i;
{
	if ( i >= CaptureSize || !Capture[i].used )
		return;
	free(Capture[i].string);
	Capture[i].used = FALSE;
}

capture_extend(str)
char *str;
{
	int i;
	for ( i=0 ; i<CaptureSize ; i++ )
		if ( Capture[i].used )
			capture_extend1(str,i);
}
capture_extend1(str,i)
char *str;
int i;
{
	bool addspace;
	char right,left;

	if ( i >= CaptureSize || !Capture[i].used )
		return;
	if ( Capture[i].length > 0 ) {
		left = Capture[i].string[Capture[i].length-1];
		right = str[0];
		if ( isalnum(left) )
			addspace = isalnum(right) || index(right,"\"'%^&*~+-|/=")!=NULL;
		else if ( isalnum(right) )
			addspace = index(left,"%^&-+=|,/;:\"']}")!=NULL;
		else
			addspace = FALSE;
	}
		if ( addspace )
			capture_addch(' ',i);
	capture_addstr(str,i);
}
capture_printf(f,p0,p1,p2,p3,p4,p5,p6,p7,p8,p9)
char *f;
int p0,p1,p2,p3,p4,p5,p6,p7,p8,p9;
{
	char buf[BUFSIZ];
	sprintf(buf,f,p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
	if ( PassThrough )
		printf("%s",buf);
	capture_addstrall(buf);
}
capture_pass_through(flag)
bool flag;
{
	PassThrough = flag;
}
