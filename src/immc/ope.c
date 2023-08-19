#include "./ope.h"
#include "../common/type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcOpe* new_base_immcope(ImmcOpeType type);

ImmcOpe* new_arg_immcope(ImmcSuffix suffix, ImmcRegister* reg) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_ARG);
    immcope->suffix = suffix;
    immcope->reg = reg;
    return immcope;
}

ImmcOpe* new_signed_arg_immcope(ImmcSuffix suffix, int reg_id) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_ARG);
    immcope->suffix = suffix;
    immcope->reg = new_signed_immcreg(reg_id);
    return immcope;
}

ImmcOpe* new_unsigned_arg_immcope(ImmcSuffix suffix, int reg_id) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_ARG);
    immcope->suffix = suffix;
    immcope->reg = new_unsigned_immcreg(reg_id);
    return immcope;
}

ImmcOpe* new_reg_immcope(ImmcSuffix suffix, ImmcRegister* reg) {
    ImmcOpe* immcope = immcope = new_base_immcope(IMMC_OPERAND_REG);
    immcope->suffix = suffix;
    immcope->reg = reg;
    return immcope;
}

ImmcOpe* new_signed_reg_immcope(ImmcSuffix suffix, int reg_id) {
    ImmcOpe* immcope = immcope = new_base_immcope(IMMC_OPERAND_REG);
    immcope->suffix = suffix;
    immcope->reg = new_signed_immcreg(reg_id);
    return immcope;
}

ImmcOpe* new_unsigned_reg_immcope(ImmcSuffix suffix, int reg_id) {
    ImmcOpe* immcope = immcope = new_base_immcope(IMMC_OPERAND_REG);
    immcope->suffix = suffix;
    immcope->reg = new_unsigned_immcreg(reg_id);
    return immcope;
}

ImmcOpe* new_ptr_immcope(int reg_id) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_PTR);
    immcope->suffix = IMMC_SUFFIX_QUAD;
    immcope->reg = new_signed_immcreg(reg_id);
    return immcope;
}

ImmcOpe* new_mem_immcope(int mem_offset) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_MEM);
    immcope->mem_offset = mem_offset;
    return immcope;
}

ImmcOpe* new_label_immcope(char* label_name) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_LABEL);
    immcope->label_name = label_name;
    return immcope;
}

ImmcOpe* new_int_immcope(ImmcSuffix suffix, IntegerLiteral* iliteral) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_INT);
    immcope->suffix = suffix;
    immcope->iliteral = iliteral;
    return immcope;
}

ImmcOpe* new_signed_int_immcope(ImmcSuffix suffix, IntegerLiteralType type, long long value) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_INT);
    immcope->suffix = suffix;
    immcope->iliteral = new_signed_iliteral(type, value);
    return immcope;
}
ImmcOpe* new_unsigned_int_immcope(ImmcSuffix suffix, IntegerLiteralType type, unsigned long long value) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_INT);
    immcope->suffix = suffix;
    immcope->iliteral = new_unsigned_iliteral(type, value);
    return immcope;
}

ImmcOpe* new_str_immcope(StringLiteral* sliteral) {
    ImmcOpe* immcope = new_base_immcope(IMMC_OPERAND_STR);
    immcope->sliteral = sliteral;
    return immcope;
}

ImmcOpe* new_base_immcope(ImmcOpeType type) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = type;
    immcope->suffix = IMMC_SUFFIX_NONE;
    immcope->reg = NULL;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    immcope->iliteral = NULL;
    immcope->sliteral = NULL;
    return immcope;
}

ImmcOpe* immcope_copy(ImmcOpe* immcope) {
    ImmcOpe* copied_immcope = malloc(sizeof(ImmcOpe));
    copied_immcope->type = immcope->type;
    copied_immcope->suffix = immcope->suffix;
    copied_immcope->reg = NULL;
    if (immcope->reg != NULL) {
        copied_immcope->reg = immcreg_copy(immcope->reg);
    }
    copied_immcope->mem_offset = immcope->mem_offset;
    copied_immcope->label_name = NULL;
    if (immcope->label_name != NULL) {
        copied_immcope->label_name = new_string(immcope->label_name);
    }
    copied_immcope->iliteral = NULL;
    if (immcope->iliteral != NULL) {
        copied_immcope->iliteral = iliteral_copy(immcope->iliteral);
    }
    copied_immcope->sliteral = NULL;
    if (immcope->sliteral != NULL) {
        copied_immcope->sliteral = sliteral_copy(immcope->sliteral);
    }
    return copied_immcope;
}

char* immcope_tostring(ImmcOpe* immcope) {
    // TODO: fix to prevent buffer overflow
    char* ope_str = malloc(50 * sizeof(char));

    switch (immcope->type) {
        case IMMC_OPERAND_ARG: {
            char sign = immcope->reg->is_unsigned ? 'u' : 's';
            char suffix = immcsuffix_tochar(immcope->suffix);
            sprintf(ope_str, "%%%ca%d%c", sign, immcope->reg->reg_id, suffix);
            break;
        }
        case IMMC_OPERAND_REG: {
            char sign = immcope->reg->is_unsigned ? 'u' : 's';
            char suffix = immcsuffix_tochar(immcope->suffix);
            sprintf(ope_str, "%%%cr%d%c", sign, immcope->reg->reg_id, suffix);
            break;
        }
        case IMMC_OPERAND_PTR: {
            char suffix = immcsuffix_tochar(immcope->suffix);
            sprintf(ope_str, "(%%r%d%c)", immcope->reg->reg_id, suffix);
            break;
        }
        case IMMC_OPERAND_MEM:
            sprintf(ope_str, "M[%d]", immcope->mem_offset);
            break;
        case IMMC_OPERAND_LABEL:
            sprintf(ope_str, "%s", immcope->label_name);
            break;
        case IMMC_OPERAND_INT: {
            char* display_str = iliteral_display_string(immcope->iliteral);
            sprintf(ope_str, "%s", display_str);
            free(display_str);
            break;
        }
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
    if (immcope->reg != NULL) {
        delete_immcreg(immcope->reg);
    }
    if (immcope->label_name != NULL) {
        free(immcope->label_name);
    }
    if (immcope->iliteral != NULL) {
        delete_iliteral(immcope->iliteral);
    }
    if (immcope->sliteral != NULL) {
        delete_sliteral(immcope->sliteral);
    }
    free(immcope);
}
