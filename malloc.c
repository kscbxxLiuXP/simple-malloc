#include "malloc.h"
#include "unistd.h"
#include "stdio.h"

MCB *head;

/**
 * +--------------------------------+
 * |  block(size)  |    new block   |
 * +--------------------------------+
 * 将一个大的block分割成size大小的block和一个空的block(含MCB)
 * 临界情况：
 * 1. block是最后一个block
 * @param block
 */
void split_block(MCB *block, size_t size) {

    //新的block大小 = 剩余空间(block.size - size) -MCB
    //新的block的位置= blockMCB +size
    MCB *newblock = (MCB *) ((char *) block + sizeof(MCB) + size);

    //改链表指针
    newblock->next = block->next;
    block->next = newblock;
    newblock->prev = block;

    //newblock->next ==NULL
    if (newblock->next) {
        newblock->next->prev = newblock;
    }

    newblock->size = block->size - size - sizeof(MCB);
    newblock->free = 1;

    block->size = size;
    block->free = 0;
}

/**
 * merge block1 and block2
 * @param block1  first block
 * @param block2  second block
 */
MCB *merge(MCB *block1, MCB *block2) {
    block1->size += block2->size + sizeof(MCB);
    block1->next = block2->next;
    if (block2->next) block2->next->prev = block1;
    return block1;
}

/**
 * 将当前block和前后的空block合并为一个大block
 * @param block
 */
MCB *merge_block(MCB *block) {
    if (block->prev && block->prev->free) block = merge(block->prev, block);
    if (block->next && block->next->free) block = merge(block, block->next);
    return block;
}

/**
 * malloc 所做的就是
 * 1. 查找是否有空的block，如果有则在此block分配内存，并且split block
 * 2. 直到遍历完了，如果还没有的话，则新建
 * @param size
 * @return
 */
void *malloc(size_t size) {
    MCB *mem = head;
    MCB *prev = NULL;
    while (mem != NULL) {
        //当前block空 且 空间可以放下
        if (mem->size >= size && mem->free) {
            //如果剩余的空间能够放下 MCB，则分割
            if (mem->size > size + sizeof(MCB)) {
                split_block(mem, size);
            }
            break;
        }
        prev = mem;
        mem = mem->next;
    }

    //遍历完内存池之后，没有空间，需要 call 系统调用sbrk()来分配
    //要分配的空间是 MCB + size
    //考虑临界情况，mem是head
    if (mem == NULL) {
        //分配新的block
        mem = (MCB *) sbrk(sizeof(MCB) + size);
        if (mem == (void *) -1) {
            return NULL;
        }
        mem->size = size;
        mem->free = 0;

        //mem是head,新分配的block就是head
        if (prev == NULL) {
            head = mem;

        } else {
            //mem不是head，修改指针
            prev->next = mem;
            mem->prev = prev;
        }
    }
    if(SHOW_MEM){
        showMem();
    }
    return (void *) ((char *) mem + sizeof(MCB));
}

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    //地址 - MCB size 得到mcb
    MCB *block = (MCB *) ((char *) ptr - sizeof(MCB));
    block = merge_block(block);
    block->free = 1;

    if(SHOW_MEM){
        showMem();
    }
}

/**
 * [ used 1 ] -> [ free 2 ]
 */
void showMem() {
    int first = 1;
    MCB *m = head;
    while (m) {
        if (!first) {
            printf(" -> ");
        }
        if (m->free) {
            printf("[ free %d ]", m->size);

        } else {
            printf("[ used %d ]", m->size);
        }
        first = 0;
        m = m->next;
    }
    printf("\n");
}