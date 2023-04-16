#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

void aux_print_tree( tree_t *, int );

///////////////////////////////////////////////////////////////

/* basic constructor */
tree_t *make_tree( int type, tree_t *l, tree_t *r )
{
	tree_t *p = (tree_t *)malloc( sizeof(tree_t) );
	assert( p != NULL );
	p->type = type;
	p->left = l;
	p->right = r;
	return p;
}

tree_t *make_id( list_t * )
{
	tree_t *p = make_tree( ID, NULL, NULL );
	p->attribute.sval = name;
	return p;
}

tree_t *make_inum( int val )
{
	tree_t *p = make_tree( INUM, NULL, NULL );
	p->attribute.ival = val;
	return p;
}

tree_t *make_rnum( float val )
{
	tree_t *p = make_tree( RNUM, NULL, NULL );
	p->attribute.rval = val;
	return p;
}

///////////////////////////////////////////////////////////////

/* pre-order tree traversal */
void print_tree( tree_t *t )
{
	aux_print_tree( t, 0 );
}

void aux_print_tree( tree_t *t, int spaces )
{
	if ( t == NULL ) return;

	/* indenting for visual effects */
	for (int i=0; i<spaces; i++)
		fprintf( stderr, " " );

	switch( t->type ) {
	case ADDOP:
		fprintf( stderr, "[ADDOP:%d]\n", t->attribute.opval );
		aux_print_tree( t->left, spaces+4 );
		aux_print_tree( t->right, spaces+4 );
		break;
	case MULOP:
		fprintf( stderr, "[MULOP:%d]\n", t->attribute.opval );
		aux_print_tree( t->left, spaces+4 );
		aux_print_tree( t->right, spaces+4 );
		break;
	case RELOP:
		fprintf( stderr, "[RELOP:%d]\n", t->attribute.opval );
		aux_print_tree( t->left, spaces+4 );
		aux_print_tree( t->right, spaces+4 );
		break;
	case ID:
		fprintf( stderr, "[ID:%s]\n", t->attribute.sval );
		break;
	case INUM:
		fprintf( stderr, "[INUM:%d]\n", t->attribute.ival );
		break;
	case RNUM:
		fprintf( stderr, "[RNUM:%.2f]\n", t->attribute.rval );
		break;
	default:
		fprintf( stderr, "Error: print_tree, unknown type\n" );
		exit(1);
	}
}


