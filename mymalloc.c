#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct meta {
	unsigned char allo : 1;		// 0000000_ - allocated
	unsigned char left : 1;		// 000000_0 - first or second
	unsigned char size : 6;		// ______00 - n where (2^n)-1 is the block size
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
	int bits = pos>>(n);
	bits-=1;
	return bits<<(n);
}

/* Fills a Meta struct with metadata at pos */
void unpack(Meta * m, int pos) {
	memset(m, myblock[pos], 1);
}

/* Fills myblock with zeros and creates first metadata */
void init_block() {
	memset(&myblock, '\0', 8192);
	memset(&myblock, 54, 1);
}

/* MYMALLOC */
void * mymalloc(size_t reqSize, char * file, int line) {

	// Check if too big
	if (reqSize > 8191) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}

	// Traverse heap to find block of correct size - algo(n)
	int n = size_to_n(reqSize);
	printf("n:      %d\n", n);
	int pos = 0;
	unsigned char c = 0;
	Meta * m = memset(&c, 0, 1);
	
	while (pos < 8192) {
		// Read metadata
		unpack(m, pos);

		if (m->size == 0) {
			printf("m.size: %d\n", m->size);
			printf("pos:    %d\n", pos);
			exit(0);
		}

		if (n <= m->size) {
			if (m->allo == 1) {				
				// Jump
				pos = jump_next(n, pos);
				continue;
			} else if (m->size == n) {		
				// Allocate
				printf("Allocate\n");
				myblock[pos]+=1;
				printf("Pos:    %d\n", (int)pos);
				printf("M1:     %d\n", (int)myblock[pos]);
				printf("Ptr:    0x%X\n\n", (int)&myblock+pos+1);
				return &myblock+pos+1;
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
	
	if (ptr <= (void *)&myblock || ptr > (void *)(&myblock + 8192)) {
		fprintf(stderr, "Error: %s.%d\n", file, line);
	}
	int pos = (int)(ptr=&myblock-1);
	unsigned char c1 = 0; c2 = 0;
	Meta * m1 = &c1; Meta * m2 = &c2;
	unpack(m1,pos);
	myblock[pos] = myblock[pos] - 1;

	while(pos >= 0 && pos <= 8196){
		unpack(m1,pos);

		if(m1.left){
			int pos2 = jump_next(int m1.size, pos);
			unpack(m2,pos2);
		}
	}
	

















}