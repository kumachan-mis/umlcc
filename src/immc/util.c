#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_label_name(int label_id) {
    // TODO: fix to prevent buffer overflow
    char* label_name = malloc(50 * sizeof(char));
    // TODO: don't use label name which can be conflict with a user-defined identifier
    sprintf(label_name, "L%d", label_id);
    label_name = realloc(label_name, (strlen(label_name) + 1) * sizeof(char));
    return label_name;
}
