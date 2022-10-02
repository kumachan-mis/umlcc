#include "./test_expression_error.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_assign_expr_error_unassignable(void);
void test_resolve_assign_expr_error_unmodifiable(void);
void test_resolve_assign_expr_error_lhs(void);
void test_resolve_assign_expr_error_rhs(void);
void test_resolve_logical_or_expr_error_non_scalar(void);
void test_resolve_logical_or_expr_error_lhs(void);
void test_resolve_logical_or_expr_error_rhs(void);
void test_resolve_logical_and_expr_error_non_scalar_rhs(void);
void test_resolve_logical_and_expr_error_non_scalar_lhs(void);
void test_resolve_logical_and_expr_error_lhs(void);
void test_resolve_logical_and_expr_error_rhs(void);
void test_resolve_equal_expr_error_operand_dtype(void);
void test_resolve_equal_expr_error_incompatible_pointer(void);
void test_resolve_equal_expr_error_lhs(void);
void test_resolve_equal_expr_error_rhs(void);
void test_resolve_not_equal_expr_error_operand_dtype(void);
void test_resolve_not_equal_expr_error_incompatible_pointer(void);
void test_resolve_not_equal_expr_error_lhs(void);
void test_resolve_not_equal_expr_error_rhs(void);
void test_resolve_add_expr_error_operand_dtype(void);
void test_resolve_add_expr_error_lhs(void);
void test_resolve_add_expr_error_rhs(void);
void test_resolve_subtract_expr_error_operand_dtype(void);
void test_resolve_subtract_expr_error_incompatible_pointer(void);
void test_resolve_subtract_expr_error_lhs(void);
void test_resolve_subtract_expr_error_rhs(void);
void test_resolve_multiply_expr_error_non_arithmetic(void);
void test_resolve_multiply_expr_error_lhs(void);
void test_resolve_multiply_expr_error_rhs(void);
void test_resolve_division_expr_error_non_arithmetic(void);
void test_resolve_division_expr_error_lhs(void);
void test_resolve_division_expr_error_rhs(void);
void test_resolve_modulo_expr_error_non_integer(void);
void test_resolve_modulo_expr_error_lhs(void);
void test_resolve_modulo_expr_error_rhs(void);
void test_resolve_address_expr_error_operand_dtype(void);
void test_resolve_address_expr_error_child(void);
void test_resolve_indirection_expr_error_non_pointer(void);
void test_resolve_indirection_expr_error_child(void);
void test_resolve_logical_not_expr_error_non_scalar(void);
void test_resolve_logical_not_expr_error_child(void);
void test_resolve_call_expr_error_non_func(void);
void test_resolve_call_expr_error_num_params(void);
void test_resolve_call_expr_error_param_dtype(void);
void test_resolve_call_expr_error_child(void);
void test_resolve_subscription_expr_error_non_obj_pointer(void);
void test_resolve_subscription_expr_error_non_pointer(void);
void test_resolve_subscription_expr_error_non_integer(void);
void test_resolve_subscription_expr_error_lhs(void);
void test_resolve_subscription_expr_error_rhs(void);
void test_resolve_ident_expr_error(void);

void run_expr_resolver_error_test(Ast* input, SymbolTable* local_table, SymbolTable* global_table, Vector* expected);

CU_Suite* add_test_suite_expr_resolver_error(void) {
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
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_obj_pointer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_pointer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_integer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_ident_expr_error);
    return suite;
}

void test_resolve_assign_expr_error_unassignable(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to lvalue\n"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_assign_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("left operand of assignment should be modifiable lvalue\n"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_array_dtype(new_integer_dtype(DTYPE_INT), 3));
    symboltable_define_memory(local_table, new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_assign_expr_error_lhs(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_assign_expr_error_rhs(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_or_expr_error_non_scalar(void) {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_or_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_or_expr_error_rhs(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_and_expr_error_non_scalar_rhs(void) {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_and_expr_error_non_scalar_lhs(void) {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_and_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_and_expr_error_rhs(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_equal_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary == expression should be "
                                    "either arithmetic == arithmetic or pointer == pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_equal_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer == pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_equal_expr_error_rhs(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_not_equal_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary != expression should be "
                                    "either arithmetic != arithmetic or pointer != pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_not_equal_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer != pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_not_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_not_equal_expr_error_rhs(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_add_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary + expression should be either arithmetic + arithmetic, "
                                    "pointer + integer, or integer + pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_add_expr_error_lhs(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_add_expr_error_rhs(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subtract_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary - expression should be either arithmetic - arithmetic, "
                                    "pointer - integer, or pointer - pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subtract_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer - pointer are not compatible\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subtract_expr_error_lhs(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subtract_expr_error_rhs(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_multiply_expr_error_non_arithmetic(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary * expression should be arithmetic * arithmetic\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_multiply_expr_error_lhs(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_multiply_expr_error_rhs(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_division_expr_error_non_arithmetic(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary / expression should be arithmetic / arithmetic\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_division_expr_error_lhs(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_division_expr_error_rhs(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_modulo_expr_error_non_integer(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary %% expression should be integer %% integer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_modulo_expr_error_lhs(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_modulo_expr_error_rhs(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_address_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_ADDR_EXPR, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary & is neither a function designator, "
                                    "a indirection, nor an object lvalue\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_address_expr_error_child(void) {
    Ast* input = new_ast(AST_ADDR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("value")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'value' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_indirection_expr_error_non_pointer(void) {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary * does not have pointer type\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_indirection_expr_error_child(void) {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'p' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_not_expr_error_non_scalar(void) {
    // TODO: implement here after non-scalar dtype is introduced
}

void test_resolve_logical_not_expr_error_child(void) {
    Ast* input = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                         new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'flag' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_non_func(void) {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 2);
    symboltable_define_memory(local_table, new_string("function"), array_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("called object is not a function or a function pointer\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_num_params(void) {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function takes 1 params, but passed 2 arguments\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_param_dtype(void) {
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
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("y"), new_array_dtype(new_integer_dtype(DTYPE_INT), 2));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("argument is not assignable to parameter\n"));
    vector_push(expected, new_error("argument is not assignable to parameter\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_child(void) {
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
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));
    vector_push(expected, new_error("identifier 'y' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_non_obj_pointer(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("a"), func_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("subscribed object should have pointer to object type\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_non_pointer(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("a"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("subscription should be pointer[integer] or integer[pointer]\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_non_integer(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("a"), array_dtype);
    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("i"), pointer_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("subscription should be pointer[integer] or integer[pointer]\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_lhs(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'a' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_rhs(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("a"), array_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'i' is used before declared\n"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_ident_expr_error(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("x"));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared\n"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void run_expr_resolver_error_test(Ast* input, SymbolTable* local_table, SymbolTable* global_table, Vector* expected) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (global_table != NULL) {
        delete_symboltable(resolver->symbol_table);
        resolver->symbol_table = global_table;
    }
    if (local_table != NULL) {
        local_table->outer_scope = resolver->symbol_table;
        resolver->symbol_table = local_table;
    }

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_expr(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}
