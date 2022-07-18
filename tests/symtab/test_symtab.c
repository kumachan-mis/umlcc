#include "./test_symtab.h"
#include "../../src/symtab/symtab.h"

#include <stdlib.h>

void test_symboltable_define_memory();
void test_symboltable_define_label();
void test_symboltable_duplicated();
void test_symboltable_scoped();

CU_Suite* add_test_suite_symboltable() {
    CU_Suite* suite = CU_add_suite("test_suite_symboltable", NULL, NULL);
    CU_add_test(suite, "test_symboltable_define_memory", test_symboltable_define_memory);
    CU_add_test(suite, "test_symboltable_define_label", test_symboltable_define_label);
    CU_add_test(suite, "test_symboltable_duplicated", test_symboltable_duplicated);
    CU_add_test(suite, "test_symboltable_scoped", test_symboltable_scoped);
    return suite;
}

void test_symboltable_define_memory() {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    CU_ASSERT_EQUAL(table->memory_size, 0);
    CU_ASSERT_PTR_NULL(table->outer_scope);
    CU_ASSERT_PTR_NULL(symboltable_search(table, "identifier"));

    char* ident_name = new_string("identifier");
    Dtype* ident_dtype = new_integer_dtype(DTYPE_INT);
    int ident_dtype_size = dtype_size(ident_dtype);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) symbol = symboltable_search(table, ident_name);
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, ident_dtype_size);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
    }

    CU_ASSERT_EQUAL(table->memory_size, ident_dtype_size);

    char* symbol_name = new_string("symbol");
    Dtype* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    int symbol_dtype_size = dtype_size(symbol_dtype);

    CU_ASSERT_TRUE(symboltable_can_define(table, symbol_name));

    symbol = symboltable_define_memory(table, symbol_name, symbol_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) symbol = symboltable_search(table, symbol_name);
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, symbol_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, ident_dtype_size + symbol_dtype_size);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, symbol_dtype));
    }

    CU_ASSERT_EQUAL(table->memory_size, ident_dtype_size + symbol_dtype_size);

    delete_symboltable(table);
}

void test_symboltable_define_label() {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    CU_ASSERT_EQUAL(table->memory_size, 0);
    CU_ASSERT_PTR_NULL(table->outer_scope);
    CU_ASSERT_PTR_NULL(symboltable_search(table, "identifier"));

    char* ident_name = new_string("identifier");
    Dtype* ident_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_label(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) symbol = symboltable_search(table, ident_name);
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
    }

    CU_ASSERT_EQUAL(table->memory_size, 0);

    char* symbol_name = new_string("symbol");
    Dtype* symbol_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_TRUE(symboltable_can_define(table, symbol_name));

    symbol = symboltable_define_label(table, symbol_name, symbol_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) symbol = symboltable_search(table, symbol_name);
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
        CU_ASSERT_STRING_EQUAL(symbol->name, symbol_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, symbol_dtype));
    }

    CU_ASSERT_EQUAL(table->memory_size, 0);

    delete_symboltable(table);
}

void test_symboltable_duplicated() {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    char* ident_name = new_string("identifier");
    Dtype* ident_dtype = new_integer_dtype(DTYPE_INT);
    int ident_dtype_size = dtype_size(ident_dtype);

    CU_ASSERT_TRUE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, ident_name, ident_dtype);
    for (int i = 0; i < 2; i++) {
        if (i > 0) symbol = symboltable_search(table, ident_name);
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
        CU_ASSERT_EQUAL(symbol->memory_offset, ident_dtype_size);
        CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));
    }

    CU_ASSERT_EQUAL(table->memory_size, ident_dtype_size);

    char* dup_name = new_string("identifier");
    Dtype* dup_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);

    CU_ASSERT_FALSE(symboltable_can_define(table, ident_name));

    symbol = symboltable_define_memory(table, dup_name, dup_dtype);
    CU_ASSERT_PTR_NULL(symbol);

    symbol = symboltable_search(table, ident_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, ident_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, ident_dtype_size);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, ident_dtype));

    free(dup_name);
    delete_dtype(dup_dtype);
    delete_symboltable(table);
}

void test_symboltable_scoped() {
    SymbolTable* table = new_symboltable();
    Symbol* symbol = NULL;

    char* outer_name = new_string("outer_scope");
    Dtype* outer_dtype = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(symboltable_can_define(table, outer_name));
    symboltable_define_label(table, outer_name, outer_dtype);

    char* outer_both_name = new_string("both_scope");
    Dtype* outer_both_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    CU_ASSERT_TRUE(symboltable_can_define(table, outer_both_name));
    symboltable_define_label(table, outer_both_name, outer_both_dtype);

    table = symboltable_enter_scope(table);

    char* inner_name = new_string("inner_scope");
    Dtype* inner_dtype = new_integer_dtype(DTYPE_CHAR);
    int inner_dtype_size = dtype_size(inner_dtype);

    CU_ASSERT_TRUE(symboltable_can_define(table, inner_name));
    symboltable_define_memory(table, inner_name, inner_dtype);

    char* inner_both_name = new_string("both_scope");
    Dtype* inner_both_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    int inner_both_dtype_size = dtype_size(inner_both_dtype);

    CU_ASSERT_TRUE(symboltable_can_define(table, inner_both_name));
    symboltable_define_memory(table, inner_both_name, inner_both_dtype);

    symbol = symboltable_search(table, outer_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_dtype));

    symbol = symboltable_search(table, inner_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, inner_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, inner_dtype_size);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, inner_dtype));

    symbol = symboltable_search(table, inner_both_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
    CU_ASSERT_STRING_EQUAL(symbol->name, inner_both_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, inner_dtype_size + inner_both_dtype_size);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, inner_both_dtype));

    table = symboltable_exit_scope(table);

    symbol = symboltable_search(table, outer_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_dtype));

    symbol = symboltable_search(table, inner_name);
    CU_ASSERT_PTR_NULL(symbol);

    symbol = symboltable_search(table, outer_both_name);
    CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
    CU_ASSERT_STRING_EQUAL(symbol->name, outer_both_name);
    CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    CU_ASSERT_TRUE(dtype_equals(symbol->dtype, outer_both_dtype));

    delete_symboltable(table);
}
