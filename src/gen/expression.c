#include "./expression.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>


Vector* gen_address_expr_code(Codegen* codegen);


Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rdx\n");
    append_code(codes, "    popq %%rax\n");

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            append_code(codes, "    movl %%eax, (%%rdx)\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->_srt = srt;
    return codes;
}

Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rdx\n");
    append_code(codes, "    popq %%rax\n");

    switch (srt->type) {
        case SRT_ADD_EXPR:
            append_code(codes, "    addl %%edx, %%eax\n");
            break;
        case SRT_SUB_EXPR:
            append_code(codes, "    subl %%edx, %%eax\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->_srt = srt;
    return codes;
}

Vector* gen_multiplicative_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rcx\n");
    append_code(codes, "    popq %%rax\n");

    switch (srt->type) {
        case SRT_MUL_EXPR:
            append_code(codes, "    imull %%ecx, %%eax\n");
            break;
        case SRT_DIV_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%ecx\n");
            break;
        case SRT_MOD_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%ecx\n");
            append_code(codes, "    movl %%edx, %%eax\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->_srt = srt;
    return codes;
}

Vector* gen_postfix_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rax\n");

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codegen->_srt = vector_at(srt->children, 1);
            sub_codes = codegen_generate_code(codegen);
            vector_extend(codes, sub_codes);
            delete_vector(sub_codes, free);
            append_code(codes, "    call *%%rax\n");
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    append_code(codes, "    pushq %%rax\n");

    codegen->_srt = srt;
    return codes;
}

Vector* gen_argument_expr_list_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    char arg_regs[][6] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

    int num_args = vector_size(srt->children);
    for (int i = 0; i < num_args; i++) {
        codegen->_srt = vector_at(srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
        if (i < 6) append_code(codes, "    popq %d\n", arg_regs[i]);
    }

    codegen->_srt = srt;
    return codes;
}

Vector* gen_unary_expr_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->_srt = srt;
    return codes;
}

Vector* gen_address_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;
    Symbol* symbol = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR:
            symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                append_code(codes, "    leaq -%d(%%rbp), %%rax\n", symbol->memory_offset);
                append_code(codes, "    pushq %%rax\n");
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                append_code(codes, "    leaq _%s(%%rip), %%rax\n", symbol->name);
                append_code(codes, "    pushq %%rax\n");
                break;
            }
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->_srt = srt;
    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;
    Symbol* symbol = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: 
            symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                append_code(codes, "    movl -%d(%%rbp), %%eax\n", symbol->memory_offset);
                append_code(codes, "    pushq %%rax\n");
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                append_code(codes, "    movl _%s(%%rip), %%eax\n", symbol->name);
                append_code(codes, "    pushq %%rax\n");
                break;
            }
        case SRT_INT_EXPR:
            append_code(codes, "    pushq $%d\n", srt->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}
