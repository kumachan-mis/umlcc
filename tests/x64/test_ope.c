#include "./test_ope.h"
#include "../../src/common/type.h"
#include "../../src/x64/ope.h"
#include "../../src/x64/register.h"

#include <stdlib.h>

void test_new_suffix_x64ope();
void test_new_reg_x64ope();
void test_new_ptr_x64ope();
void test_new_jptr_x64ope();
void test_new_mem_x64ope();
void test_new_label_x64ope();
void test_new_jlabel_x64ope();
void test_new_int_x64ope();
void test_new_signed_x64ope();
void test_new_unsigned_x64ope();
void test_x64ope_tostring_suffix();
void test_x64ope_tostring_reg();
void test_x64ope_tostring_ptr();
void test_x64ope_tostring_jptr();
void test_x64ope_tostring_mem();
void test_x64ope_tostring_label();
void test_x64ope_tostring_jlabel();
void test_x64ope_tostring_int();
void test_x64ope_tostring_signed();
void test_x64ope_tostring_unsigned();

CU_Suite* add_test_suite_x64ope() {
    CU_Suite* suite = CU_add_suite("test_suite_x64ope", NULL, NULL);
    CU_ADD_TEST(suite, test_new_suffix_x64ope);
    CU_ADD_TEST(suite, test_new_reg_x64ope);
    CU_ADD_TEST(suite, test_new_ptr_x64ope);
    CU_ADD_TEST(suite, test_new_jptr_x64ope);
    CU_ADD_TEST(suite, test_new_mem_x64ope);
    CU_ADD_TEST(suite, test_new_label_x64ope);
    CU_ADD_TEST(suite, test_new_jlabel_x64ope);
    CU_ADD_TEST(suite, test_new_int_x64ope);
    CU_ADD_TEST(suite, test_new_signed_x64ope);
    CU_ADD_TEST(suite, test_new_unsigned_x64ope);
    CU_ADD_TEST(suite, test_x64ope_tostring_suffix);
    CU_ADD_TEST(suite, test_x64ope_tostring_reg);
    CU_ADD_TEST(suite, test_x64ope_tostring_ptr);
    CU_ADD_TEST(suite, test_x64ope_tostring_jptr);
    CU_ADD_TEST(suite, test_x64ope_tostring_mem);
    CU_ADD_TEST(suite, test_x64ope_tostring_label);
    CU_ADD_TEST(suite, test_x64ope_tostring_jlabel);
    CU_ADD_TEST(suite, test_x64ope_tostring_int);
    CU_ADD_TEST(suite, test_x64ope_tostring_signed);
    CU_ADD_TEST(suite, test_x64ope_tostring_unsigned);
    return suite;
}

void test_new_suffix_x64ope() {
    X64Ope* ope = new_suffix_x64ope(X64_SUFFIX_QUAD);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_SUFFIX);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_QUAD);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_reg_x64ope() {
    X64Ope* ope = new_reg_x64ope(X64_SUFFIX_WORD, AX_REG_ID);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_REG);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_WORD);
        CU_ASSERT_EQUAL(ope->reg_id, AX_REG_ID);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_ptr_x64ope() {
    X64Ope* ope = new_ptr_x64ope(SP_REG_ID);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_PTR);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_QUAD);
        CU_ASSERT_EQUAL(ope->reg_id, SP_REG_ID);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_jptr_x64ope() {
    X64Ope* ope = new_jptr_x64ope(DX_REG_ID);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_JPTR);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_QUAD);
        CU_ASSERT_EQUAL(ope->reg_id, DX_REG_ID);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_mem_x64ope() {
    X64Ope* ope = new_mem_x64ope(8);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_MEM);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, 8);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_label_x64ope() {
    X64Ope* ope = new_label_x64ope(new_string("global_variable"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_LABEL);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_STRING_EQUAL(ope->label_name, "global_variable");
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_jlabel_x64ope() {
    X64Ope* ope = new_jlabel_x64ope(new_string("function"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_JLABEL);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_STRING_EQUAL(ope->label_name, "function");
        CU_ASSERT_PTR_NULL(ope->iliteral);
    }

    delete_x64ope(ope);
}

void test_new_int_x64ope() {
    X64Ope* ope = new_int_x64ope(X64_SUFFIX_WORD, new_signed_iliteral(INTEGER_INT, 56));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_WORD);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, 56);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 0ULL);
    }

    delete_x64ope(ope);
}

void test_new_signed_x64ope() {
    X64Ope* ope = new_signed_x64ope(X64_SUFFIX_BYTE, INTEGER_INT, 14);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_BYTE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, 14);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 0ULL);
    }

    delete_x64ope(ope);
}

void test_new_unsigned_x64ope() {
    X64Ope* ope = new_unsigned_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2147483648U);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            X64Ope* copied_ope = x64ope_copy(ope);
            delete_x64ope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, X64_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, X64_SUFFIX_LONG);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_TRUE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, -1LL);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 2147483648U);
    }

    delete_x64ope(ope);
}

void test_x64ope_tostring_suffix() {
    X64Ope* ope = new_suffix_x64ope(X64_SUFFIX_QUAD);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_reg() {
    X64Ope* ope = new_reg_x64ope(X64_SUFFIX_WORD, AX_REG_ID);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "%ax");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_ptr() {
    X64Ope* ope = new_ptr_x64ope(SP_REG_ID);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "(%rsp)");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_jptr() {
    X64Ope* ope = new_jptr_x64ope(DX_REG_ID);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "*%rdx");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_mem() {
    X64Ope* ope = new_mem_x64ope(8);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "-8(%rbp)");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_label() {
    X64Ope* ope = new_label_x64ope(new_string("global_variable"));
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "global_variable(%rip)");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_jlabel() {
    X64Ope* ope = new_jlabel_x64ope(new_string("function"));
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "function");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_int() {
    X64Ope* ope = new_int_x64ope(X64_SUFFIX_WORD, new_signed_iliteral(INTEGER_INT, 56));
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "$56");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_signed() {
    X64Ope* ope = new_signed_x64ope(X64_SUFFIX_BYTE, INTEGER_INT, 14);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "$14");

    free(ope_str);
    delete_x64ope(ope);
}

void test_x64ope_tostring_unsigned() {
    X64Ope* ope = new_unsigned_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2147483648U);
    char* ope_str = x64ope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "$2147483648");

    free(ope_str);
    delete_x64ope(ope);
}
