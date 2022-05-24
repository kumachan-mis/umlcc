#include "./declaration.h"
#include "../common/common.h"
#include "../ctoken/ctoken.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Ast* parse_decl(Parser* parser) {
    Ast* specifiers_ast = parse_decl_specifiers(parser);
    Ast* init_list_ast = parse_init_declarator_list(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    return new_ast(AST_DECL, 2, specifiers_ast, init_list_ast);
}

Ast* parse_decl_specifiers(Parser* parser) {
    Ast* ast = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_KEYWORD_INT:
            parser->index++;
            ast = new_ast(AST_DECL_SPECIFIERS, 1, new_ast(AST_TYPE_INT, 0));
            return ast;
        default:
            fprintf(stderr, "Error: unexpected ctoken type %d\n", ctoken->type);
            exit(1);
    }
}

Ast* parse_init_declarator_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR_LIST, 0);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_SEMICOLON) return ast;

    while (1) {
        vector_push(ast->children, parse_init_declarator(parser));
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_SEMICOLON) break;
        consume_ctoken(parser, CTOKEN_COMMA);
    }

    return ast;
}

Ast* parse_init_declarator(Parser* parser) {
    Ast* declarator_ast = parse_declarator(parser);
    return new_ast(AST_INIT_DECLOR, 1, declarator_ast);
}

Ast* parse_declarator(Parser* parser) {
    Ast* direct_declarator_ast = parse_direct_declarator(parser);
    return direct_declarator_ast;
}

Ast* parse_direct_declarator(Parser* parser) {
    Ast* ast = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_IDENT:
            parser->index++;
            ast = new_identifier_ast(AST_IDENT_DECLOR, new_string(ctoken->ident_name));
            break;
        default:
            fprintf(stderr, "Error: unexpected ctoken type %d\n", ctoken->type);
            exit(1);
    }

    int terminated = 0;
    while (!terminated) {
        ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LPALEN:
                parser->index++;
                ast = new_ast(AST_FUNC_DECLOR, 2, ast, parse_parameter_list(parser));
                consume_ctoken(parser, CTOKEN_RPALEN);
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

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPALEN) return ast;

    while (1) {
        vector_push(ast->children, parse_parameter_decl(parser));
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RPALEN) break;
        consume_ctoken(parser, CTOKEN_COMMA);
    }

    return ast;
}

Ast* parse_parameter_decl(Parser* parser) {
    Ast* specifiers_ast = parse_decl_specifiers(parser);
    Ast* declarator_ast = parse_declarator(parser);
    return new_ast(AST_PARAM_DECL, 2, specifiers_ast, declarator_ast);
}
