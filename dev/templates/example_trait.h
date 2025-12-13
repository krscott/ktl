
// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef ktl_example
#include "ktl/lib/allocator.h"
#define dev_example__local_allocator true, ktl_allocator

// #define sys_allocator_realloc realloc
// #define sys_allocator_free free
// #define dev_example__global_allocator true, sys_allocator

#define dev_example__type int
// #define dev_example__infallible_allocator true
#define ktl_example dev_example
typedef struct
{
    dev_example__type *ptr;
#ifdef dev_example__local_allocator
    ktl_allocator allocator;
#endif
} dev_example;
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_example_m
#define ktl_example_m(x) KTL_TEMPLATE(ktl_example, x)

#undef ktl_example_T
#define ktl_example_T ktl_example_m(_type)

#undef ktl_example_Tptr
#define ktl_example_Tptr ktl_example_T *

#undef ktl_example_alloc_ok
#undef ktl_example_infallible
#if ktl_example_m(_infallible_allocator)
#define ktl_example_infallible
#define ktl_example_alloc_ok void
#else
#define ktl_example_alloc_ok ktl_nodiscard bool
#endif

#undef ktl_allocates
#define ktl_allocates ktl_example
#include "ktl/trait/allocates.h"

KTL_DIAG_POP

#ifndef KTL_INC

// Checks

ktl_assert_has_field(ktl_example, ktl_example_Tptr, ptr);

// Methods

ktl_example_alloc_ok ktl_example_m(foo)(ktl_example *example);

#endif // KTL_INC
