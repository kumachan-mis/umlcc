#ifndef UMLCC_PARSER_EXTERNAL_H
#define UMLCC_PARSER_EXTERNAL_H

#include "./parser.h"

Ast* parse_translation_unit(Parser* parser);
Ast* parse_function_definition(Parser* parser);

#endif
