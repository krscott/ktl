// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#if !defined(ktl_vec) && !defined(ktl_slice)
#include "ktl/array/slice.h"
#include "ktl/array/vec.h"
#endif

#ifdef ktl_vec_slice_m
#undef ktl_vec_slice_m
#endif
#define ktl_vec_slice_m(x) KTL_TEMPLATE2(ktl_vec, x, ktl_slice)

// Prototypes

ktl_nodiscard struct ktl_slice ktl_vec_slice_m(as)(struct ktl_vec *vec);

ktl_vec_alloc_ok
    ktl_vec_slice_m(append)(struct ktl_vec *vec, struct ktl_slice slice);
