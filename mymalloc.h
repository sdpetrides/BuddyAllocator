#ifndef MYMALLOC_H
#define MYMALLOC_H

// Globals
static char myblock[8192];

// Functions
void init_block();
double log2(double d);
int size_to_n(size_t reqSize);
int jump_next(int n, int pos);
int jump_back(int n, int pos);
int is_left(int n, int pos);
void merge(int pos, int pos2, int n);
void * mymalloc(size_t reqSize, char * file, int line);
void myfree(void * ptr, char * file, int line);

#endif