#ifndef KTL_HASH_H_
#define KTL_HASH_H_

#include <stdint.h>
#include <stdlib.h>

typedef void (*ktl_hash_fn)(uint32_t *state, void const *key, size_t size);

static inline void
ktl_hash_murmur(uint32_t *state, void const *key, size_t size)
{
    uint8_t const *const data = (uint8_t const *)key;

    uint32_t h = *state;

    for (size_t i = 0; i < size; i++)
    {
        h ^= data[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }

    *state = h;
}

#endif
