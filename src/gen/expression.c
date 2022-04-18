#include "./expression.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>


Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->_ast;

    codegen->_ast = vector_at(ast->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_ast = vector_at(ast->children, 0);
    codegen->_assignee_mode = 1;
    sub_codes = codegen_generate_code(codegen);
    codegen->_assignee_mode = 0;
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rdx\n");
    append_code(codes, "    popq %%rax\n");

    switch (ast->type) {
        case AST_ASSIGN_EXPR:
            append_code(codes, "    movl %%eax, (%%rdx)\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->_ast = ast;
    return codes;
}

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

    append_code(codes, "    popq %%rcx\n");
    append_code(codes, "    popq %%rax\n");

    switch (ast->type) {
        case AST_MUL_EXPR:
            append_code(codes, "    imull %%ecx, %%eax\n");
            break;
        case AST_DIV_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%ecx\n");
            break;
        case AST_MOD_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%ecx\n");
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
        case AST_IDENT_EXPR: {
            Symbol* symbol = symboltable_search_symbol(codegen->_table, ast->ident_name);
            if (symbol == NULL) {
                fprintf(stderr, "Error: identifier '%s' is used before declared\n", ast->ident_name);
                exit(1);
            }
            append_code(codes, "    movl -%d(%%rbp), %%eax\n", symbol->memory_offset);
            append_code(codes, "    pushq %%rax\n");
            break;
        }
        case AST_INT_EXPR:
            append_code(codes, "    pushq $%d\n", ast->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    return codes;
}

Vector* gen_assignee_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Ast* ast = codegen->_ast;

    switch (ast->type) {
        case AST_IDENT_EXPR: {
            Symbol* symbol = symboltable_search_symbol(codegen->_table, ast->ident_name);
            if (symbol == NULL) {
                fprintf(stderr, "Error: identifier '%s' is used before declared\n", ast->ident_name);
                exit(1);
            }
            append_code(codes, "    leaq -%d(%%rbp), %%rax\n", symbol->memory_offset);
            append_code(codes, "    pushq %%rax\n");
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }

    return codes;
}
