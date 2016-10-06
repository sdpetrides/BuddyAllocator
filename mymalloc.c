#include <stdio.h>
#include <stdlib.h>

static char myblock[5000];

/** MYMALLOC 

*/
void * mymalloc(size_t reqSize, char * file, int line) {
	void * ptr;
	ptr = &myblock;

	fprintf(stderr, "Error: %s.%d\n", file, line);

	return ptr;
}


/** MYFREE

*/
void myfree(void * ptr, char * file, int line) {
	fprintf(stderr, "Error: %s.%d\n", file, line);
}