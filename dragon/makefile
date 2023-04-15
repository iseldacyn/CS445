qc: y.tab.o lex.yy.o tree.o
	gcc -g -o qc y.tab.o lex.yy.o tree.o -ly -lfl

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

tree.o: tree.c
	gcc -g -c tree.c -std=c99

y.tab.c: qc.y
	yacc -dv qc.y

lex.yy.c: qc.l
	lex -l qc.l
clean:
	rm -f y.tab.* lex.yy.* *.o qc
