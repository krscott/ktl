#include "ktest.inc"
#include "ktl/lib/clib_allocator.h"
#include "ktl/lib/hash.h"
#include <stdint.h>
#include <string.h>

#define str__type char
#define str__terminated true, '\0'
#define ktl_slice str
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

static bool str_eq(str const *a, str const *b)
{
    return a->len == b->len && 0 == strncmp(a->ptr, b->ptr, a->len);
}
#define str__eq true

static void str_hash(str const *s, uint32_t *state, ktl_hash_fn hash_func)
{
    hash_func(state, s->ptr, s->len * sizeof(s->ptr[0]));
}
#define str__hash true

#define dict__key str
#define dict__value str
#define dict__local_allocator true, ktl_allocator
#define ktl_hashmap dict
#include "ktl/struct/hashmap.h"
#include "ktl/struct/hashmap.inc"
#undef ktl_hashmap

// Always return the same hash
static inline void unlucky_hash(uint32_t *state, void const *key, size_t size)
{
    (void)key;
    (void)size;
    *state = 0xFFFFFFFF;
}

#define unluckymap__key str
#define unluckymap__value int
#define unluckymap__local_allocator true, ktl_allocator
#define unluckymap__hash_fn true, unlucky_hash
#define ktl_hashmap unluckymap
#include "ktl/struct/hashmap.h"
#include "ktl/struct/hashmap.inc"
#undef ktl_hashmap

KTEST_MAIN
{
    KTEST(t_hashmap_smoketest)
    {
        dict d = dict_init(clib_allocator);
        dict_deinit(&d);
    }

    KTEST(t_hashmap_reserve)
    {
        dict d = dict_init(clib_allocator);
        ASSERT(d.buf == NULL);
        ASSERT(d.buf_len == 0);

        ASSERT(dict_reserve(&d, 1));
        ASSERT(d.buf != NULL);
        ASSERT(d.buf_len > 0);

        dict_deinit(&d);
    }

    KTEST(t_hashmap_get_null)
    {

        dict d = dict_init(clib_allocator);
        ASSERT_FALSE(dict_get(&d, str_from_terminated("foo"), NULL));

        ASSERT(dict_reserve(&d, 1));
        ASSERT_FALSE(dict_get(&d, str_from_terminated("foo"), NULL));

        dict_deinit(&d);
    }

    KTEST(t_hashmap_insert_and_get)
    {
        str out = {0};
        dict d = dict_init(clib_allocator);
        ASSERT_INT_EQ(d.count, 0);
        ASSERT_FALSE(dict_get(&d, str_from_terminated("foo"), &out));

        ASSERT(dict_insert(
            &d,
            str_from_terminated("foo"),
            str_from_terminated("bar")
        ));
        ASSERT_INT_EQ(d.count, 1);

        ASSERT(dict_get(&d, str_from_terminated("foo"), &out));
        ASSERT(out.ptr);
        ASSERT_STR_EQ("bar", out.ptr);

        ASSERT_FALSE(dict_get(&d, str_from_terminated("qux"), &out));

        dict_deinit(&d);
    }

    KTEST(t_hashmap_remove)
    {
        str out = {0};
        dict d = dict_init(clib_allocator);
        ASSERT_FALSE(dict_remove(&d, str_from_terminated("foo"), &out));

        ASSERT(dict_insert(
            &d,
            str_from_terminated("foo"),
            str_from_terminated("bar")
        ));
        ASSERT_INT_EQ(d.count, 1);

        ASSERT(dict_remove(&d, str_from_terminated("foo"), &out));
        ASSERT(out.ptr);
        ASSERT_STR_EQ("bar", out.ptr);
        ASSERT_INT_EQ(d.count, 0);

        ASSERT_FALSE(dict_get(&d, str_from_terminated("foo"), &out));

        dict_deinit(&d);
    }

    KTEST(t_unlucky)
    {
        int out = 0;
        unluckymap m = unluckymap_init(clib_allocator);

        ASSERT(unluckymap_insert(&m, str_from_terminated("one"), 1));
        ASSERT_INT_EQ(m.count, 1);
        ASSERT(unluckymap_insert(&m, str_from_terminated("two"), 2));
        ASSERT_INT_EQ(m.count, 2);
        ASSERT(unluckymap_insert(&m, str_from_terminated("three"), 3));
        ASSERT_INT_EQ(m.count, 3);
        ASSERT(unluckymap_insert(&m, str_from_terminated("four"), 4));
        ASSERT_INT_EQ(m.count, 4);

        ASSERT(unluckymap_remove(&m, str_from_terminated("one"), &out));
        ASSERT_INT_EQ(out, 1);
        ASSERT(unluckymap_remove(&m, str_from_terminated("two"), &out));
        ASSERT_INT_EQ(out, 2);
        ASSERT(unluckymap_remove(&m, str_from_terminated("three"), &out));
        ASSERT_INT_EQ(out, 3);
        ASSERT(unluckymap_remove(&m, str_from_terminated("four"), &out));
        ASSERT_INT_EQ(out, 4);
        ASSERT_INT_EQ(m.count, 0);

        unluckymap_deinit(&m);
    }

    KTEST(t_iter)
    {
        dict m = dict_init(clib_allocator);

        ASSERT(dict_insert(
            &m,
            str_from_terminated("one"),
            str_from_terminated("1")
        ));
        ASSERT(dict_insert(
            &m,
            str_from_terminated("two"),
            str_from_terminated("2")
        ));
        ASSERT(dict_insert(
            &m,
            str_from_terminated("three"),
            str_from_terminated("3")
        ));
        ASSERT(dict_insert(
            &m,
            str_from_terminated("four"),
            str_from_terminated("4")
        ));

        size_t count = 0;
        str k;
        str v;
        for (dict__iter iter = dict_iter(&m); dict_next(&iter, &k, &v);)
        {
            ktest_infof("%.*s: %.*s", (int)k.len, k.ptr, (int)v.len, v.ptr);
            ++count;
        }
        ASSERT_INT_EQ(count, 4);

        dict_deinit(&m);
    }
}
