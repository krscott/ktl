#ifndef KTL_MACROS_H_
#define KTL_MACROS_H_

#if defined(__GNUC__) || defined(__clang__)
#define ktl_nodiscard __attribute__((warn_unused_result))
#else
#define ktl_nodiscard
#endif

#define KTL_CAT2(a, b) a##b
#define KTL_CAT3(a, b, c) a##b##c
#define KTL_CAT4(a, b, c, d) a##b##c##d
#define KTL_CAT5(a, b, c, d, e) a##b##c##d##e

#define KTL_TEMPLATE(T, name) KTL_CAT3(T, _, name)

#define ktl_countof(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
