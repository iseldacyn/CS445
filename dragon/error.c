#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "error.h"

void type_check( int type_one, int type_two )
{
	if ( type_one == type_two ) return;

	fprintf( stderr, "\nERROR: could not match type %d to %d.\n", type_one, type_two );
	exit(1);
}

void double_decl( scope_t *t, char *name )
{
	list_t *p = scope_search( t, name );

	if ( p != NULL ) {
		fprintf( stderr, "\nERROR: name(%s) already declared in scope.\n", name );
		exit(1);
	}
	return;
}

void decl_check( list_t *p, char *name )
{
	if ( p != NULL ) return;

	fprintf( stderr, "\nERROR: name(%s) used but never declared.\n", name );
	exit(1);
}
