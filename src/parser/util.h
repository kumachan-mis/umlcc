#ifndef UMLCC_PARSER_UTIL_H
#define UMLCC_PARSER_UTIL_H

#include "../ctoken/ctoken.h"
#include "../error/errint.h"
#include "./parser.h"

ErrorableInt* external_may_function_definition(Parser* parser);
int item_may_decl(Parser* parser);
int ctoken_is_storage_class_specifier(CToken* ctoken);
int ctoken_is_type_specifier(CToken* ctoken, Set* typedef_names_set);
Error* consume_ctoken(Parser* parser, CTokenType ctoken_type);

#endif
