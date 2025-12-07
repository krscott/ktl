#include "ktl/trait/array.h"

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef ktl_array_T_cmp

static int ktl_array_m(_array_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_array_T_cmp((ktl_array_T const *)a, (ktl_array_T const *)b);
}

#ifdef ktl_array_mut
void ktl_array_m(sort)(ktl_array array)
{
    assert(array.len == 0 || array.ptr);

    if (array.len > 0)
    {
        qsort(
            array.ptr,
            array.len,
            sizeof(array.ptr[0]),
            ktl_array_m(_array_void_cmp_)
        );
    }
}
#endif

ktl_nodiscard bool ktl_array_m(bsearch)(
    ktl_array const array, ktl_array_T const key, ktl_array_Tptr *const match
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
            ktl_array_m(_array_void_cmp_)
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
    ktl_array const array, ktl_array_T const key, size_t *const index
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

ktl_nodiscard bool ktl_array_m(eq)(ktl_array const a, ktl_array const b)
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
ktl_array_m(find_index)(ktl_array array, ktl_array_T x, size_t *index)
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

ktl_nodiscard bool ktl_array_m(contains)(ktl_array array, ktl_array_T x)
{
    return ktl_array_m(find_index)(array, x, NULL);
}
#endif // ktl_array_T_eq
