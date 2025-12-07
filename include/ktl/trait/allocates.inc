
#include "ktl/trait/allocates.h"

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

ktl_nodiscard void *
ktl_allocates_m(_realloc)(ktl_allocates *self, void *ptr, size_t size)
{
#ifdef ktl_allocates_local_allocator
    return KTL_TEMPLATE(ktl_allocates_local_allocator, realloc)(
        self->allocator,
        ptr,
        size
    );
#elif defined(ktl_allocates_global_allocator)
    (void)self;
    return KTL_TEMPLATE(KTL_GET1(ktl_allocates_m(_global_allocator)), realloc)(
        ptr,
        size
    );
#else
    (void)self;
    return realloc(ptr, size);
#endif
}

void ktl_allocates_m(_free)(ktl_allocates *self, void *ptr)
{
#ifdef ktl_allocates_local_allocator
    KTL_TEMPLATE(ktl_allocates_local_allocator, free)(self->allocator, ptr);
#elif defined(ktl_allocates_global_allocator)
    (void)self;
    KTL_TEMPLATE(KTL_GET1(ktl_allocates_m(_global_allocator)), free)(ptr);
#else
    (void)self;
    free(ptr);
#endif
}
