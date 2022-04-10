#include "./gen.h"
#include "../ast/ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


struct _Codegen {
    Ast* ast;
};

Vector* gen_additive_expr_code(Codegen* codegen);
Vector* gen_multiplicative_expr_code(Codegen* codegen);
Vector* gen_primary_expr_code(Codegen* codegen);
void append_code(Vector* codes, char* format, ...);

Codegen* new_codegen(Ast* ast) {
    Codegen* codegen = malloc(sizeof(Codegen));
    codegen->ast = ast;
    return codegen;
}

void delete_codegen(Codegen* codegen) {
    delete_ast(codegen->ast);
    free(codegen);
}

Vector* codegen_generate_code(Codegen* codegen) {
    Vector* codes = NULL;
    Ast* ast = codegen->ast;

    switch (ast->type) {
        case AST_ADD_EXPR:
        case AST_SUB_EXPR:
            codes = gen_additive_expr_code(codegen);
            break;
        case AST_MUL_EXPR:
        case AST_DIV_EXPR:
        case AST_MOD_EXPR:
            codes = gen_multiplicative_expr_code(codegen);
            break;
        case AST_INT_EXPR:
            codes = gen_primary_expr_code(codegen);
            break;
        default:
            break;
    }

    return codes;
}

Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->ast;

    codegen->ast = vector_at(ast->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->ast = vector_at(ast->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rdx\n");
    append_code(codes, "    popq %%rax\n");

    switch (ast->type) {
        case AST_ADD_EXPR:
            append_code(codes, "    addl %%edx, %%eax\n");
            break;
        case AST_SUB_EXPR:
            append_code(codes, "    subl %%edx, %%eax\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->ast = ast;
    return codes;
}

Vector* gen_multiplicative_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->ast;

    codegen->ast = vector_at(ast->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->ast = vector_at(ast->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rdx\n");
    append_code(codes, "    popq %%rax\n");

    switch (ast->type) {
        case AST_MUL_EXPR:
            append_code(codes, "    imull %%edx, %%eax\n");
            break;
        case AST_DIV_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%edx\n");
            break;
        case AST_MOD_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%edx\n");
            append_code(codes, "    movl %%edx, %%eax\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->ast = ast;
    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Ast* ast = codegen->ast;

    switch (ast->type) {
        case AST_INT_EXPR:
            append_code(codes, "    pushq $%d\n", ast->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    return codes;
}

void append_code(Vector* codes, char* format, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, format);

    char* code = malloc((50 + 1) * sizeof(char));
    vsprintf(code, format, arg_ptr);
    code = realloc(code, (strlen(code) + 1) * sizeof(char));
    vector_push(codes, code);

    va_end(arg_ptr);
}