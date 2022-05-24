#include <stdlib.h>

#include "./conversion.h"

Srt* convert_function_to_ptr(Srt* srt) {
    if (srt->dtype == NULL || srt->dtype->type != DTYPE_FUNCUCTION) return srt;

    Dtype* converted_dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, converted_dtype, 1, srt);
}
