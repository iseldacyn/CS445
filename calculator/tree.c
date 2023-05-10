#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "const.h"

void aux_print_tree( tree_t *, int );

/* basic constructor */
tree_t *make_tree( int type, int attribute, tree_t *l , tree_t *r ) 
{
  tree_t *p = (tree_t *)malloc( sizeof(tree_t) );
  assert( p != NULL );

  p->type = type;
  p->attribute = attribute;
  p->left = l;
  if (l != NULL)
  	p->left->side = 'l';
  p->right = r;
  if (r!= NULL)
  	p->right->side = 'r';
  
  return p;
}

/* basic deconstructor */
void free_tree( tree_t *t)
{
  if( t == NULL )
    return;

  free_tree( t->left );
  free_tree( t->right );
  free( t );
}

/* pre-order tree traversal */
void print_tree(tree_t *t) 
{
  aux_print_tree(t, 0);
}

void aux_print_tree(tree_t *t, int spaces)
{
  if( t == NULL ) return;

  /* indenting for visual effects */
  for(int i=0; i<spaces; i++)
    fprintf(stderr, " ");

  switch(t->type) {
    case ADDOP:
      fprintf( stderr, "[ADDOP:%c]\n", t->attribute );
      aux_print_tree( t->left, spaces+4 );
      aux_print_tree( t->right, spaces+4 );
      break;
    case MULOP:
      fprintf( stderr, "[MULOP:%c]\n", t->attribute );
      aux_print_tree( t->left, spaces+4 );
      aux_print_tree( t->right, spaces+4 );
      break;
    case NUM:
      fprintf( stderr, "[NUM:%d]\n", t->attribute );
      break;
    default:
      fprintf( stderr, "ERROR: print_tree, unknown type\n" );
      exit(1);
  }
}

/* evaluate using similar routine to print_tree() */
float eval_tree(tree_t *t )
{
  float value, div;

  switch(t->type) {
    case ADDOP:
      if ( t->attribute == '+' ) value = eval_tree( t->left ) + eval_tree( t->right );
      if ( t->attribute == '-' ) value = eval_tree( t->left ) - eval_tree( t-> right );
      break;
    case MULOP:
      if ( t->attribute == '*' ) value = eval_tree( t->left ) * eval_tree( t->right );
      if ( t->attribute == '/' ) {
        div = eval_tree( t-> right );
        assert(div);
        value = eval_tree( t->left ) / div;
      }
      break;
    case NUM:
      return t->attribute;
      break;
    default:
      fprintf( stderr, "ERROR: eval_tree, unknown type\n" );
      exit(1);

    return value;
  }
}
