#include "./immc.h"

#include <stdlib.h>

BaseType t_immc = {
    .copy_object = (void* (*)(void*))immc_copy,
    .delete_object = (void (*)(void*))delete_immc,
};

Immc* new_inst_immc(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src) {
    Immc* immc = malloc(sizeof(Immc));
    immc->type = IMMC_INST;
    immc->inst = new_immcinst(type, dst, fst_src, snd_src);
    immc->data = NULL;
    immc->label = NULL;
    return immc;
}

Immc* new_data_immc(ImmcDataType type, int value) {
    Immc* immc = malloc(sizeof(Immc));
    immc->type = IMMC_DATA;
    immc->inst = NULL;
    immc->data = new_immcdata(type, value);
    immc->label = NULL;
    return immc;
}

Immc* new_label_immc(ImmcLabelType type, ImmcVisibility visibility, char* name) {
    Immc* immc = malloc(sizeof(Immc));
    immc->type = IMMC_LABEL;
    immc->inst = NULL;
    immc->data = NULL;
    immc->label = new_immclabel(type, visibility, name);
    return immc;
}

Immc* immc_copy(Immc* immc) {
    Immc* copied_immc = malloc(sizeof(Immc));
    copied_immc->type = immc->type;

    copied_immc->inst = NULL;
    if (immc->inst != NULL) copied_immc->inst = immcinst_copy(immc->inst);
    copied_immc->data = NULL;
    if (immc->data != NULL) copied_immc->data = immcdata_copy(immc->data);
    copied_immc->label = NULL;
    if (immc->label != NULL) copied_immc->label = immclabel_copy(immc->label);

    return copied_immc;
}

char* immc_tostring(Immc* immc) {
    switch (immc->type) {
        case IMMC_INST:
            return immcinst_tostring(immc->inst);
        case IMMC_DATA:
            return immcdata_tostring(immc->data);
        case IMMC_LABEL:
            return immclabel_tostring(immc->label);
        default:
            return NULL;
    }
}

void delete_immc(Immc* immc) {
    if (immc->inst != NULL) delete_immcinst(immc->inst);
    if (immc->data != NULL) delete_immcdata(immc->data);
    if (immc->label != NULL) delete_immclabel(immc->label);
    free(immc);
}
