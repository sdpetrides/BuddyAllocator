all:        
	gcc -Wall -g -o memgrind memgrind.c mymalloc.c

clean:
	rm -f memgrind