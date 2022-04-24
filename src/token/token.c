#include "./token.h"

#include <stdlib.h>


Token* new_token(TokenType type) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->ident_name = NULL;
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

void delete_token(Token* token) {
    if (token->ident_name != NULL) free(token->ident_name);
    free(token);
}
