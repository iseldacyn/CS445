#ifndef SYMTAB_H
#define SYMTAB_H

#include "list.h"

#define HASH_SIZE  211
#define EOS        '\0'

typedef struct scope_s{  /* linkable scope */
	list_t *table[HASH_SIZE];  /* hash table */
	struct scope_s *next;
} scope_t;

/* constructor/destructor */
scope_t *make_scope();
void free_scope( scope_t * );

/* stack operations */
scope_t *scope_push( scope_t * );  /* push a new scope on to the stack */
scope_t *scope_pop( scope_t * );  /* pop top off the stack */

/* search/insert */
list_t *scope_insert( scope_t *, char * );  /* insert name into top scope */
list_t *scope_search( scope_t *, char * );  /* search name in top scope */
list_t *global_scope_search( scope_t *, char * );  /* search name in all scopes (starting in top scope) */

int hashpjw( char * );

#endif
