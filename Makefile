all:
	mkdir -p bin
	gcc -g cp.c -o bin/cp.o
	gcc -g count.c -o bin/count.o
	gcc -Wall -g _hello.c -o bin/_hello.o `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
