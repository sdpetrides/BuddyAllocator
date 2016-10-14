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

/* testC: Randomly choose between a 1 byte malloc() or free() 6000 times */ 
void testC() {
	srand(time(NULL));
	int i = 1;
	int j = 0;
	char * p[6000];

	p[0] = (char *)malloc(sizeof(char)*1);	

	while (i+j < 6000) {
		int random = rand() % 2;

		if (random != 0) {
			p[i] = (char *)malloc(sizeof(char)*1);
			i++;
		} else if (p[j] != 0) {
			free(p[j]);
			j++;
		} else {
			p[i] = (char *)malloc(sizeof(char)*1);
			i++;
		}
	}

	while (j <= i) {
		free(p[j]);
		j++;
	}

	return;
}

/* testD: Randomly choose between a randomly-sized malloc() or free 6000 times */
void testD() {
	srand(time(NULL));
	int i = 1;
	int j = 0;
	char * p[6000];

	p[0] = (char *)malloc(sizeof(char)*(2));	

	while (i+j < 6000) {
		int random = rand() % 2;

		if (random != 0) {
			p[i] = (char *)malloc(sizeof(char)*(2));
			i++;
		} else if (p[j] != 0) {
			free(p[j]);
			j++;
		} else {
			p[i] = (char *)malloc(sizeof(char)*(2));
			i++;
		}
	}

	while (j <= i) {
		free(p[j]);
		j++;
	}

	return;
} 

int main(int argc, char const *argv[]) {

	// Inititialize variables for workflow
	int i, j;

	// Initialize myblock
	init_block();

	// Loop through fptr array
	for (j = 0; j < 4; j++) {

		// Initialize time elapsed
		double time_elapsed_in_seconds = 0.0;

		// Run the fuction 100 times and calculate total time elapsed
		for (i = 0; i < 1; i++) {

			clock_t start = clock();
			switch(j) {
				case 0:
					testA();
					break;
				case 1:
					testB();
					break;
				case 2:
					testC();
					break;
				case 3:
					testD();
					break;
				case 4:
					//testE();
					break;
				case 5:
					//testF();
					break;
			}
			clock_t end = clock();
			time_elapsed_in_seconds+=(end - start)/(double)CLOCKS_PER_SEC;
		}

		// Print the after execution time of 100 runs
		printf("Time Elapsed test%d: %f secs\n", j, time_elapsed_in_seconds/1);
	}

	return 0;
}