#ifndef TREE_H
#define TREE_H

typedef struct tree_s {
	int type;			/*  */
	union {
		int ival; 	/* INUM */
		float rval; /* RNUM */
		char *sval; /* ID */
		int opval;  /* ADDOP, MULOP, RELOP */
	} attribute;
	
	struct tree_s *left;
	struct tree_s *right;
} tree_t;

tree_t *make_tree( int v, tree_t *l, tree_t *r );
tree_t *make_id( char * );
tree_t *make_inum( int );
tree_t *make_rnum( float );
void print_tree( tree_t *t );

#endif


