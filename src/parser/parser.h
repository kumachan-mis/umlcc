#ifndef UMLCC_PARSER_H
#define UMLCC_PARSER_H

#include "../ast/ast.h"
#include "../set/set.h"
#include "../vector/vector.h"

typedef struct Parser {
    Vector* ctokens;
    int index;
    Set* typedef_names_set;
    int typedef_flag;
} Parser;

Parser* new_parser(Vector* ctokens);
Ast* parser_create_ast(Parser* parser);
void delete_parser(Parser* parser);

#endif
