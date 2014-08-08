#ifndef __BUDDY__
#define __BUDDY__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * non-thread safe
 */

struct buddy;

struct buddy *buddy_new(unsigned int size);
void buddy_delete(struct buddy *buddy);

void *buddy_malloc(struct buddy *buddy, unsigned int size);
void buddy_free(struct buddy *buddy, void *p);

void buddy_dump(struct buddy *buddy);
void buddy_size(struct buddy *buddy, unsigned int *size, unsigned int *alloc_size, unsigned int *free_size);

#ifdef __cplusplus
}
#endif
#endif