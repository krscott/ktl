// No header guard - repeatable include

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Defaults (dev-only)

#ifndef ktl_hashmap
#include "ktl/lib/hash.h" // IWYU pragma: export
#include <stdint.h>
static inline void
int_hash(int const *x, uint32_t *state, ktl_hash_fn hash_func)
{
    hash_func(state, x, sizeof(*x));
}
#define int__hash true
// static inline bool int_eq(int const *a, int const *b)
// {
//     return *a == *b;
// }
// #define int__eq true
static inline bool int_cmp(int const *a, int const *b)
{
    return *a - *b;
}
#define int__ord true

#include "ktl/lib/allocator.h"
#define dev_hashmap__local_allocator true, ktl_allocator

#define dev_hashmap__key int
#define dev_hashmap__value int
// #define dev_hashmap__infallible_allocator true
#define ktl_hashmap dev_hashmap
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE(-Wundef)

#undef ktl_hashmap_m
#define ktl_hashmap_m(x) KTL_TEMPLATE(ktl_hashmap, x)

#undef ktl_hashmap_K
#define ktl_hashmap_K ktl_hashmap_m(_key)
#undef ktl_hashmap_V
#define ktl_hashmap_V ktl_hashmap_m(_value)

#undef ktl_hashmap_K_eq
#if KTL_TEMPLATE(ktl_hashmap_K, _eq)
#define ktl_hashmap_K_eq KTL_TEMPLATE(ktl_hashmap_K, eq)
#elif KTL_TEMPLATE(ktl_hashmap_K, _ord)
#define ktl_hashmap_K_eq(a, b) (0 == KTL_TEMPLATE(ktl_hashmap_K, cmp)((a), (b)))
#else
#error "hashmap key type must define eq or ord trait"
#endif

#if KTL_TEMPLATE(ktl_hashmap_K, _hash)
#else
#error "hashmap key type must define hash trait"
#endif

#undef ktl_hashmap_node
#define ktl_hashmap_node KTL_TEMPLATE(ktl_hashmap, _node)
#undef ktl_hashmap_iter
#define ktl_hashmap_iter KTL_TEMPLATE(ktl_hashmap, iter)

#undef ktl_hashmap_hash_fn
#if KTL_GET0(ktl_hashmap_m(_hash_fn))
#define ktl_hashmap_hash_fn KTL_GET1(ktl_hashmap_m(_hash_fn), ktl_marker)
#else
#include "ktl/lib/hash.h"
#define ktl_hashmap_hash_fn ktl_hash_murmur
#endif

#undef ktl_hashmap_local_allocator
#if KTL_GET0(ktl_hashmap_m(_local_allocator))
#define ktl_hashmap_local_allocator                                            \
    KTL_GET1(ktl_hashmap_m(_local_allocator), ktl_marker)
#endif

KTL_DIAG_POP

#ifndef KTL_INC

// Type

typedef struct ktl_hashmap_node
{
    struct ktl_hashmap_node *next;
    ktl_hashmap_K key;
    ktl_hashmap_V value;
} ktl_hashmap_node;

/**
 * Generic hashmap type
 *
 * Fields:
 * - ptr: Pointer to base array
 * - len: Length of base array
 * - allocator (optional): Allocator handle
 */
typedef struct ktl_hashmap
{
    ktl_hashmap_node *buf;
    size_t buf_len;
    size_t count;
#ifdef ktl_hashmap_local_allocator
    ktl_hashmap_local_allocator allocator;
#endif
} ktl_hashmap;

typedef struct ktl_hashmap_iter
{
    ktl_hashmap const *hashmap;
    size_t buf_i;
    ktl_hashmap_node const *node;
} ktl_hashmap_iter;

// Traits

#undef ktl_allocates
#define ktl_allocates ktl_hashmap
#include "ktl/trait/allocates.h"

// Methods

#ifdef ktl_hashmap_local_allocator
ktl_nodiscard
    ktl_hashmap ktl_hashmap_m(init)(ktl_hashmap_local_allocator allocator);
#else
ktl_nodiscard ktl_hashmap ktl_hashmap_m(init)(void);
#endif

void ktl_hashmap_m(deinit)(ktl_hashmap *hashmap);

ktl_nodiscard ktl_hashmap_iter ktl_hashmap_m(each)(ktl_hashmap const *hashmap);
ktl_nodiscard bool ktl_hashmap_m(next)(
    ktl_hashmap_iter *iter, ktl_hashmap_K *key, ktl_hashmap_V *value
);

ktl_nodiscard bool
    ktl_hashmap_m(reserve)(ktl_hashmap *hashmap, size_t additional);

ktl_allocates_retval ktl_hashmap_m(insert)(
    ktl_hashmap *hashmap, ktl_hashmap_K k, ktl_hashmap_V v
);

ktl_nodiscard bool ktl_hashmap_m(get)(
    ktl_hashmap const *hashmap, ktl_hashmap_K k, ktl_hashmap_V *v
);

ktl_nodiscard bool ktl_hashmap_m(remove)(
    ktl_hashmap *hashmap, ktl_hashmap_K k, ktl_hashmap_V *v
);

#endif // KTL_INC
