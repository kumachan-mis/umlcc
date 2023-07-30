#ifndef UMLCC_LEXER_H
#define UMLCC_LEXER_H

#include "../ctoken/ctoken.h"
#include "../map/map.h"
#include "../set/set.h"
#include "./lexerret.h"

#include <stdio.h>

typedef struct Lexer {
    FILE* file_ptr;
    Map* keyword_map;
    Set* nondigit_set;
    Map* octdigit_map;
    Map* digit_map;
    Map* hexdigit_map;
    Map* integer_suffix_map;
    Map* punctuator_map;
    Set* white_space_set;
} Lexer;

Lexer* new_lexer(FILE* file_ptr);
LexerReturn* lexer_read_ctokens(Lexer* lexer);
void delete_lexer(Lexer* lexer);

#endif
