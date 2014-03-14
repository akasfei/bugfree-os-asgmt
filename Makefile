all:
	mkdir -p bin
	gcc -g cp.c -o bin/cp.o
	gcc -g count.c -o bin/count.o
	gcc -g _hello.c -o bin/_hello.o `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
	gcc -g taskmgr.c -o bin/taskmgr.o `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
driver:
	cp ./self_destruct_driver.c ~/linux-kernel/drivers/misc