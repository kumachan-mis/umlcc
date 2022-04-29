#ifndef _UMLCC_PARSER_EXTERNAL_H_
#define _UMLCC_PARSER_EXTERNAL_H_

#include "./parser.h"

Ast* parse_translation_unit(Parser* parser);
Ast* parse_function_definition(Parser* parser);

#endif
