#ifndef UMLCC_LEXER_UTIL_H
#define UMLCC_LEXER_UTIL_H

#include "../lexer/lexer.h"

Map* new_keyword_map();
Map* new_punctuator_map();
void skip_white_spaces(Lexer* lexer);

#endif
