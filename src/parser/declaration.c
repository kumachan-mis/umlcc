#include "./declaration.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
#include "./expression.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Ast* parse_decl(Parser* parser) {
    parser->typedef_flag = 0;
    Ast* specifiers_ast = parse_decl_specifiers(parser);
    Ast* init_list_ast = parse_init_declarator_list(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    parser->typedef_flag = 0;
    return new_ast(AST_DECL, 2, specifiers_ast, init_list_ast);
}

Ast* parse_decl_specifiers(Parser* parser) {
    Ast* ast = new_ast(AST_DECL_SPECIFIERS, 0);

    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_KEYWORD_TYPEDEF:
                parser->index++;
                parser->typedef_flag = 1;
                vector_push(ast->children, new_ast(AST_STG_TYPEDEF, 0));
                break;
            case CTOKEN_KEYWORD_CHAR:
                parser->index++;
                vector_push(ast->children, new_ast(AST_TYPE_CHAR, 0));
                break;
            case CTOKEN_KEYWORD_INT:
                parser->index++;
                vector_push(ast->children, new_ast(AST_TYPE_INT, 0));
                break;
            case CTOKEN_IDENT: {
                if (!set_contains(parser->typedef_names_set, ctoken->ident_name)) return ast;
                char* typedef_name = new_string(ctoken->ident_name);
                parser->index++;
                vector_push(ast->children, new_identifier_ast(AST_TYPEDEF_NAME, typedef_name));
                break;
            }
            default:
                return ast;
        }
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
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_EQUAL) return new_ast(AST_INIT_DECLOR, 1, declarator_ast);

    parser->index++;
    Ast* initializer_ast = parse_initializer(parser);
    return new_ast(AST_INIT_DECLOR, 2, declarator_ast, initializer_ast);
}

Ast* parse_declarator(Parser* parser) {
    Ast* pointer_ast = parse_pointer(parser);
    Ast* direct_declarator_ast = parse_direct_declarator(parser);
    if (pointer_ast == NULL) return direct_declarator_ast;

    Ast* pointer_tail_ast = pointer_ast;
    while (vector_size(pointer_tail_ast->children) > 0) {
        pointer_tail_ast = vector_at(pointer_tail_ast->children, 0);
    }

    if (direct_declarator_ast->type == AST_IDENT_DECLOR) {
        vector_push(pointer_tail_ast->children, direct_declarator_ast);
        return pointer_ast;
    }

    Ast* func_or_array_tail_ast = direct_declarator_ast;
    while (1) {
        Ast* child_ast = vector_at(func_or_array_tail_ast->children, 0);
        if (child_ast->type == AST_IDENT_DECLOR) break;
        func_or_array_tail_ast = child_ast;
    }

    Ast* ident_ast = ast_copy(vector_at(func_or_array_tail_ast->children, 0));
    vector_set(func_or_array_tail_ast->children, 0, pointer_ast);
    vector_push(pointer_tail_ast->children, ident_ast);
    return direct_declarator_ast;
}

Ast* parse_pointer(Parser* parser) {
    Ast* ast = NULL;
    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_ASTERISK) break;
        parser->index++;
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

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_IDENT:
            if (parser->typedef_flag) {
                set_add(parser->typedef_names_set, new_string(ctoken->ident_name));
            }
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
            case CTOKEN_LBRACKET:
                parser->index++;
                ast = new_ast(AST_ARRAY_DECLOR, 2, ast, parse_assignment_expr(parser));
                consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
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

Ast* parse_initializer(Parser* parser) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_LBRACE) return parse_assignment_expr(parser);
    parser->index++;
    Ast* ast = parse_initializer_list(parser);
    consume_ctoken(parser, CTOKEN_RBRACE);
    return ast;
}

Ast* parse_initializer_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_LIST, 0);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RBRACE) return ast;

    while (1) {
        vector_push(ast->children, parse_initializer(parser));
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
        consume_ctoken(parser, CTOKEN_COMMA);
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
    }

    return ast;
}
