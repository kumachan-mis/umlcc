#include <stdlib.h>

#include "../ctoken/ctoken.h"
#include "./declaration.h"
#include "./external.h"
#include "./statement.h"
#include "./util.h"

ParserReturn* parse_translation_unit(Parser* parser) {
    Ast* ast = new_ast(AST_TRAS_UNIT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (1) {
        int may_func_def = -1;
        errint_assign(&may_func_def, &err, external_may_function_definition(parser));
        if (err != NULL) break;

        if (may_func_def) {
            parserret_assign(&child_ast, &err, parse_function_definition(parser));
        } else {
            parserret_assign(&child_ast, &err, parse_decl(parser));
        }

        if (err != NULL) break;
        vector_push(ast->children, child_ast);

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_EOF) break;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_function_definition(Parser* parser) {
    Ast* ast = new_ast(AST_FUNC_DEF, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    parserret_assign(&child_ast, &err, parse_declarator(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    parserret_assign(&child_ast, &err, parse_compound_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}
