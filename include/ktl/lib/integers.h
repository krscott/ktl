#ifndef KTL_INTEGERS_H_
#define KTL_INTEGERS_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint8_t u8;
#define u8_ord true
#define ktl_integral u8
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef int8_t i8;
#define i8__ord true
#define ktl_integral i8
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef uint16_t u16;
#define u16__ord true
#define ktl_integral u16
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef int16_t i16;
#define i16__ord true
#define ktl_integral i16
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef int32_t i32;
#define i32__ord true
#define ktl_integral i32
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef uint32_t u32;
#define u32__ord true
#define ktl_integral u32
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef int64_t i64;
#define i64__ord true
#define ktl_integral i64
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef uint64_t u64;
#define u64__ord true
#define ktl_integral u64
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef size_t usize;
#define usize__ord true
#define ktl_integral usize
#include "ktl/trait/integral.h"
#undef ktl_integral

typedef ssize_t isize;
#define isize__ord true
#define ktl_integral isize
#include "ktl/trait/integral.h"
#undef ktl_integral

#endif
