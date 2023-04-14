calc: calc.o tree.o
	gcc -Wall -g -o calc calc.o tree.o

calc.o: calc.c
	gcc -Wall -c calc.c

tree.o: tree.c
	gcc -Wall -c tree.c -std=c99

clean:
	rm -Rf calc *.o