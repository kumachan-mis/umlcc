#ifndef UMLCC_PARSER_H
#define UMLCC_PARSER_H

#include "../ast/ast.h"
#include "../vector/vector.h"

typedef struct Parser {
    Vector* ctokens;
    int index;
} Parser;

Parser* new_parser(Vector* ctokens);
void delete_parser(Parser* parser);
Ast* parser_create_ast(Parser* parser);

#endif
