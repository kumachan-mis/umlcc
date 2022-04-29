#ifndef _UMLCC_PARSER_UTIL_H_
#define _UMLCC_PARSER_UTIL_H_

#include "../token/token.h"
#include "./parser.h"

void consume_token(Parser* parser, TokenType token_type);

#endif
