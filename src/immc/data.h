#ifndef UMLCC_IMMC_DATA_H
#define UMLCC_IMMC_DATA_H

typedef enum ImmcDataType {
    IMMC_DATA_BYTE,
    IMMC_DATA_WORD,
    IMMC_DATA_LONG,
    IMMC_DATA_QUAD,
    IMMC_DATA_ZERO,
} ImmcDataType;

typedef struct ImmcData {
    ImmcDataType type;
    int value;
} ImmcData;

ImmcData* new_immcdata(ImmcDataType type, int value);
ImmcData* immcdata_copy(ImmcData* immcdata);
char* immcdata_tostring(ImmcData* immcdata);
void delete_immcdata(ImmcData* immcdata);
ImmcDataType immcdata_get_type(int size);

#endif
