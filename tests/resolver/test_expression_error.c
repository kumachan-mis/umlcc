#include "./test_expression_error.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_assign_expr_error_unassignable(void);
void test_resolve_assign_expr_error_void(void);
void test_resolve_assign_expr_error_unmodifiable(void);
void test_resolve_assign_expr_error_lhs(void);
void test_resolve_assign_expr_error_rhs(void);
void test_resolve_conditional_expr_error_condition_child(void);
void test_resolve_conditional_expr_error_condition_non_scalar(void);
void test_resolve_conditional_expr_error_lhs(void);
void test_resolve_conditional_expr_error_rhs(void);
void test_resolve_conditional_expr_error_incompatible_dtype(void);
void test_resolve_conditional_expr_error_incompatible_pointer(void);
void test_resolve_conditional_expr_error_different_struct(void);
void test_resolve_logical_or_expr_error_non_scalar_lhs(void);
void test_resolve_logical_or_expr_error_non_scalar_rhs(void);
void test_resolve_logical_or_expr_error_lhs(void);
void test_resolve_logical_or_expr_error_rhs(void);
void test_resolve_logical_and_expr_error_non_scalar_lhs(void);
void test_resolve_logical_and_expr_error_non_scalar_rhs(void);
void test_resolve_logical_and_expr_error_lhs(void);
void test_resolve_logical_and_expr_error_rhs(void);
void test_resolve_bitwise_inclusive_or_expr_error_non_integer_lhs(void);
void test_resolve_bitwise_inclusive_or_expr_error_non_integer_rhs(void);
void test_resolve_bitwise_inclusive_or_expr_error_lhs(void);
void test_resolve_bitwise_inclusive_or_expr_error_rhs(void);
void test_resolve_bitwise_exclusive_or_expr_error_non_integer_lhs(void);
void test_resolve_bitwise_exclusive_or_expr_error_non_integer_rhs(void);
void test_resolve_bitwise_exclusive_or_expr_error_lhs(void);
void test_resolve_bitwise_exclusive_or_expr_error_rhs(void);
void test_resolve_bitwise_and_expr_error_non_integer_lhs(void);
void test_resolve_bitwise_and_expr_error_non_integer_rhs(void);
void test_resolve_bitwise_and_expr_error_lhs(void);
void test_resolve_bitwise_and_expr_error_rhs(void);
void test_resolve_equal_expr_error_operand_dtype(void);
void test_resolve_equal_expr_error_incompatible_pointer(void);
void test_resolve_equal_expr_error_lhs(void);
void test_resolve_equal_expr_error_rhs(void);
void test_resolve_not_equal_expr_error_operand_dtype(void);
void test_resolve_not_equal_expr_error_incompatible_pointer(void);
void test_resolve_not_equal_expr_error_lhs(void);
void test_resolve_not_equal_expr_error_rhs(void);
void test_resolve_less_expr_error_operand_dtype(void);
void test_resolve_less_expr_error_incompatible_pointer(void);
void test_resolve_less_expr_error_lhs(void);
void test_resolve_less_expr_error_rhs(void);
void test_resolve_greater_expr_error_operand_dtype(void);
void test_resolve_greater_expr_error_incompatible_pointer(void);
void test_resolve_greater_expr_error_lhs(void);
void test_resolve_greater_expr_error_rhs(void);
void test_resolve_less_equal_expr_error_operand_dtype(void);
void test_resolve_less_equal_expr_error_incompatible_pointer(void);
void test_resolve_less_equal_expr_error_lhs(void);
void test_resolve_less_equal_expr_error_rhs(void);
void test_resolve_greater_equal_expr_error_operand_dtype(void);
void test_resolve_greater_equal_expr_error_incompatible_pointer(void);
void test_resolve_greater_equal_expr_error_lhs(void);
void test_resolve_greater_equal_expr_error_rhs(void);
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
void test_resolve_cast_expr_error_type_name(void);
void test_resolve_cast_expr_error_child(void);
void test_resolve_cast_expr_error_non_scalar(void);
void test_resolve_preinc_expr_error_unmodifiable(void);
void test_resolve_preinc_expr_error_operand_dtype(void);
void test_resolve_preinc_expr_error_child(void);
void test_resolve_predec_expr_error_unmodifiable(void);
void test_resolve_predec_expr_error_operand_dtype(void);
void test_resolve_predec_expr_error_child(void);
void test_resolve_address_expr_error_operand_dtype(void);
void test_resolve_address_expr_error_child(void);
void test_resolve_indirection_expr_error_non_pointer(void);
void test_resolve_indirection_expr_error_child(void);
void test_resolve_positive_expr_error_non_arithmetic(void);
void test_resolve_positive_expr_error_child(void);
void test_resolve_negative_expr_error_non_arithmetic(void);
void test_resolve_negative_expr_error_child(void);
void test_resolve_bitwise_not_expr_error_non_integer(void);
void test_resolve_bitwise_not_expr_error_child(void);
void test_resolve_logical_not_expr_error_non_scalar(void);
void test_resolve_logical_not_expr_error_child(void);
void test_resolve_sizeof_expr_error_typename(void);
void test_resolve_sizeof_expr_error_expr(void);
void test_resolve_sizeof_expr_error_incomplete(void);
void test_resolve_sizeof_expr_error_function(void);
void test_resolve_call_expr_error_non_func(void);
void test_resolve_call_expr_error_num_params(void);
void test_resolve_call_expr_error_param_dtype(void);
void test_resolve_call_expr_error_callee_child(void);
void test_resolve_call_expr_error_param_child(void);
void test_resolve_subscription_expr_error_non_obj_pointer(void);
void test_resolve_subscription_expr_error_non_pointer(void);
void test_resolve_subscription_expr_error_non_integer(void);
void test_resolve_subscription_expr_error_lhs(void);
void test_resolve_subscription_expr_error_rhs(void);
void test_resolve_member_expr_error_non_struct(void);
void test_resolve_member_expr_error_unknown_member(void);
void test_resolve_member_expr_error_incomplete_struct(void);
void test_resolve_member_expr_error_accessee_child(void);
void test_resolve_tomember_expr_error_non_pointer_to_struct(void);
void test_resolve_tomember_expr_error_unknown_member(void);
void test_resolve_tomember_expr_error_incomplete_struct(void);
void test_resolve_tomember_expr_error_accessee_child(void);
void test_resolve_postinc_expr_error_unmodifiable(void);
void test_resolve_postinc_expr_error_operand_dtype(void);
void test_resolve_postinc_expr_error_child(void);
void test_resolve_postdec_expr_error_unmodifiable(void);
void test_resolve_postdec_expr_error_operand_dtype(void);
void test_resolve_postdec_expr_error_child(void);
void test_resolve_ident_expr_error(void);

void run_expr_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected);

CU_Suite* add_test_suite_expr_resolver_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_expr_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_void);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_assign_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_condition_child);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_condition_non_scalar);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_incompatible_dtype);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_error_different_struct);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_non_scalar_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_non_scalar_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_non_scalar_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_non_scalar_rhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_inclusive_or_expr_error_non_integer_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_inclusive_or_expr_error_non_integer_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_inclusive_or_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_inclusive_or_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_exclusive_or_expr_error_non_integer_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_exclusive_or_expr_error_non_integer_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_exclusive_or_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_exclusive_or_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_and_expr_error_non_integer_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_and_expr_error_non_integer_rhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_and_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_bitwise_and_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_equal_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_less_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_less_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_less_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_less_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_greater_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_greater_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_greater_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_greater_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_less_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_less_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_less_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_less_equal_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_greater_equal_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_greater_equal_expr_error_incompatible_pointer);
    CU_ADD_TEST(suite, test_resolve_greater_equal_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_greater_equal_expr_error_rhs);
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
    CU_ADD_TEST(suite, test_resolve_cast_expr_error_type_name);
    CU_ADD_TEST(suite, test_resolve_cast_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_cast_expr_error_non_scalar);
    CU_ADD_TEST(suite, test_resolve_preinc_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_preinc_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_preinc_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_predec_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_predec_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_predec_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_address_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_address_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_indirection_expr_error_non_pointer);
    CU_ADD_TEST(suite, test_resolve_indirection_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_positive_expr_error_non_arithmetic);
    CU_ADD_TEST(suite, test_resolve_positive_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_negative_expr_error_non_arithmetic);
    CU_ADD_TEST(suite, test_resolve_negative_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_bitwise_not_expr_error_non_integer);
    CU_ADD_TEST(suite, test_resolve_bitwise_not_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr_error_non_scalar);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_error_typename);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_error_expr);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_error_incomplete);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_error_function);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_non_func);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_num_params);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_param_dtype);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_callee_child);
    CU_ADD_TEST(suite, test_resolve_call_expr_error_param_child);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_obj_pointer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_pointer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_non_integer);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_lhs);
    CU_ADD_TEST(suite, test_resolve_subscription_expr_error_rhs);
    CU_ADD_TEST(suite, test_resolve_member_expr_error_non_struct);
    CU_ADD_TEST(suite, test_resolve_member_expr_error_unknown_member);
    CU_ADD_TEST(suite, test_resolve_member_expr_error_incomplete_struct);
    CU_ADD_TEST(suite, test_resolve_member_expr_error_accessee_child);
    CU_ADD_TEST(suite, test_resolve_tomember_expr_error_non_pointer_to_struct);
    CU_ADD_TEST(suite, test_resolve_tomember_expr_error_unknown_member);
    CU_ADD_TEST(suite, test_resolve_tomember_expr_error_incomplete_struct);
    CU_ADD_TEST(suite, test_resolve_tomember_expr_error_accessee_child);
    CU_ADD_TEST(suite, test_resolve_postinc_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_postinc_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_postinc_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_postdec_expr_error_unmodifiable);
    CU_ADD_TEST(suite, test_resolve_postdec_expr_error_operand_dtype);
    CU_ADD_TEST(suite, test_resolve_postdec_expr_error_child);
    CU_ADD_TEST(suite, test_resolve_ident_expr_error);
    return suite;
}

void test_resolve_assign_expr_error_unassignable(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to lvalue"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_assign_expr_error_void(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_ast(AST_CALL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("function")), new_ast(AST_ARG_LIST, 0)));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    DType* func_dtype = new_function_dtype(params, new_void_dtype());
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("function"), func_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to lvalue"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_assign_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("left operand of assignment should be modifiable lvalue"));

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
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_condition_child(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_condition_non_scalar(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("condition of conditional expression should have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_lhs(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    SymbolTable* local_teble = new_symboltable();
    symboltable_define_memory(local_teble, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("z"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_teble, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_rhs(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    SymbolTable* local_teble = new_symboltable();
    symboltable_define_memory(local_teble, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'z' is used before declared"));

    run_expr_resolver_error_test(input, local_teble, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_incompatible_dtype(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    SymbolTable* local_teble = new_symboltable();
    symboltable_define_memory(local_teble, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("y"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("z"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of conditional expression are not compatible"));

    run_expr_resolver_error_test(input, local_teble, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    SymbolTable* local_teble = new_symboltable();
    symboltable_define_memory(local_teble, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("y"), new_pointer_dtype(new_integer_dtype(DTYPE_LONG)));
    symboltable_define_memory(local_teble, new_string("z"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of conditional expression are not compatible"));

    run_expr_resolver_error_test(input, local_teble, NULL, expected);

    delete_vector(expected);
}

void test_resolve_conditional_expr_error_different_struct(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("z")));

    Vector* members_y = new_vector(&t_dstructmember);
    vector_push(members_y, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_LONG)));
    Vector* members_z = new_vector(&t_dstructmember);
    vector_push(members_z, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_teble = new_symboltable();
    symboltable_define_memory(local_teble, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_teble, new_string("y"), new_unnamed_struct_dtype(members_y));
    symboltable_define_memory(local_teble, new_string("z"), new_unnamed_struct_dtype(members_z));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of conditional expression are not compatible"));

    run_expr_resolver_error_test(input, local_teble, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_or_expr_error_non_scalar_lhs(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("each of operands of || should have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_or_expr_error_non_scalar_rhs(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("each of operands of || should have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_or_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_and_expr_error_non_scalar_lhs(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("each of operands of && should have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_and_expr_error_non_scalar_rhs(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("each of operands of && should have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_and_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_inclusive_or_expr_error_non_integer_lhs(void) {
    Ast* input = new_ast(AST_OR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary | expression should be integer | integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_inclusive_or_expr_error_non_integer_rhs(void) {
    Ast* input = new_ast(AST_OR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_UNSIGNED_INT));
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary | expression should be integer | integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_inclusive_or_expr_error_lhs(void) {
    Ast* input = new_ast(AST_OR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_inclusive_or_expr_error_rhs(void) {
    Ast* input = new_ast(AST_OR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_exclusive_or_expr_error_non_integer_lhs(void) {
    Ast* input = new_ast(AST_XOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary ^ expression should be integer ^ integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_exclusive_or_expr_error_non_integer_rhs(void) {
    Ast* input = new_ast(AST_XOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary ^ expression should be integer ^ integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_exclusive_or_expr_error_lhs(void) {
    Ast* input = new_ast(AST_XOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_exclusive_or_expr_error_rhs(void) {
    Ast* input = new_ast(AST_XOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_and_expr_error_non_integer_lhs(void) {
    Ast* input = new_ast(AST_AND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_LONG));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary & expression should be integer & integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_and_expr_error_non_integer_rhs(void) {
    Ast* input = new_ast(AST_AND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("binary & expression should be integer & integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_and_expr_error_lhs(void) {
    Ast* input = new_ast(AST_AND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_and_expr_error_rhs(void) {
    Ast* input = new_ast(AST_AND_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_LONG));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("operands of pointer == pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("operands of pointer != pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_not_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_LESS_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected,
                new_error("binary < expression should be either arithmetic < arithmetic or pointer < pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_LESS_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer < pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LESS_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_expr_error_rhs(void) {
    Ast* input = new_ast(AST_LESS_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_GREATER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected,
                new_error("binary > expression should be either arithmetic > arithmetic or pointer > pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_GREATER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer > pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_expr_error_lhs(void) {
    Ast* input = new_ast(AST_GREATER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}
void test_resolve_greater_expr_error_rhs(void) {
    Ast* input = new_ast(AST_GREATER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_equal_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_LESSEQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected,
                new_error("binary <= expression should be either arithmetic <= arithmetic or pointer <= pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_equal_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_LESSEQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer <= pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_LESSEQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));
    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_less_equal_expr_error_rhs(void) {
    Ast* input = new_ast(AST_LESSEQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_equal_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected,
                new_error("binary >= expression should be either arithmetic >= arithmetic or pointer >= pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_equal_expr_error_incompatible_pointer(void) {
    Ast* input = new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operands of pointer >= pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_equal_expr_error_lhs(void) {
    Ast* input = new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));
    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_greater_equal_expr_error_rhs(void) {
    Ast* input = new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("operands of pointer - pointer are not compatible"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subtract_expr_error_lhs(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("binary * expression should be arithmetic * arithmetic"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_multiply_expr_error_lhs(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("binary / expression should be arithmetic / arithmetic"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_division_expr_error_lhs(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("binary %% expression should be integer %% integer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_modulo_expr_error_lhs(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

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
    vector_push(expected, new_error("identifier 'y' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_cast_expr_error_type_name(void) {
    Ast* input =
        new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                new_ast(AST_TYPE_NAME, 2,              // non-terminal
                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_ast(AST_ABS_DECLOR, 0),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_VOID, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))))),
                new_identifier_ast(AST_IDENT_EXPR, new_string("f")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("f"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function parameter 'x' has an incomplete type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_cast_expr_error_child(void) {
    Ast* input = new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_ABS_DECLOR, 0)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_cast_expr_error_non_scalar(void) {
    Ast* input = new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                         new_ast(AST_ABS_DECLOR, 0),
                                         new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                 new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                         new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                 new_ast(AST_TYPE_VOID, 0)),
                                                         new_ast(AST_ABS_DECLOR, 0))))),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("f")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("f"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("cast should be from an any type to void or from a scalar type to scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_preinc_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_PREINC_EXPR, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of ++ is not modifiable"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_preinc_expr_error_child(void) {
    Ast* input = new_ast(AST_PREINC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_preinc_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_PREINC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("i"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of ++ should be either integer or pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_predec_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_PREDEC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("b"), array_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of -- is not modifiable"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_predec_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_PREDEC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("j")));

    SymbolTable* local_table = new_symboltable();
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("j"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of -- should be either integer or pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_predec_expr_error_child(void) {
    Ast* input = new_ast(AST_PREDEC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

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
    vector_push(expected, new_error("identifier 'value' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_indirection_expr_error_non_pointer(void) {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary * does not have pointer type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_indirection_expr_error_child(void) {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'p' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_not_expr_error_non_scalar(void) {
    Ast* input = new_ast(AST_LNOT_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")));

    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary ! does not have scalar type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_logical_not_expr_error_child(void) {
    Ast* input = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                         new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'flag' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_positive_expr_error_non_arithmetic(void) {
    Ast* input = new_ast(AST_POS_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary + does not have arithmetic type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_positive_expr_error_child(void) {
    Ast* input = new_ast(AST_POS_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_negative_expr_error_non_arithmetic(void) {
    Ast* input = new_ast(AST_NEG_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary - does not have arithmetic type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_negative_expr_error_child(void) {
    Ast* input = new_ast(AST_NEG_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_not_expr_error_non_integer(void) {
    Ast* input = new_ast(AST_NOT_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("bitfield")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("bitfield"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of unary ~ does not have integer type"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_bitwise_not_expr_error_child(void) {
    Ast* input = new_ast(AST_NOT_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("bitfield")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'bitfield' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_sizeof_expr_error_typename(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 2, // non-terminal
                                         new_ast(AST_TYPE_CHAR, 0), new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_ABS_DECLOR, 0)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("combination of type specifiers is invalid"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_sizeof_expr_error_expr(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_sizeof_expr_error_incomplete(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_VOID, 0)),
                                 new_ast(AST_ABS_DECLOR, 0)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of sizeof has neither function type nor an incomplete type"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_sizeof_expr_error_function(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                         new_ast(AST_ABS_DECLOR, 0),
                                         new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                 new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                         new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                 new_ast(AST_TYPE_VOID, 0)),
                                                         new_ast(AST_ABS_DECLOR, 0))))));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of sizeof has neither function type nor an incomplete type"));

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
    vector_push(expected, new_error("called object is not a function or a function pointer"));

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
    vector_push(params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function takes 1 params, but passed 2 arguments"));

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
    vector_push(params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_named_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(params, new_named_dparam(new_string("z"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("x"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("y"), new_array_dtype(new_integer_dtype(DTYPE_INT), 2));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("argument is not assignable to parameter"));
    vector_push(expected, new_error("argument is not assignable to parameter"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_callee_child(void) {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'function' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_call_expr_error_param_child(void) {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 3, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_named_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(params, new_named_dparam(new_string("z"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));
    vector_push(expected, new_error("identifier 'y' is used before declared"));

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
    vector_push(expected, new_error("subscribed object should have pointer to object type"));

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
    vector_push(expected, new_error("subscription should be pointer[integer] or integer[pointer]"));

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
    vector_push(expected, new_error("subscription should be pointer[integer] or integer[pointer]"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_subscription_expr_error_lhs(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'a' is used before declared"));

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
    vector_push(expected, new_error("identifier 'i' is used before declared"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_member_expr_error_non_struct(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_pointer_dtype(named_struct));
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("dot-accessed object is not a struct"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_member_expr_error_incomplete_struct(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 0, 0);

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), named_struct);
    TagTable* local_tag_table = new_tagtable();

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct 'Struct' is incomplete"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_member_expr_error_unknown_member(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("unknown")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), named_struct);
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("member 'unknown' does not exist in struct"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_member_expr_error_accessee_child(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'structure' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_tomember_expr_error_non_pointer_to_struct(void) {
    Ast* input = new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), named_struct);
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("arrow-accessed object is not a pointer to a struct"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_tomember_expr_error_incomplete_struct(void) {
    Ast* input = new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 0, 0);

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_pointer_dtype(named_struct));
    TagTable* local_tag_table = new_tagtable();

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("struct 'Struct' is incomplete"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_tomember_expr_error_unknown_member(void) {
    Ast* input = new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("unknown")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_pointer_dtype(named_struct));
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("member 'unknown' does not exist in struct"));

    run_expr_resolver_error_test(input, local_table, local_tag_table, expected);

    delete_vector(expected);
}

void test_resolve_tomember_expr_error_accessee_child(void) {
    Ast* input = new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'structure' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postinc_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_POSTINC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("f")));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("f"), func_dtype);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of ++ is not modifiable"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postinc_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_POSTINC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    SymbolTable* local_table = new_symboltable();
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("i"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of ++ should be either integer or pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postinc_expr_error_child(void) {
    Ast* input = new_ast(AST_POSTINC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postdec_expr_error_unmodifiable(void) {
    Ast* input = new_ast(AST_POSTDEC_EXPR, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of -- is not modifiable"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postdec_expr_error_operand_dtype(void) {
    Ast* input = new_ast(AST_POSTDEC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("j")));

    SymbolTable* local_table = new_symboltable();
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("j"), new_unnamed_struct_dtype(members));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("operand of -- should be either integer or pointer"));

    run_expr_resolver_error_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_resolve_postdec_expr_error_child(void) {
    Ast* input = new_ast(AST_POSTDEC_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_resolve_ident_expr_error(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("x"));

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'x' is used before declared"));

    run_expr_resolver_error_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void run_expr_resolver_error_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Vector* expected) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (symbol_table != NULL) {
        symbol_table->outer_scope = resolver->symbol_table;
        resolver->symbol_table = symbol_table;
    }
    if (tag_table != NULL) {
        tag_table->outer_scope = resolver->tag_table;
        resolver->tag_table = tag_table;
    }

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_expr(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_resolver(resolver);
}
