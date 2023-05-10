calc: calc.o tree.o codegen.o
	gcc -Wall -g -o calc calc.o tree.o codegen.o

calc.o: calc.c
	gcc -Wall -c calc.c

tree.o: tree.c
	gcc -Wall -c tree.c -std=c99

codegen.o: codegen.c
	gcc -Wall -c codegen.c -std=c99

clean:
	rm -Rf calc *.o
