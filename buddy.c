#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"

struct buddy
{
    unsigned int size, alloc_size, real_size;    
    char *buffer;
    unsigned char *bitmap, depth;
};

static unsigned char _lb(unsigned int size)
{
    unsigned char i;

    for (i = 31; i > 0 && i <= 31; i--) {
        if (size >> i)
            return i;
    }
    return 0;
}

static unsigned int _fixsize(unsigned int size)
{
    unsigned char i;

    for (i = 31; i > 0 && i <= 31; i--) {
        if (size >> i)
            return power(2, i+1);
    }
    return 0;
}

struct buddy *buddy_new(unsigned int size)
{
    struct buddy *buddy;

    /* power of 2 */
    if (size & (size-1) || size < 2)
        return NULL;

    buddy = calloc(1, sizeof(struct buddy));
    buddy->bitmap = calloc(1, sizeof(unsigned char) * size);
    buddy->buffer = malloc(size);
    if (buddy && buddy->bitmap && buddy->buffer) {
        /* mark log(2, size) */
        buddy->bitmap[0] = buddy->depth = _lb(size);
        buddy->size = size;
        buddy->alloc_size = buddy->real_size = 0;
    } else {
        if (buddy->buffer)
            free(buddy->buffer);
        if (buddy->bitmap)
            free(buddy->bitmap);
        if (buddy)
            free(buddy);
        buddy = NULL;
    }
    return buddy;
}

void buddy_delete(struct buddy *buddy)
{
    free(buddy->buffer);
    free(buddy->bitmap);
    free(buddy);
}

void *buddy_malloc(struct buddy *buddy, unsigned int size)
{
    void *p = NULL;
    int i;

    if (size & (size-1))
        size = _fixsize(size);

    for (i = 0; i < buddy->size - 1; i++) {
        if (power(2, buddy->bitmap[i]) >= size) {
            
        } else {

        }
    }
    return p;
}

void buddy_free(struct buddy *buddy, void *p)
{

}

void buddy_dump(struct buddy *buddy)
{
    unsigned int size, alloc_size, free_size;

    buddy_size(buddy, &size, &alloc_size, &free_size);
    printf("size=%d, alloc_size=%d, free_size=%d\n", size, alloc_size, free_size);
}

void buddy_size(struct buddy *buddy, unsigned int *size, unsigned int *alloc_size, unsigned int *free_size)
{
    *size = buddy->size;
    *alloc_size = buddy->alloc_size;
    *free_size = buddy->size - buddy->real_size;
}