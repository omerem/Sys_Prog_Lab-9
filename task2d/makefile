all: loader
loader: loader.o
	ld  -o loader loader.o start.o startup.o -L/usr/lib32 -lc -T linking_script -dynamic-linker /lib32/ld-linux.so.2
loader.o: loader.c 
	gcc -m32 -c loader.c -o loader.o
.PHONY: clean
clean: 
	rm -f loader loader.o
