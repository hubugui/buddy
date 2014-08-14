#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"

static int _test_size(struct buddy *buddy, unsigned int size)
{
    unsigned int offset;
    int rc;

    if ((rc = buddy_alloc(buddy, size, &offset)) == 0)
        printf("%s>size=%d, offset=%d\n", __FUNCTION__, size, offset);
    else
        printf("%s>size=%d fail\n", __FUNCTION__, size);
    buddy_dump(buddy);
    return rc;
}

static int _arg(int argc, char **argv)
{
    int rc = 0;
    struct buddy *buddy;
    unsigned int size;

    if (argc > 1)
        size = atoi(argv[1]);
    else
        size = 1024;

    buddy = buddy_new(size);
    if (!buddy) {
        printf("buddy_new(%d) fail\n", size);
        goto fail;
    }
    buddy_dump(buddy);

    _test_size(buddy, 1);
    _test_size(buddy, 2);
    _test_size(buddy, 3);
    _test_size(buddy, 1);

    buddy_free(buddy, 4);
    buddy_dump(buddy);

    _test_size(buddy, 4);
    _test_size(buddy, 6);
    _test_size(buddy, 7);

    buddy_free(buddy, 8);
    buddy_dump(buddy);

    _test_size(buddy, 4);

    buddy_free(buddy, 0);
    buddy_free(buddy, 1);

    _test_size(buddy, 9);

    printf("success!\n");
    goto end;
fail:
    rc = -1;
end:
    if (buddy)
        buddy_delete(buddy);
    return rc;
}

int main(int argc, char **argv)
{
    return _arg(argc, argv);
}