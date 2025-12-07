#include "ktl/trait/convert_vec_slice.h"

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

ktl_nodiscard ktl_slice ktl_convert_vec_slice_m(as)(ktl_vec const vec)
{
    return (ktl_slice){
        .ptr = vec.ptr,
        .len = vec.len,
    };
}

ktl_dynarray_alloc_ok
ktl_convert_vec_slice_m(append)(ktl_vec *const vec, ktl_slice const slice)
{
#ifdef ktl_dynarray_infallible
    ktl_vec_m(append)(vec, slice.ptr, slice.len);
#else
    return ktl_vec_m(append)(vec, slice.ptr, slice.len);
#endif
}
