#ifndef MOCK_ALLOCATOR_H_
#define MOCK_ALLOCATOR_H_

#include <stdbool.h>
#include <stdlib.h>

struct mock_allocator
{
    bool fail;
};
static void *
mock_allocator_realloc(struct mock_allocator *allocator, void *ptr, size_t size)
{
    return allocator->fail ? NULL : realloc(ptr, size);
}
static void mock_allocator_free(struct mock_allocator *allocator, void *ptr)
{
    (void)allocator;
    free(ptr);
}

#endif
