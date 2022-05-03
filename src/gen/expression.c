#include "./expression.h"
#include "../common/common.h"
#include "../imml/imml.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    ImmlCode* tail_code = NULL;
    ImmlOpe* dest = NULL;
    ImmlOpe* src = NULL;

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    src = new_reg_immlope(tail_code->dest->reg_name);

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    dest = new_ptr_immlope(tail_code->dest->reg_name);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_immlcode(INST_STORE, dest, src, dest));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    // implementation limit
    // dest of tail of expression codes should be a register
    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;
    vector_push(codes, new_immlcode(INST_STORE, dest, src, dest));

    codegen->_srt = srt;
    return codes;
}

Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    ImmlCode* tail_code = NULL;
    ImmlOpe* dest = NULL;
    ImmlOpe* fst_src = NULL;
    ImmlOpe* snd_src = NULL;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    fst_src = new_reg_immlope(tail_code->dest->reg_name);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    snd_src = new_reg_immlope(tail_code->dest->reg_name);

    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_immlcode(INST_ADD, fst_src, snd_src, dest));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_immlcode(INST_SUB, fst_src, snd_src, dest));
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

    ImmlCode* tail_code = NULL;
    ImmlOpe* dest = NULL;
    ImmlOpe* fst_src = NULL;
    ImmlOpe* snd_src = NULL;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    fst_src = new_reg_immlope(tail_code->dest->reg_name);

    codegen->_srt = vector_at(srt->children, 1);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    snd_src = new_reg_immlope(tail_code->dest->reg_name);

    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;

    switch (srt->type) {
        case SRT_MUL_EXPR:
            vector_push(codes, new_immlcode(INST_MUL, dest, fst_src, snd_src));
            break;
        case SRT_DIV_EXPR:
            vector_push(codes, new_immlcode(INST_DIV, dest, fst_src, snd_src));
            break;
        case SRT_MOD_EXPR:
            vector_push(codes, new_immlcode(INST_MOD, dest, fst_src, snd_src));
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

    ImmlCode* tail_code = NULL;
    ImmlOpe* dest = NULL;
    ImmlOpe* fst_src = NULL;
    ImmlOpe* snd_src = NULL;

    Srt* params_srt = vector_at(srt->children, 1);
    int num_args = vector_size(params_srt->children);
    for (int i = num_args - 1; i >= 0; i--) {
        codegen->_srt = vector_at(params_srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);

        fst_src = new_imm_immlope(i);
        snd_src = new_reg_immlope(tail_code->dest->reg_name);
        vector_push(codes, new_immlcode(INST_STARG, NULL, fst_src, snd_src));
    }

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    tail_code = vector_at(sub_codes, vector_size(sub_codes) - 1);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    fst_src = new_ptr_immlope(tail_code->dest->reg_name);
    snd_src = new_imm_immlope(num_args);
    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;
    vector_push(codes, new_immlcode(INST_CALL, dest, fst_src, snd_src));

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

    ImmlOpe* dest = NULL;
    ImmlOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR:
            symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immlope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immlope(string_copy(symbol->name));
                break;
            }
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;
    vector_push(codes, new_immlcode(INST_ADDR, dest, src, NULL));

    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;
    Symbol* symbol = NULL;

    ImmlOpe* dest = NULL;
    ImmlOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR:
            symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immlope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immlope(string_copy(symbol->name));
                break;
            }
            break;
        case SRT_INT_EXPR:
            src = new_imm_immlope(srt->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    dest = new_reg_immlope(codegen->register_name);
    codegen->register_name++;
    vector_push(codes, new_immlcode(INST_LOAD, dest, src, NULL));

    return codes;
}
