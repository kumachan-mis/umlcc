#include "./ctoken.h"

#include <stdlib.h>

BaseType t_ctoken = {
    .copy_object = ctoken_copy,
    .delete_object = delete_ctoken,
};

CToken* new_ctoken(CTokenType type) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = -1;
    return ctoken;
}

CToken* new_identifier_ctoken(char* name) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = CTOKEN_IDENT;
    ctoken->ident_name = name;
    ctoken->value_int = -1;
    return ctoken;
}

CToken* new_integer_ctoken(int value) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = CTOKEN_INT;
    ctoken->ident_name = NULL;
    ctoken->value_int = value;
    return ctoken;
}

CToken* ctoken_copy(CToken* ctoken) {
    CToken* copied_ctoken = malloc(sizeof(CToken));
    copied_ctoken->type = ctoken->type;
    copied_ctoken->ident_name = NULL;
    if (ctoken->ident_name != NULL) copied_ctoken->ident_name = new_string(ctoken->ident_name);
    copied_ctoken->value_int = ctoken->value_int;
    return copied_ctoken;
}

void delete_ctoken(CToken* ctoken) {
    if (ctoken->ident_name != NULL) free(ctoken->ident_name);
    free(ctoken);
}