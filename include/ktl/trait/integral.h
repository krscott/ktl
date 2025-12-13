
// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef ktl_integral
#include <stdint.h>
typedef uint16_t devint;
#define devint__ord true
#define ktl_integral devint
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_integral_m
#define ktl_integral_m(x) KTL_TEMPLATE(ktl_integral, x)

#undef ktl_integral_ord
#if ktl_integral_m(_ord)
#define ktl_integral_ord
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

// Methods

#ifdef ktl_integral_ord
static inline int
ktl_integral_m(cmp)(ktl_integral const *const a, ktl_integral const *const b)
{
    ktl_integral const a_ = *a;
    ktl_integral const b_ = *b;
    return a_ < b_ ? -1 : (a_ > b_ ? 1 : 0);
}
#endif

#endif // KTL_INC
