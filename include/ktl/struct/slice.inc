#ifndef ktl_slice
// dev-only
#include "ktl/struct/slice.h"
#endif

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define KTL_INC
#include "ktl/struct/slice.h"
#undef KTL_INC

// Traits

#undef ktl_array
#define ktl_array ktl_slice
#include "ktl/trait/array.inc"

// Methods

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
    bool const ok = slice.len > 0;

    if (ok)
    {
        size_t index;
        if (ktl_slice_m(find_index)(slice, x, &index))
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
    }

    return ok;
}
#endif

void ktl_slice_m(split_at)(
    ktl_slice const slice,
    size_t const index,
    ktl_slice *const head,
    ktl_slice *const tail
)
{
    assert(index <= slice.len);

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
}

ktl_nodiscard bool ktl_slice_m(split_first)(
    ktl_slice slice, ktl_slice_T *const head, ktl_slice *const tail
)
{
    bool ok = slice.len > 0;
    if (ok)
    {
        ktl_slice_m(split_at)(slice, 1, &slice, tail);
        assert(slice.len == 1);
        if (head)
        {
            *head = slice.ptr[0];
        }
    }
    return ok;
}
