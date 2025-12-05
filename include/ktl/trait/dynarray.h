// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_arraydyn
typedef struct
{
    char dummy;
} dev_allocator;
#define dev_arraydyn__type int
#define dev_arraydyn__terminated true, 0
#define dev_arraydyn__custom_allocator true
#define dev_arraydyn__local_allocator true, dev_allocator
#define dev_arraydyn__realloc(arraydyn, p, size) realloc((p), (size))
#define dev_arraydyn__free(arraydyn, p) free(p)
// #define dev_arraydyn__infallible_allocator true
#define dev_arraydyn__impl true
#define ktl_arraydyn dev_arraydyn
typedef struct
{
    dev_arraydyn__type *ptr;
    size_t len;
    size_t cap;
#ifdef dev_arraydyn__local_allocator
    KTL_GET1(dev_arraydyn__local_allocator) allocator;
#endif
} dev_arraydyn;
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_arraydyn_m
#define ktl_arraydyn_m(x) KTL_TEMPLATE(ktl_arraydyn, x)

#undef ktl_arraydyn_T
#define ktl_arraydyn_T ktl_arraydyn_m(_type)

#undef ktl_arraydyn_Tptr
#define ktl_arraydyn_Tptr ktl_arraydyn_T *

#undef ktl_arraydyn_sentinel
#if KTL_GET0(ktl_arraydyn_m(_terminated))
#define ktl_arraydyn_sentinel                                                  \
    KTL_GET1(ktl_arraydyn_m(_terminated), (ktl_marker){0})
#endif

#undef ktl_arraydyn_realloc
#undef ktl_arraydyn_free
#if KTL_GET0(ktl_arraydyn_m(_custom_allocator)) ||                             \
    KTL_GET0(ktl_arraydyn_m(_local_allocator))
#define ktl_arraydyn_realloc(arraydyn, p, size)                                \
    ktl_arraydyn_m(_realloc)((arraydyn), (p), (size))
#define ktl_arraydyn_free(arraydyn, p) ktl_arraydyn_m(_free)((arraydyn), (p))
#else
#define ktl_arraydyn_realloc(arraydyn, p, size) realloc((p), (size))
#define ktl_arraydyn_free(arraydyn, p) free(p)
#endif

#undef ktl_arraydyn_local_allocator
#if KTL_GET0(ktl_arraydyn_m(_local_allocator))
#define ktl_arraydyn_local_allocator                                           \
    KTL_GET1(ktl_arraydyn_m(_local_allocator), ktl_marker)
#endif

#undef ktl_arraydyn_alloc_ok
#undef ktl_arraydyn_infallible
#if ktl_arraydyn_m(_infallible_allocator)
#define ktl_arraydyn_infallible
#define ktl_arraydyn_alloc_ok void
#else
#define ktl_arraydyn_alloc_ok ktl_nodiscard bool
#endif

#undef ktl_arraydyn_impl
#if ktl_arraydyn_m(_impl)
#define ktl_arraydyn_impl
#endif

KTL_DIAG_POP

// Type

#ifdef ktl_arraydyn_sentinel
static_assert(
    _Generic(ktl_arraydyn_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_arraydyn
    ) "__terminated true, <sentinel-value>`"
);
#endif
#ifdef ktl_arraydyn_local_allocator
static_assert(
    _Generic(((ktl_arraydyn *)0)->allocator, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(
        ktl_arraydyn
    ) "__local_allocator true, <allocator-type>`"
);
#endif

// Prototypes

ktl_arraydyn_alloc_ok ktl_arraydyn_m(reserve)(ktl_arraydyn *arraydyn, size_t n);

ktl_arraydyn_alloc_ok ktl_arraydyn_m(append)(
    ktl_arraydyn *arraydyn, ktl_arraydyn_T const *arr, size_t n
);

#ifdef ktl_arraydyn_sentinel
ktl_arraydyn_alloc_ok ktl_arraydyn_m(append_terminated)(
    ktl_arraydyn *arraydyn, ktl_arraydyn_T const *arr
);
#endif

ktl_arraydyn_alloc_ok
    ktl_arraydyn_m(push)(ktl_arraydyn *arraydyn, ktl_arraydyn_T elem);
ktl_nodiscard bool
    ktl_arraydyn_m(pop)(ktl_arraydyn *arraydyn, ktl_arraydyn_T *out);

//
// IMPLEMENTATION
//

#ifdef ktl_arraydyn_impl

#ifndef KTL_arraydyn_GROW_NUMERATOR
#define KTL_arraydyn_GROW_NUMERATOR 3
#define KTL_arraydyn_GROW_DENOMINATOR 2
#endif

#ifndef KTL_arraydyn_MIN_CAP
#define KTL_arraydyn_MIN_CAP 8
#endif

#ifndef KTL_arraydyn_GROW_CAP
#define KTL_arraydyn_GROW_CAP(cap)                                             \
    ((cap) / KTL_arraydyn_GROW_DENOMINATOR * KTL_arraydyn_GROW_NUMERATOR)
#endif

ktl_arraydyn_alloc_ok
ktl_arraydyn_m(reserve)(ktl_arraydyn *const arraydyn, size_t const n)
{
    assert(arraydyn->ptr || (arraydyn->len == 0 && arraydyn->cap == 0));

    size_t const CAP_LIMIT =
        SIZE_MAX / KTL_arraydyn_GROW_NUMERATOR * KTL_arraydyn_GROW_DENOMINATOR;

    bool success;

    size_t const target_cap = arraydyn->len + n
#ifdef ktl_arraydyn_sentinel
                              + 1
#endif
        ;

    if (target_cap < arraydyn->len + 1)
    {
        // overflow
        success = false;
    }
    else if (target_cap <= arraydyn->cap)
    {
        success = true;
    }
    else if (arraydyn->cap >= CAP_LIMIT)
    {
        success = false;
    }
    else
    {
        size_t new_cap = arraydyn->cap < KTL_arraydyn_MIN_CAP
                             ? KTL_arraydyn_MIN_CAP
                             : KTL_arraydyn_GROW_CAP(arraydyn->cap);

        // Guaranteed to terminate due to CAP_LIMIT check above
        while (new_cap < target_cap)
        {
            new_cap = KTL_arraydyn_GROW_CAP(new_cap);
        }

        void *const new_ptr = ktl_arraydyn_realloc(
            arraydyn,
            arraydyn->ptr,
            new_cap * sizeof(arraydyn->ptr[0])
        );

        if (new_ptr)
        {
            success = true;
            arraydyn->ptr = (ktl_arraydyn_Tptr)new_ptr;
            arraydyn->cap = new_cap;
        }
        else
        {
            success = false;
        }
    }

#ifdef ktl_arraydyn_infallible
    if (!success)
    {
        assert(success && "arraydyn reserve failed");
        exit(1);
    }
#else
    return success;
#endif
}

ktl_arraydyn_alloc_ok ktl_arraydyn_m(append)(
    ktl_arraydyn *const arraydyn,
    ktl_arraydyn_T const *const arr,
    size_t const n
)
{
#ifdef ktl_arraydyn_infallible

    ktl_arraydyn_m(reserve)(arraydyn, n);
    memmove(&arraydyn->ptr[arraydyn->len], arr, sizeof(arraydyn->ptr[0]) * n);
    arraydyn->len += n;

#ifdef ktl_arraydyn_sentinel
    arraydyn->ptr[arraydyn->len] = ktl_arraydyn_sentinel;
#endif

#else

    bool const success = ktl_arraydyn_m(reserve)(arraydyn, n);

    if (success)
    {
        memmove(
            &arraydyn->ptr[arraydyn->len],
            arr,
            sizeof(arraydyn->ptr[0]) * n
        );
        arraydyn->len += n;

#ifdef ktl_arraydyn_sentinel
        arraydyn->ptr[arraydyn->len] = ktl_arraydyn_sentinel;
#endif
    }

    return success;

#endif
}

#ifdef ktl_arraydyn_sentinel
ktl_arraydyn_alloc_ok ktl_arraydyn_m(append_terminated)(
    ktl_arraydyn *arraydyn, ktl_arraydyn_T const *arr
)
{
    size_t arr_len = 0;
    while (arr[arr_len] != ktl_arraydyn_sentinel)
    {
        ++arr_len;
    }

#ifdef ktl_arraydyn_infallible
    ktl_arraydyn_m(append)(arraydyn, arr, arr_len);
#else
    return ktl_arraydyn_m(append)(arraydyn, arr, arr_len);
#endif
}
#endif

ktl_arraydyn_alloc_ok
ktl_arraydyn_m(push)(ktl_arraydyn *const arraydyn, ktl_arraydyn_T const elem)
{
#ifdef ktl_arraydyn_infallible

    ktl_arraydyn_m(reserve)(arraydyn, 1);
    arraydyn->ptr[arraydyn->len++] = elem;
#ifdef ktl_arraydyn_sentinel
    arraydyn->ptr[arraydyn->len] = ktl_arraydyn_sentinel;
#endif

#else

    bool const ok = ktl_arraydyn_m(reserve)(arraydyn, 1);
    if (ok)
    {
        arraydyn->ptr[arraydyn->len++] = elem;
#ifdef ktl_arraydyn_sentinel
        arraydyn->ptr[arraydyn->len] = ktl_arraydyn_sentinel;
#endif
    }
    return ok;

#endif
}

ktl_nodiscard bool
ktl_arraydyn_m(pop)(ktl_arraydyn *const arraydyn, ktl_arraydyn_T *const out)
{
    bool const ok = arraydyn->len > 0;
    if (ok)
    {
        *out = arraydyn->ptr[--(arraydyn->len)];
#ifdef ktl_arraydyn_sentinel
        arraydyn->ptr[arraydyn->len] = ktl_arraydyn_sentinel;
#endif
    }
    return ok;
}

#endif // ktl_arraydyn_impl
