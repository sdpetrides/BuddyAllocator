#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

// MACROS
#define malloc(size) mymalloc( size, __FILE__, __LINE__)
#define free(ptr) myfree( ptr, __FILE__, __LINE__)

int main(int argc, char const *argv[]) {
	int * p;

	p = (int *)malloc(sizeof(int)*10);

	free(p);

	return 0;
}