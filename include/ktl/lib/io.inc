#include "ktl/lib/io.h"

#include "ktl/lib/strings.h"
#include "ktl/macros.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#ifndef STRBUF_STREAM_CHUNK_SIZE
#define STRBUF_STREAM_CHUNK_SIZE 4096u
#endif

ktl_nodiscard bool strbuf_append_stream(strbuf *const b, FILE *const stream)
{
    assert(stream);

    bool ok = true;

    while (ok)
    {
#ifndef ktl_allocates_infallible
        ok =
#endif
            strbuf_reserve(b, STRBUF_STREAM_CHUNK_SIZE);

        if (ok)
        {
            size_t const n = fread(
                &b->ptr[b->len],
                sizeof(char),
                STRBUF_STREAM_CHUNK_SIZE,
                stream
            );

            if (n == 0)
            {
                if (feof(stream))
                {
                    break;
                }
                if (ferror(stream))
                {
                    ok = false;
                    break;
                }
            }

            b->len += n;
            b->ptr[b->len] = '\0';
        }
    }

    return ok;
}
