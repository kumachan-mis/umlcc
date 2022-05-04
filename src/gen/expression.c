#include "./expression.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* src = new_reg_immcope(codegen->register_name);

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* dest = new_ptr_immcope(codegen->register_name);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_inst_immc(INST_STORE, dest, src, NULL));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    // implementation limit
    // dest of tail of expression codes should be a register
    codegen->register_name++;
    ImmcOpe* dummy_src = immcope_copy(src);
    ImmcOpe* dummy_dest = new_reg_immcope(codegen->register_name);
    vector_push(codes, new_inst_immc(INST_STORE, dummy_dest, dummy_src, NULL));

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
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* fst_src = new_reg_immcope(codegen->register_name);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* snd_src = new_reg_immcope(codegen->register_name);

    codegen->register_name++;
    ImmcOpe* dest = new_reg_immcope(codegen->register_name);

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_inst_immc(INST_ADD, dest, fst_src, snd_src));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_inst_immc(INST_SUB, dest, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

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
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* fst_src = new_reg_immcope(codegen->register_name);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    ImmcOpe* snd_src = new_reg_immcope(codegen->register_name);

    codegen->register_name++;
    ImmcOpe* dest = new_reg_immcope(codegen->register_name);

    switch (srt->type) {
        case SRT_MUL_EXPR:
            vector_push(codes, new_inst_immc(INST_MUL, dest, fst_src, snd_src));
            break;
        case SRT_DIV_EXPR:
            vector_push(codes, new_inst_immc(INST_DIV, dest, fst_src, snd_src));
            break;
        case SRT_MOD_EXPR:
            vector_push(codes, new_inst_immc(INST_MOD, dest, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

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
    for (int i = num_args - 1; i >= 0; i--) {
        codegen->_srt = vector_at(params_srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, (void (*)(void* item))delete_immc);

        ImmcOpe* fst_src = new_imm_immcope(i);
        ImmcOpe* snd_src = new_reg_immcope(codegen->register_name);
        vector_push(codes, new_inst_immc(INST_STARG, NULL, fst_src, snd_src));
    }

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);

    ImmcOpe* fst_src = new_ptr_immcope(codegen->register_name);
    ImmcOpe* snd_src = new_imm_immcope(num_args);

    codegen->register_name++;
    ImmcOpe* dest = new_reg_immcope(codegen->register_name);

    vector_push(codes, new_inst_immc(INST_CALL, dest, fst_src, snd_src));

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

    ImmcOpe* dest = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immcope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immcope(string_copy(symbol->name));
                break;
            }
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->register_name++;
    dest = new_reg_immcope(codegen->register_name);
    vector_push(codes, new_inst_immc(INST_ADDR, dest, src, NULL));

    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    ImmcOpe* dest = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immcope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immcope(string_copy(symbol->name));
                break;
            }
            break;
        }
        case SRT_INT_EXPR:
            src = new_imm_immcope(srt->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->register_name++;
    dest = new_reg_immcope(codegen->register_name);
    vector_push(codes, new_inst_immc(INST_LOAD, dest, src, NULL));

    return codes;
}
