#include "./datainst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];

    switch (immc_src->type) {
        case IMMC_OPERAND_IMM: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tosize(immc_src->suffix));
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_imm_x64ope(src_suffix, immc_src->imm_value);
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tosize(immc_src->suffix));
            int src_id = CALLER_SAVED_REG_IDS[immc_src->reg_id];
            append_mov_code(codes, src_id, src_suffix, dst_id, dst_suffix);
            break;
        }
        case IMMC_OPERAND_PTR: {
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_ptr_x64ope(CALLER_SAVED_REG_IDS[immc_src->reg_id]);
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_MEM: {
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_mem_x64ope(immc_src->mem_offset);
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_LABEL: {
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_label_x64ope(new_string(immc_src->label_name));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_addr_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immmc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immmc_dst->suffix));
    int dst_id = CALLER_SAVED_REG_IDS[immmc_dst->reg_id];
    X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);

    switch (immc_src->type) {
        case IMMC_OPERAND_MEM: {
            X64Ope* src = new_mem_x64ope(immc_src->mem_offset);
            vector_push(codes, new_inst_x64(X64_INST_LEAX, src, dst));
            break;
        }
        case IMMC_OPERAND_LABEL: {
            X64Ope* src = new_label_x64ope(new_string(immc_src->label_name));
            vector_push(codes, new_inst_x64(X64_INST_LEAX, src, dst));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_store_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Ope* dst = NULL;
    X64Ope* src = NULL;

    switch (immc_dst->type) {
        case IMMC_OPERAND_PTR:
            dst = new_ptr_x64ope(CALLER_SAVED_REG_IDS[immc_dst->reg_id]);
            break;
        case IMMC_OPERAND_MEM:
            dst = new_mem_x64ope(immc_dst->mem_offset);
            break;
        case IMMC_OPERAND_LABEL:
            dst = new_label_x64ope(new_string(immc_dst->label_name));
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_dst->type);
            exit(1);
    }

    switch (immc_src->type) {
        case IMMC_OPERAND_IMM: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tosize(immc_src->suffix));
            src = new_imm_x64ope(src_suffix, immc_src->imm_value);
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tosize(immc_src->suffix));
            src = new_reg_x64ope(src_suffix, CALLER_SAVED_REG_IDS[immc_src->reg_id]);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_src->type);
            exit(1);
    }

    vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix src_suffix = x64suffix_get(immcsuffix_tosize(immc_src->suffix));

    if (immc_src->imm_value < NUM_ARG_REGS) {
        X64Ope* src = new_reg_x64ope(src_suffix, ARG_REG_IDS[immc_src->imm_value]);
        X64Ope* dst = new_mem_x64ope(immc_dst->mem_offset);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
        return codes;
    }

    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)
    int mem_arg_offset = -(immc_src->imm_value - NUM_ARG_REGS + 1) * 8 - 8;
    int tmp_reg_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];

    X64Ope* src = new_mem_x64ope(mem_arg_offset);
    X64Ope* tmp_src = new_reg_x64ope(src_suffix, tmp_reg_id);
    X64Ope* tmp_dst = new_reg_x64ope(src_suffix, tmp_reg_id);
    X64Ope* dst = new_mem_x64ope(immc_dst->mem_offset);

    vector_push(codes, new_inst_x64(X64_INST_MOVX, src, tmp_src));
    vector_push(codes, new_inst_x64(X64_INST_MOVX, tmp_dst, dst));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    switch (immc_src->type) {
        case IMMC_OPERAND_IMM: {
            X64Ope* src = new_imm_x64ope(X64_SUFFIX_QUAD, immc_src->imm_value);
            vector_push(codes, new_inst_x64(X64_INST_PUSHX, src, NULL));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Ope* src = new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[immc_src->reg_id]);
            vector_push(codes, new_inst_x64(X64_INST_PUSHX, src, NULL));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_stret_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_ret = immc->inst->fst_src;
    X64Suffix ret_suffix = x64suffix_get(immcsuffix_tosize(immc_ret->suffix));
    int ret_id = CALLER_SAVED_REG_IDS[immc_ret->reg_id];
    append_mov_code(codes, ret_id, ret_suffix, AX_REG_ID, ret_suffix);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
