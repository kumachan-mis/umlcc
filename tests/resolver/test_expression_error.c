#include "./test_expression_error.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_assign_expr_error_unassignable();
void test_resolve_assign_expr_error_unmodifiable();
void test_resolve_assign_expr_error_lhs();
void test_resolve_assign_expr_error_rhs();
void test_resolve_logical_or_expr_error_non_scalar();
void test_resolve_logical_or_expr_error_lhs();
void test_resolve_logical_or_expr_error_rhs();
void test_resolve_logical_and_expr_error_non_scalar_rhs();
void test_resolve_logical_and_expr_error_non_scalar_lhs();
void test_resolve_logical_and_expr_error_lhs();
void test_resolve_logical_and_expr_error_rhs();
void test_resolve_equal_expr_error_operand_dtype();
void test_resolve_equal_expr_error_incompatible_pointer();
void test_resolve_equal_expr_error_lhs();
void test_resolve_equal_expr_error_rhs();
void test_resolve_not_equal_expr_error_operand_dtype();
void test_resolve_not_equal_expr_error_incompatible_pointer();
void test_resolve_not_equal_expr_error_lhs();
void test_resolve_not_equal_expr_error_rhs();
void test_resolve_add_expr_error_operand_dtype();
void test_resolve_add_expr_error_lhs();
void test_resolve_add_expr_error_rhs();
void test_resolve_subtract_expr_error_operand_dtype();
void test_resolve_subtract_expr_error_incompatible_pointer();
void test_resolve_subtract_expr_error_lhs();
void test_resolve_subtract_expr_error_rhs();
void test_resolve_multiply_expr_error_non_arithmetic();
void test_resolve_multiply_expr_error_lhs();
void test_resolve_multiply_expr_error_rhs();
void test_resolve_division_expr_error_non_arithmetic();
void test_resolve_division_expr_error_lhs();
void test_resolve_division_expr_error_rhs();
void test_resolve_modulo_expr_error_non_integer();
void test_resolve_modulo_expr_error_lhs();
void test_resolve_modulo_expr_error_rhs();
void test_resolve_address_expr_error_operand_dtype();
void test_resolve_address_expr_error_child();
void test_resolve_indirection_expr_error_non_pointer();
void test_resolve_indirection_expr_error_child();
void test_resolve_logical_not_expr_error_non_scalar();
void test_resolve_logical_not_expr_error_child();
void test_resolve_call_expr_error_non_func();
void test_resolve_call_expr_error_num_params();
void test_resolve_call_expr_error_param_dtype();
void test_resolve_call_expr_error_child();
void test_resolve_subscription_expr_error_non_pointer_to_object();
void test_resolve_subscription_expr_error_non_pointer();
void test_resolve_subscription_expr_error_non_integer();
void test_resolve_subscription_expr_error_lhs();
void test_resolve_subscription_expr_error_rhs();
void test_resolve_ident_expr_error();

void run_expr_resolver_error_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                                  SymbolTable* __restrict__ global_table,
                                  Vector* __restrict__ messages);

CU_Suite* add_test_suite_expr_resolver_error() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_non_scalar);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_non_scalar_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_non_scalar_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_add_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_add_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_add_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_subtract_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_subtract_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_subtract_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_subtract_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_multiply_expr_error_non_arithmetic);
    CU_ADD_TEST(suite, test_resolve_multiply_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_multiply_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_division_expr_error_non_arithmetic);
    CU_ADD_TEST(suite, test_resolve_division_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_division_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_modulo_expr_error_non_integer);
    CU_ADD_TEST(suite, test_resolve_modulo_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_modulo_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_address_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_address_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_indirection_expr_error_non_pointer);
    CU_ADD_TEST(suite, test_resolve_indirection_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr_error_non_scalar);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_non_func);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_num_params);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_param_dtype);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_pointer_to_object);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_pointer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_integer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_ident_expr_error);
    return suite;
}

void test_resolve_assign_expr_error_unassignable() {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: expression is not assignable to lvalue\n"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_assign_expr_error_unmodifiable() {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: left operand of assignment should be modifiable lvalue\n"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"),
                              new_array_dtype(new_integer_dtype(DTYPE_INT), 3));
    symboltable_define_memory(local_table, new_string("y"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_assign_expr_error_lhs() {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_assign_expr_error_rhs() {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_logical_or_expr_error_non_scalar() {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_or_expr_error_lhs() {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_logical_or_expr_error_rhs() {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_logical_and_expr_error_non_scalar_rhs() {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_and_expr_error_non_scalar_lhs() {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_and_expr_error_lhs() {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_logical_and_expr_error_rhs() {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_equal_expr_error_operand_dtype() {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: binary == expression should be "
                                     "either arithmetic == arithmetic or pointer == pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_equal_expr_error_incompatible_pointer() {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: operands of pointer == pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_equal_expr_error_lhs() {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_equal_expr_error_rhs() {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_not_equal_expr_error_operand_dtype() {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: binary != expression should be "
                                     "either arithmetic != arithmetic or pointer != pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_not_equal_expr_error_incompatible_pointer() {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: operands of pointer != pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_not_equal_expr_error_lhs() {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_not_equal_expr_error_rhs() {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_add_expr_error_operand_dtype() {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: binary + expression should be either arithmetic + arithmetic, "
                           "pointer + integer, or integer + pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_add_expr_error_lhs() {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_add_expr_error_rhs() {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subtract_expr_error_operand_dtype() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: binary - expression should be either arithmetic - arithmetic, "
                           "pointer - integer, or pointer - pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subtract_expr_error_incompatible_pointer() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: operands of pointer - pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subtract_expr_error_lhs() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subtract_expr_error_rhs() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_multiply_expr_error_non_arithmetic() {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: binary * expression should be arithmetic * arithmetic\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_multiply_expr_error_lhs() {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_multiply_expr_error_rhs() {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_division_expr_error_non_arithmetic() {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: binary / expression should be arithmetic / arithmetic\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_division_expr_error_lhs() {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_division_expr_error_rhs() {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_modulo_expr_error_non_integer() {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: binary % expression should be integer % integer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_modulo_expr_error_lhs() {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_modulo_expr_error_rhs() {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_address_expr_error_operand_dtype() {
    Ast* input = new_ast(AST_ADDR_EXPR, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: operand of unary & is neither a function designator, "
                                     "a indirection, nor an object lvalue\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_address_expr_error_child() {
    Ast* input = new_ast(AST_ADDR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'value' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_indirection_expr_error_non_pointer() {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: operand of unary * does not have pointer type\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_indirection_expr_error_child() {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'p' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_logical_not_expr_error_non_scalar() {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_not_expr_error_child() {
    Ast* input = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                         new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'flag' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_call_expr_error_non_func() {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 2);
    symboltable_define_memory(local_table, new_string("function"), array_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: called object is not a function or a function pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_call_expr_error_num_params() {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: function takes 1 params, but passed 2 arguments\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_call_expr_error_param_dtype() {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 3, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(params, new_dparam(new_string("z"), new_integer_dtype(DTYPE_INT)));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("y"),
                              new_array_dtype(new_integer_dtype(DTYPE_INT), 2));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: argument is not assignable to parameter\n"));
    vector_push(messages, new_string("Error: argument is not assignable to parameter\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_call_expr_error_child() {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 3, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(params, new_dparam(new_string("z"), new_integer_dtype(DTYPE_INT)));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));
    vector_push(messages, new_string("Error: identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subscription_expr_error_non_pointer_to_object() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("a"), func_dtype);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: subscribed object should have pointer to object type\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subscription_expr_error_non_pointer() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("a"), new_integer_dtype(DTYPE_INT));

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: subscription should be pointer[integer] or integer[pointer]\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subscription_expr_error_non_integer() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("a"), array_dtype);
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("i"), pointer_dtype);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: subscription should be pointer[integer] or integer[pointer]\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subscription_expr_error_lhs() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'a' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void test_resolve_subscription_expr_error_rhs() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("a"), array_dtype);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'i' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, messages);

    delete_vector(messages);
}

void test_resolve_ident_expr_error() {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("x"));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, messages);

    delete_vector(messages);
}

void run_expr_resolver_error_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                                  SymbolTable* __restrict__ global_table,
                                  Vector* __restrict__ messages) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    if (local_table != NULL) resolver->local_table = local_table;

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_expr(resolver));

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
