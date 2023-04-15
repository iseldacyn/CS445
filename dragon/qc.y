%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
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

%type <tval> expression_list
%type <tval> expression
%type <tval> simple_expression
%type <tval> term
%type <tval> factor

%%

program: DEF ID '(' identifier_list ')' ';'
	declarations
	subprogram_declarations
	compound_statement
	'.'
	;

identifier_list: ID
	| identifier_list ',' ID
	;

declarations: declarations VAR identifier_list ':' type ';'
	| /* empty */
	;

type: standard_type
	| ARRAY '[' range ']' OF standard_type
	;

range: INUM '.' '.' INUM
	;

standard_type: INTEGRAL
	| RATIONAL
	;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration:
		subprogram_header
		declarations
		subprogram_declarations
		compound_statement
	;

subprogram_header: FUNC ID arguments ':' standard_type ';'
	| PROC ID arguments ';'
	;

arguments: '(' parameter_list ')'
	| /* empty */
	;

parameter_list: identifier_list ':' type
	| parameter_list ';' identifier_list ':' type
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
	| statement_list ';' statement
	;

statement: variable ASSIGNOP expression
		{ print_tree( $3 ); }
	| procedure_statement
	| compound_statement
	| IF expression THEN statement ELSE statement
	| IF expression THEN statement
	| WHILE expression DO statement
	;

variable: ID
	| ID '[' expression ']'
	;

procedure_statement: ID
	| ID '(' expression_list ')'
	;

expression_list: expression
		{ $$ = $1; }
	| expression_list ',' expression
		{ $$ = make_tree( LIST, $1, $3 ); }
	;

expression: simple_expression
		{ $$ = $1; }
	| simple_expression RELOP simple_expression
		{ $$ = make_tree( RELOP, $1, $3 ); $$->attribute.opval = $2; }
	;

simple_expression: term
		{ $$ = $1; }
	| ADDOP term
		{ $$ = make_tree( ADDOP, $2, NULL ); $$->attribute.opval = $1; }
	| simple_expression ADDOP term
		{ $$ = make_tree( ADDOP, $1, $3 ); $$->attribute.opval = $2; }
	;

term: factor
		{ $$ = $1; }
    | term MULOP factor
		{ $$ = make_tree( MULOP, $1, $3 ); $$->attribute.opval = $2; }
    ;

factor: ID
		{ $$ = make_id( $1 ); }
	| ID '(' expression_list ')'
		{ $$ = make_tree( FUNCTION_CALL, make_id( $1 ), $3 ); }
	| ID '[' expression ']'
		{ $$ = make_tree( ARRAY_ACCESS, make_id( $1 ), $3 ); }
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
	yyparse();
}
