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
	char * p;

	while (i < 100) {
		p = (char *)malloc(sizeof(char)*1);
		i++;
	}

	i = 0;
	
	while (i < 100) {
		free(p);
		i++;
	}

	return;
}

/* testB: malloc() 1 byte and immediately free it 3000 times in a row */
void testB() {
	int i = 0;

	while (i < 100) {
		char * p;
		p = (char *)malloc(sizeof(char)*1);
		free(p);
		i++;
	}

	return;
}

/* testA: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one */
void testC() {
	int i = 0;
	char * p;

	while (i < 10) {
		int random = rand() % 100;
		p = (char *)malloc(sizeof(char)*random);
		i++;
	}

	i = 0;
	
	while (i < 10) {
		free(p);
		i++;
	}

	return;
}

int main(int argc, char const *argv[]) {

	// Initialize myblock
	init_block();

	// Create function pointers for each function
	//void (*fptrA)() = &testA;
	//void (*fptrB)() = &testB;
	void (*fptrC)() = &testC;

	// Create and initialize fptr array
	void (**fptr)() = &fptrC;
	//fptr[1] = fptrB;
	//fptr[2] = fptrC;

	// Inititialize variables for workflow
	double time_elapsed_in_seconds = 0.0;
	int i, j;

	// Loop through fptr array
	for (j = 0; j < 1; j++) {

		// Run the fuction 100 times and calculate total time elapsed
		for (i = 0; i < 10; i++) {
			clock_t start = clock();
			(*fptr[j])();
			clock_t end = clock();
			time_elapsed_in_seconds+=(end - start)/(double)CLOCKS_PER_SEC;
		}

		// Print the after execution time of 100 runs
		printf("Time Elapsed: %f secs\n", time_elapsed_in_seconds/10);
	}

	return 0;
}