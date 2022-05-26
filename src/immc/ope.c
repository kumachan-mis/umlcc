#include "./ope.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcOpe* new_imm_immcope(int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_IMM;
    immcope->suffix = SUFFIX_NONE;
    immcope->imm_value = imm_value;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_reg_immcope(ImmcOpeSuffix suffix, int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_REG;
    immcope->suffix = suffix;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_ptr_immcope(int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_PTR;
    immcope->suffix = SUFFIX_QUAD;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_mem_immcope(int mem_offset) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_MEM;
    immcope->suffix = SUFFIX_NONE;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = mem_offset;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_label_immcope(char* label_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_LABEL;
    immcope->suffix = SUFFIX_NONE;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = label_name;
    return immcope;
}

void delete_immope(ImmcOpe* immcope) {
    if (immcope->label_name != NULL) free(immcope->label_name);
    free(immcope);
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

    switch (immcope->type) {
        case OPERAND_IMM:
            sprintf(ope_str, "%d", immcope->imm_value);
            break;
        case OPERAND_REG:
            sprintf(ope_str, "%%r%d%c", immcope->reg_id, immcope_suffix_tochar(immcope->suffix));
            break;
        case OPERAND_PTR:
            sprintf(ope_str, "(%%r%d%c)", immcope->reg_id, immcope_suffix_tochar(immcope->suffix));
            break;
        case OPERAND_MEM:
            sprintf(ope_str, "M[%d]", immcope->mem_offset);
            break;
        case OPERAND_LABEL:
            sprintf(ope_str, "%s", immcope->label_name);
            break;
    }
    ope_str = realloc(ope_str, (strlen(ope_str) + 1) * sizeof(char));
    return ope_str;
}

ImmcOpeSuffix immcope_suffix_get(int size) {
    switch (size) {
        case 1:
            return SUFFIX_BYTE;
        case 2:
            return SUFFIX_WORD;
        case 4:
            return SUFFIX_LONG;
        case 8:
            return SUFFIX_QUAD;
        default:
            return SUFFIX_NONE;
    }
}

int immcope_suffix_tosize(ImmcOpeSuffix suffix) {
    switch (suffix) {
        case SUFFIX_BYTE:
            return 1;
        case SUFFIX_WORD:
            return 2;
        case SUFFIX_LONG:
            return 4;
        case SUFFIX_QUAD:
            return 8;
        default:
            return 0;
    }
}

char immcope_suffix_tochar(ImmcOpeSuffix suffix) {
    switch (suffix) {
        case SUFFIX_BYTE:
            return 'b';
        case SUFFIX_WORD:
            return 'w';
        case SUFFIX_LONG:
            return 'l';
        case SUFFIX_QUAD:
            return 'q';
        default:
            return '\0';
    }
}
