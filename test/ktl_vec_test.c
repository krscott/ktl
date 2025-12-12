#include "ktest.inc"
#include "ktl/lib/allocator.h"      // IWYU pragma: export
#include "ktl/lib/clib_allocator.h" // IWYU pragma: export
#include "ktl/lib/mock_allocator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char cmp

static inline int char_cmp(char const *a, char const *b)
{
    return *a - *b;
}
#define char__ord true

// str slice types

#define str__type char
#define str__mut true
#define str__terminated true, '\0'
#define ktl_slice str
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define strview__type char
#define strview__terminated true, '\0'
#define ktl_slice strview
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

// Fallible vec

#define strbuf__type char
#define strbuf__terminated true, '\0'
#define strbuf__local_allocator true, ktl_allocator

#define ktl_vec strbuf
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
// Supports converting to both str and strview
#define ktl_slice str
#include "ktl/trait/convert_vec_slice.h"
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#define ktl_slice strview
#include "ktl/trait/convert_vec_slice.h"
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#undef ktl_vec

// Infallible vec

#define strbuf_inf__type char
#define strbuf_inf__mut true // required for ord sort
#define strbuf_inf__terminated true, '\0'
#define strbuf_inf__infallible_allocator true

#define ktl_vec strbuf_inf
#define ktl_slice str
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#include "ktl/trait/convert_vec_slice.h"
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#undef ktl_vec

KTEST_MAIN
{
    KTEST(t_deinit_null)
    {
        strbuf buf = strbuf_init(clib_allocator);
        strbuf_deinit(&buf);
    }

    KTEST(t_reserve)
    {
        strbuf buf = strbuf_init(clib_allocator);

        ASSERT_TRUE(strbuf_reserve(&buf, 100));
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.len == 0);
        ASSERT_TRUE(buf.cap >= 100);

        // Don't allocate if cap does not need to increase
        size_t const old_cap = buf.cap;
        ASSERT_TRUE(strbuf_reserve(&buf, 100));
        ASSERT_TRUE(buf.cap == old_cap);

        strbuf_deinit(&buf);
    }

    KTEST(t_reserve_fail)
    {
        mock_allocator mockal = mock_allocator_init();
        strbuf buf = strbuf_init(mock_allocator_handle(&mockal));

        mockal.fail = true;
        ASSERT_TRUE(!strbuf_reserve(&buf, 100));

        mockal.fail = false;
        ASSERT_TRUE(strbuf_reserve(&buf, 100));

        strbuf_deinit(&buf);
    }

    KTEST(t_reserve_infallible)
    {
        strbuf_inf buf = strbuf_inf_init();

        strbuf_inf_reserve(&buf, 100);
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.len == 0);
        ASSERT_TRUE(buf.cap >= 100);

        // Don't allocate if cap does not need to increase
        size_t const old_cap = buf.cap;
        strbuf_inf_reserve(&buf, 100);
        ASSERT_TRUE(buf.cap == old_cap);

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_append)
    {
        char const *s = "foobar";
        size_t const s_len = strlen(s);

        strbuf buf = strbuf_init(clib_allocator);

        ASSERT_TRUE(strbuf_append(&buf, s, s_len));
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len);
        ASSERT_TRUE(buf.cap >= s_len);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(strbuf_append(&buf, s, s_len));
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len * 2);
        ASSERT_TRUE(buf.cap >= s_len * 2);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(0 == strcmp(buf.ptr, "foobarfoobar"));

        strbuf_deinit(&buf);
    }

    KTEST(t_append_infallible)
    {
        char const *s = "foobar";
        size_t const s_len = strlen(s);

        strbuf_inf buf = strbuf_inf_init();

        strbuf_inf_append(&buf, s, s_len);
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len);
        ASSERT_TRUE(buf.cap >= s_len);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        strbuf_inf_append(&buf, s, s_len);
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len * 2);
        ASSERT_TRUE(buf.cap >= s_len * 2);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(0 == strcmp(buf.ptr, "foobarfoobar"));

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_append_terminated)
    {
        char const *s = "foobar";
        size_t const s_len = strlen(s);

        strbuf buf = strbuf_init(clib_allocator);

        ASSERT_TRUE(strbuf_append_terminated(&buf, s));
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len);
        ASSERT_TRUE(buf.cap >= s_len);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(strbuf_append_terminated(&buf, s));
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len * 2);
        ASSERT_TRUE(buf.cap >= s_len * 2);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(0 == strcmp(buf.ptr, "foobarfoobar"));

        strbuf_deinit(&buf);
    }

    KTEST(t_append_terminated_infallible)
    {
        char const *s = "foobar";
        size_t const s_len = strlen(s);

        strbuf_inf buf = strbuf_inf_init();

        strbuf_inf_append_terminated(&buf, s);
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len);
        ASSERT_TRUE(buf.cap >= s_len);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        strbuf_inf_append_terminated(&buf, s);
        ASSERT_TRUE(buf.ptr != NULL);
        ASSERT_TRUE(buf.ptr != s);
        ASSERT_TRUE(buf.len == s_len * 2);
        ASSERT_TRUE(buf.cap >= s_len * 2);
        ASSERT_TRUE(buf.ptr[buf.len] == '\0');

        ASSERT_TRUE(0 == strcmp(buf.ptr, "foobarfoobar"));

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_push)
    {
        strbuf buf = strbuf_init(clib_allocator);

        ASSERT_TRUE(strbuf_append_terminated(&buf, "Hello"));
        ASSERT_TRUE(buf.len == 5);

        ASSERT_TRUE(strbuf_push(&buf, '!'));
        ASSERT_TRUE(buf.len == 6);
        ASSERT_TRUE(buf.ptr[buf.len - 1] == '!');
        ASSERT_TRUE(0 == strcmp(buf.ptr, "Hello!"));

        strbuf_deinit(&buf);
    }

    KTEST(t_push_infallible)
    {
        strbuf_inf buf = strbuf_inf_init();

        strbuf_inf_append_terminated(&buf, "Hello");
        ASSERT_TRUE(buf.len == 5);

        strbuf_inf_push(&buf, '!');
        ASSERT_TRUE(buf.len == 6);
        ASSERT_TRUE(buf.ptr[buf.len - 1] == '!');
        ASSERT_TRUE(0 == strcmp(buf.ptr, "Hello!"));

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_pop)
    {
        strbuf buf = strbuf_init(clib_allocator);
        char c = '\0';

        ASSERT_TRUE(!strbuf_pop(&buf, &c));

        ASSERT_TRUE(strbuf_append_terminated(&buf, "Hello!"));
        ASSERT_TRUE(buf.len == 6);

        ASSERT_TRUE(strbuf_pop(&buf, &c));
        ASSERT_TRUE(buf.len == 5);
        ASSERT_TRUE(c == '!');
        ASSERT_TRUE(0 == strcmp(buf.ptr, "Hello"));

        ASSERT_TRUE(strbuf_pop(&buf, &c));
        ASSERT_TRUE(strbuf_pop(&buf, &c));
        ASSERT_TRUE(strbuf_pop(&buf, &c));
        ASSERT_TRUE(strbuf_pop(&buf, &c));
        ASSERT_TRUE(strbuf_pop(&buf, &c));

        ASSERT_TRUE(buf.len == 0);
        ASSERT_TRUE(c == 'H');

        ASSERT_TRUE(!strbuf_pop(&buf, &c));

        strbuf_deinit(&buf);
    }

    KTEST(t_vec_as_slice)
    {
        strbuf buf = strbuf_init(clib_allocator);
        ASSERT_TRUE(strbuf_append_terminated(&buf, "foo"));
        ASSERT_TRUE(strbuf_append_terminated(&buf, "bar"));

        {
            str str = strbuf_as_str(buf);
            ASSERT_TRUE(str.len == 6);
            ASSERT_TRUE(0 == strcmp(str.ptr, "foobar"));
        }

        {
            strview sv = strbuf_as_strview(buf);
            ASSERT_TRUE(sv.len == 6);
            ASSERT_TRUE(0 == strcmp(sv.ptr, "foobar"));
        }

        strbuf_deinit(&buf);
    }

    KTEST(t_vec_append_slice)
    {
        str a = str_from_terminated("dead");
        strview b = strview_from_terminated("beef");

        strbuf buf = strbuf_init(clib_allocator);
        ASSERT_TRUE(strbuf_append_str(&buf, a));
        ASSERT_TRUE(strbuf_append_strview(&buf, b));

        ASSERT_TRUE(buf.len == 8);
        ASSERT_TRUE(0 == strcmp(buf.ptr, "deadbeef"));

        strbuf_deinit(&buf);
    }

    KTEST(t_vec_append_slice_infallible)
    {
        str a = str_from_terminated("dead");
        str b = str_from_terminated("beef");

        strbuf_inf buf = strbuf_inf_init();
        strbuf_inf_append_str(&buf, a);
        strbuf_inf_append_str(&buf, b);

        ASSERT_TRUE(buf.len == 8);
        ASSERT_TRUE(0 == strcmp(buf.ptr, "deadbeef"));

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_sort)
    {
        strbuf_inf buf = {0};
        strbuf_inf_append_terminated(&buf, "edcba");

        strbuf_inf_sort(buf);

        ASSERT_TRUE(0 == strcmp(buf.ptr, "abcde"));

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_bsearch)
    {
        strbuf_inf buf = strbuf_inf_init();
        strbuf_inf_append_terminated(&buf, "abcde");

        ASSERT_TRUE(strbuf_inf_bsearch(buf, 'c', NULL));
        ASSERT_TRUE(!strbuf_inf_bsearch(buf, 'z', NULL));

        {
            char *match = NULL;
            ASSERT_TRUE(strbuf_inf_bsearch(buf, 'd', &match));
            ASSERT_TRUE(match);
            ASSERT_TRUE(*match == 'd');
        }

        ASSERT_TRUE(strbuf_inf_bsearch_index(buf, 'b', NULL));
        ASSERT_TRUE(!strbuf_inf_bsearch_index(buf, 'w', NULL));

        {
            size_t idx = 0;
            ASSERT_TRUE(strbuf_inf_bsearch_index(buf, 'b', &idx));
            ASSERT_TRUE(buf.ptr[idx] == 'b');
        }

        strbuf_inf_deinit(&buf);
    }

    KTEST(t_clear)
    {
        mock_allocator mockal = mock_allocator_init();
        strbuf buf = strbuf_init(mock_allocator_handle(&mockal));

        strbuf_clear(&buf);
        ASSERT_UINT_EQ(buf.len, 0);

        ASSERT_TRUE(strbuf_append_terminated(&buf, "foobar"));
        ASSERT_UINT_NEQ(buf.len, 0);

        strbuf_clear(&buf);
        ASSERT_UINT_EQ(buf.len, 0);
        ASSERT_TRUE(buf.ptr[0] == '\0');

        strbuf_deinit(&buf);
    }
}
