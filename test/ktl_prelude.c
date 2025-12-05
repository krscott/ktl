#include <assert.h>
#include <stdio.h>

#define KTL_PRELUDE_IMPL
#include "ktl/prelude.h"

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

    return 0;
}
