// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_vec
#include "ktl/allocator.h"
#define dev_vec__local_allocator true, ktl_allocator

#define dev_vec__type int
#define dev_vec__terminated true, 0
// #define dev_vec__infallible_allocator true
#define dev_vec__impl true
#define ktl_vec dev_vec
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_vec_m
#define ktl_vec_m(x) KTL_TEMPLATE(ktl_vec, x)

#undef ktl_vec_T
#define ktl_vec_T ktl_vec_m(_type)

#undef ktl_vec_Tptr
#define ktl_vec_Tptr ktl_vec_T *

#undef ktl_vec_local_allocator
#if KTL_GET0(ktl_vec_m(_local_allocator))
#define ktl_vec_local_allocator                                                \
    KTL_GET1(ktl_vec_m(_local_allocator), ktl_marker)
#endif

#undef ktl_vec_impl
#if ktl_vec_m(_impl)
#define ktl_vec_impl
#endif

KTL_DIAG_POP

// Type

/**
 * Generic vec type
 *
 * Fields:
 * - ptr: Pointer to array
 * - len: Length of array
 * - cap: Allocated capacity
 * - allocator (optional): Allocator handle
 */
typedef struct ktl_vec
{
    ktl_vec_Tptr ptr;
    size_t len;
    size_t cap;
#ifdef ktl_vec_local_allocator
    ktl_vec_local_allocator allocator;
#endif
} ktl_vec;

// Traits

#undef ktl_array
#define ktl_array ktl_vec
#define KTL_ARRAY_MUT
#include "ktl/trait/array.h"
#undef KTL_ARRAY_MUT

#undef ktl_dynarray
#define ktl_dynarray ktl_vec
#include "ktl/trait/dynarray.h"

// Methods

#ifdef ktl_vec_local_allocator
ktl_nodiscard ktl_vec ktl_vec_m(init)(ktl_vec_local_allocator allocator);
#else
ktl_nodiscard ktl_vec ktl_vec_m(init)(void);
#endif

void ktl_vec_m(deinit)(ktl_vec *vec);

//
// IMPLEMENTATION
//

#ifdef ktl_vec_impl

#ifdef ktl_vec_local_allocator
ktl_nodiscard ktl_vec ktl_vec_m(init)(ktl_vec_local_allocator const allocator)
{
    return (ktl_vec){.allocator = allocator};
}
#else
ktl_nodiscard ktl_vec ktl_vec_m(init)(void)
{
    return (ktl_vec){0};
}
#endif

void ktl_vec_m(deinit)(ktl_vec *const vec)
{
    ktl_allocates_m(_free)(vec, vec->ptr);
}

#endif // ktl_vec_impl
