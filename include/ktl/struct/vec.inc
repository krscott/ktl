#ifndef ktl_vec
// dev-only
#include "ktl/struct/vec.h"
#endif

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define KTL_INC
#include "ktl/struct/vec.h"
#undef KTL_INC

// Traits

#undef ktl_array
#define ktl_array ktl_vec
#define KTL_ARRAY_MUT
#include "ktl/trait/array.inc"
#undef KTL_ARRAY_MUT

#undef ktl_dynarray
#define ktl_dynarray ktl_vec
#include "ktl/trait/dynarray.inc"

// Methods

#ifndef ktl_vec_no_init

#ifdef ktl_vec_local_allocator
ktl_nodiscard ktl_vec ktl_vec_m(init)(ktl_vec_local_allocator const allocator)
{
#ifdef ktl_dynarray_sentinel
    static ktl_vec_T empty[] = {ktl_dynarray_sentinel};
    return (ktl_vec){.allocator = allocator, .ptr = &empty[0]};
#else
    return (ktl_vec){.allocator = allocator};
#endif
}
#else
ktl_nodiscard ktl_vec ktl_vec_m(init)(void)
{
    return (ktl_vec){0};
}
#endif

void ktl_vec_m(deinit)(ktl_vec *const vec)
{
    if (vec->cap > 0)
    {
        ktl_allocates_m(_free)(vec, vec->ptr);
    }
}

#endif
