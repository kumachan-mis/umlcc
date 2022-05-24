#ifndef UMLCC_LEXER_H
#define UMLCC_LEXER_H

#include "../map/map.h"
#include "../token/token.h"
#include "../vector/vector.h"

#include <stdio.h>

typedef struct Lexer {
    FILE* file_ptr;
    Map* keyword_map;
    Map* punctuator_map;
} Lexer;

Lexer* new_lexer(FILE* file_ptr);
void delete_lexer(Lexer* lexer);
Vector* lexer_read_tokens(Lexer* lexer);

#endif
