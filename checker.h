
#ifndef CHECKER_H
#define CHECKER_H


#include <stdlib.h>

void* myMalloc(size_t size, char* fileName, size_t line);

void myFree(void* ptr, char* fileName, size_t line);

#define malloc(size) myMalloc(size, __FILE__, __LINE__)

#define free(ptr) myFree(ptr, __FILE__, __LINE__)


void check_memory_leak();

#endif
