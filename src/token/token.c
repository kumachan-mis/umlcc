#include "./token.h"

#include <stdlib.h>

BaseType t_token = {
    .copy_object = token_copy,
    .delete_object = delete_token,
};

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

Token* token_copy(Token* token) {
    Token* copied_token = malloc(sizeof(Token));
    copied_token->type = token->type;
    copied_token->ident_name = NULL;
    if (token->ident_name != NULL) copied_token->ident_name = new_string(token->ident_name);
    copied_token->value_int = token->value_int;
    return copied_token;
}
