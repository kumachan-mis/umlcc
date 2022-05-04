#include "./label.h"

#include <stdlib.h>
#include <string.h>

ImmcLabel* new_immclabel(ImmcLabelType type, char* label_name) {
    ImmcLabel* immclabel = malloc(sizeof(ImmcLabel));
    immclabel->type = type;
    immclabel->label_name = label_name;
    return immclabel;
}

char* immclabel_tostring(ImmcLabel* immclabel) {
    char* label_str = malloc(100 * sizeof(char));
    memset(label_str, 0, 100 * sizeof(char));

    switch (immclabel->type) {
        case LABEL_GLOBAL:
            strcat(label_str, "GLOBAL ");
            break;
        default:
            break;
    }
    strcat(label_str, immclabel->label_name);
    strcat(label_str, ":\n");
    label_str = realloc(label_str, (strlen(label_str) + 1) * sizeof(char));
    return label_str;
}

void delete_immclabel(ImmcLabel* immclabel) {
    free(immclabel->label_name);
    free(immclabel);
}
