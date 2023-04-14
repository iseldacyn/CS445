/* */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "const.h"
#include "tree.h"

/* globals for communication between parser and scanner */
int current_token;
int current_attribute;

/* prototypes */
tree_t *expr();
tree_t *term();
tree_t *factor();
int get_token();
void match( int );

///////////////////////////////////////////////////////////

/* main driver */
int main()
{
  tree_t *value;

  /* initialize first token */
  current_token = get_token();
  
  /* unleash the parser (start the derivation) */
  value = expr();
  print_tree( value );
  
  /* check that EOS is there */
  if(current_token == EOS){
    fprintf( stderr, "\nEVALUATING TREE...\n\n" );
    print_tree( value );
    fprintf( stderr, "\nResult: %.2f\n", eval_tree( value ) );
  }
  else exit(1);

  free_tree( value );

  return 0;
}

/* parser
 * uses the basic ambiguous grammar:
 *  E -> E+E | E*E | (E) | NUM
 * but revised to handle precedence and associative rules:
 *  E -> E+E | T, T -> T*T | F, F -> (E) | NUM
 * adjusted again to remove left-recursion (but not left-factoring):
 *  E -> TE', E' -> +TE' | empty
 *  T -> FT', T' -> *FT' | empty
 *  F -> (E) | NUM
 * the coroutines of the parser consist of expr(), term(), and factor()
 */

/* E -> TE', E' -> +TE' | empty */
tree_t *expr()
{
  tree_t *value = term();
  //expr'();
  while ( current_token == '+' )
  {
    match('+');
    value = make_tree( ADDOP, '+', value, term() );
  }
  while ( current_token == '-' )
  {
    match('-');
    value = make_tree( ADDOP, '-', value, term() );
  }
  return value;
}

/* T -> FT', T' -> *FT' | empty */
tree_t *term()
{
  tree_t *value = factor();
  //term'();
  while ( current_token == '*') 
  {
    match('*');
    value = make_tree( MULOP, '*', value, factor() );
  }
  while ( current_token == '/' )
  {
    match('-');
    value = make_tree( MULOP, '-', value, factor() );
  }
  return value;
}

/* F -> (E) | NUM */
tree_t *factor()
{
  tree_t *value;
  if(current_token == '('){
    match('(');
    value = expr();
    match(')');
  }
  else if (current_token == NUM) 
  {
    value = make_tree( NUM, current_attribute, NULL, NULL ); // current_attribute is lost
    match(NUM);
  }
  else
  {
    fprintf(stderr, "Error in factor: %d\n", current_token);
    exit(1);
  }
  return value;
}

/* get_token(): the scanner
 * responsible for tokenizing a string using the token types:
 *  types are +, -, *, /, (, ), NUM
 * example: "(278+312)*4659" ==> [(][NUM][+][NUM][)][*][NUM]
 */
int get_token() {
  int c, value;
  
  while( (c = getchar()) != EOF )
  {
    switch(c)
    {

      case '+': case '-': case '*': case '/': /* OPERATORS */
        fprintf( stderr, "[OP:%c]",c );
        return c;

      case '(': case ')': /* BRACKETS */
        fprintf( stderr, "[%c]",c );
        return c;
      
      case ' ': case '\t':
        continue;
      
      default:
        if( isdigit(c) ){ /* NUM */
          value = 0;
          do {
            value = 10*value + (c - '0');
          } while( isdigit( c=getchar() ));
          ungetc(c, stdin);
          fprintf( stderr, "[NUM:%d]", current_attribute = value );
          return NUM;
        } 
        else if( c=='\n' ){ /* EOS */
          fprintf( stderr, "[EOS]" );
          return EOS;
        }
        else {
          fprintf( stderr, "ERROR: unknown token {%c}\n", c );
          assert(0);
          return -1;
        }
    }
  }
}

/* match(): helpful for scanner
 * advances to next token only if token is the expected one
 */
void match(int expected_token)
{
  if( current_token == expected_token ){
    current_token = get_token();
  }
  else{
    fprintf(stderr, "Error in match: current = %d, expected = %d\n", current_token, expected_token);
    exit(1);
  }
}
