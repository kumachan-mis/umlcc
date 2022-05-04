#include "./inst.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* inst_strings[] = {"load  ", "addr  ", "store ", "ldarg ", "starg ", "add   ", "sub   ",
                        "mul   ", "div   ", "mod   ", "call  ", "enter ", "leave "};

ImmcInst* new_immcinst(ImmcInstType type, ImmcOpe* dest, ImmcOpe* fst_src, ImmcOpe* snd_src) {
    ImmcInst* immcinst = malloc(sizeof(ImmcInst));
    immcinst->type = type;
    immcinst->dest = dest;
    immcinst->fst_src = fst_src;
    immcinst->snd_src = snd_src;
    return immcinst;
}

char* immcinst_tostring(ImmcInst* immcinst) {
    char* code_str = malloc(100 * sizeof(char));
    memset(code_str, 0, 100 * sizeof(char));

    strcat(code_str, "\t");
    strcat(code_str, inst_strings[immcinst->type]);

    int operand_appears = 0;
    if (immcinst->dest != NULL) {
        char* dest_string = immcope_tostring(immcinst->dest);
        strcat(code_str, "\t");
        strcat(code_str, dest_string);
        free(dest_string);
        operand_appears = 1;
    }

    if (immcinst->fst_src != NULL) {
        char* fst_src_string = immcope_tostring(immcinst->fst_src);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, fst_src_string);
        free(fst_src_string);
        operand_appears = 1;
    }

    if (immcinst->snd_src != NULL) {
        char* snd_src_string = immcope_tostring(immcinst->snd_src);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, snd_src_string);
        free(snd_src_string);
        operand_appears = 1;
    }

    strcat(code_str, "\n");
    code_str = realloc(code_str, (strlen(code_str) + 1) * sizeof(char));
    return code_str;
}

void delete_immcinst(ImmcInst* immcinst) {
    if (immcinst->dest != NULL) delete_immope(immcinst->dest);
    if (immcinst->fst_src != NULL) delete_immope(immcinst->fst_src);
    if (immcinst->snd_src != NULL) delete_immope(immcinst->snd_src);
    free(immcinst);
}

ImmcOpe* new_imm_immcope(int imm_value) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_IMM;
    immcope->imm_value = imm_value;
    immcope->reg_name = -1;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_reg_immcope(int reg_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_REG;
    immcope->imm_value = -1;
    immcope->reg_name = reg_name;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_ptr_immcope(int reg_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_PTR;
    immcope->imm_value = -1;
    immcope->reg_name = reg_name;
    immcope->mem_offset = -1;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_mem_immcope(int mem_offset) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_MEM;
    immcope->imm_value = -1;
    immcope->reg_name = -1;
    immcope->mem_offset = mem_offset;
    immcope->label_name = NULL;
    return immcope;
}

ImmcOpe* new_label_immcope(char* label_name) {
    ImmcOpe* immcope = malloc(sizeof(ImmcOpe));
    immcope->type = OPERAND_LABEL;
    immcope->imm_value = -1;
    immcope->reg_name = -1;
    immcope->mem_offset = -1;
    immcope->label_name = label_name;
    return immcope;
}

ImmcOpe* immcope_copy(ImmcOpe* immcope) {
    ImmcOpe* copied_immcope = malloc(sizeof(ImmcOpe));
    copied_immcope->type = immcope->type;
    copied_immcope->imm_value = immcope->imm_value;
    copied_immcope->reg_name = immcope->reg_name;
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
            sprintf(ope_str, "r%d", immcope->reg_name);
            break;
        case OPERAND_PTR:
            sprintf(ope_str, "(r%d)", immcope->reg_name);
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