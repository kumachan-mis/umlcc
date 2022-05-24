#ifndef UMLCC_CTOKEN_H
#define UMLCC_CTOKEN_H

#include "../common/common.h"

typedef enum CTokenType {
    // keyword
    CTOKEN_KEYWORD_INT,
    CTOKEN_KEYWORD_RETURN,

    // identifier
    CTOKEN_IDENT,

    // constant
    CTOKEN_INT,

    // punctuator
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
} CToken;

extern BaseType t_ctoken;

CToken* new_ctoken(CTokenType type);
CToken* new_identifier_ctoken(char* name);
CToken* new_integer_ctoken(int value);
CToken* ctoken_copy(CToken* ctoken);
void delete_ctoken(CToken* ctoken);

#endif
