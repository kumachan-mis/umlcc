#ifndef UMLCC_LEXER_UTIL_H
#define UMLCC_LEXER_UTIL_H

#include "../lexer/lexer.h"

Map* new_keyword_map(void);
Set* new_nondigit_set(void);
Map* new_octdigit_map(void);
Map* new_digit_map(void);
Map* new_hexdigit_map(void);
Map* new_punctuator_map(void);
Set* new_white_space_set(void);
void skip_white_spaces(Lexer* lexer);

#endif
