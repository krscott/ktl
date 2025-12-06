#ifndef KTL_PRELUDE_H_
#define KTL_PRELUDE_H_

#include "ktl/allocator.h"
#include "ktl/macros.h"

#include "sys/types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef size_t usize;
typedef ssize_t isize;

ktl_define_integral(char);
ktl_define_integral(u8);
ktl_define_integral(i8);
ktl_define_integral(u16);
ktl_define_integral(i16);
ktl_define_integral(i32);
ktl_define_integral(u32);
ktl_define_integral(i64);
ktl_define_integral(u64);
ktl_define_integral(f32);
ktl_define_integral(f64);
ktl_define_integral(usize);
ktl_define_integral(isize);

#define char__ord true
#define u8__ord true
#define i8__ord true
#define u16__ord true
#define i16__ord true
#define i32__ord true
#define u32__ord true
#define i64__ord true
#define u64__ord true
#define f32__ord true
#define f64__ord true
#define usize__ord true
#define isize__ord true

// Strings

#ifdef KTL_PRELUDE_IMPL
#define str__impl true
#define strview__impl true
#define strbuf__impl true
#endif

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
#define strview_cmp str_cmp

#define str__ord true
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

// Allocators

extern ktl_allocator const ktl_clib_allocator;

#ifdef KTL_PRELUDE_IMPL
#undef KTL_PRELUDE_IMPL

static void *ktl_clib_allocator_realloc(void *impl, void *ptr, size_t size)
{
    (void)impl;
    return realloc(ptr, size);
}

static void ktl_clib_allocator_free(void *impl, void *ptr)
{
    (void)impl;
    free(ptr);
}

static ktl_allocator_vtable const ktl_clib_allocator_vtable = {
    .realloc = ktl_clib_allocator_realloc,
    .free = ktl_clib_allocator_free,
};
ktl_allocator const ktl_clib_allocator = {
    .vtable = &ktl_clib_allocator_vtable,
    .impl = NULL,
};

#endif

#endif
