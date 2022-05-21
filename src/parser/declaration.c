#include "./declaration.h"
#include "../common/common.h"
#include "../token/token.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Ast* parse_decl(Parser* parser) {
    Ast* specifiers_ast = parse_decl_specifiers(parser);
    Ast* init_list_ast = parse_init_declarator_list(parser);
    consume_token(parser, TOKEN_SEMICOLON);
    return new_ast(AST_DECL, 2, specifiers_ast, init_list_ast);
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

Ast* parse_init_declarator_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR_LIST, 0);

    Token* token = vector_at(parser->_tokens, parser->_index);
    if (token->type == TOKEN_SEMICOLON) return ast;

    while (1) {
        vector_push(ast->children, parse_init_declarator(parser));
        token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_SEMICOLON) break;
        consume_token(parser, TOKEN_COMMA);
    }

    return ast;
}

Ast* parse_init_declarator(Parser* parser) {
    Ast* declarator_ast = parse_declarator(parser);
    return new_ast(AST_INIT_DECLOR, 1, declarator_ast);
}

Ast* parse_declarator(Parser* parser) {
    Ast* pointer_ast = parse_pointer(parser);
    Ast* direct_declarator_ast = parse_direct_declarator(parser);
    if (pointer_ast == NULL) return direct_declarator_ast;

    Ast* pointer_tail = pointer_ast;
    while (vector_size(pointer_tail->children) > 0) {
        pointer_tail = vector_at(pointer_tail->children, 0);
    }

    if (direct_declarator_ast->type == AST_IDENT_DECLOR) {
        vector_push(pointer_tail->children, direct_declarator_ast);
        return pointer_ast;
    }

    Ast* func_or_array_tail = direct_declarator_ast;
    while (1) {
        Ast* child = vector_at(func_or_array_tail->children, 0);
        if (child->type == AST_IDENT_DECLOR) break;
        func_or_array_tail = child;
    }

    Ast* ident_ast = ast_copy(vector_at(func_or_array_tail->children, 0));
    vector_set(func_or_array_tail->children, 0, pointer_ast);
    vector_push(pointer_tail->children, ident_ast);
    return direct_declarator_ast;
}

Ast* parse_pointer(Parser* parser) {
    Ast* ast = NULL;
    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        if (token->type != TOKEN_ASTERISK) break;
        parser->_index++;
        if (ast == NULL) {
            ast = new_ast(AST_PTR_DECLOR, 0);
        } else {
            ast = new_ast(AST_PTR_DECLOR, 1, ast);
        }
    }
    return ast;
}

Ast* parse_direct_declarator(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT:
            parser->_index++;
            ast = new_identifier_ast(AST_IDENT_DECLOR, new_string(token->ident_name));
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
                ast = new_ast(AST_FUNC_DECLOR, 2, ast, parse_parameter_list(parser));
                consume_token(parser, TOKEN_RPALEN);
                break;
            default:
                terminated = 1;
                break;
        }
    }

    return ast;
}

Ast* parse_parameter_list(Parser* parser) {
    Ast* ast = new_ast(AST_PARAM_LIST, 0);

    Token* token = vector_at(parser->_tokens, parser->_index);
    if (token->type == TOKEN_RPALEN) return ast;

    while (1) {
        vector_push(ast->children, parse_parameter_decl(parser));
        token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_RPALEN) break;
        consume_token(parser, TOKEN_COMMA);
    }

    return ast;
}

Ast* parse_parameter_decl(Parser* parser) {
    Ast* specifiers_ast = parse_decl_specifiers(parser);
    Ast* declarator_ast = parse_declarator(parser);
    return new_ast(AST_PARAM_DECL, 2, specifiers_ast, declarator_ast);
}
