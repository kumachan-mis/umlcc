#ifndef UMLCC_PARSER_H
#define UMLCC_PARSER_H

#include "../set/set.h"
#include "../vector/vector.h"
#include "./parserret.h"

typedef struct Parser {
    Vector* ctokens;
    int index;
    Set* typedef_names_set;
    int typedef_flag;
} Parser;

Parser* new_parser(Vector* ctokens);
ParserReturn* parser_create_ast(Parser* parser);
void delete_parser(Parser* parser);

#endif
