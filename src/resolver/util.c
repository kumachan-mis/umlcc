#include "./util.h"
#include "../ast/ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_sliteral_name(int sliteral_id) {
    char* literal_name = malloc(50 * sizeof(char));
    sprintf(literal_name, "LC%d", sliteral_id);
    literal_name = realloc(literal_name, (strlen(literal_name) + 1) * sizeof(char));
    return literal_name;
}
