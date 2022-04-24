#include "./declaration.h"
#include "./util.h"
#include "../token/token.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>


Ast* parse_decl(Parser* parser) {
    Ast* ast = new_ast(AST_DECL, 1, parse_decl_specifiers(parser));

    Token* token = vector_at(parser->_tokens, parser->_index);
    if (token->type == TOKEN_SEMICOLON) {
        parser->_index++;
        return ast;
    }

    while (1) {
        vector_push(ast->children, parse_init_declarator(parser));
        token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_SEMICOLON) {
            parser->_index++;
            return ast;
        }
        consume_token(parser, TOKEN_COMMA);
    }
}

Ast* parse_decl_specifiers(Parser* parser) {
    Ast* ast = NULL;
    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_KEYWORD_INT:
            parser->_index++;
            ast = new_ast(AST_DECL_SPECIFIERS, 1, new_ast(AST_TYPE_INT, 0));
            return ast;
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }
}

Ast* parse_init_declarator(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR, 1, parse_declarator(parser));
    return ast;
}

Ast* parse_declarator(Parser* parser) {
    Ast* direct = parse_direct_declarator(parser);
    return new_ast(AST_DECLOR, 1, direct);
}

Ast* parse_direct_declarator(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT:
            parser->_index++;
            ast = new_identifier_ast(AST_IDENT_DECLOR, string_copy(token->ident_name));
            break;
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }

    int terminated = 0;
    while (!terminated) {
        token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_LPALEN:
                parser->_index++;
                consume_token(parser, TOKEN_RPALEN);
                ast = new_ast(AST_FUNC_DECLOR, 1, ast);
                break;
            default:
                terminated = 1;
                break;
        }
    }

    return ast;
}

int should_decl(Parser* parser) {
    Token* token = vector_at(parser->_tokens, parser->_index);
    return token->type == TOKEN_KEYWORD_INT;
}
