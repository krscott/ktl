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

#ifndef KTL_INC

// Methods

ktl_nodiscard ktl_slice ktl_convert_vec_slice_m(as)(ktl_vec vec);

ktl_dynarray_alloc_ok
    ktl_convert_vec_slice_m(append)(ktl_vec *vec, ktl_slice slice);

#endif // KTL_INC
