#include <stdlib.h>

#include "./conversion.h"

Srt* convert_function_to_ptr(Srt* srt) {
    if (srt->ctype == NULL || srt->ctype->type != CTYPE_FUNCUCTION) return srt;

    CType* converted_ctype = new_pointer_ctype(ctype_copy(srt->ctype));
    return new_ctyped_srt(SRT_ADDR_EXPR, converted_ctype, 1, srt);
}
