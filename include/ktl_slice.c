#include "ktl_slice.h"
#include "ktl_macros.h"
#include <stdlib.h>

#define KTL_T KTL_TEMPLATE(ktl_slice, _type)
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

#if ktl_slice_m(_ord)

static int ktl_slice_m(_qsort_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_slice_m(_cmp)(*(KTL_T const *)a, *(KTL_T const *)b);
}

void ktl_slice_m(sort)(struct ktl_slice slice)
{
    qsort(slice.ptr, slice.len, sizeof(slice.ptr[0]), ktl_slice_m(_qsort_cmp_));
}

#endif // ord

#undef ktl_slice_m
#undef KTL_T
