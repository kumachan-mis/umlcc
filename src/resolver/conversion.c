#include <stdlib.h>

#include "./conversion.h"

DTypeType get_unsigned_corresponding_signed(DTypeType type);

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

    if (dtype_equals(lhs_srt->dtype, rhs_srt->dtype)) {
        srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        return srt_pair;
    }

    int lhs_rank = dtype_integer_rank(lhs_srt->dtype), rhs_rank = dtype_integer_rank(rhs_srt->dtype);
    int lhs_nbytes = dtype_nbytes(lhs_srt->dtype), rhs_nbytes = dtype_nbytes(rhs_srt->dtype);
    int lhs_signed = dtype_issignedinteger(lhs_srt->dtype), rhs_signed = dtype_issignedinteger(rhs_srt->dtype);
    int lhs_unsigned = dtype_isunsignedinteger(lhs_srt->dtype), rhs_unsigned = dtype_isunsignedinteger(rhs_srt->dtype);

    // if both operands have signed integer types or both have unsigned integer types,
    // the operand with the type of lesser integer conversion rank is converted to
    // the type of the operand with greater rank
    if (lhs_signed && rhs_signed && lhs_rank > rhs_rank) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    } else if (lhs_signed && rhs_signed && lhs_rank < rhs_rank) {
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(rhs_srt->dtype), 1, lhs_srt);
    } else if (lhs_unsigned && rhs_unsigned && lhs_rank > rhs_rank) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    } else if (lhs_unsigned && rhs_unsigned && lhs_rank < rhs_rank) {
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(rhs_srt->dtype), 1, lhs_srt);
    }

    // if the operand that has unsigned integer type has rank greater or equal to
    // the rank of the type of the other operand,
    // then the operand with signed integer type is converted to the type of the operand with unsigned integer type
    else if (lhs_unsigned && rhs_signed && lhs_rank >= rhs_rank) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    } else if (lhs_signed && rhs_unsigned && lhs_rank <= rhs_rank) {
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(rhs_srt->dtype), 1, lhs_srt);
    }

    // if the type of the operand with signed integer type can represent
    // all of the values of the type of the operand with unsigned integer type,
    // then the operand with unsigned integer type is converted to the type of the operand with signed integer type
    else if (lhs_signed && rhs_unsigned && lhs_nbytes > rhs_nbytes) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    } else if (lhs_unsigned && rhs_signed && lhs_nbytes < rhs_nbytes) {
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(rhs_srt->dtype), 1, lhs_srt);
    }

    // both operands are converted to the unsigned integer type
    // corresponding to the type of the operand with signed integer type.
    else if (lhs_signed) {
        DType* dtype = new_integer_dtype(get_unsigned_corresponding_signed(lhs_srt->dtype->type));
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, lhs_srt);
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, rhs_srt);
        delete_dtype(dtype);
    } else if (rhs_signed) {
        DType* dtype = new_integer_dtype(get_unsigned_corresponding_signed(rhs_srt->dtype->type));
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, lhs_srt);
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, rhs_srt);
        delete_dtype(dtype);
    }

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

    return srt;
}

DTypeType get_unsigned_corresponding_signed(DTypeType type) {
    switch (type) {
        case DTYPE_INT:
            return DTYPE_UNSIGNED_INT;
        case DTYPE_LONG:
            return DTYPE_UNSIGNED_LONG;
        case DTYPE_LONGLONG:
            return DTYPE_UNSIGNED_LONGLONG;
        default:
            return DTYPE_VOID;
    }
}