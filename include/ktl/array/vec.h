// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_vec
struct dev_allocator
{
    char dummy;
};
#define dev_vec__type int
#define dev_vec__terminated true, 0
#define dev_vec__custom_allocator true
#define dev_vec__local_allocator true, struct dev_allocator
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
struct ktl_vec
{
    ktl_vec_Tptr ptr;
    size_t len;
    size_t cap;
#ifdef ktl_vec_local_allocator
    ktl_vec_local_allocator allocator;
#endif
};

#ifdef ktl_vec_sentinel
static_assert(
    _Generic(ktl_vec_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_vec
    ) "__terminated true, <sentinel-value>`"
);
#endif
#ifdef ktl_vec_local_allocator
static_assert(
    _Generic(((struct ktl_vec *)0)->allocator, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_vec
    ) "__local_allocator true, <allocator-type>`"
);
#endif

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

// Common

#define ktl_array ktl_vec
#include "ktl/array/common.h"
#undef ktl_array

//
// IMPLEMENTATION
//

#ifdef ktl_vec_impl

#ifndef KTL_VEC_GROW_NUMERATOR
#define KTL_VEC_GROW_NUMERATOR 3
#define KTL_VEC_GROW_DENOMINATOR 2
#endif

#ifndef KTL_VEC_MIN_CAP
#define KTL_VEC_MIN_CAP 8
#endif

#ifndef KTL_VEC_GROW_CAP
#define KTL_VEC_GROW_CAP(cap)                                                  \
    ((cap) / KTL_VEC_GROW_DENOMINATOR * KTL_VEC_GROW_NUMERATOR)
#endif

void ktl_vec_m(deinit)(struct ktl_vec *const vec)
{
    ktl_vec_free(vec, vec->ptr);
}

ktl_vec_alloc_ok ktl_vec_m(reserve)(struct ktl_vec *const vec, size_t const n)
{
    assert(vec->ptr || (vec->len == 0 && vec->cap == 0));

    size_t const CAP_LIMIT =
        SIZE_MAX / KTL_VEC_GROW_NUMERATOR * KTL_VEC_GROW_DENOMINATOR;

    bool success;

    size_t const target_cap = vec->len + n
#ifdef ktl_vec_sentinel
                              + 1
#endif
        ;

    if (target_cap < vec->len + 1)
    {
        // overflow
        success = false;
    }
    else if (target_cap <= vec->cap)
    {
        success = true;
    }
    else if (vec->cap >= CAP_LIMIT)
    {
        success = false;
    }
    else
    {
        size_t new_cap = vec->cap < KTL_VEC_MIN_CAP
                             ? KTL_VEC_MIN_CAP
                             : KTL_VEC_GROW_CAP(vec->cap);

        // Guaranteed to terminate due to CAP_LIMIT check above
        while (new_cap < target_cap)
        {
            new_cap = KTL_VEC_GROW_CAP(new_cap);
        }

        void *const new_ptr =
            ktl_vec_realloc(vec, vec->ptr, new_cap * sizeof(vec->ptr[0]));

        if (new_ptr)
        {
            success = true;
            vec->ptr = (ktl_vec_Tptr)new_ptr;
            vec->cap = new_cap;
        }
        else
        {
            success = false;
        }
    }

#ifdef ktl_vec_infallible
    if (!success)
    {
        assert(success && "vec reserve failed");
        exit(1);
    }
#else
    return success;
#endif
}

ktl_vec_alloc_ok ktl_vec_m(append)(
    struct ktl_vec *const vec, ktl_vec_T const *const arr, size_t const n
)
{
#ifdef ktl_vec_infallible

    ktl_vec_m(reserve)(vec, n);
    memmove(&vec->ptr[vec->len], arr, sizeof(vec->ptr[0]) * n);
    vec->len += n;

#ifdef ktl_vec_sentinel
    vec->ptr[vec->len] = ktl_vec_sentinel;
#endif

#else

    bool const success = ktl_vec_m(reserve)(vec, n);

    if (success)
    {
        memmove(&vec->ptr[vec->len], arr, sizeof(vec->ptr[0]) * n);
        vec->len += n;

#ifdef ktl_vec_sentinel
        vec->ptr[vec->len] = ktl_vec_sentinel;
#endif
    }

    return success;

#endif
}

#ifdef ktl_vec_sentinel
ktl_vec_alloc_ok
ktl_vec_m(append_terminated)(struct ktl_vec *vec, ktl_vec_T const *arr)
{
    size_t arr_len = 0;
    while (arr[arr_len] != ktl_vec_sentinel)
    {
        ++arr_len;
    }

#ifdef ktl_vec_infallible
    ktl_vec_m(append)(vec, arr, arr_len);
#else
    return ktl_vec_m(append)(vec, arr, arr_len);
#endif
}
#endif

ktl_vec_alloc_ok
ktl_vec_m(push)(struct ktl_vec *const vec, ktl_vec_T const elem)
{
#ifdef ktl_vec_infallible

    ktl_vec_m(reserve)(vec, 1);
    vec->ptr[vec->len++] = elem;
#ifdef ktl_vec_sentinel
    vec->ptr[vec->len] = ktl_vec_sentinel;
#endif

#else

    bool const ok = ktl_vec_m(reserve)(vec, 1);
    if (ok)
    {
        vec->ptr[vec->len++] = elem;
#ifdef ktl_vec_sentinel
        vec->ptr[vec->len] = ktl_vec_sentinel;
#endif
    }
    return ok;

#endif
}

ktl_nodiscard bool
ktl_vec_m(pop)(struct ktl_vec *const vec, ktl_vec_T *const out)
{
    bool const ok = vec->len > 0;
    if (ok)
    {
        *out = vec->ptr[--(vec->len)];
#ifdef ktl_vec_sentinel
        vec->ptr[vec->len] = ktl_vec_sentinel;
#endif
    }
    return ok;
}

#endif // ktl_vec_impl
