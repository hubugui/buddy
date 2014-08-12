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
        printf("%s> size=%d, offset=%d\n", __FUNCTION__, size, offset);
    else
        printf("%s> size=%d fail\n", __FUNCTION__, size);
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

    assert(!_test_size(buddy, 1));
    assert(!_test_size(buddy, 2));
    assert(!_test_size(buddy, 3));
    assert(!_test_size(buddy, 4));
    assert(!_test_size(buddy, 4));
    //assert(!_test_size(buddy, 1));

    buddy_dump(buddy);
    printf("success!\n");
success:
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