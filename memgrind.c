#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

void testA();
void testB();

// MACROS
#define malloc(size) mymalloc( size, __FILE__, __LINE__)
#define free(ptr) myfree( ptr, __FILE__, __LINE__)

/* testA: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one */
void testA() {
	int i = 0;
	char * p[3000];

	while (i < 3000) {
		p[i] = (char *)malloc(sizeof(char)*1);
		i++;
	}

	i = 0;
	
	while (i < 3000) {
		free(p[i]);
		i++;
	}

	return;
}

/* testB: malloc() 1 byte and immediately free it 3000 times in a row */
void testB() {
	int i = 0;
	char * p;

	while (i < 3000) {
		p = (char *)malloc(sizeof(char)*1);
		free(p);
		i++;
	}

	return;
}

/* testD: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one 
void testD() {
	int i = 0;
	char * p[3000];

	while (i < 3000) {
		int random = rand() % 100;
		p[i] = (char *)malloc(sizeof(char)*random);
		i++;
	}

	i = 0;
	
	while (i < 3000) {
		free(p[i]);
		i++;
	}

	return;
} */

/* testD: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one 
void testD() {
	int i = 0;
	char * p[3000];

	while (i < 3000) {
		int random = rand() % 100;
		p[i] = (char *)malloc(sizeof(char)*random);
		i++;
	}

	i = 0;
	
	while (i < 3000) {
		free(p[i]);
		i++;
	}

	return;
} */

int main(int argc, char const *argv[]) {

	// Initialize myblock
	init_block();

	// Create function pointers for each function
	//void (*fptrA)() = &testA;
	//void (*fptrB)() = &testB;
	//void (*fptrC)() = &testC;

	// Create and initialize fptr array
	//void (*fptr)() = &testA;
	//void (**fptr)() = &fptrA;
	//fptr[1] = fptrB;
	//fptr[2] = fptrC;

	// Inititialize variables for workflow
	double time_elapsed_in_seconds = 0.0;
	int i, j;

	// Loop through fptr array
	for (j = 0; j < 2; j++) {

		// Run the fuction 100 times and calculate total time elapsed
		for (i = 0; i < 100; i++) {
			clock_t start = clock();
			switch(j) {
				case 0:
					testA();
					break;
				case 1:
					testB();
					break;
			//case 0:
			//	break;
			//case 0:
			//	break;
			}
			clock_t end = clock();
			time_elapsed_in_seconds+=(end - start)/(double)CLOCKS_PER_SEC;
		}

		// Print the after execution time of 100 runs
		printf("Time Elapsed test%d: %f secs\n", j, time_elapsed_in_seconds/100);
	}

	return 0;
}