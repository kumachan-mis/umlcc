#include "./ope.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcOpe* new_imm_immcope(int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_IMM;
    immcope->imm_value = imm_value;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_reg_immcope(int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_REG;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_ptr_immcope(int reg_id) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_PTR;
    immcope->imm_value = -1;
    immcope->reg_id = reg_id;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_mem_immcope(int mem_offset) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_MEM;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = mem_offset;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_label_immcope(char* label_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_LABEL;
    immcope->imm_value = -1;
    immcope->reg_id = -1;
    immcope->mem_offset = -1;
    immcope->label_name = label_name;
    return immcope;
}

ImmcOpe* immcope_copy(ImmcOpe* immcope) {
    ImmcOpe* copied_immcope = malloc(sizeof(ImmcOpe));
    copied_immcope->type = immcope->type;
    copied_immcope->imm_value = immcope->imm_value;
    copied_immcope->reg_id = immcope->reg_id;
    copied_immcope->mem_offset = immcope->mem_offset;
    copied_immcope->label_name = NULL;
    if (immcope->label_name != NULL) {
        copied_immcope->label_name = string_copy(immcope->label_name);
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
            sprintf(ope_str, "r%d", immcope->reg_id);
            break;
        case OPERAND_PTR:
            sprintf(ope_str, "(r%d)", immcope->reg_id);
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

void delete_immope(ImmcOpe* immcope) {
    if (immcope->label_name != NULL) free(immcope->label_name);
    free(immcope);
}
