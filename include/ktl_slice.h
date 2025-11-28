// No header guard - repeatable include

#include "ktl_macros.h"

#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef ktl_slice
struct dev_slice
{
    int *ptr;
    size_t len;
};
static int dev_slice__cmp(int a, int b) { return a - b; }
#define dev_slice__type int
#define dev_slice__ord 1
#define dev_slice__mut 1
#define ktl_slice dev_slice
#endif

// Macros

#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)
#define ktl_T ktl_slice_m(_type)

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#if ktl_slice_m(_mut)
#define ktl_Tptr ktl_T *
#else
#define ktl_Tptr ktl_T const *
#endif

#if ktl_slice_m(_ord)
#define ktl_ord 1
#else
#define ktl_ord 0
#endif

KTL_DIAG_POP

// Prototypes

ktl_nodiscard bool ktl_slice_m(contains)(struct ktl_slice slice, ktl_T x);

ktl_nodiscard bool
    ktl_slice_m(find_index)(struct ktl_slice slice, ktl_T x, size_t *index);

ktl_nodiscard bool ktl_slice_m(eq)(struct ktl_slice a, struct ktl_slice b);

ktl_nodiscard bool ktl_slice_m(split)(
    struct ktl_slice slice,
    ktl_T x,
    struct ktl_slice *head,
    struct ktl_slice *tail
);

ktl_nodiscard bool ktl_slice_m(split_at)(
    struct ktl_slice slice,
    size_t index,
    struct ktl_slice *head,
    struct ktl_slice *tail
);

#if ktl_ord

void ktl_slice_m(sort)(struct ktl_slice slice);

ktl_nodiscard bool
    ktl_slice_m(bsearch)(struct ktl_slice slice, ktl_T key, ktl_Tptr *match);

ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice slice, ktl_T key, size_t *index
);

#endif // ktl_ord

#undef ktl_ord
#undef ktl_Tptr
#undef ktl_T
#undef ktl_slice_m
