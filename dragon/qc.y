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
%token BOOLEAN
%token INTEGRAL RATIONAL
%token DOTDOT
%token LBRACKET RBRACKET 
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
%token FUNCTION_DEFINITION PROCEDURE_DEFINITION
%token ASSIGN
%token LIST RANGE
%token IFELSE IFTHEN THENELSE
%token FORDO WHILEDO REPEATUNTIL

%token <sval> ID
%token <ival> INUM
%token <rval> RNUM

%type <ival> type
%type <ival> standard_type

%type <tval> subprogram_header
%type <tval> range
%type <tval> variable
%type <tval> expression
%type <tval> statement
%type <tval> matched_statement
%type <tval> unmatched_statement
%type <tval> other_statements
%type <tval> optional_statements
%type <tval> expression_list
%type <tval> statement_list
%type <tval> identifier_list
%type <tval> parameter_list
%type <tval> arguments
%type <tval> procedure_statement
%type <tval> compound_statement
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

type: standard_type
		{ $$ = $1; }
	| ARRAY LBRACKET range RBRACKET OF standard_type
		{ $$ = ARRAY; }
	;

range: INUM DOTDOT INUM
	 	{ 
			check_range( $1, $3 );
			$$ = make_tree( RANGE, make_inum($1), make_inum($3) );
			//$$ = make_range( $1, $3 );
		}
	;

standard_type: INTEGRAL { $$ = INTEGRAL; }
	| RATIONAL { $$ = RATIONAL; }
	| BOOLEAN { $$ = BOOLEAN; }
	;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration:
		subprogram_header
		declarations
		subprogram_declarations
		compound_statement
		{
			/* here i need to work on assignop */
			/* this shouldnt be too hard */
			needs_assignop( $1, $4 );
			top_scope = scope_pop( top_scope ); // leaving inner scope
		}
	;

subprogram_header: FUNC ID
		{
			double_decl( top_scope, $2 );
			id_ptr = scope_insert( top_scope, $2 );	// record function ID in current scope
			top_scope = scope_push( top_scope );	// create a new scope
		}
		arguments ':' standard_type ';'
		{
			id_ptr = global_scope_search( top_scope, $2 );
			id_ptr->type = $6;
			id_ptr->data.args = make_list_from_tree($4);
			$$ = make_tree( FUNCTION_DEFINITION, make_id( id_ptr ), $4 );
		}
	| PROC ID
		{ 
			double_decl( top_scope, $2 );
			id_ptr = scope_insert( top_scope, $2 );	// record function ID in current scope
			top_scope = scope_push( top_scope );	// create a new scope
		} 
		arguments ';'
		{
			id_ptr = global_scope_search( top_scope, $2 );
			id_ptr->type = -1;
			id_ptr->data.args = make_list_from_tree($4);
			$$ = make_tree( PROCEDURE_DEFINITION, make_id( id_ptr ), $4 );
		}
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
		{ $$ = $2; }
	;

optional_statements: statement_list
		{ $$ = $1; }
	| /* empty */
		{ $$ = NULL; }
	;

statement_list: statement
		{ $$ = $1; }
	| statement_list ';' statement
		{ $$ = make_tree( LIST, $1, $3 ); }
	;

statement: matched_statement
		{ $$ = $1; }
	| unmatched_statement
		{ $$ = $1; }
	;

matched_statement: IF expression THEN matched_statement ELSE matched_statement
		{ $$ = make_tree( IFELSE, $2, make_tree( THENELSE, $4, $6 ) ); }
	| WHILE expression DO matched_statement
		{ $$ = make_tree( WHILEDO, $2, $4 ); }
	| REPEAT matched_statement UNTIL expression
		{ $$ = make_tree( REPEATUNTIL, $4, $2 ); }
	| FOR ID ASSIGNOP range DO matched_statement
		{
			id_ptr = semantic_lookup( top_scope, $2 );
			type_check( id_ptr->type, INUM );
			$$ = make_tree( FORDO, make_tree( ASSIGN, make_id( id_ptr ), $4 ), $6 );
		}
	| other_statements
		{ $$ = $1; }
	;

unmatched_statement: IF expression THEN statement
		{ $$ = make_tree( IFTHEN, $2, $4 ); }
	| IF expression THEN matched_statement ELSE unmatched_statement
		{ $$ = make_tree( IFELSE, $2, make_tree( THENELSE, $4, $6 ) ); }
	| WHILE expression DO unmatched_statement
		{ $$ = make_tree( WHILEDO, $2, $4 ); }
	| REPEAT unmatched_statement UNTIL expression
		{ $$ = make_tree( REPEATUNTIL, $4, $2 ); }
	| FOR ID ASSIGNOP range DO unmatched_statement
		{
			id_ptr = semantic_lookup( top_scope, $2 );
			type_check( id_ptr->type, INUM );
			$$ = make_tree( FORDO, make_tree( ASSIGN, make_id( id_ptr ), $4 ), $6 );
		}
	;

other_statements: variable ASSIGNOP expression
		{
			type_check( type_of($1), type_of($3));
			$$ = make_tree( ASSIGN, $1, $3 );
		}
	| procedure_statement
		{ $$ = $1; }
	| compound_statement
		{ $$ = $1; }
	;

variable: ID
		{ $$ = make_id( semantic_lookup( top_scope, $1 )); }
	| ID LBRACKET expression RBRACKET
		{ 
			type_check( type_of($3), INUM );
			$$ = make_tree( ARRAY_ACCESS, make_id( semantic_lookup( top_scope, $1 )), $3 );
		}
	;

procedure_statement: ID
		{
			id_ptr = semantic_lookup( top_scope, $1 );
			$$ = make_tree( FUNCTION_CALL, make_id( id_ptr ), NULL );
		}
	| ID '(' expression_list ')'
		{
			id_ptr = semantic_lookup( top_scope, $1 );
			check_args( id_ptr->data.args, make_list_from_tree($3) );
			$$ = make_tree( FUNCTION_CALL, make_id( id_ptr ), $3 );
		}
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
		{	
			type_check( type_of($1), type_of($3) );
			$$ = make_tree( ADDOP, $1, $3 );
			$$->attribute.opval = $2;
		}
	;

term: factor
		{ $$ = $1; }
    | term MULOP factor
		{ 
			type_check( type_of($1), type_of($3) );
			$$ = make_tree( MULOP, $1, $3 );
			$$->attribute.opval = $2;
		}
    ;

factor: ID
		{ $$ = make_id( semantic_lookup( top_scope, $1 )); }
	| ID '(' expression_list ')'
		{ 
			id_ptr = semantic_lookup( top_scope, $1 );
			check_args( id_ptr->data.args, make_list_from_tree($3) );
			$$ = make_tree( FUNCTION_CALL, make_id( semantic_lookup( top_scope, $1 ) ), $3 );
		}
	| ID LBRACKET expression RBRACKET
		{
			type_check( type_of($3), INUM );
			$$ = make_tree( ARRAY_ACCESS, make_id( semantic_lookup( top_scope, $1 ) ), $3 );
		}
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

	scope_insert( top_scope, "read" );
	scope_insert( top_scope, "write" );

	yyparse();
}
