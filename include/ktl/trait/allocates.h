// No header guard - repeatable include

#include "ktl/allocator.h"
#include "ktl/macros.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Defaults (dev-only)

#ifndef ktl_allocates
#define sys_allocator_realloc realloc
#define sys_allocator_free free

#define dev_thing__local_allocator true
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
#if ktl_allocates_m(_local_allocator)
#if KTL_GET0(ktl_allocates_m(_global_allocator))
#error "Cannot specify more than one allocator"
#endif
#define ktl_allocates_local_allocator
#elif KTL_GET0(ktl_allocates_m(_global_allocator))
#define ktl_allocates_global_allocator
#endif

KTL_DIAG_POP

// Checks

#ifdef ktl_allocates_local_allocator
ktl_assert_has_field(ktl_allocates, ktl_allocator, allocator);
#endif

// Methods

static inline void *
ktl_allocates_m(_realloc)(ktl_allocates *self, void *ptr, size_t size)
{
#ifdef ktl_allocates_local_allocator
    return ktl_allocator_realloc(self->allocator, ptr, size);
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
static inline void ktl_allocates_m(_free)(ktl_allocates *self, void *ptr)
{
#ifdef ktl_allocates_local_allocator
    ktl_allocator_free(self->allocator, ptr);
#elif defined(ktl_allocates_global_allocator)
    (void)self;
    KTL_TEMPLATE(KTL_GET1(ktl_allocates_m(_global_allocator)), free)(ptr);
#else
    (void)self;
    free(ptr);
#endif
}
