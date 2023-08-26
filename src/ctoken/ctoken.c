#include "./ctoken.h"

#include <stdlib.h>

BaseType t_ctoken = {
    .copy_object = (void* (*)(void*))ctoken_copy,
    .delete_object = (void (*)(void*))delete_ctoken,
};

char* ctoken_types[] = {
    // non-punctuators
    "char", "enum", "int", "long", "return", "sizeof", "struct", "typedef", "unsigned", "void", "identifier",
    "integer-constant", "character-constant", "string-literal",
    // punctuators
    "[", "]", "(", ")", "{", "}", ".", "->", "&", "*", "+", "-", "!", "/", "%", "==", "!=", "&&", "||", ";", "=", ",",
    "EOF"};

CToken* new_base_ctoken(CTokenType type);

CToken* new_ctoken(CTokenType type) {
    CToken* ctoken = new_base_ctoken(type);
    return ctoken;
}

CToken* new_identifier_ctoken(CTokenType type, char* name) {
    CToken* ctoken = new_ctoken(type);
    ctoken->ident_name = name;
    return ctoken;
}

CToken* new_iliteral_ctoken(CTokenType type, IntegerLiteral* iliteral) {
    CToken* ctoken = new_ctoken(type);
    ctoken->iliteral = iliteral;
    return ctoken;
}

CToken* new_sliteral_ctoken(CTokenType type, StringLiteral* sliteral) {
    CToken* ctoken = new_ctoken(type);
    ctoken->sliteral = sliteral;
    return ctoken;
}

CToken* new_base_ctoken(CTokenType type) {
    CToken* ctoken = malloc(sizeof(CToken));
    ctoken->type = type;
    ctoken->ident_name = NULL;
    ctoken->iliteral = NULL;
    ctoken->sliteral = NULL;
    return ctoken;
}

CToken* ctoken_copy(CToken* ctoken) {
    CToken* copied_ctoken = malloc(sizeof(CToken));
    copied_ctoken->type = ctoken->type;
    copied_ctoken->ident_name = NULL;
    if (ctoken->ident_name != NULL) {
        copied_ctoken->ident_name = new_string(ctoken->ident_name);
    }
    copied_ctoken->iliteral = NULL;
    if (ctoken->iliteral != NULL) {
        copied_ctoken->iliteral = iliteral_copy(ctoken->iliteral);
    }
    copied_ctoken->sliteral = NULL;
    if (ctoken->sliteral != NULL) {
        copied_ctoken->sliteral = sliteral_copy(ctoken->sliteral);
    }
    return copied_ctoken;
}

void delete_ctoken(CToken* ctoken) {
    if (ctoken->ident_name != NULL) {
        free(ctoken->ident_name);
    }
    if (ctoken->iliteral != NULL) {
        delete_iliteral(ctoken->iliteral);
    }
    if (ctoken->sliteral != NULL) {
        delete_sliteral(ctoken->sliteral);
    }
    free(ctoken);
}
