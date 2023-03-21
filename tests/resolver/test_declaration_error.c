#include "./test_declaration_error.h"
#include "../../src/resolver/declaration.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_resolve_decl_specifiers_error(void);
void test_resolve_declarator_list_error_duplicated(void);
void test_resolve_declarator_error_incomplete(void);
void test_resolve_array_error_functions(void);
void test_resolve_array_error_incomplete(void);
void test_resolve_array_error_size_non_positive(void);
void test_resolve_array_error_size_limit(void);
void test_resolve_struct_error_duplicated(void);
void test_resolve_struct_member_error_duplicated(void);
void test_resolve_struct_member_error_invalid(void);
void test_resolve_enum_error_duplicated(void);
void test_resolve_enum_member_error_duplicated(void);
void test_resolve_enum_error_const_limit(void);
void test_resolve_function_error_array(void);
void test_resolve_function_error_function(void);
void test_resolve_function_error_non_void_incomplete(void);
void test_resolve_param_list_error_duplicated(void);
void test_resolve_param_error_storage_specifier(void);
void test_resolve_param_error_invalid_dtype(void);
void test_resolve_param_error_incomplete(void);
void test_resolve_typedef_error_array(void);
void test_resolve_typedef_error_struct(void);
void test_resolve_typedef_error_function(void);
void test_resolve_init_error_unassignable(void);
void test_resolve_init_error_function(void);
void test_resolve_init_error_typedef_name(void);
void test_resolve_init_error_nested_list_scalar(void);
void test_resolve_init_error_too_long_scalar(void);
void test_resolve_init_error_scalar_array(void);
void test_resolve_init_error_too_long_array(void);
void test_resolve_init_error_too_long_sliteral(void);
void test_resolve_init_error_array_child(void);
void test_resolve_init_error_too_long_nested_array(void);
void test_resolve_init_error_scalar_struct(void);
void test_resolve_init_error_too_long_struct(void);
void test_resolve_init_error_struct_child(void);
void test_resolve_init_error_too_long_nested_struct(void);

void run_local_decl_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected);
void run_global_decl_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected);

CU_Suite* add_test_suite_decl_resolver_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_decl_specifiers_error);
    CU_ADD_TEST(suite, test_resolve_declarator_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_declarator_error_incomplete);
    CU_ADD_TEST(suite, test_resolve_array_error_functions);
    CU_ADD_TEST(suite, test_resolve_array_error_incomplete);
    CU_ADD_TEST(suite, test_resolve_array_error_size_non_positive);
    CU_ADD_TEST(suite, test_resolve_array_error_size_limit);
    CU_ADD_TEST(suite, test_resolve_struct_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_struct_member_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_struct_member_error_invalid);
    CU_ADD_TEST(suite, test_resolve_enum_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_enum_member_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_enum_error_const_limit);
    CU_ADD_TEST(suite, test_resolve_function_error_array);
    CU_ADD_TEST(suite, test_resolve_function_error_function);
    CU_ADD_TEST(suite, test_resolve_function_error_non_void_incomplete);
    CU_ADD_TEST(suite, test_resolve_param_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_param_error_storage_specifier);
    CU_ADD_TEST(suite, test_resolve_param_error_invalid_dtype);
    CU_ADD_TEST(suite, test_resolve_param_error_incomplete);
    CU_ADD_TEST(suite, test_resolve_typedef_error_array);
    CU_ADD_TEST(suite, test_resolve_typedef_error_struct);
    CU_ADD_TEST(suite, test_resolve_typedef_error_function);
    CU_ADD_TEST(suite, test_resolve_init_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_init_error_function);
    CU_ADD_TEST(suite, test_resolve_init_error_typedef_name);
    CU_ADD_TEST(suite, test_resolve_init_error_nested_list_scalar);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_scalar);
    CU_ADD_TEST(suite, test_resolve_init_error_scalar_array);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_array);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_sliteral);
    CU_ADD_TEST(suite, test_resolve_init_error_array_child);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_nested_array);
    CU_ADD_TEST(suite, test_resolve_init_error_scalar_struct);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_struct);
    CU_ADD_TEST(suite, test_resolve_init_error_struct_child);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_nested_struct);
    return suite;
}

void test_resolve_decl_specifiers_error(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 2, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0), new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("combination of type specifiers is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_declarator_list_error_duplicated(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 5,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is already declared\n"));
    vector_push(expected, new_error("identifier 'x' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_declarator_error_incomplete(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_VOID, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' has an incomplete type\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_functions(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_ast(AST_PARAM_LIST, 0)),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of functions is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_incomplete(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of incomplete type is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_size_non_positive(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array size should be a positive integer\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_size_limit(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                new_ast(AST_MUL_EXPR, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("only integer constant is supported as array size\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_struct_error_duplicated(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("member"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")))));

    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct 'Struct' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_struct_member_error_duplicated(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 2,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 3, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")))));

    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct member 'x' is already declared\n"));
    vector_push(expected, new_error("struct member 'y' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_struct_member_error_invalid(void) {
    Ast* local_input = new_ast(
        AST_DECL, 1,                                                   // non-terminal
        new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                        new_ast(AST_STRUCT_DECL_LIST, 2,               // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1,  // non-terminal
                                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                                        new_ast(AST_PARAM_LIST, 0)))),
                                new_ast(AST_STRUCT_DECL, 2,                 // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1,      // non-terminal
                                                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure"))))))));

    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct member 'f' has function type\n"));
    vector_push(expected, new_error("struct member 'structure' has incomplete type\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_enum_error_duplicated(void) {
    Ast* local_input =
        new_ast(AST_DECL, 1,                      // non-terminal
                new_ast(AST_DECL_SPECS, 1,        // non-terminal
                        new_ast(AST_TYPE_ENUM, 2, // non-terminal
                                new_identifier_ast(AST_ENUM_NAME, new_string("Enum")),
                                new_ast(AST_ENUMOR_LIST, 1,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("MEMBER")))))));

    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_denummember);
    vector_push(members, new_denummember(new_string("MEMBER"), 0));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_enum(local_tag_table, new_string("Enum"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("enum 'Enum' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_enum_member_error_duplicated(void) {
    Ast* local_input =
        new_ast(AST_DECL, 1,                      // non-terminal
                new_ast(AST_DECL_SPECS, 1,        // non-terminal
                        new_ast(AST_TYPE_ENUM, 2, // non-terminal
                                new_identifier_ast(AST_ENUM_NAME, new_string("Enum")),
                                new_ast(AST_ENUMOR_LIST, 4,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST"))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST")))))));

    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'SECOND' is already declared\n"));
    vector_push(expected, new_error("identifier 'FIRST' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_enum_error_const_limit(void) {
    Ast* local_input = new_ast(
        AST_DECL, 1, // non-terminal
        new_ast(
            AST_DECL_SPECS, 1,        // non-terminal
            new_ast(AST_TYPE_ENUM, 2, // non-terminal
                    new_identifier_ast(AST_ENUM_NAME, new_string("Enum")),
                    new_ast(AST_ENUMOR_LIST, 1,    // non-terminal
                            new_ast(AST_ENUMOR, 2, // non-terminal
                                    new_identifier_ast(AST_ENUM_CONST, new_string("MEMBER")),
                                    new_ast(AST_ADD_EXPR, 2,
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))))));

    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_denummember);
    vector_push(members, new_denummember(new_string("MEMBER"), 0));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_enum(local_tag_table, new_string("Enum"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("only integer constant is supported as enumeration constant\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_function_error_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,          // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("function")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function returning an array is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_function_error_function(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                 // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,         // non-terminal
                                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("function")),
                                                new_ast(AST_PARAM_LIST, 0)),
                                        new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function returning a function is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_function_error_non_void_incomplete(void) {
    Ast* local_input = new_ast(AST_DECL, 2,                        // non-terminal
                               new_ast(AST_DECL_SPECS, 1,          // non-terminal
                                       new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                               new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                               new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                       new_identifier_ast(AST_IDENT_DECLOR, new_string("function")),
                                                       new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function returning incomplete type other than void is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_list_error_duplicated(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 5,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_PTR_DECLOR, 1,
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function parameter 'x' is already declared\n"));
    vector_push(expected, new_error("function parameter 'y' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_error_storage_specifier(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0), new_ast(AST_STG_TYPEDEF, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("storage specifiers are invalid for a function parameter\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_error_invalid_dtype(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_ARRAY_DECLOR, 2,        // non-terminal
                                                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                                new_ast(AST_PARAM_LIST, 0)),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 5)))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of functions is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_error_incomplete(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_VOID, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 1,                  // non-terminal
                                                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                                                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                                new_identifier_ast(AST_STRUCT_NAME,
                                                                                   new_string("Struct"))))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function parameter 'x' has an incomplete type\n"));
    vector_push(expected, new_error("unnamed function parameter has an incomplete type\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_typedef_error_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array_type")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of incomplete type is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_typedef_error_struct(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_STG_TYPEDEF, 0),
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_VOID, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("member"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("struct_type")))));

    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct member 'member' has incomplete type\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_typedef_error_function(void) {
    Ast* local_input = new_ast(AST_DECL, 2,                        // non-terminal
                               new_ast(AST_DECL_SPECS, 2,          // non-terminal
                                       new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                               new_identifier_ast(AST_STRUCT_NAME, new_string("Struct"))),
                                       new_ast(AST_STG_TYPEDEF, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                               new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                       new_identifier_ast(AST_IDENT_DECLOR, new_string("func_type")),
                                                       new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function returning incomplete type other than void is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_unassignable(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                                               new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                       new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to declared object\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_function(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2,                // non-terminal
                                               new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                                       new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                                       new_ast(AST_PARAM_LIST, 0)),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function cannot be initialized\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_typedef_name(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 2, // non-terminal
                                       new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("tint")),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("typedef-name cannot be initialized\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_nested_list_scalar(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_scalar(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_INIT_LIST, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_scalar_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)),
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)),
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_sliteral(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5, array_size = 3;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer string literal is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_array_child(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                        new_ast(AST_INIT_LIST, 3,                 // non-terminal
                                new_ast(AST_INIT_LIST, 1,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)))),
                                new_ast(AST_INIT_LIST, 1,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)))),
                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_nested_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 4,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_scalar_struct(void) {
    Ast* local_input = new_ast(AST_DECL, 2,                        // non-terminal
                               new_ast(AST_DECL_SPECS, 1,          // non-terminal
                                       new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                               new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)))));
    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_struct(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 2,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))),
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, NULL, expected);
    run_local_decl_resolver_error_test(global_input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_struct_child(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,    // non-terminal
            new_ast(AST_INIT_DECLOR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                    new_ast(AST_INIT_LIST, 3,                 // non-terminal
                            new_ast(AST_INIT_LIST, 1,         // non-terminal
                                    new_ast(AST_INIT_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)))),
                            new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)),
                            new_ast(AST_INIT_LIST, 1,         // non-terminal
                                    new_ast(AST_INIT_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("z"), new_integer_dtype(DTYPE_CHAR)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_nested_struct(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")),
                                new_ast(AST_INIT_LIST, 4,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Vector* child_members = new_vector(&t_dstructmember);
    vector_push(child_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(child_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("first"), new_named_struct_dtype(new_string("Child"), 8, 4)));
    vector_push(members, new_dstructmember(new_string("second"), new_named_struct_dtype(new_string("Child"), 8, 4)));

    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Child"), child_members);
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);
    TagTable* global_tag_table = tagtable_copy(local_tag_table);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, local_tag_table, expected);
    run_global_decl_resolver_error_test(global_input, NULL, global_tag_table, expected);

    delete_vector(expected);
}

void run_local_decl_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected) {
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

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_decl(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    resolver->scope_srt = NULL;
    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}

void run_global_decl_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected) {
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

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_decl(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    resolver->scope_srt = NULL;
    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}
