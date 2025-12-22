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
#include "ktl/lib/allocator.h"
#define dict__local_allocator true, ktl_allocator
#define ktl_hashmap dict
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
        dict d = dict_init(clib_allocator);
        ASSERT_INT_EQ(d.count, 0);

        ASSERT(dict_insert(
            &d,
            str_from_terminated("foo"),
            str_from_terminated("bar")
        ));
        ASSERT_INT_EQ(d.count, 1);

        str out = {0};
        ASSERT(dict_get(&d, str_from_terminated("foo"), &out));
        ASSERT(out.ptr);
        ASSERT_STR_EQ("bar", out.ptr);

        ASSERT_FALSE(dict_get(&d, str_from_terminated("qux"), &out));

        dict_deinit(&d);
    }
}
