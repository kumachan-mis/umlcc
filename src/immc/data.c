#include "./data.h"
#include "../literal/sliteral.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcData* new_imm_immcdata(ImmcDataType type, int value) {
    ImmcData* immcdata = malloc(sizeof(ImmcData));
    immcdata->type = type;
    immcdata->imm_value = value;
    immcdata->sliteral = NULL;
    return immcdata;
}

ImmcData* new_str_immcdata(ImmcDataType type, StringLiteral* sliteral) {
    ImmcData* immcdata = malloc(sizeof(ImmcData));
    immcdata->type = type;
    immcdata->imm_value = -1;
    immcdata->sliteral = sliteral;
    return immcdata;
}

ImmcData* immcdata_copy(ImmcData* immcdata) {
    ImmcData* copied_immcdata = malloc(sizeof(ImmcData));
    copied_immcdata->type = immcdata->type;
    copied_immcdata->imm_value = immcdata->imm_value;
    copied_immcdata->sliteral = NULL;
    if (immcdata->sliteral != NULL) copied_immcdata->sliteral = sliteral_copy(immcdata->sliteral);
    return copied_immcdata;
}

char* immcdata_tostring(ImmcData* immcdata) {
    char* data_str = malloc(200 * sizeof(char));

    switch (immcdata->type) {
        case IMMC_DATA_BYTE:
            sprintf(data_str, "\tbyte %d\n", immcdata->imm_value);
            break;
        case IMMC_DATA_WORD:
            sprintf(data_str, "\tword %d\n", immcdata->imm_value);
            break;
        case IMMC_DATA_LONG:
            sprintf(data_str, "\tlong %d\n", immcdata->imm_value);
            break;
        case IMMC_DATA_QUAD:
            sprintf(data_str, "\tquad %d\n", immcdata->imm_value);
            break;
        case IMMC_DATA_ZERO:
            sprintf(data_str, "\tzero %d\n", immcdata->imm_value);
            break;
        case IMMC_DATA_STR: {
            char* display_str = sliteral_display_string(immcdata->sliteral);
            sprintf(data_str, "\tstring %s\n", display_str);
            free(display_str);
            break;
        }
    }

    data_str = realloc(data_str, (strlen(data_str) + 1) * sizeof(char));
    return data_str;
}

void delete_immcdata(ImmcData* immcdata) {
    free(immcdata);
}

ImmcDataType immcdata_get_type(int size) {
    switch (size) {
        case 1:
            return IMMC_DATA_BYTE;
        case 2:
            return IMMC_DATA_WORD;
        case 4:
            return IMMC_DATA_LONG;
        case 8:
            return IMMC_DATA_QUAD;
        default:
            return IMMC_DATA_ZERO;
    }
}
