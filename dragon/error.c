#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "error.h"

/* checks type of two object */
void type_check( int type_one, int type_two )
{
	if ( type_one == type_two ) return;

	fprintf( stderr, "\nERROR: could not match type %d to %d.\n", type_one, type_two );
	exit(1);
}

/* checks if variable already declared */
void double_decl( scope_t *t, char *name )
{
	list_t *p = scope_search( t, name );

	if ( p != NULL ) {
		fprintf( stderr, "\nERROR: name(%s) already declared in scope.\n", name );
		exit(1);
	}
	return;
}

/* checks if variable has been declared */
void decl_check( list_t *p, char *name )
{
	if ( p != NULL ) return;

	fprintf( stderr, "\nERROR: name(%s) used but never declared.\n", name );
	exit(1);
}

/* prints error if assignop */
void check_assignop( int boolean )
{
	if ( boolean == 0 ) {
		fprintf( stderr, "\nERROR: function contains no return statement!\n" );
		exit(1);
	}
	else
		return;
}

/* checks if range is correct made */
void check_range( int a, int b )
{
	if ( b <= a ) {
		fprintf( stderr, "\nERROR: cannot set range of length(%d) from (%d) to (%d)\n", b-a, a, b );
		exit(1);
	}
	else
		return;
}

/* prints args error type */
void wrong_args( list_t *l1, list_t *l2, char error_type )
{
	switch (error_type) {
		case 'i':
			fprintf( stderr, "\nERROR: function call contains too many args!\n" );
			break;
		case 'c':
			fprintf( stderr, "\nERROR: function call contains too few args!\n" );
			break;
		case 't':
			fprintf( stderr, "\nERROR: could not match type of arg{%d} to type of arg{%d}\n", l1->type, l2->type );
			break;
		default:
			fprintf( stderr, "Incorrect call to args function\n" );
	}
	exit(1);
}
