#ifndef KTL_PRELUDE_H_
#define KTL_PRELUDE_H_

#include "ktl/macros.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

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

#endif
