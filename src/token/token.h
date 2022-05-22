#ifndef _UMLCC_TOKEN_H_
#define _UMLCC_TOKEN_H_

#include "../common/common.h"

typedef enum _TokenType {
    // keyword
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_RETURN,

    // identifier
    TOKEN_IDENT,

    // constant
    TOKEN_INT,

    // punctuator
    TOKEN_LPALEN,
    TOKEN_RPALEN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_AND,
    TOKEN_ASTERISK,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_EXCLAM,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_EQUAL_EQUAL,
    TOKEN_EXCLAM_EQUAL,
    TOKEN_AND_AND,
    TOKEN_VBAR_VBAR,
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

extern BaseType t_token;

Token* new_token(TokenType type);
Token* new_identifier_token(char* name);
Token* new_integer_token(int value);
void delete_token(Token* token);
Token* token_copy(Token* token);

#endif
