#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "list.h"
#include "error.h"
#include "symtab.h"
#include "semantic.h"
#include "y.tab.h"

/* checks for assignop in function declaraion */
void needs_assignop( tree_t *func, tree_t *def )
{
	if ( func->type == PROCEDURE_DEFINITION )
		return;
	
	char *func_name = func->left->attribute.sval->name;
	int has_assignop = search_tree_for_assignop( def, func_name );
	check_assignop( has_assignop );
}

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

/* checks if there are correct number of args, and correct type */
void check_args( list_t *id_list, list_t *call_list )
{
	if ( (id_list == NULL) && (call_list == NULL) ) return;
	else if ( (id_list == NULL) && (call_list != NULL) ) wrong_args( id_list, call_list, 'i' );
	else if ( (id_list != NULL) && (call_list == NULL) ) wrong_args( id_list, call_list, 'c' );
	else if ( id_list->type != call_list->type ) wrong_args( id_list, call_list, 't');
	else check_args( id_list->next, call_list->next );
}

/* helper function for check_args
 * takes an argument tree and returns a list of arguments */
list_t *make_list_from_tree( tree_t *t )
{
	if ( t == NULL ) return NULL;
	
	list_t *p = (list_t *)malloc( sizeof(list_t) );
	assert( p != NULL );

	if( type_of(t->right) == LIST ) {
		p = make_list_from_tree(t->right);
		p = list_combine( p, make_list_from_tree(t->left) );
	}
	else if( type_of(t) == LIST ) {
		p->type = type_of(t->right);
		p = list_combine( p, make_list_from_tree(t->left) );
	}
	else
		p->type = type_of(t);

	return p;
}

/* returns a range for array parsing */
int make_range( int a, int b );

/* returns the "type" of a tree */
int type_of( tree_t *t )
{
	if ( t == NULL ) return TYPE_ERROR;
	int left_type, right_type;

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
			left_type = type_of( t->left );
			right_type = type_of( t->right );
			type_check( left_type, right_type );
			return left_type;
			break;
		case MULOP:
			left_type = type_of( t->left );
			right_type = type_of( t->right );
			type_check( left_type, right_type );
			return left_type;
			break;
		case RELOP:
			left_type = type_of( t->left );
			right_type = type_of( t->right );
			type_check( left_type, right_type );
			return left_type;
			break;
		case LIST:
			return LIST;
			break;
		default:
			return TYPE_ERROR;
			break;
	}
}
