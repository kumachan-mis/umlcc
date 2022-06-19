#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_string_literal_name(int string_literal_id) {
    char* literal_name = malloc(50 * sizeof(char));
    sprintf(literal_name, "LC%d", string_literal_id);
    literal_name = realloc(literal_name, (strlen(literal_name) + 1) * sizeof(char));
    return literal_name;
}
