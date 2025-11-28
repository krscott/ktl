#include "ktl_slice.h"
#include "ktl_macros.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

ktl_nodiscard bool
ktl_slice_m(contains)(struct ktl_slice const slice, ktl_T const x)
{
    return ktl_slice_m(find_index)(slice, x, NULL);
}

ktl_nodiscard bool
ktl_slice_m(find_index)(struct ktl_slice slice, ktl_T x, size_t *index)
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
    return (a.len == b.len) &&
           (0 == memcmp(a.ptr, b.ptr, a.len * sizeof(a.ptr[0])));
}

ktl_nodiscard bool ktl_slice_m(split)(
    struct ktl_slice const slice,
    ktl_T const x,
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
    bool const ok = index < slice.len;

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
                .ptr = &slice.ptr[index],
                .len = slice.len - index,
            };
        }
    }

    return ok;
}

#if ktl_ord

static int ktl_slice_m(_void_cmp_)( //
    void const *a,
    void const *b
)
{
    return ktl_slice_m(_cmp)(*(ktl_T const *)a, *(ktl_T const *)b);
}

void ktl_slice_m(sort)(struct ktl_slice slice)
{
    qsort(slice.ptr, slice.len, sizeof(slice.ptr[0]), ktl_slice_m(_void_cmp_));
}

ktl_nodiscard bool ktl_slice_m(bsearch)(
    struct ktl_slice const slice, ktl_T const key, ktl_Tptr *const match
)
{
    void *ptr = bsearch(
        (void const *)&key,
        (void const *)slice.ptr,
        slice.len,
        sizeof(slice.ptr[0]),
        ktl_slice_m(_void_cmp_)
    );

    if (match)
    {
        if (ptr)
        {
            *match = (ktl_T *)ptr;
        }
        else
        {
            *match = NULL;
        }
    }

    return (ptr != NULL);
}

ktl_nodiscard bool ktl_slice_m(bsearch_index)(
    struct ktl_slice const slice, ktl_T const key, size_t *const index
)
{
    ktl_Tptr match;
    bool const ok = ktl_slice_m(bsearch)(slice, key, &match);

    if (ok && index)
    {
        *index = (size_t)(match - slice.ptr);
    }

    return ok;
}

#endif // ktl_ord

#undef ktl_ord
#undef ktl_Tptr
#undef ktl_T
#undef ktl_slice_m
