#include <stdlib.h>

#include "../ctoken/ctoken.h"
#include "./declaration.h"
#include "./external.h"
#include "./statement.h"
#include "./util.h"

Ast* parse_translation_unit(Parser* parser) {
    Ast* ast = new_ast(AST_TRAS_UNIT, 0);
    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_EOF) break;
        if (external_may_function_definition(parser)) {
            vector_push(ast->children, parse_function_definition(parser));
        } else {
            vector_push(ast->children, parse_decl(parser));
        }
    }
    return ast;
}

Ast* parse_function_definition(Parser* parser) {
    Ast* ast = new_ast(AST_FUNC_DEF, 0);
    vector_push(ast->children, parse_decl_specifiers(parser));
    vector_push(ast->children, parse_declarator(parser));
    vector_push(ast->children, parse_compound_stmt(parser));
    return ast;
}
