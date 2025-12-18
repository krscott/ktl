#include "ktest.inc"
#include "ktl/macros.h"
#include <stdio.h>
#include <string.h>

static inline int int_cmp_rev(void const *a, void const *b)
{
    return *(int const *)b - *(int const *)a;
}

static inline int int_cmp(int const *a, int const *b)
{
    return *a - *b;
}
#define int__ord true
static inline int char_cmp(char const *a, char const *b)
{
    return *a - *b;
}
#define char__ord true

#define intslice__type int
#define intslice__mut true
#define ktl_slice intslice
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define str__type char
#define str__terminated true, '\0'
#define ktl_slice str
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

static inline int str_cmp(str const *a, str const *b)
{
    int x = strncmp(a->ptr, b->ptr, a->len < b->len ? a->len : b->len);
    if (x == 0)
    {
        x = (int)a->len - (int)b->len;
    }
    return x;
}
#define str__ord true

#define strslice__type str
#define strslice__mut true
#define ktl_slice strslice
#include "ktl/struct/slice.h"
#include "ktl/struct/slice.inc"
#undef ktl_slice

KTEST_MAIN
{

    KTEST(t_contains)
    {
        int arr_a[] = {100, 55, 33};
        intslice a = {
            .ptr = arr_a,
            .len = ktl_countof(arr_a),
        };

        ASSERT_TRUE(intslice_contains(a, 33));
        ASSERT_TRUE(!intslice_contains(a, 0));
    }

    KTEST(t_contains_null)
    {
        intslice a = {0};
        ASSERT_TRUE(!intslice_contains(a, 0));
    }

    KTEST(t_find_index)
    {
        int arr_a[] = {100, 55, 33};
        intslice a = {
            .ptr = arr_a,
            .len = ktl_countof(arr_a),
        };

        size_t index = 0;
        ASSERT_TRUE(intslice_find_index(a, 33, &index));
        ASSERT_TRUE(index == 2);
        ASSERT_TRUE(!intslice_find_index(a, 0, &index));
    }

    KTEST(t_find_index_null)
    {
        intslice a = {0};
        ASSERT_TRUE(!intslice_find_index(a, 0, NULL));
    }

    KTEST(t_split)
    {
        char arr_a[] = "Hello World!";
        str a = {
            .ptr = arr_a,
            .len = strlen(arr_a),
        };

        str head;
        str tail;
        ASSERT_TRUE(str_split(a, ' ', &head, &tail));
        ASSERT_TRUE(head.len == 5);
        ASSERT_TRUE(0 == strncmp(head.ptr, "Hello", head.len));
        ASSERT_TRUE(tail.len == 6);
        ASSERT_TRUE(0 == strncmp(tail.ptr, "World!", tail.len));
    }

    KTEST(t_split_one)
    {
        char arr_a[] = "foo";
        str a = {
            .ptr = arr_a,
            .len = strlen(arr_a),
        };

        str head;
        str tail;
        ASSERT_TRUE(str_split(a, ' ', &head, &tail));
        ASSERT_TRUE(head.len == 3);
        ASSERT_TRUE(0 == strncmp(head.ptr, "foo", head.len));
        ASSERT_TRUE(tail.len == 0);
    }

    KTEST(t_split_null)
    {
        str a = {0};
        ASSERT_TRUE(!str_split(a, 'a', NULL, NULL));
    }

    KTEST(t_split_at)
    {
        char arr_a[] = "Hello World!";
        str a = {
            .ptr = arr_a,
            .len = strlen(arr_a),
        };

        str head;
        str tail;

        str_split_at(a, 0, &head, &tail);
        ASSERT_TRUE(head.len == 0);
        ASSERT_TRUE(tail.len == a.len);
        ASSERT_TRUE(0 == strncmp(tail.ptr, "Hello World!", tail.len));

        str_split_at(a, a.len - 1, &head, &tail);
        ASSERT_TRUE(head.len == a.len - 1);
        ASSERT_TRUE(0 == strncmp(head.ptr, "Hello World", head.len));
        ASSERT_TRUE(tail.len == 1);
        ASSERT_TRUE(0 == strncmp(tail.ptr, "!", tail.len));

        str_split_at(a, a.len, &head, &tail);
        ASSERT_TRUE(head.len == a.len);
        ASSERT_TRUE(0 == strncmp(head.ptr, "Hello World!", head.len));
        ASSERT_TRUE(tail.len == 0);
    }

    KTEST(t_split_at_null)
    {
        str a = {0};
        str head;
        str tail;
        str_split_at(a, 0, &head, &tail);
        ASSERT_TRUE(head.len == 0);
        ASSERT_TRUE(tail.len == 0);
    }

    KTEST(t_eq)
    {
        int arr_a[] = {1, 2, 3, 4, 5};
        int arr_b[] = {1, 2, 3, 4, 5};
        int arr_c[] = {1, 2, 3, 4, 55};
        int arr_d[] = {1, 2, 3, 4, 5, 6};
        intslice a = {
            .ptr = arr_a,
            .len = ktl_countof(arr_a),
        };
        intslice b = {
            .ptr = arr_b,
            .len = ktl_countof(arr_b),
        };
        intslice c = {
            .ptr = arr_c,
            .len = ktl_countof(arr_c),
        };
        intslice d = {
            .ptr = arr_d,
            .len = ktl_countof(arr_d),
        };

        ASSERT_TRUE(intslice_eq(a, b));
        ASSERT_TRUE(!intslice_eq(a, c));
        ASSERT_TRUE(!intslice_eq(a, d));
    }

    KTEST(t_eq_null)
    {

        int arr_a[] = {1, 2, 3, 4, 5};
        intslice a = {
            .ptr = arr_a,
            .len = ktl_countof(arr_a),
        };
        intslice b = {0};
        intslice c = {0};

        ASSERT_TRUE(!intslice_eq(a, b));
        ASSERT_TRUE(intslice_eq(b, c));
    }

    KTEST(t_sort)
    {
        int ints[] = {5, 10, 7, 1, -12};
        intslice a = {
            .ptr = ints,
            .len = ktl_countof(ints),
        };

        intslice_sort(a);

        ASSERT_INT_EQ(ints[0], -12);
        ASSERT_INT_EQ(ints[1], 1);
        ASSERT_INT_EQ(ints[2], 5);
        ASSERT_INT_EQ(ints[3], 7);
        ASSERT_INT_EQ(ints[4], 10);
    }

    KTEST(t_sort_null)
    {
        intslice a = {0};
        intslice_sort(a);
    }

    KTEST(t_sort_by)
    {
        int ints[] = {5, 10, 7, 1, -12};
        intslice a = {
            .ptr = ints,
            .len = ktl_countof(ints),
        };

        intslice_sort_by(a, int_cmp_rev);

        ASSERT_INT_EQ(ints[4], -12);
        ASSERT_INT_EQ(ints[3], 1);
        ASSERT_INT_EQ(ints[2], 5);
        ASSERT_INT_EQ(ints[1], 7);
        ASSERT_INT_EQ(ints[0], 10);
    }

    KTEST(t_bsearch)
    {
        int ints[] = {1, 11, 22, 33, 99};
        intslice a = {
            .ptr = ints,
            .len = ktl_countof(ints),
        };

        ASSERT_TRUE(intslice_bsearch(a, 22, NULL));
        ASSERT_TRUE(!intslice_bsearch(a, 10, NULL));

        {
            int *match = NULL;
            ASSERT_TRUE(intslice_bsearch(a, 99, &match));
            ASSERT_TRUE(match);
            ASSERT_TRUE(*match == 99);

            ASSERT_TRUE(ints[4] == 99);
            *match = 100;
            ASSERT_TRUE(ints[4] == 100);
        }

        ASSERT_TRUE(intslice_bsearch_index(a, 22, NULL));
        ASSERT_TRUE(!intslice_bsearch_index(a, 10, NULL));

        {
            size_t idx = 0;
            ASSERT_TRUE(intslice_bsearch_index(a, 33, &idx));
            ASSERT_TRUE(a.ptr[idx] == 33);
        }
    }

    KTEST(t_bsearch_null)
    {
        intslice a = {0};
        ASSERT_TRUE(!intslice_bsearch(a, 0, NULL));
        ASSERT_TRUE(!intslice_bsearch_index(a, 0, NULL));
    }

    KTEST(t_from_terminated)
    {
        char const *s = "Hello, World!";
        str a = str_from_terminated(s);
        ASSERT_TRUE(a.ptr == s);
        ASSERT_TRUE(a.len == strlen(s));
    }

    KTEST(t_slice_of_slices)
    {
        str strs[] = {
            str_from_terminated("Foo"),
            str_from_terminated("Bar"),
            str_from_terminated("Qux"),
        };

        // Make separate string to ensure compiler doesn't use same pointer
        str splitter = str_from_terminated("Bar2");
        splitter.len -= 1;

        strslice strings = {
            .ptr = strs,
            .len = ktl_countof(strs),
        };

        strslice head;
        strslice tail;
        ASSERT_TRUE(strslice_split(strings, splitter, &head, &tail));

        ASSERT_TRUE(str_eq(head.ptr[0], str_from_terminated("Foo")));
        ASSERT_TRUE(str_eq(tail.ptr[0], str_from_terminated("Qux")));
    }

    KTEST(t_alphabatize)
    {
        str strs[] = {
            str_from_terminated("banana"),
            str_from_terminated("cherry"),
            str_from_terminated("apple"),
        };

        strslice strings = {
            .ptr = strs,
            .len = ktl_countof(strs),
        };

        strslice_sort(strings);

        ASSERT_TRUE(str_eq(strings.ptr[0], str_from_terminated("apple")));
        ASSERT_TRUE(str_eq(strings.ptr[1], str_from_terminated("banana")));
        ASSERT_TRUE(str_eq(strings.ptr[2], str_from_terminated("cherry")));
    }
}
