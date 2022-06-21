#ifndef UMLCC_IMMC_DATA_H
#define UMLCC_IMMC_DATA_H

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
    int imm_value;
    char* str_value;
    int str_size;
} ImmcData;

ImmcData* new_imm_immcdata(ImmcDataType type, int value);
ImmcData* new_str_immcdata(ImmcDataType type, char* value, int size);
ImmcData* immcdata_copy(ImmcData* immcdata);
char* immcdata_tostring(ImmcData* immcdata);
void delete_immcdata(ImmcData* immcdata);
ImmcDataType immcdata_get_type(int size);

#endif
