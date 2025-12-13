#include "ktest.inc"
#include <stdint.h>

typedef int16_t i16;
#define i16__ord true
#define ktl_integral i16
#include "ktl/trait/integral.h"
#include "ktl/trait/integral.inc"
#undef ktl_integral

KTEST_MAIN
{
    KTEST(t_i16_cmp)
    {
        i16 a = 1;
        i16 b = 2;
        i16 c = 2;
        i16 d = -3;
        ASSERT_TRUE(i16_cmp(&a, &b) < 0);
        ASSERT_TRUE(i16_cmp(&b, &a) > 0);
        ASSERT_TRUE(i16_cmp(&b, &c) == 0);
        ASSERT_TRUE(i16_cmp(&d, &c) < 0);
    }
}
