#ifndef UMLCC_PARSER_UTIL_H
#define UMLCC_PARSER_UTIL_H

#include "../ctoken/ctoken.h"
#include "./parser.h"

int external_may_function_definition(Parser* parser);
int blockitem_may_decl(Parser* parser);
void consume_ctoken(Parser* parser, CTokenType ctoken_type);

#endif
