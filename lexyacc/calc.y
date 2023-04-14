%{
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
%}

%union {
  int ival;   /* NUM */
  int opval;  /* ADDOP, MULOP */
}

%token <ival> NUM

%left <opval> ADDOP
%left <opval> MULOP

%type <ival> expr

%%

boo: expr { fprintf(stderr, "\nValue = %d\n", $1); }
    ;

expr: expr ADDOP expr { $$ = $1 + $3; } 
    | expr MULOP expr { $$ = $1 * $3; }
    | '(' expr ')'    { $$ = $2; }
    | NUM             { $$ = $1; }
    ;

%%

int main()
{
  yyparse();
}