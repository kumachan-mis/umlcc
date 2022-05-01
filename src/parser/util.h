#ifndef _UMLCC_PARSER_UTIL_H_
#define _UMLCC_PARSER_UTIL_H_

#include "../token/token.h"
#include "./parser.h"

int external_may_function_definition(Parser* parser);
int blockitem_may_decl(Parser* parser);
void consume_token(Parser* parser, TokenType token_type);

#endif
