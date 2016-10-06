#ifndef MYMALLOC_H
#define MYMALLOC_H

// Globals

// Structs

// Functions
void * mymalloc(size_t reqSize, char * file, int line);
void myfree(void * ptr, char * file, int line);

#endif