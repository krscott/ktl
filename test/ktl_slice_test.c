#include "ktl_macros.h"
#include <assert.h>
#include <stdio.h>

#ifdef NDEBUG
#error "Asserts are disabled in release"
#endif

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

static void t_contains(void)
{
    int arr_a[] = {100, 55, 33};
    struct intslice a = {
        .ptr = arr_a,
        .len = ktl_countof(arr_a),
    };

    assert(intslice_contains(a, 33));
    assert(!intslice_contains(a, 0));
}

static void t_contains_null(void)
{
    struct intslice a = {0};
    assert(!intslice_contains(a, 0));
}

static void t_eq(void)
{
    int arr_a[] = {1, 2, 3, 4, 5};
    int arr_b[] = {1, 2, 3, 4, 5};
    int arr_c[] = {1, 2, 3, 4, 55};
    int arr_d[] = {1, 2, 3, 4, 5, 6};
    struct intslice a = {
        .ptr = arr_a,
        .len = ktl_countof(arr_a),
    };
    struct intslice b = {
        .ptr = arr_b,
        .len = ktl_countof(arr_b),
    };
    struct intslice c = {
        .ptr = arr_c,
        .len = ktl_countof(arr_c),
    };
    struct intslice d = {
        .ptr = arr_d,
        .len = ktl_countof(arr_d),
    };

    assert(intslice_eq(a, b));
    assert(!intslice_eq(a, c));
    assert(!intslice_eq(a, d));
}

static void t_eq_null(void)
{

    int arr_a[] = {1, 2, 3, 4, 5};
    struct intslice a = {
        .ptr = arr_a,
        .len = ktl_countof(arr_a),
    };
    struct intslice b = {0};
    struct intslice c = {0};

    assert(!intslice_eq(a, b));
    assert(intslice_eq(b, c));
}

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

static void t_sort_null(void)
{
    struct intslice a = {0};
    intslice_sort(a);
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

    {
        int *match = NULL;
        assert(intslice_bsearch(a, 99, &match));
        assert(match);
        assert(*match == 99);

        assert(ints[4] == 99);
        *match = 100;
        assert(ints[4] == 100);
    }

    assert(intslice_bsearch_index(a, 22, NULL));
    assert(!intslice_bsearch_index(a, 10, NULL));

    {
        size_t idx = 0;
        assert(intslice_bsearch_index(a, 33, &idx));
        assert(a.ptr[idx] == 33);
    }
}

static void t_bsearch_null(void)
{
    struct intslice a = {0};
    assert(!intslice_bsearch(a, 0, NULL));
    assert(!intslice_bsearch_index(a, 0, NULL));
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
    RUN(t_contains);
    RUN(t_contains_null);
    RUN(t_eq);
    RUN(t_eq_null);
    RUN(t_sort);
    RUN(t_sort_null);
    RUN(t_bsearch);
    RUN(t_bsearch_null);

    return 0;
}
