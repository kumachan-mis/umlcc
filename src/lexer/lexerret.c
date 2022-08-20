#include "./lexerret.h"

#include <stdlib.h>

LexerReturn* new_lexerret(Vector* ctokens) {
    LexerReturn* lexerret = malloc(sizeof(LexerReturn));
    lexerret->ctokens = ctokens;
    lexerret->err = NULL;
    return lexerret;
}

LexerReturn* new_lexerret_error(Error* err) {
    LexerReturn* lexerret = malloc(sizeof(LexerReturn));
    lexerret->ctokens = NULL;
    lexerret->err = err;
    return lexerret;
}

void lexerret_assign(Vector** ctokens_ptr, Error** err_ptr, LexerReturn* lexerret) {
    *ctokens_ptr = lexerret->ctokens;
    *err_ptr = lexerret->err;
    free(lexerret);
}

LexerReturnItem* new_lexerret_item(CToken* ctoken) {
    LexerReturnItem* item = malloc(sizeof(LexerReturnItem));
    item->ctoken = ctoken;
    item->err = NULL;
    return item;
}

LexerReturnItem* new_lexerret_item_error(Error* err) {
    LexerReturnItem* item = malloc(sizeof(LexerReturnItem));
    item->ctoken = NULL;
    item->err = err;
    return item;
}

void lexerret_item_assign(CToken** ctoken_ptr, Error** err_ptr, LexerReturnItem* item) {
    *ctoken_ptr = item->ctoken;
    *err_ptr = item->err;
    free(item);
}
