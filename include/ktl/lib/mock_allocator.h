#ifndef KTL_MOCK_ALLOCATOR_H_
#define KTL_MOCK_ALLOCATOR_H_

#include "ktl/allocator.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    bool fail;
} mock_allocator;

static void *mock_allocator_realloc(void *impl, void *ptr, size_t size)
{
    assert(impl);
    return ((mock_allocator *)impl)->fail ? NULL : realloc(ptr, size);
}

static void mock_allocator_free(void *impl, void *ptr)
{
    (void)impl;
    free(ptr);
}

static ktl_allocator_vtable const mock_allocator_vtable = {
    .realloc = mock_allocator_realloc,
    .free = mock_allocator_free,
};

static mock_allocator mock_allocator_init(void)
{
    return (mock_allocator){0}; //
}

static ktl_allocator mock_allocator_handle(mock_allocator *impl)
{
    return (ktl_allocator){
        .vtable = &mock_allocator_vtable,
        .impl = (void *)impl,
    };
}

#endif
