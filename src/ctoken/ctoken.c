#include "./ctoken.h"
#include "../common/util.h"

#include <stdlib.h>

BaseType t_ctoken = {
    .copy_object = (void* (*)(void*))ctoken_copy,
    .delete_object = (void (*)(void*))delete_ctoken,
};

CToken* new_ctoken(CTokenType type) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = -1;
    ctoken->value_str = NULL;
    ctoken->size_str = -1;
    return ctoken;
}

CToken* new_identifier_ctoken(CTokenType type, char* name) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = name;
    ctoken->value_int = -1;
    ctoken->value_str = NULL;
    ctoken->size_str = -1;
    return ctoken;
}

CToken* new_integer_ctoken(CTokenType type, int value) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = value;
    ctoken->value_str = NULL;
    ctoken->size_str = -1;
    return ctoken;
}

CToken* new_string_literal_ctoken(CTokenType type, char* value, int size) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = -1;
    ctoken->value_str = value;
    ctoken->size_str = size;
    return ctoken;
}

CToken* ctoken_copy(CToken* ctoken) {
    CToken* copied_ctoken = malloc(sizeof(CToken));
    copied_ctoken->type = ctoken->type;
    copied_ctoken->ident_name = NULL;
    if (ctoken->ident_name != NULL) copied_ctoken->ident_name = new_string(ctoken->ident_name);
    copied_ctoken->value_int = ctoken->value_int;
    copied_ctoken->value_str = NULL;
    if (ctoken->value_str != NULL) {
        copied_ctoken->value_str = copy_charmem(ctoken->value_str, ctoken->size_str);
    }
    copied_ctoken->size_str = ctoken->size_str;
    return copied_ctoken;
}

void delete_ctoken(CToken* ctoken) {
    if (ctoken->ident_name != NULL) free(ctoken->ident_name);
    if (ctoken->value_str != NULL) free(ctoken->value_str);
    free(ctoken);
}
