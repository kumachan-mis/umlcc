#ifndef UMLCC_CTOKEN_H
#define UMLCC_CTOKEN_H

#include "../common/type.h"

typedef enum CTokenType {
    // keyword
    CTOKEN_KEYWORD_CHAR,
    CTOKEN_KEYWORD_INT,
    CTOKEN_KEYWORD_RETURN,

    // identifier
    CTOKEN_IDENT,

    // constant
    CTOKEN_INT,
    CTOKEN_CHAR,
    CTOKEN_STRING,

    // punctuator
    CTOKEN_LBRACKET,
    CTOKEN_RBRACKET,
    CTOKEN_LPALEN,
    CTOKEN_RPALEN,
    CTOKEN_LBRACE,
    CTOKEN_RBRACE,
    CTOKEN_AND,
    CTOKEN_ASTERISK,
    CTOKEN_PLUS,
    CTOKEN_MINUS,
    CTOKEN_EXCLAM,
    CTOKEN_SLASH,
    CTOKEN_PERCENT,
    CTOKEN_EQUAL_EQUAL,
    CTOKEN_EXCLAM_EQUAL,
    CTOKEN_AND_AND,
    CTOKEN_VBAR_VBAR,
    CTOKEN_SEMICOLON,
    CTOKEN_EQUAL,
    CTOKEN_COMMA,
    CTOKEN_EOF,
} CTokenType;

typedef struct CToken {
    CTokenType type;
    char* ident_name;
    int value_int;
    char* value_str;
    int size_str;
} CToken;

extern BaseType t_ctoken;

CToken* new_ctoken(CTokenType type);
CToken* new_identifier_ctoken(CTokenType type, char* name);
CToken* new_integer_ctoken(CTokenType type, int value);
CToken* new_string_literal_ctoken(CTokenType type, char* value, int size);
CToken* ctoken_copy(CToken* ctoken);
void delete_ctoken(CToken* ctoken);

#endif
