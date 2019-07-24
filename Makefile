all: main

main: main.c tsp.o slist.o
	gcc -g -o main main.c tsp.o slist.o -Wall -pedantic

tsp.o: tsp.c
	gcc -g -c tsp.c -Wall -pedantic -O

slist.o: slist.c
	gcc -c slist.c -Wall -pedantic
