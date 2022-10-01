#ifndef UMLCC_X64_H_PRIVATE
#define UMLCC_X64_H_PRIVATE
#endif

#include "./label.h"
#include "../common/type.h"

#include <stdlib.h>
#include <string.h>

X64Label* new_x64label(X64LabelType type, X64Visibility visibility, char* name) {
    X64Label* x64label = malloc(sizeof(X64Label));
    x64label->type = type;
    x64label->visibility = visibility;
    x64label->name = name;
    return x64label;
}

X64Label* x64label_copy(X64Label* x64label) {
    X64Label* copied_x64label = malloc(sizeof(X64Label));
    copied_x64label->type = x64label->type;
    copied_x64label->visibility = x64label->visibility;
    copied_x64label->name = new_string(x64label->name);
    return copied_x64label;
}

char* x64label_tostring(X64Label* x64label) {
    // TODO: fix to prevent buffer overflow
    char* label_str = malloc(200 * sizeof(char));
    memset(label_str, 0, 200 * sizeof(char));

    switch (x64label->type) {
        case X64_LABEL_FUNCTION:
            strcat(label_str, "\t.text\n");
            break;
        case X64_LABEL_VARIABLE:
            strcat(label_str, "\t.data\n");
            break;
        default:
            break;
    }

    switch (x64label->visibility) {
        case X64_VIS_GLOBAL:
            strcat(label_str, "\t.globl ");
            strcat(label_str, x64label->name);
            strcat(label_str, "\n");
            break;
        case X64_VIS_LOCAL:
            strcat(label_str, "\t.local ");
            strcat(label_str, x64label->name);
            strcat(label_str, "\n");
            break;
        default:
            break;
    }

    strcat(label_str, x64label->name);
    strcat(label_str, ":\n");
    label_str = realloc(label_str, (strlen(label_str) + 1) * sizeof(char));
    return label_str;
}

void delete_x64label(X64Label* x64label) {
    free(x64label->name);
    free(x64label);
}
