#include "./imml.h"

#include <stdlib.h>

ImmlCode* new_immlcode(ImmlInst inst, ImmlOpe* dest, ImmlOpe* fst_src, ImmlOpe* snd_src) {
    ImmlCode* immlcode = malloc(sizeof(ImmlCode));
    immlcode->inst = inst;
    immlcode->dest = dest;
    immlcode->fst_src = fst_src;
    immlcode->snd_src = snd_src;
    return immlcode;
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

void delete_immope(ImmlOpe* immlope) {
    if (immlope->label_name != NULL) free(immlope->label_name);
    free(immlope);
}
