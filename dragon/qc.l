%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree.h"
#include "y.tab.h"
%}

identifier [[:alpha:]][[:alnum:]]*
number [[:digit:]]+
whitespace [ \t]+

%%

	/* comments */
"{"[^}]*"}"		{ ; }
"(*".*"*)"		{ ; }
"//".*\n		{ ; }
	/* code */
"def"			{ fprintf( stderr, "[DEF]" ); return DEF; }
"var" 			{ fprintf( stderr, "[VAR]" ); return VAR; }
"array" 		{ fprintf( stderr, "[ARRAY]" ); return ARRAY; }
"of" 			{ fprintf( stderr, "[OF]" ); return OF; }
"integral" 		{ fprintf( stderr, "[INTEGRAL]" ); return INTEGRAL; }
"rational" 		{ fprintf( stderr, "[RATIONAL]" ); return RATIONAL; }
"boolean" 		{ fprintf( stderr, "[BOOLEAN]" ); return BOOLEAN; }
"begin" 		{ fprintf( stderr, "[BEGIN]" ); return BBEGIN; }
"end"	 		{ fprintf( stderr, "[END]" ); return END; }
"func"			{ fprintf( stderr, "[FUNC]" ); return FUNC; }
"proc"			{ fprintf( stderr, "[PROC]" ); return PROC; }
"if"	 		{ fprintf( stderr, "[IF]" ); return IF; }
"then"	 		{ fprintf( stderr, "[THEN]" ); return THEN; }
"else" 			{ fprintf( stderr, "[ELSE]" ); return ELSE; }
"while"	 		{ fprintf( stderr, "[WHILE]" ); return WHILE; }
"do"	 		{ fprintf( stderr, "[DO]" ); return DO; }
"repeat"	 	{ fprintf( stderr, "[REPEAT]" ); return REPEAT; }
"until"	 		{ fprintf( stderr, "[UNTIL]" ); return UNTIL; }
"for" 			{ fprintf( stderr, "[FOR]" ); return FOR; }
"or"	 		{ fprintf( stderr, "[ADDOP:%d]", yylval.opval = OR ); return ADDOP; }
"div"	 		{ fprintf( stderr, "[MULOP:%d]", yylval.opval = DIV ); return MULOP; }
"mod"	 		{ fprintf( stderr, "[MULOP:%d]", yylval.opval = MOD); return MULOP; }
"and"	 		{ fprintf( stderr, "[MULOP:%d]", yylval.opval = AND ); return MULOP; }
"not"	 		{ fprintf( stderr, "[NOT]" ); return NOT; }
{identifier} 	{ fprintf( stderr, "[ID:%s]", yylval.sval = (char *)strdup(yytext) ); return ID; }
{number}		{ fprintf( stderr, "[INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }
{number}"."{number}	{ fprintf( stderr, "[RNUM:%f]", yylval.rval = atof(yytext) ); return RNUM; }
{whitespace}	{ ; }
":="	 		{ fprintf( stderr, "[ASSIGNOP]"); return ASSIGNOP; }
"<"		 		{ fprintf( stderr, "[RELOP:%d]", yylval.opval = LT ); return RELOP; }
"<="		 	{ fprintf( stderr, "[RELOP:%d]", yylval.opval = LE ); return RELOP; }
">"	 			{ fprintf( stderr, "[RELOP:%d]", yylval.opval = GT ); return RELOP; }
">="	 		{ fprintf( stderr, "[RELOP:%d]", yylval.opval = GE ); return RELOP; }
"<>"	 		{ fprintf( stderr, "[RELOP:%d]", yylval.opval = NE ); return RELOP; }
"="	 			{ fprintf( stderr, "[RELOP:%d]", yylval.opval = EQ ); return RELOP; }
"+"		 		{ fprintf( stderr, "[ADDOP:%d]", yylval.opval = PLUS ); return ADDOP; }
"-"	 			{ fprintf( stderr, "[ADDOP:%d]", yylval.opval = MINUS ); return ADDOP; }
"*"	 			{ fprintf( stderr, "[ADDOP:%d]", yylval.opval = STAR ); return MULOP; }
"/"		 		{ fprintf( stderr, "[MULOP:%d]", yylval.opval = SLASH ); return MULOP; }
"("	 			{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
")"				{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
":" 			{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
";" 			{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"," 			{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"." 			{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"[" 			{ fprintf( stderr, "[LBRACKET]" ); return LBRACKET; }
"]" 			{ fprintf( stderr, "[RBRACKET]" ); return RBRACKET; }
".."			{ fprintf( stderr, "[DOTDOT]" ); return DOTDOT; }
\n				{ fprintf( stderr, "\n" ); }
.				{ fprintf( stderr, "<%c>", yytext[0] ); return yytext[0]; }

%%

int yyerror( char *message )
{
	fprintf( stderr, "Error: %s\n", message );
	exit(1);
}
