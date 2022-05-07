#ifndef _UMLCC_LEXER_H_
#define _UMLCC_LEXER_H_

#include "../map/map.h"
#include "../token/token.h"
#include "../vector/vector.h"

#include <stdio.h>

typedef struct _Lexer {
    FILE* _file_ptr;
    Map* _keyword_map;
    Map* _punctuator_map;
} Lexer;

Lexer* new_lexer(FILE* file_ptr);
void delete_lexer(Lexer* lexer);
Vector* lexer_read_tokens(Lexer* lexer);

#endif
