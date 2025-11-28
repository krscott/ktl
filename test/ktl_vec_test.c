#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

struct strbuf
{
    char *ptr;
    size_t len;
    size_t cap;
};
#define strbuf__type char
#define strbuf__terminated true, '\0'
#define strbuf__allocator true
#define strbuf__realloc(vec, p, size) realloc((p), (size))
#define strbuf__free(vec, p) free(p)
#define ktl_vec strbuf
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

    return 0;
}
