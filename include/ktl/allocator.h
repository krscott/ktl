#ifndef KTL_ALLOCATOR_H_
#define KTL_ALLOCATOR_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct ktl_allocator_vtable
{
    void *(*realloc)(void *allocator, void *ptr, size_t size);
    void (*free)(void *allocator, void *ptr);
} ktl_allocator_vtable;

typedef struct ktl_allocator
{
    struct ktl_allocator_vtable const *vtable;
    void *impl;
} ktl_allocator;

static inline void *
ktl_allocator_realloc(ktl_allocator allocator, void *ptr, size_t size)
{
    return allocator.vtable->realloc((void *)allocator.impl, ptr, size);
}

static inline void ktl_allocator_free(ktl_allocator allocator, void *ptr)
{
    allocator.vtable->free((void *)allocator.impl, ptr);
}

#endif
