CC=gcc
YACC=yacc
LEX=lex
FLAGS=-g
YACCFLAGS=-dv
LEXFLAGS=-l


calc: y.tab.o lex.yy.o
	$(CC) $(FLAGS) -o calc y.tab.o lex.yy.o -ly -lfl

y.tab.o: y.tab.c
	$(CC) $(FLAGS) -c y.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(FLAGS) -c lex.yy.c

y.tab.c: calc.y
	$(YACC) $(YACCFLAGS) calc.y

lex.yy.c: calc.l
	$(LEX) $(LEXFLAGS) calc.l

clean:
	rm -rf lex.yy.* y.tab.* *.o calc