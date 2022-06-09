#ifndef UMLCC_LEXER_UTIL_H
#define UMLCC_LEXER_UTIL_H

#include "../lexer/lexer.h"

Map* new_keyword_map();
Set* new_nondigit_set();
Map* new_octdigit_map();
Map* new_digit_map();
Map* new_hexdigit_map();
Map* new_punctuator_map();
Set* new_white_space_set();
void skip_white_spaces(Lexer* lexer);

#endif
