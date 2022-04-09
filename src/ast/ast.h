#ifndef _UMLCC_AST_H_
#define _UMLCC_AST_H_


#include "../vector/vector.h";

typedef enum _AstType {
    AST_INT,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_MOD,
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

#endif
