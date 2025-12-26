#include "ktl/macros.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

#ifdef ktl_charslice_hash
void ktl_charslice_m(hash)(
    ktl_charslice const *s, uint32_t *state, ktl_hash_fn hash_func
)
{
    hash_func(state, s->ptr, s->len * sizeof(s->ptr[0]));
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

ktl_nodiscard ktl_charslice ktl_charslice_m(trim)(ktl_charslice s)
{
    s = ktl_charslice_m(trim_start)(s);
    s = ktl_charslice_m(trim_end)(s);
    return s;
}

ktl_nodiscard bool
ktl_charslice_m(starts_with)(ktl_charslice const s, ktl_charslice const match)
{
    return s.len >= match.len && (0 == strncmp(s.ptr, match.ptr, match.len));
}

ktl_nodiscard bool ktl_charslice_m(starts_with_cstr)(
    ktl_charslice const s, char const *const match
)
{
    bool out = true;
    for (size_t i = 0; match[i]; ++i)
    {
        if (i >= s.len || s.ptr[i] != match[i])
        {
            out = false;
            break;
        }
    }
    return out;
}
