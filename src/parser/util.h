#ifndef UMLCC_PARSER_UTIL_H
#define UMLCC_PARSER_UTIL_H

#include "../ctoken/ctoken.h"
#include "../error/errint.h"
#include "./parser.h"

ErrorableInt* external_may_function_definition(Parser* parser);
ErrorableInt* blockitem_may_decl(Parser* parser);
Error* consume_ctoken(Parser* parser, CTokenType ctoken_type);

#endif
