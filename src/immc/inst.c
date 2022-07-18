#include "./inst.h"

#include <stdlib.h>
#include <string.h>

char* immc_insts[] = {"load", "addr", "str",   "store", "ldarg", "starg", "stret",  "add", "sub",
                      "mul",  "div",  "mod",   "sal",   "sar",   "seteq", "setneq", "jmp", "jeq",
                      "jneq", "call", "enter", "leave", "prep",  "clean", "free"};

ImmcInst* new_immcinst(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src) {
    ImmcInst* immcinst = malloc(sizeof(ImmcInst));
    immcinst->type = type;
    immcinst->dst = dst;
    immcinst->fst_src = fst_src;
    immcinst->snd_src = snd_src;
    return immcinst;
}

ImmcInst* immcinst_copy(ImmcInst* immcinst) {
    ImmcInst* copied_immcinst = malloc(sizeof(ImmcInst));
    copied_immcinst->type = immcinst->type;
    copied_immcinst->dst = NULL;
    if (immcinst->dst != NULL) copied_immcinst->dst = immcope_copy(immcinst->dst);
    copied_immcinst->fst_src = NULL;
    if (immcinst->fst_src != NULL) copied_immcinst->fst_src = immcope_copy(immcinst->fst_src);
    copied_immcinst->snd_src = NULL;
    if (immcinst->snd_src != NULL) copied_immcinst->snd_src = immcope_copy(immcinst->snd_src);
    return copied_immcinst;
}

char* immcinst_tostring(ImmcInst* immcinst) {
    // TODO: fix to prevent buffer overflow
    char* code_str = malloc(200 * sizeof(char));
    memset(code_str, 0, 200 * sizeof(char));

    strcat(code_str, "\t");
    strcat(code_str, immc_insts[immcinst->type]);

    int operand_appears = 0;
    if (immcinst->dst != NULL) {
        char* dst_string = immcope_tostring(immcinst->dst);
        strcat(code_str, "\t");
        strcat(code_str, dst_string);
        free(dst_string);
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
    if (immcinst->dst != NULL) delete_immcope(immcinst->dst);
    if (immcinst->fst_src != NULL) delete_immcope(immcinst->fst_src);
    if (immcinst->snd_src != NULL) delete_immcope(immcinst->snd_src);
    free(immcinst);
}
