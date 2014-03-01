/*----------------------------------------------------------------------*/
/*									*/
/*	btree.h - B-tree maintenance routines				*/
/*									*/
/*   These routines are from N. Wirth, Algorithms + Data		*/
/*   Structures = Programs						*/
/*									*/
/*   Translated to C by Bob Brown, Purdue CS Department.                */
/*									*/
/*----------------------------------------------------------------------*/

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
