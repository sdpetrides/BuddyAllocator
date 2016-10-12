#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct meta {
	unsigned char size : 6;		// ______00 - n where (2^n)-1 is the block size
	unsigned char left : 1;		// 000000_0 - first or second
	unsigned char allo : 1;		// 0000000_ - allocated
} Meta;

typedef struct blk {
	Meta m;						// Metadata
	char * data;				// ptr to be returned
} Blk; 

static char myblock[8192];

void print_meta(Blk * ptr) {
	printf("Size: %d\n", (unsigned int)ptr->m.size);
	printf("Left: %d\n", (unsigned int)ptr->m.left);
	printf("Allo: %d\n", (unsigned int)ptr->m.allo);
}

/* Returns log base 2 of a double d */
double log2(double d) {
    return log(d) / log(2);  
}

/* Returns the level a reqSize will fit in */
int size_to_n(size_t reqSize) {
	reqSize+=1;
	double d = log2((double)reqSize);
	return (int)ceil(d);
}

/* MYMALLOC */
void * mymalloc(size_t reqSize, char * file, int line) {

	// Check if too big
	if (reqSize > 8192) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	// Traverse heap to find block of correct size - algo(n)
	int n = size_to_n(reqSize);


	// Allocate Block
	Blk * ptr = (Blk *)&myblock; // needs to be updated with correct position
	ptr = (Blk *)memset(ptr, '\0', n);
	ptr->m.size = n;
	ptr->m.left = 1; // to be changed
	ptr->m.allo = 1;

	return ptr->data;
}

/* MYFREE */
void myfree(void * ptr, char * file, int line) {
	//fprintf(stderr, "Error: %s.%d\n", file, line);
}