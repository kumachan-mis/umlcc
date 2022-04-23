#ifndef _UMLCC_AST_H_
#define _UMLCC_AST_H_


#include "../vector/vector.h"

typedef enum _AstType {
    // translation unit
    AST_TRAS_UNIT,

    // function definition
    AST_FUNC_DEF,

    // declaration
    AST_DECL,
    AST_DECL_SPECS,
    AST_INIT_DECLOR,
    //   type-specifiers 
    AST_TYPE_INT,
    //   declarators
    AST_DECLOR,
    AST_IDENT_DIRECT_DECLOR,
    AST_FUNC_DIRECT_DECLOR,

    // statement
    AST_CMPD_STMT,
    AST_EXPR_STMT,

    // expression
    AST_ASSIGN_EXPR,
    AST_ADD_EXPR,
    AST_SUB_EXPR,
    AST_MUL_EXPR,
    AST_DIV_EXPR,
    AST_MOD_EXPR,
    AST_IDENT_EXPR,
    AST_INT_EXPR,
} AstType;

typedef struct _Ast {
    AstType type;
    char* ident_name;
    int value_int;
    Vector* children;
} Ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_identifier_ast(AstType type, char* name);
Ast* new_integer_ast(AstType type, int value);
void delete_ast(Ast* ast);

#endif
