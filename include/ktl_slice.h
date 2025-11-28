// No header guard - repeatable include

#include "ktl_macros.h"

#include <stdbool.h>
#include <stddef.h>

// Default types for development
#ifndef ktl_slice
struct dev_slice
{
    int *ptr;
    size_t len;
};
static int dev_slice__cmp(int a, int b) { return a - b; }
#define dev_slice__type int
#define dev_slice__ord 1
#define ktl_slice dev_slice
#endif

#define KTL_T KTL_TEMPLATE(ktl_slice, _type)
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

ktl_nodiscard bool ktl_slice_m(contains)(struct ktl_slice slice, KTL_T x);
ktl_nodiscard bool
    ktl_slice_m(find_index)(struct ktl_slice slice, KTL_T x, size_t *index);
ktl_nodiscard bool ktl_slice_m(eq)(struct ktl_slice a, struct ktl_slice b);

#if ktl_slice_m(_ord)

void ktl_slice_m(sort)(struct ktl_slice slice);

ktl_nodiscard bool
    ktl_slice_m(bsearch)(struct ktl_slice slice, KTL_T key, KTL_T **match);
ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice slice, KTL_T key, size_t *index
);

#endif // ord

#undef ktl_slice_m
#undef KTL_T
