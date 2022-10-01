#ifndef UMLCC_IMMC_H_PRIVATE
#define UMLCC_IMMC_H_PRIVATE
#endif

#include "./data.h"
#include "../literal/sliteral.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImmcData* new_int_immcdata(ImmcDataType type, IntegerLiteral* iliteral) {
    ImmcData* immcdata = malloc(sizeof(ImmcData));
    immcdata->type = type;
    immcdata->iliteral = iliteral;
    immcdata->sliteral = NULL;
    return immcdata;
}

ImmcData* new_str_immcdata(ImmcDataType type, StringLiteral* sliteral) {
    ImmcData* immcdata = malloc(sizeof(ImmcData));
    immcdata->type = type;
    immcdata->iliteral = NULL;
    immcdata->sliteral = sliteral;
    return immcdata;
}

ImmcData* immcdata_copy(ImmcData* immcdata) {
    ImmcData* copied_immcdata = malloc(sizeof(ImmcData));
    copied_immcdata->type = immcdata->type;
    copied_immcdata->iliteral = NULL;
    if (immcdata->iliteral != NULL) copied_immcdata->iliteral = iliteral_copy(immcdata->iliteral);
    copied_immcdata->sliteral = NULL;
    if (immcdata->sliteral != NULL) copied_immcdata->sliteral = sliteral_copy(immcdata->sliteral);
    return copied_immcdata;
}

char* immcdata_tostring(ImmcData* immcdata) {
    // TODO: fix to prevent buffer overflow
    char* data_str = malloc(200 * sizeof(char));

    switch (immcdata->type) {
        case IMMC_DATA_BYTE: {
            char* display_str = iliteral_display_string(immcdata->iliteral);
            sprintf(data_str, "\tbyte\t%s\n", display_str);
            free(display_str);
            break;
        }
        case IMMC_DATA_WORD: {
            char* display_str = iliteral_display_string(immcdata->iliteral);
            sprintf(data_str, "\tword\t%s\n", display_str);
            free(display_str);
            break;
        }
        case IMMC_DATA_LONG: {
            char* display_str = iliteral_display_string(immcdata->iliteral);
            sprintf(data_str, "\tlong\t%s\n", display_str);
            free(display_str);
            break;
        }
        case IMMC_DATA_QUAD: {
            char* display_str = iliteral_display_string(immcdata->iliteral);
            sprintf(data_str, "\tquad\t%s\n", display_str);
            free(display_str);
            break;
        }
        case IMMC_DATA_ZERO: {
            char* display_str = iliteral_display_string(immcdata->iliteral);
            sprintf(data_str, "\tzero\t%s\n", display_str);
            free(display_str);
            break;
        }
        case IMMC_DATA_STR: {
            char* display_str = sliteral_display_string(immcdata->sliteral);
            sprintf(data_str, "\tstring\t%s\n", display_str);
            free(display_str);
            break;
        }
    }

    data_str = realloc(data_str, (strlen(data_str) + 1) * sizeof(char));
    return data_str;
}

void delete_immcdata(ImmcData* immcdata) {
    if (immcdata->iliteral != NULL) delete_iliteral(immcdata->iliteral);
    if (immcdata->sliteral != NULL) delete_sliteral(immcdata->sliteral);
    free(immcdata);
}
