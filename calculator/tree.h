#ifndef TREE_H
#define TREE_H

typedef struct tree_s {
  int type;       /* token types: ADDOP, MULOP, NUM */
  int attribute;  /* token attribute */
  struct tree_s *left;
  struct tree_s *right;
} tree_t;

tree_t *make_tree( int , int , tree_t *, tree_t *);
void free_tree( tree_t *);
void print_tree( tree_t *);
float eval_tree( tree_t *);

#endif