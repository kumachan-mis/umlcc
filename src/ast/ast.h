#ifndef _UMLCC_AST_H_
#define _UMLCC_AST_H_


#include "../vector/vector.h"

typedef enum _AstType {
    AST_CMPD_STMT,
    AST_EXPR_STMT,
    AST_INT_EXPR,
    AST_ADD_EXPR,
    AST_SUB_EXPR,
    AST_MUL_EXPR,
    AST_DIV_EXPR,
    AST_MOD_EXPR,
} AstType;

typedef struct _Ast {
    AstType type;
    int value_int;
    Vector* children;
} Ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_integer_ast(int value);
Ast* copy_ast(Ast* ast);
void delete_ast(Ast* ast);
void ast_append_child(Ast* ast, Ast* child);

#endif
