#include "./imml.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* inst_strings[] = {"load", "addr", "store", "ldarg", "starg", "add",    "sub",  "mul",
                        "div",  "mod",  "call",  "enter", "leave", "glabel", "label"};

ImmlCode* new_immlcode(ImmlInst inst, ImmlOpe* dest, ImmlOpe* fst_src, ImmlOpe* snd_src) {
    ImmlCode* immlcode = malloc(sizeof(ImmlCode));
    immlcode->inst = inst;
    immlcode->dest = dest;
    immlcode->fst_src = fst_src;
    immlcode->snd_src = snd_src;
    return immlcode;
}

char* immlcode_tostring(ImmlCode* immlcode) {
    char* code_str = malloc(100 * sizeof(char));
    if (immlcode->inst == INST_GLABEL || immlcode->inst == INST_LABEL) {
        sprintf(code_str, "%s", inst_strings[immlcode->inst]);
    } else {
        sprintf(code_str, "\t%s", inst_strings[immlcode->inst]);
    }

    int operand_appears = 0;
    if (immlcode->dest != NULL) {
        sprintf(code_str, "\t%s", immlope_tostring(immlcode->dest));
        operand_appears = 1;
    }

    if (immlcode->fst_src != NULL) {
        char* fst_src_string = immlope_tostring(immlcode->fst_src);
        if (operand_appears) {
            sprintf(code_str, "%s, %s", code_str, fst_src_string);
        } else {
            sprintf(code_str, "%s\t%s", code_str, fst_src_string);
        }
        operand_appears = 1;
    }

    if (immlcode->snd_src != NULL) {
        char* snd_src_string = immlope_tostring(immlcode->snd_src);
        if (operand_appears) {
            sprintf(code_str, "%s, %s", code_str, snd_src_string);
        } else {
            sprintf(code_str, "%s\t%s", code_str, snd_src_string);
        }
        operand_appears = 1;
    }

    sprintf(code_str, "%s\n", code_str);
    code_str = realloc(code_str, (strlen(code_str) + 1) * sizeof(char));
    return code_str;
}

void delete_immlcode(ImmlCode* immlcode) {
    if (immlcode->dest != NULL) delete_immope(immlcode->dest);
    if (immlcode->fst_src != NULL) delete_immope(immlcode->fst_src);
    if (immlcode->snd_src != NULL) delete_immope(immlcode->snd_src);
    free(immlcode);
}

ImmlOpe* new_imm_immlope(int imm_value) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_IMM;
    immlope->imm_value = imm_value;
    immlope->reg_name = -1;
    immlope->mem_offset = -1;
    immlope->label_name = NULL;
    return immlope;
}

ImmlOpe* new_ptr_immlope(int reg_name) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_PTR;
    immlope->imm_value = -1;
    immlope->reg_name = reg_name;
    immlope->mem_offset = -1;
    immlope->label_name = NULL;
    return immlope;
}

ImmlOpe* new_reg_immlope(int reg_name) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_REG;
    immlope->imm_value = -1;
    immlope->reg_name = reg_name;
    immlope->mem_offset = -1;
    immlope->label_name = NULL;
    return immlope;
}

ImmlOpe* new_mem_immlope(int mem_offset) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_MEM;
    immlope->imm_value = -1;
    immlope->reg_name = -1;
    immlope->mem_offset = mem_offset;
    immlope->label_name = NULL;
    return immlope;
}

ImmlOpe* new_label_immlope(char* label_name) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_LABEL;
    immlope->imm_value = -1;
    immlope->reg_name = -1;
    immlope->mem_offset = -1;
    immlope->label_name = label_name;
    return immlope;
}

char* immlope_tostring(ImmlOpe* immlope) {
    char* ope_str = malloc(20 * sizeof(char));

    switch (immlope->type) {
        case OPERAND_IMM:
            sprintf(ope_str, "%d", immlope->imm_value);
            break;
        case OPERAND_REG:
            sprintf(ope_str, "r%d", immlope->reg_name);
            break;
        case OPERAND_PTR:
            sprintf(ope_str, "(r%d)", immlope->reg_name);
            break;
        case OPERAND_MEM:
            sprintf(ope_str, "M[%d]", immlope->mem_offset);
            break;
        case OPERAND_LABEL:
            sprintf(ope_str, "L.%s", immlope->label_name);
            break;
    }
    ope_str = realloc(ope_str, (strlen(ope_str) + 1) * sizeof(char));
    return ope_str;
}

void delete_immope(ImmlOpe* immlope) {
    if (immlope->label_name != NULL) free(immlope->label_name);
    free(immlope);
}
