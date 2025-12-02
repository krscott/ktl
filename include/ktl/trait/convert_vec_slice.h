// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#if !defined(ktl_vec) && !defined(ktl_slice)
#include "ktl/struct/slice.h"
#include "ktl/struct/vec.h"
#endif

// Macros

#ifdef ktl_convert_vec_slice_m
#undef ktl_convert_vec_slice_m
#endif
#define ktl_convert_vec_slice_m(x) KTL_TEMPLATE2(ktl_vec, x, ktl_slice)

// Prototypes

ktl_nodiscard struct ktl_slice ktl_convert_vec_slice_m(as)(struct ktl_vec vec);

ktl_vec_alloc_ok ktl_convert_vec_slice_m(append)(
    struct ktl_vec *vec, struct ktl_slice slice
);

//
// IMPLEMENTATION
//

#if defined(ktl_vec_impl) && defined(ktl_slice_impl)

ktl_nodiscard struct ktl_slice
ktl_convert_vec_slice_m(as)(struct ktl_vec const vec)
{
    return (struct ktl_slice){
        .ptr = vec.ptr,
        .len = vec.len,
    };
}

ktl_vec_alloc_ok ktl_convert_vec_slice_m(append)(
    struct ktl_vec *const vec, struct ktl_slice const slice
)
{
#ifdef ktl_vec_infallible
    ktl_vec_m(append)(vec, slice.ptr, slice.len);
#else
    return ktl_vec_m(append)(vec, slice.ptr, slice.len);
#endif
}

#elif defined(ktl_vec_impl) || defined(ktl_slice_impl)
#error "Must have both vec and slice `__impl` enabled"
#endif // ktl_convert_vec_slice_impl
