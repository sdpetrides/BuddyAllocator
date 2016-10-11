#include <stdio.h>
#include <stdlib.h>

typedef struct blk {
	char meta;
	// 00000001 - allocated
	// 00000010 - first or second
	// 11111100 - n where 2^n is the size of the availible space in block
	char * data;	
	// Hold data of unknown size
} Blk; 

static char myblock[8192];
/* 4096 possible 1 byte blocks; 1 possible 4096 byte block */


/** MYMALLOC 

*/
void * mymalloc(size_t reqSize, char * file, int line) {

	if (reqSize > 4096) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	Blk * ptr = (Blk *)&myblock;
	reqSize = (reqSize<<2)+1;
	ptr->meta = (char)reqSize;

	return ptr->;
}


/** MYFREE

*/
void myfree(void * ptr, char * file, int line) {
	//fprintf(stderr, "Error: %s.%d\n", file, line);
}