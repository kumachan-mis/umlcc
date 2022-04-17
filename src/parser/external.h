#ifndef _UMLCC_PARSER_EXTERNAL_
#define _UMLCC_PARSER_EXTERNAL_

#include "./parser.h"


Ast* parse_translation_unit(Parser* parser);
Ast* parse_function_definition(Parser* parser);

#endif
