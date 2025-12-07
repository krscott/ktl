#ifndef KTL_CLIB_ALLOCATOR_H_
#define KTL_CLIB_ALLOCATOR_H_

#include "ktl/allocator.h"
#include <stddef.h>
#include <stdlib.h>

static void *clib_allocator_realloc(void *impl, void *ptr, size_t size)
{
    (void)impl;
    return realloc(ptr, size);
}

static void clib_allocator_free(void *impl, void *ptr)
{
    (void)impl;
    free(ptr);
}

static ktl_allocator_vtable const clib_allocator_vtable = {
    .realloc = clib_allocator_realloc,
    .free = clib_allocator_free,
};
static ktl_allocator const clib_allocator = {
    .vtable = &clib_allocator_vtable,
    .impl = NULL,
};

#endif
