// No header guard - repeatable include

#include "ktl/macros.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Defaults (dev-only)

#ifndef ktl_array
static int int_cmp(int const *a, int const *b)
{
    return *a - *b;
}
#define int__ord true
// static int int_eq(int const *a, int const *b) { return *a == *b; }
// #define int__eq true
#define dev_array__mut true
typedef struct
{
#ifdef dev_array__mut
    int *ptr;
#else
    int const *ptr;
#endif
    size_t len;
} dev_array;
#define dev_array__type int
#define ktl_array dev_array
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_array_m
#define ktl_array_m(x) KTL_TEMPLATE(ktl_array, x)

#undef ktl_array_T
#define ktl_array_T ktl_array_m(_type)

#undef ktl_array_iter
#define ktl_array_iter KTL_TEMPLATE(ktl_array, iter)

#undef ktl_array_mut
#undef ktl_array_Tptr
#if ktl_array_m(_mut) || defined(KTL_ARRAY_MUT)
#define ktl_array_mut
#define ktl_array_Tptr ktl_array_T *
#else
#define ktl_array_Tptr ktl_array_T const *
#endif

#undef ktl_array_T_cmp
#if KTL_TEMPLATE(ktl_array_T, _ord)
#define ktl_array_T_cmp KTL_TEMPLATE(ktl_array_T, cmp)
#endif

#undef ktl_array_T_eq
#if KTL_TEMPLATE(ktl_array_T, _eq)
#define ktl_array_T_eq KTL_TEMPLATE(ktl_array_T, eq)
#elif defined(ktl_array_T_cmp)
#define ktl_array_T_eq(a, b) (0 == ktl_array_T_cmp((a), (b)))
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

ktl_assert_has_field(ktl_array, ktl_array_Tptr, ptr);
ktl_assert_has_field(ktl_array, size_t, len);

// Types

typedef struct ktl_array_iter
{
    ktl_array const *array;
    size_t i;
} ktl_array_iter;

// Methods

ktl_nodiscard ktl_array_iter ktl_array_m(each)(ktl_array const *array);
ktl_nodiscard bool ktl_array_m(next)(ktl_array_iter *iter, ktl_array_T *x);

#ifdef ktl_array_mut
void ktl_array_m(sort_by)(
    ktl_array array, int (*comp)(void const *, void const *)
);
#endif

#ifdef ktl_array_T_cmp

#ifdef ktl_array_mut
void ktl_array_m(sort)(ktl_array array);
#endif

ktl_nodiscard bool ktl_array_m(bsearch)(
    ktl_array array, ktl_array_T key, ktl_array_Tptr *match
);

ktl_nodiscard bool
    ktl_array_m(bsearch_index)(ktl_array array, ktl_array_T key, size_t *index);

#endif // ktl_array_T_cmp

#ifdef ktl_array_T_eq
ktl_nodiscard bool ktl_array_m(eq)(ktl_array a, ktl_array b);

ktl_nodiscard bool
    ktl_array_m(find_index)(ktl_array array, ktl_array_T x, size_t *index);

ktl_nodiscard bool ktl_array_m(contains)(ktl_array array, ktl_array_T x);
#endif

#endif // KTL_INC
