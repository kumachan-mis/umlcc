#ifndef UMLCC_LEXER_H
#define UMLCC_LEXER_H

#include "../ctoken/ctoken.h"
#include "../map/map.h"
#include "../set/set.h"
#include "./lexerret.h"

#include <stdlib.h>

typedef struct Lexer {
    char* content;
    size_t content_index;
    size_t content_size;
    Map* keyword_map;
    Set* nondigit_set;
    Map* octdigit_map;
    Map* digit_map;
    Map* hexdigit_map;
    Map* integer_suffix_map;
    Map* punctuator_map;
    Set* white_space_set;
} Lexer;

Lexer* new_lexer(char* content);
LexerReturn* lexer_read_ctokens(Lexer* lexer);
void delete_lexer(Lexer* lexer);

#endif
