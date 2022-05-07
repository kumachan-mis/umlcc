#ifndef _UMLCC_LEXER_UTIL_H_
#define _UMLCC_LEXER_UTIL_H_

#include "../lexer/lexer.h"

Map* new_keyword_map();
Map* new_punctuator_map();
void delete_token_map(Map* token_map);
void skip_white_spaces(Lexer* lexer);

#endif
