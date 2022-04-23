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

CType* copy_ctype(CType* ctype) {
    CType* copied_ctype = malloc(sizeof(CType));
    copied_ctype->type = ctype->type;

    if (ctype->func_args != NULL) {
        Vector* copied_func_args = new_vector();
        int num_args = vector_size(ctype->func_args);
        for (int i = 0; i < num_args; i++) {
            vector_push(copied_func_args, copy_ctype(vector_at(ctype->func_args, i)));
        }
        copied_ctype->func_args = copied_func_args;
    }

    if (ctype->func_return != NULL) {
        copied_ctype->func_return = copy_ctype(ctype->func_return);
    }

    return copied_ctype;
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
