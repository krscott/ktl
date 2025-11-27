#include "ktl.h"
#include <assert.h>
#include <stdio.h>

static void t_sum(void)
{
    //
    assert(ktl_sum(1.0, 2.0) == 3.0);
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
    RUN(t_sum);

    return 0;
}
