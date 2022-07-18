#include "./ope.h"
#include "../common/type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcOpe* new_imm_immcope(ImmcSuffix suffix, int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_IMM;
    immcope->suffix = suffix;
    immcope->imm_value = imm_value;
    immcope->sliteral = NULL;
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
    immcope->sliteral = NULL;
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
    immcope->sliteral = NULL;
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
    immcope->sliteral = NULL;
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
    immcope->sliteral = NULL;
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
    immcope->sliteral = NULL;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = label_name;
    return immcope;
}

ImmcOpe* new_str_immcope(StringLiteral* sliteral) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = IMMC_OPERAND_STR;
    immcope->suffix = IMMC_SUFFIX_NONE;
    immcope->imm_value = -1;
    immcope->sliteral = sliteral;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* immcope_copy(ImmcOpe* immcope) {
    ImmcOpe* copied_immcope = malloc(sizeof(ImmcOpe));
    copied_immcope->type = immcope->type;
    copied_immcope->suffix = immcope->suffix;
    copied_immcope->imm_value = immcope->imm_value;
    copied_immcope->sliteral = NULL;
    if (immcope->sliteral != NULL) copied_immcope->sliteral = sliteral_copy(immcope->sliteral);
    copied_immcope->reg_id = immcope->reg_id;
    copied_immcope->mem_offset = immcope->mem_offset;
    copied_immcope->label_name = NULL;
    if (immcope->label_name != NULL) copied_immcope->label_name = new_string(immcope->label_name);
    return copied_immcope;
}

char* immcope_tostring(ImmcOpe* immcope) {
    // TODO: fix to prevent buffer overflow
    char* ope_str = malloc(50 * sizeof(char));

    switch (immcope->type) {
        case IMMC_OPERAND_IMM:
            sprintf(ope_str, "%d", immcope->imm_value);
            break;
        case IMMC_OPERAND_ARG:
            sprintf(ope_str, "%%a%d%c", immcope->imm_value, immcsuffix_tochar(immcope->suffix));
            break;
        case IMMC_OPERAND_REG:
            sprintf(ope_str, "%%r%d%c", immcope->reg_id, immcsuffix_tochar(immcope->suffix));
            break;
        case IMMC_OPERAND_PTR:
            sprintf(ope_str, "(%%r%d%c)", immcope->reg_id, immcsuffix_tochar(immcope->suffix));
            break;
        case IMMC_OPERAND_MEM:
            sprintf(ope_str, "M[%d]", immcope->mem_offset);
            break;
        case IMMC_OPERAND_LABEL:
            sprintf(ope_str, "%s", immcope->label_name);
            break;
        case IMMC_OPERAND_STR: {
            char* display_str = sliteral_display_string(immcope->sliteral);
            sprintf(ope_str, "%s", display_str);
            free(display_str);
            break;
        }
    }
    ope_str = realloc(ope_str, (strlen(ope_str) + 1) * sizeof(char));
    return ope_str;
}

void delete_immcope(ImmcOpe* immcope) {
    if (immcope->label_name != NULL) free(immcope->label_name);
    if (immcope->sliteral != NULL) delete_sliteral(immcope->sliteral);
    free(immcope);
}
