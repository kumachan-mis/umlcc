#include "./token.h"

#include <stdlib.h>
#include <string.h>


char* copy_token_ident_name(char* ident_name);

Token* new_token(TokenType type) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value_int = -1;
    return token;
}

Token* new_identifier_token(char* name) {
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_IDENT;
    token->ident_name = name;
    token->value_int = -1;
    return token;
}

Token* new_integer_token(int value) {
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_INT;
    token->ident_name = NULL;
    token->value_int = value;
    return token;
}

Token* copy_token(Token* token) {
    Token* copied_token = malloc(sizeof(Token));
    copied_token->type = token->type;
    copied_token->ident_name = copy_token_ident_name(token->ident_name);
    copied_token->value_int = token->value_int;
    return copied_token;
}

void delete_token(Token* token) {
    if (token->ident_name != NULL) free(token->ident_name);
    free(token);
}

char* copy_token_ident_name(char* ident_name) {
    if (ident_name == NULL) return NULL;
    char* copied_ident_name = malloc((strlen(ident_name) + 1) * sizeof(char));
    strcpy(copied_ident_name, ident_name);
    return copied_ident_name;
}
