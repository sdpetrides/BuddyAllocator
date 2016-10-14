all:        
	gcc -Wall -g -o memgrind memgrind.c mymalloc.c -lm

clean:
	rm -f memgrind