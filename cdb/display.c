/*---------------------------------------------------------------------------
 * This code is the user interface for cdb.
 * 
 * William L. Lynch		Tue Jul 29 13:26:08 1986
 *---------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <stdio.h>
#include "../bool.h"
#include "cdb.h"
#include "static.h"
#include "dynamic.h"
#include "stateString.h"
#include "format.h"
#include "tables.h"

#define min(a,b)	((a)<(b) ? a : b)
#define max(a,b)	((a)>(b) ? a : b)

typedef int window;
window cdbIO, statusWin, userIO, userError;
extern window control;

display_init()
{
	statusWin = win_create(CDBeRRsTART, 1, CDBeRRlINES, COLS);
	win_move(statusWin, 2, 1);
	win_cursor_off(statusWin);

	userError= win_create(USEReRRsTART, 1, USEReRRlINES, COLS);
	win_cursor_off(userError);

	userIO = win_create(USERiosTART, 1, USERiolINES, COLS);
	win_cursor_off(userIO);

	cdbIO = win_create(CDBsTART, 1, CDBlINES, COLS);
	win_cursor_off(cdbIO);
	win_select(cdbIO);
}

display_end()
{
	win_end();
}




extern sock std;


userOut( buf, size )
char *buf;
int size;
{
	win_push(userIO);
	for ( ; size > 0; size-- )
		putchar(*buf++);
	fflush(stdout);
	win_pop();
}


userErr( buf,size )
char *buf;
int size;
{
	win_push(userError);
	for ( ; size > 0 ; size-- )
		putchar(*buf++);
	fflush(stdout);
	win_pop();
}


cdbOut( s, p1, p2, p3, p4, p5, p6, p7, p8, p9 )
string s;
{
	win_push(cdbIO);
	printf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	fflush(stdout);
	win_pop();
}

cdbStatus( s, p1, p2, p3, p4, p5, p6, p7, p8, p9 )
string s;
{
	win_push(statusWin);
	printf("\n");
	printf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	fflush(stdout);
	win_pop();
}


string
getTag( pp )
process *pp;
{
	char buf[40];
	
	sprintf(buf, "%.10s<%d>", pp->name, static_infoLookup(pp->name)->tagId++);
	return newstr(buf);
}


display_clock( time )
unsigned long time;
{
	win_push(control);
	win_move(control, CLOCKrOW, CLOCKcOL);
	printf("%10d", time);
	fflush(stdout);
	win_pop();
}

display_clear( pp )
process *pp;
{
	int i;
	
	win_push(cdbIO);
	for ( i=0; i < LINESpERpROCESS; i++  ) {
		win_move(cdbIO, pp->display.row + i, 0);
		printf("%5s","");      /* 5 is CHANNEL */
		win_move(cdbIO, pp->display.row + i, pp->display.col);
		clear_to_eol();		
	}
	win_pop();
}


display_process( pp )
process *pp;
{
	win_push(cdbIO);
	win_move(cdbIO, pp->display.row, pp->display.col);
	printf("%s", stateString[(int)pp->state]);
	switch ( pp->state ) {
	case waitTrans:
		printf(" on %s.%s", pp->with->tag,
					pp->info->points.transact[pp->id].name);
		display_line(pp, pp->info->points.transact[pp->id].line);
	        break;
	case waitBorn:
		printf(" %s", pp->info->points.create[pp->id].name);
		display_line(pp, pp->info->points.create[pp->id].line);
	        break;
	case waitAccept:
		printf(" %s", pp->info->points.accept[pp->id].name);
		display_line(pp, pp->info->points.accept[pp->id].line);
	        break;
	case accepting:
		printf(" %s", pp->info->points.accept[pp->id].name);
		display_line(pp, pp->info->points.accept[pp->id].line);
	        break;
	case waitSelect:
		display_line(pp, pp->info->points.select[pp->id].line);
	        break;
	}
	clear_to_eol();
	fflush(stdout);
	win_pop();
}


display_line( pp, line )
process *pp;
int line;
{
	printf(", LINE: %d", line);
}
display_parms( pp )
process *pp;
{
	int i;
	char *comma, *parmp, *parms;
	int bound = COLS;


	win_push(cdbIO);
	win_move(cdbIO, pp->display.row+1, pp->display.col);
	
	parms = instances(bound, char);

	if ( pp->state == treturned )
		sprintf(parms, "treturned ");
	else 
		sprintf(parms, "(");
	parmp = &parms[strlen(parms)];

	comma = "";
	for ( i=0; i < pp->numParms; i++ ) {
		int length;
		
		length = parmp - parms;
		if ( length + strlen(pp->parms[i]) + 1 > bound ) {
			bound *= 2;
			parms = (char *)realloc(parms, bound);
			parmp = parms + length;
		}
					
		strcat(parmp, comma);
		strcat(parmp, pp->parms[i]);
		parmp += strlen(parmp);
		comma = ", ";
	}
	if ( pp->state == treturned ) {
		char toString[100];
		sprintf(toString," to %s.%s", pp->with->tag,pp->with->transactName);
		strcat(parmp, toString);
	} else
		strcat(parmp, ")");
	
	printf("%.60s", parms);
	clear_to_eol();
	if ( parmp - parms > 60 )
		cdbStatus(&parms[60]);
	free(parms);
	
	fflush(stdout);
	win_pop();
}

display_eraseParms( pp )
process *pp;
{
	int i;

	win_push(cdbIO);
	win_move(cdbIO, pp->display.row+1, pp->display.col);
	clear_to_eol();
	fflush(stdout);
	win_pop();
}


display_template(pp)
process *pp;
{
/*
	win_push(cdbIO);
	win_move(cdbIO, pp->display.row, CHANNEL+1);
	printf("%-.12s ", pp->tag);
	fflush(stdout);
	win_pop();
*/
}

display_locationInit(pp)
process *pp;
{
	static int line = 2;
	
	pp->display.row = line;
	line += LINESpERpROCESS;
	pp->display.col = CHANNEL+1+12+1;
	pp->display.channel = 0;
	display_template(pp);
}

static bool vert[CHANNEL] = {0,1,1,1,1};

display_drawLine( pp1, pp2 )
process *pp1, *pp2;
{
	
	int i, j;
	
	win_push(cdbIO);
	for ( i=1; i < CHANNEL; i++ ) {
		if ( vert[i] )
			break;
	}
	vert[i] = FALSE;
	
	pp1->display.channel = pp2->display.channel = i;

	drawHorizontal(pp1->display.row,  i, CHANNEL, '-', '>');
	drawHorizontal(pp2->display.row,  i, CHANNEL, '-', '<');
	drawVertical(i, min(pp1->display.row, pp2->display.row), max(pp1->display.row, pp2->display.row), '|');
	fflush(stdout);
	win_pop();
}


display_eraseLine( pp1, pp2 )
process *pp1, *pp2;
{
	int low, high, channel;
	
	win_push(cdbIO);
	low = min(pp1->display.row, pp2->display.row);
	high =  max(pp1->display.row, pp2->display.row);
	
	channel = pp1->display.channel;
	vert[channel] = TRUE;
	
	drawHorizontal(high, channel, CHANNEL, ' ', ' ');
	drawHorizontal(low, channel, CHANNEL, ' ', ' ');
	drawVertical(channel, low, high, ' ');
	win_pop();
}


drawHorizontal(row, startCol, endCol, line, arrow)
int row, startCol, endCol;
char line, arrow;
{
	win_move(cdbIO, row, endCol);
	putchar(arrow);

	win_line_move(cdbIO, row, startCol);
	if ( line == ' ' )
		win_line_erase(cdbIO, row, endCol);
	else
		win_line_draw(cdbIO, row, endCol);
}



drawVertical(col, startRow, endRow, line)
int col, startRow, endRow;
char line;
{
	win_line_move(cdbIO, startRow, col);
	if ( line == ' ' )
		win_line_erase(cdbIO, endRow, col);
	else
		win_line_draw(cdbIO, endRow, col);
}


display_refresh()
{
	int i;

	for ( i=1; i < CHANNEL; i++ )
		vert[i] = TRUE;
	dynamic_dump();
}



debugPrintf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9)
string s;
{
	win_push(cdbIO);
	win_move(cdbIO, 30, 1);
	printf(s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	fflush(stdout);
	win_pop();
}
