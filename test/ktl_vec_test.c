#include "mock_allocator.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

// str slice types

#define str__type char
#define str__mut true
#define str__terminated true, '\0'
#define str__impl true

#define ktl_slice str
#include "ktl/array/slice.h"
#undef ktl_slice

#define strview__type char
#define strview__terminated true, '\0'
#define strview__impl true

#define ktl_slice strview
#include "ktl/array/slice.h"
#undef ktl_slice

// Fallible vec

struct strbuf
{
    struct mock_allocator allocator;
    char *ptr;
    size_t len;
    size_t cap;
};
#define strbuf__type char
#define strbuf__terminated true, '\0'
#define strbuf__allocator true
// #define strbuf__realloc(vec, p, size) realloc((p), (size))
#define strbuf__realloc(vec, p, size)                                          \
    mock_allocator_realloc(&(vec)->allocator, p, size)
#define strbuf__free(vec, p) mock_allocator_free(&(vec)->allocator, p)
#define strbuf__impl true

#define ktl_vec strbuf
#include "ktl/array/vec.h"
// Supports converting to both str and strview
#define ktl_slice str
#include "ktl/array/vec_slice.h"
#undef ktl_slice
#define ktl_slice strview
#include "ktl/array/vec_slice.h"
#undef ktl_slice
#undef ktl_vec

// Infallible vec

struct strbuf_inf
{
    char *ptr;
    size_t len;
    size_t cap;
};
#define strbuf_inf__type char
#define strbuf_inf__terminated true, '\0'
#define strbuf_inf__infallible_alloc true
#define strbuf_inf__impl true

#define ktl_vec strbuf_inf
#define ktl_slice str
#include "ktl/array/vec.h"
#include "ktl/array/vec_slice.h"
#undef ktl_slice
#undef ktl_vec

static void t_deinit_null(void)
{
    struct strbuf buf = {0};
    strbuf_deinit(&buf);
}

static void t_reserve(void)
{
    struct strbuf buf = {0};

    assert(strbuf_reserve(&buf, 100));
    assert(buf.ptr != NULL);
    assert(buf.len == 0);
    assert(buf.cap >= 100);

    // Don't allocate if cap does not need to increase
    size_t const old_cap = buf.cap;
    assert(strbuf_reserve(&buf, 100));
    assert(buf.cap == old_cap);

    strbuf_deinit(&buf);
}

static void t_reserve_fail(void)
{
    struct strbuf buf = {0};

    buf.allocator.fail = true;
    assert(!strbuf_reserve(&buf, 100));

    buf.allocator.fail = false;
    assert(strbuf_reserve(&buf, 100));

    strbuf_deinit(&buf);
}

static void t_reserve_infallible(void)
{
    struct strbuf_inf buf = {0};

    strbuf_inf_reserve(&buf, 100);
    assert(buf.ptr != NULL);
    assert(buf.len == 0);
    assert(buf.cap >= 100);

    // Don't allocate if cap does not need to increase
    size_t const old_cap = buf.cap;
    strbuf_inf_reserve(&buf, 100);
    assert(buf.cap == old_cap);

    strbuf_inf_deinit(&buf);
}

static void t_append(void)
{
    char const *s = "foobar";
    size_t const s_len = strlen(s);

    struct strbuf buf = {0};

    assert(strbuf_append(&buf, s, s_len));
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len);
    assert(buf.cap >= s_len);
    assert(buf.ptr[buf.len] == '\0');

    assert(strbuf_append(&buf, s, s_len));
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len * 2);
    assert(buf.cap >= s_len * 2);
    assert(buf.ptr[buf.len] == '\0');

    assert(0 == strcmp(buf.ptr, "foobarfoobar"));

    strbuf_deinit(&buf);
}

static void t_append_infallible(void)
{
    char const *s = "foobar";
    size_t const s_len = strlen(s);

    struct strbuf_inf buf = {0};

    strbuf_inf_append(&buf, s, s_len);
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len);
    assert(buf.cap >= s_len);
    assert(buf.ptr[buf.len] == '\0');

    strbuf_inf_append(&buf, s, s_len);
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len * 2);
    assert(buf.cap >= s_len * 2);
    assert(buf.ptr[buf.len] == '\0');

    assert(0 == strcmp(buf.ptr, "foobarfoobar"));

    strbuf_inf_deinit(&buf);
}

static void t_append_terminated(void)
{
    char const *s = "foobar";
    size_t const s_len = strlen(s);

    struct strbuf buf = {0};

    assert(strbuf_append_terminated(&buf, s));
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len);
    assert(buf.cap >= s_len);
    assert(buf.ptr[buf.len] == '\0');

    assert(strbuf_append_terminated(&buf, s));
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len * 2);
    assert(buf.cap >= s_len * 2);
    assert(buf.ptr[buf.len] == '\0');

    assert(0 == strcmp(buf.ptr, "foobarfoobar"));

    strbuf_deinit(&buf);
}

static void t_append_terminated_infallible(void)
{
    char const *s = "foobar";
    size_t const s_len = strlen(s);

    struct strbuf_inf buf = {0};

    strbuf_inf_append_terminated(&buf, s);
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len);
    assert(buf.cap >= s_len);
    assert(buf.ptr[buf.len] == '\0');

    strbuf_inf_append_terminated(&buf, s);
    assert(buf.ptr != NULL);
    assert(buf.ptr != s);
    assert(buf.len == s_len * 2);
    assert(buf.cap >= s_len * 2);
    assert(buf.ptr[buf.len] == '\0');

    assert(0 == strcmp(buf.ptr, "foobarfoobar"));

    strbuf_inf_deinit(&buf);
}

static void t_push(void)
{
    struct strbuf buf = {0};

    assert(strbuf_append_terminated(&buf, "Hello"));
    assert(buf.len == 5);

    assert(strbuf_push(&buf, '!'));
    assert(buf.len == 6);
    assert(buf.ptr[buf.len - 1] == '!');
    assert(0 == strcmp(buf.ptr, "Hello!"));

    strbuf_deinit(&buf);
}

static void t_push_infallible(void)
{
    struct strbuf_inf buf = {0};

    strbuf_inf_append_terminated(&buf, "Hello");
    assert(buf.len == 5);

    strbuf_inf_push(&buf, '!');
    assert(buf.len == 6);
    assert(buf.ptr[buf.len - 1] == '!');
    assert(0 == strcmp(buf.ptr, "Hello!"));

    strbuf_inf_deinit(&buf);
}

static void t_pop(void)
{
    struct strbuf buf = {0};
    char c = '\0';

    assert(!strbuf_pop(&buf, &c));

    assert(strbuf_append_terminated(&buf, "Hello!"));
    assert(buf.len == 6);

    assert(strbuf_pop(&buf, &c));
    assert(buf.len == 5);
    assert(c == '!');
    assert(0 == strcmp(buf.ptr, "Hello"));

    assert(strbuf_pop(&buf, &c));
    assert(strbuf_pop(&buf, &c));
    assert(strbuf_pop(&buf, &c));
    assert(strbuf_pop(&buf, &c));
    assert(strbuf_pop(&buf, &c));

    assert(buf.len == 0);
    assert(c == 'H');

    assert(!strbuf_pop(&buf, &c));

    strbuf_deinit(&buf);
}

static void t_vec_as_slice(void)
{
    struct strbuf buf = {0};
    assert(strbuf_append_terminated(&buf, "foo"));
    assert(strbuf_append_terminated(&buf, "bar"));

    {
        struct str str = strbuf_as_str(&buf);
        assert(str.len == 6);
        assert(0 == strcmp(str.ptr, "foobar"));
    }

    {
        struct strview sv = strbuf_as_strview(&buf);
        assert(sv.len == 6);
        assert(0 == strcmp(sv.ptr, "foobar"));
    }

    strbuf_deinit(&buf);
}

static void t_vec_append_slice(void)
{
    struct str a = str_from_terminated("dead");
    struct strview b = strview_from_terminated("beef");

    struct strbuf buf = {0};
    assert(strbuf_append_str(&buf, a));
    assert(strbuf_append_strview(&buf, b));

    assert(buf.len == 8);
    assert(0 == strcmp(buf.ptr, "deadbeef"));

    strbuf_deinit(&buf);
}

static void t_vec_append_slice_infallible(void)
{
    struct str a = str_from_terminated("dead");
    struct str b = str_from_terminated("beef");

    struct strbuf_inf buf = {0};
    strbuf_inf_append_str(&buf, a);
    strbuf_inf_append_str(&buf, b);

    assert(buf.len == 8);
    assert(0 == strcmp(buf.ptr, "deadbeef"));

    strbuf_inf_deinit(&buf);
}

#define RUN(test)                                                              \
    do                                                                         \
    {                                                                          \
        printf("Test: " #test "\n");                                           \
        fflush(stdout);                                                        \
        test();                                                                \
    } while (0)

int main(void)
{
    RUN(t_deinit_null);
    RUN(t_reserve);
    RUN(t_reserve_fail);
    RUN(t_reserve_infallible);
    RUN(t_append);
    RUN(t_append_infallible);
    RUN(t_append_terminated);
    RUN(t_append_terminated_infallible);
    RUN(t_push);
    RUN(t_push_infallible);
    RUN(t_pop);
    RUN(t_vec_as_slice);
    RUN(t_vec_append_slice);
    RUN(t_vec_append_slice_infallible);

    return 0;
}
