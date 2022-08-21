#ifndef UMLCC_PARSER_EXTERNAL_H
#define UMLCC_PARSER_EXTERNAL_H

#include "./parser.h"

ParserReturn* parse_translation_unit(Parser* parser);
ParserReturn* parse_function_definition(Parser* parser);

#endif
