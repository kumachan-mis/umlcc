#include "./datainst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));
    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];

    switch (immc_src->type) {
        case IMMC_OPERAND_INT: {
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_int_x64ope(dst_suffix, iliteral_copy(immc_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_src->suffix));
            int src_id = CALLER_SAVED_REG_IDS[immc_src->reg->reg_id];

            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_reg_x64ope(src_suffix, src_id);
            int src_is_unsigned = immc_src->reg->is_unsigned;
            append_mov_code(codes, src, dst, src_is_unsigned);
            break;
        }
        case IMMC_OPERAND_PTR: {
            X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
            X64Ope* src = new_ptr_x64ope(CALLER_SAVED_REG_IDS[immc_src->reg->reg_id]);
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_load_x64code)\n");
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_addr_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immmc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immmc_dst->suffix));
    int dst_id = CALLER_SAVED_REG_IDS[immmc_dst->reg->reg_id];
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_addr_x64code)\n");
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_str_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immmc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    int mem_offset = immmc_dst->mem_offset;
    int index_offset = 0;

    int tmp_reg_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
    int unit_size = x64suffix_tonbytes(X64_SUFFIX_QUAD);

    while (index_offset + unit_size <= immc_src->sliteral->size) {
        unsigned long long unit_value = 0ull;
        for (int i = 0; i < unit_size; i++) {
            unsigned long long char_bits = immc_src->sliteral->value[index_offset + i];
            unit_value = (char_bits << (i * CHAR_BIT)) | unit_value;
        }

        X64Ope* imm_dst = new_reg_x64ope(X64_SUFFIX_QUAD, tmp_reg_id);
        X64Ope* imm_src = new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG, unit_value);
        vector_push(codes, new_inst_x64(X64_INST_MOVABSX, imm_src, imm_dst));

        X64Ope* mem_dst = new_mem_x64ope(mem_offset);
        X64Ope* mem_src = new_reg_x64ope(X64_SUFFIX_QUAD, tmp_reg_id);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, mem_src, mem_dst));

        mem_offset -= unit_size;
        index_offset += unit_size;
    }

    int num_rest_bytes = immc_src->sliteral->size - index_offset;

    if (num_rest_bytes == 0) {
        liveseqs_next(x64gen->liveseqs);
        return codes;
    }

    int boundary_rest_bytes = 0;
    IntegerLiteralType iliteral_type = INTEGER_UNSIGNED_INT;
    if (num_rest_bytes <= 1) {
        boundary_rest_bytes = 1;
        iliteral_type = INTEGER_UNSIGNED_INT;
    } else if (num_rest_bytes <= 2) {
        boundary_rest_bytes = 2;
        iliteral_type = INTEGER_UNSIGNED_INT;
    } else if (num_rest_bytes <= 4) {
        boundary_rest_bytes = 4;
        iliteral_type = INTEGER_UNSIGNED_INT;
    } else {
        boundary_rest_bytes = 8;
        iliteral_type = INTEGER_UNSIGNED_LONG;
    }

    X64Suffix suffix = x64suffix_get(boundary_rest_bytes);
    unsigned long long unit_value = 0ull;
    for (int i = 0; i < num_rest_bytes; i++) {
        unsigned long long char_bits = immc_src->sliteral->value[index_offset + i];
        unit_value = (char_bits << (i * CHAR_BIT)) | unit_value;
    }

    if (suffix == X64_SUFFIX_QUAD) {
        X64Ope* imm_dst = new_reg_x64ope(X64_SUFFIX_QUAD, tmp_reg_id);
        X64Ope* imm_src = new_unsigned_int_x64ope(X64_SUFFIX_QUAD, iliteral_type, unit_value);
        vector_push(codes, new_inst_x64(X64_INST_MOVABSX, imm_src, imm_dst));

        X64Ope* mem_dst = new_mem_x64ope(mem_offset);
        X64Ope* mem_src = new_reg_x64ope(X64_SUFFIX_QUAD, tmp_reg_id);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, mem_src, mem_dst));
    } else {
        X64Ope* dst = new_mem_x64ope(mem_offset);
        X64Ope* src = new_unsigned_int_x64ope(suffix, iliteral_type, unit_value);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_store_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Ope* dst = NULL;
    X64Ope* src = NULL;

    switch (immc_dst->type) {
        case IMMC_OPERAND_PTR:
            dst = new_ptr_x64ope(CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id]);
            break;
        case IMMC_OPERAND_MEM:
            dst = new_mem_x64ope(immc_dst->mem_offset);
            break;
        case IMMC_OPERAND_LABEL:
            dst = new_label_x64ope(new_string(immc_dst->label_name));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_store_x64code dst)\n");
            exit(1);
    }

    switch (immc_src->type) {
        case IMMC_OPERAND_INT: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_src->suffix));
            src = new_int_x64ope(src_suffix, iliteral_copy(immc_src->iliteral));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Suffix src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_src->suffix));
            src = new_reg_x64ope(src_suffix, CALLER_SAVED_REG_IDS[immc_src->reg->reg_id]);
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_store_x64code src)\n");
            exit(1);
    }

    vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    X64Suffix src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_src->suffix));

    if (immc_src->reg->reg_id < NUM_ARG_REGS) {
        X64Ope* src = new_reg_x64ope(src_suffix, ARG_REG_IDS[immc_src->reg->reg_id]);
        X64Ope* dst = new_mem_x64ope(immc_dst->mem_offset);

        vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));

        liveseqs_next(x64gen->liveseqs);
        return codes;
    }

    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)
    int mem_arg_offset = -(immc_src->reg->reg_id - NUM_ARG_REGS + 1) * 8 - 8;
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
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    switch (immc_src->type) {
        case IMMC_OPERAND_INT: {
            X64Ope* src = new_int_x64ope(X64_SUFFIX_QUAD, iliteral_copy(immc_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_PUSHX, src, NULL));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Ope* src = new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[immc_src->reg->reg_id]);
            vector_push(codes, new_inst_x64(X64_INST_PUSHX, src, NULL));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_starg_x64code)\n");
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_stret_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_ret = immc->inst->fst_src;
    X64Suffix ret_suffix = x64suffix_get(immcsuffix_tonbytes(immc_ret->suffix));

    switch (immc_ret->type) {
        case IMMC_OPERAND_INT: {
            X64Ope* dst = new_reg_x64ope(ret_suffix, AX_REG_ID);
            X64Ope* src = new_int_x64ope(ret_suffix, iliteral_copy(immc_ret->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG: {
            int ret_id = CALLER_SAVED_REG_IDS[immc_ret->reg->reg_id];

            X64Ope* dst = new_reg_x64ope(ret_suffix, AX_REG_ID);
            X64Ope* src = new_reg_x64ope(ret_suffix, ret_id);
            int ret_is_unsigned = immc_ret->reg->is_unsigned;
            append_mov_code(codes, src, dst, ret_is_unsigned);
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_stret_x64code)\n");
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
