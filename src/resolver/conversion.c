#include <stdlib.h>

#include "./conversion.h"

Srt* convert_to_ptr(Srt* srt) {
    DType* dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Srt* convert_to_ptr_if_array(Srt* srt) {
    if (srt->dtype == NULL || srt->dtype->type != DTYPE_ARRAY) return srt;
    DType* dtype = new_pointer_dtype(dtype_copy(srt->dtype->darray->of_dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Srt* convert_to_ptr_if_function(Srt* srt) {
    if (srt->dtype == NULL || srt->dtype->type != DTYPE_FUNCTION) return srt;
    DType* dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, srt);
}

Pair* perform_usual_arithmetic_conversion(Pair* srt_pair) {
    Srt* lhs_srt = pair_first(srt_pair);
    Srt* rhs_srt = pair_second(srt_pair);
    if (lhs_srt->dtype == NULL || rhs_srt->dtype == NULL || !dtype_isarithmetic(lhs_srt->dtype) ||
        !dtype_isarithmetic(rhs_srt->dtype)) {
        return srt_pair;
    }

    pair_assign((void**)&lhs_srt, (void**)&rhs_srt, srt_pair);

    lhs_srt = perform_integer_promotion(lhs_srt);
    rhs_srt = perform_integer_promotion(rhs_srt);

    srt_pair = new_pair(&t_srt, &t_srt);
    pair_set(srt_pair, lhs_srt, rhs_srt);
    return srt_pair;
}

Srt* perform_integer_promotion(Srt* srt) {
    if (srt->dtype == NULL || !dtype_isinteger(srt->dtype)) return srt;
    if (srt->dtype->type == DTYPE_CHAR) {
        DType* dtype = new_integer_dtype(DTYPE_INT);
        return new_dtyped_srt(SRT_CAST_EXPR, dtype, 1, srt);
    }

    // TODO: more rules may be added
    return srt;
}
