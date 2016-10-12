#ifndef MYMALLOC_H
#define MYMALLOC_H

// Globals

// Structs

// Functions
double log2(double d);
int size_to_n(size_t reqSize);
int jump_next(int n, int pos);
void init_block();
void * mymalloc(size_t reqSize, char * file, int line);
void myfree(void * ptr, char * file, int line);

#endif