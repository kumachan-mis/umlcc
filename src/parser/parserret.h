#ifndef UMLCC_PARSER_PARSERRET_H
#define UMLCC_PARSER_PARSERRET_H

#include "../ast/ast.h"
#include "../error/error.h"

typedef struct ParserReturn {
    Ast* ast;
    Error* err;
} ParserReturn;

ParserReturn* new_parserret(Ast* ast);
ParserReturn* new_parserret_error(Error* err);
void parserret_assign(Ast** ast_ptr, Error** err_ptr, ParserReturn* parserret);

#endif
