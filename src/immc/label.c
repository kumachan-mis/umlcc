#include "./label.h"

#include <stdlib.h>
#include <string.h>

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcLabelVisibility visibility, char* label_name) {
    ImmcLabel* immclabel = malloc(sizeof(ImmcLabel));
    immclabel->type = type;
    immclabel->visibility = visibility;
    immclabel->label_name = label_name;
    return immclabel;
}

char* immclabel_tostring(ImmcLabel* immclabel) {
    char* label_str = malloc(100 * sizeof(char));
    memset(label_str, 0, 100 * sizeof(char));

    switch (immclabel->visibility) {
        case VISIBILITY_GLOBAL:
            strcat(label_str, "global ");
            break;
        case VISIBILITY_LOCAL:
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
    strcat(label_str, immclabel->label_name);
    strcat(label_str, ":\n");
    label_str = realloc(label_str, (strlen(label_str) + 1) * sizeof(char));
    return label_str;
}

void delete_immclabel(ImmcLabel* immclabel) {
    free(immclabel->label_name);
    free(immclabel);
}
