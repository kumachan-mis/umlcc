#ifndef _UMLCC_TOKEN_H_
#define _UMLCC_TOKEN_H_

typedef enum _TokenType {
    TOKEN_IDENT,
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_EOF,
} TokenType;

typedef struct _Token {
    TokenType type;
    int value_int;
} Token;

Token* new_reverved_token(TokenType type);
Token* new_integer_token(int value);
Token* copy_token(Token* token);
void delete_token(Token* token);

#endif
