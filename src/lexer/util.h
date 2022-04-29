#ifndef _UMLCC_LEXER_UTIL_H_
#define _UMLCC_LEXER_UTIL_H_

#include "../lexer/lexer.h"

char* read_token_str(Lexer* lexer, int startswith(char c), int consistsof(char c));
void skip_white_spaces(Lexer* lexer);

#endif
