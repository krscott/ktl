#include "ktl_macros.h"
#include <assert.h>
#include <stdio.h>

static int intslice__cmp(int a, int b) { return a - b; }
struct intslice
{
    int *ptr;
    size_t len;
};
#define intslice__type int
#define intslice__ord 1
#define ktl_slice intslice
#include "ktl_slice.c"

static void t_sort(void)
{
    int ints[] = {5, 10, 7, 1, -12};
    struct intslice a = {
        .ptr = ints,
        .len = ktl_countof(ints),
    };

    intslice_sort(a);

    assert(ints[0] == -12);
    assert(ints[1] == 1);
    assert(ints[2] == 5);
    assert(ints[3] == 7);
    assert(ints[4] == 10);
}

static void t_bsearch(void)
{
    int ints[] = {1, 11, 22, 33, 99};
    struct intslice a = {
        .ptr = ints,
        .len = ktl_countof(ints),
    };

    assert(intslice_bsearch(a, 22, NULL));
    assert(!intslice_bsearch(a, 10, NULL));

    int *match = NULL;
    assert(intslice_bsearch(a, 99, &match));
    assert(match);
    assert(*match == 99);

    assert(ints[4] == 99);
    *match = 100;
    assert(ints[4] == 100);
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
    RUN(t_sort);
    RUN(t_bsearch);

    return 0;
}
