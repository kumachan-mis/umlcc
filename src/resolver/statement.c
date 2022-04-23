#include "./statement.h"
#include "./declaration.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_stmt(Resolver* resolver) {
    Ast* ast = resolver->_ast;
    switch (ast->type) {
        case AST_CMPD_STMT:
            return resolve_compound_stmt(resolver);
        case AST_EXPR_STMT:
            return resolve_expression_stmt(resolver);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_compound_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_CMPD_STMT, 0);

    Ast* ast = resolver->_ast;
    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Ast* child = vector_at(ast->children, i);
        resolver->_ast = child;
        if (child->type == AST_DECL) {
            Vector* children = resolve_decl(resolver);
            vector_extend(srt->children, children);
            delete_vector(children, (void (*)(void*))delete_srt);
        } else {
            Srt* child = resolve_stmt(resolver);
            vector_push(srt->children, child);
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
