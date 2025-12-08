#include "ktl/macros.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifndef ktl_charslice
// dev-only
#include "ktl/trait/charslice.h"
#endif

#define KTL_INC
#include "ktl/trait/charslice.h"
#undef KTL_INC

#ifdef ktl_charslice_ord
int ktl_charslice_m(cmp)(ktl_charslice const *a, ktl_charslice const *b)
{
    int x = strncmp(a->ptr, b->ptr, a->len < b->len ? a->len : b->len);
    if (x == 0)
    {
        x = (int)a->len - (int)b->len;
    }
    return x;
}
#endif

ktl_nodiscard ktl_charslice ktl_charslice_m(trim_start)(ktl_charslice s)
{

    size_t i = 0;
    for (; i < s.len && isspace(s.ptr[i]); ++i)
    {
    }

    return (ktl_charslice){
        .ptr = &s.ptr[i],
        .len = s.len - i,
    };
}

ktl_nodiscard ktl_charslice ktl_charslice_m(trim_end)(ktl_charslice s)
{
    size_t len = s.len;
    for (; len > 0 && isspace(s.ptr[len - 1]); --len)
    {
    }

    return (ktl_charslice){
        .ptr = s.ptr,
        .len = len,
    };
}
