#ifndef UMLCC_IMMC_H_PRIVATE
#define UMLCC_IMMC_H_PRIVATE
#endif

#include "./label.h"
#include "../common/type.h"

#include <stdlib.h>
#include <string.h>

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcVisibility visibility, char* name) {
    ImmcLabel* immclabel = malloc(sizeof(ImmcLabel));
    immclabel->type = type;
    immclabel->visibility = visibility;
    immclabel->name = name;
    return immclabel;
}

ImmcLabel* immclabel_copy(ImmcLabel* immclabel) {
    ImmcLabel* copied_immclabel = malloc(sizeof(ImmcLabel));
    copied_immclabel->type = immclabel->type;
    copied_immclabel->visibility = immclabel->visibility;
    copied_immclabel->name = new_string(immclabel->name);
    return copied_immclabel;
}

char* immclabel_tostring(ImmcLabel* immclabel) {
    // TODO: fix to prevent buffer overflow
    char* label_str = malloc(200 * sizeof(char));
    memset(label_str, 0, 200 * sizeof(char));

    switch (immclabel->visibility) {
        case IMMC_VIS_GLOBAL:
            strcat(label_str, "\tglobal\n");
            break;
        case IMMC_VIS_LOCAL:
            strcat(label_str, "\tlocal\n");
            break;
        default:
            break;
    }

    switch (immclabel->type) {
        case IMMC_LABEL_FUNCTION:
            strcat(label_str, "function ");
            strcat(label_str, immclabel->name);
            strcat(label_str, ":\n");
            break;
        case IMMC_LABEL_VARIABLE:
            strcat(label_str, "variable ");
            strcat(label_str, immclabel->name);
            strcat(label_str, ":\n");
            break;
        default:
            strcat(label_str, immclabel->name);
            strcat(label_str, ":\n");
            break;
    }

    label_str = realloc(label_str, (strlen(label_str) + 1) * sizeof(char));
    return label_str;
}

void delete_immclabel(ImmcLabel* immclabel) {
    free(immclabel->name);
    free(immclabel);
}
