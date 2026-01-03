#include "ktl/lib/io.h"
#include "ktl/lib/io.inc"
#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"

#include "ktest.inc"
#include <stdio.h>

#define TESTFILE_CONTENTS "Hello, World!\n"

KTEST_MAIN
{
    KTEST(t_strbuf_append_stream)
    {
        {
            FILE *testfile = fopen("/tmp/ktl-io-test.txt", "w");
            fprintf(testfile, TESTFILE_CONTENTS);
            fclose(testfile);
        }

        {
            strbuf b = strbuf_init();

            FILE *testfile = fopen("/tmp/ktl-io-test.txt", "r");
            ASSERT(strbuf_append_stream(&b, testfile));
            fclose(testfile);

            ASSERT_STR_EQ(b.ptr, TESTFILE_CONTENTS);

            strbuf_deinit(&b);
        }
    }
}
