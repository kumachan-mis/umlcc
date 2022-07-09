#include "./test_ast.h"
#include "../../src/ast/ast.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_new_ast();
void test_new_identifier_ast_ident();
void test_new_identifier_ast_typedefname();
void test_new_integer_ast_int();
void test_new_integer_ast_char();
void test_new_sliteral_ast_empty();
void test_new_sliteral_ast_without_null();
void test_new_sliteral_ast_with_null();

CU_Suite* add_test_suite_ast() {
    CU_Suite* suite = CU_add_suite("test_suite_ast", NULL, NULL);
    CU_add_test(suite, "test_new_ast", test_new_ast);
    CU_add_test(suite, "test_new_identifier_ast_ident", test_new_identifier_ast_ident);
    CU_add_test(suite, "test_new_identifier_ast_typedefname", test_new_identifier_ast_typedefname);
    CU_add_test(suite, "test_new_integer_ast_int", test_new_integer_ast_int);
    CU_add_test(suite, "test_new_integer_ast_char", test_new_integer_ast_char);
    CU_add_test(suite, "test_new_sliteral_ast_empty", test_new_sliteral_ast_empty);
    CU_add_test(suite, "test_new_sliteral_ast_without_null", test_new_sliteral_ast_without_null);
    CU_add_test(suite, "test_new_sliteral_ast_with_null", test_new_sliteral_ast_with_null);
    return suite;
}

void test_new_ast() {
    Ast* ast = new_ast(AST_TRAS_UNIT, 2, new_ast(AST_DECL, 0), new_ast(AST_FUNC_DEF, 0));
    Ast* child = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_TRAS_UNIT);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 2);

        child = vector_at(ast->children, 0);
        CU_ASSERT_EQUAL(child->type, AST_DECL);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, -1);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);

        child = vector_at(ast->children, 1);
        CU_ASSERT_EQUAL(child->type, AST_FUNC_DEF);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, -1);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);
    }

    delete_ast(ast);
}

void test_new_identifier_ast_ident() {
    Ast* ast = new_identifier_ast(AST_IDENT_EXPR, new_string("ident"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_IDENT_EXPR);
        CU_ASSERT_STRING_EQUAL(ast->ident_name, "ident");
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_identifier_ast_typedefname() {
    Ast* ast = new_identifier_ast(AST_TYPEDEF_NAME, new_string("typedef-name"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_TYPEDEF_NAME);
        CU_ASSERT_STRING_EQUAL(ast->ident_name, "typedef-name");
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_integer_ast_int() {
    Ast* ast = new_integer_ast(AST_INT_EXPR, 6);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_INT_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, 6);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_integer_ast_char() {
    Ast* ast = new_integer_ast(AST_CHAR_EXPR, 89);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_CHAR_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, 89);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_sliteral_ast_empty() {
    char* sliteral_value = malloc(1 * sizeof(char));
    memcpy(sliteral_value, "", 1 * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, 1);
    Ast* ast = new_sliteral_ast(AST_STRING_EXPR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_STRING_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(ast->sliteral->value, "", 1), 0);
        CU_ASSERT_EQUAL(ast->sliteral->size, 1);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_sliteral_ast_without_null() {
    char* sliteral_value = malloc(15 * sizeof(char));
    memcpy(sliteral_value, "string literal", 15 * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, 15);
    Ast* ast = new_sliteral_ast(AST_STRING_EXPR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_STRING_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(ast->sliteral->value, "string literal", 15), 0);
        CU_ASSERT_EQUAL(ast->sliteral->size, 15);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_sliteral_ast_with_null() {
    char* sliteral_value = malloc(15 * sizeof(char));
    memcpy(sliteral_value, "string\0literal", 15 * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, 15);
    Ast* ast = new_sliteral_ast(AST_STRING_EXPR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_STRING_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(ast->sliteral->value, "string\0literal", 15), 0);
        CU_ASSERT_EQUAL(ast->sliteral->size, 15);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}
