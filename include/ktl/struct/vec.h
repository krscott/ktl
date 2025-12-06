// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_vec
typedef struct
{
    char dummy;
} dev_allocator;
#define dev_vec__type int
#define dev_vec__terminated true, 0
#define dev_vec__custom_allocator true
#define dev_vec__local_allocator true, dev_allocator
#define dev_vec__realloc(vec, p, size) realloc((p), (size))
#define dev_vec__free(vec, p) free(p)
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

#undef ktl_vec_sentinel
#if KTL_GET0(ktl_vec_m(_terminated))
#define ktl_vec_sentinel KTL_GET1(ktl_vec_m(_terminated), (ktl_marker){0})
#endif

#undef ktl_vec_realloc
#undef ktl_vec_free
#if KTL_GET0(ktl_vec_m(_custom_allocator)) ||                                  \
    KTL_GET0(ktl_vec_m(_local_allocator))
#define ktl_vec_realloc(vec, p, size) ktl_vec_m(_realloc)((vec), (p), (size))
#define ktl_vec_free(vec, p) ktl_vec_m(_free)((vec), (p))
#else
#define ktl_vec_realloc(vec, p, size) realloc((p), (size))
#define ktl_vec_free(vec, p) free(p)
#endif

#undef ktl_vec_local_allocator
#if KTL_GET0(ktl_vec_m(_local_allocator))
#define ktl_vec_local_allocator                                                \
    KTL_GET1(ktl_vec_m(_local_allocator), ktl_marker)
#endif

#undef ktl_vec_alloc_ok
#undef ktl_vec_infallible
#if ktl_vec_m(_infallible_allocator)
#define ktl_vec_infallible
#define ktl_vec_alloc_ok void
#else
#define ktl_vec_alloc_ok ktl_nodiscard bool
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

// Prototypes

void ktl_vec_m(deinit)(ktl_vec *vec);

// Traits

#define ktl_dynarray ktl_vec
#include "ktl/trait/dynarray.h"
#undef ktl_dynarray

#define ktl_array ktl_vec
#include "ktl/trait/array.h"
#undef ktl_array

//
// IMPLEMENTATION
//

#ifdef ktl_vec_impl

void ktl_vec_m(deinit)(ktl_vec *const vec) { ktl_vec_free(vec, vec->ptr); }

#endif // ktl_vec_impl
