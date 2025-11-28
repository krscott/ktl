// No header guard - repeatable include

#include "ktl_macros.h"

#include <assert.h>
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
#define dev_slice__mut true
#define dev_slice__ord true
#define dev_slice__terminated true, 0
#define ktl_slice dev_slice
#endif

// Macros

#ifdef ktl_slice_m
#undef ktl_slice_m
#endif
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

#ifdef ktl_slice_T
#undef ktl_slice_T
#endif
#define ktl_slice_T ktl_slice_m(_type)

static_assert(
    _Generic(
        (struct ktl_slice){0}.ptr,
        ktl_slice_T *: 1,
        ktl_slice_T const *: 1,
        default: 0
    ),
    "Wrong type: `#define " KTL_STRINGIFY(ktl_slice) "__type " KTL_STRINGIFY(
        ktl_slice_T
    ) "`"
);

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#ifdef ktl_slice_mut
#undef ktl_slice_mut
#endif
#ifdef ktl_slice_Tptr
#undef ktl_slice_Tptr
#endif
#if ktl_slice_m(_mut)
#define ktl_slice_mut
#define ktl_slice_Tptr ktl_slice_T *
static_assert(
    _Generic((struct ktl_slice){0}.ptr, ktl_slice_T const *: 0, default: 1),
    "Remove `#define " KTL_STRINGIFY(ktl_slice) "__mut " KTL_STRINGIFY(
        ktl_slice_m(_mut)
    ) "`"
);
#else
#define ktl_slice_Tptr ktl_slice_T const *
static_assert(
    _Generic((struct ktl_slice){0}.ptr, ktl_slice_T *: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(ktl_slice) "__mut 1`"
);
#endif

#ifdef ktl_slice_ord
#undef ktl_slice_ord
#endif
#if ktl_slice_m(_ord)
#define ktl_slice_ord
#endif

#ifdef ktl_slice_sentinel
#undef ktl_slice_sentinel
#endif
#if KTL_GET0(ktl_slice_m(_terminated))
#define ktl_slice_sentinel KTL_GET1(ktl_slice_m(_terminated), (ktl_marker){0})
static_assert(
    _Generic(ktl_slice_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(ktl_slice) "__terminated 1, <sentinel-value>`"
);
#endif

KTL_DIAG_POP

// Prototypes

#ifdef ktl_slice_sentinel
ktl_nodiscard struct ktl_slice ktl_slice_m(from_terminated)(ktl_slice_Tptr ptr);
#endif

ktl_nodiscard bool ktl_slice_m(contains)(struct ktl_slice slice, ktl_slice_T x);

ktl_nodiscard bool ktl_slice_m(find_index)(
    struct ktl_slice slice, ktl_slice_T x, size_t *index
);

ktl_nodiscard bool ktl_slice_m(eq)(struct ktl_slice a, struct ktl_slice b);

ktl_nodiscard bool ktl_slice_m(split)(
    struct ktl_slice slice,
    ktl_slice_T x,
    struct ktl_slice *head,
    struct ktl_slice *tail
);

ktl_nodiscard bool ktl_slice_m(split_at)(
    struct ktl_slice slice,
    size_t index,
    struct ktl_slice *head,
    struct ktl_slice *tail
);

#ifdef ktl_slice_ord

#ifdef ktl_slice_mut
void ktl_slice_m(sort)(struct ktl_slice slice);
#endif

ktl_nodiscard bool ktl_slice_m(bsearch)(
    struct ktl_slice slice, ktl_slice_T key, ktl_slice_Tptr *match
);

ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice slice, ktl_slice_T key, size_t *index
);

#endif // ktl_slice_ord
