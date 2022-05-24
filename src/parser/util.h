#ifndef UMLCC_PARSER_UTIL_H
#define UMLCC_PARSER_UTIL_H

#include "../token/token.h"
#include "./parser.h"

int external_may_function_definition(Parser* parser);
int blockitem_may_decl(Parser* parser);
void consume_token(Parser* parser, TokenType token_type);

#endif
