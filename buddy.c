#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"

#define POW_OF_2(i)         (((i) & ((i)-1)) == 0)

#define LEFT_INDEX(i)       (2 * (i) + 1)
#define RIGHT_INDEX(i)      (LEFT_INDEX(i)+1)
#define PARENT_IDX(i)       ((((i) % 2) == 0) ? ((i) / 2 - 1) : (((i)+1) / 2 - 1))

#define SET_BITMAP(b, i, v) (b->bitmap[(i)] = v)
#define GET_BITMAP(b, i)    (b->bitmap[(i)])                            

#define MAX_CHILD(b, i)     MAX(GET_BITMAP(b, LEFT_INDEX(i)), GET_BITMAP(b, RIGHT_INDEX(i)))
#define SET_MAX_CHILD(b, i) SET_BITMAP(b, i, MAX_CHILD(b, i))
#define EQUAL_CHILD(b, i) (GET_BITMAP(b, LEFT_INDEX(i)) == GET_BITMAP(b, RIGHT_INDEX(i)))
#define SUM_CHILD(b, i)     (GET_BITMAP(b, LEFT_INDEX(i)) + GET_BITMAP(b, RIGHT_INDEX(i)))

struct buddy
{
    unsigned int size, alloc_size;    
    unsigned char depth;
    unsigned char bitmap[0];
};

static unsigned char _lb(unsigned int size)
{
    unsigned char i;

    for (i = sizeof(size) * 8 - 1; i > 0 && i <= sizeof(size) * 8 - 1; i--) {
        if (size >> i)
            return i;
    }
    return 0;
}

static unsigned int _fixsize(unsigned int size)
{
    unsigned char i;

    for (i = sizeof(size) * 8 - 1; i > 0 && i <= sizeof(size) * 8 - 1; i--) {
        if (size >> i)
            return (unsigned int) pow(2, i+1);
    }
    return 0;
}

struct buddy *buddy_new(unsigned int size)
{
    struct buddy *buddy;
    unsigned int i;
    unsigned char depth;

    if (!POW_OF_2(size) || size < 2)
        return NULL;

    buddy = malloc(sizeof(struct buddy) + sizeof(unsigned char) * (2*size-1));
    if (buddy) {
        /* mark log(2, size) */
        depth = buddy->depth = _lb(size);
        buddy->alloc_size = 0;
        buddy->size = size;
        for (i = 0; i < size - 1; i++) {
            SET_BITMAP(buddy, i, depth);
            if (POW_OF_2(i+2))
                depth--;
        }
    }
    return buddy;
}

void buddy_delete(struct buddy *buddy)
{
    free(buddy);
}

int buddy_alloc(struct buddy *buddy, unsigned int size, unsigned int *offset)
{
    unsigned char depth, alloc_depth;
    int i, p_idx;

    if (size == 0)
        return -1;
    if (!POW_OF_2(size))
        size = _fixsize(size);
    alloc_depth = _lb(size);
    if (GET_BITMAP(buddy, 0) < alloc_depth)
        return -1;

    buddy->alloc_size += size;
    for (i = 0, depth = buddy->depth; depth != alloc_depth; depth--) {
        if (GET_BITMAP(buddy, LEFT_INDEX(i)) >= alloc_depth)
            i = LEFT_INDEX(i);
        else
            i = RIGHT_INDEX(i);
    }
    SET_BITMAP(buddy, i, 0);

    for (p_idx = i; p_idx;) {
        p_idx = PARENT_IDX(p_idx);
        SET_MAX_CHILD(buddy, p_idx);
    }
    *offset = pow(2, depth) * (i + 1 - pow(2, (buddy->depth - depth)));
    return 0;
}

void buddy_free(struct buddy *buddy, unsigned int offset)
{
    unsigned int i, p_idx = pow(2, buddy->depth) - 1 + offset;

    /* last level */
    SET_BITMAP(buddy, p_idx, 0);
    for (i = 0; p_idx; i++) {
        p_idx = PARENT_IDX(p_idx);
        if (EQUAL_CHILD(buddy, p_idx))
            SUM_BITMAP(buddy, p_idx, i+1);
        else
            SET_MAX_CHILD(buddy, p_idx);        
    }
}

unsigned int buddy_size(struct buddy *buddy, unsigned int *free_size)
{
    *free_size = buddy->size - buddy->alloc_size;
    return buddy->size;
}

void buddy_dump(struct buddy *buddy)
{
    unsigned int i, size, free_size;

    for (i = 0; i < 2 * buddy->size - 1; i++) {
        if (POW_OF_2(i+1))
            printf("%s>", __FUNCTION__);
        printf("%d=2^%d,", i, GET_BITMAP(buddy, i));
        if (POW_OF_2(i+2))
            printf("\n");
    }
    size = buddy_size(buddy, &free_size);
    printf("%s>size=%d, free_size=%d\n", __FUNCTION__, size, free_size);
}