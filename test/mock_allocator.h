#ifndef MOCK_ALLOCATOR_H_
#define MOCK_ALLOCATOR_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    bool fail;
} mock_allocator;
static void *
mock_allocator_realloc(mock_allocator *allocator, void *ptr, size_t size)
{
    return allocator->fail ? NULL : realloc(ptr, size);
}
static void mock_allocator_free(mock_allocator *allocator, void *ptr)
{
    (void)allocator;
    free(ptr);
}

#endif
