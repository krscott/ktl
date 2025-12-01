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
static int int_cmp(int const *a, int const *b) { return *a - *b; }
#define int__ord true
// static int int_eq(int const *a, int const *b) { return *a == *b; }
// #define int__eq true
#define dev_array__type int
#define dev_array__mut true
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

#undef ktl_array_T_cmp
#if KTL_TEMPLATE(ktl_array_T, _ord)
#define ktl_array_T_cmp KTL_TEMPLATE(ktl_array_T, cmp)
#endif

#undef ktl_array_T_eq
#if KTL_TEMPLATE(ktl_array_T, _eq)
#define ktl_array_T_eq KTL_TEMPLATE(ktl_array_T, eq)
#elif defined(ktl_array_T_cmp)
#define ktl_array_T_eq(a, b) (0 == ktl_array_T_cmp((a), (b)))
#endif

#undef ktl_array_impl
#if ktl_array_m(_impl)
#define ktl_array_impl
#endif

KTL_DIAG_POP

// Prototypes

#ifdef ktl_array_T_cmp

#ifdef ktl_array_mut
void ktl_array_m(sort)(struct ktl_array slice);
#endif

ktl_nodiscard bool ktl_array_m(bsearch)(
    struct ktl_array slice, ktl_array_T key, ktl_array_Tptr *match
);

ktl_nodiscard bool ktl_array_m(bsearch_index)(
    struct ktl_array slice, ktl_array_T key, size_t *index
);

#endif // ktl_array_T_cmp

#ifdef ktl_array_T_eq
ktl_nodiscard bool ktl_array_m(eq)(struct ktl_array a, struct ktl_array b);

ktl_nodiscard bool ktl_array_m(find_index)(
    struct ktl_array array, ktl_array_T x, size_t *index
);

ktl_nodiscard bool ktl_array_m(contains)(struct ktl_array array, ktl_array_T x);
#endif

//
// IMPLEMENTATION
//

#ifdef ktl_array_impl

#ifdef ktl_array_T_cmp

static int ktl_array_m(_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_array_T_cmp((ktl_array_T const *)a, (ktl_array_T const *)b);
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

#endif // ktl_array_T_cmp

#ifdef ktl_array_T_eq

ktl_nodiscard bool
ktl_array_m(eq)(struct ktl_array const a, struct ktl_array const b)
{
    assert(a.len == 0 || a.ptr);
    assert(b.len == 0 || b.ptr);

    bool eq = (a.len == b.len);

    for (size_t i = 0; eq && i < a.len; ++i)
    {
        eq = ktl_array_T_eq(&a.ptr[i], &b.ptr[i]);
    }

    return eq;
}

ktl_nodiscard bool
ktl_array_m(find_index)(struct ktl_array array, ktl_array_T x, size_t *index)
{
    bool ok = false;

    for (size_t i = 0; i < array.len; ++i)
    {
        if (ktl_array_T_eq(&array.ptr[i], &x))
        {
            if (index)
            {
                *index = i;
            }
            ok = true;
            break;
        }
    }

    return ok;
}

ktl_nodiscard bool ktl_array_m(contains)(struct ktl_array array, ktl_array_T x)
{
    return ktl_array_m(find_index)(array, x, NULL);
}
#endif // ktl_array_T_eq

#endif // ktl_array_impl
