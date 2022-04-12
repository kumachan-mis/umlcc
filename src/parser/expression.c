#include "./expression.h"
#include "./util.h"
#include "../token/token.h"

#include <stdio.h>
#include <stdlib.h>


Ast* parse_assignment_expr(Parser* parser);
Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_primary_expr(Parser* parser);


Ast* parse_expr(Parser* parser) {
    Ast* ast = parse_assignment_expr(parser);
    return ast;
}

Ast* parse_assignment_expr(Parser* parser) {
   Vector* stack = new_vector();
   int terminated = 0;

    while (!terminated) {
        int index = parser->_index;
        Ast* ast = parse_primary_expr(parser);

        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_EQUAL:
                parser->_index++;
                vector_push(stack, new_ast(AST_ASSIGN_EXPR, 0, ast));
                break;
            default:
                delete_ast(ast);
                parser->_index = index;
                vector_push(stack, parse_additive_expr(parser));
                terminated = 1;
                break;
        }
   }

   Ast* ast = vector_pop(stack);
   while (vector_size(stack) > 0) {
       Ast* sub_ast = vector_pop(stack);
       vector_push(sub_ast->children, ast);
       ast = sub_ast;
   }

   return ast;
}

Ast* parse_additive_expr(Parser* parser) {
    Ast* ast = parse_multiplicative_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_PLUS:
                parser->_index++;
                ast = new_ast(AST_ADD_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            case TOKEN_MINUS:
                parser->_index++;
                ast = new_ast(AST_SUB_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_multiplicative_expr(Parser* parser) {
    Ast* ast = parse_primary_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_ASTERISK:
                parser->_index++;
                ast = new_ast(AST_MUL_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_SLASH:
                parser->_index++;
                ast = new_ast(AST_DIV_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_PERCENT:
                parser->_index++;
                ast = new_ast(AST_MOD_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT: {
            char* ident_name = malloc((strlen(token->ident_name) + 1) * sizeof(char));
            strcpy(ident_name, token->ident_name);
            ast = new_identifier_ast(ident_name);
            break;
        }
        case TOKEN_INT:
            parser->_index++;
            ast = new_integer_ast(token->value_int);
            break;
        case TOKEN_LPALEN:
            parser->_index++;
            ast = parse_expr(parser);
            consume_token(parser, TOKEN_RPALEN);
            break;
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }

    return ast;
}
