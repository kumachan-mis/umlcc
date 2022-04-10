#ifndef _UMLCC_PARSER_UTIL_H_
#define _UMLCC_PARSER_UTIL_H_

#include "./parser.h"
#include "../token/token.h"


void consume_token(Parser* parser, TokenType token_type);

#endif
