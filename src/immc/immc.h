#ifndef UMLCC_IMMC_H
#define UMLCC_IMMC_H

#include "../common/type.h"
#include "./data.h"
#include "./inst.h"
#include "./label.h"

typedef enum ImmcType {
    IMMC_INST,
    IMMC_DATA,
    IMMC_LABEL,
} ImmcType;

typedef struct Immc {
    ImmcType type;
    ImmcInst* inst;
    ImmcData* data;
    ImmcLabel* label;
} Immc;

extern BaseType t_immc;

Immc* new_inst_immc(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src);
Immc* new_imm_data_immc(ImmcDataType type, int value);
Immc* new_str_data_immc(ImmcDataType type, char* value, int size);
Immc* new_label_immc(ImmcLabelType type, ImmcVisibility visibility, char* name);
Immc* immc_copy(Immc* immc);
char* immc_tostring(Immc* immc);
void delete_immc(Immc* immc);

#endif
