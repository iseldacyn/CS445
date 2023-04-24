#ifndef SEMANTIC_H
#define SEMANTIC_H

#define TYPE_ERROR -2

list_t *semantic_lookup( scope_t *, char * );
void semantic_set_type( tree_t *, int );
int num_args( tree_t *, int );
int type_of( tree_t * );

#endif
