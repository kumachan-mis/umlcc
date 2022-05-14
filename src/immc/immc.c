#include "./immc.h"

#include <stdlib.h>

BaseType t_immc = {
    .delete_object = delete_immc,
};

Immc* new_inst_immc(ImmcInstType type, ImmcOpe* dest, ImmcOpe* fst_src, ImmcOpe* snd_src) {
    Immc* immc = malloc(sizeof(Immc));
    immc->type = IMMC_INST;
    immc->inst = new_immcinst(type, dest, fst_src, snd_src);
    immc->label = NULL;
    return immc;
}

Immc* new_label_immc(ImmcLabelType type, ImmcLabelVis visibility, char* name) {
    Immc* immc = malloc(sizeof(Immc));
    immc->type = IMMC_LABEL;
    immc->inst = NULL;
    immc->label = new_immclabel(type, visibility, name);
    return immc;
}

char* immc_tostring(Immc* immc) {
    switch (immc->type) {
        case IMMC_INST:
            return immcinst_tostring(immc->inst);
        case IMMC_LABEL:
            return immclabel_tostring(immc->label);
        default:
            return NULL;
    }
}

void delete_immc(Immc* immc) {
    if (immc->inst != NULL) delete_immcinst(immc->inst);
    if (immc->label != NULL) delete_immclabel(immc->label);
    free(immc);
}
