#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

void testB();

// MACROS
#define malloc(size) mymalloc( size, __FILE__, __LINE__)
#define free(ptr) myfree( ptr, __FILE__, __LINE__)

/* testA: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one */
void testA() {

}

/* testB: malloc() 1 byte and immediately free it 3000 times in a row */
void testB() {
	int i = 0;

	while (i < 3000) {
		int * p;
		p = (int *)malloc(sizeof(int)*10);
		free(p);
		i++;
	}
}

int main(int argc, char const *argv[]) {

	printf("testB\n");
	double time_elapsed_in_seconds = 0.0;
	int i = 0;

	while (i < 100) {
		clock_t start = clock();
		testB();
		clock_t end = clock();
		i++;
		time_elapsed_in_seconds+=(end - start)/(double)CLOCKS_PER_SEC;
	}

	printf("Time Elapsed: %f secs\n", time_elapsed_in_seconds/100);

	return 0;
}