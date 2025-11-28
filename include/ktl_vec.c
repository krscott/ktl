#include "ktl_vec.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef KTL_VEC_GROW_NUMERATOR
#define KTL_VEC_GROW_NUMERATOR 3
#define KTL_VEC_GROW_DENOMINATOR 2
#endif

#ifndef KTL_VEC_MIN_CAP
#define KTL_VEC_MIN_CAP 8
#endif

#ifndef KTL_VEC_GROW_CAP
#define KTL_VEC_GROW_CAP(cap)                                                  \
    ((cap) / KTL_VEC_GROW_DENOMINATOR * KTL_VEC_GROW_NUMERATOR)
#endif

void ktl_vec_m(deinit)(struct ktl_vec *const vec)
{
    ktl_vec_free(vec, vec->ptr);
}

ktl_vec_alloc_ok ktl_vec_m(reserve)(struct ktl_vec *const vec, size_t const n)
{
    assert(vec->ptr || (vec->len == 0 && vec->cap == 0));

    size_t const CAP_LIMIT =
        SIZE_MAX / KTL_VEC_GROW_NUMERATOR * KTL_VEC_GROW_DENOMINATOR;

    bool success;

    size_t const target_cap = vec->len + n
#ifdef ktl_vec_sentinel
                              + 1
#endif
        ;

    if (target_cap <= vec->cap)
    {
        success = true;
    }
    else if (vec->cap >= CAP_LIMIT)
    {
        success = false;
    }
    else
    {
        size_t new_cap = vec->cap < KTL_VEC_MIN_CAP
                             ? KTL_VEC_MIN_CAP
                             : KTL_VEC_GROW_CAP(vec->cap);
        while (new_cap < target_cap)
        {
            new_cap = KTL_VEC_GROW_CAP(new_cap);
        }

        void *const new_ptr =
            ktl_vec_realloc(vec, vec->ptr, new_cap * sizeof(vec->ptr[0]));

        if (new_ptr)
        {
            success = true;
            vec->ptr = (ktl_vec_Tptr)new_ptr;
            vec->cap = new_cap;
        }
        else
        {
            success = false;
        }
    }

#ifdef ktl_vec_infallible
    if (!success)
    {
        assert(success && "vec reserve failed");
        exit(1);
    }
#else
    return success;
#endif
}

ktl_vec_alloc_ok ktl_vec_m(append_array)(
    struct ktl_vec *const vec, ktl_vec_T const *const arr, size_t const n
)
{
#ifdef ktl_vec_infallible

    ktl_vec_m(reserve)(vec, n);
    memmove(&vec->ptr[vec->len], arr, sizeof(vec->ptr[0]) * n);
    vec->len += n;

#ifdef ktl_vec_sentinel
    vec->ptr[vec->len] = ktl_vec_sentinel;
#endif

#else

    bool const success = ktl_vec_m(reserve)(vec, n);

    if (success)
    {
        memmove(&vec->ptr[vec->len], arr, sizeof(vec->ptr[0]) * n);
        vec->len += n;

#ifdef ktl_vec_sentinel
        vec->ptr[vec->len] = ktl_vec_sentinel;
#endif
    }

    return success;

#endif
}
