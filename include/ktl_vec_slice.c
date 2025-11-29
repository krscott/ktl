#include "ktl_vec_slice.h"
#include "ktl_macros.h"
#include <assert.h>
#include <stddef.h>

ktl_nodiscard struct ktl_slice ktl_vec_slice_m(as)(struct ktl_vec *const vec)
{
    return (struct ktl_slice){
        .ptr = vec->ptr,
        .len = vec->len,
    };
}

ktl_vec_alloc_ok
ktl_vec_slice_m(append)(struct ktl_vec *const vec, struct ktl_slice const slice)
{
#ifdef ktl_vec_infallible
    ktl_vec_m(append)(vec, slice.ptr, slice.len);
#else
    return ktl_vec_m(append)(vec, slice.ptr, slice.len);
#endif
}
