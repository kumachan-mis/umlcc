#include "./testlib.h"

#include <stdlib.h>
#include <string.h>

int testlib_ctokens_equals(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    if (vector_size(actual) != vector_size(expected)) return 0;

    int num_tokens = vector_size(expected);
    for (int i = 0; i < num_tokens; i++) {
        CToken* actual_ctoken = vector_at(actual, i);
        CToken* expected_ctoken = vector_at(expected, i);
        if (!testlib_ctoken_equals(actual_ctoken, expected_ctoken)) return 0;
    }
    return 1;
}

int testlib_ctoken_equals(CToken* actual, CToken* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_string_equals(actual->ident_name, expected->ident_name) &&
            testlib_iliteral_equals(actual->iliteral, expected->iliteral) &&
            testlib_sliteral_equals(actual->sliteral, expected->sliteral));
}

int testlib_ast_equals(Ast* actual, Ast* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    if (actual->type != expected->type ||
        !testlib_string_equals(actual->ident_name, expected->ident_name) ||
        !testlib_iliteral_equals(actual->iliteral, expected->iliteral) ||
        !testlib_sliteral_equals(actual->sliteral, expected->sliteral) ||
        vector_size(actual->children) != vector_size(expected->children)) {
        return 0;
    }

    int num_children = vector_size(expected->children);
    for (int i = 0; i < num_children; i++) {
        Ast* actual_child = vector_at(actual->children, i);
        Ast* expected_child = vector_at(expected->children, i);
        if (!testlib_ast_equals(actual_child, expected_child)) return 0;
    }
    return 1;
}

int testlib_srt_equals(Srt* actual, Srt* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    if (actual->type != expected->type || !testlib_dtype_equals(actual->dtype, expected->dtype) ||
        !testlib_string_equals(actual->ident_name, expected->ident_name) ||
        !testlib_iliteral_equals(actual->iliteral, expected->iliteral) ||
        !testlib_sliteral_equals(actual->sliteral, expected->sliteral) ||
        vector_size(actual->children) != vector_size(expected->children)) {
        return 0;
    }

    int num_children = vector_size(expected->children);
    for (int i = 0; i < num_children; i++) {
        Srt* actual_child = vector_at(actual->children, i);
        Srt* expected_child = vector_at(expected->children, i);
        if (!testlib_srt_equals(actual_child, expected_child)) return 0;
    }
    return 1;
}

int testlib_immcs_equals(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    if (vector_size(actual) != vector_size(expected)) return 0;

    int num_immcs = vector_size(expected);
    for (int i = 0; i < num_immcs; i++) {
        Immc* actual_immc = vector_at(actual, i);
        Immc* expected_immc = vector_at(expected, i);
        if (!testlib_immc_equals(actual_immc, expected_immc)) return 0;
    }
    return 1;
}

int testlib_immc_equals(Immc* actual, Immc* expected) {
    int testlib_immcinst_equals(ImmcInst * actual, ImmcInst * expected);
    int testlib_immcdata_equals(ImmcData * actual, ImmcData * expected);
    int testlib_immclabel_equals(ImmcLabel * actual, ImmcLabel * expected);

    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_immcinst_equals(actual->inst, expected->inst) &&
            testlib_immcdata_equals(actual->data, expected->data) &&
            testlib_immclabel_equals(actual->label, expected->label));
}

int testlib_immcinst_equals(ImmcInst* actual, ImmcInst* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_immcope_equals(actual->fst_src, expected->fst_src) &&
            testlib_immcope_equals(actual->snd_src, expected->snd_src) &&
            testlib_immcope_equals(actual->dst, expected->dst));
}

int testlib_immcdata_equals(ImmcData* actual, ImmcData* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_iliteral_equals(actual->iliteral, expected->iliteral) &&
            testlib_sliteral_equals(actual->sliteral, expected->sliteral));
}

int testlib_immclabel_equals(ImmcLabel* actual, ImmcLabel* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type && actual->visibility == expected->visibility &&
            testlib_string_equals(actual->name, expected->name));
}

int testlib_immcope_equals(ImmcOpe* actual, ImmcOpe* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type && actual->suffix == expected->suffix &&
            actual->reg_id == expected->reg_id && actual->mem_offset == expected->mem_offset &&
            testlib_string_equals(actual->label_name, expected->label_name) &&
            testlib_iliteral_equals(actual->iliteral, expected->iliteral) &&
            testlib_sliteral_equals(actual->sliteral, expected->sliteral));
}

int testlib_dtype_equals(Dtype* actual, Dtype* expected) {
    int testlib_dpointer_equals(DPointer * actual, DPointer * expected);
    int testlib_darray_equals(DArray * actual, DArray * expected);
    int testlib_dfunction_equals(DFunction * actual, DFunction * expected);
    int testlib_ddecoration_equals(DDecoration * actual, DDecoration * expected);

    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_dpointer_equals(actual->pointer, expected->pointer) &&
            testlib_darray_equals(actual->array, expected->array) &&
            testlib_dfunction_equals(actual->function, expected->function) &&
            testlib_ddecoration_equals(actual->decoration, expected->decoration));
}

int testlib_dpointer_equals(DPointer* actual, DPointer* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return testlib_dtype_equals(actual->to_dtype, expected->to_dtype);
}

int testlib_darray_equals(DArray* actual, DArray* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (testlib_dtype_equals(actual->of_dtype, expected->of_dtype) &&
            actual->size == expected->size);
}

int testlib_dfunction_equals(DFunction* actual, DFunction* expected) {
    int testlib_dparam_equals(DParam * actual, DParam * expected);

    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    if (!testlib_dtype_equals(actual->return_dtype, expected->return_dtype) ||
        vector_size(actual->params) != vector_size(expected->params)) {
        return 0;
    }

    int num_params = vector_size(expected->params);
    for (int i = 0; i < num_params; i++) {
        DParam* actual_param = vector_at(actual->params, i);
        DParam* expected_param = vector_at(expected->params, i);
        if (!testlib_dparam_equals(actual_param, expected_param)) return 0;
    }
    return 1;
}

int testlib_dparam_equals(DParam* actual, DParam* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (testlib_dtype_equals(actual->dtype, expected->dtype) &&
            testlib_string_equals(actual->ident_name, expected->ident_name));
}

int testlib_ddecoration_equals(DDecoration* actual, DDecoration* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->typedef_flag == expected->typedef_flag &&
            testlib_dtype_equals(actual->deco_dtype, expected->deco_dtype));
}

int testlib_iliteral_equals(IntegerLiteral* actual, IntegerLiteral* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type && actual->is_unsigned == expected->is_unsigned &&
            actual->signed_value == expected->signed_value &&
            actual->unsigned_value == expected->unsigned_value);
}

int testlib_sliteral_equals(StringLiteral* actual, StringLiteral* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    if (actual->size != expected->size) return 0;
    return memcmp(actual->value, expected->value, expected->size) == 0;
}

int testlib_string_equals(char* actual, char* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    return strcmp(actual, expected) == 0;
}
