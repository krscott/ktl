#ifndef KTL_STRINGS_H_
#define KTL_STRINGS_H_

#define char__ord true
#define ktl_integral char
#include "ktl/trait/integral.h"
#undef ktl_integral

#define str__type char
#define str__mut true
#define str__terminated true, '\0'
#define ktl_slice str
#include "ktl/struct/slice.h"
#undef ktl_slice
#define str__ord true
#define str__hash true
#define ktl_charslice str
#include "ktl/trait/charslice.h"
#undef ktl_charslice

#define strview__type char
#define strview__terminated true, '\0'
#define ktl_slice strview
#include "ktl/struct/slice.h"
#undef ktl_slice
#define strview__ord true
#define strview__hash true
#define ktl_charslice strview
#include "ktl/trait/charslice.h"
#undef ktl_charslice

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

#endif
