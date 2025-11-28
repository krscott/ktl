#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

struct mock_allocator
{
    bool fail;
};
static void *
mock_allocator_realloc(struct mock_allocator *allocator, void *ptr, size_t size)
{
    return allocator->fail ? NULL : realloc(ptr, size);
}
static void mock_allocator_free(struct mock_allocator *allocator, void *ptr)
{
    (void)allocator;
    free(ptr);
}

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
#define ktl_vec strbuf
#include "ktl_vec.c"
#undef ktl_vec

struct strbuf_inf
{
    char *ptr;
    size_t len;
    size_t cap;
};
#define strbuf_inf__type char
#define strbuf_inf__terminated true, '\0'
#define strbuf_inf__infallible_alloc true
#define ktl_vec strbuf_inf
#include "ktl_vec.c"
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

    return 0;
}
