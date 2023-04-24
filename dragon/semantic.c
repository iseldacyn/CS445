#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "error.h"
#include "symtab.h"
#include "semantic.h"
#include "y.tab.h"

/* check that an identifier exists when used */
list_t *semantic_lookup( scope_t *top_scope, char *name)
{
	list_t *p = global_scope_search( top_scope, name );
	decl_check( p, name );
	return p;
}

/* set all types in identifier list */
void semantic_set_type( tree_t *id_list, int type_val )
{
	tree_t *left, *right;
	if ( id_list == NULL ) return;

	left = id_list->left;
	right = id_list->right;

	if ( left == NULL && right == NULL ) {
		id_list->attribute.sval->type = type_val;
		fprintf( stderr, "[SET_TYPE: %s, %d]\n", id_list->attribute.sval->name, id_list->attribute.sval->type );
	}
	else if ( right != NULL ){
		right->attribute.sval->type = type_val;
		fprintf( stderr, "[SET_TYPE: %s, %d]\n", right->attribute.sval->name, right->attribute.sval->type );
		semantic_set_type( left, type_val );
	}
}

/* returns number of arguments of an identifier list */
int num_args( tree_t *id_list, int args )
{
	if ( id_list == NULL ) return 0;

	args += num_args( id_list->left, args );
	args += num_args( id_list->right, args );
	args++;

	return args;
}

/* returns a range for array parsing */
int make_range( int a, int b );

/* returns the "type" of a tree */
int type_of( tree_t *t )
{
	if ( t == NULL ) return TYPE_ERROR;

	switch( t->type ) {
		case ID:
			return t->attribute.sval->type;
			break;
		case INUM:
			return INTEGRAL;
			break;
		case RNUM:
			return RATIONAL;
			break;
		case ADDOP:
			int left_type = type_of( t->left );
			int right_type = type_of( t->right );
			if ( left_type != right_type ) {
				fprintf( stderr, "ERROR: type mismatch in ADDOP.\n" );
				exit(1);
			}
			return left_type;
		default:
			return TYPE_ERROR;
			break;
	}
}
