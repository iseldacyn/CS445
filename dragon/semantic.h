#ifndef SEMANTIC_H
#define SEMANTIC_H

#define TYPE_ERROR -2

void needs_assignop( tree_t *, tree_t * );
list_t *semantic_lookup( scope_t *, char * );
void semantic_set_type( tree_t *, int );
void check_args( list_t *, list_t * );
list_t *make_list_from_tree( tree_t * );
int type_of( tree_t * );

#endif
