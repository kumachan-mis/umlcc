#include "./ope.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcOpe* new_imm_immcope(int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_IMM;
    immcope->suffix = IMMC_SUFFIX_NONE;
    immcope->imm_value = imm_value;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_arg_immcope(ImmcSuffix suffix, int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_ARG;
    immcope->suffix = suffix;
    immcope->imm_value = imm_value;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_reg_immcope(ImmcSuffix suffix, int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_REG;
    immcope->suffix = suffix;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_ptr_immcope(int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_PTR;
    immcope->suffix = IMMC_SUFFIX_QUAD;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_mem_immcope(int mem_offset) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_MEM;
    immcope->suffix = IMMC_SUFFIX_NONE;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = mem_offset;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_label_immcope(char* label_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_LABEL;
    immcope->suffix = IMMC_SUFFIX_NONE;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = label_name;
    return immcope;
}

ImmcOpe* immcope_copy(ImmcOpe* immcope) {
    ImmcOpe* copied_immcope = malloc(sizeof(ImmcOpe));
    copied_immcope->type = immcope->type;
    copied_immcope->suffix = immcope->suffix;
    copied_immcope->imm_value = immcope->imm_value;
    copied_immcope->reg_id = immcope->reg_id;
    copied_immcope->mem_offset = immcope->mem_offset;
    copied_immcope->label_name = NULL;
    if (immcope->label_name != NULL) {
        copied_immcope->label_name = new_string(immcope->label_name);
    }
    return copied_immcope;
}

char* immcope_tostring(ImmcOpe* immcope) {
    char* ope_str = malloc(20 * sizeof(char));
    char suffix = immcsuffix_tochar(immcope->suffix);

    switch (immcope->type) {
        case IMMC_OPERAND_IMM:
            sprintf(ope_str, "%d", immcope->imm_value);
            break;
        case IMMC_OPERAND_ARG:
            sprintf(ope_str, "%%a%d%c", immcope->imm_value, suffix);
            break;
        case IMMC_OPERAND_REG:
            sprintf(ope_str, "%%r%d%c", immcope->reg_id, suffix);
            break;
        case IMMC_OPERAND_PTR:
            sprintf(ope_str, "(%%r%d%c)", immcope->reg_id, suffix);
            break;
        case IMMC_OPERAND_MEM:
            sprintf(ope_str, "M[%d]", immcope->mem_offset);
            break;
        case IMMC_OPERAND_LABEL:
            sprintf(ope_str, "%s", immcope->label_name);
            break;
    }
    ope_str = realloc(ope_str, (strlen(ope_str) + 1) * sizeof(char));
    return ope_str;
}

void delete_immope(ImmcOpe* immcope) {
    if (immcope->label_name != NULL) free(immcope->label_name);
    free(immcope);
}

ImmcSuffix immcsuffix_get(int size) {
    switch (size) {
        case 1:
            return IMMC_SUFFIX_BYTE;
        case 2:
            return IMMC_SUFFIX_WORD;
        case 4:
            return IMMC_SUFFIX_LONG;
        case 8:
            return IMMC_SUFFIX_QUAD;
        default:
            return IMMC_SUFFIX_NONE;
    }
}

int immcsuffix_tosize(ImmcSuffix suffix) {
    switch (suffix) {
        case IMMC_SUFFIX_BYTE:
            return 1;
        case IMMC_SUFFIX_WORD:
            return 2;
        case IMMC_SUFFIX_LONG:
            return 4;
        case IMMC_SUFFIX_QUAD:
            return 8;
        default:
            return 0;
    }
}

char immcsuffix_tochar(ImmcSuffix suffix) {
    switch (suffix) {
        case IMMC_SUFFIX_BYTE:
            return 'b';
        case IMMC_SUFFIX_WORD:
            return 'w';
        case IMMC_SUFFIX_LONG:
            return 'l';
        case IMMC_SUFFIX_QUAD:
            return 'q';
        default:
            return '\0';
    }
}

ImmcSuffix immcsuffix_max(ImmcSuffix a, ImmcSuffix b) {
    return a >= b ? a : b;
}
