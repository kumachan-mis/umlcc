#ifndef UMLCC_LEXER_LEXERRET_H
#define UMLCC_LEXER_LEXERRET_H

#include "../ctoken/ctoken.h"
#include "../error/error.h"
#include "../vector/vector.h"

typedef struct LexerReturn {
    Vector* ctokens;
    Error* err;
} LexerReturn;

typedef struct LexerReturnItem {
    CToken* ctoken;
    Error* err;
} LexerReturnItem;

LexerReturn* new_lexerret(Vector* ctokens);
LexerReturn* new_lexerret_error(Error* err);
void lexerret_assign(Vector** ctokens_ptr, Error** err_ptr, LexerReturn* lexerret);
LexerReturnItem* new_lexerret_item(CToken* ctoken);
LexerReturnItem* new_lexerret_item_error(Error* err);
void lexerret_item_assign(CToken** ctoken_ptr, Error** err_ptr, LexerReturnItem* item);

#endif
