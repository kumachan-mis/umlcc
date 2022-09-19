#include "./declaration.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
#include "./expression.h"
#include "./util.h"

#include <stdlib.h>

ParserReturn* parse_decl(Parser* parser) {
    Ast* ast = new_ast(AST_DECL, 0);
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&child, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        parser->typedef_flag = 0;
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    parserret_assign(&child, &err, parse_init_declarator_list(parser));
    if (err != NULL) {
        parser->typedef_flag = 0;
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        parser->typedef_flag = 0;
        delete_ast(ast);
        return new_parserret_error(err);
    }

    parser->typedef_flag = 0;
    return new_parserret(ast);
}

ParserReturn* parse_decl_specifiers(Parser* parser) {
    Ast* ast = new_ast(AST_DECL_SPECIFIERS, 0);
    CToken* ctoken = NULL;
    char* typedef_name = NULL;

    int terminated = 0;
    while (!terminated) {
        ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_KEYWORD_TYPEDEF:
                parser->index++;
                vector_push(ast->children, new_ast(AST_STG_TYPEDEF, 0));
                parser->typedef_flag = 1;
                break;
            case CTOKEN_KEYWORD_CHAR:
                parser->index++;
                vector_push(ast->children, new_ast(AST_TYPE_CHAR, 0));
                break;
            case CTOKEN_KEYWORD_INT:
                parser->index++;
                vector_push(ast->children, new_ast(AST_TYPE_INT, 0));
                break;
            case CTOKEN_IDENT:
                if (!set_contains(parser->typedef_names_set, ctoken->ident_name)) {
                    terminated = 1;
                    break;
                }
                typedef_name = new_string(ctoken->ident_name);
                parser->index++;
                vector_push(ast->children, new_identifier_ast(AST_TYPEDEF_NAME, typedef_name));
                break;
            default:
                terminated = 1;
                break;
        }
    }

    if (vector_size(ast->children) == 0) {
        delete_ast(ast);
        Error* err = new_error("Error: one of declaration-specifiers expected, but got %s\n",
                               ctoken_types[ctoken->type]);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_init_declarator_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR_LIST, 0);
    Ast* child = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_SEMICOLON) return new_parserret(ast);

    while (err == NULL) {
        parserret_assign(&child, &err, parse_init_declarator(parser));
        if (err != NULL) break;
        vector_push(ast->children, child);
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_SEMICOLON) break;
        err = consume_ctoken(parser, CTOKEN_COMMA);
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_init_declarator(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR, 0);
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&child, &err, parse_declarator(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_EQUAL) return new_parserret(ast);

    parser->index++;
    parserret_assign(&child, &err, parse_initializer(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    return new_parserret(ast);
}

ParserReturn* parse_declarator(Parser* parser) {
    Ast* pointer_ast = NULL;
    Ast* direct_declarator_ast = NULL;
    Error* err = NULL;

    parserret_assign(&pointer_ast, &err, parse_pointer(parser));
    if (err != NULL) return new_parserret_error(err);

    parserret_assign(&direct_declarator_ast, &err, parse_direct_declarator(parser));
    if (err != NULL) {
        if (pointer_ast != NULL) delete_ast(pointer_ast);
        return new_parserret_error(err);
    }

    if (pointer_ast == NULL) return new_parserret(direct_declarator_ast);

    Ast* pointer_tail_ast = pointer_ast;
    while (vector_size(pointer_tail_ast->children) > 0) {
        pointer_tail_ast = vector_at(pointer_tail_ast->children, 0);
    }

    if (direct_declarator_ast->type == AST_IDENT_DECLOR) {
        vector_push(pointer_tail_ast->children, direct_declarator_ast);
        return new_parserret(pointer_ast);
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
    return new_parserret(direct_declarator_ast);
}

ParserReturn* parse_pointer(Parser* parser) {
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

    return new_parserret(ast);
}

ParserReturn* parse_direct_declarator(Parser* parser) {
    Ast* ast = NULL;
    Ast* ident_ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_IDENT:
            parser->index++;
            ident_ast = new_identifier_ast(AST_IDENT_DECLOR, new_string(ctoken->ident_name));
            if (!parser->typedef_flag) break;
            set_add(parser->typedef_names_set, new_string(ident_ast->ident_name));
            break;
        default:
            err = new_error("Error: unexpected token %s\n", ctoken_types[ctoken->type]);
            return new_parserret_error(err);
    }

    Ast* child = NULL;
    Ast* tail_ast = NULL;
    while (err == NULL) {
        ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LBRACKET:
                parser->index++;
                parserret_assign(&child, &err, parse_assignment_expr(parser));
                if (err != NULL) break;

                child = new_ast(AST_ARRAY_DECLOR, 1, child);
                if (ast == NULL) ast = child;
                if (tail_ast != NULL) vector_insert(tail_ast->children, 0, child);
                tail_ast = child;
                err = consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
            case CTOKEN_LPALEN:
                parser->index++;
                parserret_assign(&child, &err, parse_parameter_list(parser));
                if (err != NULL) break;

                child = new_ast(AST_FUNC_DECLOR, 1, child);
                if (ast == NULL) ast = child;
                if (tail_ast != NULL) vector_insert(tail_ast->children, 0, child);
                tail_ast = child;
                err = consume_ctoken(parser, CTOKEN_RPALEN);
                break;
            default:
                if (ast == NULL) return new_parserret(ident_ast);
                vector_insert(tail_ast->children, 0, ident_ast);
                return new_parserret(ast);
        }
    }

    delete_ast(ident_ast);
    if (ast != NULL) delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_parameter_list(Parser* parser) {
    Ast* ast = new_ast(AST_PARAM_LIST, 0);
    Ast* child = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPALEN) return new_parserret(ast);

    while (err == NULL) {
        parserret_assign(&child, &err, parse_parameter_decl(parser));
        if (err != NULL) break;
        vector_push(ast->children, child);

        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RPALEN) break;

        err = consume_ctoken(parser, CTOKEN_COMMA);
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_parameter_decl(Parser* parser) {
    Ast* ast = new_ast(AST_PARAM_DECL, 0);
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&child, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    parserret_assign(&child, &err, parse_declarator(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child);

    return new_parserret(ast);
}

ParserReturn* parse_initializer(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_LBRACE) {
        parserret_assign(&ast, &err, parse_assignment_expr(parser));
        if (err != NULL) return new_parserret_error(err);
        return new_parserret(ast);
    }

    parser->index++;
    parserret_assign(&ast, &err, parse_initializer_list(parser));
    if (err != NULL) return new_parserret_error(err);

    err = consume_ctoken(parser, CTOKEN_RBRACE);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_initializer_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_LIST, 0);
    Ast* child = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RBRACE) return new_parserret(ast);

    while (1) {
        parserret_assign(&child, &err, parse_initializer(parser));
        if (err != NULL) break;
        vector_push(ast->children, child);

        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;

        err = consume_ctoken(parser, CTOKEN_COMMA);
        if (err != NULL) break;

        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}
