#include "./ope.h"
#include "../common/type.h"
#include "./register.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

X64Ope* new_imm_x64ope(int imm_value) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_IMM;
    x64ope->suffix = X64_SUFFIX_LONG;
    x64ope->imm_value = imm_value;
    x64ope->reg_id = -1;
    x64ope->mem_offset = -1;
    x64ope->label_name = NULL;
    return x64ope;
}

X64Ope* new_reg_x64ope(X64Suffix suffix, int reg_id) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_REG;
    x64ope->suffix = suffix;
    x64ope->imm_value = -1;
    x64ope->reg_id = reg_id;
    x64ope->mem_offset = -1;
    x64ope->label_name = NULL;
    return x64ope;
}

X64Ope* new_ptr_x64ope(int reg_id) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_PTR;
    x64ope->suffix = X64_SUFFIX_QUAD;
    x64ope->imm_value = -1;
    x64ope->reg_id = reg_id;
    x64ope->mem_offset = -1;
    x64ope->label_name = NULL;
    return x64ope;
}

X64Ope* new_mem_x64ope(int mem_offset) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_MEM;
    x64ope->suffix = X64_SUFFIX_NONE;
    x64ope->imm_value = -1;
    x64ope->reg_id = -1;
    x64ope->mem_offset = mem_offset;
    x64ope->label_name = NULL;
    return x64ope;
}

X64Ope* new_label_x64ope(char* label_name) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_LABEL;
    x64ope->suffix = X64_SUFFIX_NONE;
    x64ope->imm_value = -1;
    x64ope->reg_id = -1;
    x64ope->mem_offset = -1;
    x64ope->label_name = label_name;
    return x64ope;
}

X64Ope* new_suffix_x64ope(X64Suffix suffix) {
    X64Ope* x64ope = malloc(sizeof(X64Ope));
    x64ope->type = X64_OPERAND_SUFFIX;
    x64ope->suffix = suffix;
    x64ope->imm_value = -1;
    x64ope->reg_id = -1;
    x64ope->mem_offset = -1;
    x64ope->label_name = NULL;
    return x64ope;
}

X64Ope* x64ope_copy(X64Ope* x64ope) {
    X64Ope* copied_x64ope = malloc(sizeof(X64Ope));
    copied_x64ope->type = x64ope->type;
    copied_x64ope->suffix = x64ope->suffix;
    copied_x64ope->imm_value = x64ope->imm_value;
    copied_x64ope->reg_id = x64ope->reg_id;
    copied_x64ope->mem_offset = x64ope->mem_offset;
    copied_x64ope->label_name = NULL;
    if (x64ope->label_name != NULL) copied_x64ope->label_name = new_string(x64ope->label_name);
    return copied_x64ope;
}

char* x64ope_tostring(X64Ope* x64ope) {
    char* ope_str = malloc(20 * sizeof(char));

    switch (x64ope->type) {
        case X64_OPERAND_IMM:
            sprintf(ope_str, "$%d", x64ope->imm_value);
            break;
        case X64_OPERAND_REG:
            sprintf(ope_str, "%s", reg_name(x64ope->reg_id, x64ope->suffix));
            break;
        case X64_OPERAND_PTR:
            sprintf(ope_str, "(%s)", reg_name(x64ope->reg_id, x64ope->suffix));
            break;
        case X64_OPERAND_MEM:
            sprintf(ope_str, "%d(%s)", -x64ope->mem_offset, QREG_NAMES[BP_REG_ID]);
            break;
        case X64_OPERAND_LABEL:
            sprintf(ope_str, "%s(%s)", x64ope->label_name, PC_NAME);
            break;
        case X64_OPERAND_SUFFIX:
            sprintf(ope_str, "");
            break;
    }
    ope_str = realloc(ope_str, (strlen(ope_str) + 1) * sizeof(char));
    return ope_str;
}

void delete_x64ope(X64Ope* x64ope) {
    if (x64ope->label_name != NULL) free(x64ope->label_name);
    free(x64ope);
}