#ifndef KTL_MACROS_H_
#define KTL_MACROS_H_

#if defined(__GNUC__) || defined(__clang__)
#define ktl_nodiscard __attribute__((warn_unused_result))
#else
#define ktl_nodiscard
#endif

#define KTL_STRINGIFY_(x) #x
#define KTL_STRINGIFY(x) KTL_STRINGIFY_(x)

#define KTL_CAT2(a, b) a##b
#define KTL_CAT3(a, b, c) a##b##c
#define KTL_CAT4(a, b, c, d) a##b##c##d
#define KTL_CAT5(a, b, c, d, e) a##b##c##d##e

#define KTL_GET0_(x, ...) x
#define KTL_GET1_(x, y, ...) y

#define KTL_GET0(...) KTL_GET0_(__VA_ARGS__, )
#define KTL_GET1(...) KTL_GET1_(__VA_ARGS__, , )

#define KTL_TEMPLATE(T, name) KTL_CAT3(T, _, name)
#define KTL_TEMPLATE2(T, name, U) KTL_CAT5(T, _, name, _, U)

#define ktl_countof(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(__GNUC__) || defined(__clang__)
#define KTL_PRAGMA(X) _Pragma(#X)
#define KTL_DIAG_PUSH KTL_PRAGMA(GCC diagnostic push)
#define KTL_DIAG_POP KTL_PRAGMA(GCC diagnostic pop)
#define KTL_DIAG_IGNORE(warningName)                                           \
    KTL_PRAGMA(GCC diagnostic ignored #warningName)
#else
#define KTL_DIAG_PUSH
#define KTL_DIAG_POP
#define KTL_DIAG_IGNORE(warningName)
#endif

typedef struct
{
    char dummy_;
} ktl_marker;

#endif
