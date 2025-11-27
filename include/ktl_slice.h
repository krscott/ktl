// No header guard - repeatable include

#include "ktl_macros.h"

#include "stddef.h"

// Default types for development
#ifndef ktl_slice
struct dev_slice
{
    int *ptr;
    size_t len;
};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
static int dev_slice__cmp(int a, int b) { return a - b; }
#pragma clang diagnostic pop
#define dev_slice__type int
#define dev_slice__ord 1
#define ktl_slice dev_slice
#endif

#define ktl_slice_method(x) KTL_TEMPLATE(ktl_slice, x)

#if KTL_TEMPLATE(ktl_slice, _ord)
void ktl_slice_method(sort)(struct ktl_slice slice);
#endif

#undef ktl_slice_method
