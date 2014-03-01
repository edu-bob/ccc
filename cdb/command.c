/*---------------------------------------------------------------------------
 * The command routines handle command strings.

 * William L. Lynch		Thu Jul 31 13:04:50 1986
 *---------------------------------------------------------------------------
 */

#include <signal.h>
#include <stdio.h>
#include "../btree.h"
#include "../bool.h"
#include "cdb.h"
#include "format.h"
#include "command.h"
#include "static.h"
#include "dynamic.h"
#include "tables.h"

#define SPEED 100000

typedef int window, menu, cycle, button;

extern sock std, err, cdb;
window control;
extern window cdbIO;
cycle cdbCycle;

typedef struct {
	menu m;
	pointType *p;		/* corresponding point array */
	unsigned short type:1;	/* m refers to trace or break */
} mtp;				/* menu to point */
static char *menuToPoints;

command_exit()
{
	command_quit(TRUE);
	cdb_end();
	exit();
}

command_sigPipe(sig)
int sig;
{
	cdbStatus("Connection closed.");
}

command_menuCmp( m1, m2 )
mtp *m1, *m2;
{
	return m1->m - m2->m;
}

command_init(prog)
string prog;
{
	menu m;
	cycle c;
	
	menuToPoints = btnew(10, command_menuCmp);

	signal(SIGINT, command_exit);
	signal(SIGPIPE, command_sigPipe);
	control = win_create(CONTROLsTART, 1, CONTROLlINES, COLS);

	win_push(control);
	win_move(control, CLOCKrOW-1, CLOCKcOL);
	printf("%10s", "Clock");
	win_cursor_off(control);

	keymap_bind("\030d", debug_dump, 1);
	keymap_bind("\030\030", parse, 0);
	keymap_default(commun_send);

	button_create(control, 2, 1, "step", 6, command_single, TRUE);
	button_create(control, 2, 67, "print", 7, command_scrDump, 1);

	m = menu_create(control, 2, 10, "speed", 9);
		menu_add(m, "SLOWER", command_slower, 10);
		menu_add(m, "slower", command_slower, 1);
		menu_add(m, "faster", command_faster, 1);
		menu_add(m, "FASTER", command_faster, 10);
	
	m = menu_create(control, 2, COLS-8, "QUIT", 4);
		menu_add(m, "yes", command_quit, TRUE);
		menu_add(m, "no", command_quit, FALSE);

	c = cycle_create(control, 2, 23, prog, 4, command_prog, 0);
		cycle_add(c, "stop");
		cycle_add(c, "run");

	cdbCycle = cycle_create(control, 3, 23, "       cdb:", 4, command_cdb, 0);
		cycle_add(cdbCycle, "stop");
		cycle_add(cdbCycle, "run");

	c = cycle_create(control, 2, 42, "trace:", 3, command_tracing, 0);
		cycle_add(c, "on");
		cycle_add(c, "off");

	c = cycle_create(control, 3, 42, "break:", 3, command_breaking, 0);
		cycle_add(c, "on");
		cycle_add(c, "off");
	
	display_clock(0);
	win_pop();
}

command_quit( m, arg )
menu m;
int arg;
{
	if ( arg ) {
		commun_signal(SIGPIPE);
		commun_finished(TRUE);
	}
}


command_print( m, arg )
menu m;
int arg;
{
	dynamic_dump();
}


command_slower( m, arg )
menu m;
int arg;
{
	commun_throttle(arg * SPEED);
}

command_faster( m, arg )
menu m;
int arg;
{
	commun_throttle(arg * -SPEED);
}
	

/** buttons **/
command_single( b, arg )
button b;
int arg;
{
	commun_recvPacket(TRUE);
}

/** keys **/
command_scrDump( arg )
int arg;
{
	win_push(0);
	win_basewin();
	system("/usr/local/bin/newulbg");
	win_pop();
}


/** cycles **/

command_tracing( c, arg, value )
cycle c;
int arg, value;
{
	if ( value == 1 )
		tracing(FALSE);
	else {
		tracing(TRUE);
		display_refresh();
	}
}

command_breaking( c, arg, value )
cycle c;
int arg, value;
{
	breaking( value == 1 ? FALSE : TRUE );
}


command_prog( c, arg, value )
cycle c;
int arg, value;
{
	if ( value == 0 )
		commun_signal(SIGTSTP);
	else
		commun_signal(SIGCONT);
}

command_cdb( c, arg, value )
cycle c;
int arg, value;
{
	if ( value == 0 ) {
		commun_single(TRUE);
	} else {
		commun_single(FALSE);
		commun_readyPacket();
	}
	if ( arg == 1 )
		cycle_set_value( c, value );
}





/* outside procedures */
command_traceToggle( m, arg, value )
menu m;
pointType arg[];
int value;
{
	
	if ( arg[value].traces == ON ) {
		arg[value].traces = OFF;
		menu_uncheck(m, value);
	} else {
		arg[value].traces = ON;
		menu_check(m, value);
	}
}
command_breakToggle( m, arg, value )
menu m;
pointType arg[];
int value;
{
	
	if ( arg[value].breaks == ON ) {
		arg[value].breaks = OFF;
		menu_uncheck(m, value);
	} else {
		arg[value].breaks = ON;
		menu_check(m, value);
	}
}

static
occurLabel( result, occurs, index )
string result;
occur occurs[];
int index;
{
	sprintf(result, "%s, %d", occurs[index].name, occurs[index].line);
}

static
otherOccurLabel( result, occurs, index )
string result;
otherOccur occurs[];
int index;
{
	sprintf(result, "%d", occurs[index].line);
}

static
treturnOccurLabel( result, occurs, index )
string result;
treturnOccur occurs[];
int index;
{
	sprintf(result, "%d", occurs[index].line);
}

static
selectOccurLabel( result, occurs, index )
string result;
selectOccur occurs[];
int index;
{
	sprintf(result, "%d", occurs[index].line);
}


menu
command_menu( occurs, number, header, labelFunc, toggleFunc, points, checked )
char *occurs;
int number;	/* of occurs */
string header;
int (*labelFunc)();
int (*toggleFunc)();
char *points;
bool checked;
{
	menu m;
	char menuLabel[100];
	int i;

	m = menu_create(cdbIO, 0, 0, header, strlen(header));
	for ( i=1; i < number; i++ ) {	/* 0 is the dummy */
		(*labelFunc)(menuLabel, occurs, i);
		menu_add(m, menuLabel, toggleFunc, (int)points);
		if ( checked )
			menu_check(m, i);
	}
	return m;
}

command_makeMenu( occurs, number, title, traceMenu, breakMenu, points, labelFunc )
char *occurs;
int number;
string title;
menu traceMenu, breakMenu;
char *points;
int (*labelFunc)();
{
	menu tm, bm;


	if ( number > 1 ) {
		tm = command_menu( occurs, number, title, labelFunc,
						command_traceToggle, points, TRUE);
		bm = command_menu( occurs, number, title, labelFunc,
						command_breakToggle, points, FALSE);
		menu_add_menu(traceMenu, title, tm);
		menu_add_menu(breakMenu, title, bm);
	}
	
}

command_processInit( pp )
process *pp;
{
	staticProcessInfo *infop;
	int i;
	menu tMenu, bMenu, m;

	infop = static_infoLookup(pp->name);

	m = menu_create(cdbIO, pp->display.row, CHANNEL+1, pp->tag, 11);
	
	tMenu = menu_create(cdbIO, 0, 0, "trace", 0);
	bMenu = menu_create(cdbIO, 0, 0, "break", 0);
	

	command_makeMenu( infop->points.accept, infop->points.numAccepts,
		"accepts", tMenu, bMenu, pp->point.accept, occurLabel);
	command_makeMenu( infop->points.create, infop->points.numCreates,
		"creates", tMenu, bMenu, pp->point.create, occurLabel);
	command_makeMenu( infop->points.transact, infop->points.numTransacts,
		"transactsions", tMenu, bMenu, pp->point.transact, occurLabel);
	command_makeMenu( infop->points.select, infop->points.numSelects,
		"selects", tMenu, bMenu, pp->point.select, selectOccurLabel);
	command_makeMenu( infop->points.delay, infop->points.numDelays,
		"delays", tMenu, bMenu, pp->point.delay, otherOccurLabel);
	command_makeMenu( infop->points.terminate, infop->points.numTerminates,
		"terminates", tMenu, bMenu, pp->point.terminate, otherOccurLabel);
	command_makeMenu( infop->points.treturn, infop->points.numTreturns,
		"treturns", tMenu, bMenu, pp->point.treturn, treturnOccurLabel);

	menu_add_menu(m, "break", bMenu);
	menu_add_menu(m, "trace", tMenu);
}

