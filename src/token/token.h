#ifndef _UMLCC_TOKEN_H_
#define _UMLCC_TOKEN_H_

typedef enum _TokenType {
    TOKEN_KEYWORD_INT,
    TOKEN_IDENT,
    TOKEN_INT,
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
Token* copy_token(Token* token);
void   delete_token(Token* token);

#endif
