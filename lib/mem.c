#include <sys/types.h>
#include <string.h>
#include <malloc.h>

#define HEAP_SIZE 3 * 1024 * 1024
char heap[HEAP_SIZE];
char *heap_ptr = heap;

void *sbrk(intptr_t increment) {
    char *hptr = heap_ptr;
    heap_ptr += (increment + sizeof(void*)) & ~(sizeof(void*) - 1);
    return hptr;
}
