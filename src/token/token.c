#include "./token.h"

#include <stdlib.h>
#include <string.h>


Token* new_token(TokenType type, char* value) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

Token* copy_token(Token* token) {
    char* value = malloc(sizeof(char) * (strlen(token->value) + 1));
    strcpy(token->value, value);
    return new_token(token->type, value);
}

void delete_token(Token* token) {
    free(token->value);
    free(token);
}
