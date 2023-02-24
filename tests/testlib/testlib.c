#include "./testlib.h"

#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>

void testlib_assert_ctokens_equal(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_tokens = vector_size(expected);
    for (int i = 0; i < num_tokens; i++) {
        CToken* actual_ctoken = vector_at(actual, i);
        CToken* expected_ctoken = vector_at(expected, i);
        testlib_assert_ctoken_equal(actual_ctoken, expected_ctoken);
    }
}

void testlib_assert_ctoken_equal(CToken* actual, CToken* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_string_equal(actual->ident_name, expected->ident_name);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);
}

void testlib_assert_ast_equal(Ast* actual, Ast* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_string_equal(actual->ident_name, expected->ident_name);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);

    CU_ASSERT_EQUAL(vector_size(actual->children), vector_size(expected->children));
    if (vector_size(actual->children) != vector_size(expected->children)) return;

    int num_children = vector_size(expected->children);
    for (int i = 0; i < num_children; i++) {
        Ast* actual_child = vector_at(actual->children, i);
        Ast* expected_child = vector_at(expected->children, i);
        testlib_assert_ast_equal(actual_child, expected_child);
    }
}

void testlib_assert_srt_equal(Srt* actual, Srt* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_dtype_equal(actual->dtype, expected->dtype);
    testlib_assert_string_equal(actual->ident_name, expected->ident_name);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);

    CU_ASSERT_EQUAL(vector_size(actual->children), vector_size(expected->children));
    if (vector_size(actual->children) != vector_size(expected->children)) return;

    int num_children = vector_size(expected->children);
    for (int i = 0; i < num_children; i++) {
        Srt* actual_child = vector_at(actual->children, i);
        Srt* expected_child = vector_at(expected->children, i);
        testlib_assert_srt_equal(actual_child, expected_child);
    }
}

void testlib_assert_immcs_equal(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_immcs = vector_size(expected);
    for (int i = 0; i < num_immcs; i++) {
        Immc* actual_immc = vector_at(actual, i);
        Immc* expected_immc = vector_at(expected, i);
        testlib_assert_immc_equal(actual_immc, expected_immc);
    }
}

void testlib_assert_immc_equal(Immc* actual, Immc* expected) {
    void testlib_assert_immcinst_equal(ImmcInst * actual, ImmcInst * expected);
    void testlib_assert_immcdata_equal(ImmcData * actual, ImmcData * expected);
    void testlib_assert_immclabel_equal(ImmcLabel * actual, ImmcLabel * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_immcinst_equal(actual->inst, expected->inst);
    testlib_assert_immcdata_equal(actual->data, expected->data);
    testlib_assert_immclabel_equal(actual->label, expected->label);
}

void testlib_assert_immcinst_equal(ImmcInst* actual, ImmcInst* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_immcope_equal(actual->fst_src, expected->fst_src);
    testlib_assert_immcope_equal(actual->snd_src, expected->snd_src);
    testlib_assert_immcope_equal(actual->dst, expected->dst);
}

void testlib_assert_immcdata_equal(ImmcData* actual, ImmcData* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);
}

void testlib_assert_immclabel_equal(ImmcLabel* actual, ImmcLabel* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    CU_ASSERT_EQUAL(actual->visibility, expected->visibility);
    testlib_assert_string_equal(actual->name, expected->name);
}

void testlib_assert_immcope_equal(ImmcOpe* actual, ImmcOpe* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    CU_ASSERT_EQUAL(actual->suffix, expected->suffix);
    CU_ASSERT_EQUAL(actual->reg_id, expected->reg_id);
    CU_ASSERT_EQUAL(actual->mem_offset, expected->mem_offset);
    testlib_assert_string_equal(actual->label_name, expected->label_name);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);
}

void testlib_assert_x64codes_equal(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_x64codes = vector_size(expected);
    for (int i = 0; i < num_x64codes; i++) {
        X64* actual_x64code = vector_at(actual, i);
        X64* expected_x64code = vector_at(expected, i);
        testlib_assert_x64code_equal(actual_x64code, expected_x64code);
    }
}

void testlib_assert_x64code_equal(X64* actual, X64* expected) {
    void testlib_assert_x64inst_equal(X64Inst * actual, X64Inst * expected);
    void testlib_assert_x64data_equal(X64Data * actual, X64Data * expected);
    void testlib_assert_x64label_equal(X64Label * actual, X64Label * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_x64inst_equal(actual->inst, expected->inst);
    testlib_assert_x64data_equal(actual->data, expected->data);
    testlib_assert_x64label_equal(actual->label, expected->label);
}

void testlib_assert_x64inst_equal(X64Inst* actual, X64Inst* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_x64ope_equal(actual->src, expected->src);
    testlib_assert_x64ope_equal(actual->dst, expected->dst);
}

void testlib_assert_x64data_equal(X64Data* actual, X64Data* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
    testlib_assert_sliteral_equal(actual->sliteral, expected->sliteral);
}

void testlib_assert_x64label_equal(X64Label* actual, X64Label* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    CU_ASSERT_EQUAL(actual->visibility, expected->visibility);
    testlib_assert_string_equal(actual->name, expected->name);
}

void testlib_assert_x64ope_equal(X64Ope* actual, X64Ope* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    CU_ASSERT_EQUAL(actual->suffix, expected->suffix);
    CU_ASSERT_EQUAL(actual->reg_id, expected->reg_id);
    CU_ASSERT_EQUAL(actual->mem_offset, expected->mem_offset);
    testlib_assert_string_equal(actual->label_name, expected->label_name);
    testlib_assert_iliteral_equal(actual->iliteral, expected->iliteral);
}

void testlib_assert_dtype_equal(DType* actual, DType* expected) {
    void testlib_assert_dpointer_equal(DPointer * actual, DPointer * expected);
    void testlib_assert_darray_equal(DArray * actual, DArray * expected);
    void testlib_assert_dstruct_equal(DStruct * actual, DStruct * expected);
    void testlib_assert_dfunction_equal(DFunction * actual, DFunction * expected);
    void testlib_assert_ddecoration_equal(DDecoration * actual, DDecoration * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    testlib_assert_dpointer_equal(actual->dpointer, expected->dpointer);
    testlib_assert_darray_equal(actual->darray, expected->darray);
    testlib_assert_dstruct_equal(actual->dstruct, expected->dstruct);
    testlib_assert_dfunction_equal(actual->dfunction, expected->dfunction);
    testlib_assert_ddecoration_equal(actual->ddecoration, expected->ddecoration);
}

void testlib_assert_dpointer_equal(DPointer* actual, DPointer* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_dtype_equal(actual->to_dtype, expected->to_dtype);
}

void testlib_assert_darray_equal(DArray* actual, DArray* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->size, expected->size);
    testlib_assert_dtype_equal(actual->of_dtype, expected->of_dtype);
}

void testlib_assert_dstruct_equal(DStruct* actual, DStruct* expected) {
    void testlib_assert_dstructmembers_equal(Vector * actual, Vector * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_string_equal(actual->name, expected->name);
    testlib_assert_dstructmembers_equal(actual->members, expected->members);
    CU_ASSERT_EQUAL(actual->nbytes, expected->nbytes);
    CU_ASSERT_EQUAL(actual->alignment, expected->alignment);
}

void testlib_assert_dstructmembers_equal(Vector* actual, Vector* expected) {
    void testlib_assert_dstructmember_equal(DStructMember * actual, DStructMember * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_members = vector_size(expected);
    for (int i = 0; i < num_members; i++) {
        DStructMember* actual_member = vector_at(actual, i);
        DStructMember* expected_member = vector_at(expected, i);
        testlib_assert_dstructmember_equal(actual_member, expected_member);
    }
}

void testlib_assert_dstructmember_equal(DStructMember* actual, DStructMember* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_string_equal(actual->name, expected->name);
    testlib_assert_dtype_equal(actual->dtype, expected->dtype);
}

void testlib_assert_dfunction_equal(DFunction* actual, DFunction* expected) {
    void testlib_assert_dparam_equal(DParam * actual, DParam * expected);

    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_dtype_equal(actual->return_dtype, expected->return_dtype);
    CU_ASSERT_EQUAL(vector_size(actual->params), vector_size(expected->params));
    if (vector_size(actual->params) != vector_size(expected->params)) return;

    int num_params = vector_size(expected->params);
    for (int i = 0; i < num_params; i++) {
        DParam* actual_param = vector_at(actual->params, i);
        DParam* expected_param = vector_at(expected->params, i);
        testlib_assert_dparam_equal(actual_param, expected_param);
    }
}

void testlib_assert_dparam_equal(DParam* actual, DParam* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_string_equal(actual->name, expected->name);
    testlib_assert_dtype_equal(actual->dtype, expected->dtype);
}

void testlib_assert_ddecoration_equal(DDecoration* actual, DDecoration* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->typedef_flag, expected->typedef_flag);
    testlib_assert_dtype_equal(actual->deco_dtype, expected->deco_dtype);
}

void testlib_assert_liveseqs_equal(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_liveseqs = vector_size(expected);
    for (int i = 0; i < num_liveseqs; i++) {
        Liveseq* actual_liveseq = vector_at(actual, i);
        Liveseq* expected_liveseq = vector_at(expected, i);
        testlib_assert_liveseq_equal(actual_liveseq, expected_liveseq);
    }
}

void testlib_assert_liveseq_equal(Liveseq* actual, Liveseq* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->liveness_index, expected->liveness_index);
    CU_ASSERT_EQUAL(actual->index, expected->index);

    CU_ASSERT_EQUAL(vector_size(actual->livenesses), vector_size(expected->livenesses));
    if (vector_size(actual->livenesses) != vector_size(expected->livenesses)) return;

    int num_livenesses = vector_size(expected->livenesses);
    for (int i = 0; i < num_livenesses; i++) {
        Liveness* actual_liveness = vector_at(actual->livenesses, i);
        Liveness* expected_liveness = vector_at(expected->livenesses, i);
        testlib_assert_liveness_equal(actual_liveness, expected_liveness);
    }
}

void testlib_assert_liveness_equal(Liveness* actual, Liveness* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->first_def_index, expected->first_def_index);
    CU_ASSERT_EQUAL(actual->last_use_index, expected->last_use_index);
}

void testlib_assert_iliteral_equal(IntegerLiteral* actual, IntegerLiteral* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->type, expected->type);
    CU_ASSERT_EQUAL(actual->is_unsigned, expected->is_unsigned);
    CU_ASSERT_EQUAL(actual->signed_value, expected->signed_value);
    CU_ASSERT_EQUAL(actual->unsigned_value, expected->unsigned_value);
}

void testlib_assert_sliteral_equal(StringLiteral* actual, StringLiteral* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(actual->size, expected->size);
    if (actual->size != expected->size) return;

    CU_ASSERT_EQUAL(memcmp(actual->value, expected->value, expected->size), 0);
}

void testlib_assert_errors_equal(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_EQUAL(vector_size(actual), vector_size(expected));
    if (vector_size(actual) != vector_size(expected)) return;

    int num_tokens = vector_size(expected);
    for (int i = 0; i < num_tokens; i++) {
        Error* actual_err = vector_at(actual, i);
        Error* expected_err = vector_at(expected, i);
        testlib_assert_error_equal(actual_err, expected_err);
    }
}

void testlib_assert_error_equal(Error* actual, Error* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    testlib_assert_string_equal(actual->message, expected->message);
}

void testlib_assert_string_equal(char* actual, char* expected) {
    if (actual == NULL || expected == NULL) {
        CU_ASSERT_TRUE(actual == NULL && expected == NULL);
        return;
    }

    CU_ASSERT_STRING_EQUAL(actual, expected);
}
