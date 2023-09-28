#include "stdio.h"
#include "malloc.h"

#define KB 1024

int main() {
    void *p = malloc(8*KB);
    void *q = malloc(8*KB);
    // p(8)-q(8)
    printf("p:%x\n", p);
    printf("q:%x\n", q);

    //_(8)-q(8)
    free(p);

    //a(4)_(4)-q(8)
    void * a = malloc(4*KB);
    printf("a:%x\n",a);

    //a(4)_(4)-q(8)-b(5)
    void *b = malloc(5*KB);
    printf("b:%x\n",b);

    //a(4)_(4)-q(8)_(5)
    free(b);

    //a(4)_(4)_(8)_(5)
    free(q);

    //a(4)_(4)_(8)_(5)
    //a(4)_(17)
    //a(4)_c(16)_(1)
    void *c = malloc(16*KB);

    free(c);
    return 0;
}
