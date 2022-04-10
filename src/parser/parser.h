#ifndef _UMLCC_PARSER_H_
#define _UMLCC_PARSER_H_

#include "../ast/ast.h"
#include "../vector/vector.h"


typedef struct _Parser {
    Vector* _tokens;
    int _index;
} Parser;

Parser* new_parser(Vector* tokens);
void    delete_parser(Parser* parser);
Ast*    parser_create_ast(Parser* parser);

#endif
