#include "./token.h"

#include <stdlib.h>
#include <string.h>


Token* new_token(TokenType type) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value_int = -1;
    return token;
}

Token* new_integer_token(int value) {
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_INT;
    token->value_int = value;
    return token;
}

Token* copy_token(Token* token) {
    Token* copied_token = malloc(sizeof(Token));
    copied_token->type = token->type;
    copied_token->value_int = token->value_int;
    return copied_token;
}

void delete_token(Token* token) {
    free(token);
}
