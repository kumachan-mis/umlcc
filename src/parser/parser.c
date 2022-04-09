#include "./parser.h"
#include "../token/token.h"
#include "../ast/ast.h"

#include <stdio.h>
#include <stdlib.h>


struct _Parser {
    Vector* tokens;
    int index;
};

Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_primary_expr(Parser* parser);

Parser* new_parser(Vector* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->index = 0;
    return parser;
}

void delete_parser(Parser* parser) {
    delete_vector(parser->tokens, delete_token);
    free(parser);
}

Vector* parser_create_asts(Parser* parser) {
    Vector* asts = new_vector();

    int num_tokens = vector_size(parser->tokens);
    while (parser->index < num_tokens) {
        Ast* ast = parse_additive_expr(parser);
        vector_push(asts, ast);
    }

    return asts;
}

Ast* parse_additive_expr(Parser* parser) {
    Ast* ast = parse_multiplicative_expr(parser);
    Token* token = vector_at(parser->tokens, parser->index);

    while (1) {
        switch (token->type) {
            case TOKEN_PLUS: {
                parser->index++;
                ast = new_ast(AST_ADD, 2, ast, parse_multiplicative_expr(parser));
            }
            case TOKEN_MINUS: {
                parser->index++;
                ast = new_ast(AST_SUB, 2, ast, parse_multiplicative_expr(parser));
            }
            default:
                return ast;
        }
    }
}

Ast* parse_multiplicative_expr(Parser* parser) {
    Ast* ast = parse_primary_expr(parser);
    Token* token = vector_at(parser->tokens, parser->index);

    while (1) {
        switch (token->type) {
            case TOKEN_ASTERISK: {
                parser->index++;
                ast = new_ast(AST_MUL, 2, ast, parse_primary_expr(parser));
            }
            case TOKEN_SLASH: {
                parser->index++;
                ast = new_ast(AST_DIV, 2, ast, parse_primary_expr(parser));
            }
            case TOKEN_PERCENT: {
                parser->index++;
                ast = new_ast(AST_MOD, 2, ast, parse_primary_expr(parser));
            }
            default:
                return ast;
        }
    }
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;
    Token* token = vector_at(parser->tokens, parser->index);

    switch (token->type) {
        case TOKEN_INT:
            parser->index++;
            ast = new_integer_ast(token->value_int);
        default:
            fprintf("Error: unexpected token type %d", token->type);
            exit(1);
    }

    return ast;
}
