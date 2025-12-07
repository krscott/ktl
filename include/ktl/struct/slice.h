// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_slice
static inline int int_cmp(int const *a, int const *b)
{
    return *a - *b;
}
#define int__ord true
#define dev_slice__impl true
#define dev_slice__type int
#define dev_slice__mut true
#define dev_slice__terminated true, 0
#define ktl_slice dev_slice
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_slice_m
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

#undef ktl_slice_T
#define ktl_slice_T ktl_slice_m(_type)

#undef ktl_slice_mut
#undef ktl_slice_Tptr
#if ktl_slice_m(_mut)
#define ktl_slice_mut
#define ktl_slice_Tptr ktl_slice_T *
#else
#define ktl_slice_Tptr ktl_slice_T const *
#endif

#undef ktl_slice_sentinel
#if KTL_GET0(ktl_slice_m(_terminated))
#define ktl_slice_sentinel KTL_GET1(ktl_slice_m(_terminated), (ktl_marker){0})
#endif

#undef ktl_slice_impl
#if ktl_slice_m(_impl)
#define ktl_slice_impl
#endif

KTL_DIAG_POP

// Type

/**
 * Generic slice type
 *
 * Fields:
 * - ptr: Pointer to array
 * - len: Length of array
 */
typedef struct ktl_slice
{
    ktl_slice_Tptr ptr;
    size_t len;
} ktl_slice;

#ifdef ktl_slice_sentinel
static_assert(
    _Generic(ktl_slice_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(ktl_slice) "__terminated 1, <sentinel-value>`"
);
#endif

// Common

#define ktl_array ktl_slice
#include "ktl/trait/array.h"
#undef ktl_array

// Prototypes

#ifdef ktl_slice_sentinel
ktl_nodiscard ktl_slice ktl_slice_m(from_terminated)(ktl_slice_Tptr ptr);
#endif

#ifdef ktl_array_T_eq
ktl_nodiscard bool ktl_slice_m(split)(
    ktl_slice slice, ktl_slice_T x, ktl_slice *head, ktl_slice *tail
);
#endif

ktl_nodiscard bool ktl_slice_m(split_at)(
    ktl_slice slice, size_t index, ktl_slice *head, ktl_slice *tail
);

//
// IMPLEMENTATION
//

#ifdef ktl_slice_impl

#ifdef ktl_slice_sentinel
ktl_nodiscard ktl_slice ktl_slice_m(from_terminated)(ktl_slice_Tptr const ptr)
{
    assert(ptr);

    size_t len = 0;
    for (; ptr[len] != ktl_slice_sentinel; ++len)
    {
    }

    return (ktl_slice){
        .ptr = ptr,
        .len = len,
    };
}
#endif

#ifdef ktl_array_T_eq
ktl_nodiscard bool ktl_slice_m(split)(
    ktl_slice const slice,
    ktl_slice_T const x,
    ktl_slice *const head,
    ktl_slice *const tail
)
{
    size_t index;
    bool const ok = ktl_slice_m(find_index)(slice, x, &index);

    if (ok)
    {
        if (head)
        {
            *head = (ktl_slice){
                .ptr = slice.ptr,
                .len = index,
            };
        }
        if (tail)
        {
            *tail = (ktl_slice){
                .ptr = &slice.ptr[index + 1],
                .len = slice.len - index - 1,
            };
        }
    }

    return ok;
}
#endif

ktl_nodiscard bool ktl_slice_m(split_at)(
    ktl_slice const slice,
    size_t const index,
    ktl_slice *const head,
    ktl_slice *const tail
)
{
    bool const ok = index <= slice.len;

    if (index < slice.len)
    {
        if (head)
        {
            *head = (ktl_slice){
                .ptr = slice.ptr,
                .len = index,
            };
        }
        if (tail)
        {
            *tail = (ktl_slice){
                .ptr = &slice.ptr[index],
                .len = slice.len - index,
            };
        }
    }
    else
    {
        if (head)
        {
            *head = slice;
        }
        if (tail)
        {
            *tail = (ktl_slice){0};
        }
    }

    return ok;
}

#endif // ktl_slice_impl
