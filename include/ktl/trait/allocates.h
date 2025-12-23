// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_allocates
#include "ktl/lib/allocator.h"
#define dev_thing__local_allocator true, ktl_allocator

// #define sys_allocator_realloc realloc
// #define sys_allocator_free free
// #define dev_thing__global_allocator true, sys_allocator

typedef struct
{
    ktl_allocator allocator;
} dev_thing;
#define ktl_allocates dev_thing
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_allocates_m
#define ktl_allocates_m(x) KTL_TEMPLATE(ktl_allocates, x)

#undef ktl_allocates_local_allocator
#undef ktl_allocates_global_allocator
#if KTL_GET0(ktl_allocates_m(_local_allocator))
#if KTL_GET0(ktl_allocates_m(_global_allocator))
#error "Cannot specify more than one allocator"
#endif
#define ktl_allocates_local_allocator                                          \
    KTL_GET1(ktl_allocates_m(_local_allocator), ktl_marker)
#elif KTL_GET0(ktl_allocates_m(_global_allocator))
#define ktl_allocates_global_allocator
#endif

#undef ktl_allocates_retval
#undef ktl_allocates_infallible
#if ktl_allocates_m(_infallible_allocator)
#define ktl_allocates_infallible
#define ktl_allocates_retval void
#else
#define ktl_allocates_retval ktl_nodiscard bool
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

#ifdef ktl_allocates_local_allocator
ktl_assert_has_field(ktl_allocates, ktl_allocates_local_allocator, allocator);
#endif

// Methods

ktl_nodiscard void *
    ktl_allocates_m(_realloc)(ktl_allocates *self, void *ptr, size_t size);

void ktl_allocates_m(_free)(ktl_allocates *self, void *ptr);

static inline ktl_nodiscard void *
ktl_allocates_m(_alloc)(ktl_allocates *self, size_t size)
{
    return ktl_allocates_m(_realloc)(self, NULL, size);
}

static inline ktl_nodiscard void *
ktl_allocates_m(_alloc_zero)(ktl_allocates *self, size_t size)
{
    void *const ptr = ktl_allocates_m(_alloc)(self, size);
    if (ptr)
    {
        (void)memset(ptr, 0, size);
    }
    return ptr;
}

#endif // KTL_INC
