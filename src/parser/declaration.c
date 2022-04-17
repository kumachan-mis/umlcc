#include "./declaration.h"
#include "./util.h"
#include "../token/token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Ast* parse_decl(Parser* parser) {
    Ast* ast = new_ast(AST_DECL, 1, parse_declaration_specifiers(parser));

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

Ast* parse_declaration_specifiers(Parser* parser) {
    Ast* ast = NULL;
    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_KEYWORD_INT:
            parser->_index++;
            ast = new_ast(AST_DECL_SPECS, 1, new_ast(AST_TYPE_INT, 0));
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
    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT: {
            parser->_index++;
            char* ident_name = malloc((strlen(token->ident_name) + 1) * sizeof(char));
            strcpy(ident_name, token->ident_name);
            return new_identifier_ast(ident_name);
        }
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }
}

int should_decl(Parser* parser) {
    Token* token = vector_at(parser->_tokens, parser->_index);
    return token->type == TOKEN_KEYWORD_INT;
}
