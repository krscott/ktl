#include "ktl/lib/strings.h"

#include <assert.h>

#define ktl_slice str
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define ktl_slice strview
#include "ktl/struct/slice.inc"
#undef ktl_slice

#define ktl_vec strbuf
#include "ktl/struct/vec.inc"
#define ktl_slice str
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#define ktl_slice strview
#include "ktl/trait/convert_vec_slice.inc"
#undef ktl_slice
#undef ktl_vec
