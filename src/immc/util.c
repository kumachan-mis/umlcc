#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_label_name(int label_id) {
    // TODO: fix to prevent buffer overflow
    char* label_name = malloc(50 * sizeof(char));
    sprintf(label_name, ".L%d", label_id);
    label_name = realloc(label_name, (strlen(label_name) + 1) * sizeof(char));
    return label_name;
}

char* create_sliteral_label(int sliteral_id) {
    // TODO: fix to prevent buffer overflow
    char* sliteral_name = malloc(50 * sizeof(char));
    sprintf(sliteral_name, ".SL%d", sliteral_id);
    sliteral_name = realloc(sliteral_name, (strlen(sliteral_name) + 1) * sizeof(char));
    return sliteral_name;
}
