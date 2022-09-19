#include "../../src/resolver/statement.h"
#include "../testlib/testlib.h"
#include "./test_external_error.h"

void test_resolve_compound_stmt_error_child();
void test_resolve_return_stmt_error_child();
void test_resolve_return_stmt_error_unassignable();
void test_resolve_expression_stmt_error_child();

void run_stmt_resolver_error_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                                  Dtype* __restrict__ return_dtype, Vector* __restrict__ messages);

CU_Suite* add_test_suite_stmt_resolver_error() {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_compound_stmt_error_child);
    CU_ADD_TEST(suite, test_resolve_return_stmt_error_child);
    CU_ADD_TEST(suite, test_resolve_return_stmt_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_expression_stmt_error_child);
    return suite;
}

void test_resolve_compound_stmt_error_child() {
    Ast* input = new_ast(
        AST_CMPD_STMT, 4, // non-terminal
        new_ast(
            AST_DECL, 2,                    // non-terminal
            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))),
        new_ast(
            AST_EXPR_STMT, 1,           // non-terminal
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

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));
    vector_push(messages, new_string("Error: operand of unary * does not have pointer type\n"));

    run_stmt_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_return_stmt_error_child() {
    Ast* input = new_ast(AST_RET_STMT, 1,         // non-terminal
                         new_ast(AST_ADD_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_stmt_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_return_stmt_error_unassignable() {
    Ast* input =
        new_ast(AST_RET_STMT, 1,         // non-terminal
                new_ast(AST_ADD_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: expression is not assignable to function return\n"));

    run_stmt_resolver_error_test(input, local_table, return_dtype, messages);

    delete_vector(messages);
}

void test_resolve_expression_stmt_error_child() {
    Ast* input = new_ast(AST_EXPR_STMT, 1,           // non-terminal
                         new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_ast(AST_ADD_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_stmt_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void run_stmt_resolver_error_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                                  Dtype* __restrict__ return_dtype, Vector* __restrict__ messages) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (local_table != NULL) resolver->local_table = local_table;
    resolver->return_dtype = return_dtype;

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_stmt(resolver));

    CU_ASSERT_PTR_NULL(actual);

    CU_ASSERT_EQUAL(vector_size(errs), vector_size(messages));
    int num_messages = vector_size(messages);
    for (int i = 0; i < num_messages; i++) {
        Error* err = vector_at(errs, i);
        char* message = vector_at(messages, i);
        CU_ASSERT_STRING_EQUAL(err->message, message);
    }

    delete_vector(errs);
    delete_resolver(resolver);
}
