
// No header guard - repeatable include

#include "ktl/lib/hash.h"
#include "ktl/macros.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Defaults (dev-only)

#ifndef ktl_integral
typedef uint16_t devint;
#define devint__ord true
#define devint__hash true
#define ktl_integral devint
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE("-Wundef")

#undef ktl_integral_m
#define ktl_integral_m(x) KTL_TEMPLATE(ktl_integral, x)

#undef ktl_integral_ord
#if ktl_integral_m(_ord)
#define ktl_integral_ord
#endif

#undef ktl_integral_hash
#if ktl_integral_m(_hash)
#define ktl_integral_hash
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

// Methods

#ifdef ktl_integral_ord
static inline int
ktl_integral_m(cmp)(ktl_integral const *a, ktl_integral const *b)
{
    ktl_integral const a_ = *a;
    ktl_integral const b_ = *b;
    return a_ < b_ ? -1 : (a_ > b_ ? 1 : 0);
}
#endif

#ifdef ktl_integral_hash
static inline void ktl_integral_m(hash)(
    ktl_integral const *x, uint32_t *state, ktl_hash_fn hash_func
)
{
    hash_func(state, x, sizeof(*x));
}
#endif

#define KTL_INTEGRAL_MAX(x)                                                    \
    _Generic(                                                                  \
        (x),                                                                   \
        char: CHAR_MAX,                                                        \
        int8_t: INT8_MAX,                                                      \
        uint8_t: UINT8_MAX,                                                    \
        int16_t: INT16_MAX,                                                    \
        uint16_t: UINT16_MAX,                                                  \
        int32_t: INT32_MAX,                                                    \
        uint32_t: UINT32_MAX,                                                  \
        int64_t: INT64_MAX,                                                    \
        uint64_t: UINT64_MAX                                                   \
    )

#define KTL_INTEGRAL_MIN(x)                                                    \
    _Generic(                                                                  \
        (x),                                                                   \
        char: CHAR_MIN,                                                        \
        int8_t: INT8_MIN,                                                      \
        uint8_t: 0,                                                            \
        int16_t: INT16_MIN,                                                    \
        uint16_t: 0,                                                           \
        int32_t: INT32_MIN,                                                    \
        uint32_t: 0,                                                           \
        int64_t: INT64_MIN,                                                    \
        uint64_t: 0                                                            \
    )

static inline ktl_nodiscard bool
ktl_integral_m(safe_add)(ktl_integral a, ktl_integral b, ktl_integral *out)
{
    bool const a_is_pos = a > 0;
    bool const b_is_pos = b > 0;
    bool safe;

    if (a_is_pos && b_is_pos)
    {
        safe = a <= KTL_INTEGRAL_MAX(a) - b;
    }
    else if (!a_is_pos && !b_is_pos)
    {
        safe = a >= KTL_INTEGRAL_MIN(a) - b;
    }
    else
    {
        safe = true;
    }

    if (safe && out)
    {
        *out = a + b;
    }

    return safe;
}

static inline ktl_integral ktl_integral_m(max)(ktl_integral a, ktl_integral b)
{
    return a > b ? a : b;
}
static inline ktl_integral ktl_integral_m(min)(ktl_integral a, ktl_integral b)
{
    return a < b ? a : b;
}

#endif // KTL_INC
