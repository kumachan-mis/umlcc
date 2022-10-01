#ifndef UMLCC_IMMC_DATA_H
#define UMLCC_IMMC_DATA_H

#include "../literal/iliteral.h"
#include "../literal/sliteral.h"

typedef enum ImmcDataType {
    IMMC_DATA_BYTE,
    IMMC_DATA_WORD,
    IMMC_DATA_LONG,
    IMMC_DATA_QUAD,
    IMMC_DATA_ZERO,
    IMMC_DATA_STR,
} ImmcDataType;

typedef struct ImmcData {
    ImmcDataType type;
    IntegerLiteral* iliteral;
    StringLiteral* sliteral;
} ImmcData;

#ifdef UMLCC_IMMC_H_PRIVATE
ImmcData* new_int_immcdata(ImmcDataType type, IntegerLiteral* iliteral);
ImmcData* new_str_immcdata(ImmcDataType type, StringLiteral* sliteral);
ImmcData* immcdata_copy(ImmcData* immcdata);
char* immcdata_tostring(ImmcData* immcdata);
void delete_immcdata(ImmcData* immcdata);
#endif

#endif
