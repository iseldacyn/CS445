#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "codegen.h"

void aux_gencode( tree_t *, char * );

/* temp for register stack */
typedef struct stack_s {
	char *name;
	struct stack_s *next;
} stack_t;

stack_t *register_stack()
{
	stack_t *r = (stack_t *)malloc( sizeof(stack_t) );
	assert(r != NULL);
	r->next = (stack_t *)malloc( sizeof(stack_t) );
	assert(r->next != NULL);
	r->next->next = (stack_t *)malloc( sizeof(stack_t) );
	assert(r->next->next != NULL);
	r->name = "R0";
	r->next->name = "R1";
	r->next->next->name = "R2";
	r->next->next->next = NULL;
	return r;
}

stack_t *make_stack()
{
	stack_t *r = (stack_t *)malloc( sizeof(stack_t) );
	assert(r != NULL);
	r->next = NULL;
	return r;
}

/* register stack with two registers */
stack_t *reg_stack;

stack_t *pop( stack_t *r )
{
	if (r != NULL) {
		stack_t *tmp;
		tmp = r->next;
		return tmp;
	} else return NULL;
}

void push( stack_t *curr, stack_t *dest )
{
	if ( dest != NULL )
		curr -> next = dest;
	dest = curr;
}

char *top(stack_t *r)
{
	return r->name;
}

int max_free_registers( stack_t *r, int n)
{
	if(r==NULL) return n;
	else {
		n = max_free_registers( r->next, n );
		return n+1;
	}
}

stack_t *swap(stack_t *r)
{
	stack_t *tmp = r->next;
	r->next = tmp->next;
	tmp -> next = r;
	return tmp;
}


/* rank of tree */
int rank( tree_t *t )
{
	if ( t == NULL ) return -1;
	int rank_left = rank(t->left);
	int rank_right = rank(t->right);

	if ( (rank_left == -1) && (rank_right == -1) ){
		if ( t->side == 'l' )
			return 1;
		else return 0;
	}
	else if ( rank_left != rank_right )
		return rank_left < rank_right ? rank_right : rank_left;
	else return rank_left + 1;
}

/* codegen for directed acyclic trees */
void gencode( tree_t *t, char *file )
{
	reg_stack = register_stack();
	aux_gencode( t, file );
}

void aux_gencode( tree_t *n, char *file )
{
	FILE *fp = fopen( file, "a" );

	/* case 0 */
	if ( ((n->left == NULL) && (n->right == NULL)) && (n->side == 'l') )
	{
		fprintf( stderr, "left leaf" );
		fprintf( fp, "mov %d %s\n", n->attribute, top(reg_stack) );
	}
	else if ( n->type != NUM ) {
		char *op;
		switch (n->attribute) {
			case '+': op = "add"; break;
			case '-': op = "sub"; break;
			case '*': op = "mul"; break;
			case '/': op = "div"; break;
			default:
				fprintf( stderr, "Unknown operand %d", n->attribute ); exit(1);
		}
		int rank_right = rank(n->right);
		int rank_left = rank(n->left);
		int r = max_free_registers(reg_stack, 0);
		fprintf( stderr, "left rank: %d ; right rank: %d ; registers: %d \n", rank_left, rank_right, r );
		/* case 1 */
		if ( rank_right == 0 ) {
			aux_gencode( n->left, file );
			fprintf( fp, "%s %d %s\n", op, n->right->attribute, top(reg_stack) );
		}
		/* case 2 */
		else if ( ((1 <= rank_left) && ( rank_left < rank_right)) && ( rank_left < r )) {
			reg_stack = swap(reg_stack);
			fprintf( stderr, "goingleft...\n");
			aux_gencode(n->right, file);
			stack_t *R = pop(reg_stack);
		fprintf( stderr, "%s", top(reg_stack));
			fprintf( stderr, "goingright...\n");
			aux_gencode(n->left, file);
			fprintf( fp, "%s %s %s\n", op, top(reg_stack), top(R) );
			push( R, reg_stack );
			fprintf( stderr, "%s", top(reg_stack));
			swap( reg_stack );
			fprintf( stderr, "%s", top(reg_stack));
		}
		/* case 3 */
		else if ( ((1 <= rank_right) && ( rank_right <= rank_left)) && ( rank_right < r )) {
			aux_gencode(n->left, file);
			stack_t *R = pop(reg_stack);
			aux_gencode(n->right, file);
			fprintf( fp, "%s %s %s\n", op, top(reg_stack), top(R) );
			push( R, reg_stack );
		}
		/* case 4 */
		else {
			/* no more available registers */
			fprintf( stderr, "\nError: no more available registers!\n" );
			exit(1);
		}
	}
}

