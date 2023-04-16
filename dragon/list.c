#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* constructor/deconstructor */
list_t *make_list( char *str )
{
  list_t *p = (list_t *)malloc( sizeof(list_t) );
  assert( p != NULL );
  p->name = strdup( str );  /* potentially buggy */
  p->next = NULL;
  return p;
}

void free_list( list_t *p )
{
  // stub
  free( p->name );
  free( p );
}

void list_print( list_t *top )
{
  list_t *q = top;
  while ( q != NULL ){
    printf( "[%s] -> ", q-> name );
    q = q->next;
  }
  printf( "[]\n" );
}

/* insert to top of linked list 
 * returns new top of linked list
 */
list_t *list_insert( list_t *top, char *name )
{
  list_t *p = make_list( name );  // new node to store name

  if ( top != NULL ) // linked list is empty
    p->next = top;
  
  return top = p;
}

/* search linked list for str 
 * returns pointer to node (where str is) or NULL (otherwise)
 */
list_t *list_search( list_t *top, char *str )
{
  list_t *p = top;

  while ( p != NULL ) {
    if ( !strcmp( p->name, str ) )
      return p;
    p = p->next;
  }
  return NULL;
}