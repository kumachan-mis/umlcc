#include "./statement.h"
#include "./declaration.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_stmt(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->_ast;
    switch (ast->type) {
        case AST_CMPD_STMT:
            resolver->_local_table = symboltable_enter_scope(resolver->_local_table);
            srt = resolve_compound_stmt(resolver);
            resolver->_local_table = symboltable_exit_scope(resolver->_local_table);
            break;
        case AST_EXPR_STMT:
            srt = resolve_expression_stmt(resolver);
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
    return srt;
}

Srt* resolve_compound_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_CMPD_STMT, 0);
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        if (resolver->_ast->type == AST_DECL) {
            vector_push(srt->children, resolve_decl(resolver));
        } else {
            vector_push(srt->children, resolve_stmt(resolver));
        }
    }

    resolver->_ast = ast;
    return srt;
}

Srt* resolve_expression_stmt(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* srt = new_srt(SRT_EXPR_STMT, 1, resolve_expr(resolver));

    resolver->_ast = ast;
    return srt;
}
