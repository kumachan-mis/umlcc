#include "../../src/resolver/statement.h"
#include "../testlib/testlib.h"
#include "./test_external_error.h"

void test_resolve_compound_stmt_error_child(void);
void test_resolve_return_stmt_error_child(void);
void test_resolve_return_stmt_error_unassignable(void);
void test_resolve_expression_stmt_error_child(void);

void run_stmt_resolver_error_test(Ast* input, SymbolTable* local_table, Dtype* return_dtype, Vector* expected);

CU_Suite* add_test_suite_stmt_resolver_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_error_child);
    CU_ADD_TEST(suite, test_resolve_return_stmt_error_child);
    CU_ADD_TEST(suite, test_resolve_return_stmt_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_expression_stmt_error_child);
    return suite;
}

void test_resolve_compound_stmt_error_child(void) {
    Ast* input = new_ast(AST_CMPD_STMT, 4,                       // non-terminal
                         new_ast(AST_DECL, 2,                    // non-terminal
                                 new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                         new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))),
                         new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                 new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_ast(AST_SUB_EXPR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                         new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                                 new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                         new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))),
                         new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                 new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));
    vector_push(expected, new_error("operand of unary * does not have pointer type\n"));

    run_stmt_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_return_stmt_error_child(void) {
    Ast* input = new_ast(AST_RET_STMT, 1,         // non-terminal
                         new_ast(AST_ADD_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_stmt_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_return_stmt_error_unassignable(void) {
    Ast* input = new_ast(AST_RET_STMT, 1,         // non-terminal
                         new_ast(AST_ADD_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to function return\n"));

    run_stmt_resolver_error_test(input, local_table, return_dtype, expected);

    delete_vector(expected);
}

void test_resolve_expression_stmt_error_child(void) {
    Ast* input = new_ast(AST_EXPR_STMT, 1,           // non-terminal
                         new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_ast(AST_ADD_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_stmt_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void run_stmt_resolver_error_test(Ast* input, SymbolTable* local_table, Dtype* return_dtype, Vector* expected) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (local_table != NULL) resolver->local_table = local_table;
    resolver->return_dtype = return_dtype;

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_stmt(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}
