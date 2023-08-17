#include "./test_x64.h"
#include "../../src/x64/x64.h"

#include <stdlib.h>

void test_new_inst_x64_0arg(void);
void test_new_inst_x64_1arg(void);
void test_new_inst_x64_2arg(void);
void test_new_int_data_x64(void);
void test_new_str_data_x64(void);
void test_new_label_x64(void);
void test_x64_tostring_0arg_0suffix_inst(void);
void test_x64_tostring_0arg_1suffix_inst(void);
void test_x64_tostring_1arg_0suffix_inst(void);
void test_x64_tostring_1arg_1suffix_inst(void);
void test_x64_tostring_2arg_1suffix_inst(void);
void test_x64_tostring_2arg_2suffix_inst(void);
void test_x64_tostring_int_data(void);
void test_x64_tostring_str_data(void);
void test_x64_tostring_label(void);

CU_Suite* add_test_suite_x64(void) {
    CU_Suite* suite = CU_add_suite("test_suite_x64", NULL, NULL);
    CU_ADD_TEST(suite, test_new_inst_x64_0arg);
    CU_ADD_TEST(suite, test_new_inst_x64_1arg);
    CU_ADD_TEST(suite, test_new_inst_x64_2arg);
    CU_ADD_TEST(suite, test_new_int_data_x64);
    CU_ADD_TEST(suite, test_new_str_data_x64);
    CU_ADD_TEST(suite, test_new_label_x64);
    CU_ADD_TEST(suite, test_x64_tostring_0arg_0suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_0arg_1suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_1arg_0suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_1arg_1suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_2arg_1suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_2arg_2suffix_inst);
    CU_ADD_TEST(suite, test_x64_tostring_int_data);
    CU_ADD_TEST(suite, test_x64_tostring_str_data);
    CU_ADD_TEST(suite, test_x64_tostring_label);
    return suite;
}

void test_new_inst_x64_0arg(void) {
    X64* x64 = new_inst_x64(X64_INST_RET, NULL, NULL);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_INST);
        CU_ASSERT_EQUAL(x64->inst->type, X64_INST_RET);
        CU_ASSERT_PTR_NULL(x64->inst->dst);
        CU_ASSERT_PTR_NULL(x64->inst->src);
        CU_ASSERT_PTR_NULL(x64->data);
        CU_ASSERT_PTR_NULL(x64->label);
    }

    delete_x64(x64);
}

void test_new_inst_x64_1arg(void) {
    X64Ope* src = new_jlabel_x64ope(new_string("function"));
    X64* x64 = new_inst_x64(X64_INST_CALL, src, NULL);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            src = copied_x64->inst->src;
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_INST);
        CU_ASSERT_EQUAL(x64->inst->type, X64_INST_CALL);
        CU_ASSERT_PTR_EQUAL(x64->inst->src, src);
        CU_ASSERT_PTR_NULL(x64->inst->dst);
        CU_ASSERT_PTR_NULL(x64->data);
        CU_ASSERT_PTR_NULL(x64->label);
    }

    delete_x64(x64);
}

void test_new_inst_x64_2arg(void) {
    X64Ope* src = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
    X64Ope* dst = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
    X64* x64 = new_inst_x64(X64_INST_ADDX, src, dst);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            src = copied_x64->inst->src;
            dst = copied_x64->inst->dst;
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_INST);
        CU_ASSERT_EQUAL(x64->inst->type, X64_INST_ADDX);
        CU_ASSERT_PTR_EQUAL(x64->inst->src, src);
        CU_ASSERT_PTR_EQUAL(x64->inst->dst, dst);
        CU_ASSERT_PTR_NULL(x64->data);
        CU_ASSERT_PTR_NULL(x64->label);
    }

    delete_x64(x64);
}

void test_new_int_data_x64(void) {
    X64* x64 = new_int_data_x64(X64_DATA_QUAD, new_signed_iliteral(INTEGER_INT, 623));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_DATA);
        CU_ASSERT_PTR_NULL(x64->inst);
        CU_ASSERT_EQUAL(x64->data->type, X64_DATA_QUAD);
        CU_ASSERT_EQUAL(x64->data->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(iliteral_type_isunsigned(x64->data->iliteral->type));
        CU_ASSERT_EQUAL(x64->data->iliteral->signed_value, 623);
        CU_ASSERT_EQUAL(x64->data->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(x64->data->sliteral);
        CU_ASSERT_PTR_NULL(x64->label);
    }

    delete_x64(x64);
}

void test_new_str_data_x64(void) {
    char sliteral_const[15] = "string\0literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    X64* x64 = new_str_data_x64(X64_DATA_STR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_DATA);
        CU_ASSERT_PTR_NULL(x64->inst);
        CU_ASSERT_EQUAL(x64->data->type, X64_DATA_STR);
        CU_ASSERT_PTR_NULL(x64->data->iliteral);
        CU_ASSERT_EQUAL(memcmp(x64->data->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(x64->data->sliteral->size, sliteral_size);
        CU_ASSERT_PTR_NULL(x64->label);
    }

    delete_x64(x64);
}

void test_new_label_x64(void) {
    X64* x64 = new_label_x64(X64_LABEL_VARIABLE, X64_VIS_GLOBAL, new_string("variable"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64* copied_x64 = x64_copy(x64);
            delete_x64(x64);
            x64 = copied_x64;
        }
        CU_ASSERT_EQUAL(x64->type, X64_LABEL);
        CU_ASSERT_PTR_NULL(x64->inst);
        CU_ASSERT_PTR_NULL(x64->data);
        CU_ASSERT_EQUAL(x64->label->type, X64_LABEL_VARIABLE);
        CU_ASSERT_EQUAL(x64->label->visibility, X64_VIS_GLOBAL);
        CU_ASSERT_STRING_EQUAL(x64->label->name, "variable");
    }

    delete_x64(x64);
}

void test_x64_tostring_0arg_0suffix_inst(void) {
    X64* x64 = new_inst_x64(X64_INST_RET, NULL, NULL);
    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\tret\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_0arg_1suffix_inst(void) {
    X64Ope* src = new_suffix_x64ope(X64_SUFFIX_LONG);
    X64* x64 = new_inst_x64(X64_INST_CXTD, src, NULL);
    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\tcltd\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_1arg_0suffix_inst(void) {
    X64Ope* src = new_jlabel_x64ope(new_string("function"));
    X64* x64 = new_inst_x64(X64_INST_CALL, src, NULL);

    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\tcall\tfunction\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_1arg_1suffix_inst(void) {
    X64Ope* src = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
    X64* x64 = new_inst_x64(X64_INST_PUSHX, src, NULL);
    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\tpushq\t$8\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_2arg_1suffix_inst(void) {
    X64Ope* src = NULL;
    X64Ope* dst = NULL;
    X64* x64 = NULL;
    char* x64_str = NULL;

    src = new_signed_x64ope(X64_SUFFIX_WORD, INTEGER_INT, 3);
    dst = new_reg_x64ope(X64_SUFFIX_WORD, CX_REG_ID);
    x64 = new_inst_x64(X64_INST_ADDX, src, dst);
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\taddw\t$3, %cx\n");
    delete_x64(x64);
    free(x64_str);

    src = new_mem_x64ope(8);
    dst = new_reg_x64ope(X64_SUFFIX_LONG, CX_REG_ID);
    x64 = new_inst_x64(X64_INST_MOVX, src, dst);
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\tmovl\t-8(%rbp), %ecx\n");
    delete_x64(x64);
    free(x64_str);

    src = new_reg_x64ope(X64_SUFFIX_LONG, CX_REG_ID);
    dst = new_label_x64ope(new_string("a"));
    x64 = new_inst_x64(X64_INST_MOVX, src, dst);
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\tmovl\t%ecx, a(%rip)\n");
    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_2arg_2suffix_inst(void) {
    X64Ope* src = new_reg_x64ope(X64_SUFFIX_BYTE, DX_REG_ID);
    X64Ope* dst = new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID);
    X64* x64 = new_inst_x64(X64_INST_MOVSXX, src, dst);
    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\tmovsbl\t%dl, %eax\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_int_data(void) {
    X64* x64 = NULL;
    char* x64_str = NULL;

    x64 = new_int_data_x64(X64_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 4));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.byte\t4\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_int_data_x64(X64_DATA_WORD, new_signed_iliteral(INTEGER_INT, 5));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.word\t5\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_int_data_x64(X64_DATA_LONG, new_signed_iliteral(INTEGER_INT, 26));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.long\t26\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_int_data_x64(X64_DATA_QUAD, new_signed_iliteral(INTEGER_INT, 386));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.quad\t386\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_int_data_x64(X64_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 12));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.zero\t12\n");
    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_str_data(void) {
    char sliteral_const[15] = "string\0literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    X64* x64 = new_str_data_x64(X64_DATA_STR, sliteral);
    char* x64_str = x64_tostring(x64);

    CU_ASSERT_STRING_EQUAL(x64_str, "\t.ascii\t\"string\\000literal\\000\"\n");

    delete_x64(x64);
    free(x64_str);
}

void test_x64_tostring_label(void) {
    X64* x64 = NULL;
    char* x64_str = NULL;

    x64 = new_label_x64(X64_LABEL_NORMAL, X64_VIS_NONE, new_string("normal_none_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "normal_none_label:\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_label_x64(X64_LABEL_FUNCTION, X64_VIS_NONE, new_string("function_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.text\n"
                                    "function_label:\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_label_x64(X64_LABEL_VARIABLE, X64_VIS_NONE, new_string("variable_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.data\n"
                                    "variable_label:\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_label_x64(X64_LABEL_NORMAL, X64_VIS_GLOBAL, new_string("global_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.globl global_label\n"
                                    "global_label:\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_label_x64(X64_LABEL_NORMAL, X64_VIS_LOCAL, new_string("local_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.local local_label\n"
                                    "local_label:\n");
    delete_x64(x64);
    free(x64_str);

    x64 = new_label_x64(X64_LABEL_FUNCTION, X64_VIS_GLOBAL, new_string("global_function_label"));
    x64_str = x64_tostring(x64);
    CU_ASSERT_STRING_EQUAL(x64_str, "\t.text\n"
                                    "\t.globl global_function_label\n"
                                    "global_function_label:\n");
    delete_x64(x64);
    free(x64_str);
}
