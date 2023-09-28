
#include "stddef.h"
#define SHOW_MEM 1
void *malloc(size_t size);

void free(void *ptr);
void showMem();
/**
 * memory control block
 * @param size
 * @param free
 * @param next
 */
typedef struct MCB {
    size_t size;
    int free;
    struct MCB *prev;
    struct MCB *next;
}MCB;

