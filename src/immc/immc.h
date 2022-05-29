#ifndef UMLCC_IMMC_H
#define UMLCC_IMMC_H

#include "../common/common.h"
#include "./inst.h"
#include "./label.h"

typedef enum ImmcType {
    IMMC_INST,
    IMMC_LABEL,
} ImmcType;

typedef struct Immc {
    ImmcType type;
    ImmcInst* inst;
    ImmcLabel* label;
} Immc;

extern BaseType t_immc;

Immc* new_inst_immc(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src);
Immc* new_label_immc(ImmcLabelType type, ImmcVisibility visibility, char* name);
Immc* immc_copy(Immc* immc);
char* immc_tostring(Immc* immc);
void delete_immc(Immc* immc);

#endif
