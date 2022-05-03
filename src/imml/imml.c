#include "./imml.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* inst_strings[] = {"load  ", "addr  ", "store ", "ldarg ", "starg ",
                        "add   ", "sub   ", "mul   ", "div   ", "mod   ",
                        "call  ", "enter ", "leave ", "glabel", "label "};

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
    memset(code_str, 0, 100 * sizeof(char));

    if (immlcode->inst != INST_GLABEL && immlcode->inst != INST_LABEL) strcat(code_str, "\t");
    strcat(code_str, inst_strings[immlcode->inst]);

    int operand_appears = 0;
    if (immlcode->dest != NULL) {
        char* dest_string = immlope_tostring(immlcode->dest);
        strcat(code_str, "\t");
        strcat(code_str, dest_string);
        free(dest_string);
        operand_appears = 1;
    }

    if (immlcode->fst_src != NULL) {
        char* fst_src_string = immlope_tostring(immlcode->fst_src);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, fst_src_string);
        free(fst_src_string);
        operand_appears = 1;
    }

    if (immlcode->snd_src != NULL) {
        char* snd_src_string = immlope_tostring(immlcode->snd_src);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, snd_src_string);
        free(snd_src_string);
        operand_appears = 1;
    }

    strcat(code_str, "\n");
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

ImmlOpe* new_reg_immlope(int reg_name) {
    ImmlOpe* immlope = malloc(sizeof(ImmlOpe));
    immlope->type = OPERAND_REG;
    immlope->imm_value = -1;
    immlope->reg_name = reg_name;
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

ImmlOpe* immlope_copy(ImmlOpe* immlope) {
    ImmlOpe* copied_immlope = malloc(sizeof(ImmlOpe));
    copied_immlope->type = immlope->type;
    copied_immlope->imm_value = immlope->imm_value;
    copied_immlope->reg_name = immlope->reg_name;
    copied_immlope->mem_offset = immlope->mem_offset;
    if (immlope->label_name == NULL)
        copied_immlope->label_name = NULL;
    else
        copied_immlope->label_name = string_copy(immlope->label_name);
    return copied_immlope;
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
