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

/* Returns the position of the next block of the correct size */
int jump_next(int n, int pos) {
	int bits = pos>>(n);
	bits+=1;
	return bits<<(n);
}

/* Returns the position of the left half of a pair */
int jump_back(int n, int pos) {
	// if right block, find the left block
	return 1;
}

/* Fills a Meta struct with metadata at pos */
void unpack(Meta * m, int pos) {
	memcpy(m, &myblock+pos, 1);
}

/* Fills myblock with zeros and creates first metadata */
void init_block() {
	memset(&myblock[0], '\0', 8192);
	myblock[0] = 54;
}

/* MYMALLOC */
void * mymalloc(size_t reqSize, char * file, int line) {

	// Check if too big
	if (reqSize > 8191) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	// Traverse heap to find block of correct size - algo(n)
	int n = size_to_n(reqSize);
	int pos = 0;
	unsigned char c = '\0';
	Meta * m = &c;
	
	while (pos < 8192) {
		// Read metadata
		unpack(m, pos);

		if (n <= m->size) {
			if (m->allo == 1) {
				// Jump
				pos = jump_next(n, pos);
				continue;
			} else if (m->size == n) {
				// Allocate
				myblock[pos]+=1;
				return &myblock+pos+1;
			} else {
				// Partition
					// create metadata for right side
						// set meta->allo to 0
						// set meta->left to 0
						// set meta->size to meta->size-1
					// if new level is correct
						return &myblock+1;
					// else 
						// continue
			}
		} else {
			// Jump
			pos = jump_next(n, pos);
			continue;
		}
	}

	return NULL;
}

/* MYFREE */
void myfree(void * ptr, char * file, int line) {
	//fprintf(stderr, "Error: %s.%d\n", file, line);
}