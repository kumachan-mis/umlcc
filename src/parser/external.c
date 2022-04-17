#include "./external.h"
#include "../token/token.h"


Ast* parse_translation_unit(Parser* parser) {
    Ast* ast = new_ast(AST_TRAS_UNIT, 0);
    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_EOF) break;
        vector_push(ast->children, parse_function_definition(parser));
    }
    return ast;
}

Ast* parse_function_definition(Parser* parser) {

}
