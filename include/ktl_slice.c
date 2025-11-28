#include "ktl_slice.h"
#include "ktl_macros.h"
#include <stdbool.h>
#include <stdlib.h>

#define KTL_T KTL_TEMPLATE(ktl_slice, _type)
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

#if ktl_slice_m(_ord)

static int ktl_slice_m(_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_slice_m(_cmp)(*(KTL_T const *)a, *(KTL_T const *)b);
}

void ktl_slice_m(sort)(struct ktl_slice slice)
{
    qsort(slice.ptr, slice.len, sizeof(slice.ptr[0]), ktl_slice_m(_void_cmp_));
}

ktl_nodiscard bool ktl_slice_m(bsearch)(
    struct ktl_slice const slice, KTL_T const key, KTL_T **const match
)
{
    void *ptr = bsearch(
        (void const *)&key,
        (void const *)slice.ptr,
        slice.len,
        sizeof(slice.ptr[0]),
        ktl_slice_m(_void_cmp_)
    );

    if (match)
    {
        if (ptr)
        {
            *match = (KTL_T *)ptr;
        }
        else
        {
            *match = NULL;
        }
    }

    return (ptr != NULL);
}

ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice const slice, KTL_T const key, size_t *const index
)
{
    KTL_T *match;
    bool const ok = ktl_slice_m(bsearch)(slice, key, &match);

    if (ok)
    {
        *index = (size_t)(match - slice.ptr);
    }

    return ok;
}

#endif // ord

#undef ktl_slice_m
#undef KTL_T
