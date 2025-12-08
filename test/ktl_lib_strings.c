#include <assert.h>
#include <stdio.h>

#include "ktl/lib/strings.h"

#include "ktl/lib/strings.inc"

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

static void t_ints_are_ord(void)
{
    i8 a = 11;
    i8 b = -20;
    assert(i8_cmp(&a, &a) == 0);
    assert(i8_cmp(&a, &b) > 0);
    assert(i8_cmp(&b, &a) < 0);
}

static void t_strs(void)
{
    str a = str_from_terminated("Hello");
    assert(a.ptr[4] == 'o');
}

static void t_strbufs(void)
{
    strbuf b = strbuf_init();

    strbuf_append_terminated(&b, "one");
    strbuf_push(&b, ' ');
    strbuf_append_terminated(&b, "two");
    strbuf_push(&b, ' ');
    strbuf_append_terminated(&b, "oatmeal");

    assert(strview_eq(
        strbuf_as_strview(b),
        strview_from_terminated("one two oatmeal")
    ));

    strbuf_deinit(&b);
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
    RUN(t_ints_are_ord);
    RUN(t_strs);
    RUN(t_strbufs);

    return 0;
}
