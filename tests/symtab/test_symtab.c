#include "./test_symtab.h"
#include "../../src/symtab/symtab.h"

#include <stdlib.h>

void test_symboltable_define_label(void);
void test_symboltable_define_memory(void);
void test_symboltable_define_integer(void);
void test_symboltable_duplicated(void);
void test_symboltable_scoped(void);
void test_symboltable_copy_without_outer(void);
void test_symboltable_copy_with_outer(void);

CU_Suite* add_test_suite_symboltable(void) {
    CU_Suite* suite = CU_add_suite("test_suite_symboltable", NULL, NULL);
    CU_ADD_TEST(suite, test_symboltable_define_label);
    CU_ADD_TEST(suite, test_symboltable_define_memory);
    CU_ADD_TEST(suite, test_symboltable_define_integer);
    CU_ADD_TEST(suite, test_symboltable_duplicated);
    CU_ADD_TEST(suite, test_symboltable_scoped);
    CU_ADD_TEST(suite, test_symboltable_copy_without_outer);
    CU_ADD_TEST(suite, test_symboltable_copy_with_outer);
    return suite;
}

void test_symboltable_define_integer(void) {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);
    CU_ASSERT_PTR_NULL(table->outer_scope);
    CU_ASSERT_PTR_NULL(symboltable_search(table, "IDENTIFIER"));

    char* ident_name = new_string("IDENTIFIER");
    DType* ident_dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* ident_iliteral = new_signed_iliteral(INTEGER_INT, 0);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_integer(table, ident_name, ident_dtype, ident_iliteral);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, ident_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_INT);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
        CU_ASSERT_FALSE(iliteral_isunsigned(symbol->iliteral));
        CU_ASSERT_EQUAL(symbol->iliteral->signed_value, 0);
        CU_ASSERT_EQUAL(symbol->iliteral->unsigned_value, 0ull);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);

    char* symbol_name = new_string("SYMBOL");
    DType* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    IntegerLiteral* symbol_iliteral = new_signed_iliteral(INTEGER_INT, 2);

    CU_ASSERT_TRUE(symboltable_can_define(table, symbol_name));

    symbol = symboltable_define_integer(table, symbol_name, symbol_dtype, symbol_iliteral);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, symbol_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_INT);
        CU_ASSERT_STRING_EQUAL(symbol->name, symbol_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, symbol_dtype));
        CU_ASSERT_FALSE(iliteral_isunsigned(symbol->iliteral));
        CU_ASSERT_EQUAL(symbol->iliteral->signed_value, 2);
        CU_ASSERT_EQUAL(symbol->iliteral->unsigned_value, 0ull);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);

    delete_symboltable(table);
}

void test_symboltable_define_label(void) {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);
    CU_ASSERT_PTR_NULL(table->outer_scope);
    CU_ASSERT_PTR_NULL(symboltable_search(table, "identifier"));

    char* ident_name = new_string("identifier");
    DType* ident_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_label(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, ident_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
        CU_ASSERT_PTR_NULL(symbol->iliteral);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);

    char* symbol_name = new_string("symbol");
    DType* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_TRUE(symboltable_can_define(table, symbol_name));

    symbol = symboltable_define_label(table, symbol_name, symbol_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, symbol_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
        CU_ASSERT_STRING_EQUAL(symbol->name, symbol_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, symbol_dtype));
        CU_ASSERT_PTR_NULL(symbol->iliteral);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);

    delete_symboltable(table);
}

void test_symboltable_define_memory(void) {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    CU_ASSERT_EQUAL(table->memory_nbytes, 0);
    CU_ASSERT_PTR_NULL(table->outer_scope);
    CU_ASSERT_PTR_NULL(symboltable_search(table, "identifier"));

    char* ident_name = new_string("identifier");
    DType* ident_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, ident_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, 4);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
        CU_ASSERT_PTR_NULL(symbol->iliteral);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 4);

    char* symbol_name = new_string("symbol");
    DType* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_TRUE(symboltable_can_define(table, symbol_name));

    symbol = symboltable_define_memory(table, symbol_name, symbol_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, symbol_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, symbol_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, 12);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, symbol_dtype));
        CU_ASSERT_PTR_NULL(symbol->iliteral);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 12);

    delete_symboltable(table);
}

void test_symboltable_duplicated(void) {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    char* ident_name = new_string("identifier");
    DType* ident_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            symbol = symboltable_search(table, ident_name);
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, 4);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
        CU_ASSERT_PTR_NULL(symbol->iliteral);
    }

    CU_ASSERT_EQUAL(table->memory_nbytes, 4);

    char* dup_name = new_string("identifier");
    DType* dup_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);

    CU_ASSERT_FALSE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, dup_name, dup_dtype);
    CU_ASSERT_PTR_NULL(symbol);

    symbol = symboltable_search(table, ident_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, 4);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    free(dup_name);
    delete_dtype(dup_dtype);
    delete_symboltable(table);
}

void test_symboltable_scoped(void) {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    char* outer_name = new_string("outer_scope");
    DType* outer_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, "outer_scope"));
    symboltable_define_label(table, outer_name, outer_dtype);

    char* outer_both_name = new_string("both_scope");
    DType* outer_both_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    CU_ASSERT_TRUE(symboltable_can_define(table, "both_scope"));
    symboltable_define_label(table, outer_both_name, outer_both_dtype);

    table = symboltable_enter_scope(table);

    char* inner_name = new_string("inner_scope");
    DType* inner_dtype = new_integer_dtype(DTYPE_CHAR);

    CU_ASSERT_TRUE(symboltable_can_define(table, "outer_scope"));
    symboltable_define_memory(table, inner_name, inner_dtype);

    char* inner_both_name = new_string("both_scope");
    DType* inner_both_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_TRUE(symboltable_can_define(table, "both_scope"));
    symboltable_define_memory(table, inner_both_name, inner_both_dtype);

    symbol = symboltable_search(table, outer_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    symbol = symboltable_search(table, inner_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, inner_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, 4);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, inner_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    symbol = symboltable_search(table, inner_both_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, inner_both_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, 12);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, inner_both_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    table = symboltable_exit_scope(table);

    symbol = symboltable_search(table, outer_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    symbol = symboltable_search(table, "inner_scope");
    CU_ASSERT_PTR_NULL(symbol);

    symbol = symboltable_search(table, outer_both_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_both_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_both_dtype));
    CU_ASSERT_PTR_NULL(symbol->iliteral);

    delete_symboltable(table);
}

void test_symboltable_copy_without_outer(void) {
    SymbolTable* table = new_symboltable();

    char* ident_name = new_string("identifier");
    DType* ident_dtype = new_integer_dtype(DTYPE_INT);
    symboltable_define_memory(table, ident_name, ident_dtype);

    char* symbol_name = new_string("symbol");
    DType* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    symboltable_define_label(table, symbol_name, symbol_dtype);

    SymbolTable* copied_table = symboltable_copy(table);
    Symbol* copied_symbol = NULL;
    delete_symboltable(table);

    ident_name = new_string("identifier");
    ident_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_FALSE(symboltable_can_define(copied_table, ident_name));

    copied_symbol = symboltable_search(copied_table, ident_name);
    CU_ASSERT_EQUAL(copied_symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(copied_symbol->name, ident_name);
    CU_ASSERT_EQUAL(copied_symbol->memory_offset, 4);
    CU_ASSERT_TRUE(dtype_equals(copied_symbol->dtype, ident_dtype));
    CU_ASSERT_PTR_NULL(copied_symbol->iliteral);

    delete_dtype(ident_dtype);
    free(ident_name);

    symbol_name = new_string("symbol");
    symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(symboltable_can_define(copied_table, symbol_name));

    copied_symbol = symboltable_search(copied_table, symbol_name);
    CU_ASSERT_EQUAL(copied_symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(copied_symbol->name, symbol_name);
    CU_ASSERT_EQUAL(copied_symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(copied_symbol->dtype, symbol_dtype));
    CU_ASSERT_PTR_NULL(copied_symbol->iliteral);

    delete_dtype(symbol_dtype);
    free(symbol_name);

    CU_ASSERT_EQUAL(copied_table->memory_nbytes, 4);
    CU_ASSERT_PTR_NULL(copied_table->outer_scope);

    delete_symboltable(copied_table);
}

void test_symboltable_copy_with_outer(void) {
    SymbolTable* table = new_symboltable();

    char* outer_name = new_string("outer_scope");
    DType* outer_dtype = new_integer_dtype(DTYPE_INT);
    symboltable_define_memory(table, outer_name, outer_dtype);

    table = symboltable_enter_scope(table);

    char* inner_name = new_string("inner_scope");
    DType* inner_dtype = new_integer_dtype(DTYPE_CHAR);
    symboltable_define_memory(table, inner_name, inner_dtype);

    SymbolTable* copied_table = symboltable_copy(table);
    Symbol* copied_symbol = NULL;
    delete_symboltable(table);

    outer_name = new_string("outer_scope");
    outer_dtype = new_integer_dtype(DTYPE_INT);
    copied_symbol = symboltable_search(copied_table, outer_name);

    CU_ASSERT_EQUAL(copied_symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(copied_symbol->name, outer_name);
    CU_ASSERT_EQUAL(copied_symbol->memory_offset, 4);
    CU_ASSERT_TRUE(dtype_equals(copied_symbol->dtype, outer_dtype));
    CU_ASSERT_PTR_NULL(copied_symbol->iliteral);

    delete_dtype(outer_dtype);
    free(outer_name);

    inner_name = new_string("inner_scope");
    inner_dtype = new_integer_dtype(DTYPE_CHAR);
    copied_symbol = symboltable_search(copied_table, inner_name);

    CU_ASSERT_EQUAL(copied_symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(copied_symbol->name, inner_name);
    CU_ASSERT_EQUAL(copied_symbol->memory_offset, 8);
    CU_ASSERT_TRUE(dtype_equals(copied_symbol->dtype, inner_dtype));
    CU_ASSERT_PTR_NULL(copied_symbol->iliteral);

    delete_dtype(inner_dtype);
    free(inner_name);

    copied_table = symboltable_exit_scope(copied_table);

    outer_name = new_string("outer_scope");
    outer_dtype = new_integer_dtype(DTYPE_INT);
    copied_symbol = symboltable_search(copied_table, outer_name);

    CU_ASSERT_EQUAL(copied_symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(copied_symbol->name, outer_name);
    CU_ASSERT_EQUAL(copied_symbol->memory_offset, 4);
    CU_ASSERT_TRUE(dtype_equals(copied_symbol->dtype, outer_dtype));
    CU_ASSERT_PTR_NULL(copied_symbol->iliteral);

    delete_dtype(outer_dtype);
    free(outer_name);

    inner_name = new_string("inner_scope");
    copied_symbol = symboltable_search(copied_table, inner_name);

    CU_ASSERT_PTR_NULL(copied_symbol);

    free(inner_name);

    delete_symboltable(copied_table);
}
