#ifndef ktl_dynarray
// dev-only
#include "ktl/trait/dynarray.h"
#endif

#define KTL_INC
#include "ktl/trait/dynarray.h"
#undef KTL_INC

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Traits

#undef ktl_allocates
#define ktl_allocates ktl_dynarray
#include "allocates.inc"

// Macros

#ifndef KTL_DYNARRAY_GROW_NUMERATOR
#define KTL_DYNARRAY_GROW_NUMERATOR 3
#define KTL_DYNARRAY_GROW_DENOMINATOR 2
#endif

#ifndef KTL_DYNARRAY_MIN_CAP
#define KTL_DYNARRAY_MIN_CAP 8
#endif

// Methods

ktl_dynarray_alloc_ok
ktl_dynarray_m(reserve)(ktl_dynarray *const dynarray, size_t const n)
{
    assert(dynarray->ptr || (dynarray->len == 0 && dynarray->cap == 0));

    size_t const CAP_LIMIT =
        SIZE_MAX / KTL_DYNARRAY_GROW_NUMERATOR * KTL_DYNARRAY_GROW_DENOMINATOR;

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
        size_t new_cap = dynarray->cap < KTL_DYNARRAY_MIN_CAP
                             ? KTL_DYNARRAY_MIN_CAP
                             : dynarray->cap;

        // Guaranteed to terminate due to CAP_LIMIT check above
        while (new_cap < target_cap)
        {

            size_t const next_new_cap =
                (new_cap / KTL_DYNARRAY_GROW_DENOMINATOR) *
                KTL_DYNARRAY_GROW_NUMERATOR;
            assert(next_new_cap > new_cap);
            new_cap = next_new_cap;
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

void ktl_dynarray_m(clear)(ktl_dynarray *const dynarray)
{
    if (dynarray->len > 0)
    {
        dynarray->len = 0;
#ifdef ktl_dynarray_sentinel
        dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif
    }
}

void ktl_dynarray_m(swap_remove)(ktl_dynarray *const dynarray, size_t const i)
{
    assert(i < dynarray->len);
    if (i < dynarray->len)
    {
        dynarray->ptr[i] = dynarray->ptr[--dynarray->len];
#ifdef ktl_dynarray_sentinel
        dynarray->ptr[dynarray->len] = ktl_dynarray_sentinel;
#endif
    }
}
