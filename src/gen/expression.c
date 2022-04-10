#include "./gen.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>


Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->_ast;

    codegen->_ast = vector_at(ast->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_ast = vector_at(ast->children, 1);
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

    codegen->_ast = ast;
    return codes;
}

Vector* gen_multiplicative_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->_ast;

    codegen->_ast = vector_at(ast->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_ast = vector_at(ast->children, 1);
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

    codegen->_ast = ast;
    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Ast* ast = codegen->_ast;

    switch (ast->type) {
        case AST_INT:
            append_code(codes, "    pushq $%d\n", ast->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    return codes;
}
