#include "./parserret.h"

#include <stdlib.h>

ParserReturn* new_parserret(Ast* ast) {
    ParserReturn* parserret = malloc(sizeof(ParserReturn));
    parserret->ast = ast;
    parserret->err = NULL;
    return parserret;
}

ParserReturn* new_parserret_error(Error* err) {
    ParserReturn* parserret = malloc(sizeof(ParserReturn));
    parserret->ast = NULL;
    parserret->err = err;
    return parserret;
}

void parserret_assign(Ast** ast_ptr, Error** err_ptr, ParserReturn* parserret) {
    *ast_ptr = parserret->ast;
    *err_ptr = parserret->err;
    free(parserret);
}
