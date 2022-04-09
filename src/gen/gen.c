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
        case AST_ADD:
        case AST_SUB:
            codes = gen_additive_expr_code(codegen);
            break;
        case AST_MUL:
        case AST_DIV:
        case AST_MOD:
            codes = gen_multiplicative_expr_code(codegen);
            break;
        case AST_INT:
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

    append_code(codes, "    popl %%edx");
    append_code(codes, "    popl %%eax");

    switch (ast->type) {
        case AST_ADD:
            append_code(codes, "    addl %%edx, %%eax");
            break;
        case AST_SUB:
            append_code(codes, "    subl %%edx, %%eax");
            break;
    }

    append_code(codes, "    pushl %%eax");

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

    append_code(codes, "    popl %%edx");
    append_code(codes, "    popl %%eax");

    switch (ast->type) {
        case AST_MUL:
            append_code(codes, "    imull %%edx, %%eax");
            break;
        case AST_DIV:
            append_code(codes, "    cltd");
            append_code(codes, "    idivl %%edx");
            break;
        case AST_MOD:
            append_code(codes, "    cltd");
            append_code(codes, "    idivl %%edx");
            append_code(codes, "    movel %%edx, %%eax");
            break;
    }

    append_code(codes, "    pushl %%eax");

    codegen->ast = ast;
    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Ast* ast = codegen->ast;

    switch (ast->type) {
        case AST_INT:
            append_code(codes, "    pushl $%d", ast->value_int);
            break;
        default:
            fprintf("Error: unexpected ast type %d", ast->type);
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

    va_end(arg_ptr);
    return code;
}