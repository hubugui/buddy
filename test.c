#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"

static int _arg(int argc, char **argv)
{
    int rc = 0;
    struct buddy *buddy = buddy_new(atoi(argv[1]));

    if (!buddy) {
        printf("buddy_new() fail\n");
        goto fail;
    }

    assert(buddy_malloc(buddy, 1));
    assert(buddy_malloc(buddy, 3));
    assert(buddy_malloc(buddy, 4));
    assert(buddy_malloc(buddy, 7));
    assert(buddy_malloc(buddy, 8));
    assert(buddy_malloc(buddy, 15));

    printf("success!\n");
fail:
    buddy_delete(buddy);
    return rc;
}

int main(int argc, char **argv)
{
    return _arg(argc, argv);
}