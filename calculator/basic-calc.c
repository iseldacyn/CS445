/* A small sample program for an arithmetic calculator
 * Supports addition and multiplication on integers. Allows brackets to override precedence.
 */
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<ctype.h>

/* constants */
#define EOS 257
#define NUM 258

/* globals for communication between parser and scanner */
int current_token;
int current_attribute;

/* prototypes */
int expr();
int term();
int factor();
int get_token();
void match( int );

int main() {
  int value;

  /* initialize first token */
  current_token = get_token();
  
  /* unleash the parser (start the derivation) */
  value = expr();
  
  /* check that EOS is there */
  if(current_token == EOS){
    fprintf(stderr, "Value = %d\n", value);
  }
  else exit(1);

  return 0;
}

/* parser */
/* E -> TE', E' -> +TE' | empty */
int expr(){
  int value = term();
  //expr'();
  while(current_token == '+'){
    match('+');
    value += term();
  }
  return value;
}
/*
expr(){
  if(current_token == '+'){
    match('+');
    term();
    expr'();
  }
  else;
}
*/

/* T -> FT', T' -> *FT' | empty */
int term(){
  int value = factor();
  //term'();
  while (current_token == '*') {
    match('*');
    value *= factor();
  }
  return value;
}
/*
term'(){
  if(curent_token == '*'){
    match('*');
    factor();
    term'();
  }
}
*/

/* F -> (E) | NUM */
int factor(){
  int value;
  if(current_token == '('){
    match('(');
    value = expr();
    match(')');
  }
  else if (current_token == NUM) {
    value = current_attribute;
    match(NUM);
  }
  else{
    fprintf(stderr, "Error in factor: %d\n", current_token);
    exit(1);
  }
  return value;
}

/* tokenizer/scanner: tokenize a string according to its token types */
/* example: "(278+312)*4659" ==> [(][NUM][+][NUM][)][*][NUM] */
int get_token() {
  int c;
  
  while( (c = getchar()) != EOF ){
    switch(c){

      case '+': case '*': /* OPERATORS */
        fprintf(stderr, "[OP:%c]",c);
        return c;

      case '(': case ')': /* BRACKETS */
        fprintf(stderr, "[%c]",c);
        return c;
      case ' ': case '\t':
        continue;
      default:
        if( isdigit(c) ){ /* NUM */
          int value = 0;
          do {
            value = 10*value + (c - '0');
          } while( isdigit( c=getchar() ));
          ungetc(c, stdin);
          fprintf(stderr, "[NUM:%d]", current_attribute = value);
          return NUM;
        } 
        else if( c=='\n' ){ /* EOS */
          fprintf(stderr, "[EOS]");
          return EOS;
        }
        else { /* ILLEGAL CHAR */
          fprintf(stderr, "ERROR: unknown token {%c}\n",c);
          exit(1);
        }
    }
  }

}

void match(int expected_token){
  if(current_token == expected_token){
    current_token = get_token();
  }
  else{
    fprintf(stderr, "Error in match: current = %d, expected = %d\n", current_token, expected_token);
    exit(1);
  }
}
