#include "./test_expression.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_assignment_expr(void);
void test_resolve_conditional_expr_arithmetic(void);
void test_resolve_conditional_expr_pointer(void);
void test_resolve_conditional_expr_struct(void);
void test_resolve_logical_or_expr(void);
void test_resolve_logical_and_expr(void);
void test_resolve_bitwise_inclusive_or_expr(void);
void test_resolve_bitwise_exclusive_or_expr(void);
void test_resolve_bitwise_and_expr(void);
void test_resolve_equal_expr(void);
void test_resolve_not_equal_expr(void);
void test_resolve_less_expr(void);
void test_resolve_greater_expr(void);
void test_resolve_less_equal_expr(void);
void test_resolve_greater_equal_expr(void);
void test_resolve_add_expr(void);
void test_resolve_subtract_expr(void);
void test_resolve_pointer_add_expr(void);
void test_resolve_reversed_pointer_add_expr(void);
void test_resolve_pointer_subtract_expr(void);
void test_resolve_pointer_subtract_expr(void);
void test_resolve_pointer_difference_expr(void);
void test_resolve_multiply_expr(void);
void test_resolve_division_expr(void);
void test_resolve_modulo_expr(void);
void test_resolve_cast_expr(void);
void test_resolve_address_expr(void);
void test_resolve_indirection_expr(void);
void test_resolve_positive_expr(void);
void test_resolve_negative_expr(void);
void test_resolve_bitwise_not_expr(void);
void test_resolve_logical_not_expr(void);
void test_resolve_sizeof_expr_typename(void);
void test_resolve_sizeof_expr_expr(void);
void test_resolve_call_expr(void);
void test_resolve_subscription_expr(void);
void test_resolve_reversed_subscription_expr(void);
void test_resolve_member_expr(void);
void test_resolve_tomember_expr(void);
void test_resolve_ident_expr_local(void);
void test_resolve_ident_expr_global(void);
void test_resolve_enum_ident_expr_local(void);
void test_resolve_enum_ident_expr_global(void);
void test_resolve_iliteral_expr_char(void);
void test_resolve_iliteral_expr_int(void);
void test_resolve_iliteral_expr_unsigned_int(void);
void test_resolve_iliteral_expr_long(void);
void test_resolve_iliteral_expr_unsigned_long(void);
void test_resolve_iliteral_expr_long_long(void);
void test_resolve_iliteral_expr_unsigned_long_long(void);
void test_resolve_sliteral_expr(void);

void run_global_expr_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                   Srt* expected_trans_unit);
void run_local_expr_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                  Srt* expected_trans_unit);

CU_Suite* add_test_suite_expr_resolver(void) {
    CU_Suite* suite = CU_add_suite("test_suite_expr_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_assignment_expr);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_arithmetic);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_pointer);
    CU_ADD_TEST(suite, test_resolve_conditional_expr_struct);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr);
    CU_ADD_TEST(suite, test_resolve_bitwise_inclusive_or_expr);
    CU_ADD_TEST(suite, test_resolve_bitwise_exclusive_or_expr);
    CU_ADD_TEST(suite, test_resolve_bitwise_and_expr);
    CU_ADD_TEST(suite, test_resolve_equal_expr);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr);
    CU_ADD_TEST(suite, test_resolve_less_expr);
    CU_ADD_TEST(suite, test_resolve_greater_expr);
    CU_ADD_TEST(suite, test_resolve_less_equal_expr);
    CU_ADD_TEST(suite, test_resolve_greater_equal_expr);
    CU_ADD_TEST(suite, test_resolve_add_expr);
    CU_ADD_TEST(suite, test_resolve_subtract_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_add_expr);
    CU_ADD_TEST(suite, test_resolve_reversed_pointer_add_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_subtract_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_difference_expr);
    CU_ADD_TEST(suite, test_resolve_multiply_expr);
    CU_ADD_TEST(suite, test_resolve_division_expr);
    CU_ADD_TEST(suite, test_resolve_modulo_expr);
    CU_ADD_TEST(suite, test_resolve_cast_expr);
    CU_ADD_TEST(suite, test_resolve_address_expr);
    CU_ADD_TEST(suite, test_resolve_indirection_expr);
    CU_ADD_TEST(suite, test_resolve_positive_expr);
    CU_ADD_TEST(suite, test_resolve_negative_expr);
    CU_ADD_TEST(suite, test_resolve_bitwise_not_expr);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_typename);
    CU_ADD_TEST(suite, test_resolve_sizeof_expr_expr);
    CU_ADD_TEST(suite, test_resolve_call_expr);
    CU_ADD_TEST(suite, test_resolve_subscription_expr);
    CU_ADD_TEST(suite, test_resolve_reversed_subscription_expr);
    CU_ADD_TEST(suite, test_resolve_member_expr);
    CU_ADD_TEST(suite, test_resolve_tomember_expr);
    CU_ADD_TEST(suite, test_resolve_ident_expr_local);
    CU_ADD_TEST(suite, test_resolve_ident_expr_global);
    CU_ADD_TEST(suite, test_resolve_enum_ident_expr_local);
    CU_ADD_TEST(suite, test_resolve_enum_ident_expr_global);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_char);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_int);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_unsigned_int);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_long);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_unsigned_long);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_long_long);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_unsigned_long_long);
    CU_ADD_TEST(suite, test_resolve_sliteral_expr);
    return suite;
}

void test_resolve_assignment_expr(void) {
    Ast* input = new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                         new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                         new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    SymbolTable* local_table = new_symboltable();
    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("x"), pointer_dtype);
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_CHAR), 2,                                 // non-terminal
        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 1, // non-terminal
                       new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_CHAR),
                                      1, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(pointer_dtype), new_string("x")))),
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_or_expr(void) {
    Ast* input = new_ast(AST_LOR_EXPR, 2,                           // non-terminal
                         new_ast(AST_LOR_EXPR, 2,                   // non-terminal
                                 new_ast(AST_LOR_EXPR, 2,           // non-terminal
                                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                                 new_identifier_ast(AST_IDENT_EXPR, new_string("m")))),
                                 new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)))),
                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("n"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("j"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("m"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_dtyped_srt(
                SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_dtyped_srt(
                    SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
                new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                               new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                              new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                                                 new_string("m"))))),
            new_dtyped_srt(
                SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))),
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("n")))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_conditional_expr_arithmetic(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3,            // non-terminal
                         new_ast(AST_GREATER_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ul")),
                         new_ast(AST_COND_EXPR, 3,         // non-terminal
                                 new_ast(AST_LESS_EXPR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("c")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("c"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("ul"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_COND_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 3, // non-terminal
        new_dtyped_srt(
            SRT_GREATER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("ul")),
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
            new_dtyped_srt(
                SRT_COND_EXPR, new_integer_dtype(DTYPE_INT), 3, // non-terminal
                new_dtyped_srt(
                    SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("c"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_conditional_expr_pointer(void) {
    Ast* input = new_ast(AST_COND_EXPR, 3, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_COND_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 3, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p")),
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p")),
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("q")));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_conditional_expr_struct(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2,
                         new_ast(AST_COND_EXPR, 3, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("should_use_s")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("s")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("t"))),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")));

    DType* struct_dtype = new_named_struct_dtype(new_string("X"), 12, 12);
    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("should_use_s"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("s"), dtype_copy(struct_dtype));
    symboltable_define_memory(local_table, new_string("t"), dtype_copy(struct_dtype));

    TagTable* tag_table = new_tagtable();
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("a"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("b"), new_integer_dtype(DTYPE_INT)));
    vector_push(members, new_dstructmember(new_string("c"), new_integer_dtype(DTYPE_INT)));
    tagtable_define_struct(tag_table, new_string("X"), members);

    Srt* expected = new_dtyped_srt(
        SRT_TOMEMBER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_ADDR_EXPR, new_pointer_dtype(struct_dtype), 1,
            new_dtyped_srt(SRT_COND_EXPR, dtype_copy(struct_dtype), 3, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("should_use_s")),
                           new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(struct_dtype), new_string("s")),
                           new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(struct_dtype), new_string("t")))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("a")));

    run_local_expr_resolver_test(input, local_table, tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_and_expr(void) {
    Ast* input = new_ast(AST_LAND_EXPR, 2,       // non-terminal
                         new_ast(AST_OR_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("check1")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("check2"))),
                         new_ast(AST_CALL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("validate")),
                                 new_ast(AST_ARG_LIST, 1, // non-terminal
                                         new_identifier_ast(AST_IDENT_EXPR, new_string("object")))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("check1"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("check2"), new_integer_dtype(DTYPE_CHAR));

    DType* arg_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6);
    symboltable_define_memory(local_table, new_string("object"), arg_dtype);

    Vector* params = new_vector(&t_dparam);
    DType* param_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    vector_push(params, new_named_dparam(new_string("target"), param_dtype));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("validate"), func_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_LAND_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_OR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("check1")),
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("check2")))),
        new_dtyped_srt(
            SRT_CALL_EXPR, new_integer_dtype(DTYPE_INT), 2,                             // non-terminal
            new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(func_dtype)), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(func_dtype), new_string("validate"))),
            new_srt(SRT_ARG_LIST, 1,                                          // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, dtype_copy(param_dtype), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(arg_dtype), new_string("object"))))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_bitwise_inclusive_or_expr(void) {
    Ast* input = new_ast(AST_OR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_ast(AST_XOR_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_INT));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_OR_EXPR, new_integer_dtype(DTYPE_INT), 2,                  // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))),
        new_dtyped_srt(SRT_XOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y")),
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("z"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_bitwise_exclusive_or_expr(void) {
    Ast* input = new_ast(AST_XOR_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_ast(AST_AND_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("z"), new_integer_dtype(DTYPE_INT));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_XOR_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))),
        new_dtyped_srt(SRT_AND_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y")),
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("z"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_bitwise_and_expr(void) {
    Ast* input = new_ast(AST_AND_EXPR, 2,           // non-terminal
                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_AND_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1))),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("y"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_equal_expr(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2,               // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("y"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_not_equal_expr(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2,        // non-terminal
                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("zero_flag")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("zero_flag"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("zero_flag")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_less_expr(void) {
    Ast* input = new_ast(AST_LESS_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("m")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("m"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("m"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_greater_expr(void) {
    Ast* input = new_ast(AST_GREATER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("j"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_GREATER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_less_equal_expr(void) {
    Ast* input = new_ast(AST_LESSEQ_EXPR, 2,       // non-terminal
                         new_ast(AST_LESS_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("m"))),
                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("j"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("m"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected =
        new_dtyped_srt(SRT_LESSEQ_EXPR, new_integer_dtype(DTYPE_INT), 2,              // non-terminal
                       new_dtyped_srt(SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                                      new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                                     new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                                                        new_string("m")))),
                       new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 0))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_greater_equal_expr(void) {
    Ast* input = new_ast(AST_GREATEREQ_EXPR, 2,       // non-terminal
                         new_ast(AST_GREATER_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("n"))),
                         new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("j"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected =
        new_dtyped_srt(SRT_GREATEREQ_EXPR, new_integer_dtype(DTYPE_INT), 2,              // non-terminal
                       new_dtyped_srt(SRT_GREATER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
                                      new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                                     new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                                                        new_string("n")))),
                       new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 0))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_add_expr(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("n")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("n"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 3)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_subtract_expr(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2,         // non-terminal
                         new_ast(AST_ADD_EXPR, 2, // non-terminal
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)));

    Srt* expected = new_dtyped_srt(
        SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 3)),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 4))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 6)));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_add_expr(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_reversed_pointer_add_expr(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    // In current implementation,
    // if a pointer is placed on the right side of add operator, the operands will be swaped
    Srt* expected = new_dtyped_srt(
        SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_subtract_expr(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_PSUB_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 4)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_difference_expr(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"), new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Srt* expected = new_dtyped_srt(
        SRT_PDIFF_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), new_string("p")),
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), new_string("q")));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_multiply_expr(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("n")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 4)),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("n")));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_division_expr(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("amount")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("amount"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("amount"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_modulo_expr(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2,         // non-terminal
                         new_ast(AST_MUL_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("value")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9))),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("value"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 9))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 5)));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_cast_expr(void) {
    Ast* input = new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_VOID, 0)),
                                 new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                         new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                 new_ast(AST_ABS_DECLOR, 0),
                                                 new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                         new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                 new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                         new_ast(AST_TYPE_VOID, 0)),
                                                                 new_ast(AST_ABS_DECLOR, 0)))))),
                         new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                                 new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                         new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                 new_ast(AST_TYPE_INT, 0)),
                                         new_ast(AST_ABS_DECLOR, 0)),
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("f"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("f"), new_integer_dtype(DTYPE_CHAR));

    DType* cast_dtype = new_pointer_dtype(new_function_dtype(new_vector(&t_dparam), new_void_dtype()));

    Srt* expected = new_dtyped_srt(
        SRT_CAST_EXPR, cast_dtype, 1,                                  // non-terminal,
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("f"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_address_expr(void) {
    Ast* input = new_ast(AST_ADDR_EXPR, 1,          // non-terminal
                         new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("a"), array_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_ADDR_EXPR, pointer_dtype, 1, // non-terminal
        new_dtyped_srt(
            SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_dtyped_srt(
                SRT_PADD_EXPR, dtype_copy(pointer_dtype), 2,                // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, dtype_copy(pointer_dtype), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(array_dtype), new_string("a"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1)))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_indirection_expr(void) {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"), new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("ptr")));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_positive_expr(void) {
    Ast* input = new_ast(AST_POS_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_POS_EXPR, new_integer_dtype(DTYPE_INT), 1,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_negative_expr(void) {
    Ast* input = new_ast(AST_NEG_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_NEG_EXPR, new_integer_dtype(DTYPE_INT), 1,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_bitwise_not_expr(void) {
    Ast* input = new_ast(AST_NOT_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_NOT_EXPR, new_integer_dtype(DTYPE_INT), 1,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_not_expr(void) {
    Ast* input = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                         new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("flag"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1,                // non-terminal
        new_dtyped_srt(SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("flag"))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sizeof_expr_typename(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                         new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                 new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                         new_ast(AST_ABS_DECLOR, 0))));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 8));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sizeof_expr_expr(void) {
    Ast* input = new_ast(AST_SIZEOF_EXPR, 1, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 4));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_call_expr(void) {
    Ast* input = new_ast(AST_CALL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                         new_ast(AST_ARG_LIST, 2, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                 new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_named_dparam(new_string("y"), new_integer_dtype(DTYPE_INT)));
    DType* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("a"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_CALL_EXPR, new_integer_dtype(DTYPE_INT), 2,                             // non-terminal
        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(func_dtype)), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(func_dtype), new_string("function"))),
        new_srt(SRT_ARG_LIST, 2,                                               // non-terminal
                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("a"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 3))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_subscription_expr(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("array")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    symboltable_define_memory(local_table, new_string("array"), array_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
        new_dtyped_srt(
            SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 2,                // non-terminal
            new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(array_dtype), new_string("array"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_reversed_subscription_expr(void) {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("array")));

    SymbolTable* local_table = new_symboltable();
    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 5);
    symboltable_define_memory(local_table, new_string("array"), array_dtype);

    // In current implementation,
    // if a pointer is placed on the right side of subscription operator, the operands will be swaped
    Srt* expected = new_dtyped_srt(
        SRT_INDIR_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
        new_dtyped_srt(
            SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 2,                // non-terminal
            new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(array_dtype), new_string("array"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_member_expr(void) {
    Ast* input = new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), named_struct);
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Srt* expected = new_dtyped_srt(
        SRT_TOMEMBER_EXPR, new_integer_dtype(DTYPE_INT), 2,                           // non-terminal
        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(named_struct)), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(named_struct), new_string("structure"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("member")));

    run_local_expr_resolver_test(input, local_table, local_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_tomember_expr(void) {
    Ast* input = new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("member")));

    DType* named_struct = new_named_struct_dtype(new_string("Struct"), 4, 4);
    Vector* members = new_vector(&t_dstructmember);
    vector_push(members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("structure"), new_pointer_dtype(named_struct));
    TagTable* local_tag_table = new_tagtable();
    tagtable_define_struct(local_tag_table, new_string("Struct"), members);

    Srt* expected = new_dtyped_srt(
        SRT_TOMEMBER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(dtype_copy(named_struct)), new_string("structure")),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("member")));

    run_local_expr_resolver_test(input, local_table, local_tag_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_ident_expr_local(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("local"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("local"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("local"));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_ident_expr_global(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("global"));

    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("global"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("global"));

    run_global_expr_resolver_test(input, global_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enum_ident_expr_local(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("LOCAL_MEMBER"));

    SymbolTable* local_table = new_symboltable();
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 4);
    symboltable_define_integer(local_table, new_string("LOCAL_MEMBER"), new_integer_dtype(DTYPE_INT), iliteral);

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 4));

    run_local_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_enum_ident_expr_global(void) {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("GLOBAL_MEMBER"));

    SymbolTable* global_table = new_symboltable();
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 1);
    symboltable_define_integer(global_table, new_string("GLOBAL_MEMBER"), new_integer_dtype(DTYPE_INT), iliteral);

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1));

    run_global_expr_resolver_test(input, global_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_char(void) {
    Ast* input = new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89));

    Srt* expected = new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 89));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_int(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 3));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_unsigned_int(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 16u));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT),
                                     new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 16u));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_long(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONG, 4l));

    Srt* expected =
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONG), new_signed_iliteral(INTEGER_LONG, 4l));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_unsigned_long(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 9ul));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG),
                                     new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 9ul));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_long_long(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONGLONG, 8ll));

    Srt* expected =
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_signed_iliteral(INTEGER_LONGLONG, 8ll));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_unsigned_long_long(void) {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 20ull));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG),
                                     new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 20ull));

    run_local_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_expr(void) {
    Ast* input = new_sliteral_ast(AST_STRING_EXPR, new_sliteral(new_string("Hello"), 6));

    Srt* expected =
        new_sliteral_identifier_srt(SRT_STRIDENT_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), 0);

    Srt* expected_trans_unit = new_srt(
        SRT_TRAS_UNIT, 1,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_sliteral_identifier_srt(SRT_STRDECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), 0),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6),
                                                 new_sliteral(new_string("Hello"), 6))))));

    run_local_expr_resolver_test(input, NULL, NULL, expected, expected_trans_unit);

    delete_srt(expected);
    delete_srt(expected_trans_unit);
}

void run_global_expr_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                   Srt* expected_trans_unit) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (symbol_table != NULL) {
        delete_symboltable(resolver->symbol_table);
        resolver->symbol_table = symbol_table;
    }
    if (tag_table != NULL) {
        delete_tagtable(resolver->tag_table);
        resolver->tag_table = tag_table;
    }

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_expr(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);
    if (expected_trans_unit != NULL) {
        testlib_assert_srt_equal(resolver->trans_unit_srt, expected_trans_unit);
    }

    if (actual != NULL) {
        delete_srt(actual);
    }
    delete_resolver(resolver);
}

void run_local_expr_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected,
                                  Srt* expected_trans_unit) {
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

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_expr(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);
    if (expected_trans_unit != NULL) {
        testlib_assert_srt_equal(resolver->trans_unit_srt, expected_trans_unit);
    }

    if (actual != NULL) {
        delete_srt(actual);
    }
    delete_resolver(resolver);
}
