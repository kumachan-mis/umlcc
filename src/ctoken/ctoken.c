#include "./ctoken.h"

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
    ctoken->sliteral = NULL;
    return ctoken;
}

CToken* new_identifier_ctoken(CTokenType type, char* name) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = name;
    ctoken->value_int = -1;
    ctoken->sliteral = NULL;
    return ctoken;
}

CToken* new_integer_ctoken(CTokenType type, int value) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = value;
    ctoken->sliteral = NULL;
    return ctoken;
}

CToken* new_sliteral_ctoken(CTokenType type, StringLiteral* sliteral) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->value_int = -1;
    ctoken->sliteral = sliteral;
    return ctoken;
}

CToken* ctoken_copy(CToken* ctoken) {
    CToken* copied_ctoken = malloc(sizeof(CToken));
    copied_ctoken->type = ctoken->type;
    copied_ctoken->ident_name = NULL;
    if (ctoken->ident_name != NULL) copied_ctoken->ident_name = new_string(ctoken->ident_name);
    copied_ctoken->value_int = ctoken->value_int;
    copied_ctoken->sliteral = NULL;
    if (ctoken->sliteral != NULL) copied_ctoken->sliteral = sliteral_copy(ctoken->sliteral);
    return copied_ctoken;
}

void delete_ctoken(CToken* ctoken) {
    if (ctoken->ident_name != NULL) free(ctoken->ident_name);
    if (ctoken->sliteral != NULL) delete_sliteral(ctoken->sliteral);
    free(ctoken);
}
