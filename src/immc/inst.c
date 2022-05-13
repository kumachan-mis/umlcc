#include "./inst.h"

#include <stdlib.h>
#include <string.h>

char* inst_strings[] = {"load  ", "addr  ", "store ", "ldarg ", "starg ", "stret ",
                        "add   ", "sub   ", "mul   ", "div   ", "mod   ", "seteq ",
                        "setneq", "jmp   ", "jeq   ", "jneq  ", "call  ", "enter ",
                        "leave ", "prep  ", "clean ", "free  "};

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
