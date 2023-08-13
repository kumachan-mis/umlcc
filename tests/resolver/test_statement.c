#include "./test_statement.h"
#include "../../src/resolver/statement.h"
#include "../testlib/testlib.h"

void test_resolve_compound_stmt_completed_typedef(void);
void test_resolve_compound_stmt_incompleted_typedef(void);
void test_resolve_compound_stmt_int(void);
void test_resolve_compound_stmt_unsigned_int(void);
void test_resolve_compound_stmt_long(void);
void test_resolve_compound_stmt_unsigned_long(void);
void test_resolve_compound_stmt_long_long(void);
void test_resolve_compound_stmt_unsigned_long_long(void);
void test_resolve_compound_stmt_void_pointer(void);
void test_resolve_compound_stmt_empty(void);
void test_resolve_return_stmt_without_cast(void);
void test_resolve_return_stmt_with_cast(void);
void test_resolve_return_stmt_without_value(void);
void test_resolve_expression_stmt(void);

void run_stmt_resolver_test(Ast* input, SymbolTable* local_table, DType* return_dtype, Srt* expected);

CU_Suite* add_test_suite_stmt_resolver(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_completed_typedef);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_incompleted_typedef);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_int);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_unsigned_int);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_long);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_unsigned_long);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_long_long);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_unsigned_long_long);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_void_pointer);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_empty);
    CU_ADD_TEST(suite, test_resolve_return_stmt_without_cast);
    CU_ADD_TEST(suite, test_resolve_return_stmt_with_cast);
    CU_ADD_TEST(suite, test_resolve_return_stmt_without_value);
    CU_ADD_TEST(suite, test_resolve_expression_stmt);
    return suite;
}

void test_resolve_compound_stmt_completed_typedef(void) {
    Ast* input =
        new_ast(AST_CMPD_STMT, 3,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("pint")))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_identifier_ast(AST_TYPEDEF_NAME, new_string("pint"))),
                        new_ast(AST_INIT_DECLOR_LIST, 2,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))),
                                new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("q"))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("p"))),
                                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("q"))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 7))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 3,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_typedef_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT))),
                                           new_string("pint")))),
        new_srt(
            SRT_DECL_LIST, 2,         // non-terminal
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p"))),
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("q")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      new_string("p")))),
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                                  new_identifier_srt(SRT_IDENT_EXPR,
                                                                     new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                                     new_string("q")))),
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                     new_signed_iliteral(INTEGER_INT, 7))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_incompleted_typedef(void) {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("malloc"))),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0))))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_STG_TYPEDEF, 0),
                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("Struct"))))),
        new_ast(AST_DECL, 1,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                                new_ast(AST_STRUCT_DECL_LIST, 2,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_TYPEDEF_NAME, new_string("Struct"))),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR,
                                                                                   new_string("next"))))))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_identifier_ast(AST_TYPEDEF_NAME, new_string("Struct"))),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("s"))),
                                new_ast(AST_CALL_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("malloc")),
                                        new_ast(AST_ARG_LIST, 1,                               // non-terminal
                                                new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                                                        new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                                        new_identifier_ast(AST_TYPEDEF_NAME,
                                                                                           new_string("Struct"))),
                                                                new_ast(AST_ABS_DECLOR, 0)))))))));
    Vector* malloc_params = new_vector(&t_dparam);
    vector_push(malloc_params, new_unnamed_dparam(new_integer_dtype(DTYPE_INT)));
    DType* malloc_dtype = new_function_dtype(malloc_params, new_pointer_dtype(new_void_dtype()));

    DType* incomplete_unnamed_struct_dtype = new_named_struct_dtype(new_string("Struct"), 0, 0);

    Vector* struct_members = new_vector(&t_dstructmember);
    vector_push(struct_members, new_dstructmember(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(struct_members, new_dstructmember(new_string("y"), new_integer_dtype(DTYPE_INT)));
    vector_push(
        struct_members,
        new_dstructmember(new_string("next"), new_pointer_dtype(new_named_struct_dtype(new_string("Struct"), 0, 0))));
    DType* unnamed_struct_dtype = new_unnamed_struct_dtype(struct_members);

    DType* complete_unnamed_struct_dtype = new_named_struct_dtype(new_string("Struct"), 16, 8);

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 5,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, malloc_dtype, new_string("malloc")))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_typedef_dtype(incomplete_unnamed_struct_dtype),
                                           new_string("Struct")))),
        new_identifier_srt(SRT_TAG_DECL, unnamed_struct_dtype, new_string("Struct")), new_srt(SRT_DECL_LIST, 0),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(complete_unnamed_struct_dtype), new_string("s")),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_dtyped_srt(
                                    SRT_CAST_EXPR, new_pointer_dtype(dtype_copy(complete_unnamed_struct_dtype)),
                                    1, // non-terminal
                                    new_dtyped_srt(
                                        SRT_CALL_EXPR, new_pointer_dtype(new_void_dtype()), 2, // non-terminal
                                        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(malloc_dtype)),
                                                       1, // non-terminal
                                                       new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(malloc_dtype),
                                                                          new_string("malloc"))),
                                        new_srt(SRT_ARG_LIST, 1, // non-terminal
                                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                 new_signed_iliteral(INTEGER_INT, 16)))))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_int(void) {
    Ast* input = new_ast(AST_CMPD_STMT, 3,                  // non-terminal
                         new_ast(AST_DECL, 2,               // non-terminal
                                 new_ast(AST_DECL_SPECS, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_INIT_DECLOR_LIST, 3,    // non-terminal
                                         new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                         new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                                         new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                                 new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                         new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                         new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                                 new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                         new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                         new_ast(AST_SUB_EXPR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))),
                         new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                 new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_ast(AST_MUL_EXPR, 2, // non-terminal
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 3, // non-terminal
        new_srt(
            SRT_DECL_LIST, 3,         // non-terminal
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("x")),
                    new_srt(SRT_INIT, 1,
                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                             new_signed_iliteral(INTEGER_INT, 3)))),
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("y")),
                    new_srt(SRT_INIT, 1,
                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                             new_signed_iliteral(INTEGER_INT, 1)))),
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                                  new_identifier_srt(SRT_IDENT_EXPR,
                                                                     new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                                     new_string("z")))),
                    new_dtyped_srt(SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
                new_dtyped_srt(
                    SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_unsigned_int(void) {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4u))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_UNSIGNED, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2u))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_DIV_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4u))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2u))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)),
                                           new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2, // non-terminal
                    new_dtyped_srt(
                        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)), 1, // non-terminal
                        new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 1,    // non-terminal
                                       new_identifier_srt(SRT_IDENT_EXPR,
                                                          new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)),
                                                          new_string("z")))),
                    new_dtyped_srt(
                        SRT_DIV_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("x")),
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("y"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_long(void) {
    Ast* input =
        new_ast(AST_CMPD_STMT, 4,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONG, 5l))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONG, 3l))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                new_ast(AST_ADD_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONG),
                                                 new_signed_iliteral(INTEGER_LONG, 5l))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONG),
                                                 new_signed_iliteral(INTEGER_LONG, 3l))))),
        new_srt(
            SRT_DECL_LIST, 1,         // non-terminal
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)), new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_LONG), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)),
                                                      new_string("z")))),
                new_dtyped_srt(SRT_ADD_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("x")),
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("y"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_unsigned_long(void) {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 6ul))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 3, // non-terminal
                        new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4ul))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 3, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 6ul))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4ul))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)),
                                           new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
                    new_dtyped_srt(
                        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)), 1, // non-terminal
                        new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1,    // non-terminal
                                       new_identifier_srt(SRT_IDENT_EXPR,
                                                          new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)),
                                                          new_string("z")))),
                    new_dtyped_srt(
                        SRT_SUB_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("x")),
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("y"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_long_long(void) {
    Ast* input =
        new_ast(AST_CMPD_STMT, 4,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONGLONG, 7ll))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 3, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONGLONG, 5ll))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 3, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                new_ast(AST_ADD_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONGLONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONGLONG),
                                                 new_signed_iliteral(INTEGER_LONGLONG, 7ll))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONGLONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONGLONG),
                                                 new_signed_iliteral(INTEGER_LONGLONG, 5ll))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)),
                                           new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_LONGLONG), 2,                                 // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)), 1, // non-terminal
                               new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_LONGLONG), 1,    // non-terminal
                                              new_identifier_srt(SRT_IDENT_EXPR,
                                                                 new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)),
                                                                 new_string("z")))),
                new_dtyped_srt(
                    SRT_ADD_EXPR, new_integer_dtype(DTYPE_LONGLONG), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("x")),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("y"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_unsigned_long_long(void) {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4, // non-terminal
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 3, // non-terminal
                    new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                            new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ull))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 4, // non-terminal
                    new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0),
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                            new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 6ull))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 4, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0),
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ull))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 6ull))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)),
                                           new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR,
                                                      new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)),
                                                      new_string("z")))),
                new_dtyped_srt(
                    SRT_SUB_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("x")),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("y"))))));

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_void_pointer(void) {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("malloc"))),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("n")))))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 1, // non-terminal
                    new_ast(AST_TYPE_VOID, 0)),
            new_ast(
                AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("free")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_VOID, 0)),
                                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))))))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 1, // non-terminal
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                    new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                            new_ast(AST_CALL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("malloc")),
                                    new_ast(AST_ARG_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))))),
        new_ast(AST_EXPR_STMT, 1,
                new_ast(AST_CALL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("free")),
                        new_ast(AST_ARG_LIST, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")))))

    );

    Vector* malloc_params = new_vector(&t_dparam);
    vector_push(malloc_params, new_named_dparam(new_string("n"), new_integer_dtype(DTYPE_INT)));
    DType* malloc_dtype = new_function_dtype(malloc_params, new_pointer_dtype(new_void_dtype()));

    Vector* free_params = new_vector(&t_dparam);
    vector_push(free_params, new_named_dparam(new_string("p"), new_pointer_dtype(new_void_dtype())));
    DType* free_dtype = new_function_dtype(free_params, new_void_dtype());

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, malloc_dtype, new_string("malloc")))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, free_dtype, new_string("free")))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("x")),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_dtyped_srt(
                                    SRT_CAST_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_dtyped_srt(
                                        SRT_CALL_EXPR, new_pointer_dtype(new_void_dtype()), 2, // non-terminal
                                        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(malloc_dtype)),
                                                       1, // non-terminal
                                                       new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(malloc_dtype),
                                                                          new_string("malloc"))),
                                        new_srt(SRT_ARG_LIST, 1, // non-terminal
                                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                 new_signed_iliteral(INTEGER_INT, 4)))))))),
        new_srt(SRT_EXPR_STMT, 1,
                new_dtyped_srt(
                    SRT_CALL_EXPR, new_void_dtype(), 2, // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(free_dtype)),
                                   1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(free_dtype), new_string("free"))),
                    new_srt(SRT_ARG_LIST, 1,                                                      // non-terminal
                            new_dtyped_srt(SRT_CAST_EXPR, new_pointer_dtype(new_void_dtype()), 1, // non-terminal
                                           new_identifier_srt(SRT_IDENT_EXPR,
                                                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                              new_string("x"))))))

    );

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_compound_stmt_empty(void) {
    Ast* input = new_ast(AST_CMPD_STMT, 0);

    Srt* expected = new_srt(SRT_CMPD_STMT, 0);

    run_stmt_resolver_test(input, NULL, NULL, expected);

    delete_srt(expected);
}

void test_resolve_return_stmt_without_cast(void) {
    Ast* input = new_ast(AST_RET_STMT, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    DType* return_dtype = new_integer_dtype(DTYPE_INT);

    Srt* expected =
        new_srt(SRT_RET_STMT, 1, // non-terminal
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)));

    run_stmt_resolver_test(input, NULL, return_dtype, expected);

    delete_srt(expected);
}

void test_resolve_return_stmt_with_cast(void) {
    Ast* input = new_ast(AST_RET_STMT, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    DType* return_dtype = new_integer_dtype(DTYPE_CHAR);

    Srt* expected = new_srt(SRT_RET_STMT, 1,                                                // non-terminal
                            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                           new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                            new_signed_iliteral(INTEGER_INT, 0))));

    run_stmt_resolver_test(input, NULL, return_dtype, expected);

    delete_srt(expected);
}

void test_resolve_return_stmt_without_value(void) {
    Ast* input = new_ast(AST_RET_STMT, 0);

    DType* return_dtype = new_void_dtype();

    Srt* expected = new_srt(SRT_RET_STMT, 0);

    run_stmt_resolver_test(input, NULL, return_dtype, expected);

    delete_srt(expected);
}

void test_resolve_expression_stmt(void) {
    Ast* input = new_ast(AST_EXPR_STMT, 1,           // non-terminal
                         new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 12))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(
        SRT_EXPR_STMT, 1, // non-terminal
        new_dtyped_srt(
            SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
            new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 12))));

    run_stmt_resolver_test(input, local_table, NULL, expected);

    delete_srt(expected);
}

void run_stmt_resolver_test(Ast* input, SymbolTable* local_table, DType* return_dtype, Srt* expected) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (local_table != NULL) {
        local_table->outer_scope = resolver->symbol_table;
        resolver->symbol_table = local_table;
    }
    resolver->return_dtype = return_dtype;

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_stmt(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);

    if (actual != NULL) {
        delete_srt(actual);
    }
    delete_resolver(resolver);
}
