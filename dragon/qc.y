%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "tree.h"
#include "symtab.h"
#include "semantic.h"
#include "error.h"

int yylex( void ); 
int yyerror( const char * );

list_t *id_ptr;
scope_t *top_scope;
%}

%union{
	/* token attributes */
	int ival; 	/* INUM */
	float rval; /* RNUM */
	char *sval; /* ID */
	int opval; 	/* RELOP ADDOP MULOP*/

	tree_t *tval; /* tree attribute for variables */
}

%token DEF
%token VAR
%token ARRAY OF
%token INTEGRAL RATIONAL
%token BBEGIN END
%token FUNC PROC
%token IF THEN ELSE
%token WHILE DO
%token REPEAT UNTIL
%token FOR
%token ASSIGNOP

%token <opval> RELOP
%token LT LE GT GE EQ NE
%token <opval> ADDOP
%token PLUS MINUS OR
%token <opval> MULOP
%token STAR SLASH DIV MOD AND
%token NOT

%token FUNCTION_CALL ARRAY_ACCESS
%token LIST

%token <sval> ID
%token <ival> INUM
%token <rval> RNUM

%type <ival> type
%type <ival> standard_type

%type <tval> variable
%type <tval> expression
//%type <tval> statement 
%type <tval> expression_list
//%type <tval> statement_list
%type <tval> identifier_list
%type <tval> parameter_list
%type <tval> arguments
%type <tval> simple_expression
%type <tval> term
%type <tval> factor

%%

program: 
	DEF ID '(' identifier_list ')' ';'
	declarations
	subprogram_declarations
	compound_statement
	'.'
	;

identifier_list: ID
		{ 
			double_decl( top_scope, $1 );
			id_ptr = scope_insert( top_scope, $1 );
			$$ = make_id( id_ptr );
		}
	| identifier_list ',' ID
		{
			double_decl( top_scope, $3 );
			id_ptr = scope_insert( top_scope, $3 );
			$$ = make_tree( LIST, $1, make_id( id_ptr ));
		}
	;

declarations: declarations VAR identifier_list ':' type ';'
		{ semantic_set_type( $3, $5 ); }
	| /* empty */
	;

type: standard_type { $$ = $1; }
	| ARRAY '[' range ']' OF standard_type
		{ $$ = ARRAY; }
	;

range: INUM '.' '.' INUM
	 	{ $$ = make_range( $1, $4 ); }
	;

standard_type: INTEGRAL { $$ = INTEGRAL; }
	| RATIONAL { $$ = RATIONAL; }
	;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration:
		subprogram_header
		declarations
		subprogram_declarations
		compound_statement
		{ top_scope = scope_pop( top_scope ); } // leaving inner scope
	;

subprogram_header: FUNC ID arguments ':' standard_type ';'
		{
			double_decl( top_scope, $2 );
			id_ptr = scope_insert( top_scope, $2 );	// record function ID in current scope
			top_scope = scope_push( top_scope );	// create a new scope
			id_ptr->type = $5;
			id_ptr-> = $3;
			id_ptr = scope_insert( top_scope, $2 );
		}
	| PROC ID
		{ 
			double_decl( top_scope, $2 );
			id_ptr = scope_insert( top_scope, $2 );	// record function ID in current scope
			top_scope = scope_push( top_scope );	// create a new scope
		} 
		arguments ';'
	;

arguments: '(' parameter_list ')'
		 { $$ = $2; }
	| /* empty */
		{ $$ = NULL; }
	;

parameter_list: identifier_list ':' type
		{ 
			semantic_set_type( $1, $3 );
			$$ = $1;
		}
	| parameter_list ';' identifier_list ':' type
		{
			semantic_set_type( $3, $5 );
			$$ = make_tree( LIST, $1, $3 );
		}
	;

compound_statement: 
	BBEGIN
		optional_statements
	END
	;

optional_statements: statement_list
	| /* empty */
	;

statement_list: statement
	//	{ $$ = $1; }
	| statement_list ';' statement
	//	{ $$ = make_tree( LIST, $1, $3 ); }
	;

statement: matched_statement
	| unmatched_statement
	;

matched_statement: IF expression THEN matched_statement ELSE matched_statement
	| WHILE expression DO matched_statement
	| REPEAT matched_statement UNTIL expression
	| FOR ID ASSIGNOP range DO matched_statement
	| other_statements
	;

unmatched_statement: IF expression THEN statement
	| IF expression THEN matched_statement ELSE unmatched_statement
	| WHILE expression DO unmatched_statement
	| REPEAT unmatched_statement UNTIL expression
	| FOR ID ASSIGNOP range DO unmatched_statement
	;

other_statements: variable ASSIGNOP expression
		{ type_check( type_of($1), type_of($3)); }
	| procedure_statement
	//	{ $$ = $1; }
	| compound_statement
	;

variable: ID
		{ $$ = make_id( semantic_lookup( top_scope, $1 )); }
	| ID '[' expression ']'
		{ $$ = make_tree( ARRAY_ACCESS, make_id( semantic_lookup( top_scope, $1 )), $3 ); }
	;

procedure_statement: ID
	| ID '(' expression_list ')'
	//	{ $$ = }
	;

expression_list: expression
		{ $$ = $1; }
	| expression_list ',' expression
		{ $$ = make_tree( LIST, $1, $3 ); }
	;

expression: simple_expression
		{ $$ = $1; }
	| simple_expression RELOP simple_expression
		{ 
			$$ = make_tree( RELOP, $1, $3 );
			$$->attribute.opval = $2;
		}
	;

simple_expression: term
		{ $$ = $1; }
	| ADDOP term
		{
			$$ = make_tree( ADDOP, $2, NULL );
			$$->attribute.opval = $1;
		}
	| simple_expression ADDOP term
		{	$$ = make_tree( ADDOP, $1, $3 );
			$$->attribute.opval = $2;
		}
	;

term: factor
		{ $$ = $1; }
    | term MULOP factor
		{ 
			$$ = make_tree( MULOP, $1, $3 );
			$$->attribute.opval = $2;
		}
    ;

factor: ID
		{ $$ = make_id( semantic_lookup( top_scope, $1 )); }
	| ID '(' expression_list ')'
		{ 
			id_ptr = semantic_lookup( tope_scope, $1 );
			$$ = make_tree( FUNCTION_CALL, make_id( semantic_lookup( top_scope, $1 ) ), $3 ); 
			
		}
	| ID '[' expression ']'
		{ $$ = make_tree( ARRAY_ACCESS, make_id( semantic_lookup( top_scope, $1 ) ), $3 ); }
	| INUM
		{ $$ = make_inum( $1 ); }
	| RNUM
		{ $$ = make_rnum( $1 ); }
	| '(' expression ')'
		{ $$ = $2; }
	| NOT factor
		{ $$ = make_tree( NOT, $2, NULL ); }
	;

%%

int main()
{
	top_scope = scope_push( top_scope );
	yyparse();
}
