// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_slice
static int dev_slice__cmp(int a, int b) { return a - b; }
#define dev_slice__impl true
#define dev_slice__type int
#define dev_slice__mut true
#define dev_slice__ord true
#define dev_slice__terminated true, 0
#define ktl_slice dev_slice
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_slice_m
#define ktl_slice_m(x) KTL_TEMPLATE(ktl_slice, x)

#undef ktl_slice_T
#define ktl_slice_T ktl_slice_m(_type)

#undef ktl_slice_mut
#undef ktl_slice_Tptr
#if ktl_slice_m(_mut)
#define ktl_slice_mut
#define ktl_slice_Tptr ktl_slice_T *
#else
#define ktl_slice_Tptr ktl_slice_T const *
#endif

#undef ktl_slice_ord
#if ktl_slice_m(_ord)
#define ktl_slice_ord
#endif

#undef ktl_slice_sentinel
#if KTL_GET0(ktl_slice_m(_terminated))
#define ktl_slice_sentinel KTL_GET1(ktl_slice_m(_terminated), (ktl_marker){0})
#endif

#undef ktl_slice_impl
#if ktl_slice_m(_impl)
#define ktl_slice_impl
#endif

KTL_DIAG_POP

// Type

struct ktl_slice
{
    ktl_slice_Tptr ptr;
    size_t len;
};

#ifdef ktl_slice_sentinel
static_assert(
    _Generic(ktl_slice_sentinel, ktl_marker: 0, default: 1),
    "Add `#define " KTL_STRINGIFY(ktl_slice) "__terminated 1, <sentinel-value>`"
);
#endif

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

//
// IMPLEMENTATION
//

#ifdef ktl_slice_impl

#ifdef ktl_slice_sentinel
ktl_nodiscard struct ktl_slice
ktl_slice_m(from_terminated)(ktl_slice_Tptr const ptr)
{
    assert(ptr);

    size_t len = 0;
    for (; ptr[len] != ktl_slice_sentinel; ++len)
    {
    }

    return (struct ktl_slice){
        .ptr = ptr,
        .len = len,
    };
}
#endif

ktl_nodiscard bool
ktl_slice_m(contains)(struct ktl_slice const slice, ktl_slice_T const x)
{
    return ktl_slice_m(find_index)(slice, x, NULL);
}

ktl_nodiscard bool
ktl_slice_m(find_index)(struct ktl_slice slice, ktl_slice_T x, size_t *index)
{
    bool ok = false;

    for (size_t i = 0; i < slice.len; ++i)
    {
        if (slice.ptr[i] == x)
        {
            if (index)
            {
                *index = i;
            }
            ok = true;
            break;
        }
    }

    return ok;
}

ktl_nodiscard bool
ktl_slice_m(eq)(struct ktl_slice const a, struct ktl_slice const b)
{
    assert(a.len == 0 || a.ptr);
    assert(b.len == 0 || b.ptr);

    return (a.len == b.len) &&
           (a.len == 0 || 0 == memcmp(a.ptr, b.ptr, a.len * sizeof(a.ptr[0])));
}

ktl_nodiscard bool ktl_slice_m(split)(
    struct ktl_slice const slice,
    ktl_slice_T const x,
    struct ktl_slice *const head,
    struct ktl_slice *const tail
)
{
    size_t index;
    bool const ok = ktl_slice_m(find_index)(slice, x, &index);

    if (ok)
    {
        if (head)
        {
            *head = (struct ktl_slice){
                .ptr = slice.ptr,
                .len = index,
            };
        }
        if (tail)
        {
            *tail = (struct ktl_slice){
                .ptr = &slice.ptr[index + 1],
                .len = slice.len - index - 1,
            };
        }
    }

    return ok;
}

ktl_nodiscard bool ktl_slice_m(split_at)(
    struct ktl_slice const slice,
    size_t const index,
    struct ktl_slice *const head,
    struct ktl_slice *const tail
)
{
    bool const ok = index <= slice.len;

    if (index < slice.len)
    {
        if (head)
        {
            *head = (struct ktl_slice){
                .ptr = slice.ptr,
                .len = index,
            };
        }
        if (tail)
        {
            *tail = (struct ktl_slice){
                .ptr = &slice.ptr[index],
                .len = slice.len - index,
            };
        }
    }
    else
    {
        if (head)
        {
            *head = slice;
        }
        if (tail)
        {
            *tail = (struct ktl_slice){0};
        }
    }

    return ok;
}

#ifdef ktl_slice_ord

static int ktl_slice_m(_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_slice_m(_cmp)(*(ktl_slice_T const *)a, *(ktl_slice_T const *)b);
}

#ifdef ktl_slice_mut
void ktl_slice_m(sort)(struct ktl_slice slice)
{
    assert(slice.len == 0 || slice.ptr);

    if (slice.len > 0)
    {
        qsort(
            slice.ptr,
            slice.len,
            sizeof(slice.ptr[0]),
            ktl_slice_m(_void_cmp_)
        );
    }
}
#endif

ktl_nodiscard bool ktl_slice_m(bsearch)(
    struct ktl_slice const slice,
    ktl_slice_T const key,
    ktl_slice_Tptr *const match
)
{
    assert(slice.len == 0 || slice.ptr);

    void *ptr;

    if (slice.len > 0)
    {
        ptr = bsearch(
            (void const *)&key,
            (void const *)slice.ptr,
            slice.len,
            sizeof(slice.ptr[0]),
            ktl_slice_m(_void_cmp_)
        );
    }
    else
    {
        ptr = NULL;
    }

    if (match)
    {
        if (ptr)
        {
            *match = (ktl_slice_T *)ptr;
        }
        else
        {
            *match = NULL;
        }
    }

    return (ptr != NULL);
}

ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice const slice, ktl_slice_T const key, size_t *const index
)
{
    ktl_slice_Tptr match;
    bool const ok = ktl_slice_m(bsearch)(slice, key, &match);

    if (ok && index)
    {
        *index = (size_t)(match - slice.ptr);
    }

    return ok;
}

#endif // ktl_slice_ord

#endif // ktl_slice_impl
