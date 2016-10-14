#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mymalloc.h"

typedef struct meta {
	unsigned char allo : 1;		// 0000000_ - allocated
	unsigned char left : 1;		// 000000_0 - first or second
	unsigned char size : 6;		// ______00 - n where (2^n)-1 is the block size
} Meta;

static char myblock[8192];

void unpack(Meta * m, int pos);

/* Fills myblock with zeros and creates first metadata */
void init_block() {
	memset(&myblock, '\0', 8192);
	memset(&myblock, 54, 1);
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

/* Returns the position of the next block of the correct size */
int jump_next(int n, int pos) {
	int bits = pos>>(n);
	bits+=1;
	return bits<<(n);
}

/* Returns the position of the left half of a pair */
int jump_back(int n, int pos) {
	int bits = pos>>(n);
	bits-=1;
	return bits<<(n);
}

/* Fills a Meta struct with metadata at pos */
void unpack(Meta * m, int pos) {
	memset(m, myblock[pos], 1);
}

/* Returns whether position at level n is left or right partner */
int is_left(int n, int pos) {

	// Manipulate bits to set nth bit on
	int k = 1;
	k<<=(n);
	
	// Manipulate bits to zero bits above n
	unsigned int p = (unsigned int)pos;
	p<<=(31-n);
	p>>=(31-n);

	if (k == p) {
		return 0;	// Right
	} else {
		return 1;	// Left
	}
}

/*  Mergee two unallocated blocks with same size */
void merge(int pos, int pos2, int n) {
	
	// Create new meta and set size field
	char newMeta = (n+1)<<2;

	// Set left field
	if (is_left(n+1, pos)) {
		newMeta+=2;
	}

	// Add new meta
	myblock[pos] = newMeta;

	// Delete meta on right partner
	myblock[pos2] = 0;		
}

/* MYMALLOC */
void * mymalloc(size_t reqSize, char * file, int line) {

	// Check if too big
	if (reqSize > 8191) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	// Traverse heap to find block of correct size - algo(n)
	int n = size_to_n(reqSize);
	//printf("n:        %d\n", n);
	int pos = 0;
	unsigned char c = 0;
	Meta * m = memset(&c, 0, 1);
	
	while (pos < 8192) {
		// Read metadata
		unpack(m, pos);

		// Debugging
		if (m->size == 0) {
			exit(0);
		}

		if (n <= m->size) {
			if (m->allo == 1) {				
				// Jump
				pos = jump_next(n, pos);
				continue;
			} else if (m->size == n) {		
				// Allocate
				myblock[pos]+=1;
				pos+=1;
				//printf("Position: %d\n", pos);
				//printf("Allocate: %p\n", (void*)((long int)&myblock+pos));
				return (void*)((long int)&myblock+pos);
			} else { 						
				// Partition

				// Get partner position
				int partner = jump_next((m->size)-1, pos);

				// Set Left
				char meta_1 = 2;
				char meta_2 = 0;
				
				// Set Size
				char s = ((m->size)-1)<<2;
				meta_1 = (meta_1 | s);
				meta_2 = (meta_2 | s);

				// Fill in metadata
				myblock[pos] = meta_1;
				myblock[partner] = meta_2;

				// Continue on same position with new size of block
				continue;
			}
		} else {
			// Jump
			pos = jump_next(n, pos);
			continue;
		}
	}

	fprintf(stderr, "Error: %s.%d\n", file, line);

	return NULL;
}

/* MYFREE */
void myfree(void * ptr, char * file, int line) {
	
	// Error Checking
	if (ptr <= (void *)&myblock || ptr > (void *)(&myblock + 8192)) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	//printf("Free:     %p\n", ptr);

	// Get position
	int pos = (int)(ptr-(void *)&myblock-1);

	// Check if valid metadata location
	if (pos%2 == 1 || myblock[pos] == 0) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
		return;
	}


	// Initialize variables for merge
	unsigned char c1 = 0;
	unsigned char c2 = 0;
	Meta * m1 = memset(&c1, 0, 1); 
	Meta * m2 = memset(&c2, 0, 1);
	unpack(m1,pos);

	// Change allocated field
	myblock[pos] = myblock[pos] - 1;

	while (pos >= 0 && pos <= 8196){
		// Read metadata
		unpack(m1,pos);

		if (m1->left) {	// Left Partner

			// Get position of other partner and read metadata
			int pos2 = jump_next(m1->size, pos);
			unpack(m2,pos2);
	
			// Merge or break
			if (m2->allo || m2->size != m1->size) {
				break;
			} else {
				merge(pos, pos2, m1->size);
			}

		} else {		// Right Partner

			// Get position of other partner and read metadata
			int pos2 = jump_back(m2->size,pos);
			unpack(m2,pos2);

			// Merge or break
			if (m2->allo || m2->size != m1->size) {
				break;
			} else {
				merge(pos2, pos, m1->size); 
			}	
		}
	}
}
