/*---------------------------------------------------------------------------
 * These routines maintain the static tables for cdb.  The major static
 * table is the symbol table.  The others are all occurrence tables, which
 * are simply recieved in order from DD_commun an indexed by the
 * occurrence id numbers.  The symbol table must be accesible to allow cdb
 * to display such things as structure fields on the screen.  To this end,
 * a btree is created for the symbol table indexed by namespace and name.
 * For each namespace, there is an entry DD_first, which contains the
 * offset into the symbolTable array of the first entry for that
 * namespace.  This allows sequential accessing of entries inside a
 * namespace by looking up DD_first, and then accessing directly from the
 * array.
 * 
 * William L. Lynch		Mon Aug 11 11:51:49 1986
 *---------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdio.h>
#include "cdb.h"
#include "../btree.h"
#include "../objs.h"
#include "static.h"

#define bit(x)	(1 << (x))

extern sock std, err, cdbs;

static symbol mainSymbol = {"main",0,0,0,0};

symbol *symbolTree;

symbol *symbolTable;
int symbols;

static staticProcessInfo *infoTree;
static staticProcessInfo *infoArray;

int
symCmp(s1, s2)
symbol *s1, *s2;
{
	int result;

	result = s1->space - s2->space;
	if ( result )
		return result;
	return strcmp( s1-> name, s2->name);
}

int
infoCmp( i1, i2 )
staticProcessInfo *i1, *i2;
{
	return strcmp( i1->name, i2->name );
}

static_init()
{
	int i, last = -1;
	symbol *first;
	

	infoTree = (staticProcessInfo *)btnew(10, infoCmp);
	symbolTree = (symbol *)btnew(10, symCmp);

	static_readTables();

	btinsert(symbolTree, &mainSymbol);

	for ( i=0; i < symbols; i++ ) {
		if ( symbolTable[i].space != last ) {
			first = instance(symbol);
			first->name = "DD_first";
			first->space = symbolTable[i].space;
			first->num = i;
			btinsert( symbolTree, first);
			last = symbolTable[i].space;
		}
		btinsert( symbolTree, &symbolTable[i]);
	}
}



symbol *
static_lookup( name, space )
string name;
int space;
{
	symbol *key, *result;
	
	key = instance(symbol);
	key->name = name;
	key->space = space;
	result = (symbol *)btinsert(symbolTree, key);
	if ( result == key ) {
		cdbStatus("Can't find symbol %s(%d)", name, space);
		strcpy(key->name, name);
	} else 
		free(key);
	return result;
}

staticProcessInfo *
static_infoLookup( name )
string name;
{
	staticProcessInfo *key, *spip;
	
	key = instance(staticProcessInfo);
	key->name = name;
	spip = (staticProcessInfo *)btinsert(infoTree, key);
	if ( spip == key ) {
		cdbStatus("Can't find info for %s.", name);
		strcpy(key->name, name);
	} else 
		free(key);
	return spip;
}

symbol *
static_symbol( index )
int index;
{
	return &symbolTable[index];
}


int
static_readTable( table, readFunc, size )
char **table;
int (*readFunc)();
int size;
{
	int numObjects, i;
	
	packet_recvSize(&numObjects);
	*table = (char *)malloc(size * numObjects);
	for ( i=0; i < numObjects; i++ )
		(*readFunc)(*table + size * i);
	return numObjects;
}


int packet_recvSymbol(), packet_recvOccur(), packet_recvSelectOccur(), 
	packet_recvOtherOccur(), packet_recvTreturnOccur();

static_readTables()
{
	int ready;
	char buf[80];
	int cc, i;
	int numberProcessInfo;
		
	do {
		ready = bit(err) | bit(cdbs) | bit(0) | bit(std);
		select(16, &ready, 0, 0, 0);		
		if ( ready & bit(err) ) {
			cc = read(err, buf, sizeof buf);
			userErr(buf, cc);
		}
		if ( ready & bit(std) ) {
			cc = read(std, buf, sizeof buf);
			userOut(buf, cc);
		}
		if ( ready & bit(0) )
			parse();
	} while ( !(ready & bit(cdbs)) );

	cdbStatus("\nReading symbolic information...");

	symbols = static_readTable(&symbolTable, packet_recvSymbol, sizeof(symbol));

	packet_recvSize(&numberProcessInfo);
	infoArray = instances(numberProcessInfo, staticProcessInfo);

	for ( i=0; i < numberProcessInfo; i++ ) {
		packet_recvName(&infoArray[i].name);
		infoArray[i].tagId = 0;
		static_readPoints(&infoArray[i].points);
		btinsert(infoTree, &infoArray[i]);
	}
	cdbStatus("Done");
}



static_readPoints( points )	
pointsInfo *points;
{
	points->numAccepts = static_readTable(&points->accept,
				packet_recvOccur, sizeof(occur));
	points->numCreates = static_readTable(&points->create,
				packet_recvOccur, sizeof(occur));
	points->numTransacts = static_readTable(&points->transact,
				packet_recvOccur, sizeof(occur));
	points->numSelects = static_readTable(&points->select, 
				packet_recvSelectOccur, sizeof(selectOccur));
	points->numDelays = static_readTable(&points->delay, 
				packet_recvOtherOccur, sizeof(otherOccur));
	points->numTerminates = static_readTable(&points->terminate, 
				packet_recvOtherOccur, sizeof(otherOccur));
	points->numTreturns = static_readTable(&points->treturn, 
				packet_recvTreturnOccur, sizeof(treturnOccur));
	
}



