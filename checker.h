
#ifndef CHECKER_H
#define CHECKER_H

#define malloc(size) myMalloc(size, __FILE__, __LINE__)

#define free(ptr) myFree(ptr, __FILE__, __LINE__)


void check_memory_leak();

#endif
