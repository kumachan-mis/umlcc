#include "./test_ast.h"
#include "../../src/ast/ast.h"

#include <stdlib.h>

void test_new_ast(void);
void test_new_identifier_ast_ident(void);
void test_new_identifier_ast_typedefname(void);
void test_new_iliteral_ast_int(void);
void test_new_iliteral_ast_char(void);
void test_new_sliteral_ast(void);

CU_Suite* add_test_suite_ast(void) {
    CU_Suite* suite = CU_add_suite("test_suite_ast", NULL, NULL);
    CU_ADD_TEST(suite, test_new_ast);
    CU_ADD_TEST(suite, test_new_identifier_ast_ident);
    CU_ADD_TEST(suite, test_new_identifier_ast_typedefname);
    CU_ADD_TEST(suite, test_new_iliteral_ast_int);
    CU_ADD_TEST(suite, test_new_iliteral_ast_char);
    CU_ADD_TEST(suite, test_new_sliteral_ast);
    return suite;
}

void test_new_ast(void) {
    Ast* ast = new_ast(AST_TRAS_UNIT, 2, new_ast(AST_DECL, 0), new_ast(AST_FUNC_DEF, 0));
    Ast* child_ast = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_TRAS_UNIT);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_PTR_NULL(ast->iliteral);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 2);

        child_ast = vector_at(ast->children, 0);
        CU_ASSERT_EQUAL(child_ast->type, AST_DECL);
        CU_ASSERT_PTR_NULL(child_ast->ident_name);
        CU_ASSERT_PTR_NULL(child_ast->iliteral);
        CU_ASSERT_PTR_NULL(child_ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_ast->children), 0);

        child_ast = vector_at(ast->children, 1);
        CU_ASSERT_EQUAL(child_ast->type, AST_FUNC_DEF);
        CU_ASSERT_PTR_NULL(child_ast->ident_name);
        CU_ASSERT_PTR_NULL(child_ast->iliteral);
        CU_ASSERT_PTR_NULL(child_ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_identifier_ast_ident(void) {
    Ast* ast = new_identifier_ast(AST_IDENT_EXPR, new_string("ident"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_IDENT_EXPR);
        CU_ASSERT_STRING_EQUAL(ast->ident_name, "ident");
        CU_ASSERT_PTR_NULL(ast->iliteral);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_identifier_ast_typedefname(void) {
    Ast* ast = new_identifier_ast(AST_TYPEDEF_NAME, new_string("typedef-name"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_TYPEDEF_NAME);
        CU_ASSERT_STRING_EQUAL(ast->ident_name, "typedef-name");
        CU_ASSERT_PTR_NULL(ast->iliteral);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_iliteral_ast_int(void) {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 6);
    Ast* ast = new_iliteral_ast(AST_INT_EXPR, iliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_INT_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(iliteral_isunsigned(ast->iliteral));
        CU_ASSERT_EQUAL(ast->iliteral->signed_value, 6);
        CU_ASSERT_EQUAL(ast->iliteral->unsigned_value, 0ull);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_iliteral_ast_char(void) {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 89);
    Ast* ast = new_iliteral_ast(AST_CHAR_EXPR, iliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_CHAR_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_EQUAL(ast->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(iliteral_isunsigned(ast->iliteral));
        CU_ASSERT_EQUAL(ast->iliteral->signed_value, 89);
        CU_ASSERT_EQUAL(ast->iliteral->unsigned_value, 0ull);
        CU_ASSERT_PTR_NULL(ast->sliteral);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}

void test_new_sliteral_ast(void) {
    char sliteral_const[15] = "string literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    Ast* ast = new_sliteral_ast(AST_STRING_EXPR, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Ast* copied_ast = ast_copy(ast);
            delete_ast(ast);
            ast = copied_ast;
        }
        CU_ASSERT_EQUAL(ast->type, AST_STRING_EXPR);
        CU_ASSERT_PTR_NULL(ast->ident_name);
        CU_ASSERT_PTR_NULL(ast->iliteral);
        CU_ASSERT_EQUAL(memcmp(ast->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(ast->sliteral->size, sliteral_size);
        CU_ASSERT_EQUAL(vector_size(ast->children), 0);
    }

    delete_ast(ast);
}
