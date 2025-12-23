#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef ktl_hashmap
// dev-only
#include "ktl/struct/hashmap.h"
#endif

#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define KTL_INC
#include "ktl/struct/hashmap.h"
#undef KTL_INC

// Traits

#undef ktl_allocates
#define ktl_allocates ktl_hashmap
#include "ktl/trait/allocates.inc"

// Constants

#define KTL_HASHMAP_NODE_SENTINEL ktl_hashmap_m(_node_SENTINEL)
static ktl_hashmap_node KTL_HASHMAP_NODE_SENTINEL;

// Methods

#ifdef ktl_hashmap_local_allocator
ktl_nodiscard
    ktl_hashmap ktl_hashmap_m(init)(ktl_hashmap_local_allocator const allocator)
{
    return (ktl_hashmap){.allocator = allocator};
}
#else
ktl_nodiscard ktl_hashmap ktl_hashmap_m(init)(void)
{
    return (ktl_hashmap){0};
}
#endif

void ktl_hashmap_m(deinit)(ktl_hashmap *const hashmap)
{
    if (hashmap->buf_len > 0)
    {
        assert(hashmap->buf);
        size_t const len = hashmap->buf_len;
        for (size_t i = 0; i < len; ++i)
        {
            // Free linked nodes
            ktl_hashmap_node *next = hashmap->buf[i].next;
            while (next && next != &KTL_HASHMAP_NODE_SENTINEL)
            {
                ktl_hashmap_node *const tmp = next;
                next = tmp->next;
                ktl_allocates_m(_free)(hashmap, tmp);
            }
        }

        // Free base nodes
        ktl_allocates_m(_free)(hashmap, hashmap->buf);
    }
}

ktl_nodiscard ktl_hashmap_iter
ktl_hashmap_m(iter)(ktl_hashmap const *const hashmap)
{
    return (ktl_hashmap_iter){.hashmap = hashmap};
}

ktl_nodiscard bool ktl_hashmap_m(next)(
    ktl_hashmap_iter *const iter,
    ktl_hashmap_K *const key,
    ktl_hashmap_V *const value
)
{
    assert(iter);

    ktl_hashmap const *const hashmap = iter->hashmap;
    assert(hashmap);

    bool ok = false;

    if (hashmap->count > 0)
    {
        ktl_hashmap_node const *node = iter->node;

        if (node == NULL || node == &KTL_HASHMAP_NODE_SENTINEL)
        {
            do
            {
                // A populated node will always have a valid next pointer
                // (even if just the sentinel)
                node = &hashmap->buf[iter->buf_i++];
            } while (node->next == NULL && iter->buf_i < hashmap->buf_len);
        }

        ok = node != NULL && node->next != NULL;

        if (ok)
        {
            *key = node->key;
            *value = node->value;
            iter->node = node->next;
        }
    }

    return ok;
}

static ktl_nodiscard bool ktl_hashmap_m(_insert_no_grow)(
    ktl_hashmap *const hashmap, ktl_hashmap_K const k, ktl_hashmap_V const v
)
{
    bool success = true;

    uint32_t hash = 0;
    KTL_TEMPLATE(ktl_hashmap_K, hash)(&k, &hash, ktl_hashmap_hash_fn);

    size_t idx = hash % hashmap->buf_len;

    ktl_hashmap_node *node = &hashmap->buf[idx];
    if (node->next != NULL)
    {
        while (!ktl_hashmap_K_eq(&node->key, &k))
        {
            if (node->next == &KTL_HASHMAP_NODE_SENTINEL)
            {
                ktl_hashmap_node *const new_node = (ktl_hashmap_node *)
                    ktl_allocates_m(_alloc)(hashmap, sizeof(ktl_hashmap_node));
                if (!new_node)
                {
                    success = false;
                    break;
                }

                node->next = new_node;
                node = new_node;
                break;
            }

            assert(node->next != NULL);
            node = node->next;
        }
    }

    if (success)
    {
        node->key = k;
        node->value = v;

        if (node->next == NULL)
        {
            // This is a new node
            ++hashmap->count;
            node->next = &KTL_HASHMAP_NODE_SENTINEL;
        }
    }

    return success;
}

ktl_nodiscard bool
ktl_hashmap_m(reserve)(ktl_hashmap *const hashmap, size_t const additional)
{
    // TODO: Parameterize magic numbers

    // 75% factor target
    size_t const target_len = (hashmap->count + additional) * 4 / 3;

    bool success = true;

    if (hashmap->buf_len < target_len)
    {
        size_t new_buf_len = hashmap->buf_len == 0 ? 256 : hashmap->buf_len;

        while (new_buf_len < target_len)
        {
            new_buf_len = new_buf_len * 2;
        }

        size_t const new_size = new_buf_len * sizeof(hashmap->buf[0]);
        ktl_hashmap_node *const new_buf =
            (ktl_hashmap_node *)ktl_allocates_m(_alloc)(hashmap, new_size);
        (void)memset(new_buf, 0, new_size);

        success = new_buf != NULL;
        if (success)
        {
            ktl_hashmap new_hashmap = *hashmap;
            new_hashmap.buf = new_buf;
            new_hashmap.buf_len = new_buf_len;

            // move each pair into new_hashmap
            ktl_hashmap_iter iter = ktl_hashmap_m(iter)(hashmap);
            ktl_hashmap_K k;
            ktl_hashmap_V v;
            while (success && ktl_hashmap_m(next)(&iter, &k, &v))
            {
                success = ktl_hashmap_m(_insert_no_grow)(&new_hashmap, k, v);
            }

            ktl_hashmap_m(deinit)(hashmap);
            *hashmap = new_hashmap;
        }
    }

    return success;
}

ktl_allocates_retval ktl_hashmap_m(insert)(
    ktl_hashmap *const hashmap, ktl_hashmap_K const k, ktl_hashmap_V const v
)
{
    bool const success = ktl_hashmap_m(reserve)(hashmap, 1) &&
                         ktl_hashmap_m(_insert_no_grow)(hashmap, k, v);

#ifdef ktl_allocates_infallible
    if (!success)
    {
        assert(success && "hashmap insert failed");
        exit(1);
    }
#else
    return success;
#endif
}

static ktl_nodiscard bool ktl_hashmap_m(_get_node)(
    ktl_hashmap const *const hashmap,
    ktl_hashmap_K const k,
    ktl_hashmap_node **const node,
    ktl_hashmap_node **const maybe_parent_node
)
{
    bool ok = false;

    if (hashmap->count > 0)
    {
        uint32_t hash = 0;
        KTL_TEMPLATE(ktl_hashmap_K, hash)(&k, &hash, ktl_hashmap_hash_fn);
        size_t const idx = hash % hashmap->buf_len;

        ktl_hashmap_node *current_node = &hashmap->buf[idx];
        ktl_hashmap_node *parent_node = NULL;
        while (current_node->next)
        {
            if (ktl_hashmap_K_eq(&current_node->key, &k))
            {
                ok = true;
                if (node)
                {
                    *node = current_node;
                }
                if (parent_node)
                {
                    *maybe_parent_node = parent_node;
                }
                break;
            }

            parent_node = current_node;
            current_node = current_node->next;
        }
    }

    return ok;
}

ktl_nodiscard bool ktl_hashmap_m(get)(
    ktl_hashmap const *const hashmap,
    ktl_hashmap_K const k,
    ktl_hashmap_V *const v
)
{
    ktl_hashmap_node *node;
    bool const ok = ktl_hashmap_m(_get_node)(hashmap, k, &node, NULL);

    if (ok && v)
    {
        *v = node->value;
    }

    return ok;
}

ktl_nodiscard bool
ktl_hashmap_m(remove)(ktl_hashmap *hashmap, ktl_hashmap_K k, ktl_hashmap_V *v)
{
    ktl_hashmap_node *node;
    ktl_hashmap_node *maybe_parent_node;
    bool const ok =
        ktl_hashmap_m(_get_node)(hashmap, k, &node, &maybe_parent_node);

    if (ok)
    {
        if (v)
        {
            *v = node->value;
        }

        if (maybe_parent_node)
        {
            maybe_parent_node->next = node->next;
            ktl_allocates_m(_free)(hashmap, node);
        }
        else
        {
            (void)memset(node, 0, sizeof(*node));
        }

        --(hashmap->count);
    }

    return ok;
}
