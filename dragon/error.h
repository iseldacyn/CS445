#ifndef ERROR_H
#define ERROR_H

#include "tree.h"
#include "symtab.h"

void type_check( int, int );
void double_decl( scope_t *, char * );
void decl_check( list_t *, char * );

#endif

