#include <stdio.h>
/*
**
**	btree - B-tree maintenance routines
**
**   These routines are from N. Wirth, Algorithms + Data
**   Structures = Programs
**
**   Translated to C by Bob Brown, Purdue CS Department.
**
**  USAGE:
**  ------
**	These routines allow their user to define and use symbol
**	tables with different record formats and different b-tree
**	orders.	 The user should define his record format,
**	typically (though not necessarily) a structure containing
**	at least a key and value part.	Here is a typical
**	declaration:
**		struct userentry {
**			char *key;
**			int value;
**		};
**	Before any symbol table can be used, it must be defined
**	with "btnew".  Declare a generic pointer, say, "table",
**	and then call btnew, giving it the name of your routine
**	for comparing table entries and the desired B-tree order.
**
**		char *table;
**		int compare();
**		table = btnew ( 12, compare );
**
**	Now, to insert entries, build an instance of an entry,
**	insert the "key" part, and call "btinsert".  This routine
**	returns a pointer to the entry as it exists in the table.
**	Hence, if this returned pointer does not match your instance,
**	the record already existed in the table.
**
**		struct userentry *entry, *oldentry;
**		entry = (struct userentry *)malloc(sizeof (struct ...));
**		entry.key = "some value"
**		if ((oldentry = btinsert(table,entry)) == entry )
**			{ new entry in the table }
**		else
**			{ entry already existed }
**			free ( entry );
**
**	Then, to delete an entry, again build an entry record with
**	at least a key part and call btdelete:
**
**		entry.key = "value to be deleted"
**		if ( (oldentry=btdelete ( table, entry )) == NULL )
**			{  entry didn't exist  }
**		else
**			{  entry deleted, release storage  }
**			free ( oldentry );
**
**	Next, to print or traverse the table, write a routine of
**	two arguments where the first is a pointer to the userentry
**	and the second is a level.  After calling btprint, your
**	routine will be called once for each entry, in order, in
**	the table.  The second argument will be the tree level of
**	the record, starting with 0 for the root.
**
**		int print();
**		btprint ( table, print );
**
**	If you just want to search for an entry, build a template
**	for it as before and call btsearch.
**
**		if ((oldentry=btsearch(table, entry)) == NULL )
**			{ not found }
**		else
**			{ entry found, oldentry points to it. }
**
** ENTRIES:
** --------
**   Create an entirely new tree...result is a typeless pointer to
**   an object roughly denoted as a B-tree.  Declared (char *).
**
**	btnew ( order, compare )
**		+----  +------
**		|      |---> address of a procedure that can compare
**		|	     the value of two entries, a and b. It
**		|	     should return -1 if a<b; 0 if a==b;
**		|	     +1 if a>b.
**		|
**		|--------> order of the tree.  From order to 2*order
**			   entries will be stored in each node, except
**			   the root, which may contain from 1 to
**			   2*order entries.
**
**    Insert a new item into the tree.	Returns a pointer to the
**    entry in the table.  If this doesn't match "entry", then
**    the record already existed in the file.
**
**	btinsert ( table, entry )
**		   +----  +----
**		   |	  |--> pointer to a user-typed entry to be
**		   |	       inserted into the table.	 Generically
**		   |	       types as (char *) but may be any pointer
**		   |
**		   |------> the table reference, as returned by
**			    btnew when the table was created.
**			    This may be typed (char *) but is in
**			    actuality (struct btree *).
**
**    Print the contents of the tree
**
**	btprint ( table, routine )
**		  +----	 +------
**		  |	 |--> pointer to routine that is called
**		  |	      for each entry in the table.  This
**		  |	      routine should be declared as in:
**		  |	      routine ( entry, level )
**		  |	      char *entry;  { ptr to table entry }
**		  |	      int level;    { tree level: 0,1,... }
**		  |
**		  |------> the table reference, as returned by
**			   "btnew".
**
**    Delete an entry from a tree.  Returns a pointer to the in-tree
**    entry that was deleted, NULL if not found
**
**	btdelete ( table, entry )
**		   +----  +----
**		   |	  |---> pointer to user's entry.  This should
**		   |		be a skeleton table-entry with at
**		   |		least the key part significant.
**		   |
**		   |----> pointer to the tree to use, as returned
**			  by btnew.
**
**    Search for and entry in a tree.  Return pointer the record
**    in tree if found, otherwise NULL
**
**	btsearch ( table, entry )
**		   +----  +----
**		   |	  |----> pointer to skeleton entry to search
**		   |		 for.  Only the key part is necessary.
**		   |
**		   |----> pointer to table returned by btnew.
**
**    Free all storage associated with a tree.
**
**	btfree ( table, routine )
**		 +----  +------
**		 |	|-----> routine that gets called for each entry
**		 |		in the table.  It should free the user's
**		 |		record.
**		 |-----> table address.
**
*/

/*
 * TYPES
 *	btree --- basic type for a B-tree.  initialized by btnew and
 *		  "table" in this module is always a pointer to one of
 *		  these
 *	bt_page - one "index set" in the B-tree.  Contains storage
 *		  for order*2 records and order*2+1 pointers.
 *	bt_rec -- an array of these in contained in each bt_page.
 *		  Each consists of a key and a link.  bt_rec[0].bt_key
 *		  is not used in any instance of a bt_page.
 */

struct bt_rec {
	char *bt_key;		/* pointer to user entry */
	struct bt_page *bt_link;/* right-hand link */
};
struct bt_page {
	int bt_load;			/* load order..2*order */
	struct bt_rec *bt_rec;
};
struct btree {
	struct bt_page *bt_root;	/* root node in tree */
	int bt_order;			/* order of tree 1..2*order */
	int (*bt_cmp)();		/* compare routine address */
};
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*
 *  NON-INTEGER ROUTINES
 */
char *btnew(), *btinsert(), *btdelete(), *bt_pgsrc();
int strcmp(), bttstout();
int bttstpr();
struct bt_page *bt_newpage();
/*
 *=======================================
 * btnew - create and return a new tree
 *=======================================
 */

char *
btnew ( order, compare )
int order;
int (*compare)();
{
	struct btree *table;
	table = (struct btree *) malloc ( sizeof (struct btree ) );
	table->bt_order = order;
	table->bt_cmp = compare;
	table->bt_root = NULL;
	return (char *) table;
}

/*
 *=========================================
 * btinsert - insert a new entry in table
 *=========================================
 */

char *
btinsert ( table, entry )
struct btree *table;
char *entry;
{
	struct bt_rec newrec;
	struct bt_page *tmp;
	char *rtnval;
	int rebalance;
	/*
	 * Search from root of tree, return if no balancing needed
	 */
	if ( bt_pgins(table, table->bt_root, entry, &rebalance, &newrec ) )
		rtnval = newrec.bt_key;
	else
		rtnval = entry;
	if ( ! rebalance )
		return rtnval;
	/*
	 * need to split root - make new one, set up first entry
	 */
	tmp = bt_newpage(table);
	tmp->bt_load = 1;
	tmp->bt_rec[0].bt_link = table->bt_root;
	tmp->bt_rec[1] = newrec;
	table->bt_root = tmp;
	return rtnval;
}

/*
 *=============================================
 * btsearch - search for an element in a tree
 *=============================================
 */

char *
btsearch ( table, entry )
struct btree *table;
char *entry;
{
	return bt_pgsrc ( table, entry, table->bt_root ) ;
}

/*
 *=========================================
 *  bt_pgsrc - search a page for an entry
 *=========================================
 */

char *
bt_pgsrc ( table, entry, curpage )
struct btree *table;
char *entry;
struct bt_page *curpage;
{
	int pos;
	while ( curpage != NULL ) {
		if ( bt_binsrc ( table, entry, curpage, &pos) )
			return curpage->bt_rec[pos].bt_key;
		curpage = curpage->bt_rec[pos].bt_link;
	}
	return NULL;
}
/*
 *=====================================
 * btdelete - external delete routine
 *=====================================
 *
 * returns a pointer to the entry thaat was removed
 */

char *
btdelete ( table, entry )
struct btree *table;
char *entry;
{
	int reduce;
	struct bt_page *tmp;
	struct bt_rec oldrec;
	bt_pgdel ( table, table->bt_root, entry, &reduce, &oldrec);
	if ( table->bt_root->bt_load == 0 ) {
		tmp = table->bt_root;
		table->bt_root = tmp->bt_rec[0].bt_link;
		bt_freenode ( tmp );
	}
	return oldrec.bt_key ;
}
/*
 *============================================
 * bt_pgins - internal recursive page search
 *============================================
 *
 * returns FALSE if record not found here or below, else TRUE
 */

bt_pgins ( table, curpage, entry, rebalance, newrec )

struct btree *table;		/*  IN: table pointer		*/
struct bt_page *curpage;	/*  IN: ptr to page to search	*/
char *entry;			/*  IN: ptr to entry to find	*/
int *rebalance;			/* OUT: FALSE0 => must rebalance*/
struct bt_rec *newrec;		/* OUT: record to move up or	*/
				/*	*.key if found record	*/
{
	struct bt_rec tmprec, belowrec;
	struct bt_page *newpage;
	int right, i, frombelow;
	*rebalance = FALSE;
	if ( curpage == NULL ) {
		*rebalance = TRUE;
		newrec->bt_key = entry;
		newrec->bt_link = NULL;
		return FALSE;
	}
	/*
	 * search this page - if found, save and return
	 */
	if ( bt_binsrc ( table, entry, curpage, &right) ) {
		*newrec = curpage->bt_rec[right];
		return TRUE;
	}
	/*
	 * not in this page, search down, return if found
	 */
	if ( bt_pgins ( table, curpage->bt_rec[right].bt_link, entry,
	    &frombelow, &belowrec ) ) {
		*newrec = belowrec;
		return TRUE;
	}
	/*
	 * if inserting and rebalancing needed - insert new node
	 */
	if ( frombelow ) {
		/*
		 * check if room in this page...
		 */
		if ( curpage->bt_load < table->bt_order*2 ) {
			curpage->bt_load++;
			*rebalance = FALSE;
			for ( i=curpage->bt_load ; i>=right+2 ; i-- )
				curpage->bt_rec[i] = curpage->bt_rec[i-1];
			curpage->bt_rec[right+1] = belowrec;
		} else {
			/*
			 * page is full - determine which half to put new one in.
			 */
			newpage = bt_newpage(table);
			if ( right <= table->bt_order ) { /* insert in left page */
				if ( right == table->bt_order )
					tmprec = belowrec;
				else {
					tmprec = curpage->bt_rec[table->bt_order];
					for ( i=table->bt_order ; i>=right+2 ; i-- )
						curpage->bt_rec[i] = curpage->bt_rec[i-1];
					curpage->bt_rec[right+1] = belowrec;
				}
				for ( i=1 ; i<=table->bt_order ; i++ )
					newpage->bt_rec[i] = curpage->bt_rec[i+table->bt_order];
			} else { /* insert in right page */
				right -= table->bt_order;
				tmprec = curpage->bt_rec[table->bt_order+1];
				for ( i=1 ; i<right ; i++ )
					newpage->bt_rec[i] = curpage->bt_rec[i+table->bt_order+1];
				newpage->bt_rec[right] = belowrec;
				for ( i=right+1 ; i<=table->bt_order ; i++ )
					newpage->bt_rec[i] = curpage->bt_rec[i+table->bt_order];
			}
			curpage->bt_load = newpage->bt_load = table->bt_order;
			newpage->bt_rec[0].bt_link = tmprec.bt_link;
			tmprec.bt_link = newpage;
			*rebalance = TRUE;
			*newrec = tmprec;
		}
	}
	return FALSE;
}

/*
 *===================================================
 * bt_binsrc - internal binary search ovey one page
 *===================================================
 */
bt_binsrc ( table, entry, page, result )
struct btree *table;
struct bt_page *page;
char *entry;
int *result;
{
	int left, right, mid, cmp;
	left = 1;
	right = page->bt_load;
	do {
		mid = (right+left)/2;
		cmp = (*table->bt_cmp)(entry,(char *)page->bt_rec[mid].bt_key);
		if ( cmp == 0 ) {
			*result = mid;
			return TRUE;
		} else if (cmp < 0 )		/* left <= entry < mid */
			right = mid-1;
		else				/* mid < entry <= right */
			left = mid+1;
	} while ( left <= right );
	*result = right;
	return FALSE;
}

/*
 *===================================================================
 * bt_newpage - allocate a new page (index set) from dynamic memory
 *===================================================================
 */
struct bt_page *
bt_newpage(table)
struct btree *table;
{
	struct bt_page *page;
	int i;
	page = (struct bt_page *) malloc ( sizeof (struct bt_page) );
	page->bt_load = 0;
	page->bt_rec = (struct bt_rec *)calloc(table->bt_order*2+1, sizeof (struct bt_rec) );
	for ( i=0 ; i<table->bt_order*2+1 ; i++ )
		page->bt_rec[i].bt_link = NULL;
	return page;
}

/*
 *================================================
 * bt_freenode - free storage associated with a page
 *================================================
 */

bt_freenode ( page )
struct bt_page *page;
{
	free ( page->bt_rec );
	free ( page );
}
/*
 *===============================
 * btestpr - test print routine
 *===============================
 */

btestpr ( table , routine)
struct btree *table;
int (*routine)();
{
	bt_estpr ( table, table->bt_root, routine, 0 );
}

bt_estpr ( table, page, routine, level )
struct btree *table;
struct bt_page *page;
int (*routine)();
int level;
{
	int i;
	if ( page == NULL )
		return;
	for ( i=0 ; i<level ; i++ )
		printf("  ");
	for ( i=1 ; i<=page->bt_load ; i++ )
		(*routine)(page->bt_rec[i].bt_key, level );
	printf("\n");
	for ( i=0 ; i<=page->bt_load ; i++ )
		bt_estpr ( table, page->bt_rec[i].bt_link, routine, level+1);
}

/*
 *========================================
 *  bt_pgdel - internal delete from page
 *========================================
 */

bt_pgdel ( table, curpage, entry, reduce, oldrec )
struct btree *table;
struct bt_page *curpage;
struct bt_rec *oldrec;
char *entry;
int *reduce;
{
	struct bt_page *less;
	int i, right;
	oldrec->bt_key = NULL;
	if ( curpage == NULL ) {
		*reduce = FALSE;
		return FALSE;
	}
	if ( bt_binsrc ( table, entry, curpage, &right )) { /* found=>delete */
		oldrec->bt_key = curpage->bt_rec[right].bt_key;
		less = curpage->bt_rec[right-1].bt_link;
		if ( less == NULL ) { /* curpage is a leaf node */
			*reduce = --curpage->bt_load < table->bt_order;
			for ( i=right ; i<=curpage->bt_load ; i++ )
				curpage->bt_rec[i] = curpage->bt_rec[i+1];
		} else { /* not leaf, search for leaf */
			bt_del ( table, curpage, less, right, reduce );
			if ( *reduce )
				bt_under ( table, curpage, less, right-1, reduce );
		}
	} else { /* not found => go down a level */
		less = curpage->bt_rec[right].bt_link;
		bt_pgdel ( table, less, entry, reduce, oldrec );
		if ( *reduce )
			bt_under ( table, curpage, less, right, reduce );
	}
}
/*
 *==============================================
 *  bt_del - internal recursive delete routine
 *==============================================
 */
bt_del ( table, curpage, delpage, item, reduce )
struct btree *table;
struct bt_page *curpage;
struct bt_page *delpage;
int *reduce, item;
{
	struct bt_page *nextpage;
	if ( (nextpage=delpage->bt_rec[delpage->bt_load].bt_link) != NULL ) {
		bt_del ( table, curpage, nextpage, item, reduce );
		if ( *reduce )
			bt_under ( table, delpage, nextpage, delpage->bt_load, reduce );
	} else {
		curpage->bt_rec[item].bt_key = delpage->bt_rec[delpage->bt_load].bt_key;
		*reduce = --delpage->bt_load < table->bt_order;
	}
}

/*
 *==========================================
 *  bt_under - handle underflows in a page
 *==========================================
 */
bt_under ( table, parent, child, item, reduce )
struct btree *table;
struct bt_page *parent;
struct bt_page *child;
int item;
int *reduce;
{
	int parload, i;
	struct bt_page *rtpage, *lfpage;
	int rtload, lfload;
	int rtavail, lfavail;
	parload = parent->bt_load;
	if ( item < parload ) {
		item++;
		rtpage = parent->bt_rec[item].bt_link;
		rtload = rtpage->bt_load;
		rtavail = ( rtload-table->bt_order+1 ) / 2;
		child->bt_rec[table->bt_order] = parent->bt_rec[item];
		child->bt_rec[table->bt_order].bt_link = rtpage->bt_rec[0].bt_link;
		if ( rtavail > 0 ) { /* move "rtavail" items from rtpage
					to child */
			for ( i=1 ; i<=rtavail-1 ; i++ )
				child->bt_rec[i+table->bt_order] = rtpage->bt_rec[i];
			parent->bt_rec[item] = rtpage->bt_rec[rtavail];
			parent->bt_rec[item].bt_link = rtpage;
			rtpage->bt_rec[0].bt_link = rtpage->bt_rec[rtavail].bt_link;
			rtload -= rtavail;
			for ( i=1 ; i<=rtload ; i++ )
				rtpage->bt_rec[i] = rtpage->bt_rec[i+rtavail];
			rtpage->bt_load = rtload;
			child->bt_load = table->bt_order-1+rtavail;
			*reduce = FALSE;
		} else { /* merge pages "child" and "rtpage" */
			for ( i=1 ; i<=table->bt_order ; i++ )
				child->bt_rec[i+table->bt_order] = rtpage->bt_rec[i];
			for ( i=item ; i<=parload-1 ; i++ )
				parent->bt_rec[i] = parent->bt_rec[i+1];
			child->bt_load = table->bt_order*2;
			parent->bt_load = parload-1;
			bt_freenode ( rtpage );
		}
	} else {
		lfpage = parent->bt_rec[item-1].bt_link;
		lfload = lfpage->bt_load+1;
		lfavail = (lfload-table->bt_order) / 2;
		if ( lfavail > 0 ) { /* move lfavail items from page
					lfpage to child */
			for ( i=table->bt_order ; i>=1 ; i-- )
				child->bt_rec[i+lfavail] = child->bt_rec[i];
			child->bt_rec[lfavail] = parent->bt_rec[item];
			child->bt_rec[lfavail].bt_link = child->bt_rec[0].bt_link;
			lfload -= lfavail;
			for ( i=lfavail-1 ; i>=0 ; i-- )
				child->bt_rec[i] = lfpage->bt_rec[i+lfload];
			parent->bt_rec[item] = lfpage->bt_rec[lfload];
			parent->bt_rec[item].bt_link = child;
			lfpage->bt_load = lfload-1;
			child->bt_load = table->bt_order-1+lfavail;
			*reduce = 0;
		} else { /* merge pages "child" and "lfpage" */
			lfpage->bt_rec[lfload] = parent->bt_rec[item];
			lfpage->bt_rec[lfload].bt_link = child->bt_rec[0].bt_link;
			for ( i=1 ; i<table->bt_order ; i++ )
				lfpage->bt_rec[i+lfload] = child->bt_rec[i];
			lfpage->bt_load = table->bt_order * 2;
			parent->bt_load = parload-1;
			bt_freenode ( child );
		}
	}
}
/*
 *=================================================
 * btprint - process (print) contents of a B-tree
 *=================================================
 *
 *  routine - pointer to user's routine to process each entry
 */
btprint(table, routine)
struct btree *table;
int (*routine)();
{
	bt_print(table, table->bt_root, routine, 0);
}

/*
 *=========================================================
 * bt_print - internal recursive version of print routine
 *=========================================================
 */

bt_print ( table, page, routine, level )
struct btree *table;
struct bt_page *page;
int (*routine)();
int level;
{
	int i;
	if ( page == NULL )
		return;
	bt_print(table, page->bt_rec[0].bt_link, routine, level+1 );
	for ( i=1 ; i<=page->bt_load ; i++ ) {
		(*routine) (page->bt_rec[i].bt_key, level);
		bt_print(table, page->bt_rec[i].bt_link, routine, level+1 );
	}
}
/*
**===================================
** btfree - free up an entire table
**===================================
*/

btfree(table, routine)
struct btree *table;
int (*routine)();
{
	bt_free(table, table->bt_root, routine);
}

/*
**==========================================================
** bt_free - internal recursive version of destroy routine
**==========================================================
*/
bt_free ( table, page, routine)
struct btree *table;
struct bt_page *page;
int (*routine)();
{
	int i;
	if ( page == NULL )
		return;
	bt_free(table, page->bt_rec[0].bt_link, routine);
	for ( i=1 ; i<=page->bt_load ; i++ ) {
		(*routine) (page->bt_rec[i].bt_key);
		bt_free(table, page->bt_rec[i].bt_link, routine);
	}
	bt_freenode(page);
}
#ifdef TEST
main(argc,argv)
char *argv[];
{
	btest((argc==1?40:atoi(argv[1])));
}
/*
 *===========================================
 * btest - test program for B-tree routines
 *===========================================
 */
#include <sys/types.h>
#include <sys/times.h>

btest(order)
int order;
{
	char *table, buf[80], *word, cmd;
	int count, timemod;

	table = btnew(order, strcmp);
	count = 0;
	timemod = 0;
	if ( isatty(0) ) printf(">> ");
	while ( gets(buf) != NULL ) {
		word = (char *)malloc(strlen(buf));
		sscanf ( buf, "%c %s",&cmd, word);
		switch ( cmd ) {
		case 's':
			if ( btsearch ( table, word ) == NULL )
				printf ( "not found\n" );
			else
				printf ( "found\n" );
			break;
		case 'p':
			btestpr ( table, bttstout );
			break;
		case 'a':
		case 'i':
			btinsert(table, word);
			break;
		case 'r':
		case 'd':
			btdelete ( table, word );
			break;
		case 'q':
			btprint(table,bttstpr);
			exit(0);
		case 't':
			timemod = atoi(word);
			break;
		default:
			fprintf(stderr, "INvalid command\n");
		}
		if ( timemod>0 && (count%timemod)==0 ) {
			struct tms tbuf;
			times(&tbuf);
			printf("%D\n",tbuf.tms_utime);
		}
		count++;
		if ( isatty(0) ) printf(">> ");
	}
}

bttstout(s,l)
char *s;
{
	printf("%s ",s);
}
bttstpr(s,l)
char *s;
{
	printf("%s\n",s);
}

#endif
