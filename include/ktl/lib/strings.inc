#include "ktl/lib/strings.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>

#define ktl_slice str
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define ktl_slice strview
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define ktl_vec strbuf
#include "ktl/struct/vec.inc"
#define ktl_slice str
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#define ktl_slice strview
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#undef ktl_vec

strview strview_trim_start(strview const s)
{
    size_t i = 0;
    for (; i < s.len && isspace(s.ptr[i]); ++i)
    {
    }

    return (strview){
        .ptr = &s.ptr[i],
        .len = s.len - i,
    };
}

str str_trim_start(str const s)
{
    size_t i = 0;
    for (; i < s.len && isspace(s.ptr[i]); ++i)
    {
    }

    return (str){
        .ptr = &s.ptr[i],
        .len = s.len - i,
    };
}
