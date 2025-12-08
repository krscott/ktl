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

#ifdef ktl_vec_local_allocator
ktl_nodiscard ktl_vec ktl_vec_m(init)(ktl_vec_local_allocator const allocator)
{
    return (ktl_vec){.allocator = allocator};
}
#else
ktl_nodiscard ktl_vec ktl_vec_m(init)(void)
{
    return (ktl_vec){0};
}
#endif

void ktl_vec_m(deinit)(ktl_vec *const vec)
{
    ktl_allocates_m(_free)(vec, vec->ptr);
}
