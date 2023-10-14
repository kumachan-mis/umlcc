#include "./test_declaration.h"
#include "../../src/immcgen/declaration.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_immcgen_local_scalar_decl(void);
void test_immcgen_global_scalar_decl(void);
void test_immcgen_local_array_decl(void);
void test_immcgen_global_array_decl(void);
void test_immcgen_strdecl(void);
void test_immcgen_local_struct_decl(void);
void test_immcgen_global_struct_decl(void);
void test_immcgen_local_function_decl(void);
void test_immcgen_global_function_decl(void);
void test_immcgen_local_struct_tag_decl(void);
void test_immcgen_global_struct_tag_decl(void);
void test_immcgen_local_scalar_init(void);
void test_immcgen_global_scalar_init(void);
void test_immcgen_local_sliteral_init(void);
void test_immcgen_global_sliteral_init(void);
void test_immcgen_local_array_init(void);
void test_immcgen_global_array_init(void);
void test_immcgen_local_struct_init(void);
void test_immcgen_global_struct_init(void);

void run_local_decl_immcgen_test(Srt* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected);
void run_global_decl_immcgen_test(Srt* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected);

CU_Suite* add_test_suite_decl_immcgen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_local_scalar_decl);
    CU_ADD_TEST(suite, test_immcgen_global_scalar_decl);
    CU_ADD_TEST(suite, test_immcgen_local_array_decl);
    CU_ADD_TEST(suite, test_immcgen_global_array_decl);
    CU_ADD_TEST(suite, test_immcgen_strdecl);
    CU_ADD_TEST(suite, test_immcgen_local_struct_decl);
    CU_ADD_TEST(suite, test_immcgen_global_struct_decl);
    CU_ADD_TEST(suite, test_immcgen_local_function_decl);
    CU_ADD_TEST(suite, test_immcgen_global_function_decl);
    CU_ADD_TEST(suite, test_immcgen_local_struct_tag_decl);
    CU_ADD_TEST(suite, test_immcgen_global_struct_tag_decl);
    CU_ADD_TEST(suite, test_immcgen_local_scalar_init);
    CU_ADD_TEST(suite, test_immcgen_global_scalar_init);
    CU_ADD_TEST(suite, test_immcgen_local_sliteral_init);
    CU_ADD_TEST(suite, test_immcgen_global_sliteral_init);
    CU_ADD_TEST(suite, test_immcgen_local_array_init);
    CU_ADD_TEST(suite, test_immcgen_global_array_init);
    CU_ADD_TEST(suite, test_immcgen_local_struct_init);
    CU_ADD_TEST(suite, test_immcgen_global_struct_init);
    return suite;
}

void test_immcgen_local_scalar_decl(void) {
    Srt* input = new_srt(SRT_DECL_LIST, 2,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c"))),
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_INT), new_string("i"))));

    Vector* expected = new_vector(&t_immc);

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_scalar_decl(void) {
    Srt* input = new_srt(SRT_DECL_LIST, 2,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c"))),
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_INT), new_string("i"))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("c")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("i")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 4)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_array_decl(void) {
    Srt* input = new_srt(
        SRT_DECL_LIST, 2,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 5), new_string("a"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("b"))));

    Vector* expected = new_vector(&t_immc);

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_array_decl(void) {
    Srt* input = new_srt(
        SRT_DECL_LIST, 2,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 5), new_string("a"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("b"))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("a")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 20)));
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("b")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 24)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_strdecl(void) {
    char sliteral_const[15] = "string literal";
    int sliteral_size = 15;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Srt* input =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_string_srt(SRT_STR_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), 0),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_sliteral_srt(new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), sliteral))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string(".SL0")));
    vector_push(expected, new_str_data_immc(IMMC_DATA_STR, sliteral_copy(sliteral)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_struct_decl(void) {
    Srt* input = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_named_struct_dtype(new_string("Struct"), 12, 4),
                                                    new_string("structure"))));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    TagTable* tag_table = new_tagtable();
    tagtable_define_struct(tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_immc);

    run_local_decl_immcgen_test(input, NULL, tag_table, expected);

    delete_vector(expected);
}

void test_immcgen_global_struct_decl(void) {
    Srt* input = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, new_named_struct_dtype(new_string("Struct"), 12, 4),
                                                    new_string("structure"))));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    TagTable* tag_table = new_tagtable();
    tagtable_define_struct(tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("structure")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 12)));

    run_global_decl_immcgen_test(input, NULL, tag_table, expected);

    delete_vector(expected);
}

void test_immcgen_local_function_decl(void) {
    Vector* fparams = new_vector(&t_dparam);
    DType* fdtype = new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* gparams = new_vector(&t_dparam);
    vector_push(gparams, new_named_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    DType* gdtype = new_function_dtype(gparams, new_integer_dtype(DTYPE_CHAR));

    Srt* input = new_srt(SRT_DECL_LIST, 2,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, fdtype, new_string("f"))),
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, gdtype, new_string("g"))));

    Vector* expected = new_vector(&t_immc);

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_function_decl(void) {
    Vector* fparams = new_vector(&t_dparam);
    DType* fdtype = new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* gparams = new_vector(&t_dparam);
    vector_push(gparams, new_named_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    DType* gdtype = new_function_dtype(gparams, new_integer_dtype(DTYPE_CHAR));

    Srt* input = new_srt(SRT_DECL_LIST, 2,         // non-terminal
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, fdtype, new_string("f"))),
                         new_srt(SRT_INIT_DECL, 1, // non-terminal
                                 new_identifier_srt(SRT_IDENT_DECL, gdtype, new_string("g"))));

    Vector* expected = new_vector(&t_immc);

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_struct_tag_decl(void) {
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));

    DType* struct_dtype = new_unnamed_struct_dtype(members);

    Srt* input = new_identifier_srt(SRT_TAG_DECL, struct_dtype, new_string("Struct"));

    Vector* expected = new_vector(&t_immc);

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_struct_tag_decl(void) {
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));

    DType* struct_dtype = new_unnamed_struct_dtype(members);

    Srt* input = new_identifier_srt(SRT_TAG_DECL, struct_dtype, new_string("Struct"));

    Vector* expected = new_vector(&t_immc);

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_scalar_init(void) {
    Srt* input = new_srt(
        SRT_DECL_LIST, 4,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_dtyped_srt(
                            SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 89))))),
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_INT), new_string("i")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_dtyped_srt(
                            SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                            new_dtyped_srt(
                                SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                1, // non-terminal
                                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2))))),
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_INT), new_string("j")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_dtyped_srt(
                            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("y"))))),
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("ul")),
                new_srt(SRT_INIT, 1,                                                             // non-terminal
                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                                       new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                        new_signed_iliteral(INTEGER_INT, 2147483647))))));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(symbol_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                           // inst
                              new_mem_immcope(12),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 89), // fst_src
                              NULL));                                                    // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(16),                         // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(20),                         // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(
        expected,
        new_inst_immc(IMMC_INST_STORE,                                                                 // inst
                      new_mem_immcope(28),                                                             // dst
                      new_unsigned_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_UNSIGNED_LONG, 2147483647ul), // fst_src
                      NULL));                                                                          // snd_src

    run_local_decl_immcgen_test(input, symbol_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_scalar_init(void) {
    Srt* input =
        new_srt(SRT_DECL_LIST, 3,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c")),
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 89))))),
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_INT), new_string("i")),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)))),
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("ul")),
                        new_srt(SRT_INIT, 1,                                                             // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 2147483647))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("c")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 89)));
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("i")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("ul")));
    vector_push(expected,
                new_int_data_immc(IMMC_DATA_QUAD, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 2147483647ul)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_sliteral_init(void) {
    char sliteral_const[6] = "local";
    int sliteral_size = 6;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Srt* input = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("s")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size), sliteral))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STR,                            // inst
                              new_mem_immcope(8),                       // dst
                              new_str_immcope(sliteral_copy(sliteral)), // fst_src
                              NULL));                                   // snd_src

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_sliteral_init(void) {
    char sliteral_const[7] = "global";
    int sliteral_size = 7;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Srt* input = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("s")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size), sliteral))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("s")));
    vector_push(expected, new_str_data_immc(IMMC_DATA_STR, sliteral_copy(sliteral)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_array_init(void) {
    Srt* input = new_srt(
        SRT_DECL_LIST, 2,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 2), new_string("a")),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2))))),
        new_srt(
            SRT_INIT_DECL, 2, // non-terminal
            new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 2), 3),
                               new_string("b")),
            new_srt(
                SRT_INIT, 3,                 // non-terminal
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 6))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 7)))),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 8))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))),
                new_srt(
                    SRT_INIT, 2,         // non-terminal
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(8),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(4),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(32),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 6), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(28),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 7), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(24),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(20),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(16),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(12),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    run_local_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_global_array_init(void) {
    Srt* input = new_srt(
        SRT_DECL_LIST, 2,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 2), new_string("a")),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2))))),
        new_srt(
            SRT_INIT_DECL, 2, // non-terminal
            new_identifier_srt(SRT_IDENT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 2), 3),
                               new_string("b")),
            new_srt(
                SRT_INIT, 3,                 // non-terminal
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 6))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 7)))),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 8))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))),
                new_srt(
                    SRT_INIT, 2,         // non-terminal
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("a")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("b")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));

    run_global_decl_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_local_struct_init(void) {
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    TagTable* tag_table = new_tagtable();
    tagtable_define_struct(tag_table, new_string("Struct"), members);

    Srt* input = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_named_struct_dtype(new_string("Struct"), 12, 4),
                                   new_string("structure")),
                new_srt(SRT_INIT, 3,                                                            // non-terminal
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR),
                                               1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))))));
    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(12),                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(8),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(4),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    run_local_decl_immcgen_test(input, NULL, tag_table, expected);

    delete_vector(expected);
}

void test_immcgen_global_struct_init(void) {
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    TagTable* tag_table = new_tagtable();
    tagtable_define_struct(tag_table, new_string("Struct"), members);

    Srt* input = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_IDENT_DECL, new_named_struct_dtype(new_string("Struct"), 12, 4),
                                   new_string("structure")),
                new_srt(SRT_INIT, 3,                                                            // non-terminal
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR),
                                               1, // non-terminal
                                               new_iliteral_srt(new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("structure")));
    vector_push(expected, new_int_data_immc(IMMC_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 3)));

    run_global_decl_immcgen_test(input, NULL, tag_table, expected);

    delete_vector(expected);
}

void run_local_decl_immcgen_test(Srt* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected) {
    Immcgen* immcgen = new_immcgen(input);
    if (symbol_table != NULL) {
        symbol_table->outer_scope = immcgen->symbol_table;
        immcgen->symbol_table = symbol_table;
    } else {
        immcgen->symbol_table = symboltable_enter_scope(immcgen->symbol_table);
    }
    if (tag_table != NULL) {
        tag_table->outer_scope = immcgen->tag_table;
        immcgen->tag_table = tag_table;
    } else {
        immcgen->tag_table = tagtable_enter_scope(immcgen->tag_table);
    }

    Vector* actual = immcgen_generate_immcode(immcgen);

    testlib_assert_immcs_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_immcgen(immcgen);
}

void run_global_decl_immcgen_test(Srt* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected) {
    Immcgen* immcgen = new_immcgen(input);
    if (symbol_table != NULL) {
        delete_symboltable(immcgen->symbol_table);
        immcgen->symbol_table = symbol_table;
    }
    if (tag_table != NULL) {
        delete_tagtable(immcgen->tag_table);
        immcgen->tag_table = tag_table;
    }

    Vector* actual = immcgen_generate_immcode(immcgen);

    testlib_assert_immcs_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_immcgen(immcgen);
}
