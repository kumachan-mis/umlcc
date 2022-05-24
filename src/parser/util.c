#include "./util.h"
#include "./declaration.h"

#include <stdio.h>
#include <stdlib.h>

int external_may_function_definition(Parser* parser) {
    Ast* ast = NULL;
    int index = parser->index;

    ast = parse_decl_specifiers(parser);
    delete_ast(ast);
    ast = parse_declarator(parser);
    delete_ast(ast);

    Token* token = vector_at(parser->tokens, parser->index);
    parser->index = index;
    return token->type == TOKEN_LBRACE;
}

int blockitem_may_decl(Parser* parser) {
    Token* token = vector_at(parser->tokens, parser->index);
    return token->type == TOKEN_KEYWORD_INT;
}

void consume_token(Parser* parser, TokenType token_type) {
    Token* token = vector_at(parser->tokens, parser->index);
    if (token->type == token_type) {
        parser->index++;
        return;
    }
    fprintf(stderr, "Error: unexpected token type %d\n", token->type);
    exit(1);
}
