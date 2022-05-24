#ifndef UMLCC_LEXER_H
#define UMLCC_LEXER_H

#include "../ctoken/ctoken.h"
#include "../map/map.h"
#include "../vector/vector.h"

#include <stdio.h>

typedef struct Lexer {
    FILE* file_ptr;
    Map* keyword_map;
    Map* punctuator_map;
} Lexer;

Lexer* new_lexer(FILE* file_ptr);
Vector* lexer_read_ctokens(Lexer* lexer);
void delete_lexer(Lexer* lexer);

#endif
