#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"

#include "ktest.inc"

KTEST_MAIN
{

    KTEST(t_strs)
    {
        str a = str_from_terminated("Hello");
        ASSERT_TRUE(a.ptr[4] == 'o');
    }

    KTEST(t_strbufs)
    {
        strbuf b = strbuf_init();

        strbuf_append_terminated(&b, "one");
        strbuf_push(&b, ' ');
        strbuf_append_terminated(&b, "two");
        strbuf_push(&b, ' ');
        strbuf_append_terminated(&b, "oatmeal");

        ASSERT_TRUE(strview_eq(
            strbuf_as_strview(b),
            strview_from_terminated("one two oatmeal")
        ));

        strbuf_deinit(&b);
    }

    KTEST(t_trim)
    {
        str a = str_from_terminated(" \t\r\n foo \t\r\n ");

        str b = str_trim_start(a);
        ASSERT_TRUE(str_eq(b, str_from_terminated("foo \t\r\n ")));

        str c = str_trim_end(a);
        ASSERT_TRUE(str_eq(c, str_from_terminated(" \t\r\n foo")));

        str d = str_trim(a);
        ASSERT_TRUE(str_eq(d, str_from_terminated("foo")));
    }

    KTEST(t_starts_with)
    {
        str a = str_from_terminated("Hello");

        ASSERT(str_starts_with_cstr(a, "Hello"));
        ASSERT(str_starts_with_cstr(a, "He"));
        ASSERT_FALSE(str_starts_with_cstr(a, "Ho"));
        ASSERT_FALSE(str_starts_with_cstr(a, "Helloo"));

        ASSERT(str_starts_with(a, str_from_terminated("Hello")));
        ASSERT(str_starts_with(a, str_from_terminated("He")));
        ASSERT_FALSE(str_starts_with(a, str_from_terminated("Ho")));
        ASSERT_FALSE(str_starts_with(a, str_from_terminated("Helloo")));
    }
}
