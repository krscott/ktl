// No header guard - repeatable include

#include "ktl/macros.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef ktl_array
struct dev_array
{
    int *ptr;
    size_t len;
};
static int dev_array__cmp(int a, int b) { return a - b; }
#define dev_array__type int
#define dev_array__mut true
#define dev_array__ord true
#define dev_array__impl true
#define ktl_array dev_array
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_array_m
#define ktl_array_m(x) KTL_TEMPLATE(ktl_array, x)

#undef ktl_array_T
#define ktl_array_T ktl_array_m(_type)

#undef ktl_array_mut
#undef ktl_array_Tptr
#if ktl_array_m(_mut)
#define ktl_array_mut
#define ktl_array_Tptr ktl_array_T *
#else
#define ktl_array_Tptr ktl_array_T const *
#endif

#undef ktl_array_ord
#if ktl_array_m(_ord)
#define ktl_array_ord
#endif

#undef ktl_array_sentinel
#if KTL_GET0(ktl_array_m(_terminated))
#define ktl_array_sentinel KTL_GET1(ktl_array_m(_terminated), (ktl_marker){0})
#endif

#undef ktl_array_impl
#if ktl_array_m(_impl)
#define ktl_array_impl
#endif

KTL_DIAG_POP

// Prototypes

#ifdef ktl_array_ord

#ifdef ktl_array_mut
void ktl_array_m(sort)(struct ktl_array slice);
#endif

ktl_nodiscard bool ktl_array_m(bsearch)(
    struct ktl_array slice, ktl_array_T key, ktl_array_Tptr *match
);

ktl_nodiscard bool ktl_array_m(bsearch_index)(
    struct ktl_array slice, ktl_array_T key, size_t *index
);

#endif // ktl_array_ord

//
// IMPLEMENTATION
//

#ifdef ktl_array_impl

#ifdef ktl_array_ord

static int ktl_array_m(_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_array_m(_cmp)(*(ktl_array_T const *)a, *(ktl_array_T const *)b);
}

#ifdef ktl_array_mut
void ktl_array_m(sort)(struct ktl_array array)
{
    assert(array.len == 0 || array.ptr);

    if (array.len > 0)
    {
        qsort(
            array.ptr,
            array.len,
            sizeof(array.ptr[0]),
            ktl_array_m(_void_cmp_)
        );
    }
}
#endif

ktl_nodiscard bool ktl_array_m(bsearch)(
    struct ktl_array const array,
    ktl_array_T const key,
    ktl_array_Tptr *const match
)
{
    assert(array.len == 0 || array.ptr);

    void *ptr;

    if (array.len > 0)
    {
        ptr = bsearch(
            (void const *)&key,
            (void const *)array.ptr,
            array.len,
            sizeof(array.ptr[0]),
            ktl_array_m(_void_cmp_)
        );
    }
    else
    {
        ptr = NULL;
    }

    if (match)
    {
        if (ptr)
        {
            *match = (ktl_array_T *)ptr;
        }
        else
        {
            *match = NULL;
        }
    }

    return (ptr != NULL);
}

ktl_nodiscard bool ktl_array_m(bsearch_index)(
    struct ktl_array const array, ktl_array_T const key, size_t *const index
)
{
    ktl_array_Tptr match;
    bool const ok = ktl_array_m(bsearch)(array, key, &match);

    if (ok && index)
    {
        *index = (size_t)(match - array.ptr);
    }

    return ok;
}

#endif // ktl_array_ord

#endif // ktl_array_impl
