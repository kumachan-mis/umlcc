#include "./test_ope.h"
#include "../../src/immc/ope.h"

#include <stdlib.h>

void test_new_arg_immcope();
void test_new_reg_immcope();
void test_new_ptr_immcope();
void test_new_mem_immcope();
void test_new_label_immcope();
void test_new_int_immcope();
void test_new_signed_immcope();
void test_new_unsigned_immcope();
void test_new_str_immcope();
void test_immcope_tostring_arg();
void test_immcope_tostring_reg();
void test_immcope_tostring_ptr();
void test_immcope_tostring_mem();
void test_immcope_tostring_label();
void test_immcope_tostring_int();
void test_immcope_tostring_signed();
void test_immcope_tostring_unsigned();
void test_immcope_tostring_str();

CU_Suite* add_test_suite_immcope() {
    CU_Suite* suite = CU_add_suite("test_suite_immcope", NULL, NULL);
    CU_ADD_TEST(suite, test_new_arg_immcope);
    CU_ADD_TEST(suite, test_new_reg_immcope);
    CU_ADD_TEST(suite, test_new_ptr_immcope);
    CU_ADD_TEST(suite, test_new_mem_immcope);
    CU_ADD_TEST(suite, test_new_label_immcope);
    CU_ADD_TEST(suite, test_new_int_immcope);
    CU_ADD_TEST(suite, test_new_signed_immcope);
    CU_ADD_TEST(suite, test_new_unsigned_immcope);
    CU_ADD_TEST(suite, test_new_str_immcope);
    CU_ADD_TEST(suite, test_immcope_tostring_arg);
    CU_ADD_TEST(suite, test_immcope_tostring_reg);
    CU_ADD_TEST(suite, test_immcope_tostring_ptr);
    CU_ADD_TEST(suite, test_immcope_tostring_mem);
    CU_ADD_TEST(suite, test_immcope_tostring_label);
    CU_ADD_TEST(suite, test_immcope_tostring_int);
    CU_ADD_TEST(suite, test_immcope_tostring_signed);
    CU_ADD_TEST(suite, test_immcope_tostring_unsigned);
    CU_ADD_TEST(suite, test_immcope_tostring_str);
    return suite;
}

void test_new_arg_immcope() {
    ImmcOpe* ope = new_arg_immcope(IMMC_SUFFIX_WORD, 1);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_ARG);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_WORD);
        CU_ASSERT_EQUAL(ope->reg_id, 1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_reg_immcope() {
    ImmcOpe* ope = new_reg_immcope(IMMC_SUFFIX_LONG, 8);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_REG);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_LONG);
        CU_ASSERT_EQUAL(ope->reg_id, 8);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_ptr_immcope() {
    ImmcOpe* ope = new_ptr_immcope(3);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_PTR);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_QUAD);
        CU_ASSERT_EQUAL(ope->reg_id, 3);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_mem_immcope() {
    ImmcOpe* ope = new_mem_immcope(8);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_MEM);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, 8);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_label_immcope() {
    ImmcOpe* ope = new_label_immcope(new_string("sample_label"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_LABEL);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_STRING_EQUAL(ope->label_name, "sample_label");
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_int_immcope() {
    ImmcOpe* ope = new_int_immcope(IMMC_SUFFIX_BYTE, new_signed_iliteral(INTEGER_INT, 10));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_BYTE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, 10);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_signed_immcope() {
    ImmcOpe* ope = new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 32);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_LONG);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, 32);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_unsigned_immcope() {
    ImmcOpe* ope = new_unsigned_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2147483648U);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_INT);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_LONG);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(ope->iliteral->type, INTEGER_INT);
        CU_ASSERT_TRUE(ope->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ope->iliteral->signed_value, -1LL);
        CU_ASSERT_EQUAL(ope->iliteral->unsigned_value, 2147483648U);
        CU_ASSERT_PTR_NULL(ope->sliteral);
    }

    delete_immcope(ope);
}

void test_new_str_immcope() {
    const char* sliteral_const = "string\0literal";
    const int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    ImmcOpe* ope = new_str_immcope(sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            ImmcOpe* copied_ope = immcope_copy(ope);
            delete_immcope(ope);
            ope = copied_ope;
        }
        CU_ASSERT_EQUAL(ope->type, IMMC_OPERAND_STR);
        CU_ASSERT_EQUAL(ope->suffix, IMMC_SUFFIX_NONE);
        CU_ASSERT_EQUAL(ope->reg_id, -1);
        CU_ASSERT_EQUAL(ope->mem_offset, -1);
        CU_ASSERT_PTR_NULL(ope->label_name);
        CU_ASSERT_EQUAL(memcmp(ope->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_PTR_NULL(ope->iliteral);
        CU_ASSERT_EQUAL(ope->sliteral->size, sliteral_size);
    }

    delete_immcope(ope);
}

void test_immcope_tostring_arg() {
    ImmcOpe* ope = new_arg_immcope(IMMC_SUFFIX_WORD, 1);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "%a1w");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_reg() {
    ImmcOpe* ope = new_reg_immcope(IMMC_SUFFIX_LONG, 8);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "%r8l");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_ptr() {
    ImmcOpe* ope = new_ptr_immcope(3);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "(%r3q)");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_mem() {
    ImmcOpe* ope = new_mem_immcope(8);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "M[8]");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_label() {
    ImmcOpe* ope = new_label_immcope(new_string("sample_label"));
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "sample_label");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_int() {
    ImmcOpe* ope = new_int_immcope(IMMC_SUFFIX_BYTE, new_signed_iliteral(INTEGER_INT, 10));
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "10");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_signed() {
    ImmcOpe* ope = new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 32);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "32");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_unsigned() {
    ImmcOpe* ope = new_unsigned_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2147483648U);
    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "2147483648");

    free(ope_str);
    delete_immcope(ope);
}

void test_immcope_tostring_str() {
    const char* sliteral_const = "string\0literal";
    const int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    ImmcOpe* ope = new_str_immcope(sliteral);

    char* ope_str = immcope_tostring(ope);

    CU_ASSERT_STRING_EQUAL(ope_str, "\"string\\000literal\\000\"");

    free(ope_str);
    delete_immcope(ope);
}
