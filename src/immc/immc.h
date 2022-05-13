#ifndef _UMLCC_IMMC_H_
#define _UMLCC_IMMC_H_

#include "./inst.h"
#include "./label.h"

typedef enum _ImmcType {
    IMMC_INST,
    IMMC_LABEL,
} ImmcType;

typedef struct _Immc {
    ImmcType type;
    ImmcInst* inst;
    ImmcLabel* label;
} Immc;

Immc* new_inst_immc(ImmcInstType type, ImmcOpe* dest, ImmcOpe* fst_src, ImmcOpe* snd_src);
Immc* new_label_immc(ImmcLabelType type, ImmcLabelVis visibility, char* name);
char* immc_tostring(Immc* immc);
void delete_immc(Immc* immc);

#endif
