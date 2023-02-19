#include "./test_statement.h"
#include "../../src/resolver/statement.h"
#include "../testlib/testlib.h"

void test_resolve_compound_stmt_integer_vardef(void);
void test_resolve_compound_stmt_pointer_typedef(void);
void test_resolve_compound_stmt_empty(void);
void test_resolve_return_stmt_without_cast(void);
void test_resolve_return_stmt_with_cast(void);
void test_resolve_expression_stmt(void);

void run_stmt_resolver_test(Ast* input, SymbolTable* local_table, DType* return_dtype, Srt* expected);

CU_Suite* add_test_suite_stmt_resolver(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_integer_vardef);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_pointer_typedef);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_empty);
    CU_ADD_TEST(suite, test_resolve_return_stmt_without_cast);
    CU_ADD_TEST(suite, test_resolve_return_stmt_with_cast);
    CU_ADD_TEST(suite, test_resolve_expression_stmt);
    return suite;
}

void test_resolve_compound_stmt_integer_vardef(void) {
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

void test_resolve_compound_stmt_pointer_typedef(void) {
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

    DType* pint_def_dtype = new_decoration_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    pint_def_dtype->ddecoration->typedef_flag = 1;

    Srt* expected = new_srt(
        SRT_CMPD_STMT, 3,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, pint_def_dtype, new_string("pint")))),
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

    if (actual != NULL) delete_srt(actual);
    delete_resolver(resolver);
}
