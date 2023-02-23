#ifndef UMLCC_CTOKEN_H
#define UMLCC_CTOKEN_H

#include "../common/type.h"
#include "../literal/iliteral.h"
#include "../literal/sliteral.h"

typedef enum CTokenType {
    // keyword
    CTOKEN_KEYWORD_CHAR,
    CTOKEN_KEYWORD_ENUM,
    CTOKEN_KEYWORD_INT,
    CTOKEN_KEYWORD_RETURN,
    CTOKEN_KEYWORD_STRUCT,
    CTOKEN_KEYWORD_TYPEDEF,

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
    CTOKEN_DOT,
    CTOKEN_ARROW,
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
    IntegerLiteral* iliteral;
    StringLiteral* sliteral;
} CToken;

extern BaseType t_ctoken;

extern char* ctoken_types[];

CToken* new_ctoken(CTokenType type);
CToken* new_identifier_ctoken(CTokenType type, char* name);
CToken* new_iliteral_ctoken(CTokenType type, IntegerLiteral* iliteral);
CToken* new_sliteral_ctoken(CTokenType type, StringLiteral* sliteral);
CToken* ctoken_copy(CToken* ctoken);
void delete_ctoken(CToken* ctoken);

#endif
