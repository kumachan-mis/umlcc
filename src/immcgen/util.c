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

ImmcOpe* gen_child_imm_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* srt = immcgen->srt;
    Srt* child = vector_at(srt->children, index);

    ImmcSuffix suffix = IMMC_SUFFIX_NONE;
    if (child->type == SRT_CAST_EXPR) suffix = immcsuffix_get(dtype_size(child->dtype));
    while (child->type == SRT_CAST_EXPR) {
        immcgen->srt = child;
        index = 0;
        child = vector_at(child->children, index);
    }

    if (child->type == SRT_INT_EXPR || child->type == SRT_CHAR_EXPR) {
        immcgen->srt = srt;
        if (suffix == IMMC_SUFFIX_NONE) suffix = immcsuffix_get(dtype_size(child->dtype));
        return new_imm_immcope(suffix, child->iliteral->signed_value);
    }

    append_child_immcode(immcgen, codes, index);
    immcgen->srt = srt;

    if (suffix != IMMC_SUFFIX_NONE) {
        ImmcOpe* src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
        immcgen->virtual_reg_suffix = suffix;
        immcgen->virtual_reg_id++;
        ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
        vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));
    }
    return new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
}

ImmcOpe* gen_child_reg_immcope(Immcgen* immcgen, Vector* codes, int index) {
    ImmcOpe* immc_ope = gen_child_imm_immcope(immcgen, codes, index);
    if (immc_ope->type == IMMC_OPERAND_REG) return immc_ope;

    immcgen->virtual_reg_suffix = immc_ope->suffix;
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, immc_ope, NULL));
    return new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
}

ImmcOpe* gen_child_ptr_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* child = vector_at(immcgen->srt->children, index);

    int memory_or_label = 0;
    if (child->type == SRT_ADDR_EXPR) {
        child = vector_at(child->children, 0);
        if (child->type == SRT_IDENT_EXPR) memory_or_label = 1;
    }

    if (memory_or_label) {
        Symbol* symbol = NULL;
        if (symbol == NULL && immcgen->local_table != NULL) {
            symbol = symboltable_search(immcgen->local_table, child->ident_name);
        }
        if (symbol == NULL) {
            symbol = symboltable_search(immcgen->global_table, child->ident_name);
        }
        if (symbol->type == SYMBOL_LABEL) return new_label_immcope(new_string(symbol->name));
        return new_mem_immcope(symbol->memory_offset);
    }

    append_child_immcode(immcgen, codes, index);
    return new_ptr_immcope(immcgen->virtual_reg_id);
}

ImmcOpe* create_dest_reg_immcope(Immcgen* immcgen) {
    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(immcgen->srt->dtype));
    immcgen->virtual_reg_id++;
    return new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
}

char* create_label_name(int label_id) {
    // TODO: fix to prevent buffer overflow
    char* label_name = malloc(50 * sizeof(char));
    // TODO: don't use label name which can be conflict with a user-defined identifier
    sprintf(label_name, "L%d", label_id);
    label_name = realloc(label_name, (strlen(label_name) + 1) * sizeof(char));
    return label_name;
}
