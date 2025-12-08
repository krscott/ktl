#ifndef KTL_STRINGS_H_
#define KTL_STRINGS_H_

#include "ktl/prelude.h" // IWYU pragma: export

#include <string.h>

#define str__type char
#define str__mut true
#define str__terminated true, '\0'
#define ktl_slice str
#include "ktl/struct/slice.h"
#undef ktl_slice

#define strview__type char
#define strview__terminated true, '\0'
#define ktl_slice strview
#include "ktl/struct/slice.h"
#undef ktl_slice

static inline int str_cmp(str const *a, str const *b)
{
    int x = strncmp(a->ptr, b->ptr, a->len < b->len ? a->len : b->len);
    if (x == 0)
    {
        x = (int)a->len - (int)b->len;
    }
    return x;
}
#define str__ord true

static inline int strview_cmp(strview const *a, strview const *b)
{
    int x = strncmp(a->ptr, b->ptr, a->len < b->len ? a->len : b->len);
    if (x == 0)
    {
        x = (int)a->len - (int)b->len;
    }
    return x;
}
#define strview__ord true

#define strbuf__type char
#define strbuf__terminated true, '\0'
#define strbuf__infallible_allocator true
#define ktl_vec strbuf
#include "ktl/struct/vec.h"
// Supports converting to both str and strview
#define ktl_slice str
#include "ktl/trait/convert_vec_slice.h"
#undef ktl_slice
#define ktl_slice strview
#include "ktl/trait/convert_vec_slice.h"
#undef ktl_slice
#undef ktl_vec

static inline strview strview_from_str(str s)
{
    return (strview){.ptr = s.ptr, .len = s.len};
}

str str_trim_start(str s);
strview strview_trim_start(strview s);

#endif
