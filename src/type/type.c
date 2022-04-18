#include "./type.h"

#include <stdlib.h>


CType* new_integer_ctype() {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_INT;
    ctype->func_args = NULL;
    ctype->func_return = NULL;
    return ctype;
}

CType* new_function_ctype(Vector* func_args, CType* func_return) {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_FUNC;
    ctype->func_args = func_args;
    ctype->func_return = func_return;
    return ctype;
}

void delete_ctype(CType* ctype) {
    if (ctype->func_args != NULL) {
        delete_vector(ctype->func_args, (void (*)(void* item))delete_ctype);
    }
    if (ctype->func_return != NULL) {
        delete_ctype(ctype->func_return);
    }
    free(ctype);
}
