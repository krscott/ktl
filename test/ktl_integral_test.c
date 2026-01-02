#include "ktest.inc"
#include "ktl/lib/hash.h"
#include <stdint.h>

typedef int16_t i16;
#define i16__ord true
#define i16__hash true
#define i16__neg true
#define ktl_integral i16
#include "ktl/trait/integral.h"
#include "ktl/trait/integral.inc"
#undef ktl_integral

typedef uint16_t u16;
#define u16__ord true
#define u16__hash true
#define ktl_integral u16
#include "ktl/trait/integral.h"
#include "ktl/trait/integral.inc"
#undef ktl_integral

#define SAFE_ADD_TEST(T, a, b)                                                 \
    do                                                                         \
    {                                                                          \
        T out = 111;                                                           \
        ASSERT(T##_safe_add((a), (b), &out));                                  \
        ASSERT(out == (a) + (b));                                              \
        ASSERT(T##_safe_add((b), (a), &out));                                  \
        ASSERT(out == (a) + (b));                                              \
    } while (0)
#define UNSAFE_ADD_TEST(T, a, b)                                               \
    do                                                                         \
    {                                                                          \
        T out = 111;                                                           \
        ASSERT_FALSE(T##_safe_add((a), (b), &out));                            \
        ASSERT_FALSE(T##_safe_add((b), (a), &out));                            \
        ASSERT_INT_EQ(out, 111);                                               \
    } while (0)

KTEST_MAIN
{
    KTEST(t_i16_cmp)
    {
        i16 a = 1;
        i16 b = 2;
        i16 c = 2;
        i16 d = -3;
        ASSERT(i16_cmp(&a, &b) < 0);
        ASSERT(i16_cmp(&b, &a) > 0);
        ASSERT(i16_cmp(&b, &c) == 0);
        ASSERT(i16_cmp(&d, &c) < 0);
    }

    KTEST(t_i16_minmax)
    {
        ASSERT(i16_max(1, 2) == 2);
        ASSERT(i16_max(1, -2) == 1);

        ASSERT(i16_min(1, 2) == 1);
        ASSERT(i16_min(1, -2) == -2);
    }

    KTEST(t_i16_safe_add)
    {
        SAFE_ADD_TEST(i16, 0, 0);
        SAFE_ADD_TEST(i16, 0, 1);
        SAFE_ADD_TEST(i16, 0, -1);
        SAFE_ADD_TEST(i16, 1, 2);
        SAFE_ADD_TEST(i16, 1, -2);
        SAFE_ADD_TEST(i16, INT16_MAX, 0);
        SAFE_ADD_TEST(i16, INT16_MAX, -1);
        SAFE_ADD_TEST(i16, INT16_MIN, 0);
        SAFE_ADD_TEST(i16, INT16_MIN, 1);
        SAFE_ADD_TEST(i16, INT16_MAX / 2, INT16_MAX / 2 + 1);
        SAFE_ADD_TEST(i16, INT16_MAX, INT16_MIN);

        UNSAFE_ADD_TEST(i16, INT16_MAX, 1);
        UNSAFE_ADD_TEST(i16, INT16_MIN, -1);
        UNSAFE_ADD_TEST(i16, INT16_MAX / 2 + 1, INT16_MAX / 2 + 1);
        UNSAFE_ADD_TEST(i16, INT16_MAX, INT16_MAX);
        UNSAFE_ADD_TEST(i16, INT16_MIN, INT16_MIN);
    }

    KTEST(t_u16_safe_add)
    {
        SAFE_ADD_TEST(u16, 0, 0);
        SAFE_ADD_TEST(u16, 0, 1);
        SAFE_ADD_TEST(u16, 1, 2);
        SAFE_ADD_TEST(u16, UINT16_MAX, 0);
        SAFE_ADD_TEST(u16, UINT16_MAX / 2, UINT16_MAX / 2 + 1);

        UNSAFE_ADD_TEST(u16, UINT16_MAX, 1);
        UNSAFE_ADD_TEST(u16, UINT16_MAX / 2 + 1, UINT16_MAX / 2 + 1);
        UNSAFE_ADD_TEST(u16, UINT16_MAX, UINT16_MAX);
    }

    KTEST(t_u16_hash)
    {
        uint32_t hash = 0;

        u16 x = 10;
        u16_hash(&x, &hash, ktl_hash_murmur);

        ASSERT_INT_NEQ(hash, 0);

        uint32_t const hash_prev = hash;

        x = 0;
        u16_hash(&x, &hash, ktl_hash_murmur);

        ASSERT_INT_NEQ(hash, 0);
        ASSERT_INT_NEQ(hash, hash_prev);
    }

    KTEST(t_i16_abs)
    {
        ASSERT_INT_EQ(i16_abs(-1), 1);
        ASSERT_INT_EQ(i16_abs(1), 1);
        ASSERT_INT_EQ(i16_abs(0), 0);
    }
}
