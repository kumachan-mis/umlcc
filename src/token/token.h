#ifndef _UMLCC_TOKEN_H_
#define _UMLCC_TOKEN_H_

typedef enum _TokenType {
    // keyword
    TOKEN_KEYWORD_INT,

    // identifier
    TOKEN_IDENT,

    // constant
    TOKEN_INT,

    // punctuator
    TOKEN_LPALEN,
    TOKEN_RPALEN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_SEMICOLON,
    TOKEN_EQUAL,
    TOKEN_COMMA,
    TOKEN_EOF,
} TokenType;

typedef struct _Token {
    TokenType type;
    char* ident_name;
    int value_int;
} Token;

Token* new_token(TokenType type);
Token* new_identifier_token(char* name);
Token* new_integer_token(int value);
void delete_token(Token* token);

#endif
