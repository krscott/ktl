// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_dynarray
#include "ktl/lib/allocator.h"
#define dev_dynarray__local_allocator true, ktl_allocator

// #define sys_allocator_realloc realloc
// #define sys_allocator_free free
// #define dev_dynarray__global_allocator true, sys_allocator

#define dev_dynarray__type int
#define dev_dynarray__terminated true, 0
// #define dev_dynarray__infallible_allocator true
#define ktl_dynarray dev_dynarray
typedef struct
{
    dev_dynarray__type *ptr;
    size_t len;
    size_t cap;
#ifdef dev_dynarray__local_allocator
    ktl_allocator allocator;
#endif
} dev_dynarray;
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_dynarray_m
#define ktl_dynarray_m(x) KTL_TEMPLATE(ktl_dynarray, x)

#undef ktl_dynarray_T
#define ktl_dynarray_T ktl_dynarray_m(_type)

#undef ktl_dynarray_Tptr
#define ktl_dynarray_Tptr ktl_dynarray_T *

#undef ktl_dynarray_sentinel
#if KTL_GET0(ktl_dynarray_m(_terminated))
#define ktl_dynarray_sentinel                                                  \
    KTL_GET1(ktl_dynarray_m(_terminated), (ktl_marker){0})
#endif

// TODO: Use allocates.h's version of this instead
#undef ktl_dynarray_alloc_ok
#undef ktl_dynarray_infallible
#if ktl_dynarray_m(_infallible_allocator)
#define ktl_dynarray_infallible
#define ktl_dynarray_alloc_ok void
#else
#define ktl_dynarray_alloc_ok ktl_nodiscard bool
#endif

#undef ktl_allocates
#define ktl_allocates ktl_dynarray
#include "ktl/trait/allocates.h"

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

ktl_assert_has_field(ktl_dynarray, ktl_dynarray_Tptr, ptr);
ktl_assert_has_field(ktl_dynarray, size_t, len);
ktl_assert_has_field(ktl_dynarray, size_t, cap);

#ifdef ktl_dynarray_sentinel
static_assert(
    _Generic(ktl_dynarray_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_dynarray
    ) "__terminated true, <sentinel-value>`"
);
#endif
#ifdef ktl_dynarray_local_allocator
static_assert(
    _Generic(((ktl_dynarray *)0)->allocator, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_dynarray
    ) "__local_allocator true, <allocator-type>`"
);
#endif

// Methods

ktl_dynarray_alloc_ok
    ktl_dynarray_m(reserve)(ktl_dynarray *dynarray, size_t additional);
ktl_dynarray_alloc_ok ktl_dynarray_m(resize)(
    ktl_dynarray *dynarray, size_t new_len, ktl_dynarray_T value
);

ktl_dynarray_alloc_ok ktl_dynarray_m(append)(
    ktl_dynarray *dynarray, ktl_dynarray_T const *arr, size_t n
);

#ifdef ktl_dynarray_sentinel
ktl_dynarray_alloc_ok ktl_dynarray_m(append_terminated)(
    ktl_dynarray *dynarray, ktl_dynarray_T const *arr
);
#endif

ktl_dynarray_alloc_ok
    ktl_dynarray_m(push)(ktl_dynarray *dynarray, ktl_dynarray_T value);
ktl_nodiscard bool
    ktl_dynarray_m(pop)(ktl_dynarray *dynarray, ktl_dynarray_T *out);

void ktl_dynarray_m(clear)(ktl_dynarray *dynarray);

void ktl_dynarray_m(swap_remove)(ktl_dynarray *dynarray, size_t i);

#endif // KTL_INC
