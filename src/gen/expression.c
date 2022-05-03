#include "./expression.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf
char arg_regs[][6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

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

    append_code(codes, "    popq %%rsi\n");
    append_code(codes, "    popq %%rax\n");

    switch (srt->type) {
        case SRT_MUL_EXPR:
            append_code(codes, "    imull %%esi, %%eax\n");
            break;
        case SRT_DIV_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%esi\n");
            break;
        case SRT_MOD_EXPR:
            append_code(codes, "    cltd\n");
            append_code(codes, "    idivl %%esi\n");
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
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;
    Vector* gen_call_expr_code(Codegen * codegen);

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codes = gen_call_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_call_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    Srt* params_srt = vector_at(srt->children, 1);
    int num_args = vector_size(params_srt->children);

    // The end of the input argument area shall be aligned on a 16 byte boundary.
    if (num_args > 6 && num_args % 2 == 1) append_code(codes, "    subq $%d, %%rsp\n", 8);

    // Once registers are assigned,
    // the arguments passed in memory are pushed on the stack in REVERSED order.
    for (int i = num_args - 1; i >= 0; i--) {
        codegen->_srt = vector_at(params_srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
        if (i < 6) append_code(codes, "    popq %s\n", arg_regs[i]);
    }

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);
    append_code(codes, "    popq %%rax\n");
    append_code(codes, "    call *%%rax\n");

    if (num_args > 6) {
        int params_offset = (num_args - 6) * 8;
        if (num_args % 2 == 1) params_offset += 8;
        append_code(codes, "    addq $%d, %%rsp\n", params_offset);
    }

    append_code(codes, "    pushq %%rax\n");

    return codes;
}

Vector* gen_unary_expr_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;
    Vector* gen_address_expr_code(Codegen * codegen);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_address_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = vector_at(codegen->_srt->children, 0);
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
                append_code(codes, "    leaq %s(%%rip), %%rax\n", symbol->name);
                append_code(codes, "    pushq %%rax\n");
                break;
            }
            break;
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
            break;
        case SRT_INT_EXPR:
            append_code(codes, "    pushq $%d\n", srt->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}
