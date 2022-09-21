#include "./test_immc.h"
#include "../../src/immc/immc.h"

#include <stdlib.h>

void test_new_inst_immc_1arg();
void test_new_inst_immc_2arg();
void test_new_inst_immc_3arg();
void test_new_int_data_immc(void);
void test_new_str_data_immc(void);
void test_new_label_immc(void);
void test_immc_isjumpinst(void);
void test_immc_tostring_1arg_inst();
void test_immc_tostring_2arg_inst();
void test_immc_tostring_3arg_inst();
void test_immc_tostring_int_data(void);
void test_immc_tostring_str_data(void);
void test_immc_tostring_label(void);

CU_Suite* add_test_suite_immc(void) {
    CU_Suite* suite = CU_add_suite("test_suite_immc", NULL, NULL);
    CU_ADD_TEST(suite, test_new_inst_immc_1arg);
    CU_ADD_TEST(suite, test_new_inst_immc_2arg);
    CU_ADD_TEST(suite, test_new_inst_immc_3arg);
    CU_ADD_TEST(suite, test_new_int_data_immc);
    CU_ADD_TEST(suite, test_new_str_data_immc);
    CU_ADD_TEST(suite, test_new_label_immc);
    CU_ADD_TEST(suite, test_immc_isjumpinst);
    CU_ADD_TEST(suite, test_immc_tostring_1arg_inst);
    CU_ADD_TEST(suite, test_immc_tostring_2arg_inst);
    CU_ADD_TEST(suite, test_immc_tostring_3arg_inst);
    CU_ADD_TEST(suite, test_immc_tostring_int_data);
    CU_ADD_TEST(suite, test_immc_tostring_str_data);
    CU_ADD_TEST(suite, test_immc_tostring_label);
    return suite;
}

void test_new_inst_immc_1arg() {
    ImmcOpe* arg = NULL;
    Immc* immc = NULL;

    arg = new_label_immcope(new_string("label"));
    immc = new_inst_immc(IMMC_INST_JMP, arg, NULL, NULL);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            arg = copied_immc->inst->dst;
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_INST);
        CU_ASSERT_EQUAL(immc->inst->type, IMMC_INST_JMP);
        CU_ASSERT_PTR_EQUAL(immc->inst->dst, arg);
        CU_ASSERT_PTR_NULL(immc->inst->fst_src);
        CU_ASSERT_PTR_NULL(immc->inst->snd_src);
        CU_ASSERT_PTR_NULL(immc->data);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);

    arg = new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 20);
    immc = new_inst_immc(IMMC_INST_ENTER, NULL, arg, NULL);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            arg = copied_immc->inst->fst_src;
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_INST);
        CU_ASSERT_EQUAL(immc->inst->type, IMMC_INST_ENTER);
        CU_ASSERT_PTR_NULL(immc->inst->dst);
        CU_ASSERT_PTR_EQUAL(immc->inst->fst_src, arg);
        CU_ASSERT_PTR_NULL(immc->inst->snd_src);
        CU_ASSERT_PTR_NULL(immc->data);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);
}

void test_new_inst_immc_2arg() {
    ImmcOpe* dst = new_reg_immcope(IMMC_SUFFIX_LONG, 1);
    ImmcOpe* src = new_reg_immcope(IMMC_SUFFIX_BYTE, 0);
    Immc* immc = new_inst_immc(IMMC_INST_LOAD, dst, src, NULL);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            dst = copied_immc->inst->dst;
            src = copied_immc->inst->fst_src;
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_INST);
        CU_ASSERT_EQUAL(immc->inst->type, IMMC_INST_LOAD);
        CU_ASSERT_PTR_EQUAL(immc->inst->dst, dst);
        CU_ASSERT_PTR_EQUAL(immc->inst->fst_src, src);
        CU_ASSERT_PTR_NULL(immc->inst->snd_src);
        CU_ASSERT_PTR_NULL(immc->data);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);
}

void test_new_inst_immc_3arg() {
    ImmcOpe* dst = new_reg_immcope(IMMC_SUFFIX_QUAD, 1);
    ImmcOpe* fst_src = new_reg_immcope(IMMC_SUFFIX_QUAD, 0);
    ImmcOpe* snd_src = new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8);
    Immc* immc = new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            dst = copied_immc->inst->dst;
            fst_src = copied_immc->inst->fst_src;
            snd_src = copied_immc->inst->snd_src;
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_INST);
        CU_ASSERT_EQUAL(immc->inst->type, IMMC_INST_ADD);
        CU_ASSERT_PTR_EQUAL(immc->inst->dst, dst);
        CU_ASSERT_PTR_EQUAL(immc->inst->fst_src, fst_src);
        CU_ASSERT_PTR_EQUAL(immc->inst->snd_src, snd_src);
        CU_ASSERT_PTR_NULL(immc->data);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);
}

void test_new_int_data_immc(void) {
    Immc* immc = new_int_data_immc(IMMC_DATA_WORD, new_signed_iliteral(INTEGER_INT, 6));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_DATA);
        CU_ASSERT_PTR_NULL(immc->inst);
        CU_ASSERT_EQUAL(immc->data->type, IMMC_DATA_WORD);
        CU_ASSERT_EQUAL(immc->data->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(immc->data->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(immc->data->iliteral->signed_value, 6);
        CU_ASSERT_EQUAL(immc->data->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(immc->data->sliteral);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);
}

void test_new_str_data_immc(void) {
    char sliteral_const[15] = "string\0literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    Immc* immc = new_str_data_immc(IMMC_DATA_STR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_DATA);
        CU_ASSERT_PTR_NULL(immc->inst);
        CU_ASSERT_EQUAL(immc->data->type, IMMC_DATA_STR);
        CU_ASSERT_PTR_NULL(immc->data->iliteral);
        CU_ASSERT_EQUAL(memcmp(immc->data->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(immc->data->sliteral->size, sliteral_size);
        CU_ASSERT_PTR_NULL(immc->label);
    }

    delete_immc(immc);
}

void test_new_label_immc(void) {
    Immc* immc = new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("function"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Immc* copied_immc = immc_copy(immc);
            delete_immc(immc);
            immc = copied_immc;
        }
        CU_ASSERT_EQUAL(immc->type, IMMC_LABEL);
        CU_ASSERT_PTR_NULL(immc->inst);
        CU_ASSERT_PTR_NULL(immc->data);
        CU_ASSERT_EQUAL(immc->label->type, IMMC_LABEL_FUNCTION);
        CU_ASSERT_EQUAL(immc->label->visibility, IMMC_VIS_GLOBAL);
        CU_ASSERT_STRING_EQUAL(immc->label->name, "function");
    }

    delete_immc(immc);
}

void test_immc_isjumpinst(void) {
    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;
    Immc* immc = NULL;

    dst = new_label_immcope(new_string("label"));
    immc = new_inst_immc(IMMC_INST_JMP, dst, NULL, NULL);
    CU_ASSERT_TRUE(immc_isjumpinst(immc));
    delete_immc(immc);

    dst = new_label_immcope(new_string("label"));
    fst_src = new_reg_immcope(IMMC_SUFFIX_LONG, 2);
    snd_src = new_reg_immcope(IMMC_SUFFIX_QUAD, 3);
    immc = new_inst_immc(IMMC_INST_JEQ, dst, fst_src, snd_src);
    CU_ASSERT_TRUE(immc_isjumpinst(immc));
    delete_immc(immc);

    dst = new_label_immcope(new_string("label"));
    fst_src = new_reg_immcope(IMMC_SUFFIX_BYTE, 4);
    snd_src = new_signed_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 3);
    immc = new_inst_immc(IMMC_INST_JNEQ, dst, fst_src, snd_src);
    CU_ASSERT_TRUE(immc_isjumpinst(immc));
    delete_immc(immc);

    dst = new_reg_immcope(IMMC_SUFFIX_LONG, 1);
    fst_src = new_label_immcope(new_string("callee"));
    snd_src = new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1);
    immc = new_inst_immc(IMMC_INST_CALL, dst, fst_src, snd_src);
    CU_ASSERT_FALSE(immc_isjumpinst(immc));
    delete_immc(immc);

    dst = new_reg_immcope(IMMC_SUFFIX_BYTE, 1);
    fst_src = new_reg_immcope(IMMC_SUFFIX_LONG, 2);
    snd_src = new_reg_immcope(IMMC_SUFFIX_QUAD, 3);
    immc = new_inst_immc(IMMC_INST_SETEQ, dst, fst_src, snd_src);
    CU_ASSERT_FALSE(immc_isjumpinst(immc));
    delete_immc(immc);

    dst = new_reg_immcope(IMMC_SUFFIX_BYTE, 5);
    fst_src = new_reg_immcope(IMMC_SUFFIX_BYTE, 4);
    snd_src = new_signed_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 3);
    immc = new_inst_immc(IMMC_INST_SETNEQ, dst, fst_src, snd_src);
    CU_ASSERT_FALSE(immc_isjumpinst(immc));
    delete_immc(immc);
}

void test_immc_tostring_1arg_inst() {
    ImmcOpe* arg = NULL;
    Immc* immc = NULL;
    char* immc_str = NULL;

    arg = new_label_immcope(new_string("label"));
    immc = new_inst_immc(IMMC_INST_JMP, arg, NULL, NULL);
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tjmp\tlabel\n");
    delete_immc(immc);
    free(immc_str);

    arg = new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 20);
    immc = new_inst_immc(IMMC_INST_ENTER, NULL, arg, NULL);
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tenter\t20\n");
    delete_immc(immc);
    free(immc_str);
}

void test_immc_tostring_2arg_inst() {
    ImmcOpe* dst = new_reg_immcope(IMMC_SUFFIX_LONG, 1);
    ImmcOpe* src = new_reg_immcope(IMMC_SUFFIX_BYTE, 0);
    Immc* immc = new_inst_immc(IMMC_INST_LOAD, dst, src, NULL);
    char* immc_str = immc_tostring(immc);

    CU_ASSERT_STRING_EQUAL(immc_str, "\tload\t%r1l, %r0b\n");

    delete_immc(immc);
    free(immc_str);
}

void test_immc_tostring_3arg_inst() {
    ImmcOpe* dst = new_reg_immcope(IMMC_SUFFIX_QUAD, 1);
    ImmcOpe* fst_src = new_reg_immcope(IMMC_SUFFIX_QUAD, 0);
    ImmcOpe* snd_src = new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8);
    Immc* immc = new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src);
    char* immc_str = immc_tostring(immc);

    CU_ASSERT_STRING_EQUAL(immc_str, "\tadd\t%r1q, %r0q, 8\n");

    delete_immc(immc);
    free(immc_str);
}

void test_immc_tostring_int_data(void) {
    Immc* immc = NULL;
    char* immc_str = NULL;

    immc = new_int_data_immc(IMMC_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 8));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tbyte\t8\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_int_data_immc(IMMC_DATA_WORD, new_signed_iliteral(INTEGER_INT, 6));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tword\t6\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 19));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tlong\t19\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_int_data_immc(IMMC_DATA_QUAD, new_signed_iliteral(INTEGER_INT, 551));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tquad\t551\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 10));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tzero\t10\n");
    delete_immc(immc);
    free(immc_str);
}

void test_immc_tostring_str_data(void) {
    char sliteral_const[15] = "string\0literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    Immc* immc = new_str_data_immc(IMMC_DATA_STR, sliteral);
    char* immc_str = immc_tostring(immc);

    CU_ASSERT_STRING_EQUAL(immc_str, "\tstring\t\"string\\000literal\\000\"\n");

    delete_immc(immc);
    free(immc_str);
}

void test_immc_tostring_label(void) {
    Immc* immc = NULL;
    char* immc_str = NULL;

    immc = new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("normal_none_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "normal_none_label:\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_NONE, new_string("function_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "function function_label:\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_NONE, new_string("variable_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "variable variable_label:\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_GLOBAL, new_string("global_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tglobal\n"
                                     "global_label:\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_LOCAL, new_string("local_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tlocal\n"
                                     "local_label:\n");
    delete_immc(immc);
    free(immc_str);

    immc = new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("global_function_label"));
    immc_str = immc_tostring(immc);
    CU_ASSERT_STRING_EQUAL(immc_str, "\tglobal\n"
                                     "function global_function_label:\n");
    delete_immc(immc);
    free(immc_str);
}
