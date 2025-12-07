// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_dynarray
#include "ktl/allocator.h"
#define dev_dynarray__local_allocator true, ktl_allocator

// #define sys_allocator_realloc realloc
// #define sys_allocator_free free
// #define dev_dynarray__global_allocator true, sys_allocator

#define dev_dynarray__type int
#define dev_dynarray__terminated true, 0
// #define dev_dynarray__infallible_allocator true
#define dev_dynarray__impl true
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
#include "allocates.h"

#undef ktl_dynarray_impl
#if ktl_dynarray_m(_impl)
#define ktl_dynarray_impl
#endif

KTL_DIAG_POP

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

ktl_dynarray_alloc_ok ktl_dynarray_m(reserve)(ktl_dynarray *dynarray, size_t n);

ktl_dynarray_alloc_ok ktl_dynarray_m(append)(
    ktl_dynarray *dynarray, ktl_dynarray_T const *arr, size_t n
);

#ifdef ktl_dynarray_sentinel
ktl_dynarray_alloc_ok ktl_dynarray_m(append_terminated)(
    ktl_dynarray *dynarray, ktl_dynarray_T const *arr
);
#endif

ktl_dynarray_alloc_ok
    ktl_dynarray_m(push)(ktl_dynarray *dynarray, ktl_dynarray_T elem);
ktl_nodiscard bool
    ktl_dynarray_m(pop)(ktl_dynarray *dynarray, ktl_dynarray_T *out);

//
// IMPLEMENTATION
//

#ifdef ktl_dynarray_impl

#ifndef KTL_dynarray_GROW_NUMERATOR
#define KTL_dynarray_GROW_NUMERATOR 3
#define KTL_dynarray_GROW_DENOMINATOR 2
#endif

#ifndef KTL_dynarray_MIN_CAP
#define KTL_dynarray_MIN_CAP 8
#endif

#ifndef KTL_dynarray_GROW_CAP
#define KTL_dynarray_GROW_CAP(cap)                                             \
    ((cap) / KTL_dynarray_GROW_DENOMINATOR * KTL_dynarray_GROW_NUMERATOR)
#endif

ktl_dynarray_alloc_ok
ktl_dynarray_m(reserve)(ktl_dynarray *const dynarray, size_t const n)
{
    assert(dynarray->ptr || (dynarray->len == 0 && dynarray->cap == 0));

    size_t const CAP_LIMIT =
        SIZE_MAX / KTL_dynarray_GROW_NUMERATOR * KTL_dynarray_GROW_DENOMINATOR;

    bool success;

    size_t const target_cap = dynarray->len + n
#ifdef ktl_dynarray_sentinel
                              + 1
#endif
        ;

    if (target_cap < dynarray->len + 1)
    {
        // overflow
        success = false;
    }
    else if (target_cap <= dynarray->cap)
    {
        success = true;
    }
    else if (dynarray->cap >= CAP_LIMIT)
    {
        success = false;
    }
    else
    {
        size_t new_cap = dynarray->cap < KTL_dynarray_MIN_CAP
                             ? KTL_dynarray_MIN_CAP
                             : KTL_dynarray_GROW_CAP(dynarray->cap);

        // Guaranteed to terminate due to CAP_LIMIT check above
        while (new_cap < target_cap)
        {
            new_cap = KTL_dynarray_GROW_CAP(new_cap);
        }

        void *const new_ptr = ktl_allocates_m(_realloc)(
            dynarray,
            dynarray->ptr,
            new_cap * sizeof(dynarray->ptr[0])
        );

        if (new_ptr)
        {
            success = true;
            dynarray->ptr = (ktl_dynarray_Tptr)new_ptr;
            dynarray->cap = new_cap;
        }
        else
        {
            success = false;
        }
    }

#ifdef ktl_dynarray_infallible
    if (!success)
    {
        assert(success && "dynarray reserve failed");
        exit(1);
    }
#else
    return success;
#endif
}

ktl_dynarray_alloc_ok ktl_dynarray_m(append)(
    ktl_dynarray *const dynarray,
    ktl_dynarray_T const *const arr,
    size_t const n
)
{
#ifdef ktl_dynarray_infallible

    ktl_dynarray_m(reserve)(dynarray, n);
    memmove(&dynarray->ptr[dynarray->len], arr, sizeof(dynarray->ptr[0]) * n);
    dynarray->len += n;

#ifdef ktl_dynarray_sentinel
    dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif

#else

    bool const success = ktl_dynarray_m(reserve)(dynarray, n);

    if (success)
    {
        memmove(
            &dynarray->ptr[dynarray->len],
            arr,
            sizeof(dynarray->ptr[0]) * n
        );
        dynarray->len += n;

#ifdef ktl_dynarray_sentinel
        dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif
    }

    return success;

#endif
}

#ifdef ktl_dynarray_sentinel
ktl_dynarray_alloc_ok ktl_dynarray_m(append_terminated)(
    ktl_dynarray *dynarray, ktl_dynarray_T const *arr
)
{
    size_t arr_len = 0;
    while (arr[arr_len] != ktl_dynarray_sentinel)
    {
        ++arr_len;
    }

#ifdef ktl_dynarray_infallible
    ktl_dynarray_m(append)(dynarray, arr, arr_len);
#else
    return ktl_dynarray_m(append)(dynarray, arr, arr_len);
#endif
}
#endif

ktl_dynarray_alloc_ok
ktl_dynarray_m(push)(ktl_dynarray *const dynarray, ktl_dynarray_T const elem)
{
#ifdef ktl_dynarray_infallible

    ktl_dynarray_m(reserve)(dynarray, 1);
    dynarray->ptr[dynarray->len++] = elem;
#ifdef ktl_dynarray_sentinel
    dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif

#else

    bool const ok = ktl_dynarray_m(reserve)(dynarray, 1);
    if (ok)
    {
        dynarray->ptr[dynarray->len++] = elem;
#ifdef ktl_dynarray_sentinel
        dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif
    }
    return ok;

#endif
}

ktl_nodiscard bool
ktl_dynarray_m(pop)(ktl_dynarray *const dynarray, ktl_dynarray_T *const out)
{
    bool const ok = dynarray->len > 0;
    if (ok)
    {
        *out = dynarray->ptr[--(dynarray->len)];
#ifdef ktl_dynarray_sentinel
        dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif
    }
    return ok;
}

#endif // ktl_dynarray_impl
