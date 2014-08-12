#ifndef __BUDDY__
#define __BUDDY__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * non-thread safe
 */

#ifndef MAX
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

struct buddy;

struct buddy *buddy_new(unsigned int size);
void buddy_delete(struct buddy *buddy);

int buddy_alloc(struct buddy *buddy, unsigned int size, unsigned int *offset);
void buddy_free(struct buddy *buddy, unsigned int offset);

unsigned int buddy_size(struct buddy *buddy, unsigned int *free_size);
void buddy_dump(struct buddy *buddy);

#ifdef __cplusplus
}
#endif
#endif