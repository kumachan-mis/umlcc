#include "./util.h"
#include "../immc/immc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_children_immcode(Immcgen* immcgen, Vector* codes) {
    Vector* sub_codes = NULL;
    Srt* srt = immcgen->srt;

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        immcgen->srt = vector_at(srt->children, i);
        sub_codes = immcgen_generate_immcode(immcgen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes);
    }

    immcgen->srt = srt;
}

void append_child_immcode(Immcgen* immcgen, Vector* codes, int index) {
    Vector* sub_codes = NULL;
    Srt* srt = immcgen->srt;

    immcgen->srt = vector_at(srt->children, index);
    sub_codes = immcgen_generate_immcode(immcgen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes);

    immcgen->srt = srt;
}

void update_non_void_expr_register(Immcgen* immcgen, ImmcOpe* dst) {
    immcgen->expr_reg_suffix = dst->suffix;
    immcgen->expr_reg_id = dst->reg_id;
}

void update_void_expr_register(Immcgen* immcgen) {
    immcgen->expr_reg_suffix = IMMC_SUFFIX_NONE;
    immcgen->expr_reg_id = -1;
}

ImmcOpe* gen_child_int_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, index);

    ImmcSuffix suffix = IMMC_SUFFIX_NONE;
    if (child_srt->type == SRT_CAST_EXPR) suffix = immcsuffix_get(dtype_nbytes(child_srt->dtype));
    while (child_srt->type == SRT_CAST_EXPR) {
        immcgen->srt = child_srt;
        index = 0;
        child_srt = vector_at(child_srt->children, index);
    }

    if (child_srt->type == SRT_INT_EXPR || child_srt->type == SRT_CHAR_EXPR) {
        immcgen->srt = srt;
        if (suffix == IMMC_SUFFIX_NONE) suffix = immcsuffix_get(dtype_nbytes(child_srt->dtype));
        return new_int_immcope(suffix, iliteral_copy(child_srt->iliteral));
    }

    append_child_immcode(immcgen, codes, index);
    immcgen->srt = srt;

    if (suffix == IMMC_SUFFIX_NONE) return new_reg_immcope(immcgen->expr_reg_suffix, immcgen->expr_reg_id);

    ImmcOpe* src = new_reg_immcope(immcgen->expr_reg_suffix, immcgen->expr_reg_id);
    immcgen->next_reg_id++;
    ImmcOpe* dst = new_reg_immcope(suffix, immcgen->next_reg_id);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));
    return immcope_copy(dst);
}

ImmcOpe* gen_child_reg_immcope(Immcgen* immcgen, Vector* codes, int index) {
    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, index);
    if (src->type == IMMC_OPERAND_REG) return src;

    immcgen->next_reg_id++;
    ImmcOpe* dst = new_reg_immcope(src->suffix, immcgen->next_reg_id);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));
    return immcope_copy(dst);
}

ImmcOpe* gen_child_ptr_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* child_srt = vector_at(immcgen->srt->children, index);

    int memory_or_label = 0;
    if (child_srt->type == SRT_ADDR_EXPR) {
        child_srt = vector_at(child_srt->children, 0);
        if (child_srt->type == SRT_IDENT_EXPR) memory_or_label = 1;
    }

    if (memory_or_label) {
        Symbol* symbol = symboltable_search(immcgen->symbol_table, child_srt->ident_name);
        if (symbol->type == SYMBOL_LABEL) return new_label_immcope(new_string(symbol->name));
        return new_mem_immcope(symbol->memory_offset);
    }

    append_child_immcode(immcgen, codes, index);
    return new_ptr_immcope(immcgen->next_reg_id);
}

ImmcOpe* create_dest_reg_immcope(Immcgen* immcgen) {
    ImmcSuffix suffix = immcsuffix_get(dtype_nbytes(immcgen->srt->dtype));
    immcgen->next_reg_id++;
    return new_reg_immcope(suffix, immcgen->next_reg_id);
}

char* create_label_name(int label_id) {
    // TODO: fix to prevent buffer overflow
    char* label_name = malloc(50 * sizeof(char));
    // TODO: don't use label name which can be conflict with a user-defined identifier
    sprintf(label_name, "L%d", label_id);
    label_name = realloc(label_name, (strlen(label_name) + 1) * sizeof(char));
    return label_name;
}
