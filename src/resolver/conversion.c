#include <stdlib.h>

#include "./conversion.h"

Srt* convert_to_ptr(Srt* srt) {
    Dtype* dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Srt* convert_to_ptr_if_array(Srt* srt) {
    if (srt->dtype == NULL || srt->dtype->type != DTYPE_ARRAY) return srt;
    Dtype* dtype = new_pointer_dtype(dtype_copy(srt->dtype->array->of_dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Srt* convert_to_ptr_if_function(Srt* srt) {
    if (srt->dtype == NULL || srt->dtype->type != DTYPE_FUNCUCTION) return srt;
    Dtype* dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Srt* perform_usual_arithmetic_conversion(Srt* srt) {
    if (srt->dtype == NULL || !dtype_isarithmetic(srt->dtype)) return srt;
    return perform_integer_promotion(srt);
}

Srt* perform_integer_promotion(Srt* srt) {
    if (srt->dtype == NULL || !dtype_isinteger(srt->dtype)) return srt;
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    if (srt->dtype->type == DTYPE_CHAR) return new_dtyped_srt(SRT_CAST_EXPR, dtype, 1, srt);
    return srt;
}
