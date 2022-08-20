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

void lexerret_close(LexerReturn* lexerret) {
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

void lexerret_item_close(LexerReturnItem* item) {
    free(item);
}
