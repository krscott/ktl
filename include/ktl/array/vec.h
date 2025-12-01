// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef ktl_vec
#include <stdlib.h>
struct dev_vec
{
    int *ptr;
    size_t len;
    size_t cap;
};
#define dev_vec__type int
#define dev_vec__terminated true, 0
#define dev_vec__allocator true
#define dev_vec__realloc(vec, p, size) realloc((p), (size))
#define dev_vec__free(vec, p) free(p)
// #define dev_vec__infallible_alloc true
#define ktl_vec dev_vec
#endif

// Macros

#ifdef ktl_vec_m
#undef ktl_vec_m
#endif
#define ktl_vec_m(x) KTL_TEMPLATE(ktl_vec, x)

#ifdef ktl_vec_T
#undef ktl_vec_T
#endif
#define ktl_vec_T ktl_vec_m(_type)

static_assert(
    _Generic(
        (struct ktl_vec){0}.ptr,
        ktl_vec_T *: 1,
        ktl_vec_T const *: 1,
        default: 0
    ),
    "Wrong type: `#define " KTL_STRINGIFY(ktl_vec) "__type " KTL_STRINGIFY(
        ktl_vec_T
    ) "`"
);

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#ifdef ktl_vec_Tptr
#undef ktl_vec_Tptr
#endif
#define ktl_vec_Tptr ktl_vec_T *

#ifdef ktl_vec_sentinel
#undef ktl_vec_sentinel
#endif
#if KTL_GET0(ktl_vec_m(_terminated))
#define ktl_vec_sentinel KTL_GET1(ktl_vec_m(_terminated), (ktl_marker){0})
static_assert(
    _Generic(ktl_vec_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(ktl_vec) "__terminated 1, <sentinel-value>`"
);
#endif

#ifdef ktl_vec_realloc
#undef ktl_vec_realloc
#undef ktl_vec_free
#endif
#if ktl_vec_m(_allocator)
#define ktl_vec_realloc(vec, p, size) ktl_vec_m(_realloc)((vec), (p), (size))
#define ktl_vec_free(vec, p) ktl_vec_m(_free)((vec), (p))
#else
#define ktl_vec_realloc(vec, p, size) realloc((p), (size))
#define ktl_vec_free(vec, p) free(p)
#endif

#ifdef ktl_vec_alloc_ok
#undef ktl_vec_alloc_ok
#endif
#ifdef ktl_vec_infallible
#undef ktl_vec_infallible
#endif
#if ktl_vec_m(_infallible_alloc)
#define ktl_vec_infallible
#define ktl_vec_alloc_ok void
#else
#define ktl_vec_alloc_ok ktl_nodiscard bool
#endif

KTL_DIAG_POP

// Prototypes

void ktl_vec_m(deinit)(struct ktl_vec *vec);

ktl_vec_alloc_ok ktl_vec_m(reserve)(struct ktl_vec *vec, size_t n);

ktl_vec_alloc_ok
    ktl_vec_m(append)(struct ktl_vec *vec, ktl_vec_T const *arr, size_t n);

#ifdef ktl_vec_sentinel
ktl_vec_alloc_ok
    ktl_vec_m(append_terminated)(struct ktl_vec *vec, ktl_vec_T const *arr);
#endif

ktl_vec_alloc_ok ktl_vec_m(push)(struct ktl_vec *vec, ktl_vec_T elem);
ktl_nodiscard bool ktl_vec_m(pop)(struct ktl_vec *vec, ktl_vec_T *out);
