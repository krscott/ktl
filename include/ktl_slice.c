#include "ktl_slice.h"
#include "ktl_macros.h"
#include <stdlib.h>

#define KTL_T KTL_TEMPLATE(ktl_slice, _type)
#define ktl_slice_method(x) KTL_TEMPLATE(ktl_slice, x)

#if KTL_TEMPLATE(ktl_slice, _ord)

static int ktl_slice_method(_qsort_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_slice_method(_cmp)(*(KTL_T const *)a, *(KTL_T const *)b);
}

void ktl_slice_method(sort)(struct ktl_slice slice)
{
    qsort(
        slice.ptr,
        slice.len,
        sizeof(slice.ptr[0]),
        ktl_slice_method(_qsort_cmp_)
    );
}

#endif // ord

#undef ktl_slice_method
#undef KTL_T
