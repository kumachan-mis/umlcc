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
    AST_DECLOR,
    //   type-specification 
    AST_TYPE_INT,

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

    // identifier
    AST_IDENT,

    // constant
    AST_INT,
} AstType;

typedef struct _Ast {
    AstType type;
    char* ident_name;
    int value_int;
    Vector* children;
} Ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_identifier_ast(char* name);
Ast* new_integer_ast(int value);
Ast* copy_ast(Ast* ast);
void delete_ast(Ast* ast);
void ast_append_child(Ast* ast, Ast* child);

#endif
