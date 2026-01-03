#ifndef KTL_LIB_IO_H_
#define KTL_LIB_IO_H_

#include "ktl/lib/strings.h"
#include "ktl/macros.h"
#include <stdio.h>

ktl_nodiscard bool strbuf_append_stream(strbuf *b, FILE *stream);

#endif
