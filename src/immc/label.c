#include "./label.h"
#include "../common/common.h"

#include <stdlib.h>
#include <string.h>

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcLabelVis visibility, char* name) {
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
    char* label_str = malloc(100 * sizeof(char));
    memset(label_str, 0, 100 * sizeof(char));

    switch (immclabel->visibility) {
        case LABELVIS_GLOBAL:
            strcat(label_str, "global ");
            break;
        case LABELVIS_LOCAL:
            strcat(label_str, "local ");
            break;
        default:
            break;
    }

    switch (immclabel->type) {
        case LABEL_FUNCTION:
            strcat(label_str, "function ");
            break;
        default:
            break;
    }
    strcat(label_str, immclabel->name);
    strcat(label_str, ":\n");
    label_str = realloc(label_str, (strlen(label_str) + 1) * sizeof(char));
    return label_str;
}

void delete_immclabel(ImmcLabel* immclabel) {
    free(immclabel->name);
    free(immclabel);
}
