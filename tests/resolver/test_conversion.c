#include "./test_conversion.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_resolve_int_and_unsigned_int_conv(void);
void test_resolve_int_and_long_conv(void);
void test_resolve_int_and_unsigned_long_conv(void);
void test_resolve_unsigned_int_and_int_conv(void);
void test_resolve_unsigned_int_and_long_conv(void);
void test_resolve_unsigned_int_and_unsigned_long_conv(void);
void test_resolve_long_and_int_conv(void);
void test_resolve_long_and_unsigned_int_conv(void);
void test_resolve_long_and_unsigned_long_conv(void);
void test_resolve_long_and_long_long_conv(void);
void test_resolve_long_and_unsigned_long_long_conv(void);
void test_resolve_unsigned_long_and_int_conv(void);
void test_resolve_unsigned_long_and_unsigned_int_conv(void);
void test_resolve_unsigned_long_and_long_conv(void);
void test_resolve_unsigned_long_and_long_long_conv(void);
void test_resolve_unsigned_long_and_unsigned_long_long_conv(void);
void test_resolve_long_long_and_unsigned_long_conv(void);
void test_resolve_long_long_and_unsigned_long_long_conv(void);
void test_resolve_unsigned_long_long_and_long_conv(void);
void test_resolve_unsigned_long_long_and_unsigned_long_conv(void);

void run_conv_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected);

CU_Suite* add_test_suite_conv_resolver(void) {
    CU_Suite* suite = CU_add_suite("test_suite_conv_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_int_and_unsigned_int_conv);
    CU_ADD_TEST(suite, test_resolve_int_and_long_conv);
    CU_ADD_TEST(suite, test_resolve_int_and_unsigned_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_int_and_int_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_int_and_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_int_and_unsigned_long_conv);
    CU_ADD_TEST(suite, test_resolve_long_and_int_conv);
    CU_ADD_TEST(suite, test_resolve_long_and_unsigned_int_conv);
    CU_ADD_TEST(suite, test_resolve_long_and_unsigned_long_conv);
    CU_ADD_TEST(suite, test_resolve_long_and_long_long_conv);
    CU_ADD_TEST(suite, test_resolve_long_and_unsigned_long_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_and_int_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_and_unsigned_int_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_and_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_and_long_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_and_unsigned_long_long_conv);
    CU_ADD_TEST(suite, test_resolve_long_long_and_unsigned_long_conv);
    CU_ADD_TEST(suite, test_resolve_long_long_and_unsigned_long_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_long_and_long_conv);
    CU_ADD_TEST(suite, test_resolve_unsigned_long_long_and_unsigned_long_conv);
    return suite;
}

void test_resolve_int_and_unsigned_int_conv(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Srt* expected = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_int_and_long_conv(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_SUB_EXPR, new_integer_dtype(DTYPE_LONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_int_and_unsigned_long_conv(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_int_and_int_conv(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_int_and_long_conv(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_LONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_int_and_unsigned_long_conv(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_INT));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_and_int_conv(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));
    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_and_unsigned_int_conv(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));
    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Srt* expected = new_dtyped_srt(
        SRT_SUB_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_and_unsigned_long_conv(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_and_long_long_conv(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONGLONG));

    Srt* expected = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_LONGLONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_and_unsigned_long_long_conv(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONGLONG));

    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_and_int_conv(void) {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_and_unsigned_int_conv(void) {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_INT));

    Srt* expected = new_dtyped_srt(
        SRT_SUB_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_and_long_conv(void) {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_and_long_long_conv(void) {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONGLONG));

    Srt* expected = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("a"))),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_and_unsigned_long_long_conv(void) {
    Ast* input = new_ast(AST_MOD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONGLONG));

    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_long_and_unsigned_long_conv(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONGLONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2,                             // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("a"))),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_long_long_and_unsigned_long_long_conv(void) {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_LONGLONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONGLONG));

    Srt* expected = new_dtyped_srt(
        SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2,                             // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("a"))),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("b")));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_long_and_long_conv(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONGLONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void test_resolve_unsigned_long_long_and_unsigned_long_conv(void) {
    Ast* input = new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("b")));

    SymbolTable* symbol_table = new_symboltable();
    symboltable_define_memory(symbol_table, new_string("a"), new_integer_dtype(DTYPE_UNSIGNED_LONGLONG));
    symboltable_define_memory(symbol_table, new_string("b"), new_integer_dtype(DTYPE_UNSIGNED_LONG));

    Srt* expected = new_dtyped_srt(
        SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("a")),
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("b"))));

    run_conv_resolver_test(input, symbol_table, NULL, expected);

    delete_srt(expected);
}

void run_conv_resolver_test(Ast* input, SymbolTable* symbol_table, TagTable* tag_table, Srt* expected) {
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

    if (actual != NULL) {
        delete_srt(actual);
    }
    delete_resolver(resolver);
}