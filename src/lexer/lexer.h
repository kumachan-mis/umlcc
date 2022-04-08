#ifndef _UMLCC_LEXER_H_
#define _UMLCC_LEXER_H_

#include "../token/token.h"
#include <stdio.h>

typedef struct _Lexer {
    FILE* _file_ptr;
    int _position;
} Lexer;

Lexer* new_lexer(FILE* file_ptr);
Token* read_next_token(Lexer* lexer);
void delete_lexer(Lexer* lexer);

#endif
