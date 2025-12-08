// No header guard - repeatable include

#include "ktl/macros.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef ktl_charslice
#define dev_str__type char
#define dev_str__mut true
#define dev_str__ord true
#define dev_str__terminated true, '\0'
#define ktl_slice dev_str
#include "ktl/struct/slice.h"
#undef ktl_slice
#define ktl_charslice dev_str
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_charslice_m
#define ktl_charslice_m(x) KTL_TEMPLATE(ktl_charslice, x)

#undef ktl_charslice_T
#define ktl_charslice_T ktl_charslice_m(_type)

#undef ktl_charslice_mut
#undef ktl_charslice_Tptr
#if ktl_charslice_m(_mut) || defined(KTL_ARRAY_MUT)
#define ktl_charslice_mut
#define ktl_charslice_Tptr ktl_charslice_T *
#else
#define ktl_charslice_Tptr ktl_charslice_T const *
#endif

#undef ktl_charslice_T_cmp
#if KTL_TEMPLATE(ktl_charslice_T, _ord)
#define ktl_charslice_T_cmp KTL_TEMPLATE(ktl_charslice_T, cmp)
#endif

#undef ktl_charslice_T_eq
#if KTL_TEMPLATE(ktl_charslice_T, _eq)
#define ktl_charslice_T_eq KTL_TEMPLATE(ktl_charslice_T, eq)
#elif defined(ktl_charslice_T_cmp)
#define ktl_charslice_T_eq(a, b) (0 == ktl_charslice_T_cmp((a), (b)))
#endif

#undef ktl_charslice_ord
#if ktl_charslice_m(_ord)
#define ktl_charslice_ord
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

ktl_assert_has_field(ktl_charslice, ktl_charslice_Tptr, ptr);
ktl_assert_has_field(ktl_charslice, size_t, len);

// Methods

#ifdef ktl_charslice_ord
int ktl_charslice_m(cmp)(ktl_charslice const *a, ktl_charslice const *b);
#endif

ktl_nodiscard ktl_charslice ktl_charslice_m(trim_start)(ktl_charslice s);
ktl_nodiscard ktl_charslice ktl_charslice_m(trim_end)(ktl_charslice s);

#endif // KTL_INC
