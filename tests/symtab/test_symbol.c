#include "./test_symbol.h"
#include "../../src/symtab/symbol.h"

void test_new_memory_symbol(void);
void test_new_label_symbol(void);

CU_Suite* add_test_suite_symbol(void) {
    CU_Suite* suite = CU_add_suite("test_suite_symbol", NULL, NULL);
    CU_ADD_TEST(suite, test_new_memory_symbol);
    CU_ADD_TEST(suite, test_new_label_symbol);
    return suite;
}

void test_new_memory_symbol(void) {
    char* symbol_name = new_string("abc");
    DType* symbol_dtype = new_integer_dtype(DTYPE_CHAR);
    Symbol* symbol = new_memory_symbol(symbol_name, symbol_dtype, 1);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Symbol* copied_symbol = symbol_copy(symbol);
            delete_symbol(symbol);
            symbol_name = copied_symbol->name;
            symbol_dtype = copied_symbol->dtype;
            symbol = copied_symbol;
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_MEM);
        CU_ASSERT_STRING_EQUAL(symbol->name, "abc");
        CU_ASSERT_PTR_EQUAL(symbol->dtype, symbol_dtype);
        CU_ASSERT_EQUAL(symbol->memory_offset, 1);
    }

    delete_symbol(symbol);
}

void test_new_label_symbol(void) {
    char* symbol_name = new_string("xyz");
    DType* symbol_dtype = new_integer_dtype(DTYPE_INT);
    Symbol* symbol = new_label_symbol(symbol_name, symbol_dtype);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Symbol* copied_symbol = symbol_copy(symbol);
            delete_symbol(symbol);
            symbol_name = copied_symbol->name;
            symbol_dtype = copied_symbol->dtype;
            symbol = copied_symbol;
        }
        CU_ASSERT_EQUAL(symbol->type, SYMBOL_LABEL);
        CU_ASSERT_STRING_EQUAL(symbol->name, "xyz");
        CU_ASSERT_PTR_EQUAL(symbol->dtype, symbol_dtype);
        CU_ASSERT_EQUAL(symbol->memory_offset, -1);
    }

    delete_symbol(symbol);
}
