#include "./test_declaration.h"
#include "../../src/resolver/declaration.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_resolve_char_decl(void);
void test_resolve_int_decl(void);
void test_resolve_pointer_decl(void);
void test_resolve_array_decl(void);
void test_resolve_named_struct_decl(void);
void test_resolve_unnamed_struct_decl(void);
void test_resolve_nameonly_struct_decl(void);
void test_resolve_struct_name_decl(void);
void test_resolve_named_enum_decl(void);
void test_resolve_unnamed_enum_decl(void);
void test_resolve_nameonly_enum_decl(void);
void test_resolve_enum_name_decl(void);
void test_resolve_function_decl(void);
void test_resolve_non_scalar_parameter_decl(void);
void test_resolve_unnamed_parameter_decl(void);
void test_resolve_typedef_decl(void);
void test_resolve_enclosed_decl_array(void);
void test_resolve_enclosed_decl_function(void);
void test_resolve_enclosed_decl_abstract_params(void);
void test_resolve_scalar_init(void);
void test_resolve_scalar_init_enclosed(void);
void test_resolve_sliteral_init(void);
void test_resolve_sliteral_init_border(void);
void test_resolve_sliteral_init_lacked(void);
void test_resolve_sliteral_init_enclosed(void);
void test_resolve_array_list_init_zero(void);
void test_resolve_array_list_init_zero_nested(void);
void test_resolve_array_list_init(void);
void test_resolve_array_list_init_lacked(void);
void test_resolve_array_list_init_nested(void);
void test_resolve_array_list_init_nested_lacked(void);
void test_resolve_array_list_init_flatten(void);
void test_resolve_array_list_init_mix(void);
void test_resolve_struct_list_init_zero(void);
void test_resolve_struct_list_init_zero_nested(void);
void test_resolve_struct_list_init(void);
void test_resolve_struct_list_init_lacked(void);
void test_resolve_struct_list_init_nested(void);
void test_resolve_struct_list_init_nested_lacked(void);
void test_resolve_struct_list_init_flatten(void);
void test_resolve_struct_list_init_mix(void);
void test_resolve_list_init_chararray(void);

void run_local_decl_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                  Srt* expected_scope);
void run_global_decl_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                   Srt* expected_scope);

CU_Suite* add_test_suite_decl_resolver(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_char_decl);
    CU_ADD_TEST(suite, test_resolve_int_decl);
    CU_ADD_TEST(suite, test_resolve_pointer_decl);
    CU_ADD_TEST(suite, test_resolve_array_decl);
    CU_ADD_TEST(suite, test_resolve_named_struct_decl);
    CU_ADD_TEST(suite, test_resolve_unnamed_struct_decl);
    CU_ADD_TEST(suite, test_resolve_nameonly_struct_decl);
    CU_ADD_TEST(suite, test_resolve_struct_name_decl);
    CU_ADD_TEST(suite, test_resolve_named_enum_decl);
    CU_ADD_TEST(suite, test_resolve_unnamed_enum_decl);
    CU_ADD_TEST(suite, test_resolve_nameonly_enum_decl);
    CU_ADD_TEST(suite, test_resolve_enum_name_decl);
    CU_ADD_TEST(suite, test_resolve_function_decl);
    CU_ADD_TEST(suite, test_resolve_non_scalar_parameter_decl);
    CU_ADD_TEST(suite, test_resolve_unnamed_parameter_decl);
    CU_ADD_TEST(suite, test_resolve_typedef_decl);
    CU_ADD_TEST(suite, test_resolve_enclosed_decl_array);
    CU_ADD_TEST(suite, test_resolve_enclosed_decl_function);
    CU_ADD_TEST(suite, test_resolve_enclosed_decl_abstract_params);
    CU_ADD_TEST(suite, test_resolve_scalar_init);
    CU_ADD_TEST(suite, test_resolve_scalar_init_enclosed);
    CU_ADD_TEST(suite, test_resolve_sliteral_init);
    CU_ADD_TEST(suite, test_resolve_sliteral_init_border);
    CU_ADD_TEST(suite, test_resolve_sliteral_init_lacked);
    CU_ADD_TEST(suite, test_resolve_sliteral_init_enclosed);
    CU_ADD_TEST(suite, test_resolve_array_list_init_zero);
    CU_ADD_TEST(suite, test_resolve_array_list_init_zero_nested);
    CU_ADD_TEST(suite, test_resolve_array_list_init);
    CU_ADD_TEST(suite, test_resolve_array_list_init_lacked);
    CU_ADD_TEST(suite, test_resolve_array_list_init_nested);
    CU_ADD_TEST(suite, test_resolve_array_list_init_nested_lacked);
    CU_ADD_TEST(suite, test_resolve_array_list_init_flatten);
    CU_ADD_TEST(suite, test_resolve_array_list_init_mix);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_zero);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_zero_nested);
    CU_ADD_TEST(suite, test_resolve_struct_list_init);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_lacked);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_nested);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_nested_lacked);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_flatten);
    CU_ADD_TEST(suite, test_resolve_struct_list_init_mix);
    CU_ADD_TEST(suite, test_resolve_list_init_chararray);
    return suite;
}

void test_resolve_char_decl(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("c")))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_int_decl(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("i")))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("i"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 2,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("p")))),
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_PTR_DECLOR, 1,         // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("q")))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 2,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p"))),
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT))),
                                           new_string("q"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))),
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,       // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("b"))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 8)))),
                        new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("c")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 3,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 5), new_string("a"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 8),
                                   new_string("b"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 6), 3),
                                   new_string("c"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_named_struct_decl(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                        new_ast(AST_STRUCT_DECL_LIST, 3,               // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_CHAR, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1,   // non-terminal
                                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("s")),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 10))))),
                                new_ast(AST_STRUCT_DECL, 2,                 // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1,      // non-terminal
                                                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("next")))))))),
        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure"))))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_pointer_dtype(new_named_struct_dtype(new_string("Struct"), 32, 8)),
                                   new_string("structure"))));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("s"), new_array_dtype(new_integer_dtype(DTYPE_CHAR), 10)));
    vector_push(members, new_dstructmember(new_string("next"),
                                           new_pointer_dtype(new_named_struct_dtype(new_string("Struct"), 0, 0))));
    DType* struct_dtype = new_unnamed_struct_dtype(members);

    Srt* expected_scope = new_srt(SRT_CMPD_STMT, 1, // non-terminal
                                  new_identifier_srt(SRT_TAG_DECL, struct_dtype, new_string("Struct")));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, expected_scope);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, expected_scope);

    delete_srt(expected);
    delete_srt(expected_scope);
}

void test_resolve_unnamed_struct_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("member"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")))));

    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* struct_dtype = new_unnamed_struct_dtype(members);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, struct_dtype, new_string("structure"))));

    Srt* expected_scope = new_srt(SRT_CMPD_STMT, 0);

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, expected_scope);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, expected_scope);

    delete_srt(expected);
    delete_srt(expected_scope);
}

void test_resolve_nameonly_struct_decl(void) {
    Ast* local_input = new_ast(AST_DECL, 2,                        // non-terminal
                               new_ast(AST_DECL_SPECS, 1,          // non-terminal
                                       new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                               new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")))));

    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 4, 4),
                                                       new_string("structure"))));

    Srt* expected_scope = new_srt(SRT_CMPD_STMT, 0);

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, expected_scope);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, expected_scope);

    delete_srt(expected);
    delete_srt(expected_scope);
}

void test_resolve_struct_name_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 1,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 0);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* struct_dtype = new_unnamed_struct_dtype(members);

    Srt* expected_scope = new_srt(SRT_CMPD_STMT, 1, // non-terminal
                                  new_identifier_srt(SRT_TAG_DECL, struct_dtype, new_string("Struct")));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, expected_scope);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, expected_scope);

    delete_srt(expected);
    delete_srt(expected_scope);
}

void test_resolve_named_enum_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                     // non-terminal
                        new_ast(AST_TYPE_ENUM, 1,              // non-terminal
                                new_ast(AST_ENUMOR_LIST, 2,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("PLUS"))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("MINUS")))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("enumeration"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_unnamed_enum_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                     // non-terminal
                        new_ast(AST_TYPE_ENUM, 1,              // non-terminal
                                new_ast(AST_ENUMOR_LIST, 3,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST"))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND"))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("THIRD")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("enumeration"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_nameonly_enum_decl(void) {
    Ast* local_input = new_ast(AST_DECL, 2,                      // non-terminal
                               new_ast(AST_DECL_SPECS, 1,        // non-terminal
                                       new_ast(AST_TYPE_ENUM, 1, // non-terminal
                                               new_identifier_ast(AST_ENUM_NAME, new_string("Enum")))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("enumeration"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enum_name_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 1,                      // non-terminal
                new_ast(AST_DECL_SPECS, 1,        // non-terminal
                        new_ast(AST_TYPE_ENUM, 2, // non-terminal
                                new_identifier_ast(AST_ENUM_NAME, new_string("Enum")),
                                new_ast(AST_ENUMOR_LIST, 3,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST"))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("THIRD")))))));

    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 0);

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_function_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_VOID, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("h")),
                                        new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("b"))),
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("c"))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* fparams = new_vector(&t_dparam);
    DType* fdtype = new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* gparams = new_vector(&t_dparam);
    vector_push(gparams, new_named_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    DType* gdtype = new_function_dtype(gparams, new_integer_dtype(DTYPE_CHAR));

    Vector* hparams = new_vector(&t_dparam);
    vector_push(hparams, new_named_dparam(new_string("b"), new_integer_dtype(DTYPE_INT)));
    vector_push(hparams, new_named_dparam(new_string("c"), new_integer_dtype(DTYPE_INT)));
    DType* hdtype = new_function_dtype(hparams, new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_srt(SRT_DECL_LIST, 3,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, fdtype, new_string("f"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, gdtype, new_string("g"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, hdtype, new_string("h"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_non_scalar_parameter_decl(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                                new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 5)))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                new_ast(AST_TYPE_INT, 0)),
                                                                        new_identifier_ast(AST_IDENT_DECLOR,
                                                                                           new_string("p")))))))))));
    Ast* global_input = ast_copy(local_input);

    DType* adtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    // array parameter is converted to pointer to initial element

    Vector* fparams = new_vector(&t_dparam);
    vector_push(fparams, new_named_dparam(new_string("p"), new_integer_dtype(DTYPE_INT)));
    DType* fdtype = new_pointer_dtype(new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR))));
    // function parameter is converted to pointer to function

    Vector* func_params = new_vector(&t_dparam);
    vector_push(func_params, new_named_dparam(new_string("a"), adtype));
    vector_push(func_params, new_named_dparam(new_string("f"), fdtype));
    DType* func_dtype = new_function_dtype(func_params, new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, func_dtype, new_string("func"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_unnamed_parameter_decl(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,            // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_VOID, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_CHAR, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("h")),
                                        new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* fparams = new_vector(&t_dparam);
    DType* fdtype = new_function_dtype(fparams, new_void_dtype());

    Vector* gparams = new_vector(&t_dparam);
    vector_push(gparams, new_unnamed_dparam(new_integer_dtype(DTYPE_CHAR)));
    DType* gdtype = new_function_dtype(gparams, new_void_dtype());

    Vector* hparams = new_vector(&t_dparam);
    vector_push(hparams, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(hparams, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* hdtype = new_function_dtype(hparams, new_void_dtype());

    Srt* expected = new_srt(SRT_DECL_LIST, 3,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, fdtype, new_string("f"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, gdtype, new_string("g"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, hdtype, new_string("h"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_typedef_decl(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_identifier_ast(AST_TYPEDEF_NAME, new_string("test_type"))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    SymbolTable* local_table = new_symboltable();
    symboltable_define_label(local_table, new_string("test_type"),
                             new_typedef_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_CHAR))));
    SymbolTable* global_table = symboltable_copy(local_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), new_string("x"))));

    run_local_decl_resolver_test(local_input, local_table, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, global_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enclosed_decl_array(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1, // non-terminal
            new_ast(
                AST_INIT_DECLOR, 1, // non-terminal
                new_ast(
                    AST_ARRAY_DECLOR, 2,                // non-terminal
                    new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                            new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("apfi")),
                                    new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                            new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                    new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))),
                                            new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                    new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))))))),
                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));

    Ast* global_input = ast_copy(local_input);

    Vector* of_params = new_vector(&t_dparam);
    vector_push(of_params, new_named_dparam(new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_INT))));
    vector_push(of_params, new_named_dparam(new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_INT))));

    DType* of_return_dtype = new_integer_dtype(DTYPE_INT);

    DType* of_dtype = new_pointer_dtype(new_function_dtype(of_params, of_return_dtype));
    DType* expected_dtype = new_array_dtype(of_dtype, 3);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, expected_dtype, new_string("apfi"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enclosed_decl_function(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_VOID, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,                         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,                 // non-terminal
                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("signal")),
                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                    new_ast(AST_TYPE_INT, 0)),
                                                            new_ast(AST_ABS_DECLOR, 0))))),
                            new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("sig"))),
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_VOID, 0)),
                                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                    new_ast(AST_TYPE_INT, 0)),
                                                                            new_ast(AST_ABS_DECLOR, 0)))))))))));
    Ast* global_input = ast_copy(local_input);

    DType* sig_dtype = new_integer_dtype(DTYPE_INT);

    Vector* func_params = new_vector(&t_dparam);
    vector_push(func_params, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_pointer_dtype(new_function_dtype(func_params, new_void_dtype()));

    Vector* return_params = new_vector(&t_dparam);
    vector_push(return_params, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* return_dtype = new_pointer_dtype(new_function_dtype(return_params, new_void_dtype()));

    Vector* expected_params = new_vector(&t_dparam);
    vector_push(expected_params, new_named_dparam(new_string("sig"), sig_dtype));
    vector_push(expected_params, new_named_dparam(new_string("func"), func_dtype));
    DType* expected_dtype = new_function_dtype(expected_params, return_dtype);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, expected_dtype, new_string("signal"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enclosed_decl_abstract_params(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_VOID, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,                         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,                 // non-terminal
                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("signal")),
                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                    new_ast(AST_TYPE_INT, 0)),
                                                            new_ast(AST_ABS_DECLOR, 0))))),
                            new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_ast(AST_ABS_DECLOR, 0)),
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_VOID, 0)),
                                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                            new_ast(AST_ABS_DECLOR, 0),
                                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                    new_ast(AST_TYPE_INT, 0)),
                                                                            new_ast(AST_ABS_DECLOR, 0)))))))))));
    Ast* global_input = ast_copy(local_input);

    DType* sig_dtype = new_integer_dtype(DTYPE_INT);

    Vector* func_params = new_vector(&t_dparam);
    vector_push(func_params, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_pointer_dtype(new_function_dtype(func_params, new_void_dtype()));

    Vector* return_params = new_vector(&t_dparam);
    vector_push(return_params, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* return_dtype = new_pointer_dtype(new_function_dtype(return_params, new_void_dtype()));

    Vector* expected_params = new_vector(&t_dparam);
    vector_push(expected_params, new_unnamed_dparam(sig_dtype));
    vector_push(expected_params, new_unnamed_dparam(func_dtype));
    DType* expected_dtype = new_function_dtype(expected_params, return_dtype);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, expected_dtype, new_string("signal"))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_scalar_init(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));
    Ast* global_input = ast_copy(local_input);

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("scalar")),
                new_srt(SRT_INIT, 1,                                                     // non-terminal
                        new_dtyped_srt(SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                       new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("x"))),
                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                        new_signed_iliteral(INTEGER_INT, 2))))));

    run_local_decl_resolver_test(local_input, local_table, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, global_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_scalar_init_enclosed(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 2, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("scalar")),
                                    new_srt(SRT_INIT, 1, // non-terminal
                                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                             new_signed_iliteral(INTEGER_INT, 9)))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_init(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, sliteral_size))),
                        new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                         sliteral_copy(sliteral)))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_init_border(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5, array_size = 4;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* zero_padding_sliteral = sliteral_zero_padding_copy(sliteral, array_size);

    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, array_size))),
                                new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                         zero_padding_sliteral))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_init_lacked(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5, array_size = 10;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* zero_padding_sliteral = sliteral_zero_padding_copy(sliteral, array_size);

    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, array_size))),
                                new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                         zero_padding_sliteral))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_init_enclosed(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, sliteral_size))),
                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                new_sliteral_ast(AST_STRING_EXPR, sliteral)))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                         sliteral_copy(sliteral)))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_zero(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 2), new_string("array")),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_zero_nested(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 3), new_string("array")),
                new_srt(SRT_INIT, 3,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 1))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 4))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_lacked(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 3), new_string("array")),
                new_srt(SRT_INIT, 3,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_nested(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                        new_ast(AST_INIT_LIST, 2,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 2))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 4))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_nested_lacked(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 9))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_flatten(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_array_list_init_mix(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                     // non-terminal
            new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                    new_ast(AST_INIT_LIST, 3, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                            new_ast(AST_INIT_LIST, 1,         // non-terminal
                                    new_ast(AST_INIT_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 2), 3),
                                   new_string("array")),
                new_srt(SRT_INIT, 3,                 // non-terminal
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_zero(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 8, 4),
                                           new_string("structure")),
                        new_srt(SRT_INIT, 2,                                                            // non-terminal
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 0)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_zero_nested(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(child_members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 12, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 12, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1, // non-terminal
        new_srt(
            SRT_INIT_DECL, 2, // non-terminal
            new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 24, 4), new_string("structure")),
            new_srt(SRT_INIT, 2,                                                                    // non-terminal
                    new_srt(SRT_INIT, 3,                                                            // non-terminal
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0)))),
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 0))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0))))),
                    new_srt(SRT_INIT, 3,                                                            // non-terminal
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0)))),
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 0))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0))))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 3,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))),
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))));
    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_unnamed_struct_dtype(members), new_string("structure")),
                        new_srt(SRT_INIT, 3,                                                            // non-terminal
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 1)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 2))),
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 4)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_lacked(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                        new_ast(AST_INIT_LIST, 2, new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));
    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR))));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 16, 8),
                                           new_string("structure")),
                        new_srt(SRT_INIT, 3,                                                            // non-terminal
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 3)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 2))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                                                       1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_nested(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                                                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,                                          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1,                                 // non-terminal
                        new_ast(AST_STRUCT_DECL_LIST, 1,                    // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,                 // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1,      // non-terminal
                                                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                        new_identifier_ast(AST_STRUCT_NAME, new_string("Child")))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("first")),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("second"))))))),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                        new_ast(AST_INIT_LIST, 2,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(child_members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_INT)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 12, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 12, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_unnamed_struct_dtype(members), new_string("structure")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 2)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 4))),
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 5)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6)))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_nested_lacked(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 7)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 10)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(child_members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_INT)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 12, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 12, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1, // non-terminal
        new_srt(
            SRT_INIT_DECL, 2, // non-terminal
            new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 24, 4), new_string("structure")),
            new_srt(SRT_INIT, 2,                 // non-terminal
                    new_srt(SRT_INIT, 3,         // non-terminal
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 7))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 5)))),
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 10)))),
                    new_srt(SRT_INIT, 3,         // non-terminal
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 0))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0)))),
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_flatten(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                                                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,                                          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1,                                 // non-terminal
                        new_ast(AST_STRUCT_DECL_LIST, 1,                    // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,                 // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1,      // non-terminal
                                                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                        new_identifier_ast(AST_STRUCT_NAME, new_string("Child")))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("first")),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("second"))))))),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                        new_ast(AST_INIT_LIST, 2, // non-terminal
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(child_members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_INT)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 12, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 12, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    ;
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_unnamed_struct_dtype(members), new_string("structure")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6))),
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 1)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1,                                                    // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 0)))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_struct_list_init_mix(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,    // non-terminal
            new_ast(AST_INIT_DECLOR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                    new_ast(AST_INIT_LIST, 3, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                            new_ast(AST_INIT_LIST, 1,         // non-terminal
                                    new_ast(AST_INIT_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_CHAR)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 8, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 8, 4)));
    vector_push(members, new_dstructmember(new_string("third"), new_named_struct_dtype(new_string("Child"), 8, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1, // non-terminal
        new_srt(
            SRT_INIT_DECL, 2, // non-terminal
            new_identifier_srt(SRT_DECL, new_named_struct_dtype(new_string("Struct"), 24, 4), new_string("structure")),
            new_srt(SRT_INIT, 3,                 // non-terminal
                    new_srt(SRT_INIT, 2,         // non-terminal
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 1))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 3))))),
                    new_srt(SRT_INIT, 2,         // non-terminal
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 5))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0))))),
                    new_srt(SRT_INIT, 2,         // non-terminal
                            new_srt(SRT_INIT, 1, // non-terminal
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 0))),
                            new_srt(SRT_INIT, 1,                                                    // non-terminal
                                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                    new_signed_iliteral(INTEGER_INT, 0))))))));

    run_local_decl_resolver_test(local_input, NULL, local_tag_table, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, global_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_list_init_chararray(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 72))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 3), new_string("array")),
                new_srt(SRT_INIT, 3,                                                            // non-terminal
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 72)))),
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, NULL, expected, NULL);
    run_global_decl_resolver_test(global_input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void run_local_decl_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                  Srt* expected_scope) {
    Resolver* resolver = new_resolver(input);
    if (symbol_table != NULL) {
        symbol_table->outer_scope = resolver->symbol_table;
        resolver->symbol_table = symbol_table;
    } else {
        resolver->symbol_table = symboltable_enter_scope(resolver->symbol_table);
    }
    if (tag_table != NULL) {
        tag_table->outer_scope = resolver->tag_table;
        resolver->tag_table = tag_table;
    } else {
        resolver->tag_table = tagtable_enter_scope(resolver->tag_table);
    }
    resolver->scope_srt = new_srt(SRT_CMPD_STMT, 0);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 1, resolver->scope_srt);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_decl(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);
    if (expected_scope != NULL) testlib_assert_srt_equal(resolver->scope_srt, expected_scope);

    resolver->scope_srt = NULL;
    if (actual != NULL) delete_srt(actual);
    delete_resolver(resolver);
}

void run_global_decl_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                   Srt* expected_scope) {
    Resolver* resolver = new_resolver(input);
    if (symbol_table != NULL) {
        delete_symboltable(resolver->symbol_table);
        resolver->symbol_table = symbol_table;
    }
    if (tag_table != NULL) {
        delete_tagtable(resolver->tag_table);
        resolver->tag_table = tag_table;
    }
    resolver->scope_srt = new_srt(SRT_CMPD_STMT, 0);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 1, resolver->scope_srt);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_decl(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);
    if (expected_scope != NULL) testlib_assert_srt_equal(resolver->scope_srt, expected_scope);

    resolver->scope_srt = NULL;
    if (actual != NULL) delete_srt(actual);
    delete_resolver(resolver);
}
